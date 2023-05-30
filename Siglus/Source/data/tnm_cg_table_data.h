#pragma		once

// ****************************************************************
// ＣＧテーブルデータ
// ================================================================

const int	CG_TABLE_DATA_CODE_MAX = 5;
const int	CG_TABLE_DATA_CODE_VALUE_MAX = 999;

// ＣＧテーブルデータ構造体
struct S_cg_table_data_sub
{
	TSTR		name;
	int			flag_no;
	int			code_exist_cnt;
	int			code[CG_TABLE_DATA_CODE_MAX];
	int			list_no;
	int			group[CG_TABLE_DATA_CODE_MAX];
};

// ＣＧテーブル名前検索マップ
typedef		std::pair<TSTR, S_cg_table_data_sub*>	CG_TABLE_NAME_FIND_PAIR;
typedef		std::map<TSTR, S_cg_table_data_sub*>	CG_TABLE_NAME_FIND_MAP;

// ＣＧテーブルフラグ検索マップ
typedef		std::pair<int, S_cg_table_data_sub*>	CG_TABLE_FLAG_FIND_PAIR;
typedef		std::map<int, S_cg_table_data_sub*>		CG_TABLE_FLAG_FIND_MAP;

// ＣＧテーブルグループツリー
struct S_cg_table_group_tree
{
	S_cg_table_data_sub *sub;
	ARRAY<S_cg_table_group_tree> tree;

	S_cg_table_group_tree()	{	sub = NULL;	}		// コンストラクタ
};

// 本体
class C_cg_table_data
{
public:

	ARRAY<S_cg_table_data_sub>		cg_table_data_list;			// ＣＧテーブルデータリスト
	CG_TABLE_NAME_FIND_MAP			cg_table_name_find_map;		// ＣＧテーブル名前検索マップ
	CG_TABLE_FLAG_FIND_MAP			cg_table_flag_find_map;		// ＣＧテーブルフラグ検索マップ
	ARRAY<S_cg_table_data_sub *>	cg_table_sort_list;			// ＣＧテーブルソートリスト
	S_cg_table_group_tree			cg_table_group_tree;		// ＣＧテーブルグループツリー

	// 読み込み
	bool		load(CTSTR& file_path);											// 読み込み

	// ＣＧテーブルデータ構造体のポインタ取得
	S_cg_table_data_sub * get_sub_pointer_from_name_func(CTSTR& name);											// 名前から取得
	S_cg_table_data_sub * get_sub_pointer_from_list_no_func(int list_no);										// リスト番号から取得
	S_cg_table_data_sub * get_sub_pointer_from_flag_no_func(int flag_no);										// フラグ番号から取得
	S_cg_table_data_sub * get_sub_pointer_from_group_code_func(int gc0, int gc1, int gc2, int gc3, int gc4);		// グループコードから取得

	// ＣＧテーブルグループツリーのポインタ取得
	S_cg_table_group_tree * get_groupe_tree_pointer_func(int cg0, int cg1, int cg2, int cg3, int cg4);

	// グループコードからフラグリスト取得する（存在しない場合、ARRAY<int> は空になります）
	void		get_flag_list_func(int gc0, int gc1, int gc2, int gc3, int gc4, ARRAY<int>& flag_list);

private:

	// 読み込み時の処理関数
	bool	expand(ARRAY<BYTE>& buf);		// 解凍
	bool	create_name_find_map();			// 名前検索マップ作成
	bool	create_flag_find_map();			// フラグ検索マップ作成
	bool	create_sort_list();				// ソートリスト作成
	bool	create_group_tree();			// グループツリー作成
	void	create_group_tree_func(S_cg_table_group_tree& group_tree, int sort_list_index, int *code, int code_index);		// グループツリー作成ファンクション

	// 「ＣＧテーブルグループツリーのポインタ取得」の再起関数
	S_cg_table_group_tree * get_groupe_tree_pointer_funcfunc(S_cg_table_group_tree *group, int *code, int code_index);

	// 「グループコードからフラグリスト取得する」の再起関数
	void		get_flag_list_funcfunc(S_cg_table_group_tree *group, ARRAY<int>& flag_list);

};


