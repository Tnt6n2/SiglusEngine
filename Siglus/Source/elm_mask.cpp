#include	"pch.h"

#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_mask.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// マスク：初期化
// ================================================================
void C_elm_mask::init(S_element element, CTSTR& name)
{
	// 基底クラスの初期化
	C_tnm_element::init(element, FM_MASK, name, NULL);

	// 再初期化
	reinit();
}

// ****************************************************************
// マスク：再初期化
// ================================================================
void C_elm_mask::reinit()
{
	// パラメータ
	m_param.x.init(0);
	m_param.y.init(0);
	m_mask_name.clear();

	// 作業パラメータ
	m_album.reset();
}

// ****************************************************************
// マスク：セーブ
// ================================================================
void C_elm_mask::save(C_tnm_save_stream& stream)
{
	stream.save(m_param);
	stream.save(m_mask_name);
}

// ****************************************************************
// マスク：ロード
// ================================================================
void C_elm_mask::load(C_tnm_save_stream& stream)
{
	stream.load(m_param);
	stream.load(m_mask_name);

	// 再構築
	restruct();
}

// ****************************************************************
// マスク：マスクを作成
// ================================================================
void C_elm_mask::create(CTSTR& file_name)
{
	// 作りなおしに備えてアルバムを保持しておく（参照カウンタを１あげる）
	BSP<C_d3d_album> temp_album = m_album;

	// 再初期化
	reinit();

	// パラメータを設定
	m_mask_name = file_name;

	// 再構築
	restruct();
}

// ****************************************************************
// マスク：再構築
// ================================================================
void C_elm_mask::restruct()
{
	// アルバムを取得
	if (!m_mask_name.empty())	{
		m_album = tnm_load_pct_d3d(m_mask_name, false, false);
		if (!m_album)	{
			m_mask_name.clear();	// 失敗した場合は初期化
		}
	}
}

// ****************************************************************
// マスク：時間を更新
// ================================================================
void C_elm_mask::update_time(int past_game_time, int past_real_time)
{
	m_param.x.update_time(past_game_time, past_real_time);
	m_param.y.update_time(past_game_time, past_real_time);
}

// ****************************************************************
// マスク：フレーム処理
// ================================================================
void C_elm_mask::frame()
{
	m_param.x.frame();
	m_param.y.frame();
}

// ****************************************************************
// マスクリスト：サブの初期化
// ================================================================
void C_elm_mask_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"));
}

// ****************************************************************
// マスクリスト：サブの再初期化
// ================================================================
void C_elm_mask_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// マスクリスト：サブのセーブ
// ================================================================
void C_elm_mask_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// マスクリスト：サブのロード
// ================================================================
void C_elm_mask_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}
