#include	"pch.h"

#include	"tnm_code.h"
#include	"ifc_eng.h"

#include	"data/tnm_global_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_lexer.h"

#include	"element/elm_prop.h"
#include	"element/elm_flag.h"
#include	"element/elm_call.h"

#include	"engine/ifc_stack.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_flag.h"
#include	"engine/eng_message.h"

#include	"engine/eng_scene.h"
#include	"engine/flow_command.h"
#include	"engine/flow_script.h"

#include	<list>


// ****************************************************************
// プロトタイプ宣言
// ================================================================
void	tnm_calculate_1(int form, BYTE opr);							// 単項演算
void	tnm_calculate_2(int form_l, int form_r, BYTE opr);				// 二項演算
void	tnm_calculate_2_str_int(BYTE opr);								// 二項演算：文字列 op 整数
void	tnm_calculate_2_str_str(BYTE opr);								// 二項演算：文字列 op 文字列
bool	tnm_declare_call_prop(int form_code, int prop_id, int size);	// コールプロパティを宣言
bool	tnm_expand_arg_into_call_flag();								// 引数をコールプロパティに展開

// ****************************************************************
// フロー：スクリプト処理
// ================================================================
//		スクリプトを解析していきます。
// ================================================================

// ================================================================
// 超重要！！！！！
// スクリプト実行用のデータを確保するのにかなりの時間がかかります。
// 使いまわすことで高速化を図ります。
// ただし、エンドアクションにより再帰的に呼ばれる可能性があります。
// 再帰の深さを記憶しておき、適切に確保しつつ使います。
// ----------------------------------------------------------------

bool tnm_proc_script_sub();

// スクリプトプロセスを（再帰的に）呼び出した回数
static int _ex_call_cnt = 0;

// スクリプトプロセスを実行するためのデータ構造体
struct S_script_proc_data
{
	// エレメント
	S_element		element_set;

	// 引数の型コードリスト
	ARRAY<int>		arg_form_code_set;

	// 引数リスト
	C_tnm_prop_list	arg_set;
};

// スクリプトプロセスを実行するためのデータ
// 確保に時間がかかるので使いまわす
// ポインタで保持されるので vector では動かない。list を使うこと。
static std::list<S_script_proc_data>	_ex_script_proc_data;

// ****************************************************************
// フロー：スクリプト処理本体
// ================================================================
bool tnm_proc_script()
{
	// ゲーム終了時は何もしない（フレームアクションが複数設定されたときはここで抜けることもあるよ！）
	if (Gp_global->game_end_flag)
		return true;

	// 再帰的に呼ばれるので呼ばれた回数をカウント
	_ex_call_cnt ++;

	// 呼ばれた回数分準備が出来ていなければ準備
	if ((int)_ex_script_proc_data.size() < _ex_call_cnt)	{
	
		const int ARG_CNT_MAX = 1024;

		// スクリプト実行用のデータを確保
		_ex_script_proc_data.resize(_ex_call_cnt);

		// 各メンバを準備する
		_ex_script_proc_data.back().arg_form_code_set.resize(ARG_CNT_MAX);
		_ex_script_proc_data.back().arg_set.resize(ARG_CNT_MAX);
		for (int i = 0; i < ARG_CNT_MAX; i++)	{
			_ex_script_proc_data.back().arg_set[i].exp_list.resize(8);			// ▲ワイプオプションの最大数が８個なので 4 から 8 に変更しました。
		}
	}

	// 本体を呼ぶ
	bool ret = tnm_proc_script_sub();

	// 呼ばれた回数を減らす
	_ex_call_cnt --;

	return ret;
}

