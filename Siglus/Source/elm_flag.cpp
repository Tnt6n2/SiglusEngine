#include	"pch.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"data/tnm_lexer.h"
#include	"element/elm_flag.h"

// ****************************************************************
// フラグ：初期化（グローバル）
// ================================================================
void C_elm_flag::init_global()
{
	// グローバルフラグ
	G.init(S_element(ELM_GLOBAL_G), _T("G"), Gp_ini->global_flag_cnt, false);
	Z.init(S_element(ELM_GLOBAL_Z), _T("Z"), Gp_ini->global_flag_cnt, false);
	M.init(S_element(ELM_GLOBAL_M), _T("M"), Gp_ini->global_flag_cnt, false);

	// グローバル名前フラグ
	global_namae.init(S_element(ELM_GLOBAL_NAMAE_GLOBAL), _T("global namae"), 26 + 26 * 26, false);

	// ダミーチェックＩＤ
	dummy_check_id = 0;
}

// ****************************************************************
// フラグ：初期化（ローカル）
// ================================================================
void C_elm_flag::init_local()
{
	// ローカルフラグ
	A.init(S_element(ELM_GLOBAL_A), _T("A"), Gp_ini->flag_cnt, false);
	B.init(S_element(ELM_GLOBAL_B), _T("B"), Gp_ini->flag_cnt, false);
	C.init(S_element(ELM_GLOBAL_C), _T("C"), Gp_ini->flag_cnt, false);
	D.init(S_element(ELM_GLOBAL_D), _T("D"), Gp_ini->flag_cnt, false);
	E.init(S_element(ELM_GLOBAL_E), _T("E"), Gp_ini->flag_cnt, false);
	F.init(S_element(ELM_GLOBAL_F), _T("F"), Gp_ini->flag_cnt, false);
	X.init(S_element(ELM_GLOBAL_X), _T("X"), Gp_ini->flag_cnt, false);
	S.init(S_element(ELM_GLOBAL_S), _T("S"), Gp_ini->flag_cnt, false);

	// ローカル可変フラグ
	H.init(S_element(), _T("H"), 0, true);		// 可変長
	I.init(S_element(), _T("I"), 0, true);		// 可変長
	J.init(S_element(), _T("J"), 0, true);		// 可変長

	// ローカル名前フラグ
	local_namae.init(S_element(ELM_GLOBAL_NAMAE_LOCAL), _T("local namae"), 26 + 26 * 26, false);
}

// ****************************************************************
// フラグ：セーブ（グローバル）
// ================================================================
void C_elm_flag::save_global(C_tnm_save_stream& stream)
{
	G.save(stream);
	Z.save(stream);
	M.save(stream);

	// グローバル名前フラグ
	global_namae.save(stream);

	// ダミーチェックＩＤ
	stream.save(dummy_check_id);
}

// ****************************************************************
// フラグ：ロード（グローバル）
// ================================================================
void C_elm_flag::load_global(C_tnm_save_stream& stream)
{
	G.load(stream);
	Z.load(stream);
	M.load(stream);

	// グローバル名前フラグ
	global_namae.load(stream);

	// ダミーチェックＩＤ
	stream.load(dummy_check_id);
}

// ****************************************************************
// フラグ：セーブ（ローカル）
// ================================================================
void C_elm_flag::save_local(C_tnm_save_stream& stream)
{
	A.save(stream);
	B.save(stream);
	C.save(stream);
	D.save(stream);
	E.save(stream);
	F.save(stream);
	X.save(stream);
	S.save(stream);
	H.save(stream);
	I.save(stream);
	J.save(stream);

	// ローカル名前フラグ
	local_namae.save(stream);
}

// ****************************************************************
// フラグ：ロード（ローカル）
// ================================================================
void C_elm_flag::load_local(C_tnm_save_stream& stream)
{
	A.load(stream);
	B.load(stream);
	C.load(stream);
	D.load(stream);
	E.load(stream);
	F.load(stream);
	X.load(stream);
	S.load(stream);
	H.load(stream);
	I.load(stream);
	J.load(stream);

	// ローカル名前フラグ
	local_namae.load(stream);
}

