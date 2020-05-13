// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#include <print_center_fix/reunion_api.h>
#include <cssdk/public/utils.h>
#include <metamod/engine.h>
#include <metamod/metamod_config.h>
#include <metamod/utils.h>
#include <print_center_fix/rehlds_api.h>

/// <summary>
/// </summary>
bool ReunionApi::initialized_ = false;

/// <summary>
/// </summary>
int ReunionApi::major_version_ = 0;

/// <summary>
/// </summary>
int ReunionApi::minor_version_ = 0;

/// <summary>
/// </summary>
ReunionApiInterface* ReunionApi::api_ = nullptr;

/// <summary>
/// </summary>
bool ReunionApi::init()
{
	const auto* const rehlds_funcs = RehldsApi::funcs();

	if (!rehlds_funcs)
		return false;

	api_ = static_cast<ReunionApiInterface*>(rehlds_funcs->get_plugin_api("reunion"));

	if (!api_)
		return false;

	major_version_ = api_->get_major_version();
	minor_version_ = api_->get_minor_version();

	if (major_version_ != REUNION_API_VERSION_MAJOR) {
		MetaUtils::log_console("[%s] Reunion API major version mismatch; expected %d, real %d.",
		                       META_PLUGIN_LOG_TAG, REUNION_API_VERSION_MAJOR, major_version_);

		if (major_version_ < REUNION_API_VERSION_MAJOR) {
			MetaUtils::log_console("[%s] Please update the Reunion up to a major version API >= %d.\n",
			                       META_PLUGIN_LOG_TAG, REUNION_API_VERSION_MAJOR);
		}
		else {
			MetaUtils::log_console("[%s] Please update the %s up to a major version API >= %d.\n",
			                       META_PLUGIN_LOG_TAG, META_PLUGIN_NAME, REUNION_API_VERSION_MAJOR);
		}

		return false;
	}

	if (minor_version_ < REUNION_API_VERSION_MINOR) {
		MetaUtils::log_console("[%s] Reunion API minor version mismatch; expected %d, real %d.",
		                       META_PLUGIN_LOG_TAG, REUNION_API_VERSION_MINOR, minor_version_);

		MetaUtils::log_console("[%s] Please update the Reunion up to a minor version API >= %d.\n",
		                       META_PLUGIN_LOG_TAG, REUNION_API_VERSION_MINOR);

		return false;
	}

	initialized_ = true;
	return initialized_;
}

/// <summary>
/// </summary>
bool ReunionApi::is_steam_client(const Edict* client)
{
	if (!initialized_ || !cssdk_is_valid_entity(client))
		return false;

	const auto index = Engine::index_of_edict(client);
	if (!index) return false;

	return get_client_auth_type(index - 1) == DpAuthKind::Steam;
}
