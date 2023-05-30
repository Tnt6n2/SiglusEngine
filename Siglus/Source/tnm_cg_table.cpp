#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_cg_table.h"
#include	"data/tnm_save_00.h"
#include	"engine/eng_dir.h"
#include	"engine/ifc_error.h"

// ****************************************************************
// ＣＧテーブル：初期化
// ================================================================
bool C_tnm_cg_table::init()
{
	// ＣＧテーブルを読み込む
	return load_cg_table_data(Gp_ini->cg_table_file);
}

// ****************************************************************
// ＣＧテーブル：セーブ
// ================================================================
void C_tnm_cg_table::save(C_tnm_save_stream& stream)
{
	flag.save(stream);
}

// ****************************************************************
// ＣＧテーブル：ロード
// ================================================================
void C_tnm_cg_table::load(C_tnm_save_stream& stream)
{
	flag.load(stream);
}

// ****************************************************************
// ＣＧテーブル：ファイルを読み込む
// ================================================================
bool C_tnm_cg_table::load_cg_table_data(TSTR file_name)
{
	if (file_name.empty())
		return true;

	file_name = change_extension_ret(file_name, _T("cgm"));

	TSTR file_path = tnm_find_dat(file_name);
	if (!cg_table.load(file_path))
		return tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("ＣＧテーブル ") + file_name + _T(" が見つかりませんでした。"));

	// フラグを確保する（固定長）
	flag.init(S_element(), _T("cg_table.flag"), Gp_ini->cg_table_flag_cnt, false);

	return true;
}

// ****************************************************************
// ＣＧテーブル：フラグの総数取得
// ================================================================
int C_tnm_cg_table::get_cg_cnt()
{
	return (int)cg_table.cg_table_data_list.size();
}

// ****************************************************************
// ＣＧテーブル：「見た」フラグの総数取得
// ================================================================
int C_tnm_cg_table::get_look_cnt()
{
	int look_cnt = 0;
	int list_cnt = get_cg_cnt();
	for (int i = 0; i < list_cnt; i++)	{
		S_cg_table_data_sub* sub = cg_table.get_sub_pointer_from_list_no_func(i);
		if (sub)	{
			if (flag.get_value(sub->flag_no) == 1)	{
				look_cnt ++;
			}
		}
	}

	return look_cnt;
}

// ****************************************************************
// ＣＧテーブル：「見た」パーセントの取得
// ================================================================
int C_tnm_cg_table::get_look_percent()
{
	int cg_cnt = get_cg_cnt();
	if (cg_cnt == 0)
		return 0;		// 登録がない

	int look_cnt = get_look_cnt();
	if (look_cnt == 0)
		return 0;		// １枚も見ていない

	int percent = look_cnt * 100 / cg_cnt;
	if (percent == 0)
		percent = 1;	// １枚でも見ていたら最低 1% に補正する

	return percent;
}

// ****************************************************************
// ＣＧテーブル：全てのフラグ値を設定
// ================================================================
void C_tnm_cg_table::set_flag_value_for_all(int flag_value)
{
	int list_cnt = get_cg_cnt();
	for (int i = 0; i < list_cnt; i++)	{
		S_cg_table_data_sub* sub = cg_table.get_sub_pointer_from_list_no_func(i);
		if (sub)	{
			flag.set_value(sub->flag_no, flag_value);
		}
	}
}

// ****************************************************************
// ＣＧテーブル：フラグ値を設定（変更前の値が戻ります。存在しない場合、-1 が戻ります。）
// ================================================================

// 名前から
int C_tnm_cg_table::set_flag_value_from_name(CTSTR& name, int flag_value)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_name_func(name);
	if (!sub)	{	return -1;	}

	// フラグを設定
	int now_value = flag.get_value(sub->flag_no);
	flag.set_value(sub->flag_no, flag_value);

	return now_value;
}