bool tnm_proc_script_sub()
{
	// 今回使うスクリプト実行用のデータを取り出す
	std::list<S_script_proc_data>::iterator itr = _ex_script_proc_data.begin();
	for (int i = 0; i < _ex_call_cnt - 1; i++)
		++ itr;
	S_element& _ex_element = itr->element_set;
	ARRAY<int>& _ex_arg_form_code_list = itr->arg_form_code_set;
	C_tnm_prop_list& _ex_arg_list = itr->arg_set;

	// ↓間違い
	//S_element& _ex_element = _ex_script_proc_data.back().element_set;
	//ARRAY<int>& _ex_arg_form_code_list = _ex_script_proc_data.back().arg_form_code_set;
	//C_tnm_prop_list& _ex_arg_list = _ex_script_proc_data.back().arg_set;

	// ----------------------------------------------------------------

	// スクリプト処理を行う
	// 高速化を図るため、よく通るものから順に並べます。

	while (Gp_local->cur_proc.type == TNM_PROC_TYPE_SCRIPT)	{
		T_tnm_code code = Gp_lexer->pop_ret<BYTE>();		// スクリプトコード

		if (code == CD_PUSH)	{			// stack push

			// スタックに値を push します。

			int form_code = Gp_lexer->pop_ret<int>();
			if (form_code == FM_INT)		tnm_stack_push_int(Gp_lexer->pop_ret<int>());
			else if (form_code == FM_STR)	tnm_stack_push_str(Gp_lexer->pop_str_ret());
		}
		else if (code == CD_PROPERTY)	{		// プロパティ値を取得

			// スタックの末尾のエレメントを値に変換します。
			// 例：front.object[0].x → 100

			// エレメントをスタックから取得
			int elm_cnt = tnm_stack_pop_element(_ex_element);
			// コマンドを実行
			if (!tnm_command_proc(_ex_element.begin(), _ex_element.end()))
				return true;
		}
		else if (code == CD_OPERATE_2)	{		// 二項演算

			// スタック内で二項演算を行います。

			int form_l = Gp_lexer->pop_ret<int>();
			int form_r = Gp_lexer->pop_ret<int>();
			BYTE opr = Gp_lexer->pop_ret<BYTE>();
			tnm_calculate_2(form_l, form_r, opr);
		}
		else if (code == CD_ELM_POINT)	{		// エレメントポイントを立てる
			Gp_stack->stack_point_list.push_back(Gp_stack->int_now_cnt);
		}
		else if (code == CD_ASSIGN)	{			// プロパティ値を設定

			// 代入処理を行います。

			// 型を取得
			int left_form = Gp_lexer->pop_ret<int>();
			int right_form = Gp_lexer->pop_ret<int>();
			int al_id = Gp_lexer->pop_ret<int>();
			// スタックから引数を取得
			tnm_stack_pop_arg_list(_ex_arg_list, right_form);
			// エレメントをスタックから取得
			int elm_cnt = tnm_stack_pop_element(_ex_element);
			// コマンドを実行（引数は１つで固定なので、_ex_arg_list.begin() ～ _ex_arg_list.begin() + 1 までを渡す）
			tnm_command_proc(_ex_element.begin(), _ex_element.end(), al_id, _ex_arg_list.get_sub().get(), _ex_arg_list.get_sub().get() + 1, NULL, FM_VOID);
		}
		else if (code == CD_NL)	{				// 改行

			// 改行コードが見つかりました。
			// ステップ実行中の場合は、いったん処理を止めます。

			// ステップ実行中の場合
			int old_line_no = Gp_lexer->get_cur_line_no();
			int line_no = Gp_lexer->pop_ret<int>();
			Gp_lexer->set_cur_line_no(line_no);
			if (Gp_global->is_breaking && Gp_global->break_step_flag)	{
				// 更新された場合、処理を抜ける
				if (Gp_lexer->get_cur_line_no() != old_line_no)	{
					Gp_global->break_step_flag = false;		// ステップ実行終了
					Gp_global->disp_flag = false;			// ステップ実行による処理抜けは描画しない
					return true;
				}
			}
		}
		else if (code == CD_COMMAND)	{		// コマンドを実行

			// 引数リストＩＤを取得
			int arg_list_id = Gp_lexer->pop_ret<int>();
			// スタックから引数を取得
			int arg_cnt = tnm_stack_pop_arg_list(_ex_arg_list, _ex_arg_form_code_list);
			// エレメントをスタックから取得
			int elm_cnt = tnm_stack_pop_element(_ex_element);
			// 名前つき引数の個数
			int named_arg_cnt = Gp_lexer->pop_ret<int>();
			// 名前つき引数リストの引数ＩＤを取得
			for (int a = 0; a < named_arg_cnt; a++)	{
				_ex_arg_list[arg_cnt - a - 1].id = Gp_lexer->pop_ret<int>();
			}
			// 戻り値の型を取得
			int ret_form_code = Gp_lexer->pop_ret<int>();
			// コマンドを実行
			tnm_command_proc(_ex_element.begin(), _ex_element.end(), arg_list_id, 
				_ex_arg_list.get_sub().get(), 
				_ex_arg_list.get_sub().get() + arg_cnt - named_arg_cnt, 
				_ex_arg_list.get_sub().get() + arg_cnt,
				ret_form_code);
		}
		else	{

			switch (code)	{

				case CD_GOTO:			// goto
				{
					// 無条件でラベルにジャンプします。

					int label_no = Gp_lexer->pop_ret<int>();
					Gp_lexer->jump_to_label(label_no);
				}
				break;

				case CD_GOTO_FALSE:		// goto false
				{
					// 結果が偽の場合のみラベルにジャンプします。

					int cond = tnm_stack_pop_int();
					int label_no = Gp_lexer->pop_ret<int>();
					if (cond == 0)	// 偽の場合
						Gp_lexer->jump_to_label(label_no);
				}
				break;

				case CD_GOTO_TRUE:		// goto true
				{
					// 結果が真の場合のみラベルにジャンプします。

					int cond = tnm_stack_pop_int();
					int label_no = Gp_lexer->pop_ret<int>();
					if (cond != 0)	// 真の場合
						Gp_lexer->jump_to_label(label_no);
				}
				break;

				case CD_GOSUB:			// gosub
				{
					// gosub の処理を行います。戻り値は int 型です。

					tnm_command_proc_gosub(_ex_arg_list, _ex_arg_form_code_list, FM_INT);
				}
				break;

				case CD_GOSUBSTR:		// gosub str
				{
					// gosub の処理を行います。戻り値は str 型です。

					tnm_command_proc_gosub(_ex_arg_list, _ex_arg_form_code_list, FM_STR);
				}
				break;

				case CD_RETURN:			// return
				{
					// return の処理を行います。

					if (!tnm_command_proc_return(_ex_arg_list, _ex_arg_form_code_list))
						return true;
				}
				break;

				case CD_POP:			// stack pop
				{
					// スタックから値を pop します。

					int form_code = Gp_lexer->pop_ret<int>();
					switch (form_code)	{
						case FM_INT:	tnm_stack_pop_int();	break;
						case FM_STR:	tnm_stack_pop_str();	break;
					}
				}
				break;

				case CD_COPY:			// stack copy
				{
					// スタックの末尾の値を複製します。+= などの処理に使います。

					int form_code = Gp_lexer->pop_ret<int>();
					switch (form_code)	{
						case FM_INT:	tnm_stack_push_int(tnm_stack_back_int());		break;
						case FM_STR:	tnm_stack_push_str(tnm_stack_back_str());		break;
					}
				}
				break;

				case CD_COPY_ELM:		// stack copy element
				{
					// スタックの末尾のエレメントを複製します。

					tnm_stack_copy_element(_ex_element);
				}
				break;

				case CD_DEC_PROP:		// プロパティを定義
				{
					// 変数定義を行います。

					int form_code = Gp_lexer->pop_ret<int>();
					int prop_id = Gp_lexer->pop_ret<int>();
					int size = 0;
					if (form_code == FM_INTLIST || form_code == FM_STRLIST)	{
						size = tnm_stack_pop_int();
					}

					tnm_declare_call_prop(form_code, prop_id, size);
				}
				break;

				case CD_ARG:			// 引数を展開する
				{
					// 引数をコールフラグに展開します。

					tnm_expand_arg_into_call_flag();
				}
				break;

				case CD_OPERATE_1:		// 単項演算
				{
					// スタック内で単項演算を行います。

					int form = Gp_lexer->pop_ret<int>();
					BYTE opr = Gp_lexer->pop_ret<BYTE>();
					tnm_calculate_1(form, opr);
				}
				break;

				case CD_NAME:			// 名前
				{
					tnm_msg_proc_name(tnm_get_cur_mwnd(), tnm_stack_pop_str());
				}
				break;

				case CD_TEXT:			// テキスト
				{
					int rf_flag_no = Gp_lexer->pop_ret<int>();	// 既読フラグのフラグ番号を取得
					int rf_scn_no = Gp_lexer->get_cur_scn_no();	// 既読フラグのシーン番号は現在のシーン番号
					TSTR msg = tnm_stack_pop_str();				// 文字列

					tnm_msg_proc_print(tnm_get_cur_mwnd(), msg, rf_scn_no, rf_flag_no, true);
				}
				break;

				case CD_NONE:			// エラー
				{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("スクリプトの解析に失敗しました。"));
					tnm_push_proc(TNM_PROC_TYPE_NONE);
				}
				break;

				case CD_EOF:			// ファイル終端
				{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("ファイルの終端に到達しました。"));
					tnm_push_proc(TNM_PROC_TYPE_NONE);
				}
			}
		}
	}

	return true;
}


