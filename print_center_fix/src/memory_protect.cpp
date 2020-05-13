// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#include <print_center_fix/memory_protect.h>

#ifndef _WIN32
#include <sys/mman.h>
#include <unistd.h>

/// <summary>
/// </summary>
long MemoryProtect::page_size_ = sysconf(_SC_PAGE_SIZE);

/// <summary>
/// </summary>
long MemoryProtect::page_mask_ = ~(page_size_ - 1);
#endif

/// <summary>
/// </summary>
MemoryProtect::MemoryProtect(void* address, const std::size_t size)
	: MemoryProtect(reinterpret_cast<uintptr_t>(address), size) {}

/// <summary>
/// </summary>
MemoryProtect::MemoryProtect(const uintptr_t address, const std::size_t size)
	: address_(address), size_(size)
{
#ifndef _WIN32
	if (!size_) size_ = page_size_;
#endif
}

/// <summary>
/// </summary>
MemoryProtect::~MemoryProtect()
{
	restore_protection();
}

#ifdef _WIN32
/// <summary>
/// </summary>
bool MemoryProtect::unprotect_memory()
{
	if (!is_protected_)
		return true;

	if (!size_)
		VirtualQuery(LPCVOID(address_), &mem_info_, sizeof mem_info_);
	else
		mem_info_.RegionSize = size_;

	mem_info_.BaseAddress = PVOID(address_);
	is_protected_ = !VirtualProtect(mem_info_.BaseAddress, mem_info_.RegionSize, PAGE_EXECUTE_READWRITE, &mem_info_.Protect);

	return is_protected_;
}

/// <summary>
/// </summary>
bool MemoryProtect::restore_protection()
{
	if (is_protected_)
		return true;

	const auto protect = mem_info_.Protect;
	is_protected_ = VirtualProtect(mem_info_.BaseAddress, mem_info_.RegionSize, protect, &mem_info_.Protect);

	return is_protected_;
}
#else
/// <summary>
/// </summary>
bool MemoryProtect::unprotect_memory()
{
	if (!is_protected_)
		return true;
	
	const auto address = reinterpret_cast<void*>(address_ & page_mask_);
	is_protected_ = mprotect(address, size_, PROT_READ | PROT_WRITE | PROT_EXEC);

	return is_protected_;
}

/// <summary>
/// </summary>
bool MemoryProtect::restore_protection()
{
	if (is_protected_)
		return true;
	
	const auto address = reinterpret_cast<void*>(address_ & page_mask_);
	is_protected_ = !mprotect(address, size_, PROT_READ | PROT_EXEC);

	return is_protected_;
}
#endif
