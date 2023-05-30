#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_input.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_click_disable.h"
#include	"element/elm_stage.h"
#include	"engine/eng_input.h"
#include	"engine/ifc_msg_back.h"
#include	"engine/eng_syscom.h"

static const int TNM_KEY_ACTION_TYPE_DOWN = 0;
static const int TNM_KEY_ACTION_TYPE_UP = 1;
static const int TNM_KEY_ACTION_TYPE_DOWN_UP = 2;

static const int TNM_KEY_CHECK_TYPE_REAL_STATE = 0;
static const int TNM_KEY_CHECK_TYPE_CHECK_STOCK = 1;
static const int TNM_KEY_CHECK_TYPE_USE_STOCK = 2;

// ****************************************************************
// システム的に禁止できるかを判定
// ================================================================
static bool tnm_input_is_disable_ok()
{
	// ＥＸコール中は禁止にならない
	if (Gp_global->ex_call_flag)
		return false;	// disable になれない

	// メッセージバック中は禁止にならない
	if (tnm_msg_back_is_open())
		return false;	// disable になれない

	// メッセージウィンドウを隠している場合は禁止にならない
	if (tnm_syscom_hide_mwnd_get_onoff_flag())
		return false;	// disable になれない

	return true;	// disable になれる
}

// ****************************************************************
// クリック禁止矩形に入っている
// ================================================================
static bool tnm_input_check_left_click_disable_rect()
{
	// 禁止矩形を判定する
	C_point mp = Gp_cur_input->mouse.pos;
	C_rect rect = Gp_click_disable->m_rect;
	if (rect.left <= mp.x && mp.x < rect.right && rect.top <= mp.y && mp.y < rect.bottom)
		return true;	// 禁止されてる

	// オブジェクトのクリック禁止矩形を調べる
	C_elm_object* p_object;
	C_elm_stage* p_front = &(*Gp_stage_list)[TNM_STAGE_FRONT];
	for (int i = 0; i < p_front->m_object_list.get_size(); i++)	{
		p_object = &p_front->m_object_list[i];
		if (p_object->check_click_disable(mp, true))
			return true;	// 禁止されてる
	}
	for (int i = 0; i < p_front->m_mwnd_list.get_size(); i++)	{
		C_elm_mwnd* p_mwnd = &p_front->m_mwnd_list[i];
		for (int j = 0; j < p_mwnd->object_list().get_size(); j++)	{
			p_object = &p_mwnd->object_list()[j];
			if (p_object->check_click_disable(mp, true))
				return true;	// 禁止されてる
		}
	}

	return false;	// 禁止されてない
}
static bool tnm_input_check_right_click_disable_rect()
{
	// 禁止矩形を判定する
	C_point mp = Gp_cur_input->mouse.pos;
	C_rect rect = Gp_click_disable->m_rect;
	if (rect.left <= mp.x && mp.x < rect.right && rect.top <= mp.y && mp.y < rect.bottom)
		return true;	// 禁止されてる

	// オブジェクトのクリック禁止矩形を調べる
	C_elm_object* p_object;
	C_elm_stage* p_front = &(*Gp_stage_list)[TNM_STAGE_FRONT];
	for (int i = 0; i < p_front->m_object_list.get_size(); i++)	{
		p_object = &p_front->m_object_list[i];
		if (p_object->check_click_disable(mp, true))
			return true;	// 禁止されてる
	}
	for (int i = 0; i < p_front->m_mwnd_list.get_size(); i++)	{
		C_elm_mwnd* p_mwnd = &p_front->m_mwnd_list[i];
		for (int j = 0; j < p_mwnd->object_list().get_size(); j++)	{
			p_object = &p_mwnd->object_list()[j];
			if (p_object->check_click_disable(mp, true))
				return true;	// 禁止されてる
		}
	}

	return false;	// 禁止されてない
}

// ****************************************************************
// キーが禁止されている
// ================================================================
static bool tnm_input_check_key_disable(int key)
{
	if (Gp_local->pod.key_disable[key])
		return true;	// 禁止されてる

	return false;	// 禁止されてない
}

