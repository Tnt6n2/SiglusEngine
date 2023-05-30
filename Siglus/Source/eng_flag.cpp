#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_lexer.h"
#include	"element/elm_flag.h"
#include	"engine/eng_flag.h"

// ****************************************************************
// フラグ：ユーザフラグＩＤからユーザプロパティを取得
// ================================================================
C_tnm_prop* tnm_flag_get_user_prop(int user_prop_id)
{
	C_tnm_prop* p_prop = NULL;

	// インクフラグの場合
	if (user_prop_id < Gp_lexer->get_user_inc_prop_cnt())	{
		p_prop = &Gp_user_inc_prop->prop_list[user_prop_id];
	}
	// シーンフラグの場合
	else	{
		user_prop_id -= Gp_lexer->get_user_inc_prop_cnt();
		p_prop = &(*Gp_user_scn_prop_list)[Gp_lexer->get_cur_scn_no()].prop_list[user_prop_id];
	}

	return p_prop;
}

// ****************************************************************
// フラグ：ダミーチェックをクリア
// ================================================================
void clear_dummy_check_flag()
{
	Gp_flag->dummy_check_id = 0;
}

// ****************************************************************
// フラグ：ダミーチェックを設定
// ================================================================
void set_dummy_check_flag(int value)
{
	Gp_flag->dummy_check_id = value;
}

// ****************************************************************
// フラグ：ダミーチェックフラグを判定
// ================================================================
bool check_dummy_check_flag(int value)
{
	return Gp_flag->dummy_check_id == value;
}
