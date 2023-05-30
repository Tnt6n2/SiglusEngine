#pragma		once

#include	"tnm_common_header.h"

class	C_tnm_save_stream;

// ****************************************************************
// シーン情報
// ================================================================
//		各シーンデータの情報を格納します。
// ================================================================
struct S_scn_info
{
	BYTE*					p_data_top;
	S_tnm_scn_header*		p_header;					// ヘッダ
	BYTE*					p_scn;						// シーン
	int*					p_label_list;				// ラベルリスト
	int*					p_z_label_list;				// Ｚラベルリスト
	C_index*				p_str_index_list;			// 文字列リストトップ
	TCHAR*					p_str_list;					// 文字列リスト
	S_tnm_scn_scn_prop*		p_scn_prop_list;			// シーンプロパティリスト
	C_index*				p_scn_prop_name_index_list;	// シーンプロパティ名インデックスリスト
	TCHAR*					p_scn_prop_name_list;		// シーンプロパティ名リスト
	C_index*				p_call_prop_name_index_list;// コールプロパティ名インデックスリスト
	TCHAR*					p_call_prop_name_list;		// コールプロパティ名リスト
	S_tnm_scn_scn_cmd*		p_scn_cmd_list;				// シーンコマンドリスト
	C_index*				p_scn_cmd_name_index_list;	// シーンコマンド名インデックスリスト
	TCHAR*					p_scn_cmd_name_list;		// シーンコマンド名リスト
	int*					p_namae_list;				// 【名前】リスト
	S_tnm_scn_read_flag*	p_read_flag_list;			// 既読フラグリスト

	std::map<TSTR, int>		scn_prop_no_map;		// シーンプロパティ名 → シーンプロパティ番号のマップ
	std::map<TSTR, int>		scn_cmd_no_map;			// シーンコマンド名 → シーンコマンド番号のマップ
};

// ****************************************************************
// パックシーン情報
// ================================================================
//		パックシーンデータの情報を格納します。
// ================================================================
struct S_pack_scn_info
{
	BYTE*						p_data_top;
	S_tnm_pack_scn_header*		p_header;
	S_tnm_pack_scn_inc_prop*	p_inc_prop_list;
	C_index*					p_inc_prop_name_index_list;
	TCHAR*						p_inc_prop_name_list;
	C_index*					p_inc_cmd_name_index_list;
	TCHAR*						p_inc_cmd_name_list;
	S_tnm_pack_scn_inc_cmd*		p_inc_cmd_list;
	C_index*					p_scn_name_index_list;
	TCHAR*						p_scn_name_list;
	C_index*					p_scn_data_index_list;
	BYTE*						p_scn_data_list;

	std::map<TSTR, int>			scn_no_map;			// シーン名 → シーン番号のマップ
	std::map<TSTR, int>			inc_prop_no_map;	// inc プロパティ名 → inc プロパティ番号のマップ
	std::map<TSTR, int>			inc_cmd_no_map;		// inc コマンド名 → inc コマンド番号のマップ
};

// ****************************************************************
// スクリプトレクサー
// ================================================================
class C_tnm_scene_lexer
{
public:
	C_tnm_scene_lexer();

	// 初期化
	void	init();

	// Scene.pck を読み込み
	bool	load_scn(CTSTR& scn_pack_name);

	// オリジナルソース判定
	bool	is_exist_org_source();

	// ユーザプロパティ
	int		get_user_inc_prop_cnt();
	int		get_user_scn_prop_cnt(int scn_no);
	int		get_user_inc_prop_form(int prop_id);
	int		get_user_scn_prop_form(int scn_no, int prop_id);
	int		get_user_inc_prop_size(int prop_id);
	int		get_user_scn_prop_size(int scn_no, int prop_id);
	TSTR	get_user_inc_prop_name(int prop_id);
	TSTR	get_user_scn_prop_name(int scn_no, int prop_id);
	TSTR	get_user_call_prop_name(int scn_no, int prop_id);

	int		get_user_prop_no(CTSTR& name);					// ユーザプロパティ番号を取得
	int		get_user_prop_no(int scn_no, CTSTR& name);		// ユーザプロパティ番号を取得

	// ユーザコマンド
	int		get_user_cmd_cnt(int scn_no);					// ユーザコマンド数を取得
	int		get_user_cmd_no(int scn_no, CTSTR& name);		// ユーザコマンド番号を取得
	TSTR	get_user_cmd_name(int scn_no, int user_cmd_no);	// ユーザコマンドの名前を取得
	bool	is_exist_user_cmd(int scn_no, CTSTR& name);		// ユーザコマンド存在判定

	// シーン
	int		get_scn_cnt();									// シーン数を取得
	int		get_scn_no(CTSTR& name);						// シーン番号を取得
	TSTR	get_scn_name(int scn_no);						// シーンの名前を取得
	int		get_cur_scn_no();								// 現在のシーン番号を取得
	void	set_cur_scn_no(int scn_no);						// 現在のシーン番号を設定
	int		get_cur_line_no();								// 現在の行番号を取得
	void	set_cur_line_no(int line_no);					// 現在の行番号を設定
	BYTE*	get_cut_ptr();									// 現在のポインタを取得
	int		get_prg_cntr();									// プログラムカウンタを取得
	void	set_prg_cntr(int prg_cntr);						// プログラムカウンタを設定
	bool	is_exist_scn(CTSTR& name);						// シーン存在判定
	bool	is_exist_z(int scn_no, int z_no);				// Ｚラベル存在判定

