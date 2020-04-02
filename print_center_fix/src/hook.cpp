// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#include <print_center_fix/hook.h>
#include <cstring>

/// <summary>
/// </summary>
Hook::Hook(const uintptr_t original_func, const uintptr_t trampoline)
	: original_func_(reinterpret_cast<void*>(original_func))
{
	memory_protect_ = new MemoryProtect(original_func_, patch_size_);
	memory_protect_->unprotect_memory();

	patch_bytes_[0] = 0xE9;
	std::memcpy(static_cast<void*>(original_bytes_), original_func_, sizeof original_bytes_);
	*reinterpret_cast<uintptr_t*>(&patch_bytes_[1]) = trampoline - (original_func + patch_size_);
}

#ifdef __INTEL_COMPILER
	#pragma warning(push)
	#pragma warning(disable: 2017)
#endif

/// <summary>
/// </summary>
Hook::~Hook()
{
	disable();
	delete memory_protect_;
}

#ifdef __INTEL_COMPILER
	#pragma warning(pop)
#endif

/// <summary>
/// </summary>
void Hook::enable()
{
	if (is_enabled_)
		return;

	std::memcpy(original_func_, patch_bytes_, sizeof patch_bytes_);
	is_enabled_ = true;
}

/// <summary>
/// </summary>
void Hook::disable()
{
	if (!is_enabled_)
		return;

	std::memcpy(original_func_, original_bytes_, sizeof original_bytes_);
	is_enabled_ = false;
}
