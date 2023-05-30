#include	"pch.h"
#include	"ifc_eng.h"
#include	"tnm_element_code.h"
#include	"data/tnm_def.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_btn_template.h"
#include	"element/elm_object.h"
#include	"element/elm_group.h"
#include	"element/elm_call.h"
#include	"engine/eng_config.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_syscom.h"
#include	"engine/flow_command.h"
#include	"engine/ifc_sound.h"

// ****************************************************************
// オブジェクト：ボタンパラメータの初期化
// ================================================================
void C_elm_object::init_button_param()
{
	m_op.button.sys_type = 0;
	m_op.button.sys_type_opt = 0;
	m_op.button.button_no = 0;
	m_op.button.elm_group.clear();
	m_op.button.action_no = -1;
	m_op.button.se_no = -1;
	m_op.button.push_keep = 0;
	m_op.button.state = TNM_BTN_STATE_NORMAL;
	m_op.button.mode = 0;
	m_op.button.cut_no = 0;
	m_op.button.decided_action_scn_no = -1;
	m_op.button.decided_action_cmd_no = -1;
	m_op.button.decided_action_z_no = 0;
	m_op.button.excall_enable = 0;
	m_op.button.alpha_test = 0;

	// ボタンアクション
	m_op.button_decided_action_scn_name.clear();
	m_op.button_decided_action_cmd_name.clear();
}

// ****************************************************************
// オブジェクト：ボタンパラメータの設定
// ================================================================
void C_elm_object::set_button_param(int button_no, int group_no, int action_no, int se_no)
{
	m_op.button.button_no = button_no;
	m_op.button.action_no = action_no;
	m_op.button.se_no = se_no;

	// グループを設定する
	set_button_group_no(group_no);
}

// ****************************************************************
// ボタン決定アクションを設定
// ================================================================
bool C_elm_object::set_button_decided_action(CTSTR& scn_name, CTSTR& cmd_name)
{
	m_op.button_decided_action_scn_name = scn_name;
	m_op.button_decided_action_cmd_name = cmd_name;
	m_op.button.decided_action_z_no = -1;

	if (!restruct_button_action())
		return false;

	return true;
}

bool C_elm_object::set_button_decided_action(CTSTR& scn_name, int z_no)
{
	m_op.button_decided_action_scn_name = scn_name;
	m_op.button_decided_action_cmd_name.clear();
	m_op.button.decided_action_z_no = z_no;

	if (!restruct_button_action())
		return false;

	return true;
}

// ****************************************************************
// ボタンアクションをクリア
// ================================================================
bool C_elm_object::clear_button_decide_action()
{
	m_op.button_decided_action_scn_name.clear();
	m_op.button_decided_action_cmd_name.clear();
	m_op.button.decided_action_z_no = -1;

	if (!restruct_button_action())
		return false;

	return true;
}

// ****************************************************************
// ボタンのグループを設定
// ================================================================
void C_elm_object::set_button_group_no(int group_no)
{
	if (group_no >= 0)	{
		m_op.button.elm_group = m_op_def.elm_group_list + S_element(ELM_ARRAY, group_no);
	}
	else	{
		m_op.button.elm_group.clear();
	}
}
void C_elm_object::set_button_group(const S_element& element)
{
	m_op.button.elm_group = element;
}

// ****************************************************************
// ボタンのグループ番号を取得
// ================================================================
int	C_elm_object::get_button_group_no()
{
	if (m_op.button.elm_group.is_empty())	{
		return -1;
	}
	else	{
		return m_op.button.elm_group.back();
	}
}

// ****************************************************************
// ボタンアクションを再構築
// ================================================================
bool C_elm_object::restruct_button_action()
{
	m_op.button.decided_action_scn_no = Gp_lexer->get_scn_no(m_op.button_decided_action_scn_name);
	m_op.button.decided_action_cmd_no = Gp_lexer->get_user_cmd_no(m_op.button.decided_action_scn_no, m_op.button_decided_action_cmd_name);

	return true;
}

