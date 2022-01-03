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

#include "amxx_appmod.h"
#include "tramp_hook.h"
#include <core/console.h>
#include <core/messages.h>
#include <core/reunion_api.h>
#include <cssdk/public/utils.h>
#include <metamod/api.h>
#include <metamod/engine.h>

using namespace core;
using namespace cssdk;
using namespace metamod;
using namespace print_center_fix;

namespace
{
    bool g_reunion{};
    TrampHook* g_client_print_hook{};
    TrampHook* g_client_say_text_hook{};

    bool IsUserSteam(const Edict* const client)
    {
        if (g_reunion && IsValidEntity(client)) {
            if (const auto index = engine::IndexOfEdict(client); IsClient(index)) {
                return reunion_api::IsUserSteam(index - 1);
            }
        }

        return false;
    }

    void OnClientPrint(Edict* const client, const int dest, char* const message)
    {
        if (IsUserSteam(client)) {
            g_client_print_hook->CallOriginal(client, dest, message);
            return;
        }

        if (client) {
            if (IsValidEntity(client)) {
                messages::SendTextMessage(client, static_cast<HudPrint>(dest), message);
            }

            return;
        }

        for (auto i = 1; i <= g_global_vars->max_clients; ++i) {
            if (auto* const entity = engine::EntityOfEntIndex(i); IsValidEntity(entity)) {
                if (const auto* const player = EntityPrivateData<PlayerBase>(entity); !player->has_disconnected) {
                    if (IsUserSteam(entity)) {
                        g_client_print_hook->CallOriginal(entity, dest, message);
                    }
                    else {
                        messages::SendTextMessage(entity, static_cast<HudPrint>(dest), message);
                    }
                }
            }
        }
    }

    void OnClientSayText(Edict* const client, const int sender, char* const message)
    {
        if (!IsValidEntity(client)) {
            return;
        }

        if (IsUserSteam(client)) {
            g_client_say_text_hook->CallOriginal(client, sender, message);
        }
        else {
            messages::SendChatMessage(client, sender, message);
        }
    }

    bool Initialize(AmxxAppMod& amxx_module)
    {
        return amxx_module.Initialize() &&
            ((g_client_print_hook = amxx_module.HookClientPrint(OnClientPrint))) &&
            ((g_client_say_text_hook = amxx_module.HookClientSayText(OnClientSayText)));
    }

    void CleanUp()
    {
        delete g_client_print_hook;
        g_client_print_hook = nullptr;

        delete g_client_say_text_hook;
        g_client_say_text_hook = nullptr;
    }
}

MetaStatus OnMetaAttach()
{
    g_reunion = reunion_api::Init(true);

    if (AmxxAppMod amxx_module{}; !Initialize(amxx_module)) {
        console::Error("%s", amxx_module.GetLastErrorStr());
        CleanUp();

        return Status::Failed;
    }

    return Status::Ok;
}

void OnMetaDetach()
{
    CleanUp();
}