// リスト番号から
int C_tnm_cg_table::set_flag_value_from_list_no(int list_no, int flag_value)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_list_no_func(list_no);
	if (!sub)	{	return -1;	}

	// フラグを設定
	int now_value = flag.get_value(sub->flag_no);
	flag.set_value(sub->flag_no, flag_value);

	return now_value;
}

// フラグ番号から
int C_tnm_cg_table::set_flag_value_from_flag_no(int flag_no, int flag_value)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_flag_no_func(flag_no);
	if (!sub)	{	return -1;	}

	// フラグを設定
	int now_value = flag.get_value(sub->flag_no);
	flag.set_value(sub->flag_no, flag_value);

	return now_value;
}

// グループコードから
int C_tnm_cg_table::set_flag_value_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4, int flag_value)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_group_code_func(gc0, gc1, gc2, gc3, gc4);
	if (!sub)	{	return -1;	}

	// フラグを設定
	int now_value = flag.get_value(sub->flag_no);
	flag.set_value(sub->flag_no, flag_value);

	return now_value;
}

// ****************************************************************
// ＣＧテーブル：フラグ値を取得（存在しない場合、false が戻ります。）
// ================================================================

// 名前から
bool C_tnm_cg_table::get_flag_value_from_name(CTSTR& name, int *ret_value)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_name_func(name);
	if (!sub)	{	return false;	}

	*ret_value = flag.get_value(sub->flag_no);

	return true;
}

// リスト番号から
bool C_tnm_cg_table::get_flag_value_from_list_no(int list_no, int *ret_value)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_list_no_func(list_no);
	if (!sub)	{	return false;	}

	*ret_value = flag.get_value(sub->flag_no);

	return true;
}

// フラグ番号から
bool C_tnm_cg_table::get_flag_value_from_flag_no(int flag_no, int *ret_value)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_flag_no_func(flag_no);
	if (!sub)	{	return false;	}

	*ret_value = flag.get_value(sub->flag_no);

	return true;
}

// グループコードから
bool C_tnm_cg_table::get_flag_value_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4, int *ret_value)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_group_code_func(gc0, gc1, gc2, gc3, gc4);
	if (!sub)	{	return false;	}

	*ret_value = flag.get_value(sub->flag_no);

	return true;
}

// ****************************************************************
// ＣＧテーブル：名前を取得（存在しない場合、空の TSTR が戻ります）
// ================================================================

// リスト番号から
TSTR C_tnm_cg_table::get_name_from_list_no(int list_no)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_list_no_func(list_no);
	return (sub) ? sub->name : _T("");
}

// フラグ番号から
TSTR C_tnm_cg_table::get_name_from_flag_no(int flag_no)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_flag_no_func(flag_no);
	return (sub) ? sub->name : _T("");
}

// グループコードから
TSTR C_tnm_cg_table::get_name_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_group_code_func(gc0, gc1, gc2, gc3, gc4);
	return (sub) ? sub->name : _T("");
}

// ****************************************************************
// ＣＧテーブル：フラグ番号を取得（存在しない場合、-1 が戻ります）
// ================================================================

// 名前から
int C_tnm_cg_table::get_flag_no_from_name(CTSTR& name)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_name_func(name);
	return (sub) ? sub->flag_no : -1;
}

// リスト番号から
int C_tnm_cg_table::get_flag_no_from_list_no(int list_no)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_list_no_func(list_no);
	return (sub) ? sub->flag_no : -1;
}

// グループコードから
int C_tnm_cg_table::get_flag_no_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4)
{
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_group_code_func(gc0, gc1, gc2, gc3, gc4);
	return (sub) ? sub->flag_no : -1;
}

// ****************************************************************
// ＣＧテーブル：１つのコード値を取得（存在しない場合、-1 が戻ります）
// ================================================================

// 名前から
int C_tnm_cg_table::get_one_code_value_from_name(CTSTR& name, int get_code_no)
{
	if (!(get_code_no >= 0 && get_code_no < CG_TABLE_DATA_CODE_MAX))	{	return -1;	}
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_name_func(name);
	return (sub) ? sub->code[get_code_no] : -1;
}

