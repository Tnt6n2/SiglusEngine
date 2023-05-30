#include	"pch.h"

#include	"tnm_form_code.h"
#include	"ifc_eng.h"

#include	"data/tnm_lexer.h"
#include	"element/elm_prop.h"
#include	"engine/ifc_stack.h"

// ****************************************************************
// スタック：インターフェース：スタック再確保
// ================================================================
void tnm_stack_realloc(int cnt)
{
	if (cnt >= (int)Gp_stack->int_alloc_cnt)	{
		while (cnt >= (int)Gp_stack->int_alloc_cnt)	{
			Gp_stack->int_alloc_cnt += 1024;
		}
		Gp_stack->int_p = (int *)::HeapReAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, Gp_stack->int_p, Gp_stack->int_alloc_cnt * sizeof(int));
	}
}

// ****************************************************************
// スタック：インターフェース：スタックに int 値を出力
// ================================================================
void tnm_stack_push_int(int value)
{
	if (Gp_stack->int_now_cnt >= Gp_stack->int_alloc_cnt)	{
		while (Gp_stack->int_now_cnt >= Gp_stack->int_alloc_cnt)	{
			Gp_stack->int_alloc_cnt += 1024;
		}
		Gp_stack->int_p = (int *)::HeapReAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, Gp_stack->int_p, Gp_stack->int_alloc_cnt * sizeof(int));
	}
	*(Gp_stack->int_p + Gp_stack->int_now_cnt) = value;
	Gp_stack->int_now_cnt++;
}

// ****************************************************************
// スタック：インターフェース：スタックから int 値を取得
// ================================================================
int tnm_stack_pop_int()
{
	Gp_stack->int_now_cnt--;
	return *(Gp_stack->int_p + Gp_stack->int_now_cnt);
}

// ****************************************************************
// スタック：インターフェース：スタックから末尾の int 値を参照
// ================================================================
int tnm_stack_back_int()
{
	return *(Gp_stack->int_p + Gp_stack->int_now_cnt - 1);
}

// ****************************************************************
// スタック：インターフェース：スタックに文字列を出力
// ================================================================
void tnm_stack_push_str(CTSTR& str)
{
	Gp_stack->str_stack.push_back(str);
}

// ****************************************************************
// スタック：インターフェース：スタックから文字列を取得
// ================================================================
TSTR tnm_stack_pop_str()
{
	TSTR ret_value = Gp_stack->str_stack.back();
	Gp_stack->str_stack.pop_back();
	return ret_value;
}

// ****************************************************************
// スタック：インターフェース：スタックから末尾の文字列を参照
// ================================================================
TSTR tnm_stack_back_str()
{
	return Gp_stack->str_stack.back();
}

// ****************************************************************
// スタック：インターフェース：スタックにエレメントを出力
// ================================================================
void tnm_stack_push_element(S_element& element)
{
	// ヒープを拡張
	if (Gp_stack->int_now_cnt + element.code_cnt + 1 >= Gp_stack->int_alloc_cnt)	{
		Gp_stack->int_alloc_cnt += 1024 + element.code_cnt + 1;
		Gp_stack->int_p = (int *)::HeapReAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, Gp_stack->int_p, Gp_stack->int_alloc_cnt * sizeof(int));
	}

	// スタックポイントリストにポイントを追加
	Gp_stack->stack_point_list.push_back(Gp_stack->int_now_cnt);

	// エレメントコードを正順に出力
	int *dp = Gp_stack->int_p + Gp_stack->int_now_cnt;
	for (int i = 0; i < element.code_cnt; i++)
		*dp++ = element.code[i];
	// スタックを調整
	Gp_stack->int_now_cnt += element.code_cnt;// + 1;
}

// ****************************************************************
// スタック：インターフェース：スタックからエレメントを取得
// ================================================================
int tnm_stack_pop_element(S_element& element)
{
	// エレメントコードの長さを取得
	int elm_cnt = Gp_stack->int_now_cnt - Gp_stack->stack_point_list.back();
	Gp_stack->stack_point_list.pop_back();

	// コピーポイントを設定
	int *sp = Gp_stack->int_p + Gp_stack->int_now_cnt - elm_cnt;
	int *dp = element.code;

	// コピー
	for (int i = 0; i < elm_cnt; i++)
		*dp++ = *sp++;

	// エレメント長
	element.code_cnt = elm_cnt;

	// スタックを調整
	Gp_stack->int_now_cnt -= elm_cnt;// + 1;

	return elm_cnt;
}

