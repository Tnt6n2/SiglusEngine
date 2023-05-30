#pragma		once

#include	"IA.h"

// ****************************************************************
// 文字解析構造体
// ================================================================
struct S_tnms_ca_data
{
	TSTR	scn_text;

	ARRAY<S_tnms_ia_property>		property_list;	// プロパティ
};

// ****************************************************************
// 文字解析 - Character Analysis -
// ================================================================
class C_tnms_character_analizer
{
public:
	bool	analize_file(CTSTR& in_text, S_tnms_ia_data* piad, S_tnms_ca_data* pcad);
	bool	analize_line(CTSTR& in_text, S_tnms_ia_data* piad, TSTR& out_text);
	int		get_error_line()	{	return error_line;	}
	TSTR	get_error_str()		{	return error_str;	}

private:

	bool	analize_file_1(CTSTR& in_text, TSTR& out_text);
	bool	analize_file_2(CTSTR& in_text, TSTR& out_text, TSTR& inc_text);

	bool	std_replace(TSTR& text, TSTR::iterator& p, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree);
	bool	analize_macro(TSTR::iterator& p, S_tnms_ia_replace* p_macro, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree, TSTR& result_text);
	bool	analize_macro_replace(CTSTR& src_text, ARRAY<S_tnms_ia_macro_arg>& tmp_arg_list, ARRAY<TSTR>& real_arg_list, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree, TSTR& result_text);

	S_tnms_ia_data*	m_piad;			// ia データ
	int				m_line;			// 現在の行番号

	bool	error(int line, CTSTR& str);
	int		error_line;
	TSTR	error_str;
};