// ****************************************************************
// ボタンを登録する
// ================================================================
void C_elm_object::regist_button(int min_order)
{
	// グループを使うオブジェクト系
	if (m_op_def.use_group_flag)
	{
		// グループを取得する
		C_elm_group* p_group = get_group();
		if (p_group)
		{
			if (p_group->is_doing())
			{
				if (m_button.sorter().order >= min_order)
				{
					Gp_btn_mng->regist_button(&m_button);
				}
			}
		}
	}
	// グループを使わないシステム系
	else
	{
		if (m_op.button.action_no >= 0)
		{
			if (m_button.sorter().order >= min_order)
			{
				Gp_btn_mng->regist_button(&m_button);
			}
		}
	}

	// 子供
	int child_cnt = m_child_list.get_size();
	for (int i = 0; i < child_cnt; i++)
	{
		m_child_list[i].regist_button(min_order);
	}
}

// ****************************************************************
// ボタンステート（当たり）を取得する
// ================================================================
int C_elm_object::get_button_hit_state()
{
	// アクション番号が無効なら通常状態
	if (m_op.button.action_no < 0 || (int)Gp_btn_action_list->m_sub.size() <= m_op.button.action_no)
		return TNM_BTN_STATE_NORMAL;

	// 通常の当たり状態を取得
	return Gp_btn_mng->get_hit_state(&m_button);
}

// ****************************************************************
// ボタンステート（合計）を取得する
// ================================================================
int C_elm_object::get_button_real_state()
{
	C_tnm_btn_action_template* p_templete = NULL;
	S_tnm_btn_action_template_pat* p_action = NULL;

	// アクション番号が無効なら通常状態
	if (m_op.button.action_no < 0 || (int)Gp_btn_action_list->m_sub.size() <= m_op.button.action_no)
		return TNM_BTN_STATE_NORMAL;

	// 使えない状態判定
	if (false);
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_READ_SKIP)	{
		if (!tnm_syscom_read_skip_is_enable())	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_AUTO_MODE)	{
		if (!tnm_syscom_auto_mode_is_enable())	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_RETURN_SEL)	{
		if (!tnm_syscom_return_to_sel_is_enable())	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_HIDE_MWND)	{
		if (!tnm_syscom_hide_mwnd_is_enable())	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_SAVE)	{
		if (!tnm_syscom_open_save_is_enable())	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_LOAD)	{
		if (!tnm_syscom_open_load_is_enable())	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_QUICK_SAVE)	{
		if (!tnm_syscom_quick_save_is_enable(m_op.button.sys_type_opt))	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_QUICK_LOAD)	{
		if (!tnm_syscom_quick_load_is_enable(m_op.button.sys_type_opt))	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_MSG_BACK)	{
		if (!tnm_syscom_msg_back_is_enable())	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_KOE_PLAY)	{
		if (!tnm_syscom_koe_play_is_enable())	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_CONFIG)	{
		if (!tnm_syscom_open_config_is_enable())	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_LOCAL_EXTRA_SWITCH)	{
		if (!tnm_syscom_local_extra_switch_is_enable(m_op.button.sys_type_opt))	{
			return TNM_BTN_STATE_DISABLE;
		}
	}
	else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_LOCAL_EXTRA_MODE)	{
		if (!tnm_syscom_local_extra_mode_is_enable(m_op.button.sys_type_opt))	{
			return TNM_BTN_STATE_DISABLE;
		}
	}

	// ステートによって分岐
	if (false);
	else if (m_op.button.state == TNM_BTN_STATE_SELECT)	{
		return TNM_BTN_STATE_SELECT;
	}
	else if (m_op.button.state == TNM_BTN_STATE_DISABLE)	{
		return TNM_BTN_STATE_DISABLE;
	}

	// 触れない場合
	if (Gp_local->pod.mwnd_btn_touch_disable)
		return TNM_BTN_STATE_NORMAL;

	// グループを取得
	C_elm_group* p_group = get_group();

	// 所属するグループが決定されていた場合
	if (p_group && p_group->get_decided())	{

		// 自分自身が決定されていた場合は PUSH
		if (p_group->get_decided_button_no() == m_op.button.button_no)	{
			return TNM_BTN_STATE_PUSH;
		}
	}
	if (p_group && p_group->get_hit_button_no() >= 0 && p_group->get_hit_button_no() == m_op.button.button_no)	{
		return TNM_BTN_STATE_HIT;
	}
	if (p_group && p_group->get_pushed_button_no() >= 0 && p_group->get_pushed_button_no() == m_op.button.button_no)	{
		return TNM_BTN_STATE_PUSH;
	}

	// 通常の当たり状態を取得
	return Gp_btn_mng->get_hit_state(&m_button);
}

