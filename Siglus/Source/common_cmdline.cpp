#include	<tona3.h>

using namespace NT3;

#include	"common_command_line.h"

// ****************************************************************
// コマンドライン解析クラス
// ================================================================
//		str:		コマンドライン文字列
//		cmd_list:	トークン分割結果
// ================================================================
bool C_command_line_analizer::analize_command_line(TSTR str, ARRAY<S_command_line_analizer_struct>& cmd_list)
{
	C_str_analizer anl(str.begin(), str.end());

	enum STATE
	{
		EMPTY,
		SLASH,
		KEY,
		EQUAL,
		VALUE,
	}	state = EMPTY;

	int cmd_no = 0;
	TSTR key, value;
	ARRAY<TSTR> value_list;
	while (1)	{

		// 文末の場合は特殊処理
		if (anl.is_end())	{
			if (state == EMPTY)	{
				return true;		// 成功！
			}
			else if (state == VALUE || state == KEY)	{

				// 現在のコマンドを登録
				S_command_line_analizer_struct cmd;
				cmd.key = key;
				cmd.value_list = value_list;
				cmd_list.push_back(cmd);

				// コマンドをクリア
				key.clear();
				value_list.clear();

				// 成功！
				return true;
			}
			else	{
				erorr_str = tostr(std::max(cmd_no, 1)) + _T("個目の引数の解析に失敗しました。");
				return false;		// エラー
			}
		}
		// まだ空の場合
		else if (state == EMPTY)	{
			if (anl.check_moji(_T('/')))	{
				state = SLASH;	// コマンドの '/' が見つかった（次のコマンドへ）
				cmd_no ++;
			}
			else	{
				erorr_str = tostr(std::max(cmd_no, 1)) + _T("個目の引数の解析に失敗しました。コマンドの最初は '/' で始めなければいけません。。");
				return false;		// エラー
			}
		}
		// '/' が見つかった後
		else if (state == SLASH)	{
			if (anl.get_word(key))	{
				state = KEY;	// コマンドのキーが見つかった
			}
			else	{
				erorr_str = _T("'/' に続く単語の解析に失敗しました。") + tostr(std::max(cmd_no, 1)) + _T("個目の引数の解析に失敗しました。");
				return false;		// エラー
			}
		}
		// キーが見つかった後
		else if (state == KEY)	{
			if (anl.check_moji(_T('/')))	{
				state = SLASH;	// コマンドの '/' が見つかった（次のコマンドへ）
				cmd_no ++;

				// 現在のコマンドを登録
				S_command_line_analizer_struct cmd;
				cmd.key = key;
				cmd.value_list = value_list;
				cmd_list.push_back(cmd);

				// コマンドをクリア
				key.clear();
				value_list.clear();
			}
			else if (anl.check_moji(_T('=')))	{
				state = EQUAL;	// コマンドの '=' が見つかった
			}
			else	{
				erorr_str = tostr(std::max(cmd_no, 1)) + _T("個目の引数の解析に失敗しました。コマンドの最初は '/' で始めなければいけません。");
				return false;		// エラー
			}
		}
		// '=' が見つかった後
		else if (state == EQUAL)	{
			if (anl.get_dquote_str(value, _T('\\')))	{
				state = VALUE;	// コマンドの値が見つかった

				// 値を登録
				value_list.push_back(value);
			}
			else if (anl.get_word(key))	{
				state = VALUE;	// コマンドの値が見つかった

				// 値を登録
				value_list.push_back(value);
			}
			else	{
				erorr_str = tostr(std::max(cmd_no, 1)) + _T("個目の引数の解析に失敗しました。'=' に続く値が見つかりません。");
				return false;		// エラー
			}
		}
		// 値が見つかった後
		else if (state == VALUE)	{
			if (anl.check_moji(_T('/')))	{
				state = SLASH;	// コマンドの '/' が見つかった（次のコマンドへ）
				cmd_no ++;

				// 現在のコマンドを登録
				S_command_line_analizer_struct cmd;
				cmd.key = key;
				cmd.value_list = value_list;
				cmd_list.push_back(cmd);

				// コマンドをクリア
				key.clear();
				value_list.clear();
			}
			else if (anl.get_dquote_str(value, _T('\\')))	{
				state = VALUE;	// コマンドの値が見つかった

				// 値を登録
				value_list.push_back(value);
			}
			else if (anl.get_word(key))	{
				state = VALUE;	// コマンドの値が見つかった

				// 値を登録
				value_list.push_back(value);
			}
			else	{
				erorr_str = tostr(std::max(cmd_no, 1)) + _T("個目の引数の解析に失敗しました。'=' に続く値が見つかりません。");
				return false;		// エラー
			}
		}
	}

	return true;
}
