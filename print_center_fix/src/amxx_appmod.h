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

#include "appmod.h"

namespace cssdk
{
    struct Edict;
}

namespace print_center_fix
{
    class AmxxAppMod final : public AppMod
    {
    public:
        using ClientPrintFunc = void (*)(cssdk::Edict* client, int dest, char* message);
        using ClientSayTextFunc = void (*)(cssdk::Edict* client, int sender, char* message);

        /**
         * @brief N/D 
         *
         * @return \c true if successful, \c false otherwise.
        */
        bool Initialize()
        {
            return AppMod::Initialize("amxmodx_mm");
        }

        /**
         * @brief Hooks the UTIL_ClientPrint function.
        */
        [[nodiscard]] TrampHook* HookClientPrint(ClientPrintFunc tramp);

        /**
         * @brief Hooks the UTIL_ClientSayText function.
        */
        [[nodiscard]] TrampHook* HookClientSayText(ClientSayTextFunc tramp);
    };
}
