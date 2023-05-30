#include	"pch.h"

#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"element/elm_call.h"

// ****************************************************************
// コール：初期化
// ================================================================
void C_elm_call::init(S_element element, CTSTR& name)
{
	// 基底クラスの初期化
	C_tnm_element::init(element, FM_CALL, name, NULL);

	// 子エレメントの初期化
	L.init(element + S_element(ELM_CALL_L), name + _T(".L"), Gp_ini->call_flag_cnt, true);		// コールフラグは可変長！
	K.init(element + S_element(ELM_CALL_K), name + _T(".K"), Gp_ini->call_flag_cnt, true);		// コールフラグは可変長！
	user_prop_list.init(S_element(), FM_VOID, _T("user_call_prop"), 0, true, NULL);

	reinit();
}

// ****************************************************************
// コール：再初期化
// ================================================================
void C_elm_call::reinit()
{
	L.reinit();
	K.reinit();
	user_prop_list.reinit();

	call_type = E_tnm_call_type_none;
	excall_flag = 0;
	frame_action_flag = 0;
	ret_form_code = FM_VOID;
	arg_cnt = 0;
}

// ****************************************************************
// コール：セーブ
// ================================================================
void C_elm_call::save(C_tnm_save_stream& stream)
{
	L.save(stream);
	K.save(stream);
	user_prop_list.save(stream);

	stream.save(call_type);
	stream.save(ret_form_code);

	stream.save(m_call_save.lexer_scn_name);
	stream.save(m_call_save.lexer_line_no);
	stream.save(m_call_save.lexer_prg_cntr);
}

// ****************************************************************
// コール：ロード
// ================================================================
void C_elm_call::load(C_tnm_save_stream& stream)
{
	L.load(stream);
	K.load(stream);
	user_prop_list.load(stream);

	stream.load(call_type);
	stream.load(ret_form_code);

	stream.load(m_call_save.lexer_scn_name);
	stream.load(m_call_save.lexer_line_no);
	stream.load(m_call_save.lexer_prg_cntr);
}

// ****************************************************************
// コールリスト：初期化
// ================================================================
void C_elm_call_list::init(S_element element, int form, CTSTR& name, int size, bool extend_enable, C_tnm_element* p_parent)
{
	m_call_cnt = size;

	// 基底クラスの初期化
	C_elm_list<C_elm_call>::init(element, form, name, size, extend_enable, p_parent);
}

// ****************************************************************
// コールリスト：再初期化
// ================================================================
void C_elm_call_list::reinit()
{
	m_call_cnt = m_def_size;

	// 基底クラスの再初期化
	C_elm_list<C_elm_call>::reinit();
}

// ****************************************************************
// コールリスト：セーブ
// ================================================================
void C_elm_call_list::save(C_tnm_save_stream& stream)
{
	// サイズ可変の場合
	stream.save(m_call_cnt);
	_save(stream, 0, m_call_cnt);
}

// ****************************************************************
// コールリスト：ロード
// ================================================================
void C_elm_call_list::load(C_tnm_save_stream& stream)
{
	// サイズ可変の場合
	m_call_cnt = stream.load_ret<int>();
	resize(m_call_cnt);
	_load(stream, 0, m_call_cnt);
}

// ****************************************************************
// コールリスト：コールの数を設定する
// ================================================================
void C_elm_call_list::set_call_cnt(int call_cnt)
{
	if (call_cnt > m_call_cnt)	{		// 増やす
		int add_cnt = call_cnt - m_call_cnt;
		for (int i = 0; i < add_cnt; i++)	{
			add_call();
		}
	}
	else if (call_cnt < m_call_cnt)	{	// 減らす
		int del_cnt = m_call_cnt - call_cnt;
		for (int i = 0; i < del_cnt; i++)	{
			sub_call();
		}
	}
}

// ****************************************************************
// コールリスト：コールを１つ増やす
// ================================================================
void C_elm_call_list::add_call()
{
	m_call_cnt ++;
	if (get_size() < m_call_cnt)	{
		resize(m_call_cnt);
	}
	else	{
		m_sub[m_call_cnt - 1].reinit();
	}
}

// ****************************************************************
// コールリスト：コールを１つ減らす
// ================================================================
void C_elm_call_list::sub_call()
{
	m_call_cnt --;
}

// ****************************************************************
// コールリスト：サブの初期化
// ================================================================
void C_elm_call_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"));
}

// ****************************************************************
// コールリスト：サブの再初期化
// ================================================================
void C_elm_call_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// コールリスト：サブのセーブ
// ================================================================
void C_elm_call_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// コールリスト：サブのロード
// ================================================================
void C_elm_call_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}