// ****************************************************************
// フラグ：名前フラグを取得
// ================================================================
int get_zenkaku_alpha_no(TCHAR alpha)
{
	switch (alpha)	{
		case _T('Ａ'):	return 0;
		case _T('Ｂ'):	return 1;
		case _T('Ｃ'):	return 2;
		case _T('Ｄ'):	return 3;
		case _T('Ｅ'):	return 4;
		case _T('Ｆ'):	return 5;
		case _T('Ｇ'):	return 6;
		case _T('Ｈ'):	return 7;
		case _T('Ｉ'):	return 8;
		case _T('Ｊ'):	return 9;
		case _T('Ｋ'):	return 10;
		case _T('Ｌ'):	return 11;
		case _T('Ｍ'):	return 12;
		case _T('Ｎ'):	return 13;
		case _T('Ｏ'):	return 14;
		case _T('Ｐ'):	return 15;
		case _T('Ｑ'):	return 16;
		case _T('Ｒ'):	return 17;
		case _T('Ｓ'):	return 18;
		case _T('Ｔ'):	return 19;
		case _T('Ｕ'):	return 20;
		case _T('Ｖ'):	return 21;
		case _T('Ｗ'):	return 22;
		case _T('Ｘ'):	return 23;
		case _T('Ｙ'):	return 24;
		case _T('Ｚ'):	return 25;
	}

	return -1;
}

bool C_elm_flag::get_flag_by_name(CTSTR& name, C_elm_str_list** pp_flag, int* p_flag_no)
{
	if (name.length() < 2)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("名前フラグ名が短すぎます。"));
		return false;
	}
	if (name.length() > 3)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("名前フラグ名が長すぎます。"));
		return false;
	}

	// フラグを取得
	if (name[0] == _T('＊'))	{
		*pp_flag = &global_namae;
	}
	else if (name[0] == _T('％'))	{
		*pp_flag = &local_namae;
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("名前フラグの１文字目がが不正です。"));
		return false;
	}

	// フラグ番号を求める
	if (name.length() == 2)	{			// ％Ａ

		int alpha_no = get_zenkaku_alpha_no(name[1]);
		if (alpha_no == -1)	{
			tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("名前フラグの２文字目が不正です。"));
			return false;
		}

		*p_flag_no = alpha_no;
	}
	else if (name.length() == 3)	{	// ％ＡＡ

		int alpha_no_0 = get_zenkaku_alpha_no(name[1]);
		if (alpha_no_0 == -1)	{
			tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("名前フラグの２文字目がが不正です。"));
			return false;
		}

		int alpha_no_1 = get_zenkaku_alpha_no(name[2]);
		if (alpha_no_1 == -1)	{
			tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("名前フラグの３文字目がが不正です。"));
			return false;
		}

		*p_flag_no = alpha_no_0 * 26 + 26 + alpha_no_1;
	}

	return true;
}

S_element C_elm_flag::get_element_by_name(CTSTR& name)
{
	C_elm_str_list* p_flag = NULL;
	int flag_no;

	if (!get_flag_by_name(name, &p_flag, &flag_no))
		return S_element();

	return p_flag->get_element() + S_element(ELM_ARRAY, flag_no);
}

TSTR* C_elm_flag::get_namae_flag_by_name(CTSTR& name)
{
	C_elm_str_list* p_flag = NULL;
	int flag_no;

	if (!get_flag_by_name(name, &p_flag, &flag_no))
		return NULL;
	
	return p_flag->get_sub(flag_no, true);
}

// ****************************************************************
// ユーザインクプロパティ：初期化
// ================================================================
void C_elm_user_inc_prop::init()
{
	// フラグを作成
	int prop_cnt = Gp_lexer->get_user_inc_prop_cnt();
	prop_list.init(S_element(), FM_VOID, _T("inc_prop"), prop_cnt, false, NULL);

	// フラグの型を設定する
	for (int f = 0; f < prop_cnt; f++)	{
		prop_list[f].form = Gp_lexer->get_user_inc_prop_form(f);

		// 配列の場合は初期化
		if (false);
		else if (prop_list[f].form == FM_INTLIST)	{
			BSP<C_elm_int_list> int_list(new C_elm_int_list);
			int_list->init(S_element(), Gp_lexer->get_user_inc_prop_name(f), Gp_lexer->get_user_inc_prop_size(f), true);
			prop_list[f].any = int_list;
		}
		else if (prop_list[f].form == FM_STRLIST)	{
			BSP<C_elm_str_list> str_list(new C_elm_str_list);
			str_list->init(S_element(), Gp_lexer->get_user_inc_prop_name(f), Gp_lexer->get_user_inc_prop_size(f), true);
			prop_list[f].any = str_list;
		}
	}

	// フラグを再初期化
	prop_list.reinit();
}

