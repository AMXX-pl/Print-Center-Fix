// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#pragma once

#include <cstddef>
#include <cstdint>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
/// <summary>
/// </summary>
constexpr auto MAX_PATH_LENGTH = MAX_PATH;
#else
#include <limits.h>

/// <summary>
/// </summary>
constexpr auto MAX_PATH_LENGTH = PATH_MAX;
#endif

/// <summary>
/// Struct ModuleInfo
/// </summary>
struct ModuleInfo {
	/// <summary>
	/// </summary>
	char name[MAX_PATH_LENGTH]{'\0'};

	/// <summary>
	/// </summary>
	uintptr_t address{};

	/// <summary>
	/// </summary>
	std::size_t size{};
};

/// <summary>
/// Class AppModule.
/// </summary>
class AppModule {
public:
	/// <summary>
	/// </summary>
	AppModule() = default;

	/// <summary>
	/// </summary>
	virtual ~AppModule() = default;

	/// <summary>
	/// </summary>
	virtual bool init() = 0;

	/// <summary>
	/// </summary>
	uintptr_t find_pattern(const char* pattern, const char* mask, int extra_offset = 0) const;

	/// <summary>
	/// </summary>
	[[nodiscard]] const char* name() const
	{
		return module_info_.name;
	}

	/// <summary>
	/// </summary>
	[[nodiscard]] uintptr_t address() const
	{
		return module_info_.address;
	}

	/// <summary>
	/// </summary>
	[[nodiscard]] std::size_t size() const
	{
		return module_info_.size;
	}

	AppModule(AppModule&&) = delete;
	AppModule(const AppModule&) = delete;
	AppModule& operator=(AppModule&&) = delete;
	AppModule& operator=(const AppModule&) = delete;

protected:
	/// <summary>
	/// </summary>
	ModuleInfo module_info_{};

	/// <summary>
	/// </summary>
	bool get_module_info(const char* name);
};
