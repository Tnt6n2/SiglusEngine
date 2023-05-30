#pragma		once

#include	"element.h"
#include	"CA.h"

// ****************************************************************
// 字句解析アトムタイプ
// ================================================================
enum TNMS_LA_ATOM_TYPE
{
	#define		DEFINE(str)		str,
	#include	"def_la_atom.h"
	#undef		DEFINE
};

const TSTR TNMS_ATOM_TYPE_STR[] =
{
	#define		DEFINE(str)		_T(#str),
	#include	"def_la_atom.h"
	#undef		DEFINE
};

// ****************************************************************
// 字句解析アトム
// ================================================================
struct S_tnms_atom
{
	int					id;			// ＩＤ（通し番号）
	int					line;		// 行
	TNMS_LA_ATOM_TYPE	type;		// アトムタイプ
	int					opt;		// オプション
	int					subopt;		// オプション

	S_tnms_atom()
	{
		id     = 0;
		line   = 0;
		type   = TNMS_LA_ATOM_NONE;
		opt    = 0;
		subopt = 0;
	}
};

// ****************************************************************
// ラベル構造体
// ================================================================
struct S_tnms_la_label
{
	TSTR	name;
	int		line;

	S_tnms_la_label(CTSTR& name_, int line_)
	{
		name = name_;
		line = line_;
	}

	bool	operator == (const S_tnms_la_label& rhs)
	{
		return name == rhs.name;
	}
};


// ****************************************************************
// 字句解析構造体
// ================================================================
struct S_tnms_la_data
{
	ARRAY<S_tnms_atom>		atom_list;		// アトムリスト
	ARRAY<TSTR>				str_list;		// 文字列リスト
	ARRAY<S_tnms_la_label>	label_list;		// ラベルリスト
	ARRAY<TSTR>				unknown_list;	// 未定義語リスト
};

// ****************************************************************
// 字句解析 - Lexcial Analizer -
// ================================================================
class C_tnms_lexcial_analizer
{
public:
	bool	analize(S_tnms_ca_data* pcad, S_tnms_la_data* plad);

	int		get_error_line()	{	return m_error_line;	}
	TSTR	get_error_str()		{	return m_error_str;		}

	static	TSTR	get_atom_text(S_tnms_atom atom, S_tnms_la_data* plad);

private:
	bool	skip_separator(TSTR::iterator& p, TSTR::iterator end);
	bool	get_label(TSTR::iterator& p, TSTR::iterator end, S_tnms_atom* atom);
	bool	get_integer(TSTR::iterator& p, TSTR::iterator end, S_tnms_atom* atom);
	bool	get_single_quoted_string(TSTR::iterator& p, TSTR::iterator end, S_tnms_atom* atom);
	bool	get_double_quoted_string(TSTR::iterator& p, TSTR::iterator end, S_tnms_atom* atom);
	bool	get_zenkaku_string(TSTR::iterator& p, TSTR::iterator end, S_tnms_atom* atom);
	bool	get_word(TSTR::iterator& p, TSTR::iterator end, S_tnms_atom* atom);

	int				m_cur_atom_id;	// 現在のＩＤ
	int				m_cur_line;		// 現在の行
	S_tnms_ca_data	m_ca_data;		// CA データ
	S_tnms_la_data	m_la_data;		// LA データ

	bool			error(int line, CTSTR& str);	// エラー
	int				m_error_line;					// エラー行
	TSTR			m_error_str;					// エラー内容
};
