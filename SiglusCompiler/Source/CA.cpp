#include	"pch.h"
#include	"CA.h"

// ****************************************************************
// 文字列比較
// ================================================================
bool check_str(TSTR::const_iterator& p_ret, CTSTR& str)
{
	TSTR::const_iterator p = p_ret;

	for (TSTR::const_iterator q = str.begin(); q != str.end(); ++p, ++q)	{
		if (*p != *q)
			return false;
	}

	p_ret = p;

	return true;
}

bool check_word(TSTR::const_iterator& p_ret, TSTR& word_ret)
{
	TSTR::const_iterator p = p_ret;

	while (*p == ' ' || *p == '\t')
		p++;

	TSTR::const_iterator word_start_p = p;

	if (*p == '_' || *p == '@' || is_alpha(*p) || is_zenkaku(*p)) {
		p++;

		while (*p == '_' || *p == '@' || is_alpha(*p) || is_num(*p) || is_zenkaku(*p))
			p++;

		word_ret = TSTR(word_start_p, p);
		p_ret = p;

		return true;

	}
	else {

		return false;
	}
}

// ****************************************************************
// 文字解析 - Character Analysis -
// ================================================================

bool C_tnms_character_analizer::analize_file_1(CTSTR& in_text_, TSTR& out_text)
{
	TSTR in_text = in_text_;

	// 番兵の挿入
	for (int i = 0; i < 256; i++)
		in_text.push_back(_T('\0'));

	// 出力用バッファの確保
	out_text.reserve(in_text.size() * 5);		// ＩＮテキストの５倍リザーブ

	// 解析用ステート
	enum STATE
	{
		STATE_NONE,
		STATE_SINGLE_QUOTE,			// シングルコーテーション
		STATE_SINGLE_QUOTE_ESCAPE,	// シングルコーテーションエスケープ
		STATE_SINGLE_QUOTE_END,		// シングルコーテーション終了
		STATE_DOUBLE_QUOTE,			// ダブルコーテーション
		STATE_DOUBLE_QUOTE_ESCAPE,	// ダブルコーテーションエスケープ
		STATE_COMMENT_LINE,			// 行コメント
		STATE_COMMENT_BLOCK,		// ブロックコメント
	};

	// 解析準備
	m_line = 1;							// 行番号（１行目からスタート）
	int block_comment_start_line = 1;	// ブロックコメントの開始行
	STATE state = STATE_NONE;			// 現在のステート

	// 解析開始！
	for (TSTR::const_iterator p = in_text.begin(); *p != _T('\0'); ++p)	{

		TCHAR moji = *p;	// 出力文字

		// 改行の場合のみ特殊処理
		if (*p == _T('\n'))	{
			// シングルコーテーション中ならエラー
			if (state == STATE_SINGLE_QUOTE || state == STATE_SINGLE_QUOTE_ESCAPE || state == STATE_SINGLE_QUOTE_END)
				return error(m_line, _T("シングルコーテーション中は改行できません。"));
			// ダブルコーテーション中ならエラー
			if (state == STATE_DOUBLE_QUOTE || state == STATE_DOUBLE_QUOTE_ESCAPE)
				return error(m_line, _T("ダブルコーテーション中は改行できません。"));
			// 行コメント中なら行コメント終了
			if (state == STATE_COMMENT_LINE)
				state = STATE_NONE;
			// 行数インクリメント
			m_line++;
		}
		// シングルコーテーション中
		else if (state == STATE_SINGLE_QUOTE)	{
			// エスケープ
			if (*p == _T('\\'))	{
				state = STATE_SINGLE_QUOTE_ESCAPE;
			}
			// シングルコーテーション
			else if (*p == _T('\''))	{
				return error(m_line, _T("' は必ず１文字を囲ってください。"));
			}
			else	{
				state = STATE_SINGLE_QUOTE_END;
			}
		}
		// シングルコーテーションエスケープ中
		else if (state == STATE_SINGLE_QUOTE_ESCAPE)	{
			if (*p == _T('\\') || *p == _T('\''))	{
				state = STATE_SINGLE_QUOTE_END;
			}
			else if (*p == _T('n'))	{
				state = STATE_SINGLE_QUOTE_END;
			}
			else	{
				return error(m_line, _T("不正なエスケープ（\\）です。'\\' を使うには '\\\\' と書いてください。"));
			}
		}
		// シングルコーテーション終了
		else if (state == STATE_SINGLE_QUOTE_END)	{
			// シングルコーテーション終了
			if (*p == _T('\''))	{
				state = STATE_NONE;
			}
			else	{
				return error(m_line, _T("' が閉じられていない、または２文字以上の文字を囲んでいます。"));
			}
		}
		// ダブルコーテーション中
		else if (state == STATE_DOUBLE_QUOTE)	{
			// エスケープ
			if (*p == _T('\\'))	{
				state = STATE_DOUBLE_QUOTE_ESCAPE;
			}
			// ダブルコーテーション終了
			else if (*p == _T('"'))	{
				state = STATE_NONE;
			}
		}
		// ダブルコーテーションエスケープ中
		else if (state == STATE_DOUBLE_QUOTE_ESCAPE)	{
			if (*p == _T('\\') || *p == _T('"'))	{
				state = STATE_DOUBLE_QUOTE;
			}
			else if (*p == _T('n'))	{
				state = STATE_DOUBLE_QUOTE;
			}
			else	{
				return error(m_line, _T("不正なエスケープ（\\）です。'\\' を使うには '\\\\' と書いてください。"));
			}
		}
		// 行コメント中
		else if (state == STATE_COMMENT_LINE)	{
			continue;	// 何もしない
		}
		// ブロックコメント中
		else if (state == STATE_COMMENT_BLOCK)	{
			// ブロックコメント終了
			if (*p == _T('*') && *(p + 1) == _T('/'))	{
				state = STATE_NONE;
				p++;
			}
			continue;	// 何もしない
		}
		// 通常処理
		else if (state == STATE_NONE)	{

			if (false);
			// シングルコーテーション開始
			else if (*p == _T('\''))
				state = STATE_SINGLE_QUOTE;
			// ダブルコーテーション開始
			else if (*p == _T('"'))
				state = STATE_DOUBLE_QUOTE;
			// 行コメント開始
			else if (*p == _T(';'))	{
				state = STATE_COMMENT_LINE;
				continue;
			}
			// 行コメント開始
			else if (*p == _T('/') && *(p + 1) == _T('/'))	{
				state = STATE_COMMENT_LINE;
				continue;
			}
			// ブロックコメント開始
			else if (*p == _T('/') && *(p + 1) == _T('*'))	{
				block_comment_start_line = m_line;
				state = STATE_COMMENT_BLOCK;
				continue;
			}
			// アルファベット大文字を小文字に変換
			else if (_T('A') <= *p && *p <= _T('Z'))
				moji += _T('a') - _T('A');
		}

		// 出力
		out_text.push_back(moji);
	}

	// 終了時のステートによってエラー
	if (state == STATE_SINGLE_QUOTE || state == STATE_SINGLE_QUOTE_ESCAPE || state == STATE_SINGLE_QUOTE_END)
		return error(m_line, _T("閉じられていない ' があります。"));
	if (state == STATE_DOUBLE_QUOTE || state == STATE_DOUBLE_QUOTE_ESCAPE)
		return error(m_line, _T("閉じられていない \" があります。"));
	if (state == STATE_COMMENT_BLOCK)
		return error(block_comment_start_line, _T("閉じられていない /* があります。"));

	return true;
}

