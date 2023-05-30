#pragma		once

#include	"data/tnm_local_data.h"
#include	"element/elm_counter.h"
#include	"element/elm_stage.h"

// ****************************************************************
// �G�������g�F�d�w�R�[��
// ================================================================
class C_elm_excall : public C_tnm_element
{
public:
	void	init(S_element element, CTSTR& name);			// ������
	void	reinit(bool finish_flag, bool restruct_flag);	// �ď�����
	void	finish();										// �I��
	// ���̃G�������g�̃Z�[�u���[�h�͂���܂���B

	void	ready();	// ��������
	void	free();		// �������
	bool	is_ready()	{	return m_ready_flag;	}

	// �q�G�������g
	C_elm_int_list			m_F;					// �t���O
	C_elm_counter_list		m_counter_list;			// �J�E���^
	C_elm_frame_action		m_frame_action;			// �t���[���A�N�V����
	C_elm_frame_action_list	m_frame_action_ch_list;	// �t���[���A�N�V�����`�����l��
	C_elm_stage_list		m_stage_list;			// �X�e�[�W

	// �t�H���g
	TSTR					m_font_name;			// �t�H���g��

	// pod
	C_tnm_local_data_pod	m_pod;

private:
	bool					m_ready_flag;			// �����t���O
};