// リスト番号から
int C_tnm_cg_table::get_one_code_value_from_list_no(int list_no, int get_code_no)
{
	if (!(get_code_no >= 0 && get_code_no < CG_TABLE_DATA_CODE_MAX))	{	return -1;	}
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_list_no_func(list_no);
	return (sub) ? sub->code[get_code_no] : -1;
}

// フラグ番号から
int C_tnm_cg_table::get_one_code_value_from_flag_no(int flag_no, int get_code_no)
{
	if (!(get_code_no >= 0 && get_code_no < CG_TABLE_DATA_CODE_MAX))	{	return -1;	}
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_flag_no_func(flag_no);
	return (sub) ? sub->code[get_code_no] : -1;
}

// グループコードから
int C_tnm_cg_table::get_one_code_value_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4, int get_code_no)
{
	if (!(get_code_no >= 0 && get_code_no < CG_TABLE_DATA_CODE_MAX))	{	return -1;	}
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_group_code_func(gc0, gc1, gc2, gc3, gc4);
	return (sub) ? sub->code[get_code_no] : -1;
}

// ****************************************************************
// ＣＧテーブル：全コード値を取得（存在しない場合、空の ARRAY<int> が戻ります）
// ================================================================

// 名前から
ARRAY<int> C_tnm_cg_table::get_all_code_value_from_name(CTSTR& name)
{
	ARRAY<int> code;
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_name_func(name);
	if (sub)	{
		code.resize(CG_TABLE_DATA_CODE_MAX);
		CopyMemory(code.get(), sub->code, CG_TABLE_DATA_CODE_MAX * 4); 
	}
	return code;
}

// リスト番号から
ARRAY<int> C_tnm_cg_table::get_all_code_value_from_list_no(int list_no)
{
	ARRAY<int> code;
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_list_no_func(list_no);
	if (sub)	{
		code.resize(CG_TABLE_DATA_CODE_MAX);
		CopyMemory(code.get(), sub->code, CG_TABLE_DATA_CODE_MAX * 4); 
	}
	return code;
}

// フラグ番号から
ARRAY<int> C_tnm_cg_table::get_all_code_value_from_flag_no(int flag_no)
{
	ARRAY<int> code;
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_flag_no_func(flag_no);
	if (sub)	{
		code.resize(CG_TABLE_DATA_CODE_MAX);
		CopyMemory(code.get(), sub->code, CG_TABLE_DATA_CODE_MAX * 4); 
	}
	return code;
}

// グループコードから
ARRAY<int> C_tnm_cg_table::get_all_code_value_from_group_code(int gc0, int gc1, int gc2, int gc3, int gc4)
{
	ARRAY<int> code;
	S_cg_table_data_sub *sub = cg_table.get_sub_pointer_from_group_code_func(gc0, gc1, gc2, gc3, gc4);
	if (sub)	{
		code.resize(CG_TABLE_DATA_CODE_MAX);
		CopyMemory(code.get(), sub->code, CG_TABLE_DATA_CODE_MAX * 4); 
	}
	return code;
}

// ****************************************************************
// ＣＧテーブル：グループコードからグループのメンバー数を取得
// ================================================================
int C_tnm_cg_table::get_group_member_cnt(int gc0, int gc1, int gc2, int gc3, int gc4)
{
	S_cg_table_group_tree *group = cg_table.get_groupe_tree_pointer_func(gc0, gc1, gc2, gc3, gc4);
	return (group) ? (int)group->tree.size() : 0;
}

