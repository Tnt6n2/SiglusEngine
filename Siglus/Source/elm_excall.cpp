#include	"pch.h"

#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"element/elm_btn_sel_item.h"
#include	"element/elm_mwnd_moji.h"
#include	"element/elm_excall.h"

// ****************************************************************
// ＥＸコール：初期化
// ================================================================
void C_elm_excall::init(S_element element, CTSTR& name)
{
	// 基底クラスの初期化
	C_tnm_element::init(element, FM_EXCALL, name, NULL);

	// フラグの初期化
	m_F.init(element + S_element(ELM_EXCALL_F), name + _T(".F"), 0, true);

	// カウンタの初期化
	m_counter_list.init(element + S_element(ELM_EXCALL_COUNTER), FM_COUNTERLIST, name + _T(".counter"), 0, true, NULL);

	// フレームアクションを初期化します。
	m_frame_action.init(element + S_element(ELM_EXCALL_FRAME_ACTION), _T(".frame_action"), S_element());

	// フレームアクションチャンネルを初期化します。
	m_frame_action_ch_list.init(element + S_element(ELM_EXCALL_FRAME_ACTION_CH), _T(".frame_action_ch"), 0, true, S_element());

	// ステージの初期化
	m_stage_list.init(element + S_element(ELM_EXCALL_STAGE), name + _T(".stage"), name + _T("."), 0, true, false, NULL);

	// 再初期化
	reinit(false, true);
}

// ****************************************************************
// ＥＸコール：再初期化
// ================================================================
void C_elm_excall::reinit(bool finish_flag, bool restruct_flag)
{
	m_ready_flag = false;

	// 子エレメント
	m_F.reinit();
	m_counter_list.reinit();
	m_frame_action.reinit(finish_flag);
	m_frame_action_ch_list.reinit(finish_flag, restruct_flag);
	m_stage_list.reinit(restruct_flag);

	// フォント
	m_font_name.clear();

	// pod
	m_pod.init();
}

// ****************************************************************
// ＥＸコール：終了
// ================================================================
void C_elm_excall::finish()
{
	// フレームアクション
	m_frame_action.finish();

	// フレームアクションチャンネル
	m_frame_action_ch_list.finish();

	// ステージ
	m_stage_list.finish();
}

// ****************************************************************
// ＥＸコール：準備する
// ================================================================
void C_elm_excall::ready()
{
	// エレメントを準備する
	m_F.resize(Gp_ini->flag_cnt);
	m_counter_list.resize(Gp_ini->counter_cnt);
	m_frame_action_ch_list.resize(Gp_ini->frame_action_ch_cnt);
	m_stage_list.resize(TNM_STAGE_CNT);
	m_ready_flag = true;
}

// ****************************************************************
// ＥＸコール：解放する
// ================================================================
void C_elm_excall::free()
{
	// 終了
	finish();

	// エレメントを解放する
	m_F.clear();
	m_counter_list.clear();
	m_frame_action_ch_list.clear();
	m_stage_list.clear();
	m_ready_flag = false;

	// フォント
	m_font_name.clear();
}















