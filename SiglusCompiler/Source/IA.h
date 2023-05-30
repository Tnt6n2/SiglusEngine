#pragma		once

#include	"code.h"
#include	"element.h"

// ****************************************************************
// inc ファイル解析
//		*.inc ファイルを解析するクラスを宣言します。
// ================================================================

// ****************************************************************
// マクロの引数
// ================================================================
struct S_tnms_ia_macro_arg
{
	TSTR	name;			// 名前
	TSTR	def_value;		// デフォルト値
};

// ****************************************************************
// 置換タイプ
// ================================================================
enum TNMS_IA_REPLACE_TYPE
{
	TNMS_IA_REPLACE_TYPE_NONE,
	TNMS_IA_REPLACE_TYPE_REPLACE,		// replace
	TNMS_IA_REPLACE_TYPE_DEFINE,		// define
	TNMS_IA_REPLACE_TYPE_MACRO,			// macro
};

// ****************************************************************
// 置換構造体
// ================================================================
struct S_tnms_ia_replace
{
	TNMS_IA_REPLACE_TYPE		type;		// 置換タイプ
	TSTR						name;		// 名前
	TSTR						after;		// 置換後の文字列
	ARRAY<S_tnms_ia_macro_arg>	arg_list;	// 引数リスト（マクロのみ）
};

// ****************************************************************
// 置換を並び替えるための比較関数
// ================================================================
//		長さのみで判定します。
// ================================================================
inline bool G_tnms_ia_replace_cmp_func(BSP<S_tnms_ia_replace>& lhs, BSP<S_tnms_ia_replace>& rhs)
{
	return lhs->name.length() < rhs->name.length();
}

// ****************************************************************
// 置換ツリー用ノード
// ================================================================
struct S_replace_node
{
	void	clear()
	{
		child_map.clear();
		p_replace.reset();
	}

	std::map<TCHAR, S_replace_node>	child_map;
	BSP<S_tnms_ia_replace>			p_replace;
};

// ****************************************************************
// 置換ツリー
// ================================================================
class C_replace_tree
{
public:
	static	void	create_replace_tree(S_replace_node& root, ARRAY< BSP<S_tnms_ia_replace> >& replace_list);
	static	void	add_replace(S_replace_node& node, BSP<S_tnms_ia_replace> p_replace, int index);
	static	void	search_replace(S_replace_node& node, TSTR::const_iterator p, BSP<S_tnms_ia_replace>& ret_replace);
};

// ****************************************************************
// プロパティ構造体
// ================================================================
struct S_tnms_ia_property
{
	int				id;				// プロパティID
	int				form;			// 型
	int				size;			// 配列の場合のサイズ
	TSTR			name;			// 名前
};

// ****************************************************************
// コマンド構造体
// ================================================================
struct S_tnms_ia_command
{
	S_tnms_ia_command() : is_defined(false)	{}

	int				id;				// コマンドID（通し番号）
	int				form;			// 型
	TSTR			name;			// 名前
	S_tnms_arg_list	arg_list;		// 引数リスト
	bool			is_defined;		// 定義されたかどうか
};

// ****************************************************************
// 宣言タイプ
// ================================================================
enum TNMS_IA_DECLARE_TYPE
{
	TNMS_IA_DECLARE_TYPE_NONE,
	TNMS_IA_DECLARE_TYPE_REPLACE,
	TNMS_IA_DECLARE_TYPE_DEFINE,
	TNMS_IA_DECLARE_TYPE_DEFINE_SPACE,
	TNMS_IA_DECLARE_TYPE_MACRO,
	TNMS_IA_DECLARE_TYPE_PROPERTY,
	TNMS_IA_DECLARE_TYPE_COMMAND,
	TNMS_IA_DECLARE_TYPE_EXPAND,
};

// ****************************************************************
// 宣言構造体
// ================================================================
struct S_tnms_ia_declare
{
	TNMS_IA_DECLARE_TYPE	type;
	BSP<S_tnms_ia_replace>	replace;	// 置換（replace, define, macro）
	BSP<S_tnms_ia_property>	Property;	// プロパティ
	BSP<S_tnms_ia_command>	command;	// コマンド
};

// ****************************************************************
// inc エラー定数
// ================================================================
enum TNSMERR_IA
{
	TNMSERR_IA_NONE,
	TNMSERR_IA_NEWLINE_IN_SQUOTE,
	TNMSERR_IA_NEWLINE_IN_DQUOTE,
	TNMSERR_IA_SQUOTE_NO_CLOSE,
	TNMSERR_IA_DQUOTE_NO_CLOSE,
	TNMSERR_IA_COMMENT_NO_CLOSE,
	TNMSERR_IA_MISS_DECLARE,
	TNMSERR_IA_MISS_REPLACE,
	TNMSERR_IA_MISS_DEFINE,
	TNMSERR_IA_MISS_MACRO,
	TNMSERR_IA_MISS_COMMAND,
	TNMSERR_IA_MISS_PROPERTY,
	TNMSERR_IA_MACRO_NO_NAME,
	TNMSERR_IA_MACRO_NO_ARG_LIST,
	TNMSERR_IA_MACRO_NO_AFTER,
	TNMSERR_IA_MACRO_ARG_LIST_NO_OPEN,
	TNMSERR_IA_MACRO_ARG_LIST_NO_CLOSE,
	TNMSERR_IA_MACRO_ARG_LIST_NO_ARG,
	TNMSERR_IA_MACRO_ARG_LIST_MISS_ARG,
	TNMSERR_IA_PROPERTY_MISS_FORM,
	TNMSERR_IA_COMMAND_MISS_FORM,
	TNMSERR_IA_RE_DECRARE,
	TNMSERR_IA_MISS_FORM,
};

