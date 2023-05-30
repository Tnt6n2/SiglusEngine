#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_proc.h"
#include	"data/tnm_wipe.h"
#include	"element/elm_excall.h"
#include	"engine/eng_config.h"
#include	"engine/ifc_stack.h"
#include	"engine/ifc_proc_stack.h"

// ****************************************************************
// ���C�v�F������
// ================================================================
void C_tnm_wipe::init()
{
	m_type = -1;
	m_speed_mode = 0;
	for (int i = 0; i < TNM_WIPE_OPTION_MAX; i++)
		m_option[i] = 0;

	m_start_time = 0;
	m_end_time = 0;
	m_begin_sorter = S_tnm_sorter(0, 0);
	m_end_sorter = S_tnm_sorter(0, 0);
	m_wipe_range = TNM_WIPE_RANGE_NORMAL;
	m_mask_file.clear();
	m_wipe_step = -1;
	m_wipe_prg = 0;
}

// ****************************************************************
// ���C�v�F�V�X�e���R���t�B�O�ɂ���ă��C�v�^�C�v��u��������
// ================================================================
void tnm_change_wipe_type(int* p_wipe_type, int* option)
{
	// �V�X�e���R���t�B�O�ɂ���āA���C�v�̎�ނ𐧌�����
	if (Gp_global->wipe_buffer_cnt < 2)	{

		// �N���X���X�^�i220�j
		if (*p_wipe_type == 220)	{
			*p_wipe_type = 221;		// ���X�^�i221�j�ɒu��������
			option[4] = 0;			// ���G���t�F�[�h�C��������
		}
		// �N���X�����u���[�i240�j�^�����u���[�i241�j
		else if (*p_wipe_type == 240 || *p_wipe_type == 241)	{
			int center_x = option[0];
			int center_y = option[1];

			*p_wipe_type = 214;		// ���G���t�F�[�h�n�t�s���Ȃ���g��\���i214�j�ɒu��������
			option[0] = 11;			// �w����W����
			option[1] = 0;			// ���g�p
			option[2] = center_x;	// �w���W
			option[3] = center_y;	// �x���W
		}
		// �N���X�N�G�C�N�u���[�i242�j�^�N�G�C�N�u���[�i243�j
		else if (*p_wipe_type == 242 || *p_wipe_type == 243)	{

			*p_wipe_type = 214;		// ���G���t�F�[�h�n�t�s���Ȃ���g��\���i214�j�ɒu��������
			option[0] = 0;			// ���S����
			option[1] = 0;			// ���g�p
		}
	}
}

// ****************************************************************
// ���C�v�J�n
// ================================================================
//		key_skip: 0=�L�[�Ŕ�΂��Ȃ��A1=�L�[�Ŕ�΂���A-1=�V�X�e���ݒ�ɏ]��
// ================================================================
void C_tnm_wipe::start(S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter
					   , int wipe_range, int with_low_order, int wipe_type, int wipe_time, int start_time, int speed_mode, int* option
					   , bool wait_flag, int key_skip_mode, bool return_value_flag, bool wipe_flag)
{
	// ���݂̃��C�v���I��������
	end();

	// �V�X�e���R���t�B�O�ɂ���āA���C�v�̎�ނ𐧌�����
	tnm_change_wipe_type(&wipe_type, option);

	// ���C�v�p�����[�^��ݒ�
	m_type = wipe_type;
	m_speed_mode = speed_mode;
	for (int i = 0; i < TNM_WIPE_OPTION_MAX; i++)
		m_option[i] = option[i];
	m_cur_time = 0;
	m_start_time = start_time;
	m_end_time = wipe_time;
	m_begin_sorter = begin_sorter;
	m_end_sorter = end_sorter;
	m_wipe_range = wipe_range;
	m_with_low_order = with_low_order;

	// ���ʏ���
	start_sub(wait_flag, key_skip_mode, return_value_flag, wipe_flag);
}

// ****************************************************************
// �}�X�N���C�v�J�n
// ================================================================
//		key_skip: 0=�L�[�Ŕ�΂��Ȃ��A1=�L�[�Ŕ�΂���A-1=�V�X�e���ݒ�ɏ]��
// ================================================================
void C_tnm_wipe::start_mask(CTSTR& mask_file, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter
							, int wipe_range, int with_low_order, int wipe_type, int wipe_time, int start_time, int speed_mode, int* option
							, bool wait_flag, int key_skip_mode, bool return_value_flag, bool wipe_flag)
{
	// ���݂̃��C�v���I��������
	end();

	// �V�X�e���R���t�B�O�ɂ���āA���C�v�̎�ނ𐧌�����
	tnm_change_wipe_type(&wipe_type, option);

	// ���C�v�J�n
	m_type = wipe_type;
	m_speed_mode = speed_mode;
	for (int i = 0; i < TNM_WIPE_OPTION_MAX; i++)
		m_option[i] = option[i];
	m_cur_time = 0;
	m_start_time = start_time;
	m_end_time = wipe_time;
	m_begin_sorter = begin_sorter;
	m_end_sorter = end_sorter;
	m_wipe_range = wipe_range;
	m_with_low_order = with_low_order;
	m_mask_file = mask_file;

	// ���ʏ���
	start_sub(wait_flag, key_skip_mode, return_value_flag, wipe_flag);
}

