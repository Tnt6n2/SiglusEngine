#include	"pch.h"

#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_mask.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// �}�X�N�F������
// ================================================================
void C_elm_mask::init(S_element element, CTSTR& name)
{
	// ���N���X�̏�����
	C_tnm_element::init(element, FM_MASK, name, NULL);

	// �ď�����
	reinit();
}

// ****************************************************************
// �}�X�N�F�ď�����
// ================================================================
void C_elm_mask::reinit()
{
	// �p�����[�^
	m_param.x.init(0);
	m_param.y.init(0);
	m_mask_name.clear();

	// ��ƃp�����[�^
	m_album.reset();
}

// ****************************************************************
// �}�X�N�F�Z�[�u
// ================================================================
void C_elm_mask::save(C_tnm_save_stream& stream)
{
	stream.save(m_param);
	stream.save(m_mask_name);
}

// ****************************************************************
// �}�X�N�F���[�h
// ================================================================
void C_elm_mask::load(C_tnm_save_stream& stream)
{
	stream.load(m_param);
	stream.load(m_mask_name);

	// �č\�z
	restruct();
}

// ****************************************************************
// �}�X�N�F�}�X�N���쐬
// ================================================================
void C_elm_mask::create(CTSTR& file_name)
{
	// ���Ȃ����ɔ����ăA���o����ێ����Ă����i�Q�ƃJ�E���^���P������j
	BSP<C_d3d_album> temp_album = m_album;

	// �ď�����
	reinit();

	// �p�����[�^��ݒ�
	m_mask_name = file_name;

	// �č\�z
	restruct();
}

// ****************************************************************
// �}�X�N�F�č\�z
// ================================================================
void C_elm_mask::restruct()
{
	// �A���o�����擾
	if (!m_mask_name.empty())	{
		m_album = tnm_load_pct_d3d(m_mask_name, false, false);
		if (!m_album)	{
			m_mask_name.clear();	// ���s�����ꍇ�͏�����
		}
	}
}

// ****************************************************************
// �}�X�N�F���Ԃ��X�V
// ================================================================
void C_elm_mask::update_time(int past_game_time, int past_real_time)
{
	m_param.x.update_time(past_game_time, past_real_time);
	m_param.y.update_time(past_game_time, past_real_time);
}

// ****************************************************************
// �}�X�N�F�t���[������
// ================================================================
void C_elm_mask::frame()
{
	m_param.x.frame();
	m_param.y.frame();
}

// ****************************************************************
// �}�X�N���X�g�F�T�u�̏�����
// ================================================================
void C_elm_mask_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"));
}

// ****************************************************************
// �}�X�N���X�g�F�T�u�̍ď�����
// ================================================================
void C_elm_mask_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// �}�X�N���X�g�F�T�u�̃Z�[�u
// ================================================================
void C_elm_mask_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// �}�X�N���X�g�F�T�u�̃��[�h
// ================================================================
void C_elm_mask_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}
