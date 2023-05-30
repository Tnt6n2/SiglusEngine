#include	"pch.h"

#include	"tnm_form_code.h"

#include	"data/tnm_save_00.h"

#include	"element/elm_prop.h"

// ****************************************************************
// �v���p�e�B�F������
// ================================================================
void C_tnm_prop::init()
{
	// �ď�����
	reinit();
}

// ****************************************************************
// �v���p�e�B�F�ď�����
// ================================================================
void C_tnm_prop::reinit()
{
	id = 0;
	Int = 0;
	str.clear();
	element.clear();
	exp_list.reinit();
	exp_cnt = 0;

	if (false);
	else if (form == FM_INTLIST)	{
		C_elm_int_list* p_int_list = boost::any_cast< BSP<C_elm_int_list> >(any).get();		
		p_int_list->reinit();
	}
	else if (form == FM_STRLIST)	{
		C_elm_str_list* p_str_list = boost::any_cast< BSP<C_elm_str_list> >(any).get();		
		p_str_list->reinit();
	}
}

// ****************************************************************
// �v���p�e�B�F�Z�[�u
// ================================================================
void C_tnm_prop::save(C_tnm_save_stream& stream)
{
	stream.save(id);
	stream.save(form);
	stream.save(Int);
	stream.save(str);
	stream.save(element);
	exp_list.save(stream);
	stream.save(exp_cnt);

	if (false);
	else if (form == FM_INTLIST)	{
		C_elm_int_list* p_int_list = boost::any_cast< BSP<C_elm_int_list> >(any).get();		
		p_int_list->save(stream);
	}
	else if (form == FM_STRLIST)	{
		C_elm_str_list* p_str_list = boost::any_cast< BSP<C_elm_str_list> >(any).get();		
		p_str_list->save(stream);
	}
}

// ****************************************************************
// �v���p�e�B�F���[�h
// ================================================================
void C_tnm_prop::load(C_tnm_save_stream& stream)
{
	stream.load(id);
	stream.load(form);
	stream.load(Int);
	stream.load(str);
	stream.load(element);
	exp_list.load(stream);
	stream.load(exp_cnt);

	if (false);
	else if (form == FM_INTLIST)	{
		C_elm_int_list* p_int_list = boost::any_cast< BSP<C_elm_int_list> >(any).get();		
		p_int_list->load(stream);
	}
	else if (form == FM_STRLIST)	{
		C_elm_str_list* p_str_list = boost::any_cast< BSP<C_elm_str_list> >(any).get();		
		p_str_list->load(stream);
	}
}

// ****************************************************************
// �v���p�e�B���X�g�F�T�u�̏�����
// ================================================================
void C_tnm_prop_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init();
}

// ****************************************************************
// �v���p�e�B���X�g�F�T�u�̍ď�����
// ================================================================
void C_tnm_prop_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// �v���p�e�B���X�g�F�T�u�̃Z�[�u
// ================================================================
void C_tnm_prop_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// �v���p�e�B���X�g�F�T�u�̃��[�h
// ================================================================
void C_tnm_prop_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}
