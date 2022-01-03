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

#include "tramp_hook.h"
#include <cassert>

namespace print_center_fix
{
    TrampHook::TrampHook(const std::uintptr_t orig_func, const std::uintptr_t tramp_func, const bool enable)
        : orig_func_(orig_func)
    {
        assert(orig_func_ != 0);
        assert(tramp_func != 0);

        funchook_ = {funchook_create(), +[](funchook_t* const fh) {
                         funchook_uninstall(fh, 0);
                         funchook_destroy(fh);
                     }};

        if (funchook_ != nullptr) {
            // NOLINTNEXTLINE(performance-no-int-to-ptr)
            funchook_prepare(funchook_.get(), reinterpret_cast<void**>(&orig_func_), reinterpret_cast<void*>(tramp_func));
        }

        if (enable) {
            Enable();
        }
    }

    bool TrampHook::Enable()
    {
        if (is_enabled_) {
            return true;
        }

        if (funchook_ == nullptr || funchook_install(funchook_.get(), 0)) {
            return false;
        }

        {
            is_enabled_ = true;
        }

        return true;
    }

    bool TrampHook::Disable()
    {
        if (!is_enabled_) {
            return true;
        }

        if (funchook_ == nullptr || funchook_uninstall(funchook_.get(), 0)) {
            return false;
        }

        {
            is_enabled_ = false;
        }

        return true;
    }
}
