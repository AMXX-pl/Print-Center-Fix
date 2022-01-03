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

#pragma once

#include <core/strings.h>
#include <cassert>
#include <cstddef>
#include <cstdint>

namespace print_center_fix
{
    class AppMod
    {
        std::string name_{};
        std::uintptr_t address_{};
        std::size_t size_{};

    protected:
        std::string error_{"The application module has not been initialized."};

    public:
        /**
         * @brief Returns \c true if the module address and module size are initialized, \c false otherwise.
        */
        [[nodiscard]] bool Initialized() const
        {
            return address_ > 0 && size_ > 0;
        }

        /**
         * @brief Returns a textual description of the last error.
        */
        [[nodiscard]] std::string GetLastErrorStr() const
        {
            return error_;
        }

        /**
         * @brief The load address of the module.
        */
        [[nodiscard]] std::uintptr_t Address() const
        {
            return address_;
        }

        /**
         * @brief The size of the linear space that the module occupies, in bytes.
        */
        [[nodiscard]] std::size_t Size() const
        {
            return size_;
        }

        /**
         * @brief N/D
        */
        bool Initialize(const std::string& module_name);

        /**
         * @brief N/D
        */
        [[nodiscard]] std::uintptr_t FindPattern(const std::byte* pattern, const char* mask, int extra_offset = 0) const;

        /**
         * @brief N/D
        */
        [[nodiscard]] std::uintptr_t FindPattern(const char* const pattern, const char* const mask, const int extra_offset = 0) const
        {
            assert(!core::str::IsNullOrWhiteSpace(pattern) && !core::str::IsNullOrWhiteSpace(mask));
            return FindPattern(reinterpret_cast<const std::byte*>(pattern), mask, extra_offset);
        }

    protected:
        /**
         * @brief N/D
        */
        class TrampHook* CreateHookOpcodeCall(const char* pattern, const char* mask, int extra_offset, std::uintptr_t tramp);
    };
}
