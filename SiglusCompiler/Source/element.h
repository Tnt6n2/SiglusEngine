#pragma		once

// ****************************************************************
// def ファイルから取り込み用の構造体
// ================================================================
struct S_tnms_element_def
{
	int		type;		// プロパティ、コマンド
	int		parent;		// 親の型
	int		form;		// 型
	TSTR	name;		// 名前
	BYTE	owner;		// 作成者
	BYTE	group;		// グループ
	WORD	code;		// コード
	TSTR	arg_info;	// 引数情報
};

struct S_tnms_form_def
{
	TSTR	name;
	int		code;
};

// ****************************************************************
// 引数構造体
// ================================================================
struct S_tnms_arg
{
	S_tnms_arg() : def_int(0), def_exist(false)	{}

	int		id;			// ID
	int		form;		// 型
	TSTR	name;		// 名前
	int		def_int;	// 整数型の場合のデフォルト値
	TSTR	def_str;	// 文字列型の場合のデフォルト値
	bool	def_exist;	// デフォルト引数があるかどうか
};

// ****************************************************************
// 引数リスト構造体
// ================================================================
struct S_tnms_arg_list
{
	ARRAY<S_tnms_arg>		arg_list;	// 引数リスト
};

// ****************************************************************
// エレメントタイプ
// ================================================================
enum TNMS_ELEMENT_TYPE
{
	TNMS_ELEMENT_TYPE_PROPERTY,	// プロパティ
	TNMS_ELEMENT_TYPE_COMMAND,	// コマンド
};

// ****************************************************************
// エレメント構造体
// ================================================================
struct S_tnms_element
{
	int		type;		// プロパティ、コマンド
	int		code;		// コード
	TSTR	name;		// 名前
	int		form;		// 型

	void	add_arg_list(int id, S_tnms_arg_list& arg_list);

	std::map<int, S_tnms_arg_list>	arg_map;	// 引数マップ
};

// ****************************************************************
// 型構造体
// ================================================================
struct S_tnms_form
{
	int		code;		// コード
	TSTR	name;		// 名前

	void			clear();
	void			add_element(S_tnms_element* element);
	S_tnms_element*	get_element_by_name(CTSTR& name);
	S_tnms_element*	get_element_by_code(int code);

private:

	std::map<int, S_tnms_element>	element_map_by_code;	// エレメントマップ（コードから）
	std::map<TSTR, S_tnms_element>	element_map_by_name;	// エレメントマップ（名前から）
};

// ****************************************************************
// 型テーブル
// ================================================================
class C_tnms_form_table
{
public:

	void				create_system_form_table();
	void				add_form(S_tnms_form* form);
	S_tnms_form*		get_form_by_name(CTSTR& name);
	S_tnms_form*		get_form_by_code(int code);
	S_tnms_element*		find_element_info(CTSTR& element_name, int *ret_parent_form_code);
	bool				check_element_info(CTSTR& element_name);
	bool				get_element_info(int parent_form_code, TSTR name, S_tnms_element** element_info);

	S_tnms_form		form_call;		// "call" 型デフォルト値

private:

	std::map<int, S_tnms_form>	form_map_by_code;	// 型マップ（コードから）
	std::map<TSTR, S_tnms_form>	form_map_by_name;	// 型マップ（名前から）
};

// ****************************************************************
// 型名 → 型に変換
// ================================================================
int get_form_code_by_name(CTSTR& name);
