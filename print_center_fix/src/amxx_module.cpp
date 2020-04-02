// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#include <print_center_fix/amxx_module.h>
#include <metamod/metamod_config.h>
#include <metamod/utils.h>
#include <print_center_fix/hook.h>

/// <summary>
/// </summary>
bool AmxxModule::init()
{
	return get_module_info("amxmodx_mm");
}

/// <summary>
/// </summary>
Hook* AmxxModule::hook_client_print(const client_print_func trampoline, const bool enable) const
{
#ifdef _WIN32
	const auto mask = "xxxxxx????x";
	const auto pattern = "\xFF\x73\x08\xFF\x37\xE8\x00\x00\x00\x00\x83";
	const auto call_opcode_address = find_pattern(pattern, mask, 0x5);
#else
	const auto mask = "x????xxxxxxx";
	const auto pattern = "\xE8\x00\x00\x00\x00\x8B\x44\x24\x1C\x83\xC4\x20";
	const auto call_opcode_address = find_pattern(pattern, mask, 0x0);
#endif

	if (!call_opcode_address) {
		MetaUtils::log_console("[%s] Could not find UTIL_ClientPrint function.", META_PLUGIN_LOG_TAG);
		return nullptr;
	}

	const auto relative_call_address = *reinterpret_cast<uintptr_t*>(call_opcode_address + 0x1);
	const auto client_print_address = call_opcode_address + 0x1 + relative_call_address + sizeof(uintptr_t);

	auto hook = new Hook(client_print_address, reinterpret_cast<uintptr_t>(trampoline));
	if (enable) hook->enable();

	return hook;
}

Hook* AmxxModule::hook_client_say_text(const client_say_text_func trampoline, const bool enable) const
{
#ifdef _WIN32
	const auto mask = "xxxx????xxx";
	const auto pattern = "\x56\xFF\x30\xE8\x00\x00\x00\x00\x83\xC4\x0C";
	const auto call_opcode_address = find_pattern(pattern, mask, 0x3);
#else
	const auto mask = "xxxxxxxxxxxxx";
	const auto pattern = "\xCA\x89\x74\x24\x08\x89\x4C\x24\x04\x89\x04\x24\xE8";
	const auto call_opcode_address = find_pattern(pattern, mask, 0xC);
#endif

	if (!call_opcode_address) {
		MetaUtils::log_console("[%s] Could not find UTIL_ClientSayText function.", META_PLUGIN_LOG_TAG);
		return nullptr;
	}

	const auto relative_call_address = *reinterpret_cast<uintptr_t*>(call_opcode_address + 0x1);
	const auto client_say_text_address = call_opcode_address + 0x1 + relative_call_address + sizeof(uintptr_t);

	auto hook = new Hook(client_say_text_address, reinterpret_cast<uintptr_t>(trampoline));
	if (enable) hook->enable();

	return hook;
}