bool C_tnms_character_analizer::analize_file_2(CTSTR& in_text_, TSTR& out_text, TSTR& inc_text)
{
	TSTR in_text = in_text_;

	// 番兵の挿入
	for (int i = 0; i < 256; i++)
		in_text.push_back(_T('\0'));

	// 出力用バッファの確保
	out_text.reserve(in_text.size() * 5);		// ＩＮテキストの５倍リザーブ
	inc_text.reserve(in_text.size() * 5);		// ＩＮテキストの５倍リザーブ

	// 解析用ステート
	enum STATE
	{
		STATE_NONE,
		STATE_SINGLE_QUOTE,			// シングルコーテーション
		STATE_SINGLE_QUOTE_ESCAPE,	// シングルコーテーションエスケープ
		STATE_SINGLE_QUOTE_END,		// シングルコーテーション終了
		STATE_DOUBLE_QUOTE,			// ダブルコーテーション
		STATE_DOUBLE_QUOTE_ESCAPE,	// ダブルコーテーションエスケープ
	};
	enum IF_STATE
	{
		IF_STATE_NONE = 0,
		IF_STATE_TRUE = 1,
		IF_STATE_FALSE = 2,
		IF_STATE_FALSE_MORE = 3,
	};

	// 解析準備
	m_line = 1;										// 行番号（１行目からスタート）
	STATE state = STATE_NONE;						// 現在のステート
	IF_STATE if_state[16] = { IF_STATE_NONE };		// 現在の if ステート
	int if_depth = 0;								// 現在の if の深さ
	bool inc_start = false;							// inc_start

	// 解析開始！
	for (TSTR::const_iterator p = in_text.begin(); *p != _T('\0'); )	{

		// 改行の場合のみ特殊処理
		if (*p == _T('\n'))	{
			// シングルコーテーション中ならエラー
			if (state == STATE_SINGLE_QUOTE || state == STATE_SINGLE_QUOTE_ESCAPE || state == STATE_SINGLE_QUOTE_END)
				return error(m_line, _T("シングルコーテーション中は改行できません。"));
			// ダブルコーテーション中ならエラー
			if (state == STATE_DOUBLE_QUOTE || state == STATE_DOUBLE_QUOTE_ESCAPE)
				return error(m_line, _T("ダブルコーテーション中は改行できません。"));
			// 行数インクリメント
			m_line++;
		}
		// シングルコーテーション中
		else if (state == STATE_SINGLE_QUOTE)	{
			// エスケープ
			if (*p == _T('\\'))	{
				state = STATE_SINGLE_QUOTE_ESCAPE;
			}
			// シングルコーテーション
			else if (*p == _T('\''))	{
				return error(m_line, _T("' は必ず１文字を囲ってください。"));
			}
			else	{
				state = STATE_SINGLE_QUOTE_END;
			}
		}
		// シングルコーテーションエスケープ中
		else if (state == STATE_SINGLE_QUOTE_ESCAPE)	{
			if (*p == _T('\\') || *p == _T('\''))	{
				state = STATE_SINGLE_QUOTE_END;
			}
			else if (*p == _T('n'))	{
				state = STATE_SINGLE_QUOTE_END;
			}
			else	{
				return error(m_line, _T("不正なエスケープ（\\）です。'\\' を使うには '\\\\' と書いてください。"));
			}
		}
		// シングルコーテーション終了
		else if (state == STATE_SINGLE_QUOTE_END)	{
			// シングルコーテーション終了
			if (*p == _T('\''))	{
				state = STATE_NONE;
			}
			else	{
				return error(m_line, _T("' が閉じられていない、または２文字以上の文字を囲んでいます。"));
			}
		}
		// ダブルコーテーション中
		else if (state == STATE_DOUBLE_QUOTE)	{
			// エスケープ
			if (*p == _T('\\'))	{
				state = STATE_DOUBLE_QUOTE_ESCAPE;
			}
			// ダブルコーテーション終了
			else if (*p == _T('"'))	{
				state = STATE_NONE;
			}
		}
		// ダブルコーテーションエスケープ中
		else if (state == STATE_DOUBLE_QUOTE_ESCAPE)	{
			if (*p == _T('\\') || *p == _T('"'))	{
				state = STATE_DOUBLE_QUOTE;
			}
			else if (*p == _T('n'))	{
				state = STATE_DOUBLE_QUOTE;
			}
			else	{
				return error(m_line, _T("不正なエスケープ（\\）です。'\\' を使うには '\\\\' と書いてください。"));
			}
		}
		// 通常処理
		else if (state == STATE_NONE)	{

			if (false);
			// シングルコーテーション開始
			else if (*p == _T('\''))
				state = STATE_SINGLE_QUOTE;
			// ダブルコーテーション開始
			else if (*p == _T('"'))
				state = STATE_DOUBLE_QUOTE;

			// #ifdef
			else if (check_str(p, _T("#ifdef"))) {

				TSTR word;
				if (check_word(p, word)) {

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
					return error(m_line, _T("#ifdef に続く単語がありません。"));
				}
			}
			// #elseifdef
			else if (check_str(p, _T("#elseifdef"))) {

				if (if_state[if_depth] > 0) {

					TSTR word;
					if (check_word(p, word)) {

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
						return error(m_line, _T("#elseifdef に続く単語がありません。"));
					}
				}
				else {
					return error(m_line, _T("この #elseifdef に対応する #if がありません。"));
				}
			}
			// #else
			else if (check_str(p, _T("#else"))) {

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
					return error(m_line, _T("この #else に対応する #if がありません。"));
				}
			}
			// #endif
			else if (check_str(p, _T("#endif"))) {

				if (if_state[if_depth] > 0) {
					--if_depth;
					continue;
				}
				else {
					return error(m_line, _T("この #endif に対応する #if がありません。"));
				}
			}
			// #inc_start
			else if (check_str(p, _T("#inc_start"))) {
				inc_start = true;
				continue;
			}
			// #inc_end
			else if (check_str(p, _T("#inc_end"))) {

				if (inc_start) {
					inc_start = false;
					continue;
				}
				else {
					return error(m_line, _T("この #inc_end に対応する #inc_start がありません。"));
				}
			}
		}

		// 出力
		if (*p == '\n') {
			if (inc_start)	{ inc_text.push_back(*p); }
			out_text.push_back(*p);		// 改行は必ず出力
		}
		else if (if_state[if_depth] == IF_STATE_NONE || if_state[if_depth] == IF_STATE_TRUE) {
			if (inc_start)	{ inc_text.push_back(*p); }
			else			{ out_text.push_back(*p); }
		}

		p++;
	}

	// 終了時のステートによってエラー
	if (state == STATE_SINGLE_QUOTE || state == STATE_SINGLE_QUOTE_ESCAPE || state == STATE_SINGLE_QUOTE_END)
		return error(m_line, _T("閉じられていない ' があります。"));
	if (state == STATE_DOUBLE_QUOTE || state == STATE_DOUBLE_QUOTE_ESCAPE)
		return error(m_line, _T("閉じられていない \" があります。"));
	if (inc_start)
		return error(m_line, _T("閉じられていない #inc_start があります。"));
	if (if_depth > 0)
		return error(m_line, _T("閉じられていない #ifdef があります。"));

	return true;
}


