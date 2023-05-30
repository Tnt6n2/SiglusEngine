#pragma		once

#include	"data/tnm_cg_table_data.h"
#include	"element/elm_list.h"

// ****************************************************************
// ＣＧテーブル
// ================================================================
class C_tnm_cg_table
{
public:

	bool		init();
	void		save(C_tnm_save_stream& stream);
	void		load(C_tnm_save_stream& stream);

	// ＣＧテーブルデータの読み込み
	bool		load_cg_table_data(TSTR file_name);																	// ＣＧテーブルデータの読み込み

	// ＣＧの総数取得
	int			get_cg_cnt();																						// フラグの総数取得

	// 「見た」フラグの総数取得
	int			get_look_cnt();																						// 「見た」フラグの総数取得

	// 「見た」パーセントの取得
	int			get_look_percent();																					// 「見た」パーセントの取得

	// 全てのフラグ値を設定
	void		set_flag_value_for_all(int flag_value);																// 全てのフラグ値を設定

	// フラグ値を設定（変更前の値が戻ります。存在しない場合、-1 が戻ります。）
	int			set_flag_value_from_name(CTSTR& name, int flag_value);												// 名前からフラグ値を設定（変更前の値が戻ります。存在しない場合、-1 が戻ります。）
	int			set_flag_value_from_list_no(int list_no, int flag_value);											// リスト番号からフラグ値を設定（変更前の値が戻ります。存在しない場合、-1 が戻ります。）
	int			set_flag_value_from_flag_no(int flag_no, int flag_value);											// フラグ番号からフラグ値を設定（変更前の値が戻ります。存在しない場合、-1 が戻ります。）
	int			set_flag_value_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4, int flag_value);		// グループコードからフラグ値を設定（変更前の値が戻ります。存在しない場合、-1 が戻ります。）

	// フラグ値を取得（存在しない場合、false が戻ります。）
	bool		get_flag_value_from_name(CTSTR& name, int *ret_value);												// 名前からフラグ値を取得（存在しない場合、false が戻ります。）
	bool		get_flag_value_from_list_no(int list_no, int *ret_value);											// リスト番号からフラグ値を取得（存在しない場合、false が戻ります。）
	bool		get_flag_value_from_flag_no(int flag_no, int *ret_value);											// フラグ番号からフラグ値を取得（存在しない場合、false が戻ります。）
	bool		get_flag_value_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4, int *ret_value);		// グループコードからフラグ値を取得（存在しない場合、false が戻ります。）

	// 名前を取得（存在しない場合、空の TSTR が戻ります）
	TSTR		get_name_from_list_no(int list_no);																	// リスト番号から名前を取得（存在しない場合、空の TSTR が戻ります）
	TSTR		get_name_from_flag_no(int flag_no);																	// フラグ番号から名前を取得（存在しない場合、空の TSTR が戻ります）
	TSTR		get_name_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4);								// グループコードから名前を取得（存在しない場合、空の TSTR が戻ります）

	// フラグ番号を取得（存在しない場合、-1 が戻ります）
	int			get_flag_no_from_name(CTSTR& name);																	// 名前からフラグ番号を取得（存在しない場合、-1 が戻ります）
	int			get_flag_no_from_list_no(int list_no);																// リスト番号からフラグ番号を取得（存在しない場合、-1 が戻ります）
	int			get_flag_no_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4);							// グループコードからフラグ番号を取得（存在しない場合、-1 が戻ります）

	// １つのコード値を取得（存在しない場合、-1 が戻ります）
	int			get_one_code_value_from_name(CTSTR& name, int get_code_no);											// 名前から１つのコード値を取得（存在しない場合、-1 が戻ります）
	int			get_one_code_value_from_list_no(int list_no, int get_code_no);										// リスト番号から１つのコード値を取得（存在しない場合、-1 が戻ります）
	int			get_one_code_value_from_flag_no(int flag_no, int get_code_no);										// フラグ番号から１つのコード値を取得（存在しない場合、-1 が戻ります）
	int			get_one_code_value_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4, int get_code_no);	// グループコードから１つのコード値を取得（存在しない場合、-1 が戻ります）

	// 全コード値を取得（存在しない場合、空の ARRAY<int> が戻ります）
	ARRAY<int>	get_all_code_value_from_name(CTSTR& name);															// 名前から全コード値を取得（存在しない場合、空の ARRAY<int> が戻ります）
	ARRAY<int>	get_all_code_value_from_list_no(int list_no);														// リスト番号から全コード値を取得（存在しない場合、空の ARRAY<int> が戻ります）
	ARRAY<int>	get_all_code_value_from_flag_no(int flag_no);														// フラグ番号から全コード値を取得（存在しない場合、空の ARRAY<int> が戻ります）
	ARRAY<int>	get_all_code_value_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4);					// グループコードから全コード値を取得（存在しない場合、空の ARRAY<int> が戻ります）

	// グループコードからグループのメンバー数を取得
	int			get_group_member_cnt(int gc0, int gc1, int gc2, int gc3, int gc4);									// グループコードからグループのメンバー数を取得

	// グループコードからグループの「見た」メンバー数を取得
	int			get_group_member_cnt_for_look(int cg0, int cg1, int cg2, int cg3, int cg4);							// グループコードからグループの「見た」メンバー数を取得

	// グループコードからグループを一括してフラグ値を設定
	void		set_flag_value_for_group_member(int cg0, int cg1, int cg2, int cg3, int cg4, int flag_value);		// グループコードからグループを一括してフラグ値を設定

	// ＣＧテーブルデータへのポインタを取得する
	C_cg_table_data*	get_ptr_to_cg_table_data()	{	return &cg_table;	}

	// フラグへのポインタを取得する
	C_elm_int_list*		get_ptr_to_flag()			{	return &flag;	}

private:

	C_cg_table_data		cg_table;
	C_elm_int_list		flag;
};


