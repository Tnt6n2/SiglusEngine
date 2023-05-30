#include	"pch.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"data/tnm_lexer.h"
#include	"element/elm_flag.h"

// ****************************************************************
// �t���O�F�������i�O���[�o���j
// ================================================================
void C_elm_flag::init_global()
{
	// �O���[�o���t���O
	G.init(S_element(ELM_GLOBAL_G), _T("G"), Gp_ini->global_flag_cnt, false);
	Z.init(S_element(ELM_GLOBAL_Z), _T("Z"), Gp_ini->global_flag_cnt, false);
	M.init(S_element(ELM_GLOBAL_M), _T("M"), Gp_ini->global_flag_cnt, false);

	// �O���[�o�����O�t���O
	global_namae.init(S_element(ELM_GLOBAL_NAMAE_GLOBAL), _T("global namae"), 26 + 26 * 26, false);

	// �_�~�[�`�F�b�N�h�c
	dummy_check_id = 0;
}

// ****************************************************************
// �t���O�F�������i���[�J���j
// ================================================================
void C_elm_flag::init_local()
{
	// ���[�J���t���O
	A.init(S_element(ELM_GLOBAL_A), _T("A"), Gp_ini->flag_cnt, false);
	B.init(S_element(ELM_GLOBAL_B), _T("B"), Gp_ini->flag_cnt, false);
	C.init(S_element(ELM_GLOBAL_C), _T("C"), Gp_ini->flag_cnt, false);
	D.init(S_element(ELM_GLOBAL_D), _T("D"), Gp_ini->flag_cnt, false);
	E.init(S_element(ELM_GLOBAL_E), _T("E"), Gp_ini->flag_cnt, false);
	F.init(S_element(ELM_GLOBAL_F), _T("F"), Gp_ini->flag_cnt, false);
	X.init(S_element(ELM_GLOBAL_X), _T("X"), Gp_ini->flag_cnt, false);
	S.init(S_element(ELM_GLOBAL_S), _T("S"), Gp_ini->flag_cnt, false);

	// ���[�J���σt���O
	H.init(S_element(), _T("H"), 0, true);		// �ϒ�
	I.init(S_element(), _T("I"), 0, true);		// �ϒ�
	J.init(S_element(), _T("J"), 0, true);		// �ϒ�

	// ���[�J�����O�t���O
	local_namae.init(S_element(ELM_GLOBAL_NAMAE_LOCAL), _T("local namae"), 26 + 26 * 26, false);
}

// ****************************************************************
// �t���O�F�Z�[�u�i�O���[�o���j
// ================================================================
void C_elm_flag::save_global(C_tnm_save_stream& stream)
{
	G.save(stream);
	Z.save(stream);
	M.save(stream);

	// �O���[�o�����O�t���O
	global_namae.save(stream);

	// �_�~�[�`�F�b�N�h�c
	stream.save(dummy_check_id);
}

// ****************************************************************
// �t���O�F���[�h�i�O���[�o���j
// ================================================================
void C_elm_flag::load_global(C_tnm_save_stream& stream)
{
	G.load(stream);
	Z.load(stream);
	M.load(stream);

	// �O���[�o�����O�t���O
	global_namae.load(stream);

	// �_�~�[�`�F�b�N�h�c
	stream.load(dummy_check_id);
}

// ****************************************************************
// �t���O�F�Z�[�u�i���[�J���j
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

	// ���[�J�����O�t���O
	local_namae.save(stream);
}

// ****************************************************************
// �t���O�F���[�h�i���[�J���j
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

	// ���[�J�����O�t���O
	local_namae.load(stream);
}

// ****************************************************************
// �t���O�F���O�t���O���擾
// ================================================================
int get_zenkaku_alpha_no(TCHAR alpha)
{
	switch (alpha)	{
		case _T('�`'):	return 0;
		case _T('�a'):	return 1;
		case _T('�b'):	return 2;
		case _T('�c'):	return 3;
		case _T('�d'):	return 4;
		case _T('�e'):	return 5;
		case _T('�f'):	return 6;
		case _T('�g'):	return 7;
		case _T('�h'):	return 8;
		case _T('�i'):	return 9;
		case _T('�j'):	return 10;
		case _T('�k'):	return 11;
		case _T('�l'):	return 12;
		case _T('�m'):	return 13;
		case _T('�n'):	return 14;
		case _T('�o'):	return 15;
		case _T('�p'):	return 16;
		case _T('�q'):	return 17;
		case _T('�r'):	return 18;
		case _T('�s'):	return 19;
		case _T('�t'):	return 20;
		case _T('�u'):	return 21;
		case _T('�v'):	return 22;
		case _T('�w'):	return 23;
		case _T('�x'):	return 24;
		case _T('�y'):	return 25;
	}

	return -1;
}

