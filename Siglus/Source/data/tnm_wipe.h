#pragma		once

#include	"data/tnm_sorter.h"

// ****************************************************************
// ���C�v
// ================================================================

// �萔
const int TNM_WIPE_PRG_MAX = 65536;
const int TNM_WIPE_OPTION_MAX = 8;

const int TNM_WIPE_RANGE_NORMAL = 0;		// �ʏ�X�e�[�W�̂�
const int TNM_WIPE_RANGE_EX_CALL = 1;		// �d�w�X�e�[�W�̂�
const int TNM_WIPE_RANGE_SYSTEM_OUT = 2;	// �S�āi�t�F�[�h�C���j
const int TNM_WIPE_RANGE_SYSTEM_IN = 3;		// �S�āi�t�F�[�h�A�E�g�j

// ���C�v
class C_tnm_wipe
{
public:
	void			init();
	void			start(S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter, int wipe_range, int with_low_order, int wipe_type, int wipe_time, int start_time, int m_speed_mode, int* option, bool wait_flag, int key_skip_mode, bool return_value_flag, bool wipe_flag);
	void			start_mask(CTSTR& mask_file, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter, int wipe_range, int with_low_order, int wipe_type, int wipe_time, int start_time, int m_speed_mode, int* option, bool wait_flag, int key_skip_mode, bool return_value_flag, bool wipe_flag);
	void			end();
	void			wait(int key_skip_mode, bool return_value_flag);
	bool			check();
	void			update_time(int past_time);
	void			frame();
	bool			is_wipe_doing()	{	return m_type >= 0;	}

	int				m_type;							// ���C�v�^�C�v
	int				m_speed_mode;					// �X�s�[�h���[�h
	int				m_option[TNM_WIPE_OPTION_MAX];	// �I�v�V����
	int				m_cur_time;						// ���C�v���ݎ���
	int				m_start_time;					// ���C�v�J�n����
	int				m_end_time;						// ���C�v�I������
	S_tnm_sorter	m_begin_sorter;					// �J�n�\�[�^�[
	S_tnm_sorter	m_end_sorter;					// �I���\�[�^�[
	int				m_wipe_range;					// ���C�v�͈�
	int				m_with_low_order;				// �Ⴂ�I�[�_�[�܂Ŋ܂߂�
	TSTR			m_mask_file;					// �}�X�N�t�@�C��
	int				m_wipe_step;					// �X�e�b�v
	int				m_wipe_prg;						// ���C�v�i�s�x

private:
	void			start_sub(bool wait_flag, int key_skip_mode, bool return_value_flag, bool wipe_flag);
};
