#include	"pch.h"

#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"

#include	"data/tnm_save_00.h"
#include	"data/tnm_proc.h"
#include	"data/tnm_lexer.h"

#include	"element/elm_call.h"
#include	"element/elm_frame_action.h"

#include	"engine/flow_script.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_scene.h"
#include	"engine/ifc_stack.h"

// ****************************************************************
// 初期化
// ================================================================
void C_elm_frame_action::init(S_element element, CTSTR& name, S_element target)
{
	// 基底クラスの初期化
	C_tnm_element::init(element, FM_FRAMEACTION, name, NULL);

	// 固定パラメータ
	m_target = target;
	m_counter.init(element + S_element(ELM_FRAMEACTION_COUNTER), _T("counter"));

	// 通常パラメータ
	m_scn_no = -1;
	m_cmd_no = -1;
	m_end_time = 0;
	m_scn_name.clear();
	m_cmd_name.clear();
	m_arg_list.clear();
	m_end_action_flag = false;
}

// ****************************************************************
// 再初期化
// ================================================================
void C_elm_frame_action::reinit(bool finish_flag)
{
	// 終了
	if (finish_flag)
		finish();

	// 初期化
	m_scn_no = -1;
	m_cmd_no = -1;
	m_scn_name.clear();
	m_cmd_name.clear();
	m_arg_list.clear();
	m_counter.reinit();
	m_end_action_flag = false;
}

// ****************************************************************
// 終了
// ================================================================
void C_elm_frame_action::finish()
{
	// フレームアクションの情報を記憶
	int scn_no = m_scn_no;
	int cmd_no = m_cmd_no;
	int end_time = m_end_time;

	// フレームアクションを初期化（その１）
	// エンドアクションの中でフレームアクションを初期化すると、さらにエンドアクションが呼ばれて…という無限ループを防ぐため、先にパラメータを初期化しておきます。
	m_scn_no = -1;
	m_cmd_no = -1;
	m_scn_name.clear();
	m_cmd_name.clear();
	m_end_action_flag = true;	// エンドアクションフラグを立てる

	// エンドアクションを起こす
	if (scn_no >= 0 && cmd_no >= 0)	{
		m_counter.set_count(end_time == -1 ? 0 : end_time);
		do_action(scn_no, cmd_no);
	}

	// フレームアクションを初期化（その２）
	m_end_action_flag = false;	// エンドアクションフラグを降ろす
}

// ****************************************************************
// セーブ
// ================================================================
void C_elm_frame_action::save(C_tnm_save_stream& stream)
{
	stream.save(m_end_time);		// 終了時刻
	stream.save(m_scn_name);		// シーン名
	stream.save(m_cmd_name);		// コマンド名
	m_arg_list.save(stream);		// 引数
	m_counter.save(stream);			// カウンタ
}

// ****************************************************************
// ロード
// ================================================================
void C_elm_frame_action::load(C_tnm_save_stream& stream)
{
	// ★初期化済みを前提としています。

	stream.load(m_end_time);		// 終了時刻
	stream.load(m_scn_name);		// シーン名
	stream.load(m_cmd_name);		// コマンド名
	m_arg_list.load(stream);		// 引数
	m_counter.load(stream);			// カウンタ

	// 再構築
	restruct();
}

// ****************************************************************
// コピー
// ================================================================
void C_elm_frame_action::copy(const C_elm_frame_action* src, bool finish_flag)
{
	if (finish_flag)
		finish();

	m_end_time = src->m_end_time;		// 終了時刻
	m_scn_name = src->m_scn_name;		// シーン名
	m_cmd_name = src->m_cmd_name;		// コマンド名
	m_arg_list = src->m_arg_list;		// 引数
	m_counter.copy(&src->m_counter);	// カウンタ

	// 再構築
	restruct();
}

// ****************************************************************
// 再構築
// ================================================================
void C_elm_frame_action::restruct()
{
	m_scn_no = Gp_lexer->get_scn_no(m_scn_name);
	m_cmd_no = Gp_lexer->get_user_cmd_no(m_scn_no, m_cmd_name);
}

// ****************************************************************
// パラメータを設定する
// ================================================================
void C_elm_frame_action::set_param(int end_time, int real_flag, CTSTR& scn_name, CTSTR& cmd_name, C_tnm_prop_list& arg_list)
{
	// 初期化
	reinit(true);

	// パラメータを設定
	m_end_time = end_time;
	m_scn_name = scn_name;
	m_cmd_name = cmd_name;
	m_arg_list = arg_list;

	// カウンタをスタートする
	if (real_flag)	{
		m_counter.start_real();
	}
	else	{
		m_counter.start();
	}

	// 再構築
	restruct();
}

