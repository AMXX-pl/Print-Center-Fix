// ***********************************************************************
// Author           : the_hunter
// Created          : 04-01-2020
//
// Last Modified By : the_hunter
// Last Modified On : 04-01-2020
// ***********************************************************************

#pragma once

struct Edict;

/// <summary>
/// </summary>
extern int g_msgTextMsg;

/// <summary>
/// </summary>
extern int g_msgSayText;

/// <summary>
/// </summary>
int on_reg_user_message(const char* name, int size);

/// <summary>
/// </summary>
void client_print_tramp(Edict* client, int msg_dest, char* msg);

/// <summary>
/// </summary>
void client_say_text_tramp(Edict* client, int sender, char* msg);
