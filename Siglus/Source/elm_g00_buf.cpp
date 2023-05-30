#include	"pch.h"
#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_g00_buf.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// Ｇ００バッファ：初期化
// ================================================================
void C_elm_g00_buf::init(S_element element, CTSTR& name)
{
	// 基底クラスの初期化
	C_tnm_element::init(element, FM_G00BUF, name, NULL);

	// 再初期化
	reinit();
}

// ****************************************************************
// Ｇ００バッファ：再初期化
// ================================================================
void C_elm_g00_buf::reinit()
{
	m_file_name.clear();
	m_album.reset();
}

// ****************************************************************
// Ｇ００バッファ：セーブ
// ================================================================
void C_elm_g00_buf::save(C_tnm_save_stream& stream)
{
	stream.save(m_file_name);
}

// ****************************************************************
// Ｇ００バッファ：ロード
// ================================================================
void C_elm_g00_buf::load(C_tnm_save_stream& stream)
{
	TSTR file_name;
	stream.load(file_name);

	if (!file_name.empty())	{
		load_file(file_name);
	}
}

// ****************************************************************
// Ｇ００バッファ：ファイルを読み込み
// ================================================================
bool C_elm_g00_buf::load_file(CTSTR& file_name)
{
	// テクスチャを読み込む
	m_album = tnm_load_pct_d3d(file_name, false, false);	// ＣＧテーブルには登録しない！
	if (!m_album)
		return false;

	m_file_name = file_name;
	return true;
}

// ****************************************************************
// Ｇ００バッファ：解放
// ================================================================
void C_elm_g00_buf::free()
{
	m_file_name.clear();
	m_album.reset();
}

// ****************************************************************
// Ｇ００バッファリスト：サブの初期化
// ================================================================
void C_elm_g00_buf_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"));
}

// ****************************************************************
// Ｇ００バッファリスト：サブの再初期化
// ================================================================
void C_elm_g00_buf_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// Ｇ００バッファリスト：サブのセーブ
// ================================================================
void C_elm_g00_buf_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// Ｇ００バッファリスト：サブのロード
// ================================================================
void C_elm_g00_buf_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}