// ****************************************************************
// 単項演算
// ================================================================
//		単項演算を行い、結果をスタックに積みます。
//			[+, -]
// ================================================================
void tnm_calculate_1(int form, BYTE opr)
{
	if (false);
	else if (form == FM_INT)	{
		int rhs = tnm_stack_pop_int();			// 右値

		switch (opr)	{
			case OP_PLUS:		tnm_stack_push_int(+ rhs);		break;
			case OP_MINUS:		tnm_stack_push_int(- rhs);		break;
			case OP_TILDE:		tnm_stack_push_int(~ rhs);		break;
		}
	}
}

// ****************************************************************
// 二項演算
// ================================================================
//		二項演算を行い、結果をスタックに積みます。
//			[+, -, *, /, %, ==, !=, >, >=, <, <=, &&, ||]
// ================================================================
void tnm_calculate_2(int form_l, int form_r, BYTE opr)
{
	if (false);

	// 整数 op 整数
	else if (form_l == FM_INT && form_r == FM_INT)	{
		int rhs = tnm_stack_pop_int();			// 右値を取り出す
		int lhs = tnm_stack_pop_int();			// 左値を取り出す
		switch (opr)	{
			case OP_PLUS:			tnm_stack_push_int(lhs + rhs);				break;
			case OP_MINUS:			tnm_stack_push_int(lhs - rhs);				break;
			case OP_MULTIPLE:		tnm_stack_push_int(lhs * rhs);				break;
			case OP_EQUAL:			tnm_stack_push_int(lhs == rhs ? 1 : 0);		break;
			case OP_NOT_EQUAL:		tnm_stack_push_int(lhs != rhs ? 1 : 0);		break;
			case OP_GREATER:		tnm_stack_push_int(lhs >  rhs ? 1 : 0);		break;
			case OP_GREATER_EQUAL:	tnm_stack_push_int(lhs >= rhs ? 1 : 0);		break;
			case OP_LESS:			tnm_stack_push_int(lhs <  rhs ? 1 : 0);		break;
			case OP_LESS_EQUAL:		tnm_stack_push_int(lhs <= rhs ? 1 : 0);		break;
			case OP_LOGICAL_AND:	tnm_stack_push_int((lhs && rhs) ? 1 : 0);	break;
			case OP_LOGICAL_OR:		tnm_stack_push_int((lhs || rhs) ? 1 : 0);	break;

			case OP_AND:			tnm_stack_push_int(lhs & rhs);				break;
			case OP_OR:				tnm_stack_push_int(lhs | rhs);				break;
			case OP_HAT:			tnm_stack_push_int(lhs ^ rhs);				break;
			case OP_SL:				tnm_stack_push_int(lhs << rhs);				break;
			case OP_SR:				tnm_stack_push_int(lhs >> rhs);				break;

			// 右論理シフト
			// int の右シフトは算術シフトだが、UINT にキャストすることで、論理シフトになる。
			// 結果は int に戻す。
			case OP_SR3:			tnm_stack_push_int((int)((UINT)lhs >> (UINT)rhs));		break;

			// 割り算は０除算があるため特殊処理
			case OP_DIVIDE:
				if (rhs == 0)	{
					tnm_set_error(TNM_ERROR_TYPE_DIVIDED_BY_ZERO, _T("0 除算を行いました！'/'"));
					tnm_stack_push_int(0);	// 強制的に演算結果を０とする
				}
				else	{
					tnm_stack_push_int(lhs / rhs);
				}
				break;

			// 余り算は０除算があるため特殊処理
			case OP_AMARI:
				if (rhs == 0)	{
					tnm_set_error(TNM_ERROR_TYPE_DIVIDED_BY_ZERO, _T("0 除算を行いました！'%'"));
					tnm_stack_push_int(0);	// 強制的に演算結果を０とする
				}
				else	{
					tnm_stack_push_int(lhs % rhs);
				}
				break;
		}
	}

	// 文字列 op 整数
	else if (form_l == FM_STR && form_r == FM_INT)	{	tnm_calculate_2_str_int(opr);	}

	// 文字列 op 文字列
	else if (form_l == FM_STR && form_r == FM_STR)	{	tnm_calculate_2_str_str(opr);	}
}

