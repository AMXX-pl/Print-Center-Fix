// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#include <print_center_fix/main.h>
#include <cssdk/public/utils.h>
#include <metamod/api.h>
#include <metamod/engine.h>
#include <metamod/engine_hooks.h>
#include <print_center_fix/amxx_module.h>
#include <print_center_fix/rehlds_api.h>
#include <print_center_fix/reunion_api.h>
#include <cstring>

/// <summary>
/// </summary>
int g_msgTextMsg = 0;

/// <summary>
/// </summary>
int g_msgSayText = 0;

/// <summary>
/// </summary>
static Hook* hook_client_print = nullptr;

/// <summary>
/// </summary>
static Hook* hook_client_say_text = nullptr;

/// <summary>
/// </summary>
MetamodStatus on_meta_attach()
{
	AmxxModule amxx_module;

	if (!amxx_module.init())
		return MetamodStatus::Failed;

	hook_client_print = amxx_module.hook_client_print(client_print_tramp);
	hook_client_say_text = amxx_module.hook_client_say_text(client_say_text_tramp);

	if (!hook_client_print || !hook_client_say_text) {
		on_meta_detach();
		return MetamodStatus::Failed;
	}

	RehldsApi::init();
	ReunionApi::init();
	EngineHooks::reg_user_message(on_reg_user_message);

	return MetamodStatus::Ok;
}

/// <summary>
/// </summary>
void on_meta_detach()
{
	delete hook_client_print;
	hook_client_print = nullptr;

	delete hook_client_say_text;
	hook_client_say_text = nullptr;
}

/// <summary>
/// </summary>
int on_reg_user_message(const char* name, const int)
{
	const auto ret = meta_result_orig_ret<int>();

	if (!std::strcmp(name, "TextMsg")) {
		g_msgTextMsg = ret;
	}
	else if (!std::strcmp(name, "SayText")) {
		g_msgSayText = ret;
	}

	RETURN_META_VALUE(MetaResult::Ignored, ret);
}

/// <summary>
/// </summary>
static void send_text_message(Edict* client, const int msg_dest, char* msg)
{
	constexpr auto index = 190;
	const auto c = msg[index];
	msg[index] = 0; // Truncate without checking with strlen()

	Engine::message_begin(MessageType::OneUnreliable, g_msgTextMsg, nullptr, client);
	Engine::write_byte(msg_dest);
	Engine::write_string(msg);
	Engine::message_end();

	msg[index] = c;
}

/// <summary>
/// </summary>
void client_print_tramp(Edict* client, const int msg_dest, char* msg)
{
	if (!g_msgTextMsg || ReunionApi::is_steam_client(client)) {
		hook_client_print->call_original<void, Edict*, int, char*>(client, msg_dest, msg);
		return;
	}

	if (client) {
		send_text_message(client, msg_dest, msg);
		return;
	}

	for (auto i = 1; i <= g_global_vars->max_clients; ++i) {
		const auto entity = Engine::entity_of_ent_index(i);

		if (!cssdk_is_valid_entity(entity) || entity->vars.net_name.is_null_or_empty())
			continue;

		if (ReunionApi::is_steam_client(entity))
			hook_client_print->call_original<void, Edict*, int, char*>(entity, msg_dest, msg);
		else
			send_text_message(entity, msg_dest, msg);
	}
}

/// <summary>
/// </summary>
void client_say_text_tramp(Edict* client, const int sender, char* msg)
{
	if (!g_msgSayText || ReunionApi::is_steam_client(client)) {
		hook_client_say_text->call_original<void, Edict*, int, char*>(client, sender, msg);
		return;
	}

	constexpr auto index = 190;
	const auto c = msg[index];
	msg[index] = 0; // Truncate without checking with strlen()

	Engine::message_begin(MessageType::One, g_msgSayText, nullptr, client);
	Engine::write_byte(sender);
	Engine::write_string(msg);
	Engine::message_end();

	msg[index] = c;
}
