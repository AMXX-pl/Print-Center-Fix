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

#include <funchook.h>
#include <cstdint>
#include <memory>
#include <string>

namespace print_center_fix
{
    class TrampHook final
    {
        bool is_enabled_{};
        std::uintptr_t orig_func_;

        using funchook_deleter = void (*)(funchook_t*);
        std::unique_ptr<funchook_t, funchook_deleter> funchook_{nullptr, nullptr};

    public:
        /**
         * @brief Constructor.
         *
         * @param orig_func The address of the original function.
         * @param tramp_func The address of the hook function.
         * @param enable Should a hook be enabled?
        */
        TrampHook(std::uintptr_t orig_func, std::uintptr_t tramp_func, bool enable = true);

        /**
         * @brief Returns a textual description of the last error.
        */
        [[nodiscard]] std::string GetLastErrorStr() const
        {
            return std::string{funchook_ ? "Out of memory." : funchook_error_message(funchook_.get())};
        }

        /**
         * @brief N/D
        */
        [[nodiscard]] bool IsEnabled() const
        {
            return is_enabled_;
        }

        /**
         * @brief N/D
        */
        bool Enable();

        /**
         * @brief N/D
        */
        bool Disable();

        /**
         * @brief N/D
        */
        template <typename Ret = void, typename... Args>
        Ret CallOriginal(Args&&... args)
        {
            // NOLINTNEXTLINE(performance-no-int-to-ptr)
            const auto orig_func = reinterpret_cast<Ret (*)(Args...)>(orig_func_); // cppcheck-suppress unreadVariable
            return orig_func(std::forward<Args>(args)...);
        }
    };
}