// 二項演算：文字列 op 整数
void tnm_calculate_2_str_int(BYTE opr)
{
	int rhs = tnm_stack_pop_int();		// 右値を取り出す
	TSTR lhs = tnm_stack_pop_str();		// 左値を取り出す
	switch (opr)	{
		case OP_MULTIPLE:		tnm_stack_push_str(lhs * rhs);			break;
	}
}

// 二項演算：文字列 op 文字列
void tnm_calculate_2_str_str(BYTE opr)
{
	TSTR rhs = tnm_stack_pop_str();		// 右値を取り出す
	TSTR lhs = tnm_stack_pop_str();		// 左値を取り出す
	switch (opr)	{
		case OP_PLUS:			tnm_stack_push_str(lhs + rhs);			break;
		case OP_EQUAL:			tnm_stack_push_int(str_to_lower_ret(lhs) == str_to_lower_ret(rhs) ? 1 : 0);	break;
		case OP_NOT_EQUAL:		tnm_stack_push_int(str_to_lower_ret(lhs) != str_to_lower_ret(rhs) ? 1 : 0);	break;
		case OP_GREATER:		tnm_stack_push_int(str_to_lower_ret(lhs) >  str_to_lower_ret(rhs) ? 1 : 0);	break;
		case OP_GREATER_EQUAL:	tnm_stack_push_int(str_to_lower_ret(lhs) >= str_to_lower_ret(rhs) ? 1 : 0);	break;
		case OP_LESS:			tnm_stack_push_int(str_to_lower_ret(lhs) <  str_to_lower_ret(rhs) ? 1 : 0);	break;
		case OP_LESS_EQUAL:		tnm_stack_push_int(str_to_lower_ret(lhs) <= str_to_lower_ret(rhs) ? 1 : 0);	break;
	}
}

