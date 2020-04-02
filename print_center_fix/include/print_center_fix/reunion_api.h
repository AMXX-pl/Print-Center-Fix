// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#pragma once

#include <cssdk/engine/userid_rehlds.h>
#include <cstddef>
#include <cstdint>

struct Edict;

/// <summary>
/// </summary>
constexpr auto REUNION_API_VERSION_MAJOR = 1;

/// <summary>
/// </summary>
constexpr auto REUNION_API_VERSION_MINOR = 4;

/// <summary>
/// </summary>
constexpr auto LONG_AUTHID_LEN = 16;

/// <summary>
/// Enum DpAuthKind
/// </summary>
enum class DpAuthKind {
	/// <summary>
	/// </summary>
	None = 0,

	/// <summary>
	/// </summary>
	DProto = 1,

	/// <summary>
	/// </summary>
	Steam = 2,

	/// <summary>
	/// </summary>
	SteamEmu = 3,

	/// <summary>
	/// </summary>
	RevEmu = 4,

	/// <summary>
	/// </summary>
	OldRevEmu = 5,

	/// <summary>
	/// </summary>
	Hltv = 6,

	/// <summary>
	/// </summary>
	Sc2009 = 7,

	/// <summary>
	/// </summary>
	Avsmp = 8,

	/// <summary>
	/// </summary>
	Sxei = 9,

	/// <summary>
	/// </summary>
	RevEmu2013 = 10,

	/// <summary>
	/// </summary>
	Sse3 = 11
};

/// <summary>
/// Enum ReuAuthKind
/// </summary>
enum class ReuAuthKind {
	/// <summary>
	/// </summary>
	Unknown = -1,

	/// <summary>
	/// </summary>
	Steam,

	/// <summary>
	/// </summary>
	VolumeId,

	/// <summary>
	/// </summary>
	HddSn,

	/// <summary>
	/// </summary>
	FileId,

	/// <summary>
	/// </summary>
	SxeId,

	/// <summary>
	/// </summary>
	Other,

	/// <summary>
	/// </summary>
	Max
};

/// <summary>
/// Class ReunionApiInterface.
/// </summary>
class ReunionApiInterface {
public:
	/// <summary>
	/// <para>index: 0-31</para>
	/// </summary>
	virtual int get_client_protocol(int index) = 0;

	/// <summary>
	/// <para>index: 0-31</para>
	/// </summary>
	virtual DpAuthKind get_client_auth_type(int index) = 0;

	/// <summary>
	/// <para>Get auth data as binary. index: 0-31</para>
	/// </summary>
	virtual std::size_t get_client_auth_data(int index, void* data, int max_len) = 0;

	/// <summary>
	/// <para>Get auth data as string. index: 0-31</para>
	/// </summary>
	virtual const char* get_client_auth_data_string(int index, char* data, int max_len) = 0;

	/// <summary>
	/// </summary>
	virtual int get_major_version() = 0;

	/// <summary>
	/// </summary>
	virtual int get_minor_version() = 0;

	/// <summary>
	/// </summary>
	virtual void get_long_auth_id(int index, unsigned char (&auth_id)[LONG_AUTHID_LEN]) = 0;

	/// <summary>
	/// </summary>
	virtual ReuAuthKind get_auth_key_kind(int index) = 0;

	/// <summary>
	/// <para>index: 0-31</para>
	/// </summary>
	virtual void set_connect_time(int index, double time) = 0;

	/// <summary>
	/// <para>index: 0-31</para>
	/// </summary>
	[[nodiscard]] virtual UserId* get_serialized_id(int index) const = 0;

	/// <summary>
	/// <para>index: 0-31</para>
	/// </summary>
	[[nodiscard]] virtual UserId* get_storage_id(int index) const = 0;

	/// <summary>
	/// <para>index: 0-31</para>
	/// </summary>
	[[nodiscard]] virtual uint64_t get_display_steam_id(int index) const = 0;
};

/// <summary>
/// Class ReunionApi.
/// </summary>
class ReunionApi {
public:
	/// <summary>
	/// </summary>
	static bool init();

	/// <summary>
	/// </summary>
	static bool initialized()
	{
		return initialized_;
	}

	/// <summary>
	/// <para>index: 0-31</para>
	/// </summary>
	static int get_client_protocol(const int index)
	{
		return api_->get_client_protocol(index);
	}

	/// <summary>
	/// <para>index: 0-31</para>
	/// </summary>
	static DpAuthKind get_client_auth_type(const int index)
	{
		return api_->get_client_auth_type(index);
	}

	/// <summary>
	/// <para>Get auth data as binary. index: 0-31</para>
	/// </summary>
	[[deprecated]] static std::size_t get_client_auth_data(const int index, void* data, const int max_len)
	{
		return api_->get_client_auth_data(index, data, max_len);
	}

	/// <summary>
	/// <para>Get auth data as string. index: 0-31</para>
	/// </summary>
	[[deprecated]] static const char* get_client_auth_data_string(const int index, char* data, const int max_len)
	{
		return api_->get_client_auth_data_string(index, data, max_len);
	}

	/// <summary>
	/// </summary>
	static int get_major_version()
	{
		return major_version_;
	}

	/// <summary>
	/// </summary>
	static int get_minor_version()
	{
		return minor_version_;
	}

	/// <summary>
	/// </summary>
	static void get_long_auth_id(const int index, unsigned char (&auth_id)[LONG_AUTHID_LEN])
	{
		api_->get_long_auth_id(index, auth_id);
	}

	/// <summary>
	/// </summary>
	static ReuAuthKind get_auth_key_kind(const int index)
	{
		return api_->get_auth_key_kind(index);
	}

	/// <summary>
	/// <para>index: 0-31</para>
	/// </summary>
	static void set_connect_time(const int index, const double time)
	{
		api_->set_connect_time(index, time);
	}

	/// <summary>
	/// <para>index: 0-31</para>
	/// </summary>
	[[nodiscard]] static UserId* get_serialized_id(const int index)
	{
		return api_->get_serialized_id(index);
	}

	/// <summary>
	/// <para>index: 0-31</para>
	/// </summary>
	[[nodiscard]] static UserId* get_storage_id(const int index)
	{
		return api_->get_serialized_id(index);
	}

	/// <summary>
	/// <para>index: 0-31</para>
	/// </summary>
	[[nodiscard]] static uint64_t get_display_steam_id(const int index)
	{
		return api_->get_display_steam_id(index);
	}

	/// <summary>
	/// </summary>
	static bool is_steam_client(const Edict* client);

private:
	/// <summary>
	/// </summary>
	static bool initialized_;

	/// <summary>
	/// </summary>
	static int major_version_;

	/// <summary>
	/// </summary>
	static int minor_version_;

	/// <summary>
	/// </summary>
	static ReunionApiInterface* api_;
};
