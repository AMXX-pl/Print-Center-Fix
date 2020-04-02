// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#pragma once

#include <cssdk/public/base_types.h>
#include <print_center_fix/memory_protect.h>
#include <cstddef>
#include <cstdint>
#include <utility>

/// <summary>
/// Class Hook.
/// </summary>
class Hook {
	/// <summary>
	/// </summary>
	static constexpr std::size_t patch_size_ = 5;

public:
	/// <summary>
	/// </summary>
	Hook(uintptr_t original_func, uintptr_t trampoline);

	/// <summary>
	/// </summary>
	~Hook();

	Hook(Hook&&) = delete;
	Hook(const Hook&) = delete;
	Hook& operator=(Hook&&) = delete;
	Hook& operator=(const Hook&) = delete;

	/// <summary>
	/// </summary>
	void enable();

	/// <summary>
	/// </summary>
	void disable();

	/// <summary>
	/// </summary>
	template <typename TRet, typename ...TArgs>
	TRet call_original(TArgs ...args);

	/// <summary>
	/// </summary>
	[[nodiscard]] inline bool is_enabled() const;

private:
	/// <summary>
	/// </summary>
	bool is_enabled_ = false;

	/// <summary>
	/// </summary>
	void* original_func_ = nullptr;

	/// <summary>
	/// </summary>
	byte original_bytes_[patch_size_] = {};

	/// <summary>
	/// </summary>
	byte patch_bytes_[patch_size_] = {};

	/// <summary>
	/// </summary>
	MemoryProtect* memory_protect_ = nullptr;

	/// <summary>
	/// Class HookDisabler.
	/// </summary>
	class HookDisabler {
	public:
		/// <summary>
		/// </summary>
		explicit HookDisabler(Hook& hook) : hook_(hook), should_enable_(hook.is_enabled())
		{
			hook_.disable();
		}

		/// <summary>
		/// </summary>
		~HookDisabler()
		{
			if (should_enable_)
				hook_.enable();
		}

		HookDisabler(HookDisabler&&) = delete;
		HookDisabler(const HookDisabler&) = delete;
		HookDisabler& operator=(HookDisabler&&) = delete;
		HookDisabler& operator=(const HookDisabler&) = delete;

	private:
		/// <summary>
		/// </summary>
		Hook& hook_;

		/// <summary>
		/// </summary>
		bool should_enable_;
	};
};

bool Hook::is_enabled() const
{
	return is_enabled_;
}

template <typename TRet, typename ...TArgs>
TRet Hook::call_original(TArgs ...args)
{
	HookDisabler disable(*this);
	return (reinterpret_cast<TRet(*)(TArgs ...)>(original_func_))(std::forward<TArgs>(args)...);
}
