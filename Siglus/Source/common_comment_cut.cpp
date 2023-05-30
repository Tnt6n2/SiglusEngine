#include	"pch.h"
#include	"common_comment_cut.h"


// ****************************************************************
// コメントカット
// ================================================================
bool C_ini_comment_cut::comment_cut(ARRAY<TSTR>& in_txt_data)
{
	// エラーの初期化
	error_line_no = -1;
	error_str.clear();

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
	
	STATE state = STATE_NONE;				// 現在のステート
	int block_comment_start_line_no = 1;	// ブロックコメントの開始行
	int line_no = 1;						// 現在の行番号

	ARRAY<TSTR> out_txt_data;
	for (int i = 0; i < (int)in_txt_data.size(); i++)	{
		line_no = i + 1;
		TSTR in_line_data = in_txt_data[i];
		TSTR out_line_data;
		for (TSTR::const_iterator p = in_line_data.begin(); ; ++p)	{

			// 改行の場合のみ特殊処理
			if (p == in_line_data.end())	{
				// シングルコーテーション中ならエラー
				if (state == STATE_SINGLE_QUOTE || state == STATE_SINGLE_QUOTE_ESCAPE)
					return error(line_no, _T("シングルコーテーション (') 中に改行が見つかりました。"));
				// ダブルコーテーション中ならエラー
				if (state == STATE_DOUBLE_QUOTE || state == STATE_DOUBLE_QUOTE_ESCAPE)
					return error(line_no, _T("ダブルコーテーション (\") 中に改行が見つかりました。"));
				// 行コメント中なら行コメント終了
				if (state == STATE_COMMENT_LINE)
					state = STATE_NONE;
				// 次の行へ
				break;
			}

			TCHAR moji = *p;	// 出力文字

			if (false);
			// シングルコーテーション中
			else if (state == STATE_SINGLE_QUOTE)	{
				// シングルコーテーション終了
				if (*p == _T('\''))
					state = STATE_NONE;
				// シングルコーテーションエスケープ
				else if (*p == _T('\\'))
					state = STATE_SINGLE_QUOTE_ESCAPE;
			}
			// シングルコーテーションエスケープ中
			else if (state == STATE_SINGLE_QUOTE_ESCAPE)	{
				// エスケープ可能な文字
				if (*p == _T('\'') || *p == _T('\\'))
					state = STATE_SINGLE_QUOTE;
				else
					return error(line_no, _T("不正なエスケープ（\\）です。'\\' を使うには '\\\\' と書いて下さい。"));
			}
			// ダブルコーテーション中
			else if (state == STATE_DOUBLE_QUOTE)	{
				// ダブルコーテーション終了
				if (*p == _T('"'))
					state = STATE_NONE;
				// ダブルコーテーションエスケープ
				else if (*p == _T('\\'))
					state = STATE_DOUBLE_QUOTE_ESCAPE;
			}
			// ダブルコーテーションエスケープ中
			else if (state == STATE_DOUBLE_QUOTE_ESCAPE)	{
				// エスケープ可能な文字
				if (*p == _T('"') || *p == _T('\\'))
					state = STATE_DOUBLE_QUOTE;
				else
					return error(line_no, _T("不正なエスケープ（\\）です。'\\' を使うには '\\\\' と書いて下さい。"));
			}
			// 行コメント中
			else if (state == STATE_COMMENT_LINE)	{
				continue;	// 何もしない
			}
			// ブロックコメント中
			else if (state == STATE_COMMENT_BLOCK)	{
				// ブロックコメント終了
				if (str_analize_check_str(p, in_line_data.end(), _T("*/"), false))	{
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
				else if (str_analize_check_str(p, in_line_data.end(), _T("//"), false))	{
					state = STATE_COMMENT_LINE;
					continue;
				}
				// ブロックコメント開始
				else if (str_analize_check_str(p, in_line_data.end(), _T("/*"), false))	{
					block_comment_start_line_no = line_no;
					state = STATE_COMMENT_BLOCK;
					continue;
				}
			}

			// 出力
			out_line_data.push_back(moji);
		}
		// 出力
		out_txt_data.push_back(out_line_data);
	}

	// 終了時のステートによってエラー
	if (state == STATE_SINGLE_QUOTE)
		return error(line_no, _T("シングルコーテーション (') が閉じられていません。"));
	if (state == STATE_DOUBLE_QUOTE)
		return error(line_no, _T("ダブルコーテーション (\") が閉じられていません。"));
	if (state == STATE_COMMENT_BLOCK)
		return error(block_comment_start_line_no, _T(" コメント (/*) が閉じられていません。"));

	// 成功
	in_txt_data = out_txt_data;
	return true;
}

bool C_ini_comment_cut::error(int line_no, CTSTR& str)
{
	error_line_no = line_no;
	error_str = str;

	return false;
}