S_tnm_btn_action_template_pat* C_elm_object::get_action(int btn_state)
{
	C_tnm_btn_action_template* p_templete = NULL;
	S_tnm_btn_action_template_pat* p_action = NULL;

	// アクション番号が無効ならアクションなし
	if (m_op.button.action_no < 0 || (int)Gp_btn_action_list->m_sub.size() <= m_op.button.action_no)
		return NULL;

	// テンプレートを取得
	p_templete = &Gp_btn_action_list->m_sub[m_op.button.action_no];

	// アクションを返す
	return p_templete->get_action(btn_state);
}

// ****************************************************************
// ボタンイベント処理
// ================================================================
void C_elm_object::button_event()
{
	int btn_state = get_button_real_state();
	if (btn_state == TNM_BTN_STATE_SELECT || btn_state == TNM_BTN_STATE_DISABLE)
		return;

	// 触れない場合ここまで
	if (Gp_local->pod.mwnd_btn_touch_disable)
		return;

	// ボタンの場合のみ処理
	if (m_op.button.action_no >= 0)	{

		// グループを取得
		C_elm_group* p_group = get_group();

		// グループがない（システムボタンなど）もしくは、グループがあって稼働中の場合
		if (p_group == NULL || p_group->is_doing())	{

			// 当たった
			if (Gp_btn_mng->is_hit_this_frame(&m_button))	{
				if (p_group == NULL || m_op.button.button_no != p_group->get_last_hit_button_no())	{

					// 効果音
					if (0 <= m_op.button.se_no && m_op.button.se_no < (int)Gp_btn_se_list->m_sub.size())	{
						int se_no = Gp_btn_se_list->m_sub[m_op.button.se_no].hit_no;
						tnm_play_se(se_no);
					}
				}
			}

			// 当たっている
			if (p_group && Gp_btn_mng->is_hit(&m_button))	{
				p_group->set_hit_button_no(m_op.button.button_no);
			}

			// 押された
			if (Gp_btn_mng->is_pushed_this_frame(&m_button))	{
				if (p_group == NULL || m_op.button.button_no != p_group->get_last_pushed_button_no())	{

					// 効果音
					if (0 <= m_op.button.se_no && m_op.button.se_no < (int)Gp_btn_se_list->m_sub.size())	{
						int se_no = Gp_btn_se_list->m_sub[m_op.button.se_no].push_no;
						tnm_play_se(se_no);
					}
				}
			}

			// 押されている
			if (p_group && Gp_btn_mng->is_pushed(&m_button))	{
				p_group->set_pushed_button_no(m_op.button.button_no);
			}

			// 決定された
			if (Gp_btn_mng->is_decided_this_frame(&m_button))	{

				// 効果音
				if (0 <= m_op.button.se_no && m_op.button.se_no < (int)Gp_btn_se_list->m_sub.size())	{
					int se_no = Gp_btn_se_list->m_sub[m_op.button.se_no].decide_no;
					tnm_play_se(se_no);
				}

				// グループに属しているならグループに通知
				if (p_group)
					p_group->decide(m_op.button.button_no);

				// ボタン決定アクション
				if (false);
				else if (m_op.button.decided_action_scn_no >= 0 && m_op.button.decided_action_cmd_no >= 0)	{

					// 指定のコマンドをコール
					tnm_scene_proc_call_user_cmd(m_op.button.decided_action_scn_no, m_op.button.decided_action_cmd_no, FM_VOID, true, false);
				}
				else if (m_op.button.decided_action_scn_no >= 0 && m_op.button.decided_action_z_no >= 0)	{

					// 指定のシーンをコール
					tnm_scene_proc_farcall(m_op.button.decided_action_scn_no, m_op.button.decided_action_z_no, FM_VOID, true, false);
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_READ_SKIP)	{

					if (m_op.button.mode == 0)	{
						tnm_syscom_read_skip_set_onoff_flag(true);				// 既読早送り開始
					}
					else	{
						tnm_syscom_read_skip_set_onoff_flag(false);				// 既読早送り停止
					}
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_AUTO_MODE)	{

					if (m_op.button.mode == 0)	{
						tnm_syscom_auto_mode_set_onoff_flag(true);				// オートモード開始
					}
					else	{
						tnm_syscom_auto_mode_set_onoff_flag(false);				// オートモード停止
					}
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_HIDE_MWND)	{

					tnm_syscom_hide_mwnd_set_onoff_flag(true);					// メッセージウィンドウを隠す
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_SAVE)	{

					tnm_syscom_open_save(false, true, true);					// （★セーブ：ＭＷボタン）ダイアログが呼ばれる場合はキャプチャーする
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_LOAD)	{

					tnm_syscom_open_load(false, true);							// ロード
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_QUICK_SAVE)	{

					tnm_syscom_quick_save(m_op.button.sys_type_opt, true, true);		// クイックセーブ
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_QUICK_LOAD)	{

					tnm_syscom_quick_load(m_op.button.sys_type_opt, true, true, true);	// クイックロード
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_RETURN_SEL)	{

					tnm_syscom_return_to_sel(true, true, true);							// 前の選択肢に戻る
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_MSG_BACK)	{

					tnm_syscom_open_msg_back();									// メッセージバック
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_KOE_PLAY)	{

					tnm_syscom_koe_play();										// 声を再生
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_CONFIG)	{

					tnm_syscom_open_config();									// 環境設定
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_LOCAL_EXTRA_SWITCH)	{

					if (m_op.button.mode == 0)	{
						tnm_syscom_local_extra_switch_set_onoff_flag(m_op.button.sys_type_opt, true);		// ローカル汎用スイッチ
					}
					else	{
						tnm_syscom_local_extra_switch_set_onoff_flag(m_op.button.sys_type_opt, false);		// ローカル汎用スイッチ
					}
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_LOCAL_EXTRA_MODE)	{

					tnm_syscom_local_extra_mode_increment(m_op.button.sys_type_opt);		// ローカル汎用モード
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_GLOBAL_EXTRA_SWITCH)	{
					if (0 <= m_op.button.sys_type_opt && m_op.button.sys_type_opt < (int)Gp_config->global_extra_switch_flag.size())	{
						Gp_config->global_extra_switch_flag[m_op.button.sys_type_opt].value = !Gp_config->global_extra_switch_flag[m_op.button.sys_type_opt].value;
					}
				}
				else if (m_op.button.sys_type == TNM_SYSCOM_TYPE_GLOBAL_EXTRA_MODE)	{
					if (0 <= m_op.button.sys_type_opt && m_op.button.sys_type_opt < (int)Gp_config->global_extra_mode_flag.size())	{
						Gp_config->global_extra_mode_flag[m_op.button.sys_type_opt] ++;
						Gp_config->global_extra_mode_flag[m_op.button.sys_type_opt] %= Gp_ini->config.global_extra_mode[m_op.button.sys_type_opt].item_cnt;
					}
				}
			}
		}
	}

	// 子供
	int child_cnt = m_child_list.get_size();
	for (int i = 0; i < child_cnt; i++)
		m_child_list[i].button_event();
}

