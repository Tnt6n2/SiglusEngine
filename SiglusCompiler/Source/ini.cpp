#include	"pch.h"
#include	"ini.h"

// ****************************************************************
// ini ファイル解析
// ================================================================
bool C_ini_file_analizer::analize(TSTR src, TSTR& dst)
{
	TSTR result;

	if (!inia_comment_cut(src))
		return false;

	dst = src;
	return true;
}

bool C_ini_file_analizer::inia_comment_cut(TSTR& text)
{
	TSTR result;

	// 番兵を入れる
	for (int i = 0; i < 256; i++)
		text.push_back(_T('\0'));

	enum STATE
	{
		STATE_NONE,
		STATE_DOUBLE_QUOTE,			// ダブルコーテーション
		STATE_DOUBLE_QUOTE_ESCAPE,	// ダブルコーテーションエスケープ
		STATE_COMMENT_LINE,			// 行コメント
		STATE_COMMENT_BLOCK,		// ブロックコメント
	};
	
	int line = 1;						// 行番号（１行目からスタート）
	int block_comment_start_line = 1;	// ブロックコメントの開始行
	STATE state = STATE_NONE;			// 現在のステート

	for (TSTR::iterator p = text.begin(); *p != _T('\0'); ++p)	{

		TCHAR moji = *p;	// 出力文字

		// 改行の場合のみ特殊処理
		if (*p == _T('\n'))	{
			// ダブルコーテーション中ならエラー
			if (state == STATE_DOUBLE_QUOTE || state == STATE_DOUBLE_QUOTE_ESCAPE)
				return error(line, _T("ダブルコーテーション中は改行できません。"));
			// 行コメント中なら行コメント終了
			if (state == STATE_COMMENT_LINE)
				state = STATE_NONE;
			// 行数インクリメント
			line ++;
		}
		// ダブルコーテーション中
		else if (state == STATE_DOUBLE_QUOTE)	{
			// エスケープ
			if (*p == _T('\\'))
				state = STATE_DOUBLE_QUOTE_ESCAPE;
			// ダブルコーテーション終了
			if (*p == _T('"'))
				state = STATE_NONE;
		}
		// ダブルコーテーションエスケープ中
		else if (state == STATE_DOUBLE_QUOTE_ESCAPE)	{
			if (*p == _T('\\') || *p == _T('"'))
				state = STATE_DOUBLE_QUOTE;
			else
				return error(line, _T("不正なエスケープ（\\）です。'\\' を使うには '\\\\' と書いてください。"));
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
				block_comment_start_line = line;
				state = STATE_COMMENT_BLOCK;
				continue;
			}
			// アルファベット小文字を大文字に変換
			else if (_T('a') <= *p && *p <= _T('z'))
				moji += _T('A') - _T('a');
		}

		// 結果の出力
		result.push_back(moji);
	}

	// 終了時のステートによってエラー
	if (state == STATE_DOUBLE_QUOTE || state == STATE_DOUBLE_QUOTE_ESCAPE)
		return error(line, _T("閉じられていない \" があります。"));
	if (state == STATE_COMMENT_BLOCK)
		return error(block_comment_start_line, _T("閉じられていない /* があります。"));

	// 成功
	text = result;
	return true;
}

// ****************************************************************
// inc 解析：エラーの設定
// ================================================================
bool C_ini_file_analizer::error(int line, CTSTR& str)
{
	error_line = line;
	error_str = str;

	return false;
}

// ****************************************************************
// inc 解析：エラーの取得
// ================================================================
int C_ini_file_analizer::get_error_line()
{
	return error_line;
}

TSTR C_ini_file_analizer::get_error_str()
{
	return error_str;
}

