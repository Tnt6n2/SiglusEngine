#include	"pch.h"
#include	"data/tnm_input.h"

// ****************************************************************
// インプットのベース
// ================================================================
void C_tnm_input_base::clear_ex_key(int ex_key)
{
	switch (ex_key)	{
		case VK_EX_DECIDE:	now.mouse.left.clear();		now.keyboard.key[VK_RETURN].clear();	now.keyboard.key[_T('X')].clear();	break;
		case VK_EX_CANCEL:	now.mouse.right.clear();	now.keyboard.key[VK_ESCAPE].clear();	now.keyboard.key[_T('Z')].clear();	break;
	}
}

bool C_tnm_input_base::is_ex_key_down(int ex_key)
{
	switch (ex_key)	{
		case VK_EX_DECIDE:	return now.mouse.left.is_down() || now.keyboard.key[VK_RETURN].is_down() || now.keyboard.key[_T('X')].is_down();
		case VK_EX_CANCEL:	return now.mouse.right.is_down() || now.keyboard.key[VK_ESCAPE].is_down() || now.keyboard.key[_T('Z')].is_down();
	}

	return false;
}

bool C_tnm_input_base::is_ex_key_up(int ex_key)
{
	switch (ex_key)	{
		case VK_EX_DECIDE:	return now.mouse.left.is_up() && now.keyboard.key[VK_RETURN].is_up() && now.keyboard.key[_T('X')].is_up();
		case VK_EX_CANCEL:	return now.mouse.right.is_up() && now.keyboard.key[VK_ESCAPE].is_up() && now.keyboard.key[_T('Z')].is_up();
	}

	return false;
}

bool C_tnm_input_base::check_ex_key_down(int ex_key)
{
	switch (ex_key)	{
		case VK_EX_DECIDE:	return now.mouse.left.check_down_stock() || now.keyboard.key[VK_RETURN].check_down_stock() || now.keyboard.key[_T('X')].check_down_stock();
		case VK_EX_CANCEL:	return now.mouse.right.check_down_stock() || now.keyboard.key[VK_ESCAPE].check_down_stock() || now.keyboard.key[_T('Z')].check_down_stock();
	}

	return false;
}

bool C_tnm_input_base::check_ex_key_up(int ex_key)
{
	switch (ex_key)	{
		case VK_EX_DECIDE:	return now.mouse.left.check_up_stock() || now.keyboard.key[VK_RETURN].check_up_stock() || now.keyboard.key[_T('X')].check_up_stock();
		case VK_EX_CANCEL:	return now.mouse.right.check_up_stock() || now.keyboard.key[VK_ESCAPE].check_up_stock() || now.keyboard.key[_T('Z')].check_up_stock();
	}

	return false;
}

bool C_tnm_input_base::check_ex_key_down_up(int ex_key)
{
	switch (ex_key)	{
		case VK_EX_DECIDE:	return now.mouse.left.check_down_up_stock() || now.keyboard.key[VK_RETURN].check_down_up_stock() || now.keyboard.key[_T('X')].check_down_up_stock();
		case VK_EX_CANCEL:	return now.mouse.right.check_down_up_stock() || now.keyboard.key[VK_ESCAPE].check_down_up_stock() || now.keyboard.key[_T('Z')].check_down_up_stock();
	}

	return false;
}

bool C_tnm_input_base::use_ex_key_down(int ex_key)
{
	switch (ex_key)	{
		case VK_EX_DECIDE:	return now.mouse.left.use_down_stock() || now.keyboard.key[VK_RETURN].use_down_stock() || now.keyboard.key[_T('X')].use_down_stock();
		case VK_EX_CANCEL:	return now.mouse.right.use_down_stock() || now.keyboard.key[VK_ESCAPE].use_down_stock() || now.keyboard.key[_T('Z')].use_down_stock();
	}

	return false;
}

bool C_tnm_input_base::use_ex_key_up(int ex_key)
{
	switch (ex_key)	{
		case VK_EX_DECIDE:	return now.mouse.left.use_up_stock() || now.keyboard.key[VK_RETURN].use_up_stock() || now.keyboard.key[_T('X')].use_up_stock();
		case VK_EX_CANCEL:	return now.mouse.right.use_up_stock() || now.keyboard.key[VK_ESCAPE].use_up_stock() || now.keyboard.key[_T('Z')].use_up_stock();
	}

	return false;
}

bool C_tnm_input_base::use_ex_key_down_up(int ex_key)
{
	switch (ex_key)	{
		case VK_EX_DECIDE:	return now.mouse.left.use_down_up_stock() || now.keyboard.key[VK_RETURN].use_down_up_stock() || now.keyboard.key[_T('X')].use_down_up_stock();
		case VK_EX_CANCEL:	return now.mouse.right.use_down_up_stock() || now.keyboard.key[VK_ESCAPE].use_down_up_stock() || now.keyboard.key[_T('Z')].use_down_up_stock();
	}

	return false;
}

// ****************************************************************
// スクリプトインプット
// ================================================================
void C_tnm_script_input::frame()
{
	C_input_base::frame();
}
