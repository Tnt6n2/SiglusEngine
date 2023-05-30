#include	"pch.h"

#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"element/elm_btn_sel_item.h"
#include	"element/elm_mwnd_moji.h"
#include	"element/elm_excall.h"

// ****************************************************************
// �d�w�R�[���F������
// ================================================================
void C_elm_excall::init(S_element element, CTSTR& name)
{
	// ���N���X�̏�����
	C_tnm_element::init(element, FM_EXCALL, name, NULL);

	// �t���O�̏�����
	m_F.init(element + S_element(ELM_EXCALL_F), name + _T(".F"), 0, true);

	// �J�E���^�̏�����
	m_counter_list.init(element + S_element(ELM_EXCALL_COUNTER), FM_COUNTERLIST, name + _T(".counter"), 0, true, NULL);

	// �t���[���A�N�V���������������܂��B
	m_frame_action.init(element + S_element(ELM_EXCALL_FRAME_ACTION), _T(".frame_action"), S_element());

	// �t���[���A�N�V�����`�����l�������������܂��B
	m_frame_action_ch_list.init(element + S_element(ELM_EXCALL_FRAME_ACTION_CH), _T(".frame_action_ch"), 0, true, S_element());

	// �X�e�[�W�̏�����
	m_stage_list.init(element + S_element(ELM_EXCALL_STAGE), name + _T(".stage"), name + _T("."), 0, true, false, NULL);

	// �ď�����
	reinit(false, true);
}

// ****************************************************************
// �d�w�R�[���F�ď�����
// ================================================================
void C_elm_excall::reinit(bool finish_flag, bool restruct_flag)
{
	m_ready_flag = false;

	// �q�G�������g
	m_F.reinit();
	m_counter_list.reinit();
	m_frame_action.reinit(finish_flag);
	m_frame_action_ch_list.reinit(finish_flag, restruct_flag);
	m_stage_list.reinit(restruct_flag);

	// �t�H���g
	m_font_name.clear();

	// pod
	m_pod.init();
}

// ****************************************************************
// �d�w�R�[���F�I��
// ================================================================
void C_elm_excall::finish()
{
	// �t���[���A�N�V����
	m_frame_action.finish();

	// �t���[���A�N�V�����`�����l��
	m_frame_action_ch_list.finish();

	// �X�e�[�W
	m_stage_list.finish();
}

// ****************************************************************
// �d�w�R�[���F��������
// ================================================================
void C_elm_excall::ready()
{
	// �G�������g����������
	m_F.resize(Gp_ini->flag_cnt);
	m_counter_list.resize(Gp_ini->counter_cnt);
	m_frame_action_ch_list.resize(Gp_ini->frame_action_ch_cnt);
	m_stage_list.resize(TNM_STAGE_CNT);
	m_ready_flag = true;
}

// ****************************************************************
// �d�w�R�[���F�������
// ================================================================
void C_elm_excall::free()
{
	// �I��
	finish();

	// �G�������g���������
	m_F.clear();
	m_counter_list.clear();
	m_frame_action_ch_list.clear();
	m_stage_list.clear();
	m_ready_flag = false;

	// �t�H���g
	m_font_name.clear();
}