bool C_tnms_character_analizer::analize_file(CTSTR& in_text_, S_tnms_ia_data* piad, S_tnms_ca_data* pcad)
{
	TSTR in_text = in_text_;
	TSTR out_text;
	TSTR inc_text;

	// 初期化
	m_piad = piad;

	// ステップ１：コメントカット、小文字化
	if (!analize_file_1(in_text, out_text))
		return false;

	// テキストの入れ替え
	in_text = out_text;
	out_text.clear();

	// ステップ２：プリプロセッサ処理
	if (!analize_file_2(in_text, out_text, inc_text))
		return false;

	// テキストの入れ替え
	in_text = out_text;
	out_text.clear();

	// 番兵の挿入
	for (int z = 0; z < 256; z++)
		in_text.push_back(_T('\0'));

	// 出来上がった inc_text を inc analizer に通す
	C_inc_file_analizer ia;
	S_tnms_ia_data_2 ia_data_2;
	if (!ia.analize_step_1(inc_text, FM_SCENE, piad, &ia_data_2))
		return error(ia.get_error_line(), _T("inc: ") + ia.get_error_str());
	if (!ia.analize_step_2(piad, &ia_data_2))
		return error(ia.get_error_line(), _T("inc: ") + ia.get_error_str());

	// inc ファイルによる置換
	m_line = 1;
	int rest_length_min = (int)in_text.size();
	int loop_replace_check_cnt = 0;
	for (TSTR::iterator p = in_text.begin(); *p != _T('\0');)	{

		// 改行
		if (*p == _T('\n'))	{
			m_line ++;
			p ++;
		}
		// 通常の文字
		else	{
			// コマンドによる置換
			if (!std_replace(in_text, p, m_piad->replace_tree, S_replace_node()))
				return false;
		}

		// 残り長さを求める（置換などにより残り長さは伸びることがある）
		int rest_length = in_text.end() - p;

		// ★無限ループチェック
		if (rest_length >= rest_length_min)	{
			loop_replace_check_cnt ++;
			if (loop_replace_check_cnt > 10000)	{
				return error(m_line, _T("inc ファイルによる置換中に無限ループが発生しました。"));
			}
		}
		else	{
			rest_length_min = rest_length;
			loop_replace_check_cnt = 0;
		}
	}

	// 結果を作成する
	pcad->scn_text = in_text;

	return true;
}