// ****************************************************************
// キーの状態を判定
// ================================================================

// ****************************************************************
// 入力判定処理
// ================================================================
static bool tnm_input_ex_key_proc_sub(C_input_state::BUTTON* p_key, int action_type, int check_type)
{
	switch (check_type)	{

		case TNM_KEY_CHECK_TYPE_REAL_STATE:
			switch (action_type)	{
				case TNM_KEY_ACTION_TYPE_DOWN:		return p_key->is_down();
				case TNM_KEY_ACTION_TYPE_UP:		return p_key->is_up();
			}
			break;

		case TNM_KEY_CHECK_TYPE_CHECK_STOCK:
			switch (action_type)	{
				case TNM_KEY_ACTION_TYPE_DOWN:		return p_key->check_down_stock();
				case TNM_KEY_ACTION_TYPE_UP:		return p_key->check_up_stock();
				case TNM_KEY_ACTION_TYPE_DOWN_UP:	return p_key->check_down_up_stock();
			}
			break;

		case TNM_KEY_CHECK_TYPE_USE_STOCK:
			switch (action_type)	{
				case TNM_KEY_ACTION_TYPE_DOWN:		return p_key->use_down_stock();
				case TNM_KEY_ACTION_TYPE_UP:		return p_key->use_up_stock();
				case TNM_KEY_ACTION_TYPE_DOWN_UP:	return p_key->use_down_up_stock();
			}
			break;
	}

	return false;
}

static bool tnm_input_ex_key_proc_decide_key_sub(bool disable_ok, int action_type, int check_type)
{
	// REAL_STATE もしくは CHECK_STOCK の場合は普通に調べる
	if (check_type == TNM_KEY_CHECK_TYPE_REAL_STATE || check_type == TNM_KEY_CHECK_TYPE_CHECK_STOCK)	{
		if (tnm_input_ex_key_proc_sub(&Gp_input->now.mouse.left, action_type, check_type))	{
			if (!(disable_ok && tnm_input_check_left_click_disable_rect()))	{
				return true;
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[VK_RETURN], action_type, check_type))	{
			if (!(disable_ok && tnm_input_check_key_disable(VK_RETURN)))	{
				return true;
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[_T('X')], action_type, check_type))	{
			if (!(disable_ok && tnm_input_check_key_disable(_T('X'))))	{
				return true;
			}
		}
	}

	// USE_STOCK の場合は CHECK_STOCK を先に行い、最後に時間のかかる矩形チェックをした後に USE_CHECK を行う
	// 矩形チェックを最後に持ってきている点が重要です。
	else	{
		if (tnm_input_ex_key_proc_sub(&Gp_input->now.mouse.left, action_type, TNM_KEY_CHECK_TYPE_CHECK_STOCK))	{
			if (!(disable_ok && tnm_input_check_left_click_disable_rect()))	{
				if (tnm_input_ex_key_proc_sub(&Gp_input->now.mouse.left, action_type, TNM_KEY_CHECK_TYPE_USE_STOCK))	{
					return true;
				}
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[VK_RETURN], action_type, TNM_KEY_CHECK_TYPE_CHECK_STOCK))	{
			if (!(disable_ok && tnm_input_check_key_disable(VK_RETURN)))	{
				if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[VK_RETURN], action_type, TNM_KEY_CHECK_TYPE_USE_STOCK))	{
					return true;
				}
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[_T('X')], action_type, TNM_KEY_CHECK_TYPE_CHECK_STOCK))	{
			if (!(disable_ok && tnm_input_check_key_disable(_T('X'))))	{
				if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[_T('X')], action_type, TNM_KEY_CHECK_TYPE_USE_STOCK))	{
					return true;
				}
			}
		}
	}

	return false;
}