// ****************************************************************
// スタック：インターフェース：スタック内でエレメントをコピー
// ================================================================
void tnm_stack_copy_element(S_element& element)
{
	// エレメントの長さを取得
	int elm_cnt = Gp_stack->int_now_cnt - Gp_stack->stack_point_list.back();

	// ヒープを拡張
	if (Gp_stack->int_now_cnt + elm_cnt + 1 >= Gp_stack->int_alloc_cnt)	{
		Gp_stack->int_alloc_cnt += 1024 + elm_cnt + 1;
		Gp_stack->int_p = (int *)::HeapReAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, Gp_stack->int_p, Gp_stack->int_alloc_cnt * sizeof(int));
	}

	// コピーポイントを設定
	int* sp = Gp_stack->int_p + Gp_stack->int_now_cnt - elm_cnt;
	int* dp = Gp_stack->int_p + Gp_stack->int_now_cnt;

	// コピー
	for (int i = 0; i < elm_cnt; i++)
		*dp++ = *sp++;

	// スタックポイントリストにポイントを追加
	Gp_stack->stack_point_list.push_back(Gp_stack->int_now_cnt);

	// スタック長を調整
	Gp_stack->int_now_cnt += elm_cnt;
}

// ****************************************************************
// スタック：インターフェース：引数リストをスタックから取得
// ================================================================
//		スタックには、第一引数から順に push されています。
//		最後の引数から取り出していくことになるため、逆順に並び替えます。
// ================================================================
void tnm_stack_pop_arg_list(C_tnm_prop_list& arg_list, int arg_form_code)
{
	if (false);
	else if (arg_form_code == FM_INT)	{
		arg_list[0].form = FM_INT;
		arg_list[0].Int = tnm_stack_pop_int();
	}
	else if (arg_form_code == FM_STR)	{
		arg_list[0].form = FM_STR;
		arg_list[0].str = tnm_stack_pop_str();
	}
	else	{
		arg_list[0].form = arg_form_code;
		tnm_stack_pop_element(arg_list[0].element);
	}
}

int tnm_stack_pop_arg_list(C_tnm_prop_list& arg_list, ARRAY<int>& arg_form_code_list)
{
	// 引数の型コードの個数をシーンから取得
	int arg_cnt = Gp_lexer->pop_ret<int>();

	// 引数を順に取得する
	for (int i = arg_cnt - 1; i >= 0; i--)	{

		// 引数IDはまだ分からないので初期化しておく
		arg_list[i].id = -1;

		// 引数の型を取得する
		arg_form_code_list[i] = Gp_lexer->pop_ret<int>();

		// 引数の型を頼りに引数を取得する
		if (false);
		else if (arg_form_code_list[i] == FM_INT)	{
			arg_list[i].form = FM_INT;
			arg_list[i].Int = tnm_stack_pop_int();
		}
		else if (arg_form_code_list[i] == FM_STR)	{
			arg_list[i].form = FM_STR;
			arg_list[i].str = tnm_stack_pop_str();
		}
		else if (arg_form_code_list[i] == FM_LABEL)	{
			arg_list[i].form = FM_INT;
			arg_list[i].Int = tnm_stack_pop_int();
		}
		else if (arg_form_code_list[i] == FM_LIST)	{
			ARRAY<int> exp_form_code_list(64);	// ▲これもできれば外部で宣言したいところ…

			arg_list[i].form = FM_LIST;
			arg_list[i].exp_cnt = tnm_stack_pop_arg_list(arg_list[i].exp_list, exp_form_code_list);
		}
		else	{
			arg_list[i].form = arg_form_code_list[i];
			tnm_stack_pop_element(arg_list[i].element);
		}
	}

	return arg_cnt;
}

