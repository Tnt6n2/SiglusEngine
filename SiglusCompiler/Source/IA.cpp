#include	"pch.h"
#include	"IA.h"
#include	"CA.h"

// ****************************************************************
// 文字列解析：文字列から単語を取得（拡張版）
// ================================================================
bool str_analize_get_word_ex(TSTR::const_iterator& itr_, TSTR::const_iterator end, TSTR& word)
{
	TSTR::const_iterator itr = itr_;

	// １文字目は英字または '_'
	if (itr == end || (!is_alpha(*itr) && !is_zenkaku(*itr) && *itr != _T('_') && *itr != _T('@')))
		return false;

	++itr;

	// ２文字目以降は英数字または '_'
	for (; itr != end; ++itr)	{
		if (!is_alpha(*itr) && !is_num(*itr) && !is_zenkaku(*itr) && *itr != _T('_') && *itr != _T('@'))
			break;
	}

	word = TSTR(itr_, itr);
	itr_ = itr;
	return true;
}


// ****************************************************************
// 置換ツリー：置換ツリーを作成
// ================================================================
void C_replace_tree::create_replace_tree(S_replace_node& root, ARRAY< BSP<S_tnms_ia_replace> >& replace_list)
{
	for (ARRAY< BSP<S_tnms_ia_replace> >::iterator itr = replace_list.begin(); itr != replace_list.end(); ++itr)
		add_replace(root, *itr, 0);
}

// ****************************************************************
// 置換ツリー：置換を追加
// ================================================================
void C_replace_tree::add_replace(S_replace_node& node, BSP<S_tnms_ia_replace> p_replace, int index)
{
	// 名前の終端に来た場合、データを設定する
	if (index == p_replace->name.size())	{
		node.p_replace = p_replace;
	}
	// 名前がまだ続く場合、子供マップに進む
	else	{
		TCHAR moji = p_replace->name[index];
		add_replace(node.child_map[moji], p_replace, index + 1);
	}
}

// ****************************************************************
// 置換ツリー：置換を検索
// ================================================================
void C_replace_tree::search_replace(S_replace_node& node, TSTR::const_iterator p, BSP<S_tnms_ia_replace>& ret_replace)
{
	// 文字列
	if (*p == _T('\0'))	{
		ret_replace = node.p_replace;
	}
	else	{

		std::map<TCHAR, S_replace_node>::iterator itr = node.child_map.find(*p);
		if (itr != node.child_map.end())	{
			search_replace(itr->second, p + 1, ret_replace);
			if (!ret_replace)	{
				ret_replace = node.p_replace;
			}
		}
		else	{
			ret_replace = node.p_replace;
		}
	}
}

// ****************************************************************
// inc ファイル解析データ：初期化
// ================================================================
void S_tnms_ia_data::init()
{
	replace_tree.clear();
	property_list.clear();
	command_list.clear();
	name_set.clear();

	// デフォルトの型テーブルを作成
	form_table.create_system_form_table();

	// エレメント数の初期化
	property_cnt = 0;
	command_cnt = 0;
	inc_property_cnt = 0;
	inc_command_cnt = 0;
}

// ****************************************************************
// inc ファイル解析データ２：初期化
// ================================================================
void S_tnms_ia_data_2::init()
{
	property_temp_list.clear();
	property_line_list.clear();
	command_temp_list.clear();
	command_line_list.clear();
}

// ****************************************************************
// inc ファイル解析
// ================================================================
bool C_inc_file_analizer::analize_step_1(CTSTR& text, int parent_form, S_tnms_ia_data* piad, S_tnms_ia_data_2* piad2)
{
	m_text = text;
	m_parent_form = parent_form;
	m_piad = piad;
	m_piad2 = piad2;

	// コメントカット（さらに小文字に変換します）
	if (!ia_comment_cut())
		return false;

	// メインステップ１
	if (!ia_main_step_1())
		return false;

	return true;
}

bool C_inc_file_analizer::analize_step_2(S_tnms_ia_data* piad, S_tnms_ia_data_2* piad2)
{
	m_piad = piad;
	m_piad2 = piad2;

	// メインステップ２
	if (!ia_main_step_2())
		return false;

	return true;
}

// ****************************************************************
// inc 解析：コメントカット
// ================================================================
bool C_inc_file_analizer::ia_comment_cut()
{
	TSTR out_text;

	// エラーの初期化
	m_error_line = -1;
	m_error_str.clear();

	enum STATE
	{
		STATE_NONE,
		STATE_SINGLE_QUOTE,			// シングルコーテーション
		STATE_SINGLE_QUOTE_ESCAPE,	// シングルコーテーションエスケープ
		STATE_DOUBLE_QUOTE,			// ダブルコーテーション
		STATE_DOUBLE_QUOTE_ESCAPE,	// ダブルコーテーションエスケープ
		STATE_COMMENT_LINE,			// 行コメント
		STATE_COMMENT_BLOCK,		// ブロックコメント
	};
	
	STATE state = STATE_NONE;			// 現在のステート
	int block_comment_start_line = 1;	// ブロックコメントの開始行
	int line = 1;						// 現在の行数

	for (TSTR::const_iterator p = m_text.begin(); p != m_text.end(); ++p)	{

		TCHAR moji = *p;	// 出力文字

		// 改行の場合のみ特殊処理
		if (*p == _T('\n'))	{
			// シングルコーテーション中ならエラー
			if (state == STATE_SINGLE_QUOTE || state == STATE_SINGLE_QUOTE_ESCAPE)
				return error(line, _T("シングルコーテーション (') 中に改行が見つかりました。"));
			// ダブルコーテーション中ならエラー
			if (state == STATE_DOUBLE_QUOTE || state == STATE_DOUBLE_QUOTE_ESCAPE)
				return error(line, _T("ダブルコーテーション (\") 中に改行が見つかりました。"));
			// 行コメント中なら行コメント終了
			if (state == STATE_COMMENT_LINE)
				state = STATE_NONE;
			// 行数インクリメント
			line ++;
		}
		// シングルコーテーション中
		else if (state == STATE_SINGLE_QUOTE)	{
			// シングルコーテーション終了
			if (*p == _T('\''))	{
				state = STATE_NONE;
			}
			// シングルコーテーションエスケープ
			else if (*p == _T('\\'))	{
				state = STATE_SINGLE_QUOTE_ESCAPE;
			}
		}
		// シングルコーテーションエスケープ中
		else if (state == STATE_SINGLE_QUOTE_ESCAPE)	{
			// エスケープ可能な文字
			if (*p == _T('"') || *p == _T('\\') || *p == _T('n'))	{
				state = STATE_SINGLE_QUOTE;
			}
			else	{
				return error(line, _T("不正なエスケープ（\\）です。'\\' を使うには '\\\\' と書いてください。"));
			}
		}
		// ダブルコーテーション中
		else if (state == STATE_DOUBLE_QUOTE)	{
			// ダブルコーテーション終了
			if (*p == _T('"'))	{
				state = STATE_NONE;
			}
			// ダブルコーテーションエスケープ
			else if (*p == _T('\\'))	{
				state = STATE_DOUBLE_QUOTE_ESCAPE;
			}
		}
		// ダブルコーテーションエスケープ中
		else if (state == STATE_DOUBLE_QUOTE_ESCAPE)	{
			// エスケープ可能な文字
			if (*p == _T('"') || *p == _T('\\') || *p == _T('n'))	{
				state = STATE_DOUBLE_QUOTE;
			}
			else	{
				return error(line, _T("不正なエスケープ（\\）です。'\\' を使うには '\\\\' と書いてください。"));
			}
		}
		// 行コメント中
		else if (state == STATE_COMMENT_LINE)	{
			continue;	// 何もしない
		}
		// ブロックコメント中
		else if (state == STATE_COMMENT_BLOCK)	{
			// ブロックコメント終了
			if (p + 1 != m_text.end() && *p == _T('*') && *(p + 1) == _T('/'))	{
				state = STATE_NONE;
				p++;
			}
			continue;	// 何もしない
		}
		// 通常処理
		else if (state == STATE_NONE)	{

			if (false);
			// シングルコーテーション開始
			else if (*p == _T('\''))	{
				state = STATE_SINGLE_QUOTE;
			}
			// ダブルコーテーション開始
			else if (*p == _T('"'))	{
				state = STATE_DOUBLE_QUOTE;
			}
			// 行コメント開始
			else if (*p == _T(';'))	{
				state = STATE_COMMENT_LINE;
				continue;
			}
			// 行コメント開始
			else if (p + 1 != m_text.end() && *p == _T('/') && *(p + 1) == _T('/'))	{
				state = STATE_COMMENT_LINE;
				continue;
			}
			// ブロックコメント開始
			else if (p + 1 != m_text.end() && *p == _T('/') && *(p + 1) == _T('*'))	{
				block_comment_start_line = line;
				state = STATE_COMMENT_BLOCK;
				continue;
			}
			else	{
				if (_T('A') <= moji && moji <= _T('Z'))
					moji -= _T('A') - _T('a');
			}
		}

		// 出力
		out_text.push_back(moji);
	}

	// 終了時のステートによってエラー
	if (state == STATE_SINGLE_QUOTE)
		return error(line, _T("シングルコーテーション (') が閉じられていません。"));
	if (state == STATE_DOUBLE_QUOTE)
		return error(line, _T("ダブルコーテーション (\") が閉じられていません。"));
	if (state == STATE_COMMENT_BLOCK)
		return error(block_comment_start_line, _T(" コメント (/*) が閉じられていません。"));

	// 成功
	m_text = out_text;
	return true;
}

