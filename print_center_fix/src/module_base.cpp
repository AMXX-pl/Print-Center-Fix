// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#include <print_center_fix/module_base.h>
#include <cssdk/public/base_types.h>
#include <metamod/metamod_config.h>
#include <metamod/utils.h>
#include <cstdio>
#include <cstring>

/// <summary>
/// </summary>
uintptr_t AppModule::find_pattern(const char* pattern, const char* mask, const int extra_offset) const
{
	const auto pattern_length = std::strlen(mask);
	const auto pattern_bytes = reinterpret_cast<const byte*>(pattern);

	auto cur_address = reinterpret_cast<const byte*>(module_info_.address) - 1;
	const auto end_address = cur_address + module_info_.size - pattern_length;

	while (++cur_address <= end_address) {
		auto found = true;

		for (size_t i = 0; i < pattern_length; ++i) {
			if (cur_address[i] != pattern_bytes[i] && mask[i] != '?') {
				found = false;
				break;
			}
		}

		if (found)
			return uintptr_t(cur_address) + extra_offset;
	}

	return 0;
}

#ifdef _WIN32
#include <Psapi.h>

/// <summary>
/// </summary>
bool AppModule::get_module_info(const char* name)
{
	MODULEINFO info = {};
	const auto handle = GetModuleHandle(name);

	if (handle == nullptr) {
		MetaUtils::log_console("[%s] Failed to get %s module handle.\n", META_PLUGIN_LOG_TAG, name);
		return false;
	}

	if (!GetModuleInformation(GetCurrentProcess(), handle, &info, sizeof info)) {
		MetaUtils::log_console("[%s] Failed to get %s module info.\n", META_PLUGIN_LOG_TAG, name);
		return false;
	}

	std::snprintf(module_info_.name, sizeof module_info_.name, "%s", name);
	module_info_.address = uintptr_t(info.lpBaseOfDll);
	module_info_.size = info.SizeOfImage;

	return true;
}
#else
#include <link.h>

/// <summary>
/// </summary>
static int find_module(dl_phdr_info* info, std::size_t, void* data)
{
	auto module_info = static_cast<ModuleInfo*>(data);
	
	if (!std::strstr(info->dlpi_name, module_info->name))
		return 0;

	std::snprintf(module_info->name, sizeof module_info->name, "%s", info->dlpi_name);
	module_info->address = info->dlpi_addr;
	module_info->size = 0;

	for (int i = 0; i < info->dlpi_phnum; i++)
		module_info->size += info->dlpi_phdr[i].p_memsz;

	return 1;
}

/// <summary>
/// </summary>
bool AppModule::get_module_info(const char* name)
{
	std::snprintf(module_info_.name, sizeof module_info_.name, "%s", name);
	dl_iterate_phdr(find_module, static_cast<void*>(&module_info_));

	if (!module_info_.address || !module_info_.size) {
		MetaUtils::log_console("[%s] Failed to get %s module info.\n", META_PLUGIN_LOG_TAG, name);
		return false;
	}
	
	return true;
}
#endif