// ****************************************************************
// 文字解析：置換
// ================================================================
bool C_tnms_character_analizer::std_replace(TSTR& text, TSTR::iterator& p, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree)
{
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
		TSTR::iterator define_start_pos = p;
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
		TSTR::iterator define_start_pos = p;
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
		TSTR::iterator macro_start_pos = p;
		int macro_start_pos_int = (int)(macro_start_pos - text.begin());

		// 置換する文字列の名前部分の最後に移動する
		p += name_len;
		// マクロを解析する
		if (!analize_macro(p, p_replace.get(), default_replace_tree, added_replace_tree, result_text))
			return false;
		// 解析後の文字列に置換
		text.replace(macro_start_pos, p, result_text);
		// 解析ポインタの再設定
		p = text.begin() + macro_start_pos_int + result_text.size();
	}

	return true;
}

// ****************************************************************
// 文字解析：マクロの解析
// ================================================================
bool C_tnms_character_analizer::analize_macro(TSTR::iterator& p, S_tnms_ia_replace* p_macro, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree, TSTR& result_text)
{
	ARRAY<TSTR> arg_list_real;			// 実引数リスト
	int kakko_cnt = 0;					// 現在の括弧の数
	int arg_cnt = 0;					// 引数の個数

	// 括弧がある場合
	if (*p == _T('('))	{
		p++;

		TSTR::iterator arg_start_pos = p;	// 現在の引数開始位置

		// 実引数リストを作成
		while (1)	{
			if (false);
			else if (*p == _T('\0'))	{
				return error(m_line, _T("マクロの解析中にファイルの終端が見つかりました。"));
			}
			//else if (*p == _T('\n'))	{
			//	return error(m_line, _T("マクロの解析中に改行が見つかりました。"));
			//}
			else if (*p == _T('\''))	{
				for (++p; ; ++p)	{
					if (*p == _T('\''))	{	++p;	break;	}	// 終わりの ' が見つかったので抜ける
					if (*p == _T('\\'))	{	++p;			}	// \ が見つかったので１文字無視する
				}
			}
			else if (*p == _T('"'))	{
				for (++p; ; ++p)	{
					if (*p == _T('"'))	{	++p;	break;	}	// 終わりの " が見つかったので抜ける
					if (*p == _T('\\'))	{	++p;			}	// \ が見つかったので１文字無視する
				}
			}
			else if (*p == _T('('))	{
				kakko_cnt ++;
				++p;	// ( の次の文字まで進む
			}
			else if (*p == _T(','))	{
				if (kakko_cnt == 0)	{
					if (arg_start_pos == p)	{	// 引数が空ならエラー！
						return error(m_line, _T("マクロの") + tostr(arg_cnt) + _T("個目の引数が空です。"));
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
						return error(m_line, _T("マクロの") + tostr(arg_cnt) + _T("個目の引数が空です。"));
					}
					// 引数があれば引数をスタック
					else	{
						arg_list_real.push_back(TSTR(arg_start_pos, p));
						++p;	// ) の次の文字まで進む
					}
					break;
				}
				else	{
					kakko_cnt --;
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
		return error(m_line, _T("引数のいらないマクロには括弧は必要ありません。"));
	if (p_macro->arg_list.size() < arg_list_real.size())
		return error(m_line, _T("マクロの引数が多すぎます。"));

	// 引数展開
	if (!analize_macro_replace(p_macro->after, p_macro->arg_list, arg_list_real, default_replace_tree, added_replace_tree, result_text))
		return false;

	return true;
}

// ****************************************************************
// 文字解析：マクロの置換
// ================================================================
bool C_tnms_character_analizer::analize_macro_replace(CTSTR& src_text, ARRAY<S_tnms_ia_macro_arg>& tmp_arg_list, ARRAY<TSTR>& real_arg_list, S_replace_node& default_replace_tree, S_replace_node& added_replace_tree, TSTR& result_text)
{
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
				return error(m_line, _T("マクロの引数が足りません。"));
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
			std_replace((*itr)->after, p, default_replace_tree, added_replace_tree);

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
		if (!std_replace(result_text, p, default_replace_tree, arg_replace_tree))
			return false;
	}

	// 番兵を削除
	result_text.erase(result_text.end() - 256, result_text.end());

	return true;
}

// ****************************************************************
// 文字解析：１行を解析
// ================================================================
bool C_tnms_character_analizer::analize_line(CTSTR& in_text_, S_tnms_ia_data* piad, TSTR& out_text)
{
	TSTR in_text = in_text_;

	// 情報のコピー
	m_piad = piad;

	// 番兵の挿入
	for (int i = 0; i < 1; i++)
		in_text.push_back(_T('\0'));

	// inc ファイルによる置換
	m_line = 1;
	int loop_replace_check_cnt = 0;
	int rest_length_min = (int)in_text.size();
	for (TSTR::iterator p = in_text.begin(); *p != _T('\0');)	{
		TSTR::iterator last_p = p;

		// 改行
		if (*p == _T('\n'))	{
			m_line ++;
			p ++;
		}
		// 通常の文字
		else	{
			// コマンドによる置換
			if (!std_replace(in_text, p, m_piad->replace_tree, S_replace_node()))
				return false;
		}

		// 残り長さを求める（置換などにより残り長さは伸びることがある）
		int rest_length = in_text.end() - p;

		// ★無限ループチェック
		if (rest_length >= rest_length_min)	{
			loop_replace_check_cnt ++;
			if (loop_replace_check_cnt > 10000)	{
				return error(m_line, _T("inc ファイルによる置換中に無限ループが発生しました。"));
			}
		}
		else	{
			rest_length_min = rest_length;
			loop_replace_check_cnt = 0;
		}
	}

	// 番兵を削除
	in_text.erase(in_text.end() - 1, in_text.end());

	// 結果を作成する
	out_text = in_text;

	return true;
}

// ****************************************************************
// 文字解析：エラーのセット
// ================================================================
bool C_tnms_character_analizer::error(int line, CTSTR& str)
{
	error_line = line;
	error_str = str;

	return false;
}