// ****************************************************************
// inc 解析：メイン
// ================================================================
bool C_inc_file_analizer::ia_main_step_1()
{
	// パラメータの初期化
	m_error_line = -1;
	m_error_str.clear();
	int line = 1;

	// 解析開始
	for (TSTR::const_iterator p = m_text.begin(); p != m_text.end();)	{

		// セパレータをスキップ
		if (!ia_skip_separator(p, &line))
			break;	// 終端なら終了

		// 宣言を探す
		BSP<S_tnms_ia_declare> declare(new S_tnms_ia_declare);
		if (!ia_declare(p, &line, declare))
			return false;
	}

	return true;
}

// ****************************************************************
// inc 解析：セパレータをスキップ
// ================================================================
bool C_inc_file_analizer::ia_skip_separator(TSTR::const_iterator& p_ret, int* line_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;

	while (1)	{
		if (false);

		// 終端
		else if (p == m_text.end())
			return false;
		// 改行
		else if (*p == _T('\n'))	{
			p++;
			line++;
		}
		// その他のセパレータ
		else if (*p == ' ' || *p == '\t')
			p++;
		// セパレータ以外の文字
		else
			break;
	}

	// 成功
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc 解析：宣言の取得
// ================================================================
bool C_inc_file_analizer::ia_declare(TSTR::const_iterator& p_ret, int* line_ret, BSP<S_tnms_ia_declare>& declare_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;

	TSTR::const_iterator p_org = p_ret;
	int p_org_int = (int)(p_org - m_text.begin());

	BSP<S_tnms_ia_declare> declare(new S_tnms_ia_declare);

	// セパレータをスキップ
	ia_skip_separator(p, &line);
	// コマンドタイプ
	if (!ia_declare_type(p, &line, &declare->type))
		return false;

	TSTR declare_name;			// 宣言の名前
	int declare_line = line;	// 宣言の行

	if (false);

	// #replace の処理
	else if (declare->type == TNMS_IA_DECLARE_TYPE_REPLACE)	{
		declare->replace = BSP<S_tnms_ia_replace>(new S_tnms_ia_replace);
		declare->replace->type = TNMS_IA_REPLACE_TYPE_REPLACE;
		if (!ia_define_name(p, &line, declare->replace->name))				return false;
		if (!ia_define_after(p, &line, declare->replace->after))			return false;
		declare_name = declare->replace->name;		// 重複判定用の名前

		// マクロは２文字以上必須（１文字ＯＫになりました 2013/01/17）
		if (declare_name.length() < 1)
			return error(declare_line, _T("#replace の名前は１文字以上必要です。"));

		// 重複判定
		if (m_piad->name_set.find(declare_name) != m_piad->name_set.end())
			return error(declare_line, declare_name + _T(" が二重に宣言されています。"));
		m_piad->name_set.insert(declare_name);

		// 置換ツリーを追加
		C_replace_tree::add_replace(m_piad->replace_tree, declare->replace, 0);
	}

	// #define の処理
	else if (declare->type == TNMS_IA_DECLARE_TYPE_DEFINE)	{
		declare->replace = BSP<S_tnms_ia_replace>(new S_tnms_ia_replace);
		declare->replace->type = TNMS_IA_REPLACE_TYPE_DEFINE;
		if (!ia_define_name(p, &line, declare->replace->name))				return false;
		if (!ia_define_after(p, &line, declare->replace->after))			return false;
		declare_name = declare->replace->name;		// 重複判定用の名前

		// マクロは２文字以上必須（１文字ＯＫになりました 2013/01/17）
		if (declare_name.length() < 1)
			return error(declare_line, _T("#define の名前は１文字以上必要です。"));

		// 重複判定
		if (m_piad->name_set.find(declare_name) != m_piad->name_set.end())
			return error(declare_line, declare_name + _T(" が二重に宣言されています。"));
		m_piad->name_set.insert(declare_name);

		// 置換ツリーを追加
		C_replace_tree::add_replace(m_piad->replace_tree, declare->replace, 0);
	}

	// #define_s の処理
	else if (declare->type == TNMS_IA_DECLARE_TYPE_DEFINE_SPACE)	{
		declare->replace = BSP<S_tnms_ia_replace>(new S_tnms_ia_replace);
		declare->replace->type = TNMS_IA_REPLACE_TYPE_DEFINE;
		if (!ia_define_space_name(p, &line, declare->replace->name))		return false;
		if (!ia_define_after(p, &line, declare->replace->after))			return false;
		declare_name = declare->replace->name;		// 重複判定用の名前

		// マクロは２文字以上必須（１文字ＯＫになりました 2013/01/17）
		if (declare_name.length() < 1)
			return error(declare_line, _T("#define の名前は１文字以上必要です。"));

		// 重複判定
		if (m_piad->name_set.find(declare_name) != m_piad->name_set.end())
			return error(declare_line, declare_name + _T(" が二重に宣言されています。"));
		m_piad->name_set.insert(declare_name);

		// 置換ツリーを追加
		C_replace_tree::add_replace(m_piad->replace_tree, declare->replace, 0);
	}

	// #macro の処理
	else if (declare->type == TNMS_IA_DECLARE_TYPE_MACRO)	{
		declare->replace = BSP<S_tnms_ia_replace>(new S_tnms_ia_replace);
		declare->replace->type = TNMS_IA_REPLACE_TYPE_MACRO;
		if (!ia_macro_name(p, &line, declare->replace->name))				return false;
		if (!ia_macro_arg_list(p, &line, declare->replace->arg_list))		return false;
		if (!ia_macro_after(p, &line, declare->replace->after))				return false;
		declare_name = declare->replace->name;		// 重複判定用の名前

		// マクロは１文字以上必須（１文字ＯＫになりました 2013/01/17）
		if (declare_name.length() < 1)
			return error(declare_line, _T("#macro の名前は１文字以上必要です。"));

		// マクロの先頭は '@'
		if (declare_name[0] != _T('@'))
			return error(declare_line, _T("#macro の名前は '@' で始めなければいけません。"));

		// 重複判定
		if (m_piad->name_set.find(declare_name) != m_piad->name_set.end())
			return error(declare_line, declare_name + _T(" が二重に宣言されています。"));
		m_piad->name_set.insert(declare_name);

		// 置換ツリーを追加
		C_replace_tree::add_replace(m_piad->replace_tree, declare->replace, 0);
	}

	// #property の処理
	else if (declare->type == TNMS_IA_DECLARE_TYPE_PROPERTY)	{

		// セパレータをスキップ
		ia_skip_separator(p, &line);
		int name_line = line;

		// 詳細な解析はまだ行わない（次のステップで行う）
		TSTR text;
		ia_property(p, &line, text);
		m_piad2->property_temp_list.push_back(text);
		m_piad2->property_line_list.push_back(name_line);
	}

	// #command の処理
	else if (declare->type == TNMS_IA_DECLARE_TYPE_COMMAND)	{

		// セパレータをスキップ
		ia_skip_separator(p, &line);
		int name_line = line;

		// 詳細な解析はまだ行わない（次のステップで行う）
		TSTR text;
		ia_command(p, &line, text);
		m_piad2->command_temp_list.push_back(text);
		m_piad2->command_line_list.push_back(name_line);
	}

	// #expand の処理
	else if (declare->type == TNMS_IA_DECLARE_TYPE_EXPAND)	{
		TSTR after;
		if (!ia_macro_after(p, &line, after))				return false;

		// 番兵の挿入
		for (int i = 0; i < 256; i++)
			after.push_back(_T('\0'));

		// 置換
		for (TSTR::iterator p2 = after.begin(); *p2 != _T('\0');)
			std_replace(after, p2, &line, m_piad->replace_tree, S_replace_node());

		// 番兵を削除
		after.erase(after.end() - 256, after.end());

		// ▲テキストに追加するとかいう暴挙（行番号ずれまくるよ…）
		TSTR left = m_text.substr(0, (int)(p_org - m_text.begin()));
		TSTR right = m_text.substr((int)(p - m_text.begin()));
		m_text = left + after + right;

		// 再展開するために位置を戻す
		p = m_text.begin() + p_org_int;
	}

	// 成功
	declare_ret = declare;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc 解析：宣言タイプの取得
// ================================================================
bool C_inc_file_analizer::ia_declare_type(TSTR::const_iterator& p_ret, int* line_ret, TNMS_IA_DECLARE_TYPE* type_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	TNMS_IA_DECLARE_TYPE type = TNMS_IA_DECLARE_TYPE_NONE;

	// セパレータをスキップ
	ia_skip_separator(p, &line);

	// コマンド名を取得
	if (false);
	else if (str_analize_check_str(p, m_text.end(), _T("#replace")))
		type = TNMS_IA_DECLARE_TYPE_REPLACE;
	else if (str_analize_check_str(p, m_text.end(), _T("#define_s")))
		type = TNMS_IA_DECLARE_TYPE_DEFINE_SPACE;
	else if (str_analize_check_str(p, m_text.end(), _T("#define")))
		type = TNMS_IA_DECLARE_TYPE_DEFINE;
	else if (str_analize_check_str(p, m_text.end(), _T("#macro")))
		type = TNMS_IA_DECLARE_TYPE_MACRO;
	else if (str_analize_check_str(p, m_text.end(), _T("#property")))
		type = TNMS_IA_DECLARE_TYPE_PROPERTY;
	else if (str_analize_check_str(p, m_text.end(), _T("#command")))
		type = TNMS_IA_DECLARE_TYPE_COMMAND;
	else if (str_analize_check_str(p, m_text.end(), _T("#expand")))
		type = TNMS_IA_DECLARE_TYPE_EXPAND;
	else
		return error(*line_ret, _T("不正な宣言があります。ラベルの場合は、'#' を '##' に変更してください。"));

	// 成功
	*type_ret = type;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc 解析：#define の名前
// ================================================================
bool C_inc_file_analizer::ia_define_name(TSTR::const_iterator& p_ret, int* line_ret, TSTR& name_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;

	// セパレータをスキップ
	ia_skip_separator(p, &line);

	// 現在の位置を記憶
	TSTR::const_iterator name_start_pos = p;

	// 区切り以外が続く間進む
	for (; !(p == m_text.end() || *p == _T(' ') || *p == _T('\t') || *p == _T('\n')); ++p);

	// 名前を作成
	TSTR name = TSTR(name_start_pos, p);
	if (name.empty())
		return false;

	// 成功
	name_ret = name;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc 解析：#define の名前
// ================================================================
bool C_inc_file_analizer::ia_define_space_name(TSTR::const_iterator& p_ret, int* line_ret, TSTR& name_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;

	// セパレータをスキップ
	ia_skip_separator(p, &line);

	// 現在の位置を記憶
	TSTR::const_iterator name_start_pos = p;

	// 区切り以外が続く間進む
	for (; !(p == m_text.end() || *p == _T('\t') || *p == _T('\n')); ++p);

	// 名前を作成
	TSTR name = TSTR(name_start_pos, p);
	if (name.empty())
		return false;

	// 成功
	name_ret = name;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc 解析：#define の置換後
// ================================================================
bool C_inc_file_analizer::ia_define_after(TSTR::const_iterator& p_ret, int* line_ret, TSTR& after_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	TSTR after;

	enum IF_STATE
	{
		IF_STATE_NONE = 0,
		IF_STATE_TRUE = 1,
		IF_STATE_FALSE = 2,
		IF_STATE_FALSE_MORE = 3,
	};

	IF_STATE if_state[16] = { IF_STATE_NONE };		// 現在の if ステート
	int if_depth = 0;								// 現在の if の深さ

	// セパレータをスキップ
	ia_skip_separator(p, &line);

	// 解析開始
	for (; p != m_text.end();)	{
		if (false);

		// 文末
		else if (p == m_text.end())	{
			break;
		}
		// エスケープ
		else if (str_analize_check_str(p, m_text.end(), _T("##")))	{
			after += _T('#');
		}
		// #ifdef
		else if (str_analize_check_str(p, m_text.end(), _T("#ifdef")))	{

			// セパレータをスキップ
			if (!ia_skip_separator(p, &line))
				return error(line, _T("#ifdef に続く単語がありません。"));

			TSTR word;
			if (str_analize_get_word_ex(p, m_text.end(), word)) {

				std::set<TSTR>::iterator itr = m_piad->name_set.find(word);
				if (itr != m_piad->name_set.end()) {
					if_state[++if_depth] = IF_STATE_TRUE;
					continue;
				}
				else {
					if_state[++if_depth] = IF_STATE_FALSE;
					continue;
				}
			}
			else {
				return error(line, _T("#ifdef に続く単語がありません。"));
			}
		}
		// #elseifdef
		else if (str_analize_check_str(p, m_text.end(), _T("#elseifdef"))) {

			if (if_state[if_depth] > 0) {

				// セパレータをスキップ
				if (!ia_skip_separator(p, &line))
					return error(line, _T("#elseifdef に続く単語がありません。"));

				TSTR word;
				if (str_analize_get_word_ex(p, m_text.end(), word)) {

					if (if_state[if_depth] == IF_STATE_FALSE_MORE)	{
						continue;
					}
					else if (if_state[if_depth] == IF_STATE_TRUE)	{
						if_state[if_depth] = IF_STATE_FALSE_MORE;
						continue;
					}
					else {

						std::set<TSTR>::iterator itr = m_piad->name_set.find(word);
						if (itr != m_piad->name_set.end()) {
							if_state[if_depth] = IF_STATE_TRUE;
							continue;
						}
						else {
							if_state[if_depth] = IF_STATE_FALSE;
							continue;
						}
					}
				}
				else {
					return error(line, _T("#elseifdef に続く単語がありません。"));
				}
			}
			else {
				return error(line, _T("この #elseifdef に対応する #if がありません。"));
			}
		}
		// #else
		else if (str_analize_check_str(p, m_text.end(), _T("#else"))) {

			if (if_state[if_depth] > 0) {

				if (if_state[if_depth] == IF_STATE_FALSE_MORE)	{
					continue;
				}
				else if (if_state[if_depth] == IF_STATE_TRUE)	{
					if_state[if_depth] = IF_STATE_FALSE_MORE;
					continue;
				}
				else {
					if_state[if_depth] = IF_STATE_TRUE;
					continue;
				}
			}
			else {
				return error(line, _T("この #else に対応する #if がありません。"));
			}
		}
		// #endif
		else if (str_analize_check_str(p, m_text.end(), _T("#endif"))) {

			if (if_state[if_depth] > 0) {
				--if_depth;
				continue;
			}
			else {
				return error(line, _T("この #endif に対応する #if がありません。"));
			}
		}
		// 改行
		else if (*p == _T('\n'))	{
			after += _T(' ');
			line++;
			p++;
		}
		// if false 中はここまで
		else if (if_state[if_depth] == IF_STATE_FALSE || if_state[if_depth] == IF_STATE_FALSE_MORE) {
			p++;
		}
		// # があれば終了
		else if (*p == _T('#'))	{
			break;
		}
		// その他の文字はそのまま
		else	{
			after += *p;
			p++;
		}
	}

	// 最後のスペースなどを切る
	int not_space_pos = (int)after.find_last_not_of(_T(" \t"));
	if (not_space_pos != TSTR::npos)	{
		after.erase(not_space_pos + 1, after.size());
	}

	// 成功
	after_ret = after;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc 解析：#macro の名前
// ================================================================
bool C_inc_file_analizer::ia_macro_name(TSTR::const_iterator& p_ret, int* line_ret, TSTR& name_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;

	// セパレータをスキップ
	ia_skip_separator(p, &line);

	// 現在の位置を記憶
	TSTR::const_iterator name_start_pos = p;

	// 区切り以外が続く間進む
	for (; !(p == m_text.end() || *p == _T(' ') || *p == _T('\t') || *p == _T('\n') || *p == _T('(')); ++p);

	// 名前を作成
	TSTR name = TSTR(name_start_pos, p);
	if (name.empty())
		return false;

	// 成功
	name_ret = name;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc 解析：#macro の引数リスト
// ================================================================
bool C_inc_file_analizer::ia_macro_arg_list(TSTR::const_iterator& p_ret, int* line_ret, ARRAY<S_tnms_ia_macro_arg>& arg_list_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	ARRAY<S_tnms_ia_macro_arg> arg_list;

	// セパレータをスキップ
	if (!ia_skip_separator(p, &line))
		return error(line, _T("引数リストの引数の解析に失敗しました。"));

	// '('
	if (*p == _T('('))	{
		p++;

		// 引数リスト
		while (1)	{

			// 引数を取得
			S_tnms_ia_macro_arg arg;
			if (!ia_macro_arg(p, &line, arg))
				return error(line, _T("引数リストの引数の解析に失敗しました。"));

			arg_list.push_back(arg);

			// セパレータをスキップ
			if (!ia_skip_separator(p, &line))
				return error(line, _T("引数リストの '(' が閉じられていません。"));

			// ',' でないなら終了
			if (*p != _T(','))
				break;

			// 次の文字へ
			p++;
		}

		// セパレータをスキップ
		if (!ia_skip_separator(p, &line))
			return error(line, _T("引数リストの '(' が閉じられていません。"));

		// ')'
		if (*p++ != ')')
			return error(line, _T("引数リストの '(' が閉じられていません。"));
	}

	// 成功
	arg_list_ret = arg_list;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc 解析：#macro の引数
// ================================================================
bool C_inc_file_analizer::ia_macro_arg(TSTR::const_iterator& p_ret, int* line_ret, S_tnms_ia_macro_arg& arg_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	S_tnms_ia_macro_arg arg;

	// セパレータをスキップ
	if (!ia_skip_separator(p, &line))
		return error(line, _T("引数リストの引数が見つかりません。"));

	// 現在の位置を記憶
	TSTR::const_iterator arg_start_pos = p;

	// 区切り以外が続く間進む
	for (; !(p == m_text.end() || *p == _T(' ') || *p == _T('\t') || *p == _T('\n') || *p == _T(',') || *p == _T('(') || *p == _T(')') || *p == _T('"') || *p == _T('\'')); ++p);

	// 引数の名前確定
	arg.name = TSTR(arg_start_pos, p);
	if (arg.name.empty())
		return error(line, _T("引数リストの引数が見つかりません。"));

	// セパレータをスキップ
	if (!ia_skip_separator(p, &line))
		return error(line, _T("引数リスト終わりの ')' が見つかりません。"));

	// デフォルト値
	if (*p == _T('('))	{
		p++;

		// 現在の位置を記憶
		TSTR::const_iterator def_start_pos = p;

		// デフォルト値終了が見つかるまで進む
		for (; *p != _T(')'); ++p)	{

			if (p == m_text.end() || *p == _T('\t') || *p == _T('\n'))
				return error(line, _T("引数のデフォルト値を解析中に不正な文字が見つかりました。"));
		}

		arg.def_value = TSTR(def_start_pos, p);
		p++;
	}

	// 成功
	arg_ret = arg;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc 解析：#macro の置換後
// ================================================================
bool C_inc_file_analizer::ia_macro_after(TSTR::const_iterator& p_ret, int* line_ret, TSTR& after_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	TSTR after;

	enum IF_STATE
	{
		IF_STATE_NONE = 0,
		IF_STATE_TRUE = 1,
		IF_STATE_FALSE = 2,
		IF_STATE_FALSE_MORE = 3,
	};

	IF_STATE if_state[16] = { IF_STATE_NONE };		// 現在の if ステート
	int if_depth = 0;								// 現在の if の深さ

	// セパレータをスキップ
	ia_skip_separator(p, &line);

	// 解析開始
	for (; p != m_text.end();)	{
		if (false);

		// 文末
		else if (p == m_text.end())	{
			break;
		}
		// エスケープ
		else if (str_analize_check_str(p, m_text.end(), _T("##"), false))	{
			after += _T('#');
			p += 2;
		}
		// #ifdef
		else if (str_analize_check_str(p, m_text.end(), _T("#ifdef")))	{

			// セパレータをスキップ
			if (!ia_skip_separator(p, &line))
				return error(line, _T("#ifdef に続く単語がありません。"));

			TSTR word;
			if (str_analize_get_word_ex(p, m_text.end(), word)) {

				std::set<TSTR>::iterator itr = m_piad->name_set.find(word);
				if (itr != m_piad->name_set.end()) {
					if_state[++if_depth] = IF_STATE_TRUE;
					continue;
				}
				else {
					if_state[++if_depth] = IF_STATE_FALSE;
					continue;
				}
			}
			else {
				return error(line, _T("#ifdef に続く単語がありません。"));
			}
		}
		// #elseifdef
		else if (str_analize_check_str(p, m_text.end(), _T("#elseifdef"))) {

			if (if_state[if_depth] > 0) {

				// セパレータをスキップ
				if (!ia_skip_separator(p, &line))
					return error(line, _T("#elseifdef に続く単語がありません。"));

				TSTR word;
				if (str_analize_get_word_ex(p, m_text.end(), word)) {

					if (if_state[if_depth] == IF_STATE_FALSE_MORE)	{
						continue;
					}
					else if (if_state[if_depth] == IF_STATE_TRUE)	{
						if_state[if_depth] = IF_STATE_FALSE_MORE;
						continue;
					}
					else {

						std::set<TSTR>::iterator itr = m_piad->name_set.find(word);
						if (itr != m_piad->name_set.end()) {
							if_state[if_depth] = IF_STATE_TRUE;
							continue;
						}
						else {
							if_state[if_depth] = IF_STATE_FALSE;
							continue;
						}
					}
				}
				else {
					return error(line, _T("#elseifdef に続く単語がありません。"));
				}
			}
			else {
				return error(line, _T("この #elseifdef に対応する #if がありません。"));
			}
		}
		// #else
		else if (str_analize_check_str(p, m_text.end(), _T("#else"))) {

			if (if_state[if_depth] > 0) {

				if (if_state[if_depth] == IF_STATE_FALSE_MORE)	{
					continue;
				}
				else if (if_state[if_depth] == IF_STATE_TRUE)	{
					if_state[if_depth] = IF_STATE_FALSE_MORE;
					continue;
				}
				else {
					if_state[if_depth] = IF_STATE_TRUE;
					continue;
				}
			}
			else {
				return error(line, _T("この #else に対応する #if がありません。"));
			}
		}
		// #endif
		else if (str_analize_check_str(p, m_text.end(), _T("#endif"))) {

			if (if_state[if_depth] > 0) {
				--if_depth;
				continue;
			}
			else {
				return error(line, _T("この #endif に対応する #if がありません。"));
			}
		}
		// 改行
		else if (*p == _T('\n'))	{
			after += _T(' ');
			line++;
			p++;
		}
		// if false 中はここまで
		else if (if_state[if_depth] == IF_STATE_FALSE || if_state[if_depth] == IF_STATE_FALSE_MORE) {
			p++;
		}
		// # があれば終了
		else if (*p == _T('#'))	{
			break;
		}
		// その他の文字はそのまま
		else	{
			after += *p;
			p ++;
		}
	}

	// 最後のスペースなどを切る
	int not_space_pos = (int)after.find_last_not_of(_T(" \t"));
	if (not_space_pos != TSTR::npos)
		after.erase(not_space_pos + 1, after.size());

	// 成功
	after_ret = after;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc 解析：#property（置換前）
// ================================================================
bool C_inc_file_analizer::ia_property(TSTR::const_iterator& p_ret, int* line_ret, TSTR& name_ret)
{
	// 終わりから # が見つかるまで進む
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	
	// セパレータをスキップ
	ia_skip_separator(p, &line);

	// 現在の位置を記憶
	TSTR::const_iterator name_start_pos = p;

	// 区切り以外が続く間進む
	while (1)	{
		if (false);

		// 文末
		else if (p == m_text.end() || *p == _T('#'))	{
			break;
		}
		// 改行
		else if (*p == _T('\n'))	{
			line ++;
			p ++;
		}
		// その他の文字はそのまま
		else	{
			p ++;
		}
	}

	// 名前を作成
	TSTR name = TSTR(name_start_pos, p);
	if (name.empty())
		return false;

	// 成功
	name_ret = name;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc 解析：#command（置換前）
// ================================================================
bool C_inc_file_analizer::ia_command(TSTR::const_iterator& p_ret, int* line_ret, TSTR& name_ret)
{
	// 終わりから # が見つかるまで進む
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	
	// セパレータをスキップ
	ia_skip_separator(p, &line);

	// 現在の位置を記憶
	TSTR::const_iterator name_start_pos = p;

	// 区切り以外が続く間進む
	while (1)	{
		if (false);

		// 文末
		else if (p == m_text.end() || *p == _T('#'))	{
			break;
		}
		// 改行
		else if (*p == _T('\n'))	{
			line ++;
			p ++;
		}
		// その他の文字はそのまま
		else	{
			p ++;
		}
	}

	// 名前を作成
	TSTR name = TSTR(name_start_pos, p);
	if (name.empty())
		return false;

	// 成功
	name_ret = name;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc 解析：#property の名前
// ================================================================
bool C_inc_file_analizer::ia_property_name(TSTR::const_iterator& p_ret, int* line_ret, TSTR& name_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;

	// セパレータをスキップ
	ia_skip_separator(p, &line);

	// 現在の位置を記憶
	TSTR::const_iterator name_start_pos = p;

	// 区切り以外が続く間進む
	for (; !(p == m_text.end() || *p == _T(' ') || *p == _T(':') || *p == _T('\t') || *p == _T('\n')); ++p);

	// 名前を作成
	TSTR name = TSTR(name_start_pos, p);
	if (name.empty())
		return false;

	// 成功
	name_ret = name;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc 解析：#command の名前
// ================================================================
bool C_inc_file_analizer::ia_command_name(TSTR::const_iterator& p_ret, int* line_ret, TSTR& name_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;

	// セパレータをスキップ
	ia_skip_separator(p, &line);

	// 現在の位置を記憶
	TSTR::const_iterator name_start_pos = p;

	// 区切り以外が続く間進む
	for (; !(p == m_text.end() || *p == _T(' ') || *p == _T('(') || *p == _T(':') || *p == _T('\t') || *p == _T('\n')); ++p);

	// 名前を作成
	TSTR name = TSTR(name_start_pos, p);
	if (name.empty())
		return false;

	// 成功
	name_ret = name;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc 解析：#command の引数リスト
// ================================================================
bool C_inc_file_analizer::ia_command_arg_list(TSTR::const_iterator& p_ret, int* line_ret, S_tnms_arg_list& arg_list_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	S_tnms_arg_list arg_list;

	// セパレータをスキップ
	if (!ia_skip_separator(p, &line))
		return true;	// ポインタを進めずに成功（引数なし）

	// '('
	if (*p++ != _T('('))
		return true;	// ポインタを進めずに成功（引数なし）

	// 引数リスト
	bool def_value_exist = false;
	for (int i = 0; ; i++)	{

		// 引数を取得
		S_tnms_arg arg;
		if (!ia_command_arg(p, &line, arg))
			return error(line, _T("引数リストの引数の解析に失敗しました。"));

		arg_list.arg_list.push_back(arg);

		// デフォルト引数の判定
		if (def_value_exist)	{
			if (!arg.def_exist)	{
				return error(line, tostr(i + 1) + _T(" 個目の引数にデフォルト引数が必要です。"));
			}
		}

		if (arg.def_exist)
			def_value_exist = true;

		// セパレータをスキップ
		if (!ia_skip_separator(p, &line))
			return error(line, _T("引数リストの '(' が閉じられていません。"));

		// ',' でないなら終了
		if (*p != _T(','))
			break;

		// 次の文字へ
		p++;
	}

	// セパレータをスキップ
	if (!ia_skip_separator(p, &line))
		return error(line, _T("引数リストの '(' が閉じられていません。"));

	// ')'
	if (*p++ != ')')
		return error(line, _T("引数リストの '(' が閉じられていません。"));

	// 成功
	arg_list_ret = arg_list;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc 解析：#command の引数
// ================================================================
bool C_inc_file_analizer::ia_command_arg(TSTR::const_iterator& p_ret, int* line_ret, S_tnms_arg& arg_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	S_tnms_arg arg;

	// 引数の型を解析
	if (!ia_form(p, line, line_ret, &arg.form))
		return false;

	// セパレータをスキップ
	ia_skip_separator(p, &line);

	// デフォルト引数（▲未実装）
	if (str_analize_check_moji(p, m_text.end(), _T('(')))	{

		// セパレータをスキップ
		ia_skip_separator(p, &line);

		// 引数を取得
		if (false);
		else if (arg.form == FM_INT)	{
			int num = 0;
			if (!str_analize_get_integer(p, m_text.end(), &num))
				return error(line, _T("int 型のデフォルト引数"));

			arg.def_exist = true;
			arg.def_int = num;
		}
		else if (arg.form == FM_STR)	{
			TSTR str;
			if (!str_analize_get_dquote_str(p, m_text.end(), str, _T('\n')))
				return error(line, _T("str 型のデフォルト引数"));

			arg.def_exist = true;
			arg.def_str = str;
		}

		// セパレータをスキップ
		ia_skip_separator(p, &line);

		// ')'
		if (!str_analize_check_moji(p, m_text.end(), _T(')')))
			return false;
	}

	// 成功
	arg_ret = arg;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc 解析：#property の型
// ================================================================
bool C_inc_file_analizer::ia_declare_property_form(TSTR::const_iterator& p_ret, int* line_ret, int* form_ret, int* size_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	int form = FM_INT;		// デフォルトでは int 型
	int size = 0;			// デフォルトではサイズ０

	// セパレータをスキップ
	ia_skip_separator(p, &line);

	// ':'
	if (str_analize_check_moji(p, m_text.end(), _T(':')))	{
		int colon_line = line;

		// 型
		if (!ia_form(p, colon_line, &line, &form))
			return false;

		// セパレータをスキップ
		ia_skip_separator(p, &line);

		// '['
		if (str_analize_check_moji(p, m_text.end(), _T('[')))	{
			int kakko_line = line;

			// セパレータをスキップ
			ia_skip_separator(p, &line);

			// 整数
			if (!str_analize_get_num(p, m_text.end(), &size))
				return error(kakko_line, _T("配列の添え字が整数でありません。"));

			// セパレータをスキップ
			ia_skip_separator(p, &line);

			// ']'
			if (!str_analize_check_moji(p, m_text.end(), ']'))
				return error(kakko_line, _T("配列の添え字が ] で閉じられていません。"));

			// 配列になれるのは特定の型のみ
			if (form != FM_INTLIST && form != FM_STRLIST)
				return error(kakko_line, _T("intlist および strlist 以外は配列になれません。"));
		}
	}

	// 成功
	*form_ret = form;
	*size_ret = size;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc 解析：宣言の型
// ================================================================
bool C_inc_file_analizer::ia_declare_form(TSTR::const_iterator& p_ret, int* line_ret, int* form_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	int form = FM_INT;		// デフォルトでは int 型

	// セパレータをスキップ
	ia_skip_separator(p, &line);

	// ':'
	if (*p == ':')	{
		p++;

		// 型
		if (!ia_form(p, line, &line, &form))
			return false;
	}

	// 成功
	*form_ret = form;
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// inc 解析：型
// ================================================================
bool C_inc_file_analizer::ia_form(TSTR::const_iterator& p_ret, int error_line, int* line_ret, int* form_ret)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;
	int form = FM_INT;		// デフォルトでは int 型

	// セパレータをスキップ
	ia_skip_separator(p, &line);

	// 型名を取得
	TSTR form_name;
	str_analize_get_word(p, m_text.end(), form_name);
	form = get_form_code_by_name(form_name);
	if (form == -1)
		return error(error_line, _T("不正な型です。"));	// オリジナルの行数でエラーを出す

	// 成功
	*form_ret = form;
	*line_ret = line;
	p_ret = p;
	return true;
}


// ****************************************************************
// 文字解析：置換
// ================================================================
bool C_inc_file_analizer::std_replace(TSTR& text, TSTR::const_iterator& p_ret, int* line_ret, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;

	BSP<S_tnms_ia_replace> p_default_replace;
	BSP<S_tnms_ia_replace> p_added_replace;
	BSP<S_tnms_ia_replace> p_replace;

	// デフォルト置換ツリーから置換を検索する
	C_replace_tree::search_replace(default_replace_tree, p, p_default_replace);

	// 追加置換ツリーから置換を検索する
	if (!added_replace_tree.child_map.empty())
		C_replace_tree::search_replace(added_replace_tree, p, p_added_replace);

	// 置換がなかった場合は１文字進む
	if (!p_default_replace && !p_added_replace)	{
		++p;
		*line_ret = line;
		p_ret = p;
		return true;
	}
	// 置換があった場合は置換を選択
	else if (p_default_replace && p_added_replace)	{
		if (p_default_replace->name > p_added_replace->name)	{
			p_replace = p_default_replace;
		}
		else	{
			p_replace = p_added_replace;
		}
	}
	else if (p_default_replace)	{
		p_replace = p_default_replace;
	}
	else	{
		p_replace = p_added_replace;
	}

	// 置換開始
	if (false);

	// #replace
	else if (p_replace->type == TNMS_IA_REPLACE_TYPE_REPLACE)	{
		int name_len = (int)p_replace->name.size();
		TSTR::const_iterator define_start_pos = p;
		int define_start_pos_int = (int)(define_start_pos - text.begin());

		// 置換する文字列の最後に移動する
		p += name_len;
		// 解析後の文字列に置換
		text.replace(define_start_pos, p, p_replace->after);
		// 解析ポインタの再設定
		p = text.begin() + define_start_pos_int + p_replace->after.size();
	}

	// #define
	else if (p_replace->type == TNMS_IA_REPLACE_TYPE_DEFINE)	{
		int name_len = (int)p_replace->name.size();
		TSTR::const_iterator define_start_pos = p;
		int define_start_pos_int = (int)(define_start_pos - text.begin());

		// 置換する文字列の最後に移動する
		p += name_len;
		// 解析後の文字列に置換
		text.replace(define_start_pos, p, p_replace->after);
		// 解析ポインタの再設定
		p = text.begin() + define_start_pos_int;
	}

	// #macro
	else if (p_replace->type == TNMS_IA_REPLACE_TYPE_MACRO)	{
		int name_len = (int)p_replace->name.size();

		// マクロの解析
		TSTR result_text;
		TSTR::const_iterator macro_start_pos = p;
		int macro_start_pos_int = (int)(macro_start_pos - text.begin());

		// 置換する文字列の名前部分の最後に移動する
		p += name_len;
		// マクロを解析する
		if (!analize_macro(text, p, &line, p_replace.get(), default_replace_tree, added_replace_tree, result_text))
			return false;
		// 解析後の文字列に置換
		text.replace(macro_start_pos, p, result_text);
		// 解析ポインタの再設定
		p = text.begin() + macro_start_pos_int + result_text.size();
	}

	// 成功
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// 文字解析：マクロの解析
// ================================================================
bool C_inc_file_analizer::analize_macro(TSTR& text, TSTR::const_iterator& p_ret, int* line_ret, S_tnms_ia_replace* p_macro, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree, TSTR& result_text)
{
	TSTR::const_iterator p = p_ret;
	int line = *line_ret;

	ARRAY<TSTR> arg_list_real;			// 実引数リスト
	int kakko_cnt = 0;					// 現在の括弧の数
	int arg_cnt = 0;					// 引数の個数

	// 括弧がある場合
	if (*p == _T('('))	{
		p++;

		TSTR::const_iterator arg_start_pos = p;	// 現在の引数開始位置

		// 実引数リストを作成
		while (1)	{
			if (false);
			else if (*p == _T('\0'))	{
				return error(line, _T("マクロの解析中にファイルの終端が見つかりました。"));
			}
			//else if (*p == _T('\n'))	{
			//	return error(m_line, _T("マクロの解析中に改行が見つかりました。"));
			//}
			else if (*p == _T('\''))	{
				for (++p;; ++p)	{
					if (*p == _T('\''))	{ ++p;	break; }	// 終わりの ' が見つかったので抜ける
					if (*p == _T('\\'))	{ ++p; }	// \ が見つかったので１文字無視する
				}
			}
			else if (*p == _T('"'))	{
				for (++p;; ++p)	{
					if (*p == _T('"'))	{ ++p;	break; }	// 終わりの " が見つかったので抜ける
					if (*p == _T('\\'))	{ ++p; }	// \ が見つかったので１文字無視する
				}
			}
			else if (*p == _T('('))	{
				kakko_cnt++;
				++p;	// ( の次の文字まで進む
			}
			else if (*p == _T(','))	{
				if (kakko_cnt == 0)	{
					if (arg_start_pos == p)	{	// 引数が空ならエラー！
						return error(line, _T("マクロの") + tostr(arg_cnt) + _T("個目の引数が空です。"));
					}
					arg_list_real.push_back(TSTR(arg_start_pos, p));
					arg_start_pos = p + 1;
					++p;	// , の次の文字まで進む
				}
				else	{
					++p;	// , の次の文字まで進む
				}
			}
			else if (*p == _T(')'))	{
				if (kakko_cnt == 0)	{

					// １つ目の引数が空なら引数なし
					if (arg_start_pos == p && arg_list_real.size() == 0)	{
						++p;	// ) の次の文字まで進む
					}
					// ２つ目以降の引数が空ならエラー
					else if (arg_start_pos == p)	{
						return error(line, _T("マクロの") + tostr(arg_cnt) + _T("個目の引数が空です。"));
					}
					// 引数があれば引数をスタック
					else	{
						arg_list_real.push_back(TSTR(arg_start_pos, p));
						++p;	// ) の次の文字まで進む
					}
					break;
				}
				else	{
					kakko_cnt--;
					++p;	// ) の次の文字まで進む
				}
			}
			else	{
				++p;
			}
		}
	}

	// 引数の個数のチェック
	if (p_macro->arg_list.size() == 0 && arg_list_real.size() > 0)
		return error(line, _T("引数のいらないマクロには括弧は必要ありません。"));
	if (p_macro->arg_list.size() < arg_list_real.size())
		return error(line, _T("マクロの引数が多すぎます。"));

	// 引数展開
	if (!analize_macro_replace(text, &line, p_macro->after, p_macro->arg_list, arg_list_real, default_replace_tree, added_replace_tree, result_text))
		return false;

	// 成功
	*line_ret = line;
	p_ret = p;
	return true;
}

// ****************************************************************
// 文字解析：マクロの置換
// ================================================================
bool C_inc_file_analizer::analize_macro_replace(TSTR& text, int* line_ret, CTSTR& src_text, ARRAY<S_tnms_ia_macro_arg>& tmp_arg_list, ARRAY<TSTR>& real_arg_list, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree, TSTR& result_text)
{
	int line = *line_ret;
	result_text = src_text;

	// 引数の置換リストを作る
	ARRAY< BSP<S_tnms_ia_replace> > arg_replace_list;
	for (int a = 0; a < (int)tmp_arg_list.size(); a++)	{
		BSP<S_tnms_ia_replace> replace(new S_tnms_ia_replace);
		replace->type = TNMS_IA_REPLACE_TYPE_REPLACE;
		replace->name = tmp_arg_list[a].name;

		// 実引数がない場合デフォルト引数を与える
		if (a >= (int)real_arg_list.size())	{
			if (tmp_arg_list[a].def_value.empty())
				return error(line, _T("マクロの引数が足りません。"));
			replace->after = tmp_arg_list[a].def_value;
		}
		else	{
			replace->after = real_arg_list[a];
		}

		// リストに加える
		arg_replace_list.push_back(replace);
	}

	// 実引数を先に置換する
	// デフォルト引数があるので、実引数の確定したここでやらないといけないのです。
	for (ARRAY< BSP<S_tnms_ia_replace> >::iterator itr = arg_replace_list.begin(); itr != arg_replace_list.end(); ++itr)	{

		// 番兵の挿入
		for (int i = 0; i < 256; i++)
			(*itr)->after.push_back(_T('\0'));

		// 置換
		for (TSTR::iterator p = (*itr)->after.begin(); *p != _T('\0');)
			std_replace((*itr)->after, p, &line, default_replace_tree, added_replace_tree);

		// 番兵を削除
		(*itr)->after.erase((*itr)->after.end() - 256, (*itr)->after.end());
	}

	// 引数の置換リストを並び替える
	std::sort(arg_replace_list.begin(), arg_replace_list.end(), G_tnms_ia_replace_cmp_func);
	std::reverse(arg_replace_list.begin(), arg_replace_list.end());

	// 引数の置換ツリーを作る
	S_replace_node arg_replace_tree;
	C_replace_tree::create_replace_tree(arg_replace_tree, arg_replace_list);

	// 番兵の挿入
	for (int z = 0; z < 256; z++)
		result_text.push_back(_T('\0'));

	// マクロ内のマクロの置換を行う
	for (TSTR::iterator p = result_text.begin(); *p != _T('\0');)	{
		if (!std_replace(result_text, p, &line, default_replace_tree, arg_replace_tree))
			return false;
	}

	// 番兵を削除
	result_text.erase(result_text.end() - 256, result_text.end());

	// 成功
	*line_ret = line;
	return true;
}



// ****************************************************************
// inc 解析：メイン：ステップ２
// ================================================================
bool C_inc_file_analizer::ia_main_step_2()
{
	// パラメータの初期化
	m_error_line = -1;
	m_error_str.clear();
	int line = 1;

	// プロパティ
	int property_cnt = (int)m_piad2->property_temp_list.size();
	for (int i = 0; i < property_cnt; i++)	{
		m_text = m_piad2->property_temp_list[i];
		line = m_piad2->property_line_list[i];

		// まずはプロパティに対して置換を行う
		C_tnms_character_analizer ca;
		TSTR out_text;
		if (!ca.analize_line(m_text, m_piad, m_text))
			return error(line, ca.get_error_str());

		// 置換後のプロパティを解析
		TSTR::iterator p = m_text.begin();
		BSP<S_tnms_ia_declare> declare(new S_tnms_ia_declare);
		declare->Property = BSP<S_tnms_ia_property>(new S_tnms_ia_property);
		if (!ia_property_name(p, &line, declare->Property->name))										return false;
		if (!ia_declare_property_form(p, &line, &declare->Property->form, &declare->Property->size))	return false;
		TSTR declare_name = declare->Property->name;		// 重複判定用の名前

		// 重複判定
		if (m_piad->name_set.find(declare_name) != m_piad->name_set.end())
			return error(line, declare_name + _T(" が二重に宣言されています。"));
		m_piad->name_set.insert(declare_name);

		// プロパティが void 型ならエラー
		if (declare->Property->form == FM_VOID)
			return error(line, _T("void 型のプロパティは宣言できません。"));

		// プロパティに id をつける
		declare->Property->id = m_piad->property_cnt ++;

		// プロパティリストに追加
		m_piad->property_list.push_back(declare->Property);

		// ユーザプロパティを追加
		S_tnms_element element;
		element.type = TNMS_ELEMENT_TYPE_PROPERTY;
		element.code = create_elm_code(ELM_OWNER_USER_PROP, 0, declare->Property->id);
		element.name = declare->Property->name;
		element.form = declare->Property->form;

		// エレメントを追加
		S_tnms_form* form_global = m_piad->form_table.get_form_by_code(m_parent_form);
		form_global->add_element(&element);

		// グローバルの場合は個数を記憶しておく
		if (m_parent_form == FM_GLOBAL)
			m_piad->inc_property_cnt ++;
	}

	// コマンド
	int command_cnt = (int)m_piad2->command_temp_list.size();
	for (int i = 0; i < command_cnt; i++)	{
		m_text = m_piad2->command_temp_list[i];
		line = m_piad2->command_line_list[i];
		int org_line = line;

		// まずはコマンドに対して置換を行う
		C_tnms_character_analizer ca;
		TSTR out_text;
		if (!ca.analize_line(m_text, m_piad, m_text))
			return error(org_line, ca.get_error_str());

		// 置換後のプロパティを解析
		TSTR::iterator p = m_text.begin();
		BSP<S_tnms_ia_declare> declare(new S_tnms_ia_declare);
		declare->command = BSP<S_tnms_ia_command>(new S_tnms_ia_command);
		if (!ia_command_name(p, &line, declare->command->name))				return false;
		if (!ia_command_arg_list(p, &line, declare->command->arg_list))		return false;
		if (!ia_declare_form(p, &line, &declare->command->form))			return false;
		TSTR declare_name = declare->command->name;		// 重複判定用の名前

		// 重複判定
		if (m_piad->name_set.find(declare_name) != m_piad->name_set.end())
			return error(org_line, declare_name + _T(" が二重に宣言されています。"));
		m_piad->name_set.insert(declare_name);

		// コマンドに id をつける
		declare->command->id = m_piad->command_cnt ++;
		declare->command->is_defined = false;

		// コマンドリストに追加
		m_piad->command_list.push_back(declare->command);

		// ユーザコマンドを追加
		S_tnms_element element;
		element.type = TNMS_ELEMENT_TYPE_COMMAND;
		element.code = create_elm_code(ELM_OWNER_USER_CMD, 0, declare->command->id);
		element.name = declare->command->name;
		element.form = declare->command->form;
		element.add_arg_list(0, declare->command->arg_list);

		// エレメントを追加
		S_tnms_form* form_global = m_piad->form_table.get_form_by_code(m_parent_form);
		form_global->add_element(&element);

		// グローバルの場合は個数を記憶しておく
		if (m_parent_form == FM_GLOBAL)
			m_piad->inc_command_cnt ++;
	}

	return true;
}

// ****************************************************************
// inc 解析：エラーをセット
// ================================================================
bool C_inc_file_analizer::error(int line, CTSTR& str)
{
	if (m_error_str.empty())	{
		m_error_line = line;
		m_error_str = str;
	}

	return false;
}

// ****************************************************************
// inc 解析：エラーを取得
// ================================================================
TSTR C_inc_file_analizer::get_error_str()
{
	return m_error_str;
}

int C_inc_file_analizer::get_error_line()
{
	return m_error_line;
}



