#include	"pch.h"

#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_input.h"
#include	"data/tnm_save_00.h"

#include	"element/elm_object.h"
#include	"element/elm_group.h"

#include	"engine/eng_input.h"
#include	"engine/ifc_stack.h"
#include	"engine/ifc_sound.h"
#include	"engine/ifc_proc_stack.h"

// ****************************************************************
// グループ：初期化
// ================================================================
void C_elm_group::init(S_element element, CTSTR& name, int group_no, S_element target_object)
{
	// 基底クラスの初期化
	C_tnm_element::init(element, FM_GROUP, name, NULL);

	// 固定パラメータを設定
	m_def.group_no = group_no;
	m_def.target_object = target_object;

	// 再初期化
	reinit();
}

// ****************************************************************
// グループ：再初期化
// ================================================================
void C_elm_group::reinit()
{
	m_param.sorter = S_tnm_sorter(0, 0);
	m_param.cancel_priority = 0;
	m_param.cancel_se_no = -1;
	m_param.decided_button_no = TNM_GROUP_NOT_DECIDED;

	m_param.result = TNM_GROUP_RESULT_NONE;
	m_param.result_button_no = 0;

	m_param.doing = false;
	m_param.pause_flag = false;
	m_param.wait_flag = false;
	m_param.cancel_flag = false;
	m_param.target_object = m_def.target_object;

	m_work.hit_button_no = -1;
	m_work.pushed_button_no = -1;
	m_work.last_hit_button_no = -1;
	m_work.last_pushed_button_no = -1;
}

// ****************************************************************
// グループ：選択を初期化
// ================================================================
void C_elm_group::init_sel()
{
	m_param.cancel_priority = 0;
	m_param.cancel_se_no = -1;
	m_param.decided_button_no = TNM_GROUP_NOT_DECIDED;

	m_param.result = TNM_GROUP_RESULT_NONE;
	m_param.result_button_no = 0;

	m_param.doing = false;
	m_param.pause_flag = false;
	m_param.wait_flag = false;
	m_param.cancel_flag = false;

	m_work.hit_button_no = -1;
	m_work.pushed_button_no = -1;
	m_work.last_hit_button_no = -1;
	m_work.last_pushed_button_no = -1;
}

// ****************************************************************
// グループ：セーブ
// ================================================================
void C_elm_group::save(C_tnm_save_stream& stream)
{
	stream.save(m_param);
}

// ****************************************************************
// グループ：ロード
// ================================================================
void C_elm_group::load(C_tnm_save_stream& stream)
{
	stream.load(m_param);
}

// ****************************************************************
// グループ：コピー
// ================================================================
void C_elm_group::copy(C_elm_group* src)
{
	m_param = src->m_param;
}

// ****************************************************************
// グループ：ボタンフレーム初期化処理
// ================================================================
void C_elm_group::button_frame_init()
{
	m_work.hit_button_no = m_work.last_hit_button_no;
	m_work.pushed_button_no = m_work.last_pushed_button_no;
}

// ****************************************************************
// グループ：キャンセル判定
// ================================================================
void C_elm_group::cancel_check()
{
	// キャンセル可能な場合
	if (m_param.doing && m_param.cancel_flag)	{

		// 右クリックされた
		if (tnm_input_use_key_down_up(VK_EX_CANCEL))	{

			// キャンセル
			cancel();
		}
	}
}

// ****************************************************************
// グループ：決定された
// ================================================================
void C_elm_group::decide(int button_no)
{
	if (m_param.doing)	{

		m_param.doing = false;

		// result v1
		m_param.decided_button_no = button_no;
		// result v2
		m_param.result = TNM_GROUP_RESULT_DECIDED;
		m_param.result_button_no = button_no;

		// 当たり情報を初期化
		m_work.hit_button_no = -1;
		m_work.pushed_button_no = -1;
		m_work.last_hit_button_no = -1;
		m_work.last_pushed_button_no = -1;

		// 効果音はオブジェクトが鳴らす

		// 待ち状態だった場合
		if (get_wait_flag())	{
			tnm_stack_push_int(button_no);			// 結果をスタックに積む
			tnm_pop_proc();							// 次のプロセスへ
		}
	}
}

// ****************************************************************
// グループ：キャンセルされた
// ================================================================
void C_elm_group::cancel()
{
	if (m_param.doing)	{

		m_param.doing = false;

		// result v1
		m_param.decided_button_no = TNM_GROUP_CANCELED;

		// result v2
		m_param.result = TNM_GROUP_RESULT_CANCELLED;
		m_param.result_button_no = m_work.hit_button_no;

		// 当たり情報を初期化
		m_work.hit_button_no = -1;
		m_work.pushed_button_no = -1;
		m_work.last_hit_button_no = -1;
		m_work.last_pushed_button_no = -1;

		// キャンセル音はグループが鳴らす
		tnm_play_se(m_param.cancel_se_no);

		// 待ち状態だった場合
		if (get_wait_flag())	{
			tnm_stack_push_int(TNM_GROUP_CANCELED);	// 結果をスタックに積む
			tnm_pop_proc();							// 次のプロセスへ
		}
	}
}

// ****************************************************************
// グループリスト：初期化
// ================================================================
void C_elm_group_list::init(S_element element, CTSTR& name, int size, bool extend_enable, S_element target_object)
{
	// 固定パラメータ
	m_target_object = target_object;

	// 基底の初期化
	C_elm_list<C_elm_group>::init(element, FM_GROUPLIST, name, size, extend_enable, NULL);
}

// ****************************************************************
// グループリスト：サブの初期化
// ================================================================
void C_elm_group_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"), i, m_target_object);
}

// ****************************************************************
// グループリスト：サブの再初期化
// ================================================================
void C_elm_group_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// グループリスト：サブのセーブ
// ================================================================
void C_elm_group_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// グループリスト：サブのロード
// ================================================================
void C_elm_group_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}