// ****************************************************************
// アクションを実行する
// ================================================================
void C_elm_frame_action::do_action()
{
	do_action(m_scn_no, m_cmd_no);
}

void C_elm_frame_action::do_action(int scn_no, int cmd_no)
{
	if (scn_no < 0 || cmd_no < 0)
		return;

	// 自身のエレメントを引数として積む
	tnm_stack_push_element(get_element());

	// ターゲットのエレメントを引数として積む
	if (m_target.code_cnt > 0)
		tnm_stack_push_element(m_target);

	// 型チェックをしながら引数をスタックに積む
	for (C_tnm_prop* arg = m_arg_list.get_ptr(); arg < m_arg_list.get_end_ptr(); arg++)	{
		if (false);
		else if (arg->form == FM_INT)	{
			tnm_stack_push_int(arg->Int);
		}
		else if (arg->form == FM_STR)	{
			tnm_stack_push_str(arg->str);
		}
	}

	// 現在のコール数を取得
	int cur_call_cnt = tnm_get_cur_call_cnt();

	// コマンドをコール
	tnm_scene_proc_call_user_cmd(scn_no, cmd_no, FM_VOID, false, true);

	// コールの種類を設定する
	C_elm_call* p_cur_call = tnm_get_cur_call();
	p_cur_call->arg_cnt = m_arg_list.get_size() + 1 + (m_target.code_cnt > 0 ? 1 : 0);		// 引数の個数
	p_cur_call->scn_no = scn_no;	// ユーザコマンドのシーン番号
	p_cur_call->cmd_no = cmd_no;	// ユーザコマンドのコマンド番号

	// 再度スクリプトを実行する
	tnm_proc_script();

	// もしコール数が変わっていたら、return ではなく disp などで抜けた可能性
	// return を行う
	//int new_call_cnt = tnm_get_cur_call_cnt();
	//if (new_call_cnt != cur_call_cnt)	{
	//	tnm_return();
	//}
}

// ****************************************************************
// 時間を更新
// ================================================================
void C_elm_frame_action::update_time(int past_game_time, int past_real_time)
{
	// フレームアクションカウンタ
	m_counter.update_time(past_game_time, past_real_time);
}

// ****************************************************************
// フレーム処理
// ================================================================
void C_elm_frame_action::frame()
{
	// フレームアクション終了
	if (m_scn_no >= 0 && m_cmd_no >= 0 && m_end_time >= 0)	{
		if (m_counter.get_count() >= m_end_time)	{
			reinit(true);
		}
	}
}

// ****************************************************************
// フレームアクションリスト：初期化
// ================================================================
void C_elm_frame_action_list::init(S_element element, CTSTR& name, int size, bool extend_enable, S_element target)
{
	m_target = target;

	// 基底の初期化
	C_elm_list_ex<C_elm_frame_action>::init(element, FM_FRAMEACTIONLIST, name, size, extend_enable, NULL);
}

// ****************************************************************
// フレームアクションリスト：再初期化
// ================================================================
void C_elm_frame_action_list::reinit(bool finish_flag, bool restruct_flag)
{
	// 終了処理
	if (finish_flag)
		finish();

	// 基底の再初期化
	C_elm_list_ex<C_elm_frame_action>::reinit(restruct_flag);
}

// ****************************************************************
// フレームアクションリスト：終了
// ================================================================
void C_elm_frame_action_list::finish()
{
	int fa_cnt = (int)m_sub.size();
	for (int i = 0; i < fa_cnt; i++)
		m_sub[i].finish();
}

// ****************************************************************
// フレームアクションリスト：サブの初期化
// ================================================================
void C_elm_frame_action_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"), m_target);
}

// ****************************************************************
// フレームアクションリスト：サブの再初期化
// ================================================================
void C_elm_frame_action_list::_reinit(int begin, int end, bool restruct_flag)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit(true);				// これは finish フラグ
}

// ****************************************************************
// フレームアクションリスト：サブのセーブ
// ================================================================
void C_elm_frame_action_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// フレームアクションリスト：サブのロード
// ================================================================
void C_elm_frame_action_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}