static bool tnm_input_ex_key_proc_keyboard_decide_key_sub(bool disable_ok, int action_type, int check_type)
{
	// REAL_STATE もしくは CHECK_STOCK の場合は普通に調べる
	if (check_type == TNM_KEY_CHECK_TYPE_REAL_STATE || check_type == TNM_KEY_CHECK_TYPE_CHECK_STOCK)	{
		if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[VK_RETURN], action_type, check_type))	{
			if (!(disable_ok && tnm_input_check_key_disable(VK_RETURN)))	{
				return true;
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[_T('X')], action_type, check_type))	{
			if (!(disable_ok && tnm_input_check_key_disable(_T('X'))))	{
				return true;
			}
		}
	}

	// USE_STOCK の場合は CHECK_STOCK を先に行い、最後に時間のかかる矩形チェックをした後に USE_CHECK を行う
	// 矩形チェックを最後に持ってきている点が重要です。
	else	{
		if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[VK_RETURN], action_type, TNM_KEY_CHECK_TYPE_CHECK_STOCK))	{
			if (!(disable_ok && tnm_input_check_key_disable(VK_RETURN)))	{
				if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[VK_RETURN], action_type, TNM_KEY_CHECK_TYPE_USE_STOCK))	{
					return true;
				}
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[_T('X')], action_type, TNM_KEY_CHECK_TYPE_CHECK_STOCK))	{
			if (!(disable_ok && tnm_input_check_key_disable(_T('X'))))	{
				if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[_T('X')], action_type, TNM_KEY_CHECK_TYPE_USE_STOCK))	{
					return true;
				}
			}
		}
	}

	return false;
}

static bool tnm_input_ex_key_proc_cancel_key_sub(bool disable_ok, int action_type, int check_type)
{
	// REAL_STATE もしくは CHECK_STOCK の場合は普通に調べる
	if (check_type == TNM_KEY_CHECK_TYPE_REAL_STATE || check_type == TNM_KEY_CHECK_TYPE_CHECK_STOCK)	{
		if (tnm_input_ex_key_proc_sub(&Gp_input->now.mouse.right, action_type, check_type))	{
			if (!(disable_ok && tnm_input_check_right_click_disable_rect()))	{
				return true;
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[VK_ESCAPE], action_type, check_type))	{
			if (!(disable_ok && tnm_input_check_key_disable(VK_ESCAPE)))	{
				return true;
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[_T('Z')], action_type, check_type))	{
			if (!(disable_ok && tnm_input_check_key_disable(_T('Z'))))	{
				return true;
			}
		}
	}

	// USE_STOCK の場合は CHECK_STOCK を先に行い、最後に時間のかかる矩形チェックをした後に USE_CHECK を行う
	// 矩形チェックを最後に持ってきている点が重要です。
	else	{
		if (tnm_input_ex_key_proc_sub(&Gp_input->now.mouse.right, action_type, TNM_KEY_CHECK_TYPE_CHECK_STOCK))	{
			if (!(disable_ok && tnm_input_check_right_click_disable_rect()))	{
				if (tnm_input_ex_key_proc_sub(&Gp_input->now.mouse.right, action_type, TNM_KEY_CHECK_TYPE_USE_STOCK))	{
					return true;
				}
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[VK_ESCAPE], action_type, TNM_KEY_CHECK_TYPE_CHECK_STOCK))	{
			if (!(disable_ok && tnm_input_check_key_disable(VK_ESCAPE)))	{
				if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[VK_ESCAPE], action_type, TNM_KEY_CHECK_TYPE_USE_STOCK))	{
					return true;
				}
			}
		}
		else if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[_T('Z')], action_type, TNM_KEY_CHECK_TYPE_CHECK_STOCK))	{
			if (!(disable_ok && tnm_input_check_key_disable(_T('Z'))))	{
				if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[_T('Z')], action_type, TNM_KEY_CHECK_TYPE_USE_STOCK))	{
					return true;
				}
			}
		}
	}

	return false;
}

