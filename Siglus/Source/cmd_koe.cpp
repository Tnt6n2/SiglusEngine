#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_def.h"
#include	"data/tnm_config_data.h"
#include	"element/elm_prop.h"
#include	"element/elm_sound.h"
#include	"engine/flow_command.h"
#include	"engine/eng_config.h"

// ****************************************************************
// �d�w�����Đ�����
// ================================================================
void tnm_play_ex_koe(C_elm_koe* p_koe, int koe_no, int chara_no, bool wait_flag, bool key_skip_flag, bool return_value_flag)
{
	// �����Đ�����i�Đ����Ȃ��ꍇ�̓t�@�C���`�F�b�N�̂݁j
	p_koe->play(koe_no, chara_no, TNM_JITAN_RATE_NORMAL, 0, wait_flag, key_skip_flag, return_value_flag, true, false);
}

// ****************************************************************
// �R�}���h�����F�d�w��
// ================================================================
bool tnm_command_proc_ex_koe(C_elm_koe* p_koe, int* elm_top, int* elm_begin, int* elm_end, S_tnm_command_proc_arg_struct* p_ai)
{
	int koe_no = 0, chara_no = -1;
	bool wait_flag = false, key_skip_flag = false, jitan_flag = false;

	// �R�}���h�ɂ���ăp�����[�^��ύX
	if (elm_begin[0] == ELM_GLOBAL_EXKOE_PLAY_WAIT)	{
		wait_flag = true;
	}
	else if (elm_begin[0] == ELM_GLOBAL_EXKOE_PLAY_WAIT_KEY)	{
		wait_flag = true;
		key_skip_flag = true;
	}

	// ���O�Ȃ�����
	switch (p_ai->al_id)	{	// no break!
		case 1:		chara_no = p_ai->al_begin[1].Int;
		case 0:		koe_no = p_ai->al_begin[0].Int;
	}

	// ���O������
	for (C_tnm_prop* arg = p_ai->al_end; arg < p_ai->named_al_end; arg++)	{
		switch (arg->id)	{
			case 0:		koe_no = arg->Int;							break;
			case 1:		chara_no = arg->Int;						break;
			case 2:		wait_flag = arg->Int ? true : false;		break;
			case 3:		key_skip_flag = arg->Int ? true : false;	break;
			case 4:		jitan_flag = arg->Int ? true : false;		break;
		}
	}

	// ���Z�Đ��̃��[�g�����߂�
	int jitan_rate = jitan_flag ? Gp_config->jitan_speed : TNM_JITAN_RATE_NORMAL;

	// �����Đ�����i�d�w���A�߂�l��ςށA�`�F�b�N�݂̂ł͂Ȃ��j
	p_koe->play(koe_no, chara_no, jitan_rate, 0, wait_flag, key_skip_flag, true, true, false);

	return true;
}


/*
			{
			int koe_no = 0, chr_no = -1;
			switch (al_id)	{	// no break!
				case 1:		chr_no = p_ai->al_begin[1].Int;
				case 0:		koe_no = p_ai->al_begin[0].Int;
			}

			tnm_mwnd_proc_ex_koe(p_mwnd, koe_no, chr_no, false, false, false);
			break;
		}
		case ELM_GLOBAL_EXKOE_PLAY_WAIT:
		{
			int koe_no = 0, chr_no = -1;
			switch (al_id)	{	// no break!
				case 1:		chr_no = p_ai->al_begin[1].Int;
				case 0:		koe_no = p_ai->al_begin[0].Int;
			}

			tnm_mwnd_proc_ex_koe(p_mwnd, koe_no, chr_no, true, false, false);
			break;
		}
		case ELM_GLOBAL_EXKOE_PLAY_WAIT_KEY:
		{
			int koe_no = 0, chr_no = -1;
			switch (al_id)	{	// no break!
				case 1:		chr_no = p_ai->al_begin[1].Int;
				case 0:		koe_no = p_ai->al_begin[0].Int;
			}

			tnm_mwnd_proc_ex_koe(p_mwnd, koe_no, chr_no, true, true, true);
			break;
		}
*/