// ****************************************************************
// ユーザインクプロパティ：再初期化
// ================================================================
void C_elm_user_inc_prop::reinit()
{
	prop_list.reinit();
}

// ****************************************************************
// ユーザインクプロパティ：セーブ
// ================================================================
void C_elm_user_inc_prop::save(C_tnm_save_stream& stream)
{
	prop_list.save(stream);
}

// ****************************************************************
// ユーザインクプロパティ：ロード
// ================================================================
void C_elm_user_inc_prop::load(C_tnm_save_stream& stream)
{
	prop_list.load(stream);
}

// ****************************************************************
// ユーザシーンプロパティ：初期化
// ================================================================
void C_elm_user_scn_prop::init(int scn_no)
{
	// ユーザシーンフラグを作成
	int prop_cnt = Gp_lexer->get_user_scn_prop_cnt(scn_no);
	prop_list.init(S_element(), FM_VOID, _T("scn_prop"), prop_cnt, false, NULL);

	// フラグの型を設定する
	for (int f = 0; f < prop_cnt; f++)	{
		prop_list[f].form = Gp_lexer->get_user_scn_prop_form(scn_no, f);

		// 配列の場合は初期化
		if (false);
		else if (prop_list[f].form == FM_INTLIST)	{
			BSP<C_elm_int_list> int_list(new C_elm_int_list);
			int_list->init(S_element(), Gp_lexer->get_user_scn_prop_name(scn_no, f), Gp_lexer->get_user_scn_prop_size(scn_no, f), true);
			prop_list[f].any = int_list;
		}
		else if (prop_list[f].form == FM_STRLIST)	{
			BSP<C_elm_str_list> str_list(new C_elm_str_list);
			str_list->init(S_element(), Gp_lexer->get_user_scn_prop_name(scn_no, f), Gp_lexer->get_user_scn_prop_size(scn_no, f), true);
			prop_list[f].any = str_list;
		}
	}

	// ユーザシーンフラグを再初期化
	prop_list.reinit();
}

// ****************************************************************
// ユーザシーンプロパティ：再初期化
// ================================================================
void C_elm_user_scn_prop::reinit()
{
	prop_list.reinit();
}

// ****************************************************************
// ユーザシーンプロパティ：セーブ
// ================================================================
void C_elm_user_scn_prop::save(C_tnm_save_stream& stream)
{
	prop_list.save(stream);
}

// ****************************************************************
// ユーザシーンプロパティ：ロード
// ================================================================
void C_elm_user_scn_prop::load(C_tnm_save_stream& stream)
{
	prop_list.load(stream);
}

// ****************************************************************
// ユーザシーンプロパティリスト：サブの初期化
// ================================================================
void C_elm_user_scn_prop_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(i);
}

// ****************************************************************
// ユーザシーンプロパティリスト：サブの再初期化
// ================================================================
void C_elm_user_scn_prop_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// ユーザシーンプロパティリスト：サブのセーブ
// ================================================================
void C_elm_user_scn_prop_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// ユーザシーンプロパティリスト：サブのロード
// ================================================================
void C_elm_user_scn_prop_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}

// ****************************************************************
// ユーザコールプロパティ
// ================================================================
void C_elm_user_call_prop::save(C_tnm_save_stream &stream)
{
	stream.save(scn_no);
	stream.save(prop_id);

	C_tnm_prop::save(stream);
}

void C_elm_user_call_prop::load(C_tnm_save_stream &stream)
{
	stream.load(scn_no);
	stream.load(prop_id);

	C_tnm_prop::load(stream);
}

// ****************************************************************
// ユーザコールプロパティリスト：サブの初期化
// ================================================================
void C_tnm_user_call_prop_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init();
}

// ****************************************************************
// ユーザコールプロパティリスト：サブの再初期化
// ================================================================
void C_tnm_user_call_prop_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// ユーザコールプロパティリスト：サブのセーブ
// ================================================================
void C_tnm_user_call_prop_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// ユーザコールプロパティリスト：サブのロード
// ================================================================
void C_tnm_user_call_prop_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}