static bool tnm_input_ex_key_proc_normal_key_sub(bool disable_ok, int key, int action_type, int check_type)
{
	// REAL_STATE もしくは CHECK_STOCK の場合は普通に調べる
	if (check_type == TNM_KEY_CHECK_TYPE_REAL_STATE || check_type == TNM_KEY_CHECK_TYPE_CHECK_STOCK)	{
		if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[key], action_type, check_type))	{
			if (!(disable_ok && tnm_input_check_key_disable(key)))	{
				return true;
			}
		}
	}

	// USE_STOCK の場合は CHECK_STOCK を先に行い、最後に時間のかかる矩形チェックをした後に USE_CHECK を行う
	// 矩形チェックを最後に持ってきている点が重要です。
	else	{
		if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[key], action_type, TNM_KEY_CHECK_TYPE_CHECK_STOCK))	{
			if (!(disable_ok && tnm_input_check_key_disable(key)))	{
				if (tnm_input_ex_key_proc_sub(&Gp_input->now.keyboard.key[key], action_type, TNM_KEY_CHECK_TYPE_USE_STOCK))	{
					return true;
				}
			}
		}
	}

	return false;
}

static bool tnm_input_ex_key_proc(int ex_key, int action_type, int check_type)
{
	// スクリプトでの禁止不可能が可能かを判定
	bool disable_ok = tnm_input_is_disable_ok();

	// 決定キー
	if (ex_key == VK_EX_DECIDE)	{
		if (tnm_input_ex_key_proc_decide_key_sub(disable_ok, action_type, check_type))	{
			return true;	// 押している
		}
	}
	// キーボードの決定キー
	else if (ex_key == VK_EX_KEYBOARD_DECIDE)	{
		if (tnm_input_ex_key_proc_keyboard_decide_key_sub(disable_ok, action_type, check_type))	{
			return true;	// 押している
		}
	}
	// キャンセルキー
	else if (ex_key == VK_EX_CANCEL)	{
		if (tnm_input_ex_key_proc_cancel_key_sub(disable_ok, action_type, check_type))	{
			return true;	// 押している
		}
	}
	// 普通のキー
	else	{
		if (tnm_input_ex_key_proc_normal_key_sub(disable_ok, ex_key, action_type, check_type))	{
			return true;	// 押している
		}
	}

	return false;
}

// ****************************************************************
// 各種入力判定インターフェイス
// ================================================================
C_point tnm_input_get_mouse_pos()
{
	return Gp_cur_input->mouse.pos;
}

bool tnm_input_is_key_down(int key)
{
	return tnm_input_ex_key_proc(key, TNM_KEY_ACTION_TYPE_DOWN, TNM_KEY_CHECK_TYPE_REAL_STATE);
}
bool tnm_input_is_key_up(int key)
{
	return tnm_input_ex_key_proc(key, TNM_KEY_ACTION_TYPE_UP, TNM_KEY_CHECK_TYPE_REAL_STATE);
}
bool tnm_input_check_key_down(int key)
{
	return tnm_input_ex_key_proc(key, TNM_KEY_ACTION_TYPE_DOWN, TNM_KEY_CHECK_TYPE_CHECK_STOCK);
}
bool tnm_input_check_key_up(int key)
{
	return tnm_input_ex_key_proc(key, TNM_KEY_ACTION_TYPE_UP, TNM_KEY_CHECK_TYPE_CHECK_STOCK);
}
bool tnm_input_check_key_down_up(int key)
{
	return tnm_input_ex_key_proc(key, TNM_KEY_ACTION_TYPE_DOWN_UP, TNM_KEY_CHECK_TYPE_CHECK_STOCK);
}
bool tnm_input_use_key_down(int key)
{
	return tnm_input_ex_key_proc(key, TNM_KEY_ACTION_TYPE_DOWN, TNM_KEY_CHECK_TYPE_USE_STOCK);
}
bool tnm_input_use_key_up(int key)
{
	return tnm_input_ex_key_proc(key, TNM_KEY_ACTION_TYPE_UP, TNM_KEY_CHECK_TYPE_USE_STOCK);
}
bool tnm_input_use_key_down_up(int key)
{
	return tnm_input_ex_key_proc(key, TNM_KEY_ACTION_TYPE_DOWN_UP, TNM_KEY_CHECK_TYPE_USE_STOCK);
}