	int		get_namae_cnt(int scn_no);						// 【名前】の個数を取得
	TSTR	get_namae_str(int scn_no, int name_no);			// 【名前】の文字列を取得

	int		get_read_flag_cnt(int scn_no);					// 既読フラグ数を取得
	int		get_read_flag_line_no(int scn_no, int rf_no);	// 既読フラグの行番号を取得

	bool	jump_to_scn(int scn_no, int z_no);				// シーンにジャンプ
	bool	jump_to_label(int label);						// ラベルにジャンプ
	bool	jump_to_user_cmd(int scn_no, int user_cmd_id);	// ユーザコマンドにジャンプ

	// セーブ／ロード
	bool	load_local(int scn_no, int line_no, int prg_cntr);
	bool	save_call(C_tnm_save_stream& stream);
	bool	load_call(C_tnm_save_stream& stream);

	template <typename TYPE>
	inline TYPE	pop_ret()
	{
		m_cur_ptr += sizeof(TYPE);
		return *(TYPE *)(m_cur_ptr - sizeof(TYPE));
	}

	TSTR	pop_str_ret()
	{
		int str_index = pop_ret<int>();
		return get_string(str_index);
	}

	template <typename TYPE>
	TYPE	pop_str_indirect()
	{
		typedef TYPE::value_type CHAR_TYPE;

		int size = pop_ret<int>() * sizeof(CHAR_TYPE);		// データ長を取得

		cur_ptr += size;
		return TYPE((CHAR_TYPE *)(m_cur_ptr - size), (CHAR_TYPE *)m_cur_ptr);
	}

	TSTR	get_string_with_scn_no(int scn_no, int str_index);
	TSTR	get_string(int str_index);

private:

	// シーンデータ
	BUFFER				m_scn_data;			// シーンデータ
	S_pack_scn_info		m_pack_scn_info;	// パックシーン情報
	ARRAY<S_scn_info>	m_scn_info_list;	// シーン情報リスト

	BYTE*				m_cur_ptr;			// 現在のポインタ
	int					m_cur_scn_no;		// 現在のシーン番号
	int					m_cur_line_no;		// 現在の行番号
};

// ****************************************************************
// シーン数を取得
// ================================================================
inline int C_tnm_scene_lexer::get_scn_cnt()
{
	return m_pack_scn_info.p_header->scn_data_cnt;
}

// ****************************************************************
// 現在のシーン番号を取得
// ================================================================
inline int C_tnm_scene_lexer::get_cur_scn_no()
{
	return m_cur_scn_no;
}

// ****************************************************************
// 現在のシーン番号を設定
// ================================================================
inline void C_tnm_scene_lexer::set_cur_scn_no(int cur_scn_no)
{
	m_cur_scn_no = cur_scn_no;
}

// ****************************************************************
// 現在の行番号を取得
// ================================================================
inline int C_tnm_scene_lexer::get_cur_line_no()
{
	return m_cur_line_no;
}

// ****************************************************************
// 現在の行番号を設定
// ================================================================
inline void C_tnm_scene_lexer::set_cur_line_no(int cur_line_no)
{
	m_cur_line_no = cur_line_no;
}

// ****************************************************************
// 現在のポインタを取得
// ================================================================
inline BYTE* C_tnm_scene_lexer::get_cut_ptr()
{
	return m_cur_ptr;
}

// ****************************************************************
// ユーザインクプロパティの個数を取得
// ================================================================
inline int C_tnm_scene_lexer::get_user_inc_prop_cnt()
{
	return m_pack_scn_info.p_header->inc_prop_cnt;
}

// ****************************************************************
// ユーザシーンプロパティの個数を取得
// ================================================================
inline int C_tnm_scene_lexer::get_user_scn_prop_cnt(int scn_no)
{
	return m_scn_info_list[scn_no].p_header->scn_prop_cnt;
}

// ****************************************************************
// ユーザインクプロパティの型を取得
// ================================================================
inline int C_tnm_scene_lexer::get_user_inc_prop_form(int prop_id)
{
	return m_pack_scn_info.p_inc_prop_list[prop_id].form;
}

// ****************************************************************
// ユーザインクプロパティのサイズを取得（配列の場合）
// ================================================================
inline int C_tnm_scene_lexer::get_user_inc_prop_size(int prop_id)
{
	return m_pack_scn_info.p_inc_prop_list[prop_id].size;
}

// ****************************************************************
// ユーザシーンプロパティの型を取得
// ================================================================
inline int C_tnm_scene_lexer::get_user_scn_prop_form(int scn_no, int prop_id)
{
	return m_scn_info_list[scn_no].p_scn_prop_list[prop_id].form;
}

// ****************************************************************
// ユーザシーンプロパティのサイズを取得（配列の場合）
// ================================================================
inline int C_tnm_scene_lexer::get_user_scn_prop_size(int scn_no, int prop_id)
{
	return m_scn_info_list[scn_no].p_scn_prop_list[prop_id].size;
}
