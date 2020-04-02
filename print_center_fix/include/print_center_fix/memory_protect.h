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
#endif

/// <summary>
/// Class MemoryProtect.
/// </summary>
class MemoryProtect {
public:
	/// <summary>
	/// </summary>
	explicit MemoryProtect(void* address, std::size_t size = 0);

	/// <summary>
	/// </summary>
	explicit MemoryProtect(uintptr_t address, std::size_t size = 0);

	/// <summary>
	/// </summary>
	~MemoryProtect();

	/// <summary>
	/// </summary>
	bool unprotect_memory();

	/// <summary>
	/// </summary>
	bool restore_protection();

	MemoryProtect(MemoryProtect&&) = delete;
	MemoryProtect(const MemoryProtect&) = delete;
	MemoryProtect& operator=(MemoryProtect&&) = delete;
	MemoryProtect& operator=(const MemoryProtect&) = delete;

private:
	/// <summary>
	/// </summary>
	uintptr_t address_;

	/// <summary>
	/// </summary>
	std::size_t size_;

	/// <summary>
	/// </summary>
	bool is_protected_ = true;

#ifdef _WIN32
	/// <summary>
	/// </summary>
	MEMORY_BASIC_INFORMATION mem_info_{};
#else
	/// <summary>
	/// </summary>
	static long page_size_;

	/// <summary>
	/// </summary>
	static long page_mask_;
#endif
};