// ****************************************************************
// gosub 処理
// ================================================================
bool tnm_command_proc_gosub(C_tnm_prop_list& arg_list, ARRAY<int>& arg_form_code_list, int ret_form_code)
{
	// シーンからラベル番号を取得
	int label_no = Gp_lexer->pop_ret<int>();

	// スタックから引数リストを取得
	int arg_cnt = tnm_stack_pop_arg_list(arg_list, arg_form_code_list);

	// コール
	if (!tnm_scene_proc_gosub(label_no, ret_form_code))
		return false;

	// 引数をコール変数に配置
	C_elm_call* p_cur_call = tnm_get_cur_call();
	int l_cnt = 0;
	int k_cnt = 0;
	for (int i = 0; i < arg_cnt; i++)	{
		if (arg_list[i].form == FM_INT)		p_cur_call->L.set_value(l_cnt++, arg_list[i].Int);
		if (arg_list[i].form == FM_STR)		p_cur_call->K.set_value(k_cnt++, arg_list[i].str);
	}

	return true;
}

// ****************************************************************
// farcall 処理
// ================================================================
bool tnm_command_proc_farcall(int al_id, C_tnm_prop* al_begin, C_tnm_prop* al_end, int ret_form_code)
{
	TSTR scn_name = al_begin[0].str;	// シーン名
	int z_no = 0;
	switch (al_id)	{
		case 1:		z_no = al_begin[1].Int;
		case 0:		scn_name = al_begin[0].str;
	}

	// コール
	if (!tnm_scene_proc_farcall(scn_name, z_no, ret_form_code, false, false))
		return false;

	// 引数をコール変数に配置
	if (al_id == 1)	{
		C_elm_call* p_cur_call = tnm_get_cur_call();
		int l_cnt = 0;
		int k_cnt = 0;
		for (C_tnm_prop* arg = al_begin + 2; arg != al_end; ++arg)	{	// 2 = コールフラグ以外の引数
			if (arg->form == FM_INT)	p_cur_call->L.set_value(l_cnt++, arg->Int);
			if (arg->form == FM_STR)	p_cur_call->K.set_value(k_cnt++, arg->str);
		}
	}

	return true;
}

