#pragma		once

struct		S_element;
class		C_tnm_prop;
class		C_tnm_prop_list;

// ****************************************************************
// スタック：構造体
// ================================================================
class C_tnm_stack
{
public:

	C_tnm_stack()
	{
		int_p = NULL;
		int_now_cnt = 0;
		int_alloc_cnt = 0;
	}

	// 整数型のスタック
	int*				int_p;
	DWORD				int_now_cnt;
	DWORD				int_alloc_cnt;

	// 文字列型のスタック
	ARRAY<TSTR>			str_stack;

	// エレメントコードの先頭位置を記憶するスタック
	// エレメントコードをスタック内でコピーしたりするのに使う
	ARRAY<int>			stack_point_list;
};

// ****************************************************************
// スタック：インターフェース
// ================================================================
void		tnm_stack_realloc(int cnt);						// スタック確保
void		tnm_stack_push_int(int value);					// スタックに int 値を出力
int			tnm_stack_pop_int();							// スタックから int 値を取得
int			tnm_stack_back_int();							// スタックから末尾の int 値を参照
void		tnm_stack_push_str(CTSTR& str);					// スタックに文字列を出力
TSTR		tnm_stack_pop_str();							// スタックから文字列を取得
TSTR		tnm_stack_back_str();							// スタックから末尾の文字列を参照
void		tnm_stack_push_element(S_element& element);		// スタックにエレメントを出力
int			tnm_stack_pop_element(S_element& element);		// スタックからエレメントを取得
void		tnm_stack_copy_element(S_element& element);		// スタック内でエレメントをコピー
void		tnm_stack_pop_arg_list(C_tnm_prop_list& arg_list, int arg_form_code);					// スタックから引数を展開（型コードを１つ指定する）
int			tnm_stack_pop_arg_list(C_tnm_prop_list& arg_list, ARRAY<int>& arg_form_code_list);		// スタックから引数を展開（型コードはシーンから取得する）

