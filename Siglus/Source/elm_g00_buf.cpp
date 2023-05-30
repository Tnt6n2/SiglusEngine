#include	"pch.h"
#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_g00_buf.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// �f�O�O�o�b�t�@�F������
// ================================================================
void C_elm_g00_buf::init(S_element element, CTSTR& name)
{
	// ���N���X�̏�����
	C_tnm_element::init(element, FM_G00BUF, name, NULL);

	// �ď�����
	reinit();
}

// ****************************************************************
// �f�O�O�o�b�t�@�F�ď�����
// ================================================================
void C_elm_g00_buf::reinit()
{
	m_file_name.clear();
	m_album.reset();
}

// ****************************************************************
// �f�O�O�o�b�t�@�F�Z�[�u
// ================================================================
void C_elm_g00_buf::save(C_tnm_save_stream& stream)
{
	stream.save(m_file_name);
}

// ****************************************************************
// �f�O�O�o�b�t�@�F���[�h
// ================================================================
void C_elm_g00_buf::load(C_tnm_save_stream& stream)
{
	TSTR file_name;
	stream.load(file_name);

	if (!file_name.empty())	{
		load_file(file_name);
	}
}

// ****************************************************************
// �f�O�O�o�b�t�@�F�t�@�C����ǂݍ���
// ================================================================
bool C_elm_g00_buf::load_file(CTSTR& file_name)
{
	// �e�N�X�`����ǂݍ���
	m_album = tnm_load_pct_d3d(file_name, false, false);	// �b�f�e�[�u���ɂ͓o�^���Ȃ��I
	if (!m_album)
		return false;

	m_file_name = file_name;
	return true;
}

// ****************************************************************
// �f�O�O�o�b�t�@�F���
// ================================================================
void C_elm_g00_buf::free()
{
	m_file_name.clear();
	m_album.reset();
}

// ****************************************************************
// �f�O�O�o�b�t�@���X�g�F�T�u�̏�����
// ================================================================
void C_elm_g00_buf_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"));
}

// ****************************************************************
// �f�O�O�o�b�t�@���X�g�F�T�u�̍ď�����
// ================================================================
void C_elm_g00_buf_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// �f�O�O�o�b�t�@���X�g�F�T�u�̃Z�[�u
// ================================================================
void C_elm_g00_buf_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// �f�O�O�o�b�t�@���X�g�F�T�u�̃��[�h
// ================================================================
void C_elm_g00_buf_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}