// ****************************************************************
// farcall_ex 処理
// ================================================================
bool tnm_command_proc_farcall_ex(int al_id, C_tnm_prop* al_begin, C_tnm_prop* al_end, int ret_form_code)
{
	TSTR scn_name = al_begin[0].str;	// シーン名
	int z_no = 0;
	switch (al_id)	{
		case 1:		z_no = al_begin[1].Int;
		case 0:		scn_name = al_begin[0].str;
	}

	// コール
	if (!tnm_scene_proc_farcall(scn_name, z_no, ret_form_code, true, false))
		return false;

	// 引数をコール変数に配置
	if (al_id == 1)	{
		C_elm_call* p_cur_call = tnm_get_cur_call();
		int l_cnt = 0;
		int k_cnt = 0;
		for (C_tnm_prop* arg = al_begin + 2; arg != al_end; ++arg)	{	// 2 = コールフラグ以外の引数
			if (arg->form == FM_INT)	p_cur_call->L.set_value(l_cnt++, arg->Int);
			if (arg->form == FM_STR)	p_cur_call->K.set_value(k_cnt++, arg->str);
		}
	}

	return true;
}

// ****************************************************************
// return 処理
// ================================================================
bool tnm_command_proc_return(C_tnm_prop_list& arg_list, ARRAY<int>& arg_form_code_list)
{
	// スタックから引数リストを取得
	int arg_cnt = tnm_stack_pop_arg_list(arg_list, arg_form_code_list);

	// リターン元のコール情報を記憶しておく（ロードした時点で消えてしまうため）
	C_elm_call* p_src_call = tnm_get_cur_call();
	int call_type = p_src_call->call_type;
	int frame_action_flag = p_src_call->frame_action_flag;

	// リターン
	if (!tnm_scene_proc_return())
		return false;

	// リターン先のコール情報を取得
	C_elm_call* p_dst_call = tnm_get_cur_call();
	int ret_form_code = p_dst_call->ret_form_code;

	// 戻り値の型と実戻り値の型をチェック
	// 型が違う場合はダミーの戻り値を入れる
	if (false);
	else if (ret_form_code == FM_INT)	{	// 整数を返す
		if (arg_cnt == 1 && arg_list[0].form == FM_INT)		tnm_stack_push_int(arg_list[0].Int);
		else												tnm_stack_push_int(0);
	}
	else if (ret_form_code == FM_STR)	{	// 文字列を返す
		if (arg_cnt == 1 && arg_list[0].form == FM_STR)		tnm_stack_push_str(arg_list[0].str);
		else												tnm_stack_push_str(_T(""));
	}

	// フレームアクションの場合はループを抜ける！
	if (frame_action_flag == 1)
		return false;

	return true;
}