bool C_elm_flag::get_flag_by_name(CTSTR& name, C_elm_str_list** pp_flag, int* p_flag_no)
{
	if (name.length() < 2)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("���O�t���O�����Z�����܂��B"));
		return false;
	}
	if (name.length() > 3)	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("���O�t���O�����������܂��B"));
		return false;
	}

	// �t���O���擾
	if (name[0] == _T('��'))	{
		*pp_flag = &global_namae;
	}
	else if (name[0] == _T('��'))	{
		*pp_flag = &local_namae;
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("���O�t���O�̂P�����ڂ����s���ł��B"));
		return false;
	}

	// �t���O�ԍ������߂�
	if (name.length() == 2)	{			// ���`

		int alpha_no = get_zenkaku_alpha_no(name[1]);
		if (alpha_no == -1)	{
			tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("���O�t���O�̂Q�����ڂ��s���ł��B"));
			return false;
		}

		*p_flag_no = alpha_no;
	}
	else if (name.length() == 3)	{	// ���`�`

		int alpha_no_0 = get_zenkaku_alpha_no(name[1]);
		if (alpha_no_0 == -1)	{
			tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("���O�t���O�̂Q�����ڂ����s���ł��B"));
			return false;
		}

		int alpha_no_1 = get_zenkaku_alpha_no(name[2]);
		if (alpha_no_1 == -1)	{
			tnm_set_error(TNM_ERROR_TYPE_OUT_OF_RANGE, _T("���O�t���O�̂R�����ڂ����s���ł��B"));
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
// ���[�U�C���N�v���p�e�B�F������
// ================================================================
void C_elm_user_inc_prop::init()
{
	// �t���O���쐬
	int prop_cnt = Gp_lexer->get_user_inc_prop_cnt();
	prop_list.init(S_element(), FM_VOID, _T("inc_prop"), prop_cnt, false, NULL);

	// �t���O�̌^��ݒ肷��
	for (int f = 0; f < prop_cnt; f++)	{
		prop_list[f].form = Gp_lexer->get_user_inc_prop_form(f);

		// �z��̏ꍇ�͏�����
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

	// �t���O���ď�����
	prop_list.reinit();
}

// ****************************************************************
// ���[�U�C���N�v���p�e�B�F�ď�����
// ================================================================
void C_elm_user_inc_prop::reinit()
{
	prop_list.reinit();
}

// ****************************************************************
// ���[�U�C���N�v���p�e�B�F�Z�[�u
// ================================================================
void C_elm_user_inc_prop::save(C_tnm_save_stream& stream)
{
	prop_list.save(stream);
}

// ****************************************************************
// ���[�U�C���N�v���p�e�B�F���[�h
// ================================================================
void C_elm_user_inc_prop::load(C_tnm_save_stream& stream)
{
	prop_list.load(stream);
}

// ****************************************************************
// ���[�U�V�[���v���p�e�B�F������
// ================================================================
void C_elm_user_scn_prop::init(int scn_no)
{
	// ���[�U�V�[���t���O���쐬
	int prop_cnt = Gp_lexer->get_user_scn_prop_cnt(scn_no);
	prop_list.init(S_element(), FM_VOID, _T("scn_prop"), prop_cnt, false, NULL);

	// �t���O�̌^��ݒ肷��
	for (int f = 0; f < prop_cnt; f++)	{
		prop_list[f].form = Gp_lexer->get_user_scn_prop_form(scn_no, f);

		// �z��̏ꍇ�͏�����
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

	// ���[�U�V�[���t���O���ď�����
	prop_list.reinit();
}

// ****************************************************************
// ���[�U�V�[���v���p�e�B�F�ď�����
// ================================================================
void C_elm_user_scn_prop::reinit()
{
	prop_list.reinit();
}

// ****************************************************************
// ���[�U�V�[���v���p�e�B�F�Z�[�u
// ================================================================
void C_elm_user_scn_prop::save(C_tnm_save_stream& stream)
{
	prop_list.save(stream);
}

// ****************************************************************
// ���[�U�V�[���v���p�e�B�F���[�h
// ================================================================
void C_elm_user_scn_prop::load(C_tnm_save_stream& stream)
{
	prop_list.load(stream);
}

// ****************************************************************
// ���[�U�V�[���v���p�e�B���X�g�F�T�u�̏�����
// ================================================================
void C_elm_user_scn_prop_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(i);
}

// ****************************************************************
// ���[�U�V�[���v���p�e�B���X�g�F�T�u�̍ď�����
// ================================================================
void C_elm_user_scn_prop_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// ���[�U�V�[���v���p�e�B���X�g�F�T�u�̃Z�[�u
// ================================================================
void C_elm_user_scn_prop_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// ���[�U�V�[���v���p�e�B���X�g�F�T�u�̃��[�h
// ================================================================
void C_elm_user_scn_prop_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}

// ****************************************************************
// ���[�U�R�[���v���p�e�B
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
// ���[�U�R�[���v���p�e�B���X�g�F�T�u�̏�����
// ================================================================
void C_tnm_user_call_prop_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init();
}

// ****************************************************************
// ���[�U�R�[���v���p�e�B���X�g�F�T�u�̍ď�����
// ================================================================
void C_tnm_user_call_prop_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// ���[�U�R�[���v���p�e�B���X�g�F�T�u�̃Z�[�u
// ================================================================
void C_tnm_user_call_prop_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// ���[�U�R�[���v���p�e�B���X�g�F�T�u�̃��[�h
// ================================================================
void C_tnm_user_call_prop_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}

