#pragma		once

#include	"element/elm_list.h"

// ****************************************************************
// Ｇ００バッファ
// ================================================================
class C_elm_g00_buf : public C_tnm_element
{
public:

	// エレメントの基本動作
	void	init(S_element element, CTSTR& name);	// 初期化
	void	reinit();								// 再初期化
	void	save(C_tnm_save_stream& stream);		// セーブ
	void	load(C_tnm_save_stream& stream);		// ロード

	// ファイルの読み込みと解放
	bool	load_file(CTSTR& file_name);
	void	free();

	// 状態取得
	bool	is_load()		{ return m_album ? true : false; }
	TSTR	get_file_name()	{ return m_file_name; }

private:
	TSTR				m_file_name;
	BSP<C_d3d_album>	m_album;
};

// ****************************************************************
// Ｇ００バッファリスト
// ================================================================
class C_elm_g00_buf_list : public C_elm_list<C_elm_g00_buf>
{
protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end);							// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード
};