// ****************************************************************
// 現在のボタン状態を判定
// ================================================================
bool C_elm_object::is_hit()
{
	return Gp_btn_mng->is_hit(&m_button);
}
bool C_elm_object::is_pushed()
{
	return Gp_btn_mng->is_pushed(&m_button);
}
bool C_elm_object::is_decided()
{
	return Gp_btn_mng->is_decided(&m_button);
}
bool C_elm_object::is_hit_this_frame()
{
	return Gp_btn_mng->is_hit_this_frame(&m_button);
}
bool C_elm_object::is_pushed_this_frame()
{
	return Gp_btn_mng->is_pushed_this_frame(&m_button);
}
bool C_elm_object::is_decided_this_frame()
{
	return Gp_btn_mng->is_decided_this_frame(&m_button);
}

// ****************************************************************
// 所属するグループを取得
// ================================================================
C_elm_group* C_elm_object::get_group()
{
	// グループを使うオブジェクト系
	if (m_op_def.use_group_flag)
	{
		// グループを取得する
		if (!m_op.button.elm_group.is_empty())
		{
			if (m_op.button.elm_group.code[0] == ELM_UP)
			{
				// 相対パス
				S_tnm_command_proc_arg_struct arg;
				arg.disp_out_of_range_error = false;
				if (tnm_command_proc_object(this, m_op.button.elm_group.begin(), m_op.button.elm_group.begin(), m_op.button.elm_group.end(), &arg))
				{
					C_elm_group* p_group = (C_elm_group *)arg.p_ret_element;
					if (p_group)
					{
						return p_group;
					}
				}
			}
			else
			{
				// 絶対パス
				C_elm_group* p_group = (C_elm_group *)tnm_get_element_ptr(m_op.button.elm_group.begin(), m_op.button.elm_group.end(), false);
				if (p_group)
				{
					return p_group;
				}
			}
		}
	}

	return NULL;
}