// ****************************************************************
// インクデータ構造体
// ================================================================
//		全インクデータ共通のデータです。
//		全インクデータで共通して１つだけ作成して下さい。
// ================================================================
struct S_tnms_ia_data
{
	void	init();

	S_replace_node						replace_tree;	// 置換ツリー
	ARRAY< BSP<S_tnms_ia_property> >	property_list;	// プロパティ
	ARRAY< BSP<S_tnms_ia_command> >		command_list;	// コマンド
	std::set<TSTR>						name_set;		// 重複判定用の名前セット

	C_tnms_form_table	form_table;				// 型テーブル
	int					property_cnt;			// プロパティ数（inc + scn）
	int					command_cnt;			// コマンド数（inc + scn）
	int					inc_property_cnt;		// プロパティ数（inc）
	int					inc_command_cnt;		// コマンド数（inc）
};

// ****************************************************************
// インクデータ構造体２
// ================================================================
//		各インクデータ共通のデータです。
//		インクデータごとに１つずつ作成して下さい。
// ================================================================
struct S_tnms_ia_data_2
{
	void	init();

	ARRAY<TSTR>			property_temp_list;	// 解析前のプロパティリスト
	ARRAY<int>			property_line_list;	// 解析前のプロパティリスト
	ARRAY<TSTR>			command_temp_list;	// 解析前のコマンドリスト
	ARRAY<int>			command_line_list;	// 解析前のコマンドリスト
};

// ****************************************************************
// inc analizer
// ================================================================
class C_inc_file_analizer
{
public:
	bool	analize_step_1(CTSTR& text, int parent_form, S_tnms_ia_data* piad, S_tnms_ia_data_2* piad2);		// parent_form には、FM_GLOBAL か FM_SCENE を指定します
	bool	analize_step_2(S_tnms_ia_data* piad, S_tnms_ia_data_2* piad2);

	TSTR	get_error_str();
	int		get_error_line();

private:

	bool	ia_comment_cut();
	bool	ia_main_step_1();
	bool	ia_main_step_2();

	bool	ia_skip_separator(TSTR::const_iterator& p, int* line_ret);
	bool	ia_declare(TSTR::const_iterator& p, int* line_ret, BSP<S_tnms_ia_declare>& declare);
	bool	ia_declare_type(TSTR::const_iterator& p, int* line_ret, TNMS_IA_DECLARE_TYPE* type);
	bool	ia_define_name(TSTR::const_iterator& p, int* line_ret, TSTR& name);
	bool	ia_define_space_name(TSTR::const_iterator& p, int* line_ret, TSTR& name);
	bool	ia_define_after(TSTR::const_iterator& p, int* line_ret, TSTR& after);
	bool	ia_macro_name(TSTR::const_iterator& p, int* line_ret, TSTR& name);
	bool	ia_macro_arg_list(TSTR::const_iterator& p, int* line_ret, ARRAY<S_tnms_ia_macro_arg>& arg_list);
	bool	ia_macro_arg(TSTR::const_iterator& p, int* line_ret, S_tnms_ia_macro_arg& arg);
	bool	ia_macro_after(TSTR::const_iterator& p, int* line_ret, TSTR& after);
	bool	ia_property(TSTR::const_iterator& p_ret, int* line_ret, TSTR& name_ret);
	bool	ia_command(TSTR::const_iterator& p_ret, int* line_ret, TSTR& name_ret);

	bool	ia_property_name(TSTR::const_iterator& p, int* line_ret, TSTR& name);
	bool	ia_command_name(TSTR::const_iterator& p, int* line_ret, TSTR& name);
	bool	ia_command_arg_list(TSTR::const_iterator& p, int* line_ret, S_tnms_arg_list& arg_list);
	bool	ia_command_arg(TSTR::const_iterator& p, int* line_ret, S_tnms_arg& arg);
	bool	ia_declare_property_form(TSTR::const_iterator& p, int* line_ret, int* form_ret, int* size_ret);
	bool	ia_declare_form(TSTR::const_iterator& p, int* line_ret, int* form_ret);
	bool	ia_form(TSTR::const_iterator& p, int error_line, int* line_ret, int* form_ret);
	bool	ia_array_size(TSTR::const_iterator& p_ret, int* line_ret, int* size_ret);

	// マクロの解析
	bool	std_replace(TSTR& text, TSTR::const_iterator& p, int* line_ret, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree);
	bool	analize_macro(TSTR& text, TSTR::const_iterator& p, int* line_ret, S_tnms_ia_replace* p_macro, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree, TSTR& result_text);
	bool	analize_macro_replace(TSTR& text, int* line_ret, CTSTR& src_text, ARRAY<S_tnms_ia_macro_arg>& tmp_arg_list, ARRAY<TSTR>& real_arg_list, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree, TSTR& result_text);


	S_tnms_ia_data*			m_piad;
	S_tnms_ia_data_2*		m_piad2;
	TSTR					m_text;
	int						m_parent_form;

	bool		error(int line, CTSTR& error_str);
	int			m_error_line;
	TSTR		m_error_str;
};




