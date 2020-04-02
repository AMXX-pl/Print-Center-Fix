// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#pragma once

#include <print_center_fix/hook.h>
#include <print_center_fix/module_base.h>
#include <type_traits>

struct Edict;
using client_print_func = std::add_pointer_t<void(Edict*, int, char*)>;
using client_say_text_func = std::add_pointer_t<void(Edict*, int, char*)>;

/// <summary>
/// </summary>
class AmxxModule final : public AppModule {
public:
	/// <summary>
	/// </summary>
	bool init() override;

	/// <summary>
	/// </summary>
	Hook* hook_client_print(client_print_func trampoline, bool enable = true) const;

	/// <summary>
	/// </summary>
	Hook* hook_client_say_text(client_say_text_func trampoline, bool enable = true) const;
};