// ****************************************************************
// ユーザコマンド処理
// ================================================================
bool tnm_command_proc_user_cmd(int user_cmd_id, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	// ユーザコマンド処理
	if (!tnm_scene_proc_call_user_cmd(Gp_lexer->get_cur_scn_no(), user_cmd_id, p_ai->ret_form_code, false, false))
		return false;

	// 引数をスタックに積む
	for (C_tnm_prop* arg = p_ai->al_begin; arg < p_ai->al_end; arg++)	{
		if (false);
		else if (arg->form == FM_INT)	{
			tnm_stack_push_int(arg->Int);
		}
		else if (arg->form == FM_STR)	{
			tnm_stack_push_str(arg->str);
		}
		else	{
			tnm_stack_push_element(arg->element);
		}
	}

	return true;
}

// ****************************************************************
// ユーザプロパティ処理
// ================================================================
bool tnm_command_proc_user_prop(int user_prop_id, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{	
	C_tnm_prop* p_prop;

	// インクフラグの場合
	if (user_prop_id < Gp_lexer->get_user_inc_prop_cnt())	{
		p_prop = &Gp_user_inc_prop->prop_list[user_prop_id];
	}
	// シーンフラグの場合
	else	{
		user_prop_id -= Gp_lexer->get_user_inc_prop_cnt();
		p_prop = &(*Gp_user_scn_prop_list)[Gp_lexer->get_cur_scn_no()].prop_list[user_prop_id];
	}

	// プロパティ処理
	if (!tnm_command_proc_prop(p_prop, elm_top, elm_begin, elm_end, p_ai))
		return false;

	return true;
}

// ****************************************************************
// コールフラグ処理
// ================================================================
bool tnm_command_proc_call_prop(int call_prop_id, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	C_elm_call* p_cur_call = tnm_get_cur_call();
	C_tnm_prop* p_prop = &p_cur_call->user_prop_list[call_prop_id];

	// プロパティ処理
	if (!tnm_command_proc_prop(p_prop, elm_top, elm_begin, elm_end, p_ai))
		return false;


	return true;
}

// ****************************************************************
// プロパティ処理
// ================================================================
bool tnm_command_proc_prop(C_tnm_prop* p_prop, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int al_id = p_ai->al_id;

	// 型を求める
	int form = p_prop->form;

	// 型によって処理を分岐
	if (false);
	else if (form == FM_INT && al_id == 0)	{
		tnm_stack_push_int(p_prop->Int);
	}
	else if (form == FM_INT && al_id == 1)	{
		p_prop->Int = p_ai->al_begin[0].Int;
	}
	else if (form == FM_STR)	{
		tnm_command_proc_str(p_prop->str, elm_top, elm_begin, elm_end, p_ai);
	}
	else if (form == FM_INTLIST)	{
		tnm_command_proc_int_list(boost::any_cast< BSP<C_elm_int_list> >(p_prop->any).get(), 32, elm_top, elm_begin, elm_end, p_ai);
	}
	else if (form == FM_STRLIST)	{
		tnm_command_proc_str_list(boost::any_cast< BSP<C_elm_str_list> >(p_prop->any).get(), elm_top, elm_begin, elm_end, p_ai);
	}
	else if (form == FM_INTREF || form == FM_STRREF)	{	// 参照外し（$a = 0 -> a[0] = 0）
		tnm_stack_push_element(p_prop->element);
	}
	else if (form == FM_INTLISTREF || form == FM_STRLISTREF)	{	// 参照外し（$a = 0 -> a[0] = 0）
		tnm_stack_push_element(p_prop->element);
	}
	else if (elm_begin != elm_end)	{						// 参照外し（$obj.x -> front.object[0].x）
		tnm_stack_push_element(p_prop->element);
	}
	else if (al_id == 0)	{								// 参照を取得
		tnm_stack_push_element(p_prop->element);
	}
	else if (al_id == 1)	{								// 参照を代入
		p_prop->element = p_ai->al_begin[0].element;
	}

	return true;
}

// ****************************************************************
// コールプロパティを定義
// ================================================================
bool tnm_declare_call_prop(int form_code, int prop_id, int size)
{
	// 現在のコールを取得
	C_elm_call* p_cur_call = tnm_get_cur_call();

	// コールプロパティを作成
	int old_cnt = p_cur_call->user_prop_list.get_size();
	p_cur_call->user_prop_list.resize(old_cnt + 1);
	C_elm_user_call_prop* p_prop = p_cur_call->user_prop_list.get_sub(old_cnt, true);

	// コールプロパティを初期化
	p_prop->scn_no = Gp_lexer->get_cur_scn_no();
	p_prop->prop_id = prop_id;
	p_prop->form = form_code;

	// 配列の場合は初期化
	if (false);
	else if (form_code == FM_INTLIST)	{
		BSP<C_elm_int_list> int_list(new C_elm_int_list);
		TSTR name = Gp_lexer->get_user_call_prop_name(p_prop->scn_no, p_prop->prop_id);
		int_list->init(S_element(), name, size, true);
		p_prop->any = int_list;
	}
	else if (form_code == FM_STRLIST)	{
		BSP<C_elm_str_list> str_list(new C_elm_str_list);
		TSTR name = Gp_lexer->get_user_call_prop_name(p_prop->scn_no, p_prop->prop_id);
		str_list->init(S_element(), name, size, true);
		p_prop->any = str_list;
	}

	return true;
}


// ****************************************************************
// 引数をコールフラグに展開する
// ================================================================
bool tnm_expand_arg_into_call_flag()
{
	// 現在のコールを取得
	C_elm_call* p_cur_call = tnm_get_cur_call();

	// フレームアクションの場合は引数チェック
	// ▲本来は型チェックを行う必要がある！
	if (p_cur_call->frame_action_flag == 1)	{

		// 引数チェック
		if (p_cur_call->user_prop_list.get_size() == 0 || p_cur_call->user_prop_list[0].form != FM_FRAMEACTION)	{
			TSTR str = _T("フレームアクションの引数の第一引数は frameaction 型でなければいけません。（新しい機能）");
			TSTR cmd_name = Gp_lexer->get_user_cmd_name(p_cur_call->scn_no, p_cur_call->cmd_no);
			tnm_set_error(TNM_ERROR_TYPE_FATAL, str + _T("\n(") + cmd_name + _T(")"));
			return false;
		}
		// 引数の個数が違う
		if (p_cur_call->arg_cnt != p_cur_call->user_prop_list.get_size())	{
			TSTR str = _T("フレームアクションの引数の個数が不正です。");
			TSTR cmd_name = Gp_lexer->get_user_cmd_name(p_cur_call->scn_no, p_cur_call->cmd_no);
			tnm_set_error(TNM_ERROR_TYPE_FATAL, str + _T("\n(") + cmd_name + _T(")"));
			return false;
		}
	}

	// 逆順に積まれているので逆順に読み込む
	for (int i = (int)p_cur_call->user_prop_list.get_size() - 1; i >= 0; i--)	{

		if (false);
		else if (p_cur_call->user_prop_list[i].form == FM_INT)	{
			p_cur_call->user_prop_list[i].Int = tnm_stack_pop_int();
		}
		else if (p_cur_call->user_prop_list[i].form == FM_STR)	{
			p_cur_call->user_prop_list[i].str = tnm_stack_pop_str();
		}
		else	{	// 参照型
			tnm_stack_pop_element(p_cur_call->user_prop_list[i].element);
		}
	}

	return true;
}