// ****************************************************************
// ＣＧテーブル：グループコードからグループの「見た」メンバー数を取得
// ================================================================
int C_tnm_cg_table::get_group_member_cnt_for_look(int cg0, int cg1, int cg2, int cg3, int cg4)
{
	ARRAY<int> flag_list;
	cg_table.get_flag_list_func(cg0, cg1, cg2, cg3, cg4, flag_list);
	int look_cnt = 0;
	for (int i = 0; i < (int)flag_list.size(); i++)	{
		// ▲ＣＧテーブル：ここでフラグの値をチェックして look_cnt++ する
	}
	return look_cnt;
}

// ****************************************************************
// ＣＧテーブル：グループコードからグループのメンバーを一括でフラグ値を設定
// ================================================================
void C_tnm_cg_table::set_flag_value_for_group_member(int cg0, int cg1, int cg2, int cg3, int cg4, int flag_value)
{
	ARRAY<int> flag_list;
	cg_table.get_flag_list_func(cg0, cg1, cg2, cg3, cg4, flag_list);
	for (int i = 0; i < (int)flag_list.size(); i++)	{
		// ▲ＣＧテーブル：ここでフラグの値を設定する
	}
}

#if 0		// ▲後で削除する RealLive のオリジナルソース

/*=============================================================================
	ＣＧテーブル フラグ番号エラー
void avg_cg_table_flag_no_err_func(UC *name, int flag_no);
-----------------------------------------------------------------------------*/
void avg_cg_table_flag_no_err_func(UC *name, int flag_no){
	if (name)	{
		if (Gavg_ini.cg_table_mod)	{	format(Gavg_err_mess, (UC *)"%s : Z[%05d]", name, flag_no);	}		// １ビットフラグモード
		else						{	format(Gavg_err_mess, (UC *)"%s : Z[%04d]", name, flag_no);	}		// ３２ビットフラグモード
	}
	else	{
		UC name_str[MAXX_PATH];
		if (avg_cg_table_get_name_str_from_flag_no(flag_no, name_str))	{
			if (Gavg_ini.cg_table_mod)	{	format(Gavg_err_mess, (UC *)"%s : Z[%05d]", name_str, flag_no);	}		// １ビットフラグモード
			else						{	format(Gavg_err_mess, (UC *)"%s : Z[%04d]", name_str, flag_no);	}		// ３２ビットフラグモード
		}
		else	{
			if (Gavg_ini.cg_table_mod)	{	format(Gavg_err_mess, (UC *)"UnKnownName : Z[%05d]", name, flag_no);	}		// １ビットフラグモード
			else						{	format(Gavg_err_mess, (UC *)"UnKnownName : Z[%04d]", name, flag_no);	}		// ３２ビットフラグモード
		}
	}
	avg_dbwin11_message((UC *)"■ＥＲＲ：ＣＧテーブルのフラグ番号が異常です。", Gavg_err_mess, AVGDBM_SE_WARNING, AVGDBM_TYPE_NONE);
}

/***********************************************************************************************************/
/***********************************************************************************************************/
/***********************************************************************************************************/

/*=============================================================================
	ＣＧテーブル フラグ値の設定（フラグ番号から）
int avg_cg_table_set_flag_value(int flag_no, int nom);
-----------------------------------------------------------------------------*/
int avg_cg_table_set_flag_value(int flag_no, int nom){
	int alpha = 0;
	if (Gavg_ini.cg_table_mod)	{		// １ビットフラグモード
		alpha = 26;
		if (nom != 0)	{	nom = 1;	}
	}
	return avg_math_set_flag(('Z'-'A') + alpha, flag_no, nom, &Gavg_save.flag);
}

/*=============================================================================
	ＣＧテーブル フラグ値の取得（フラグ番号から）
int avg_cg_table_get_flag_value(int flag_no, int *ret_nom);
-----------------------------------------------------------------------------*/
int avg_cg_table_get_flag_value(int flag_no, int *ret_nom){
	int alpha = 0;
	if (Gavg_ini.cg_table_mod)	{		// １ビットフラグモード
		alpha = 26;
	}
	return avg_math_get_flag(('Z'-'A') + alpha, flag_no, ret_nom, &Gavg_save.flag);
}

#endif