// ****************************************************************
// ���C�v�J�n�i���ʏ����j
// ================================================================
void C_tnm_wipe::start_sub(bool wait_flag, int key_skip_mode, bool return_value_flag, bool wipe_flag)
{
	// ���C�v���s��
	if (wipe_flag)	{

		// �d�w�R�[�����C�v�̏ꍇ
		if (m_wipe_range == TNM_WIPE_RANGE_EX_CALL)	{

			if (Gp_excall->is_ready())	{

				Gp_excall->m_stage_list.wipe(m_begin_sorter, m_end_sorter);
			}
		}
		// �ʏ탏�C�v�̏ꍇ
		else if (m_wipe_range == TNM_WIPE_RANGE_NORMAL)	{

			Gp_stage_list->wipe(m_begin_sorter, m_end_sorter);
		}
	}

	// ���C�v�̓X�e�b�v�O����
	m_wipe_step = 0;

	// ���C�v�҂��̏ꍇ�̓v���Z�X��ύX
	if (wait_flag)	{
		wait(key_skip_mode, return_value_flag);
	}
	// ���C�v�҂��łȂ��ꍇ�͂����ɖ߂�l��Ԃ�
	else if (return_value_flag)	{
		tnm_stack_push_int(0);
	}

	// �V�X�e���ݒ�́u���C�v���Ȃ��v�`�F�b�N
	if (Gp_config->system.no_wipe_anime_flag)
		end();		// �����Ƀ��C�v���I��点��
}

// ****************************************************************
// ���C�v�I��
// ================================================================
void C_tnm_wipe::end()
{
	// ���C�v���̏ꍇ
	if (m_type >= 0)	{

		// �d�w�R�[�����C�v�̏ꍇ
		if (m_wipe_range == TNM_WIPE_RANGE_EX_CALL)	{

			if (Gp_excall->is_ready())	{

				// �l�N�X�g�X�e�[�W���ď�����
				Gp_excall->m_stage_list[TNM_STAGE_NEXT].reinit(false);
			}
		}
		// �ʏ탏�C�v�̏ꍇ
		else if (m_wipe_range == TNM_WIPE_RANGE_NORMAL)	{

			// �l�N�X�g�X�e�[�W���ď�����
			(*Gp_stage_list)[TNM_STAGE_NEXT].reinit(false);
		}
	}

	// ���C�v��������
	init();
}

// ****************************************************************
// ���C�v�҂�
// ================================================================
void C_tnm_wipe::wait(int key_skip_mode, bool return_value_flag)
{
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_WIPE_WAIT;
	proc.key_skip_enable_flag = 
		key_skip_mode == 0 ? false :
		key_skip_mode == 1 ? true :
		Gp_config->system.skip_wipe_anime_flag;
	proc.return_value_flag = return_value_flag;
	tnm_push_proc(proc);
}

// ****************************************************************
// ���C�v�𔻒�
// ================================================================
bool C_tnm_wipe::check()
{
	return m_type >= 0;
}

// ****************************************************************
// ���C�v�F���Ԃ��X�V
// ================================================================
void C_tnm_wipe::update_time(int past_time)
{
	// ������i�߂�
	m_cur_time += past_time;
}

// ****************************************************************
// ���C�v�F�t���[������
// ================================================================
void C_tnm_wipe::frame()
{
	if (m_wipe_step == 0)	{

		// ���̃X�e�b�v��
		m_wipe_step = 1;

		// ���ԍX�V�̂��߂ɏ����𔲂���
		return;
	}

	if (m_wipe_step == 1)	{

		// �����������ōĐݒ�
		// ���C�v�J�n���ɒx���ŃJ�N���Ȃ��悤�ɂ��邽�߂̏���
		m_cur_time = m_start_time;

		// ���̃X�e�b�v��
		m_wipe_step = 2;
	}

	if (m_wipe_step == 2)	{

		// ���C�v�̐i�s�x���v�Z�i0�`wipe_prg_max�j
		double et = (double)m_end_time;
		double ct = (double)m_cur_time;

		m_wipe_prg = 
			m_end_time <= 0 ? TNM_WIPE_PRG_MAX : 
			m_speed_mode == 0 ? (int)((ct) * TNM_WIPE_PRG_MAX / et) : 
			m_speed_mode == 1 ? (int)((ct) * (ct) * TNM_WIPE_PRG_MAX / et / et) : 
			m_speed_mode == 2 ? (int)(- (ct - et) * (ct - et) * TNM_WIPE_PRG_MAX / et / et) + TNM_WIPE_PRG_MAX : 0;

		// ���Ԃ������烏�C�v�I��
		if (m_cur_time - m_end_time >= 0)
			end();
	}
}

