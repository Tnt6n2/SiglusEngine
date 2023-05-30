#pragma		once

#include	"data/tnm_local_data.h"
#include	"element/elm_counter.h"
#include	"element/elm_stage.h"

// ****************************************************************
// エレメント：ＥＸコール
// ================================================================
class C_elm_excall : public C_tnm_element
{
public:
	void	init(S_element element, CTSTR& name);			// 初期化
	void	reinit(bool finish_flag, bool restruct_flag);	// 再初期化
	void	finish();										// 終了
	// このエレメントのセーブロードはありません。

	void	ready();	// 準備する
	void	free();		// 解放する
	bool	is_ready()	{	return m_ready_flag;	}

	// 子エレメント
	C_elm_int_list			m_F;					// フラグ
	C_elm_counter_list		m_counter_list;			// カウンタ
	C_elm_frame_action		m_frame_action;			// フレームアクション
	C_elm_frame_action_list	m_frame_action_ch_list;	// フレームアクションチャンネル
	C_elm_stage_list		m_stage_list;			// ステージ

	// フォント
	TSTR					m_font_name;			// フォント名

	// pod
	C_tnm_local_data_pod	m_pod;

private:
	bool					m_ready_flag;			// 準備フラグ
};
