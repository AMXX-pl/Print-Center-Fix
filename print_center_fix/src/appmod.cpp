/*
 *  Copyright (c) 2021 the_hunter
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "appmod.h"
#include "tramp_hook.h"
#include <core/strings.h>
#include <core/system.h>

#ifdef _WIN32
#include <Psapi.h>
#else
#include <link.h>
#endif

using namespace core;
using namespace print_center_fix;

namespace
{
    constexpr auto* PATTERN_MATCH_NOT_FOUND = "Pattern match not found.";

#ifdef _WIN32
    HANDLE g_current_process{GetCurrentProcess()};

    bool FormatInitializeError(const bool ret_val, const std::string& name, std::string& error)
    {
        if (ret_val) {
            error = system::StrError(GetLastError());
        }
        else {
            error = str::Format("Failed to initialize \"%s\" application module: %s",
                                name, system::StrError(GetLastError()));
        }

        return ret_val;
    }
#else
    struct ModuleData
    {
        const std::string& name;
        std::uintptr_t& address;
        std::size_t& size;
    };

    int FindModule(dl_phdr_info* info, const std::size_t, void* const data)
    {
        const auto* const module_info = static_cast<ModuleData*>(data);
        const auto object_name = std::string{str::GetFileNameFromPath(info->dlpi_name)};

        if (!str::IStartsWith(object_name.c_str(), module_info->name.c_str())) {
            return 0;
        }

        module_info->address = info->dlpi_addr;
        module_info->size = 0;

        for (ElfW(Half) i = 0; i < info->dlpi_phnum; ++i) {
            module_info->size += static_cast<std::size_t>(info->dlpi_phdr[i].p_memsz);
        }

        return 1;
    }
#endif
}

namespace print_center_fix
{
#ifdef _WIN32
    bool AppMod::Initialize(const std::string& module_name)
    {
        assert(g_current_process != nullptr);
        assert(!str::IsEmptyOrWhiteSpace(module_name));

        name_ = module_name;
        auto* const module = GetModuleHandle(name_.c_str());

        if (module == nullptr) {
            return FormatInitializeError(false, name_, error_);
        }

        if (MODULEINFO info{}; GetModuleInformation(g_current_process, module, &info, sizeof(MODULEINFO))) {
            address_ = reinterpret_cast<std::uintptr_t>(info.lpBaseOfDll);
            size_ = static_cast<std::size_t>(info.SizeOfImage);
            return FormatInitializeError(true, name_, error_);
        }

        return FormatInitializeError(false, name_, error_);
    }
#else
    bool AppMod::Initialize(const std::string& module_name)
    {
        assert(!str::IsEmptyOrWhiteSpace(module_name));

        name_ = module_name;
        ModuleData data{name_, address_, size_};
        dl_iterate_phdr(FindModule, static_cast<void*>(&data));

        if (!Address() || !Size()) {
            error_ = str::Format("Failed to initialize \"%s\" application module: Not found.", name_);
            return false;
        }

        return true;
    }
#endif

    std::uintptr_t AppMod::FindPattern(const std::byte* const pattern, const char* const mask, const int extra_offset) const
    {
        assert(pattern != nullptr && !str::IsNullOrWhiteSpace(mask));
        assert(Address() > 0 && Size() > 0);

        const auto pattern_length = std::strlen(mask); // NOLINTNEXTLINE(performance-no-int-to-ptr)
        const auto* cur_address = reinterpret_cast<const std::byte*>(Address()) - 1;
        const auto* const end_address = cur_address + Size() - pattern_length + 1;

        while (++cur_address <= end_address) {
            auto found = true;

            for (std::string::size_type i = 0; i < pattern_length; ++i) {
                if (cur_address[i] != pattern[i] && mask[i] != '?') {
                    found = false;
                    break;
                }
            }

            if (found) {
                return reinterpret_cast<std::uintptr_t>(cur_address) + extra_offset;
            }
        }

        return std::uintptr_t{0};
    }

    TrampHook* AppMod::CreateHookOpcodeCall(const char* const pattern, const char* const mask,
                                            const int extra_offset, const std::uintptr_t tramp)
    {
        if (!Initialized()) {
            return nullptr;
        }

        assert(tramp != 0);
        const auto opcode_call = FindPattern(pattern, mask, extra_offset);

        if (!opcode_call) {
            error_ = PATTERN_MATCH_NOT_FOUND;
            return nullptr;
        }

        // NOLINTNEXTLINE(performance-no-int-to-ptr)
        const auto relative_address = *reinterpret_cast<std::uintptr_t*>(opcode_call + 0x1);
        const auto absolute_address = opcode_call + 0x1 + relative_address + sizeof(std::uintptr_t);

        // NOLINTNEXTLINE(performance-no-int-to-ptr)
        auto* const hook = new TrampHook{absolute_address, tramp, true};

        if (hook->IsEnabled()) {
            return hook;
        }

        error_ = hook->GetLastErrorStr();
        delete hook;

        return nullptr;
    }
}
