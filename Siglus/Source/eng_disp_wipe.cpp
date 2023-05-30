#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_wnd.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_tool.h"
#include	"element/elm_stage.h"
#include	"element/elm_screen.h"
#include	"engine/eng_graphics.h"
#include	"engine/eng_mask_wipe.h"
#include	"../resource.h"

// ****************************************************************
// ���C�v�p�����[�^�`�F�b�N���R�s�[
// ================================================================
bool C_tnm_wnd::check_and_copy_for_wipe_param()
{
	if (m_ready_wipe_type != Gp_wipe->m_type
		|| m_ready_wipe_file != Gp_wipe->m_mask_file
		|| memcmp(m_ready_wipe_option, Gp_wipe->m_option, sizeof(int) * TNM_WIPE_OPTION_MAX)
		)
	{
		m_ready_wipe_type = Gp_wipe->m_type;
		m_ready_wipe_file = Gp_wipe->m_mask_file;
		memcpy(m_ready_wipe_option, Gp_wipe->m_option, sizeof(int) * TNM_WIPE_OPTION_MAX);
		return true;
	}

	return false;
}

// ****************************************************************
// �}�X�N�쐬
// ================================================================
void C_tnm_wnd::make_mask()
{
	m_mask_buffer_dib.reset();

	switch (Gp_wipe->m_type)	{

		case 5:		// �����C�v�e�X�g
			tnm_make_mask_texture_for_direction_slice(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;

		case 10:	// �S�~�S�̂��킾���X�p�^�[��
			tnm_make_mask_texture_for_4x4_jiwa9(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 11:	// �S�~�S�̂��킾���V�p�^�[��
			tnm_make_mask_texture_for_4x4_jiwa7(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;

		case 20:	// �S�~�S�̉��悤��
			tnm_make_mask_texture_for_4x4_turn_around(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 21:	// �S�~�S�̉��s���悤��
			tnm_make_mask_texture_for_4x4_turn_ret(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 22:	// �S�~�S�̐܂�Ԃ��悤��
			tnm_make_mask_texture_for_4x4_turn_down(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;

		case 30:	// �W�~�W�̉��悤��
			tnm_make_mask_texture_for_8x8_turn_around(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 31:	// �W�~�W�̉��s���悤��
			tnm_make_mask_texture_for_8x8_turn_ret(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 32:	// �W�~�W�̐܂�Ԃ��悤��
			tnm_make_mask_texture_for_8x8_turn_down(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;

		case 40:	// �����_��
			tnm_make_mask_texture_for_random(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 41:	// �����_�����C��
			tnm_make_mask_texture_for_random_line(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 42:	// �����_���΂߃��C��
			tnm_make_mask_texture_for_random_slant_line(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 43:	// �����_���N���X
			tnm_make_mask_texture_for_random_cross(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 44:	// �����_���΂߃N���X
			tnm_make_mask_texture_for_random_slant_cross(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1]);
			break;

		case 60:	// �P��
			tnm_make_mask_texture_for_around_one(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1]);
			break;
		case 61:	// ����
			tnm_make_mask_texture_for_around_half(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1]);
			break;
		case 62:	// ������
			tnm_make_mask_texture_for_around_divide(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1]);
			break;
		case 63:	// ���S��
			tnm_make_mask_texture_for_oogi_center(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1] ? true : false);
			break;
		case 64:	// �p��
			tnm_make_mask_texture_for_oogi_corner(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1], false);
			break;
		case 65:	// ����
			tnm_make_mask_texture_for_oogi_edge(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1], 0, false);
			break;
		case 66:	// ����Q
			tnm_make_mask_texture_for_oogi_edge(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1], 1, false);
			break;
		case 67:	// �p��i���j
			tnm_make_mask_texture_for_oogi_corner(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1], true);
			break;
		case 68:	// ����i���j
			tnm_make_mask_texture_for_oogi_edge(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1], 0, true);
			break;
		case 69:	// ����Q�i���j
			tnm_make_mask_texture_for_oogi_edge(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1], 1, true);
			break;

		case 70:	// ����`�̂P��
			tnm_make_mask_texture_for_srect_around_one(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[5] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3], Gp_wipe->m_option[4]);
			break;
		case 71:	// ����`�̔���
			tnm_make_mask_texture_for_srect_around_half(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[5] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3], Gp_wipe->m_option[4]);
			break;
		case 72:	// ����`�̕�����
			tnm_make_mask_texture_for_srect_around_divide(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[5] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3], Gp_wipe->m_option[4]);
			break;
		case 73:	// ����`�̒��S��
			tnm_make_mask_texture_for_srect_oogi_center(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 74:	// ����`�̊p��
			tnm_make_mask_texture_for_srect_oogi_corner(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[5] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3], Gp_wipe->m_option[4], false);
			break;
		case 75:	// ����`�̉���
			tnm_make_mask_texture_for_srect_oogi_edge(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[5] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3], Gp_wipe->m_option[4], 0, false);
			break;
		case 76:	// ����`�̉���Q
			tnm_make_mask_texture_for_srect_oogi_edge(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[5] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3], Gp_wipe->m_option[4], 1, false);
			break;
		case 77:	// ����`�̊p��i���j
			tnm_make_mask_texture_for_srect_oogi_corner(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[5] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3], Gp_wipe->m_option[4], true);
			break;
		case 78:	// ����`�̉���i���j
			tnm_make_mask_texture_for_srect_oogi_edge(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[5] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3], Gp_wipe->m_option[4], 0, true);
			break;
		case 79:	// ����`�̉���Q�i���j
			tnm_make_mask_texture_for_srect_oogi_edge(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[5] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3], Gp_wipe->m_option[4], 1, true);
			break;

		case 80:	// �l�p�`
			tnm_make_mask_texture_for_square(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1] ? true : false);
			break;
		case 81:	// �H�`
			tnm_make_mask_texture_for_rhombus(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1] ? true : false);
			break;
		case 82:	// �\��
			tnm_make_mask_texture_for_jyuuji(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1] ? true : false);
			break;
		case 83:	// �e���r
			tnm_make_mask_texture_for_television(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1] ? true : false);
			break;

		case 90:	// ����`�̎l�p�`
			tnm_make_mask_texture_for_srect_square(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 91:	// ����`�̕H�`
			tnm_make_mask_texture_for_srect_rhombus(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 92:	// ����`�̏\��
			tnm_make_mask_texture_for_srect_jyuuji(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 93:	// ����`�̃e���r
			tnm_make_mask_texture_for_srect_television(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;

		case 100:	// �w���������
			tnm_make_mask_texture_for_direction(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1]);
			break;
		case 101:	// �w���������X���C�X
			tnm_make_mask_texture_for_direction_slice(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 102:	// �w���������u���C���h
			tnm_make_mask_texture_for_direction_blind(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;

		case 110:	// �w��̎΂ߕ�������
			tnm_make_mask_texture_for_direction_slant(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1]);
			break;
		case 111:	// �w���������u���b�N�u���C���h
			tnm_make_mask_texture_for_direction_block_blind(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;

		case 120:	// ����������
			tnm_make_mask_texture_for_bothdir(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1]);
			break;
		case 121:	// ����������X���C�X
			tnm_make_mask_texture_for_bothdir_slice(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[3] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 122:	// ����������u���C���h
			tnm_make_mask_texture_for_bothdir_blind(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[3] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;

		case 130:	// ����������X�g���C�v
			tnm_make_mask_texture_for_bothdir_stripe(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 131:	// ����������X�g���C�v�Q
			tnm_make_mask_texture_for_bothdir_stripe2(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 132:	// ����������u���b�N�u���C���h
			tnm_make_mask_texture_for_bothdir_block_blind(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;

		case 140:	// �S��������
			tnm_make_mask_texture_for_crossdir(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[1] ? true : false);
			break;
		case 141:	// �S��������X���C�X
			tnm_make_mask_texture_for_crossdir_slice(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1]);
			break;
		case 142:	// �S��������u���C���h
			tnm_make_mask_texture_for_crossdir_blind(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[2] ? true : false, Gp_wipe->m_option[1]);
			break;

		case 150:	// ����`�̂S��������
			tnm_make_mask_texture_for_srect_crossdir(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[3] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2]);
			break;
		case 151:	// ����`�̂S��������X���C�X
			tnm_make_mask_texture_for_srect_crossdir_slice(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 152:	// ����`�̂S��������u���C���h
			tnm_make_mask_texture_for_srect_crossdir_blind(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_option[4] ? true : false, Gp_wipe->m_option[1], Gp_wipe->m_option[2], Gp_wipe->m_option[3]);
			break;
		case 900:	// �}�X�N�t�@�C�����g��
		case 901:	// �}�X�N�t�@�C�����g��
			tnm_make_mask_texture_from_file(&m_mask_buffer_texture, &m_mask_buffer_dib, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, Gp_wipe->m_mask_file);
			break;
	}
}

// ****************************************************************
// ���C�v�`��
// ================================================================
bool C_tnm_wnd::disp_proc_wipe()
{
	int wipe_prg = Gp_wipe->m_wipe_prg;

	if (false);
	else if (Gp_wipe->m_type == 0)	{
		if (!disp_proc_wipe_for_cross_fade(wipe_prg))
			return false;
	}
	else if (Gp_wipe->m_type == 1)	{
		if (!disp_proc_wipe_for_not_wipe_after_wait(wipe_prg))
			return false;
	}
	else if (Gp_wipe->m_type == 2)	{
		if (!disp_proc_wipe_for_not_wipe_befor_wait(wipe_prg))
			return false;
	}
	else if (
		Gp_wipe->m_type == 5 ||		// �����C�v�e�X�g

		Gp_wipe->m_type == 10 ||
		Gp_wipe->m_type == 11 ||

		Gp_wipe->m_type == 20 ||
		Gp_wipe->m_type == 21 ||
		Gp_wipe->m_type == 22 ||

		Gp_wipe->m_type == 30 ||
		Gp_wipe->m_type == 31 ||
		Gp_wipe->m_type == 32 ||

		Gp_wipe->m_type == 40 ||
		Gp_wipe->m_type == 41 ||
		Gp_wipe->m_type == 42 ||
		Gp_wipe->m_type == 43 ||
		Gp_wipe->m_type == 44 ||

		Gp_wipe->m_type == 60 ||
		Gp_wipe->m_type == 61 ||
		Gp_wipe->m_type == 62 ||
		Gp_wipe->m_type == 63 ||
		Gp_wipe->m_type == 64 ||
		Gp_wipe->m_type == 65 ||
		Gp_wipe->m_type == 66 ||
		Gp_wipe->m_type == 67 ||
		Gp_wipe->m_type == 68 ||
		Gp_wipe->m_type == 69 ||

		Gp_wipe->m_type == 70 ||
		Gp_wipe->m_type == 71 ||
		Gp_wipe->m_type == 72 ||
		Gp_wipe->m_type == 73 ||
		Gp_wipe->m_type == 74 ||
		Gp_wipe->m_type == 75 ||
		Gp_wipe->m_type == 76 ||
		Gp_wipe->m_type == 77 ||
		Gp_wipe->m_type == 78 ||
		Gp_wipe->m_type == 79 ||

		Gp_wipe->m_type == 80 ||
		Gp_wipe->m_type == 81 ||
		Gp_wipe->m_type == 82 ||
		Gp_wipe->m_type == 83 ||

		Gp_wipe->m_type == 90 ||
		Gp_wipe->m_type == 91 ||
		Gp_wipe->m_type == 92 ||
		Gp_wipe->m_type == 93 ||

		Gp_wipe->m_type == 100 ||
		Gp_wipe->m_type == 101 ||
		Gp_wipe->m_type == 102 ||

		Gp_wipe->m_type == 110 ||
		Gp_wipe->m_type == 111 ||

		Gp_wipe->m_type == 120 ||
		Gp_wipe->m_type == 121 ||
		Gp_wipe->m_type == 122 ||

		Gp_wipe->m_type == 130 ||
		Gp_wipe->m_type == 131 ||
		Gp_wipe->m_type == 132 ||

		Gp_wipe->m_type == 140 ||
		Gp_wipe->m_type == 141 ||
		Gp_wipe->m_type == 142 ||

		Gp_wipe->m_type == 150 ||
		Gp_wipe->m_type == 151 ||
		Gp_wipe->m_type == 152 ||

		Gp_wipe->m_type == 900 ||
		Gp_wipe->m_type == 901 ||

		false
	)	{
		if (!disp_proc_wipe_for_mask(wipe_prg))
			return false;
	}
	else if (Gp_wipe->m_type == 50)	{
		if (!disp_proc_wipe_for_shimi(wipe_prg))
			return false;
	}
	else if (Gp_wipe->m_type == 200)	{
		if (!disp_proc_wipe_for_move(wipe_prg))
			return false;
	}
	else if (
		Gp_wipe->m_type == 210 ||
		Gp_wipe->m_type == 211 ||
		Gp_wipe->m_type == 212 ||
		Gp_wipe->m_type == 213 ||
		Gp_wipe->m_type == 214 ||
		Gp_wipe->m_type == 215 ||
		false
	)	{
		if (!disp_proc_wipe_for_scale(wipe_prg))
			return false;
	}
	else if (Gp_wipe->m_type == 220)	{
		if (!disp_proc_wipe_for_cross_raster(wipe_prg))
			return false;
	}
	else if (Gp_wipe->m_type == 221)	{
		if (!disp_proc_wipe_for_raster(wipe_prg))
			return false;
	}
	else if (
		Gp_wipe->m_type == 230 ||
		Gp_wipe->m_type == 231 ||
		false
	)	{
		if (!disp_proc_wipe_for_mosaic(wipe_prg))
			return false;
	}
	else if (
		Gp_wipe->m_type == 240 ||
		Gp_wipe->m_type == 241 ||
		Gp_wipe->m_type == 242 ||
		Gp_wipe->m_type == 243 ||
		false
	)	{
		if (!disp_proc_wipe_for_explosion_blur(wipe_prg))
			return false;
	}
	else if (Gp_wipe->m_type == 300 || Gp_wipe->m_type == 301)	{
		if (!disp_proc_wipe_for_move(wipe_prg))
			return false;
	}

	return true;
}

// ****************************************************************
// ���C�v�`��F���C�v�Ȃ��i�\�����Ă���҂j
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_not_wipe_after_wait(int wipe_prg)
{
	// --- ���� ---

	// ���C�v��艺�̃I�[�_�[��`�揀������
	if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃t�����g��`�揀������
	if (!disp_proc_wipe_order_ready(TNM_STAGE_FRONT))
		return false;

	// ���C�v����̃I�[�_�[��`�揀������
	if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
		return false;

	// --- �`�� ---

	// ���C�v��艺�̃I�[�_�[��`�悷��
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃t�����g��`�悷��
	if (!disp_proc_wipe_order(TNM_STAGE_FRONT))
		return false;

	// ���C�v����̃I�[�_�[��`�悷��
	if (!disp_proc_over_order(TNM_STAGE_FRONT))
		return false;

	return true;
}

// ****************************************************************
// ���C�v�`��F���C�v�Ȃ��i�҂��Ă���\���j
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_not_wipe_befor_wait(int wipe_prg)
{
	// --- ���� ---

	// ���C�v��艺�̃I�[�_�[��`�揀������
	if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃l�N�X�g��`�悷��
	if (!disp_proc_wipe_order_ready(TNM_STAGE_NEXT))
		return false;

	// ���C�v����̃I�[�_�[��`�揀������
	if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
		return false;

	// --- �`�� ---

	// ���C�v��艺�̃I�[�_�[��`�悷��
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃l�N�X�g��`�悷��
	if (!disp_proc_wipe_order(TNM_STAGE_NEXT))
		return false;

	// ���C�v����̃I�[�_�[��`�悷��
	if (!disp_proc_over_order(TNM_STAGE_FRONT))
		return false;

	return true;
}

// ****************************************************************
// ���C�v�`��F�N���X�t�F�[�h
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_cross_fade(int wipe_prg)
{
	// --- ���� ---

	// ���C�v��艺�̃I�[�_�[��`�揀������
	if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃l�N�X�g��`�揀������
	if (!disp_proc_wipe_order_ready(TNM_STAGE_NEXT))
		return false;

	// ���C�v����I�[�_�[�̃t�����g��`�揀������
	if (!disp_proc_wipe_order_ready(TNM_STAGE_FRONT))
		return false;

	// ���C�v����̃I�[�_�[��`�揀������
	if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
		return false;

	// --- �`�� ---

	// ���C�v��艺�̃I�[�_�[��`�悷��
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃l�N�X�g��`�悷��
	if (!disp_proc_wipe_order(TNM_STAGE_NEXT))
		return false;

	// ���C�v�o�b�t�@����������
	if (!disp_proc_ready_wipe_buffer_0())
		return false;

	// �Ŕw�ʂ�`�悷��
	m_view.clear_render_target(m_wipe_back_color);

	// ���C�v��艺�̃I�[�_�[��`�悷��
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�悷��
	if (!disp_proc_wipe_order(TNM_STAGE_FRONT))
		return false;

	// ���C�v�o�b�t�@��`�悷��
	{
		// ���C�v�X�v���C�g�̏�����
		C_d3d_sprite wipe_sprite;
		wipe_sprite.create_polygon_no_init();
		disp_proc_init_wipe_sprite(&wipe_sprite);
		wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
		wipe_sprite.set_index_buffer(G_rect_index_buffer);

		// ���C�v�X�v���C�g�̒ǉ��ݒ�
		wipe_sprite.rp.tr = linear_limit(wipe_prg, 0, 0, TNM_WIPE_PRG_MAX, 255);

		// ���C�v�X�v���C�g���X�V����
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
			G_rect_vertex_buffer_d2_screen->unlock();

			// �����_�[�^�[�Q�b�g���^�[�Q�b�g�o�b�t�@�ɐݒ肷��
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// �r���[�|�[�g���Q�[����ʃT�C�Y�ɂ��킹��
			m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

			// ���C�v�X�v���C�g��`��
			if (!disp_proc_sprite(&wipe_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d2_screen->unlock();
		}
	}

	// ���C�v����̃I�[�_�[��`�悷��
	if (!disp_proc_over_order(TNM_STAGE_FRONT))
		return false;

	return true;
}

// ****************************************************************
// ���C�v�`��F�}�X�N
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_mask(int wipe_prg)
{
	// ����̂݃}�X�N���쐬����
	if (check_and_copy_for_wipe_param())
		make_mask();

	// �X�e�[�W�擾
	bool is_front = (Gp_wipe->m_type == 901 && Gp_wipe->m_option[0] != 0) ? true : false;

	// --- ���� ---

	// ���C�v��艺�̃I�[�_�[��`�揀������
	if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃l�N�X�g��`�揀������
	if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
		return false;

	// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�揀������
	if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
		return false;

	// ���C�v����̃I�[�_�[��`�揀������
	if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
		return false;

	// --- �`�� ---

	// ���C�v��艺�̃I�[�_�[��`�悷��
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃l�N�X�g��`�悷��
	if (!disp_proc_wipe_order(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
		return false;

	// ���C�v�o�b�t�@����������
	if (!disp_proc_ready_wipe_buffer_0())
		return false;

	// ���C�v�o�b�t�@��h��Ԃ�
	m_view.clear_render_target(m_wipe_back_color);

	// ���C�v��艺�̃I�[�_�[��`�悷��
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�悷��
	if (!disp_proc_wipe_order(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
		return false;

	// ���C�v�o�b�t�@��`�悷��
	{
		// �}�X�N���C�v�p�̃X�v���C�g�̏�����
		C_d3d_sprite mask_wipe_sprite;
		mask_wipe_sprite.create_polygon_no_init();
		disp_proc_init_wipe_sprite(&mask_wipe_sprite);
		mask_wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_mask_wipe);
		mask_wipe_sprite.set_index_buffer(G_rect_index_buffer);

		// �}�X�N���C�v�p�̃X�v���C�g�̒ǉ��ݒ�
		disp_proc_wipe_for_mask_func(&mask_wipe_sprite, wipe_prg);

		// �}�X�N���C�v�p�̃X�v���C�g���X�V����
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d2_mask_wipe->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (mask_wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
			G_rect_vertex_buffer_d2_mask_wipe->unlock();

			// �����_�[�^�[�Q�b�g���Q�[���o�b�t�@�ɐݒ肷��
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// �r���[�|�[�g���Q�[����ʃT�C�Y�ɂ��킹��
			m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

			// �}�X�N���C�v�p�̃X�v���C�g��`�悷��
			if (!disp_proc_sprite(&mask_wipe_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d2_mask_wipe->unlock();
		}
	}

	// ���C�v����̃I�[�_�[��`�悷��
	if (!disp_proc_over_order(TNM_STAGE_FRONT))
		return false;

	return true;
}

// ****************************************************************
// ���C�v�`��F�}�X�N�e�t�m�b
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_mask_func(C_d3d_sprite* p_sprite, int wipe_prg)
{
	switch (Gp_wipe->m_type)	{
		case 901:
			if (m_mask_buffer_dib)	{
				S_mask_buffer buf;
				BYTE *dp;
				int ex1, ey1, ex2, ey2;
				int buf_w = Gp_global->game_screen_size.cx;
				int buf_h = Gp_global->game_screen_size.cy;
				if (Gp_wipe->m_option[3] > 0)	{
					int alpha = (Gp_wipe->m_option[3] >= 64) ? 64 : Gp_wipe->m_option[3];
					buf_w = Gp_global->game_screen_size.cx / alpha;
					buf_h = Gp_global->game_screen_size.cy / alpha;
				}
				if (tnm_alloc_mask_buffer(&buf, buf_w, buf_h, &dp, &ex1, &ey1, &ex2, &ey2))	{
					int dib_w = m_mask_buffer_dib->get_width();
					int dib_h = m_mask_buffer_dib->get_height();
					double max = sqrt((double)Gp_wipe->m_option[1] / 1000);
					double now;
					if (Gp_wipe->m_option[0] == 0)	{
						now = linear_limit<double, double>(wipe_prg, 0, (double)0.0, TNM_WIPE_PRG_MAX, (double)max);
					}
					else	{
						now = linear_limit<double, double>(wipe_prg, 0, (double)max, TNM_WIPE_PRG_MAX, (double)0.0);
					}
					double scale = now * now;
					int rotate = linear_limit<int, int>(wipe_prg, 0, 0, TNM_WIPE_PRG_MAX, 3600 * Gp_wipe->m_option[2]);
					tnm_draw_32bit_mask_rotate_scale(
						dp, buf.w, buf.h,
						m_mask_buffer_dib->get_ptr(), dib_w, dib_h,
						(dib_w / 2 - 1), (dib_h / 2 - 1),
						(buf.w / 2 - 1), (buf.h / 2 - 1),
						rotate, scale, scale, false
					);
					if (buf.w == Gp_global->game_screen_size.cx && buf.h == Gp_global->game_screen_size.cy)	{
						tnm_make_mask_texture(&m_mask_buffer_texture, &buf);
					}
					else	{
						if (Gp_wipe->m_option[4] == 0)	{
							tnm_make_mask_texture_with_duplicate(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, &buf);
						}
						else	{
							S_mask_buffer buf2;
							tnm_make_mask_texture_for_gap(&buf2, buf.w, buf.h, &buf);
							tnm_make_mask_texture_with_duplicate(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, &buf2);
						}
					}
				}
			}
			p_sprite->set_effect_constant_f(0, 1.0, 1.0, 1.0, 0.5);
			break;
		default:
			{
				float fade_float;
				float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;
				switch (Gp_wipe->m_option[0])	{
					case 0:		fade_float = 0.0f;				break;
					case 1:		fade_float = 1.0f - 1.0f / 2;	break;
					case 2:		fade_float = 1.0f - 1.0f / 4;	break;
					case 3:		fade_float = 1.0f - 1.0f / 8;	break;
					case 4:		fade_float = 1.0f - 1.0f / 16;	break;
					case 5:		fade_float = 1.0f - 1.0f / 32;	break;
					case 6:		fade_float = 1.0f - 1.0f / 64;	break;
					case 7:		fade_float = 1.0f - 1.0f / 128;	break;
					case 8:		fade_float = 1.0f;				break;
					default:	fade_float = 1.0f;				break;
				}
				p_sprite->set_effect_constant_f(0, fade_float, fade_float, fade_float, wipe_prg_float);
			}
			break;
	}

	// ���R�͂킩��Ȃ�����Ɏ����čs���ƃo�O��
	p_sprite->set_texture(m_mask_buffer_texture, TNM_TEXTURE_STAGE_MASK);
	p_sprite->set_effect_technique(m_view.m_default_effect.m_mask_technique);
	p_sprite->rp.size_fit_to_texture = 0;
}

// ****************************************************************
// ���C�v�`��F�V�~
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_shimi(int wipe_prg)
{
	// --- ���� ---

	// ���C�v��艺�̃I�[�_�[��`�揀������
	if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃l�N�X�g��`�揀������
	if (!disp_proc_wipe_order_ready(TNM_STAGE_NEXT))
		return false;

	// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�揀������
	if (!disp_proc_wipe_order_ready(TNM_STAGE_FRONT))
		return false;

	// ���C�v����̃I�[�_�[��`�揀������
	if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
		return false;

	// --- �`�� ---

	// ���C�v��艺�̃I�[�_�[��`�悷��
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃l�N�X�g��`�悷��
	if (!disp_proc_wipe_order(TNM_STAGE_NEXT))
		return false;

	// ���C�v�o�b�t�@����������
	if (!disp_proc_ready_wipe_buffer_0())
		return false;

	// ���C�v�o�b�t�@��h��Ԃ�
	m_view.clear_render_target(m_wipe_back_color);

	// ���C�v��艺�̃I�[�_�[��`�悷��
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�悷��
	if (!disp_proc_wipe_order(TNM_STAGE_FRONT))
		return false;

	// �����܂ł�RT�̓��C�v�o�b�t�@�ɐݒ肳��A���C�v�o�b�t�@���̂ւ̕`��͊���
	// ��������RT��ύX���A���C�v�o�b�t�@���X�v���C�g�Ƃ��ĕ`�悷��

	// ���C�v�o�b�t�@��`�悷��
	{
		// ���C�v�X�v���C�g�̏�����
		C_d3d_sprite wipe_sprite;
		wipe_sprite.create_polygon_no_init();
		disp_proc_init_wipe_sprite(&wipe_sprite);
		wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
		wipe_sprite.set_index_buffer(G_rect_index_buffer);

		// ���݃��C�v�p�̃X�v���C�g�̒ǉ��ݒ�
		disp_proc_wipe_for_shimi_func(&wipe_sprite, wipe_prg);

		// ���C�v�X�v���C�g���X�V����
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
			G_rect_vertex_buffer_d2_screen->unlock();

			// �����_�[�^�[�Q�b�g���^�[�Q�b�g�o�b�t�@�ɐݒ肷��
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// �r���[�|�[�g���Q�[����ʃT�C�Y�ɂ��킹��
			m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

			// ���C�v�X�v���C�g��`��
			if (!disp_proc_sprite(&wipe_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d2_screen->unlock();
		}
	}

	// ���C�v����̃I�[�_�[��`�悷��
	if (!disp_proc_over_order(TNM_STAGE_FRONT))
		return false;

	return true;
}

// ****************************************************************
// ���C�v�`��F�V�~�e�t�m�b
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_shimi_func(C_d3d_sprite* p_sprite, int wipe_prg)
{
	//switch (Gp_wipe->m_type)	{
	//	default:
			{
				float fade_float;
				float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;
				switch (Gp_wipe->m_option[0])	{
					case 0:		fade_float = 0.0f;				break;
					case 1:		fade_float = 1.0f - 1.0f / 2;	break;
					case 2:		fade_float = 1.0f - 1.0f / 4;	break;
					case 3:		fade_float = 1.0f - 1.0f / 8;	break;
					case 4:		fade_float = 1.0f - 1.0f / 16;	break;
					case 5:		fade_float = 1.0f - 1.0f / 32;	break;
					case 6:		fade_float = 1.0f - 1.0f / 64;	break;
					case 7:		fade_float = 1.0f - 1.0f / 128;	break;
					case 8:		fade_float = 1.0f;				break;
					default:	fade_float = 1.0f;				break;
				}
				p_sprite->set_effect_constant_f(0, fade_float, fade_float, fade_float, wipe_prg_float);
			}
	//		break;
	//}

	// �Á���
	if(Gp_wipe->m_option[1] == 0)
		p_sprite->set_effect_technique(m_view.m_default_effect.m_shimi_technique);
	// ������
	else// if(Gp_wipe->m_option[1] == 1)
		p_sprite->set_effect_technique(m_view.m_default_effect.m_shimi_inv_technique);
}


// ****************************************************************
// ���C�v�`��F�N���X���X�^
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_cross_raster(int wipe_prg)
{
	// --- ���� ---

	// ���C�v��艺�̃I�[�_�[��`�揀������
	if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃l�N�X�g��`�揀������
	if (!disp_proc_wipe_order_ready(TNM_STAGE_NEXT))
		return false;

	// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�揀������
	if (!disp_proc_wipe_order_ready(TNM_STAGE_FRONT))
		return false;

	// ���C�v����̃I�[�_�[��`�揀������
	if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
		return false;

	// --- �`�� ---
	
	// ���C�v�o�b�t�@�P����������
	if (!disp_proc_ready_wipe_buffer_1())
		return false;

	// ���C�v�o�b�t�@��h��Ԃ�
	m_view.clear_render_target(m_wipe_back_color);

	// ���C�v��艺�̃I�[�_�[��`�悷��
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃l�N�X�g��`�悷��
	if (!disp_proc_wipe_order(TNM_STAGE_NEXT))
		return false;

	// ���C�v�o�b�t�@�O����������
	if (!disp_proc_ready_wipe_buffer_0())
		return false;

	// ���C�v�o�b�t�@��h��Ԃ�
	m_view.clear_render_target(m_wipe_back_color);

	// ���C�v��艺�̃I�[�_�[��`�悷��
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�悷��
	if (!disp_proc_wipe_order(TNM_STAGE_FRONT))
		return false;

	// �����܂ł�RT�̓��C�v�o�b�t�@�ɐݒ肳��A���C�v�o�b�t�@���̂ւ̕`��͊���
	// ��������RT��ύX���A���C�v�o�b�t�@���X�v���C�g�Ƃ��ĕ`�悷��

	// ���C�v�o�b�t�@��`�悷��
	{
		// ���C�v�X�v���C�g�̏�����
		C_d3d_sprite wipe_sprite;
		wipe_sprite.create_polygon_no_init();
		disp_proc_init_wipe_sprite(&wipe_sprite);
		wipe_sprite.set_texture(m_wipe_back_buffer_texture_1, TNM_TEXTURE_STAGE_MASK);		// ���C�v�o�b�t�@�P���ݒ肷��
		wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
		wipe_sprite.set_index_buffer(G_rect_index_buffer);

		// ���X�^���C�v�p�̃X�v���C�g�̒ǉ��ݒ�
		disp_proc_wipe_for_raster_func(&wipe_sprite, wipe_prg);

		// ���C�v�X�v���C�g���X�V����
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
			G_rect_vertex_buffer_d2_screen->unlock();

			// �����_�[�^�[�Q�b�g���^�[�Q�b�g�o�b�t�@�ɐݒ肷��
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// �r���[�|�[�g���Q�[����ʃT�C�Y�ɂ��킹��
			m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

			// ���C�v�X�v���C�g��`��
			if (!disp_proc_sprite(&wipe_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d2_screen->unlock();
		}
	}

	// ���C�v����̃I�[�_�[��`�悷��
	if (!disp_proc_over_order(TNM_STAGE_FRONT))
		return false;

	return true;
}

// ****************************************************************
// ���C�v�`��F���X�^����
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_raster(int wipe_prg)
{
	// �X�e�[�W�擾
	// �V���O�����X�^�̏ꍇ
	bool is_front = (Gp_wipe->m_type == 221 && Gp_wipe->m_option[4] != 0) ? true : false;

	// --- ���� ---

	// ���C�v��艺�̃I�[�_�[��`�揀������
	if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃l�N�X�g��`�揀������
	if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
		return false;

	// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�揀������
	if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
		return false;

	// ���C�v����̃I�[�_�[��`�揀������
	if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
		return false;

	// --- �`�� ---

	// ���C�v��艺�̃I�[�_�[��`�悷��
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃l�N�X�g��`�悷��
	if (!disp_proc_wipe_order(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
		return false;

	// ���C�v�o�b�t�@����������
	if (!disp_proc_ready_wipe_buffer_0())
		return false;

	// ���C�v�o�b�t�@��h��Ԃ�
	m_view.clear_render_target(m_wipe_back_color);

	// ���C�v��艺�̃I�[�_�[��`�悷��
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�悷��
	if (!disp_proc_wipe_order(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
		return false;

	// �����܂ł�RT�̓��C�v�o�b�t�@�ɐݒ肳��A���C�v�o�b�t�@���̂ւ̕`��͊���
	// ��������RT��ύX���A���C�v�o�b�t�@���X�v���C�g�Ƃ��ĕ`�悷��

	// ���C�v�o�b�t�@��`�悷��
	{
		// ���C�v�X�v���C�g�̏�����
		C_d3d_sprite wipe_sprite;
		wipe_sprite.create_polygon_no_init();
		disp_proc_init_wipe_sprite(&wipe_sprite);
		wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
		wipe_sprite.set_index_buffer(G_rect_index_buffer);

		// ���X�^���C�v�p�̃X�v���C�g�̒ǉ��ݒ�
		disp_proc_wipe_for_raster_func(&wipe_sprite, wipe_prg);

		// ���C�v�X�v���C�g���X�V����
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
			G_rect_vertex_buffer_d2_screen->unlock();

			// �����_�[�^�[�Q�b�g���^�[�Q�b�g�o�b�t�@�ɐݒ肷��
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// �r���[�|�[�g���Q�[����ʃT�C�Y�ɂ��킹��
			m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

			// ���C�v�X�v���C�g��`��
			if (!disp_proc_sprite(&wipe_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d2_screen->unlock();
		}
	}

	// ���C�v����̃I�[�_�[��`�悷��
	if (!disp_proc_over_order(TNM_STAGE_FRONT))
		return false;

	return true;
}


// ****************************************************************
// ���C�v�`��F���X�^���ʂe�t�m�b
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_raster_func(C_d3d_sprite* p_sprite, int wipe_prg)
{
	float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;
	p_sprite->set_effect_constant_f(0, wipe_prg_float, wipe_prg_float, wipe_prg_float, wipe_prg_float);

	switch(Gp_wipe->m_type) {
		// �N���X���X�^
		case 220:
			// Gp_wipe->m_option[1]�s�N�Z�����Ƃɂ����,��{�I�Ȕg����3,�␳�͖���
			if(Gp_wipe->m_option[1] <= 0)
				Gp_wipe->m_option[1] = 1;

			// �c
			if(Gp_wipe->m_option[0] == 0) {
				p_sprite->set_effect_technique(m_view.m_default_effect.m_cross_raster_vertical_technique);
				//p_sprite->set_effect_constant_f((float)Gp_global->game_screen_size.cy / Gp_wipe->m_option[1], Gp_wipe->m_option[2], wipe_prg_float, wipe_prg_float);
				p_sprite->set_effect_constant_f(0, p_sprite->rp.size.y / Gp_wipe->m_option[1], (float)Gp_wipe->m_option[2], (float)Gp_wipe->m_option[3], wipe_prg_float);
			}
			// ��
			else// if(Gp_wipe->m_option[0] == 1) {
			{
				p_sprite->set_effect_technique(m_view.m_default_effect.m_cross_raster_horizontal_technique);
				//p_sprite->set_effect_constant_f((float)Gp_global->game_screen_size.cx / Gp_wipe->m_option[1], Gp_wipe->m_option[2], wipe_prg_float, wipe_prg_float);
				p_sprite->set_effect_constant_f(0, p_sprite->rp.size.x / Gp_wipe->m_option[1], (float)Gp_wipe->m_option[2], (float)Gp_wipe->m_option[3], wipe_prg_float);
			}
			break;
		// �ʏ탉�X�^
		case 221:
			// Gp_wipe->m_option[1]�s�N�Z�����Ƃɂ����,��{�I�Ȕg����3,�␳�͖���
			if(Gp_wipe->m_option[1] <= 0)
				Gp_wipe->m_option[1] = 1;

			// �c
			if(Gp_wipe->m_option[0] == 0) {
				p_sprite->set_effect_technique(m_view.m_default_effect.m_raster_vertical_technique);
				float wpf = (Gp_wipe->m_option[4]) ? 1.0f - wipe_prg_float : wipe_prg_float;
				p_sprite->set_effect_constant_f(0, p_sprite->rp.size.y / Gp_wipe->m_option[1], (float)Gp_wipe->m_option[2], (float)Gp_wipe->m_option[3], wpf);
			}
			// ��
			else// if(Gp_wipe->m_option[0] == 1) {
			{
				p_sprite->set_effect_technique(m_view.m_default_effect.m_raster_horizontal_technique);
				float wpf = (Gp_wipe->m_option[4]) ? 1.0f - wipe_prg_float : wipe_prg_float;
				p_sprite->set_effect_constant_f(0, p_sprite->rp.size.x / Gp_wipe->m_option[1], (float)Gp_wipe->m_option[2], (float)Gp_wipe->m_option[3], wpf);
			}
			break;
		default:
			break;
	}
}

// ****************************************************************
// ���C�v�`��F�ړ��n
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_move(int wipe_prg)
{
	// ����̂݃}�X�N���쐬����
	if (check_and_copy_for_wipe_param())
		tnm_make_mask_texture_for_direction(&m_mask_buffer_texture, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy, false, 0);

	// �X�e�[�W�擾
	bool is_front = disp_proc_wipe_for_move_get_stage(wipe_prg);

	// --- ���� ---

	// ���C�v��艺�̃I�[�_�[��`�揀������
	if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃l�N�X�g�����C�v�o�b�t�@�ɕ`�揀������
	if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
		return false;

	// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�揀������
	if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
		return false;

	// ���C�v����̃I�[�_�[��`�揀������
	if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
		return false;

	// --- �`�� ---

	// ���ʂ̃I�[�_�[���܂߂����C�v
	if (Gp_wipe->m_with_low_order)	{

		// ���C�v�o�b�t�@����������
		if (!disp_proc_ready_wipe_buffer_0())
			return false;

		// ���C�v�o�b�t�@��h��Ԃ�
		m_view.clear_render_target(m_wipe_back_color);

		// ���C�v��艺�̃I�[�_�[��`�悷��
		if (!disp_proc_under_order(TNM_STAGE_FRONT))
			return false;

		// ���C�v����I�[�_�[�̃l�N�X�g�����C�v�o�b�t�@�ɕ`�悷��
		if (!disp_proc_wipe_order(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
			return false;

		// ���C�v�o�b�t�@���Q�[���o�b�t�@�ɕ`�悷��
		if (!disp_proc_wipe_for_move_switch(wipe_prg, is_front ? true : false))
			return false;

		// ���C�v�o�b�t�@����������
		if (!disp_proc_ready_wipe_buffer_0())
			return false;

		// ���C�v�o�b�t�@��h��Ԃ�
		m_view.clear_render_target(m_wipe_back_color);

		// ���C�v��艺�̃I�[�_�[��`�悷��
		if (!disp_proc_under_order(TNM_STAGE_FRONT))
			return false;

		// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�悷��
		if (!disp_proc_wipe_order(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
			return false;

		// ���C�v�o�b�t�@���Q�[���o�b�t�@�ɕ`�悷��
		if (!disp_proc_wipe_for_move_switch(wipe_prg, is_front ? false : true))
			return false;
	}
	// �w�肵���I�[�_�[�݂̂̃��C�v
	else	{

		// ���C�v��艺�̃I�[�_�[��`�悷��
		if (!disp_proc_under_order(TNM_STAGE_FRONT))
			return false;

		// ���C�v�o�b�t�@����������
		if (!disp_proc_ready_wipe_buffer_0())
			return false;

		// ���C�v����I�[�_�[�̃l�N�X�g�����C�v�o�b�t�@�ɕ`�悷��
		if (!disp_proc_wipe_order(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
			return false;

		// ���C�v�o�b�t�@���Q�[���o�b�t�@�ɕ`�悷��
		if (!disp_proc_wipe_for_move_switch(wipe_prg, is_front ? true : false))
			return false;

		// ���C�v�o�b�t�@����������
		if (!disp_proc_ready_wipe_buffer_0())
			return false;

		// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�悷��
		if (!disp_proc_wipe_order(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
			return false;

		// ���C�v�o�b�t�@���Q�[���o�b�t�@�ɕ`�悷��
		if (!disp_proc_wipe_for_move_switch(wipe_prg, is_front ? false : true))
			return false;
	}

	// ���C�v����̃I�[�_�[��`�悷��
	if (!disp_proc_over_order(TNM_STAGE_FRONT))
		return false;

	return true;
}

// ****************************************************************
// ���C�v�`��F�ړ��n�X�e�[�W�擾
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_move_get_stage(int wipe_prg)
{
	switch (Gp_wipe->m_type)	{
		case 200:		// �X�N���[��
			if (Gp_wipe->m_option[1] == 0)			{	return true;	}
			break;
		case 301:		// �y�[�W�߂���
			if (wipe_prg < TNM_WIPE_PRG_MAX / 2)	{	return true;	}
			break;
	}
	return false;
}

// ****************************************************************
// ���C�v�`��F�ړ��n�X�C�b�`
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_move_switch(int wipe_prg, bool is_front)
{
	if (Gp_wipe->m_type == 200)	{

		// ���C�v�X�v���C�g
		C_d3d_sprite wipe_sprite;
		wipe_sprite.create_polygon_no_init();
		disp_proc_init_wipe_sprite(&wipe_sprite);
		wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
		wipe_sprite.set_index_buffer(G_rect_index_buffer);

		// ����
		switch (Gp_wipe->m_type)	{
			case 200:		// �X�N���[���ƐL�k�ό`
				disp_proc_wipe_for_move_scroll_and_expansion_and_contraction(&wipe_sprite, wipe_prg, is_front);
				break;
		}

		// ���C�v�X�v���C�g���X�V����
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
			G_rect_vertex_buffer_d2_screen->unlock();

			// �����_�[�^�[�Q�b�g���^�[�Q�b�g�o�b�t�@�ɐݒ肷��
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// �r���[�|�[�g���Q�[����ʃT�C�Y�ɂ��킹��
			m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

			// �X�v���C�g��`�悷��
			if (!disp_proc_sprite(&wipe_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d2_screen->unlock();
		}
	}
	else if (Gp_wipe->m_type == 300)	{

		float sx = (float)Gp_global->game_screen_size.cx;
		float sy = (float)Gp_global->game_screen_size.cy;

		// ���C�v�X�v���C�g
		C_d3d_sprite wipe_sprite;
		wipe_sprite.create_polygon_no_init();
		disp_proc_init_wipe_sprite(&wipe_sprite);
		wipe_sprite.rp.d2_rect = false;			// �Q�c��`���O��
		wipe_sprite.rp.culling = true;			// �J�����O����
		wipe_sprite.rp.alpha_test = true;		// �A���t�@�e�X�g����
		wipe_sprite.rp.alpha_blend = true;		// �A���t�@�u�����h����
		wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d3_screen);
		wipe_sprite.set_index_buffer(G_rect_index_buffer);

		bool reverse_flag = Gp_wipe->m_option[0] ? true : false;
		int range_type = Gp_wipe->m_option[2];

		float s_rad = 0.0f, h_rad = 0.0f, e_rad = 0.0f;	// start, half, end
		if (is_front)	{
			if (reverse_flag)	{	s_rad = D3DX_PI;	e_rad = 0.0f;			}
			else				{	s_rad = D3DX_PI;	e_rad = D3DX_PI * 2;	}
		}
		else	{
			if (reverse_flag)	{	s_rad = 0.0f;		e_rad = - D3DX_PI;		}
			else				{	s_rad = 0.0f;		e_rad = D3DX_PI;		}
		}
		h_rad = (s_rad + e_rad) / 2;

		switch (range_type)	{
			case 1:
				wipe_sprite.rp.rotate.y = (float)linear_limit<int, float>(wipe_prg, 0, h_rad, TNM_WIPE_PRG_MAX, e_rad);
				break;
			case 2:
				wipe_sprite.rp.rotate.y = (float)linear_limit<int, float>(wipe_prg, 0, s_rad, TNM_WIPE_PRG_MAX, h_rad);
				break;
			default:
				wipe_sprite.rp.rotate.y = (float)linear_limit<int, float>(wipe_prg, 0, s_rad, TNM_WIPE_PRG_MAX, e_rad);
				break;
		}

		// ���_�̐ݒ�
		// ���[���h�̒u���Ƃ��� x-=0.5, y+=0.5
		D3DXVECTOR4 vertex_pos[4];
		vertex_pos[0] = D3DXVECTOR4(- sx / 2 - 0.5f, sy / 2 + 0.5f, 0.0f, 1.0f);
		vertex_pos[1] = D3DXVECTOR4(sx / 2 - 0.5f, sy / 2 + 0.5f, 0.0f, 1.0f);
		vertex_pos[2] = D3DXVECTOR4(- sx / 2 - 0.5f, - sy / 2 + 0.5f, 0.0f, 1.0f);
		vertex_pos[3] = D3DXVECTOR4(sx / 2 - 0.5f, - sy / 2 + 0.5f, 0.0f, 1.0f);

		// ���C�v�X�v���C�g���X�V����
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d3_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, vertex_pos, NULL))	{
			G_rect_vertex_buffer_d3_screen->unlock();

			// �����_�[�^�[�Q�b�g���^�[�Q�b�g�o�b�t�@�ɐݒ肷��
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// �r���[�̃J������ݒ�
			D3DXVECTOR3 camera_eye, camera_pint, camera_up;
			float camera_aspect, view_angle_degree, view_angle_radian, zn, zf;
			view_angle_degree = (float)Gp_wipe->m_option[1] / TNM_ANGLE_UNIT;
			camera_eye.x = 0.0f;
			camera_eye.y = 0.0f;
			camera_eye.z = -(sy / 2) * tan(D3DXToRadian(90.0f - view_angle_degree / 2));
			camera_pint.x = 0.0f;
			camera_pint.y = 0.0f;
			camera_pint.z = 0.0f;
			camera_up.x = 0.0f;
			camera_up.y = 1.0f;
			camera_up.z = 0.0f;
			camera_aspect = (float)Gp_global->game_screen_size.cx / Gp_global->game_screen_size.cy;
			view_angle_radian = D3DXToRadian(view_angle_degree);
			zn = 1.0f;
			zf = 10000.0f;

			// �r���[�s��̐ݒ�
			D3DXMatrixLookAtLH(&m_view.m_mat_view, &camera_eye, &camera_pint, &camera_up);

			// �ˉe�s��̐ݒ�
			D3DXMatrixPerspectiveFovLH(&m_view.m_mat_proj, view_angle_radian, camera_aspect, zn, zf);
			
			// �r���[�|�[�g���Q�[����ʃT�C�Y�ɂ��킹��
			m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

			// ���C�g�̐ݒ�
			wipe_sprite.rp.light_no = TNM_LIGHT_NONE;

			// �X�v���C�g��`�悷��
			if (!disp_proc_sprite(&wipe_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d3_screen->unlock();
		}
	}

	// ���C�v�`��F�ړ��n�F�y�[�W�߂���i�R�O�P�ԁj
	else if (Gp_wipe->m_type == 301)	{

		for (int i = 0; i < 2; i++)	{	// ���A�E

			float sx = (float)Gp_global->game_screen_size.cx;
			float sy = (float)Gp_global->game_screen_size.cy;

			// ���C�v�X�v���C�g
			C_d3d_sprite wipe_sprite;
			wipe_sprite.create_polygon_no_init();
			disp_proc_init_wipe_sprite(&wipe_sprite);
			wipe_sprite.rp.d2_rect = false;			// �Q�c��`���O��
			wipe_sprite.rp.culling = true;			// �J�����O����
			wipe_sprite.rp.alpha_test = true;		// �A���t�@�e�X�g����
			wipe_sprite.rp.alpha_blend = true;		// �A���t�@�u�����h����
			wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d3_screen);
			wipe_sprite.set_index_buffer(G_rect_index_buffer);
		
			bool reverse_flag = Gp_wipe->m_option[0] ? true : false;
			int range_type = Gp_wipe->m_option[2];

			float z = 0.0f;
			float s_rad = 0.0f, h_rad = 0.0f, e_rad = 0.0f;	// start, half, end
			if (i == 0)	{
				if (is_front)	{
					if (reverse_flag)	{	s_rad = 0.0f;			e_rad = 0.0f;			z = 0.0f;	}
					else				{	s_rad = D3DX_PI;		e_rad = D3DX_PI * 2;	z = -1.0f;	}
				}
				else	{
					if (reverse_flag)	{	s_rad = D3DX_PI * 2;	e_rad = D3DX_PI;		z = -1.0f;	}
					else				{	s_rad = 0.0f;			e_rad = 0.0f;			z = 0.0f;	}
				}
			}
			else	{
				if (is_front)	{
					if (reverse_flag)	{	s_rad = D3DX_PI;		e_rad = 0.0f;			z = -1.0f;	}
					else				{	s_rad = 0.0f;			e_rad = 0.0f;			z = 0.0f;	}
				}
				else	{
					if (reverse_flag)	{	s_rad = 0.0f;			e_rad = 0.0f;			z = 0.0f;	}
					else				{	s_rad = 0.0f;			e_rad = D3DX_PI;		z = -1.0f;	}
				}
			}

			h_rad = (s_rad + e_rad) / 2;

			switch (range_type)	{
				case 1:
					wipe_sprite.rp.rotate.y = (float)linear_limit<int, float>(wipe_prg, 0, h_rad, TNM_WIPE_PRG_MAX, e_rad);
					break;
				case 2:
					wipe_sprite.rp.rotate.y = (float)linear_limit<int, float>(wipe_prg, 0, s_rad, TNM_WIPE_PRG_MAX, h_rad);
					break;
				default:
					wipe_sprite.rp.rotate.y = (float)linear_limit<int, float>(wipe_prg, 0, s_rad, TNM_WIPE_PRG_MAX, e_rad);
					break;
			}

			// �ʒu
			// ���[���h�ɒu���Ƃ��� x-=0.5, y+=0.5
			D3DXVECTOR4 vertex_pos[4];
			if (i == 0)	{
				vertex_pos[0] = D3DXVECTOR4(- sx / 2 - 0.5f, sy / 2 + 0.5f, z, 1.0f);
				vertex_pos[1] = D3DXVECTOR4(- 0.5f, sy / 2 + 0.5f, z, 1.0f);
				vertex_pos[2] = D3DXVECTOR4(- sx / 2 - 0.5f, - sy / 2 + 0.5f, z, 1.0f);
				vertex_pos[3] = D3DXVECTOR4(- 0.5f, - sy / 2 + 0.5f, z, 1.0f);
			}
			else	{
				vertex_pos[0] = D3DXVECTOR4(- 0.5f, sy / 2 + 0.5f, z, 1.0f);
				vertex_pos[1] = D3DXVECTOR4(sx / 2 - 0.5f, sy / 2 + 0.5f, z, 1.0f);
				vertex_pos[2] = D3DXVECTOR4(- 0.5f, - sy / 2 + 0.5f, z, 1.0f);
				vertex_pos[3] = D3DXVECTOR4(sx / 2 - 0.5f, - sy / 2 + 0.5f, z, 1.0f);
			}

			// �e�N�X�`���t�u
			D3DXVECTOR2 tex_pos[4];
			if (i == 0)	{
				tex_pos[0] = D3DXVECTOR2(0.0f, 0.0f);
				tex_pos[1] = D3DXVECTOR2(0.5f, 0.0f);
				tex_pos[2] = D3DXVECTOR2(0.0f, 1.0f);
				tex_pos[3] = D3DXVECTOR2(0.5f, 1.0f);
			}
			else	{
				tex_pos[0] = D3DXVECTOR2(0.5f, 0.0f);
				tex_pos[1] = D3DXVECTOR2(1.0f, 0.0f);
				tex_pos[2] = D3DXVECTOR2(0.5f, 1.0f);
				tex_pos[3] = D3DXVECTOR2(1.0f, 1.0f);
			}

			// ���C�v�X�v���C�g���X�V����
			void* p_locked_vertex_buffer;
			G_rect_vertex_buffer_d3_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
			if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, vertex_pos, tex_pos))	{
				G_rect_vertex_buffer_d3_screen->unlock();

				// �����_�[�^�[�Q�b�g���^�[�Q�b�g�o�b�t�@�ɐݒ肷��
				m_view.set_render_target(m_target_back_buffer.body());
				m_view.set_dpst_buffer(m_target_dpst_buffer.body());

				// �J�����̐ݒ�
				D3DXVECTOR3 camera_eye, camera_pint, camera_up;
				float camera_aspect, view_angle_degree, view_angle_radian, zn, zf;

				view_angle_degree = (float)Gp_wipe->m_option[1] / TNM_ANGLE_UNIT;
				camera_eye.x = 0.0f;
				camera_eye.y = 0.0f;
				camera_eye.z = -(sy / 2) * tan(D3DXToRadian(90.0f - view_angle_degree / 2));
				camera_pint.x = 0.0f;
				camera_pint.y = 0.0f;
				camera_pint.z = 0.0f;
				camera_up.x = 0.0f;
				camera_up.y = 1.0f;
				camera_up.z = 0.0f;
				camera_aspect = (float)Gp_global->game_screen_size.cx / Gp_global->game_screen_size.cy;
				view_angle_radian = D3DXToRadian(view_angle_degree);
				zn = 1.0f;
				zf = 10000.0f;

				// �r���[�s��̐ݒ�
				D3DXMatrixLookAtLH(&m_view.m_mat_view, &camera_eye, &camera_pint, &camera_up);

				// �ˉe�s��̐ݒ�
				D3DXMatrixPerspectiveFovLH(&m_view.m_mat_proj, view_angle_radian, camera_aspect, zn, zf);
			
				// �r���[�|�[�g���Q�[����ʃT�C�Y�ɂ��킹��
				m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
				//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

				// ���C�g�̐ݒ�
				wipe_sprite.rp.light_no = TNM_LIGHT_NONE;

				// �X�v���C�g��`�悷��
				if (!disp_proc_sprite(&wipe_sprite))
					return false;
			}
			else	{
				G_rect_vertex_buffer_d3_screen->unlock();
			}
		}
	}

	return true;
}


// ****************************************************************
// ���C�v�`��F�ړ��n�F�X�N���[���ƐL�k�ό`
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_move_scroll_and_expansion_and_contraction(C_d3d_sprite* p_sprite, int wipe_prg, bool is_front)
{
	int scroll_mod = is_front ? Gp_wipe->m_option[1] : Gp_wipe->m_option[2];
	switch (scroll_mod)	{
		case 1:		disp_proc_wipe_for_move_func_scroll(wipe_prg, p_sprite, Gp_wipe->m_option[0], is_front);						break;
		case 2:		disp_proc_wipe_for_move_func_expansion_and_contraction(wipe_prg, p_sprite, Gp_wipe->m_option[0], is_front);		break;
	}
}

// ****************************************************************
// ���C�v�`��F�ړ��n�e�t�m�b�F�X�N���[��
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_move_func_scroll(int wipe_prg, C_d3d_sprite *sprite, int dir, bool is_in)
{
	if (is_in)	{
		switch (dir)	{
			case 0:		sprite->rp.pos.y = (float)linear_limit<int, int>(wipe_prg, 0, - Gp_global->game_screen_size.cy, TNM_WIPE_PRG_MAX, 0);	break;
			case 1:		sprite->rp.pos.y = (float)linear_limit<int, int>(wipe_prg, 0, Gp_global->game_screen_size.cy, TNM_WIPE_PRG_MAX, 0);		break;
			case 2:		sprite->rp.pos.x = (float)linear_limit<int, int>(wipe_prg, 0, - Gp_global->game_screen_size.cx, TNM_WIPE_PRG_MAX, 0);	break;
			case 3:		sprite->rp.pos.x = (float)linear_limit<int, int>(wipe_prg, 0, Gp_global->game_screen_size.cx, TNM_WIPE_PRG_MAX, 0);		break;
		}
	}
	else	{
		switch (dir)	{
			case 0:		sprite->rp.pos.y = (float)linear_limit<int, int>(wipe_prg, 0, 0, TNM_WIPE_PRG_MAX, Gp_global->game_screen_size.cy);		break;
			case 1:		sprite->rp.pos.y = (float)linear_limit<int, int>(wipe_prg, 0, 0, TNM_WIPE_PRG_MAX, - Gp_global->game_screen_size.cy);	break;
			case 2:		sprite->rp.pos.x = (float)linear_limit<int, int>(wipe_prg, 0, 0, TNM_WIPE_PRG_MAX, Gp_global->game_screen_size.cx);		break;
			case 3:		sprite->rp.pos.x = (float)linear_limit<int, int>(wipe_prg, 0, 0, TNM_WIPE_PRG_MAX, - Gp_global->game_screen_size.cx);	break;
		}
	}
}

// ****************************************************************
// ���C�v�`��F�ړ��n�e�t�m�b�F�L�k�ό`
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_move_func_expansion_and_contraction(int wipe_prg, C_d3d_sprite *sprite, int dir, bool is_in)
{
	if (is_in)	{
		switch (dir)	{
			case 0:
				sprite->rp.scale.y = linear_limit<int, float>(wipe_prg, 0, 0.0f, TNM_WIPE_PRG_MAX, 1.0f);
				break;
			case 1:
				sprite->rp.center.y = (float)Gp_global->game_screen_size.cy;
				sprite->rp.pos.y = (float)Gp_global->game_screen_size.cy;
				sprite->rp.scale.y = linear_limit<int, float>(wipe_prg, 0, 0.0f, TNM_WIPE_PRG_MAX, 1.0f);
				break;
			case 2:
				sprite->rp.scale.x = linear_limit<int, float>(wipe_prg, 0, 0.0f, TNM_WIPE_PRG_MAX, 1.0f);
				break;
			case 3:
				sprite->rp.center.x = (float)Gp_global->game_screen_size.cx;
				sprite->rp.pos.x = (float)Gp_global->game_screen_size.cx;
				sprite->rp.scale.x = linear_limit<int, float>(wipe_prg, 0, 0.0f, TNM_WIPE_PRG_MAX, 1.0f);
				break;
		}
	}
	else	{
		switch (dir)	{
			case 0:
				sprite->rp.center.y = (float)Gp_global->game_screen_size.cy;
				sprite->rp.pos.y = (float)Gp_global->game_screen_size.cy;
				sprite->rp.scale.y = linear_limit<int, float>(wipe_prg, 0, 1.0f, TNM_WIPE_PRG_MAX, 0.0f);
				break;
			case 1:
				sprite->rp.scale.y = linear_limit<int, float>(wipe_prg, 0, 1.0f, TNM_WIPE_PRG_MAX, 0.0f);
				break;
			case 2:
				sprite->rp.center.x = (float)Gp_global->game_screen_size.cx;
				sprite->rp.pos.x = (float)Gp_global->game_screen_size.cx;
				sprite->rp.scale.x = linear_limit<int, float>(wipe_prg, 0, 1.0f, TNM_WIPE_PRG_MAX, 0.0f);
				break;
			case 3:
				sprite->rp.scale.x = linear_limit<int, float>(wipe_prg, 0, 1.0f, TNM_WIPE_PRG_MAX, 0.0f);
				break;
		}
	}
}





// ****************************************************************
// ���C�v�`��F�X�P�[���n
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_scale(int wipe_prg)
{
	// �X�e�[�W�擾
	bool is_front = disp_proc_wipe_for_scale_get_stage(wipe_prg);

	// --- ���� ---

	// ���C�v��艺�̃I�[�_�[��`�揀������
	if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃l�N�X�g�����C�v�o�b�t�@�ɕ`�揀������
	if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
		return false;

	// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�揀������
	if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
		return false;

	// ���C�v����̃I�[�_�[��`�揀������
	if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
		return false;

	// --- �`�� ---

	// ���C�v��艺�̃I�[�_�[��`�悷��
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃l�N�X�g��`�悷��
	if (!disp_proc_wipe_order(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
		return false;

	// ���C�v�o�b�t�@����������
	if (!disp_proc_ready_wipe_buffer_0())
		return false;

	// ���C�v�o�b�t�@��h��Ԃ�
	m_view.clear_render_target(m_wipe_back_color);

	// ���C�v��艺�̃I�[�_�[��`�悷��
	if (!disp_proc_under_order(TNM_STAGE_FRONT))
		return false;

	// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�悷��
	if (!disp_proc_wipe_order(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
		return false;

	// ���C�v�o�b�t�@���Q�[���o�b�t�@�ɕ`�悷��
	if (!disp_proc_wipe_for_scale_switch(wipe_prg, is_front ? false : true))
		return false;

	// ���C�v����̃I�[�_�[��`�悷��
	if (!disp_proc_over_order(TNM_STAGE_FRONT))
		return false;

	return true;
}

// ****************************************************************
// ���C�v�`��F�X�P�[���n�X�e�[�W�擾
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_scale_get_stage(int wipe_prg)
{
	float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;
	
	switch (Gp_wipe->m_type)	{
		case 210:
			// ���G���g��\��
			return false;
			break;
		case 211:
			// ���G���k���\��
			return true;
			break;
		case 212:
			// ���G���g���A���G�ɕύX���ďk���\��
			if(wipe_prg_float < 0.5f) {
				return true;
			}
			else {
				return false;
			}
			break;
		case 213:
			// ���G���g���Ԃ���t�F�[�h�h�m���Ȃ���k���\��
			return false;
			break;
		case 214:
			// ���G���t�F�[�h�n�t�s���Ȃ���g��\��
			return true;
			break;
		case 215:
			// ���C�v�J�n���̋�`�͈͂��w�肵�āA���G���g��\���i�Q�P�O�̏����ς�����o�[�W�����ł��B�j
			if(Gp_wipe->m_option[1] == 0)
				return false;

			// ���C�v�I�����̋�`�͈͂��w�肵�āA���G���k���\���i�Q�P�P�̏����ς�����o�[�W�����ł��B�j
			else
				return true;
			break;
	}
	return false;
}


// ****************************************************************
// ���C�v�`��F�X�P�[���n�X�C�b�`
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_scale_switch(int wipe_prg, bool is_front)
{
	// ���C�v�o�b�t�@��`�悷��
	if (Gp_wipe->m_type == 210 || Gp_wipe->m_type == 211)	{
		// ���C�v�X�v���C�g�̏�����
		C_d3d_sprite wipe_sprite;
		wipe_sprite.create_polygon_no_init();
		disp_proc_init_wipe_sprite(&wipe_sprite);
		wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
		wipe_sprite.set_index_buffer(G_rect_index_buffer);

		// �X�P�[�����C�v�p�̃X�v���C�g�̒ǉ��ݒ�
		disp_proc_wipe_for_scale_func(&wipe_sprite, wipe_prg);

		// ���C�v�X�v���C�g���X�V����
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
			G_rect_vertex_buffer_d2_screen->unlock();

			// �����_�[�^�[�Q�b�g���^�[�Q�b�g�o�b�t�@�ɐݒ肷��
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// �r���[�|�[�g���Q�[����ʃT�C�Y�ɂ��킹��
			m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

			// ���C�v�X�v���C�g��`��
			if (!disp_proc_sprite(&wipe_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d2_screen->unlock();
		}
	}
	// 212�͊g�嗦�����Ȃ荂���Ȃ蒸�_���W��GPU���\�̌��E���z���ăN���b�v����邽�߁A���̃X�P�[���n���C�v�̂ݒ��ڒ��_��UV���w�肷��
	// 213,214�͂قƂ�ǂ̃}�V���ł͖��͂Ȃ����ǂ��Ȃ�̒�X�y�b�N�}�V�����ƃI�[�o�[���Ă��܂��̂ł�����ŏ���
	// 215�͒��ڎw��̕����y�Ȃ̂�
	else if(Gp_wipe->m_type == 212 || Gp_wipe->m_type == 213 || Gp_wipe->m_type == 214 || Gp_wipe->m_type == 215) {
		// ���C�v�X�v���C�g�̏�����
		C_d3d_sprite wipe_sprite;
		wipe_sprite.create_polygon_no_init();
		disp_proc_init_wipe_sprite(&wipe_sprite);
		wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
		wipe_sprite.set_index_buffer(G_rect_index_buffer);

		// �X�P�[�����C�v�p�̃X�v���C�g�̒ǉ��ݒ�
		D3DXVECTOR4 v_pos[4];
		D3DXVECTOR2 v_uv[4];
		disp_proc_wipe_for_scale_func(v_pos, v_uv, &wipe_sprite, wipe_prg);

		// ���C�v�X�v���C�g���X�V����
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, v_pos, v_uv))	{
			G_rect_vertex_buffer_d2_screen->unlock();

			// �����_�[�^�[�Q�b�g���^�[�Q�b�g�o�b�t�@�ɐݒ肷��
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// �r���[�|�[�g���Q�[����ʃT�C�Y�ɂ��킹��
			m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

			// ���C�v�X�v���C�g��`��
			if (!disp_proc_sprite(&wipe_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d2_screen->unlock();
		}
	}

	return true;
}

// ****************************************************************
// ���C�v�`��F�X�P�[���n�e�t�m�b
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_scale_func(C_d3d_sprite* sprite, int wipe_prg)
{
	if(Gp_wipe->m_type == 210 || Gp_wipe->m_type == 211) {
		float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;

		// �X�P�[������
		float scale_linear_st = 1.0f;
		float scale_linear_ed = 1.0f;
		// ���G���g��\��
		if(Gp_wipe->m_type == 210) {
			scale_linear_st = 0.0f;
			scale_linear_ed = 1.0f;
		}
		// ���G���k���\��
		else if(Gp_wipe->m_type == 211) {
			scale_linear_st = 1.0f;
			scale_linear_ed = 0.0f;
		}

		// �����x����
		BYTE tr_linear_st = 255;
		BYTE tr_linear_ed = 255;
		if(Gp_wipe->m_type == 210) {
			if(Gp_wipe->m_option[1] == 1) {
				// �t�F�[�h�C��
				tr_linear_st = 0;
				tr_linear_ed = 255;
			}
		}
		else if(Gp_wipe->m_type == 211) {
			if(Gp_wipe->m_option[1] == 1) {
				// �t�F�[�h�A�E�g
				tr_linear_st = 255;
				tr_linear_ed = 0;
			}
		}
		sprite->rp.tr = linear_limit<int, BYTE>(wipe_prg, 0, tr_linear_st, TNM_WIPE_PRG_MAX, tr_linear_ed);

		// ���ʏ���
		bool set_scale_x = false;
		bool set_scale_y = false;
		switch (Gp_wipe->m_option[0]) {
			case 0:		// ���S����
				sprite->rp.pos.x = sprite->rp.size.x * 0.5f;
				sprite->rp.pos.y = sprite->rp.size.y * 0.5f;
				sprite->rp.center.x = sprite->rp.size.x * 0.5f;
				sprite->rp.center.y = sprite->rp.size.y * 0.5f;
				set_scale_x = true;
				set_scale_y = true;
				break;
			case 1:		// ���ォ��
				//sprite->rp.pos.x = sprite->rp.size.x * 0.5f;
				//sprite->rp.pos.y = sprite->rp.size.y * 0.5f;
				//sprite->rp.center.x = sprite->rp.size.x * 0.5f;
				//sprite->rp.center.y = sprite->rp.size.y * 0.5f;
				set_scale_x = true;
				set_scale_y = true;
				break;
			case 2:		// �E�ォ��
				sprite->rp.pos.x = sprite->rp.size.x;
				//sprite->rp.pos.y = sprite->rp.size.y * 0.5f;
				sprite->rp.center.x = sprite->rp.size.x;
				//sprite->rp.center.y = sprite->rp.size.y * 0.5f;
				set_scale_x = true;
				set_scale_y = true;
				break;
			case 3:		// ��������
				//sprite->rp.pos.x = sprite->rp.size.x * 0.5f;
				sprite->rp.pos.y = sprite->rp.size.y;
				//sprite->rp.center.x = sprite->rp.size.x * 0.5f;
				sprite->rp.center.y = sprite->rp.size.y;
				set_scale_x = true;
				set_scale_y = true;
				break;
			case 4:		// �E������
				sprite->rp.pos.x = sprite->rp.size.x;
				sprite->rp.pos.y = sprite->rp.size.y;
				sprite->rp.center.x = sprite->rp.size.x;
				sprite->rp.center.y = sprite->rp.size.y;
				set_scale_x = true;
				set_scale_y = true;
				break;
			case 5:		// ���S����㉺
				sprite->rp.pos.x = sprite->rp.size.x * 0.5f;
				sprite->rp.pos.y = sprite->rp.size.y * 0.5f;
				sprite->rp.center.x = sprite->rp.size.x * 0.5f;
				sprite->rp.center.y = sprite->rp.size.y * 0.5f;
				//set_scale_x = true;
				set_scale_y = true;
				break;
			case 6:		// ���S���獶�E
				sprite->rp.pos.x = sprite->rp.size.x * 0.5f;
				sprite->rp.pos.y = sprite->rp.size.y * 0.5f;
				sprite->rp.center.x = sprite->rp.size.x * 0.5f;
				sprite->rp.center.y = sprite->rp.size.y * 0.5f;
				set_scale_x = true;
				//set_scale_y = true;
				break;
			case 7:		// �ォ��
				//sprite->rp.pos.x = sprite->rp.size.x * 0.5f;
				//sprite->rp.pos.y = sprite->rp.size.y * 0.5f;
				//sprite->rp.center.x = sprite->rp.size.x * 0.5f;
				//sprite->rp.center.y = sprite->rp.size.y * 0.5f;
				//set_scale_x = true;
				set_scale_y = true;
				break;
			case 8:		// ������
				//sprite->rp.pos.x = sprite->rp.size.x * 0.5f;
				sprite->rp.pos.y = sprite->rp.size.y;
				//sprite->rp.center.x = sprite->rp.size.x * 0.5f;
				sprite->rp.center.y = sprite->rp.size.y;
				//set_scale_x = true;
				set_scale_y = true;
				break;
			case 9:		// ������
				//sprite->rp.pos.x = sprite->rp.size.x * 0.5f;
				//sprite->rp.pos.y = sprite->rp.size.y * 0.5f;
				//sprite->rp.center.x = sprite->rp.size.x * 0.5f;
				//sprite->rp.center.y = sprite->rp.size.y * 0.5f;
				set_scale_x = true;
				//set_scale_y = true;
				break;
			case 10:	// �E����
				sprite->rp.pos.x = sprite->rp.size.x;
				//sprite->rp.pos.y = sprite->rp.size.y * 0.5f;
				sprite->rp.center.x = sprite->rp.size.x;
				//sprite->rp.center.y = sprite->rp.size.y * 0.5f;
				set_scale_x = true;
				//set_scale_y = true;
				break;
			case 11:
				// �l�␳
				if (Gp_wipe->m_option[2] < 0)					Gp_wipe->m_option[2] = 0;
				if (Gp_wipe->m_option[2] > sprite->rp.size.x)	Gp_wipe->m_option[2] = (int)sprite->rp.size.x;
				if (Gp_wipe->m_option[3] < 0)					Gp_wipe->m_option[3] = 0;
				if (Gp_wipe->m_option[3] > sprite->rp.size.y)	Gp_wipe->m_option[3] = (int)sprite->rp.size.y;

				float x_rate = (float)Gp_wipe->m_option[2] / sprite->rp.size.x;
				float y_rate = (float)Gp_wipe->m_option[3] / sprite->rp.size.y;
				
				sprite->rp.pos.x = sprite->rp.size.x * x_rate;
				sprite->rp.pos.y = sprite->rp.size.y * y_rate;
				sprite->rp.center.x = sprite->rp.size.x * x_rate;
				sprite->rp.center.y = sprite->rp.size.y * y_rate;
				set_scale_x = true;
				set_scale_y = true;
				break;
		}
		if(set_scale_x) {
			sprite->rp.scale.x = linear_limit<int, float>(wipe_prg, 0, scale_linear_st, TNM_WIPE_PRG_MAX, scale_linear_ed);
		}
		if(set_scale_y) {
			sprite->rp.scale.y = linear_limit<int, float>(wipe_prg, 0, scale_linear_st, TNM_WIPE_PRG_MAX, scale_linear_ed);
		}
	}
	
	// �e�N�j�b�N��ݒ肵�Ȃ��ꍇ�̓f�t�H���g�ɂȂ�ׁA�ݒ肵�Ȃ�
	//p_sprite->set_effect_technique(m_view.m_default_effect.m_sprite_technique);
}

// ****************************************************************
// ���C�v�`��F�X�P�[���n�e�t�m�b
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_scale_func(D3DXVECTOR4 pos[], D3DXVECTOR2 uv[], C_d3d_sprite* sprite, int wipe_prg)
{
	if(Gp_wipe->m_type == 212 || Gp_wipe->m_type == 213 || Gp_wipe->m_type == 214) {
		// 0.0 ~ 1.0 �ɕ␳����
		float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;

		// ���_���W��ݒ肷��
		pos[0] = D3DXVECTOR4(0.0f,				0.0f,				0.5f,	1.0f);
		pos[1] = D3DXVECTOR4(sprite->rp.size.x,	0.0f,				0.5f,	1.0f);
		pos[2] = D3DXVECTOR4(0.0f,				sprite->rp.size.y,	0.5f,	1.0f);
		pos[3] = D3DXVECTOR4(sprite->rp.size.x, sprite->rp.size.y,	0.5f,	1.0f);;

		// �X�P�[������
		float scale_linear_st = 1.0f;
		float scale_linear_ed = 1.0f;
		float u_rate;
		float v_rate;
		// ���G���g���A���G�ɕύX���ďk���\��
		if(Gp_wipe->m_type == 212) {
			if(wipe_prg_float < 0.5f) {
				scale_linear_st = 1.0f;
				scale_linear_ed = 0.001f;
				u_rate = linear_limit<float, float>((float)wipe_prg, 0.0f, scale_linear_st, (float)TNM_WIPE_PRG_MAX / 2.0f, scale_linear_ed);
				v_rate = u_rate;
			}
			else {
				scale_linear_st = 0.001f;
				scale_linear_ed = 1.0f;
				u_rate = linear_limit<float, float>((float)wipe_prg, (float)TNM_WIPE_PRG_MAX / 2.0f, scale_linear_st, (float)TNM_WIPE_PRG_MAX, scale_linear_ed);
				v_rate = u_rate;
			}
		}
		// ���G���g���Ԃ���t�F�[�h�h�m���Ȃ���k���\��
		else if(Gp_wipe->m_type == 213) {
			scale_linear_st = 0.333f;
			scale_linear_ed = 1.0f;
			u_rate = linear_limit<float, float>((float)wipe_prg, 0.0f, scale_linear_st, (float)TNM_WIPE_PRG_MAX, scale_linear_ed);
			v_rate = u_rate;
			// �����x����
			sprite->rp.tr = linear_limit<int, BYTE>(wipe_prg, 0, 0, TNM_WIPE_PRG_MAX, 255);
		}
		// ���G���t�F�[�h�n�t�s���Ȃ���g��\��
		else if(Gp_wipe->m_type == 214) {
			scale_linear_st = 1.0f;
			scale_linear_ed = 0.333f;
			u_rate = linear_limit<float, float>((float)wipe_prg, 0.0f, scale_linear_st, (float)TNM_WIPE_PRG_MAX, scale_linear_ed);
			v_rate = u_rate;
			// �����x����
			sprite->rp.tr = linear_limit<int, BYTE>(wipe_prg, 0, 255, TNM_WIPE_PRG_MAX, 0);
		}

		float su = 0.0f;
		float sv = 0.0f;
		float eu = 1.0f;
		float ev = 1.0f;

		// ���ʏ���
		bool set_scale_x = false;
		bool set_scale_y = false;
		switch (Gp_wipe->m_option[0]) {
			case 0:		// ���S����
				su = 0.5f - 0.5f * u_rate;
				eu = 0.5f + 0.5f * u_rate;
				sv = 0.5f - 0.5f * v_rate;
				ev = 0.5f + 0.5f * v_rate;
				break;
			case 1:		// ���ォ��
				su = 0.0f;
				eu = 1.0f * u_rate;
				sv = 0.0f;
				ev = 1.0f * v_rate;
				break;
			case 2:		// �E�ォ��
				su = 1.0f - 1.0f * u_rate;
				eu = 1.0f;
				sv = 0.0f;
				ev = 1.0f * v_rate;
				break;
			case 3:		// ��������
				su = 0.0f;
				eu = 1.0f * u_rate;
				sv = 1.0f - 1.0f * v_rate;
				ev = 1.0f;
				break;
			case 4:		// �E������
				su = 1.0f - 1.0f * u_rate;
				eu = 1.0f;
				sv = 1.0f - 1.0f * v_rate;
				ev = 1.0f;
				break;
			case 5:		// ���S����㉺
				v_rate = linear_limit<float, float>(v_rate, 0.0f, 0.49f, 1.0f, 1.0f);	// 0.5�ȏ�Ŕ��]����̂�
				su = 0.0f;
				eu = 1.0f;
				sv = 1.0f - 1.0f * v_rate;
				ev = 1.0f * v_rate;
				break;
			case 6:		// ���S���獶�E
				u_rate = linear_limit<float, float>(u_rate, 0.0f, 0.49f, 1.0f, 1.0f);	// 0.5�ȏ�Ŕ��]����̂�
				su = 1.0f - 1.0f * u_rate;
				eu = 1.0f * u_rate;
				sv = 0.0f;
				ev = 1.0f;
				break;
			case 7:		// �ォ��
				su = 0.0f;
				eu = 1.0f;
				sv = 0.0f;
				ev = 1.0f * v_rate;
				break;
			case 8:		// ������
				su = 0.0f;
				eu = 1.0f;
				sv = 1.0f - 1.0f * v_rate;
				ev = 1.0f;
				break;
			case 9:		// ������
				su = 0.0f;
				eu = 1.0f * u_rate;
				sv = 0.0f;
				ev = 1.0f;
				break;
			case 10:	// �E����
				su = 1.0f - 1.0f * u_rate;
				eu = 1.0f;
				sv = 0.0f;
				ev = 1.0f;
				break;
			case 11:
				// �l�␳
				if (Gp_wipe->m_option[2] < 0)					Gp_wipe->m_option[2] = 0;
				if (Gp_wipe->m_option[2] > sprite->rp.size.x)	Gp_wipe->m_option[2] = (int)sprite->rp.size.x;
				if (Gp_wipe->m_option[3] < 0)					Gp_wipe->m_option[3] = 0;
				if (Gp_wipe->m_option[3] > sprite->rp.size.y)	Gp_wipe->m_option[3] = (int)sprite->rp.size.y;

				float x_rate = (float)Gp_wipe->m_option[2] / sprite->rp.size.x;
				float y_rate = (float)Gp_wipe->m_option[3] / sprite->rp.size.y;
				
				su = x_rate - (1.0f-(1.0f - x_rate)) * u_rate;
				eu = x_rate + (1.0f - x_rate) * u_rate;
				sv = y_rate - (1.0f-(1.0f - y_rate)) * v_rate;
				ev = y_rate + (1.0f - y_rate) * v_rate;
				break;
		}
		uv[0] = D3DXVECTOR2(su, sv);
		uv[1] = D3DXVECTOR2(eu, sv);
		uv[2] = D3DXVECTOR2(su, ev);
		uv[3] = D3DXVECTOR2(eu, ev);
	}
	// ���C�v�J�n(�I��)���̋�`�͈͂��w�肵�āA��(��)�G���g��(�k��)�\��
	else if(Gp_wipe->m_type == 215) {
		// 0.0 ~ 1.0 �ɕ␳����
		float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;

		// UV���W��ݒ肷��
		uv[0] = D3DXVECTOR2(0.0f, 0.0f);
		uv[1] = D3DXVECTOR2(1.0f, 0.0f);
		uv[2] = D3DXVECTOR2(0.0f, 1.0f);
		uv[3] = D3DXVECTOR2(1.0f, 1.0f);

		// �X�P�[������
		float scale_linear_st = 1.0f;
		float scale_linear_ed = 1.0f;
		// �g��
		if(Gp_wipe->m_option[1] == 0) {
			scale_linear_st = 0.0f;
			scale_linear_ed = 1.0f;
		}
		// �k��
		else {
			scale_linear_st = 1.0f;
			scale_linear_ed = 0.0f;
		}

		// �����x����
		BYTE tr_linear_st = 255;
		BYTE tr_linear_ed = 255;
		// �����x�����Ȃ�
		if(Gp_wipe->m_option[0] == 0) {
			tr_linear_st = 255;
			tr_linear_ed = 255;
		}
		// �t�F�[�h�C��
		else if(Gp_wipe->m_option[0] == 1) {
			tr_linear_st = 0;
			tr_linear_ed = 255;
		}
		// �t�F�[�h�A�E�g
		else if(Gp_wipe->m_option[0] == 2) {
			tr_linear_st = 255;
			tr_linear_ed = 0;
		}
		sprite->rp.tr = linear_limit<int, BYTE>(wipe_prg, 0, tr_linear_st, TNM_WIPE_PRG_MAX, tr_linear_ed);



		int sx = Gp_wipe->m_option[2];
		int sy = Gp_wipe->m_option[3];
		int ex = Gp_wipe->m_option[4];
		int ey = Gp_wipe->m_option[5];
		// �܂��͔͈͕␳
		if (sx < 0)						sx = 0;
		if (sx > sprite->rp.size.x)		sx = (int)sprite->rp.size.x;
		if (sy < 0)						sy = 0;
		if (sy > sprite->rp.size.y)		sy = (int)sprite->rp.size.y;
		if (ex < 0)						ex = 0;
		if (ex > sprite->rp.size.x)		ex = (int)sprite->rp.size.x;
		if (ey < 0)						ey = 0;
		if (ey > sprite->rp.size.y)		ey = (int)sprite->rp.size.y;
		// ���]�E���l�␳
		if (sx >= ex) {
			// ���l�̏ꍇ�͍Œ�ł�1���m��
			if (sx == ex) {
				// end��+1�Ŕ͈͊O�ɂȂ�ꍇ��start��-1
				if (sx + 1 >= sprite->rp.size.x)
					sx = ex - 1;
				else
					ex = sx + 1;
			}
			// sx > ex �̏ꍇ�� sx < ex �ɂȂ�悤��XOR�X���b�v(float�͖����Ȃ̂Œ���)
			else {
				sx ^= ex;
				ex ^= sx;
				sx ^= ex;
			}
		}
		// ���]�E���l�␳
		if (sy >= ey) {
			// ���l�̏ꍇ�͍Œ�ł�1���m��
			if (sy == ey) {
				// end��+1�Ŕ͈͊O�ɂȂ�ꍇ��start��-1
				if (sy + 1 >= sprite->rp.size.y)
					sy = ey - 1;
				else
					ey = sy + 1;
			}
			// sy > ey �̏ꍇ�� sy < ey �ɂȂ�悤��XOR�X���b�v(float�͖����Ȃ̂Œ���)
			else {
				sy ^= ey;
				ey ^= sy;
				sy ^= ey;
			}
		}

		int fsx = 0;
		int fsy = 0;
		int fex = (int)sprite->rp.size.x;
		int fey = (int)sprite->rp.size.y;

		// �g��
		if (Gp_wipe->m_option[1] == 0) {
			fsx = linear_limit<int, int>(wipe_prg, 0, sx, TNM_WIPE_PRG_MAX, 0);
			fsy = linear_limit<int, int>(wipe_prg, 0, sy, TNM_WIPE_PRG_MAX, 0);
			fex = linear_limit<int, int>(wipe_prg, 0, ex, TNM_WIPE_PRG_MAX, (int)sprite->rp.size.x);
			fey = linear_limit<int, int>(wipe_prg, 0, ey, TNM_WIPE_PRG_MAX, (int)sprite->rp.size.y);
		}
		else if (Gp_wipe->m_option[1] == 1) {
			fsx = linear_limit<int, int>(wipe_prg, 0, 0, TNM_WIPE_PRG_MAX, sx);
			fsy = linear_limit<int, int>(wipe_prg, 0, 0, TNM_WIPE_PRG_MAX, sy);
			fex = linear_limit<int, int>(wipe_prg, 0, (int)sprite->rp.size.x, TNM_WIPE_PRG_MAX, ex);
			fey = linear_limit<int, int>(wipe_prg, 0, (int)sprite->rp.size.y, TNM_WIPE_PRG_MAX, ey);
		}

		// ���_���W��ݒ肷��
		pos[0] = D3DXVECTOR4((float)fsx,	(float)fsy,	0.5f,	1.0f);
		pos[1] = D3DXVECTOR4((float)fex,	(float)fsy,	0.5f,	1.0f);
		pos[2] = D3DXVECTOR4((float)fsx,	(float)fey,	0.5f,	1.0f);
		pos[3] = D3DXVECTOR4((float)fex,	(float)fey,	0.5f,	1.0f);
	}
}

// ****************************************************************
// ���C�v�`��F�����u���[
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_explosion_blur(int wipe_prg)
{
	float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;

	if (Gp_wipe->m_type == 240 || Gp_wipe->m_type == 242) {
		// --- ���� ---

		// ���C�v��艺�̃I�[�_�[��`�揀������
		if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
			return false;

		// ���C�v����I�[�_�[�̃l�N�X�g��`�揀������
		if (!disp_proc_wipe_order_ready(TNM_STAGE_NEXT))
			return false;

		// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�揀������
		if (!disp_proc_wipe_order_ready(TNM_STAGE_FRONT))
			return false;

		// ���C�v����̃I�[�_�[��`�揀������
		if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
			return false;

		// --- �`�� ---
		
		// ���C�v�o�b�t�@�P����������
		if (!disp_proc_ready_wipe_buffer_1())
			return false;

		// ���C�v�o�b�t�@��h��Ԃ�
		m_view.clear_render_target(m_wipe_back_color);

		// ���C�v��艺�̃I�[�_�[��`�悷��
		if (!disp_proc_under_order(TNM_STAGE_FRONT))
			return false;

		// ���C�v����I�[�_�[�̃l�N�X�g��`�悷��
		if (!disp_proc_wipe_order(TNM_STAGE_NEXT))
			return false;

		// ���C�v�o�b�t�@�O����������
		if (!disp_proc_ready_wipe_buffer_0())
			return false;

		// ���C�v�o�b�t�@��h��Ԃ�
		m_view.clear_render_target(m_wipe_back_color);

		// ���C�v��艺�̃I�[�_�[��`�悷��
		if (!disp_proc_under_order(TNM_STAGE_FRONT))
			return false;

		// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�悷��
		if (!disp_proc_wipe_order(TNM_STAGE_FRONT))
			return false;

		// �����܂ł�RT�̓��C�v�o�b�t�@�ɐݒ肳��A���C�v�o�b�t�@���̂ւ̕`��͊���
		// ��������RT��ύX���A���C�v�o�b�t�@���X�v���C�g�Ƃ��ĕ`�悷��

		// ���C�v�o�b�t�@��`�悷��
		{
			// ���C�v�X�v���C�g�̏�����
			C_d3d_sprite wipe_sprite;
			wipe_sprite.create_polygon_no_init();
			disp_proc_init_wipe_sprite(&wipe_sprite);
			wipe_sprite.set_texture(m_wipe_back_buffer_texture_1, TNM_TEXTURE_STAGE_MASK);		// ���C�v�o�b�t�@�P���ݒ肷��
			wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
			wipe_sprite.set_index_buffer(G_rect_index_buffer);

			// �N���X�����u���[���C�v�p�̃X�v���C�g�̒ǉ��ݒ�
			disp_proc_wipe_for_explosion_blur_func(&wipe_sprite, wipe_prg);

			// ���C�v�X�v���C�g���X�V����
			void* p_locked_vertex_buffer;
			G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
			if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
				G_rect_vertex_buffer_d2_screen->unlock();

				// �����_�[�^�[�Q�b�g���^�[�Q�b�g�o�b�t�@�ɐݒ肷��
				m_view.set_render_target(m_target_back_buffer.body());
				m_view.set_dpst_buffer(m_target_dpst_buffer.body());

				// �r���[�|�[�g���Q�[����ʃT�C�Y�ɂ��킹��
				m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
				//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

				// ���C�v�X�v���C�g��`��
				if (!disp_proc_sprite(&wipe_sprite))
					return false;
			}
			else	{
				G_rect_vertex_buffer_d2_screen->unlock();
			}
		}

		// ���C�v����̃I�[�_�[��`�悷��
		if (!disp_proc_over_order(TNM_STAGE_FRONT))
			return false;

		return true;
	}
	else if(Gp_wipe->m_type == 241 || Gp_wipe->m_type == 243) {
		// �X�e�[�W�擾
		bool is_front = disp_proc_wipe_for_explosion_blur_get_stage(wipe_prg);

		// --- ���� ---

		// ���C�v��艺�̃I�[�_�[��`�揀������
		if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
			return false;

		// ���C�v����I�[�_�[�̃l�N�X�g�����C�v�o�b�t�@�ɕ`�揀������
		if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
			return false;

		// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�揀������
		if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
			return false;

		// ���C�v����̃I�[�_�[��`�揀������
		if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
			return false;

		// --- �`�� ---

		// ���C�v��艺�̃I�[�_�[��`�悷��
		if (!disp_proc_under_order(TNM_STAGE_FRONT))
			return false;

		// ���C�v����I�[�_�[�̃l�N�X�g��`�悷��
		if (!disp_proc_wipe_order(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
			return false;

		// ���C�v�o�b�t�@����������
		if (!disp_proc_ready_wipe_buffer_0())
			return false;
	
		// ���C�v�o�b�t�@��h��Ԃ�
		m_view.clear_render_target(m_wipe_back_color);

		// ���C�v��艺�̃I�[�_�[��`�悷��
		if (!disp_proc_under_order(TNM_STAGE_FRONT))
			return false;

		// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�悷��
		if (!disp_proc_wipe_order(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
			return false;

		// �����܂ł�RT�̓��C�v�o�b�t�@�ɐݒ肳��A���C�v�o�b�t�@���̂ւ̕`��͊���
		// ��������RT��ύX���A���C�v�o�b�t�@���X�v���C�g�Ƃ��ĕ`�悷��

		// ���C�v�o�b�t�@��`�悷��
		{
			// ���C�v�X�v���C�g�̏�����
			C_d3d_sprite wipe_sprite;
			wipe_sprite.create_polygon_no_init();
			disp_proc_init_wipe_sprite(&wipe_sprite);
			wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
			wipe_sprite.set_index_buffer(G_rect_index_buffer);

			// �����u���[���C�v�p�̃X�v���C�g�̒ǉ��ݒ�
			disp_proc_wipe_for_explosion_blur_func(&wipe_sprite, wipe_prg);

			// ���C�v�X�v���C�g���X�V����
			void* p_locked_vertex_buffer;
			G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
			if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
				G_rect_vertex_buffer_d2_screen->unlock();

				// �����_�[�^�[�Q�b�g���^�[�Q�b�g�o�b�t�@�ɐݒ肷��
				m_view.set_render_target(m_target_back_buffer.body());
				m_view.set_dpst_buffer(m_target_dpst_buffer.body());

				// �r���[�|�[�g���Q�[����ʃT�C�Y�ɂ��킹��
				m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
				//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

				// ���C�v�X�v���C�g��`��
				if (!disp_proc_sprite(&wipe_sprite))
					return false;
			}
			else	{
				G_rect_vertex_buffer_d2_screen->unlock();
			}
		}

		// ���C�v����̃I�[�_�[��`�悷��
		if (!disp_proc_over_order(TNM_STAGE_FRONT))
			return false;
	}

	return true;
}
// ****************************************************************
// ���C�v�`��F�����u���[�X�e�[�W�擾
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_explosion_blur_get_stage(int wipe_prg)
{
	float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;
	// ���G = true
	// ���G = false
	switch (Gp_wipe->m_type)	{
		case 240:
			return false;
			break;
		case 241:
			// ���G
			if(Gp_wipe->m_option[7] == 0)
				return true;
			else
				return false;
			break;
		case 242:
			return false;
			break;
		case 243:
			// ���G
			if(Gp_wipe->m_option[5] == 0)
				return true;
			else
				return false;
			break;
	}
	return false;
}
// ****************************************************************
// ���C�v�`��F�����u���[�e�t�m�b
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_explosion_blur_func(C_d3d_sprite* sprite, int wipe_prg)
{
	float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;

	// �����x����
	int alpha_type    = (Gp_wipe->m_type == 240 || Gp_wipe->m_type == 241) ? Gp_wipe->m_option[2] : Gp_wipe->m_option[0];
	int alpha_reverse = (Gp_wipe->m_type == 240 || Gp_wipe->m_type == 241) ? Gp_wipe->m_option[3] : Gp_wipe->m_option[1];
	BYTE tr = 255;
	if(alpha_type == 0 || alpha_type == 10) {
		// �E��������
		if(alpha_type == 0)
			tr = (BYTE)(255.0f * linear_limit<float, float>(wipe_prg_float, 0.0f, 1.0f, 1.0f, 0.0f));
		// �E���オ��
		else
			tr = (BYTE)(255.0f * linear_limit<float, float>(wipe_prg_float, 0.0f, 0.0f, 1.0f, 1.0f));

		// �A���t�@���]����
		if(alpha_reverse)
			sprite->rp.tr = 255 - tr;
		// �A���t�@���]�Ȃ�
		else
			sprite->rp.tr = tr;
	}
	else if(alpha_type > 0 || alpha_type < 10) {
		float threshold = (float)alpha_type / 10.0f;	// 臒l
		// 臒l�܂ł͉E���オ��
		if(wipe_prg_float < threshold)
			tr = (BYTE)(255.0f * linear_limit<float, float>(wipe_prg_float, 0.0f, 0.0f, threshold, 1.0f));
		// 臒l�ȍ~�͉E��������
		else
			tr = (BYTE)(255.0f * linear_limit<float, float>(wipe_prg_float, threshold, 1.0f, 1.0f, 0.0f));
		
		// �A���t�@���]����
		if(alpha_reverse)
			sprite->rp.tr = 255 - tr;
		// �A���t�@���]�Ȃ�
		else
			sprite->rp.tr = tr;
	}
	else {
		// �^�C�v�͈͊O�̓A���t�@���]�������Ƃ��đS��255�Ƃ���
		tr = 255;
	}
	
	// �u���[�p���[����
	int bp_type    = (Gp_wipe->m_type == 240 || Gp_wipe->m_type == 241) ? Gp_wipe->m_option[4] : Gp_wipe->m_option[2];
	int bp_reverse = (Gp_wipe->m_type == 240 || Gp_wipe->m_type == 241) ? Gp_wipe->m_option[5] : Gp_wipe->m_option[3];
	float bp = 0.0f;
	if(bp_type == 0 || bp_type == 10) {
		// �E��������
		if(bp_type == 0)
			bp = linear_limit<float, float>(wipe_prg_float, 0.0f, 1.0f, 1.0f, 0.0f);
		// �E���オ��
		else
			bp = linear_limit<float, float>(wipe_prg_float, 0.0f, 0.0f, 1.0f, 1.0f);

		// ���]����
		if(bp_reverse)
			bp = 1.0f - bp;
		// ���]�Ȃ�
		else
			bp = bp;
	}
	else if(bp_type > 0 || bp_type < 10) {
		float threshold = (float)bp_type / 10.0f;	// 臒l
		// 臒l�܂ł͉E���オ��
		if(wipe_prg_float < threshold)
			bp = linear_limit<float, float>(wipe_prg_float, 0.0f, 0.0f, threshold, 1.0f);
		// 臒l�ȍ~�͉E��������
		else
			bp = linear_limit<float, float>(wipe_prg_float, threshold, 1.0f, 1.0f, 0.0f);
		
		// ���]����
		if(bp_reverse)
			bp = 1.0f - bp;
		// ���]�Ȃ�
		else
			bp = bp;
	}
	else {
		// �^�C�v�͈͊O�͔��]�������Ƃ��đS��0.0f�Ƃ���
		bp = 0.0f;
	}

	
	if(!sprite->get_texture(0))
		return;
	// �萔�ݒ�
	float texel_size_u = (float)1.0f / sprite->get_texture(0)->get_width_ex();
	float texel_size_v = (float)1.0f / sprite->get_texture(0)->get_height_ex();
	//float texel_size_u = (float)1.0f / sprite->rp.size.cx;
	//float texel_size_v = (float)1.0f / sprite->rp.size.cy;
	//float texel_center_u = (float)Gp_wipe->m_option[0] / sprite->get_texture(0)->get_width_ex();
	//float texel_center_v = (float)Gp_wipe->m_option[1] / sprite->get_texture(0)->get_height_ex();
	float texel_center_u = (float)Gp_wipe->m_option[0] / sprite->get_texture(0)->get_width_ex();
	float texel_center_v = (float)Gp_wipe->m_option[1] / sprite->get_texture(0)->get_height_ex();
	if (Gp_wipe->m_type == 242 || Gp_wipe->m_type == 243) {
		// �����_���Œ��S��ݒ�
		texel_center_u = (float)(rand() % (int)sprite->rp.size.x) / sprite->get_texture(0)->get_width_ex();
		texel_center_v = (float)(rand() % (int)sprite->rp.size.y) / sprite->get_texture(0)->get_height_ex();
	}
	float blur_coefficient = (Gp_wipe->m_type == 240 || Gp_wipe->m_type == 241) ? (float)Gp_wipe->m_option[6] : (float)Gp_wipe->m_option[4];
	sprite->set_effect_constant_f(0, texel_size_u, texel_size_v, texel_center_u, texel_center_v);
	sprite->set_effect_constant_f(1, bp, blur_coefficient, 0, wipe_prg_float);

	// �e�N�j�b�N�ݒ�
	if(Gp_wipe->m_type == 240 || Gp_wipe->m_type == 242)	{
		sprite->set_effect_technique(m_view.m_default_effect.m_cross_explosion_blur_technique);
	}
	else if(Gp_wipe->m_type == 241 || Gp_wipe->m_type == 243)	{
		sprite->set_effect_technique(m_view.m_default_effect.m_explosion_blur_technique);
	}
}

// ****************************************************************
// ���C�v�`��F���U�C�N
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_mosaic(int wipe_prg)
{
	float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;

	if(Gp_wipe->m_type == 230 || Gp_wipe->m_type == 231) {
		// �X�e�[�W�擾
		bool is_front = disp_proc_wipe_for_mosaic_get_stage(wipe_prg);

		// --- ���� ---

		// ���C�v��艺�̃I�[�_�[��`�揀������
		if (!disp_proc_under_order_ready(TNM_STAGE_FRONT))
			return false;

		// ���C�v����I�[�_�[�̃l�N�X�g�����C�v�o�b�t�@�ɕ`�揀������
		if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
			return false;

		// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�揀������
		if (!disp_proc_wipe_order_ready(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
			return false;

		// ���C�v����̃I�[�_�[��`�揀������
		if (!disp_proc_over_order_ready(TNM_STAGE_FRONT))
			return false;

		// --- �`�� ---

		// ���C�v��艺�̃I�[�_�[��`�悷��
		if (!disp_proc_under_order(TNM_STAGE_FRONT))
			return false;

		// ���C�v����I�[�_�[�̃l�N�X�g��`�悷��
		if (!disp_proc_wipe_order(is_front ? TNM_STAGE_FRONT : TNM_STAGE_NEXT))
			return false;

		// ���C�v�o�b�t�@����������
		if (!disp_proc_ready_wipe_buffer_0())
			return false;

		// ���C�v�o�b�t�@��h��Ԃ�
		m_view.clear_render_target(m_wipe_back_color);

		// ���C�v��艺�̃I�[�_�[��`�悷��
		if (!disp_proc_under_order(TNM_STAGE_FRONT))
			return false;

		// ���C�v����I�[�_�[�̃t�����g�����C�v�o�b�t�@�ɕ`�悷��
		if (!disp_proc_wipe_order(is_front ? TNM_STAGE_NEXT : TNM_STAGE_FRONT))
			return false;

		// �����܂ł�RT�̓��C�v�o�b�t�@�ɐݒ肳��A���C�v�o�b�t�@���̂ւ̕`��͊���
		// ��������RT��ύX���A���C�v�o�b�t�@���X�v���C�g�Ƃ��ĕ`�悷��

		// ���C�v�o�b�t�@��`�悷��
		{
			// ���C�v�X�v���C�g�̏�����
			C_d3d_sprite wipe_sprite;
			wipe_sprite.create_polygon_no_init();
			disp_proc_init_wipe_sprite(&wipe_sprite);
			wipe_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
			wipe_sprite.set_index_buffer(G_rect_index_buffer);

			// ���U�C�N���C�v�p�̃X�v���C�g�̒ǉ��ݒ�
			disp_proc_wipe_for_mosaic_func(&wipe_sprite, wipe_prg);

			// ���C�v�X�v���C�g���X�V����
			void* p_locked_vertex_buffer;
			G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
			if (wipe_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
				G_rect_vertex_buffer_d2_screen->unlock();

				// �����_�[�^�[�Q�b�g���^�[�Q�b�g�o�b�t�@�ɐݒ肷��
				m_view.set_render_target(m_target_back_buffer.body());
				m_view.set_dpst_buffer(m_target_dpst_buffer.body());

				// �r���[�|�[�g���Q�[����ʃT�C�Y�ɂ��킹��
				m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
				//G_d3d.device.set_view_port(C_point(0, 0), Gp_global->game_screen_size);

				// ���C�v�X�v���C�g��`��
				if (!disp_proc_sprite(&wipe_sprite))
					return false;
			}
			else	{
				G_rect_vertex_buffer_d2_screen->unlock();
			}
		}

		// ���C�v����̃I�[�_�[��`�悷��
		if (!disp_proc_over_order(TNM_STAGE_FRONT))
			return false;
	}

	return true;
}
// ****************************************************************
// ���C�v�`��F���U�C�N�X�e�[�W�擾
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_for_mosaic_get_stage(int wipe_prg)
{
	float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;
	// ���G = true
	// ���G = false
	switch (Gp_wipe->m_type)	{
		case 230:
			if(wipe_prg_float < 0.5f)
				return true;
			else
				return false;
			break;
		case 231:
			// ���G
			if(Gp_wipe->m_option[1] == 0)
				return true;
			else
				return false;
			break;
	}
	return false;
}
// ****************************************************************
// ���C�v�`��F���U�C�N�e�t�m�b
// ================================================================
void C_tnm_wnd::disp_proc_wipe_for_mosaic_func(C_d3d_sprite* sprite, int wipe_prg)
{
	float wipe_prg_float = (float)wipe_prg / TNM_WIPE_PRG_MAX;

	// �����x����
	BYTE tr = 255;
	if(Gp_wipe->m_type == 230) {
		// �s��Ȃ�
		sprite->rp.tr = tr;
	}
	else if(Gp_wipe->m_type == 231) {
		// ���G�t�F�[�h�A�E�g
		if(Gp_wipe->m_option[1] == 0)
			tr = (BYTE)(255.0f * linear_limit<float, float>(wipe_prg_float, 0.0f, 1.0f, 1.0f, 0.0f));
		// ���G�t�F�[�h�C��
		else
			tr = (BYTE)(255.0f * linear_limit<float, float>(wipe_prg_float, 0.0f, 0.0f, 1.0f, 1.0f));

		sprite->rp.tr = tr;
	}

	// ���U�C�N����
	float st_texel_size = 1.0f;
	float ed_texel_size = 1.0f;
	float cut_per_texel_size = 0.1f;
	if(Gp_wipe->m_type == 230) {
		// st_texel_size - ed_texel_size - st_texel_size �ɂȂ�
		switch(Gp_wipe->m_option[0]) {
			case 0:
				st_texel_size = 0.001f;
				ed_texel_size = 0.025f;
				break;
			case 1:
				st_texel_size = 0.002f;
				ed_texel_size = 0.04f;
				break;
			case 2:
				st_texel_size = 0.003f;
				ed_texel_size = 0.06f;
				break;
			case 3:
				st_texel_size = 0.004f;
				ed_texel_size = 0.08f;
				break;
			case 4:
				st_texel_size = 0.005f;
				ed_texel_size = 0.1f;
				break;
				// ���� 0.005
				// ���� 0.025 - 0.25 - 0.5
			case 5:
				st_texel_size = 0.006f;
				ed_texel_size = 0.15f;
				break;
			case 6:
				st_texel_size = 0.007f;
				ed_texel_size = 0.2f;
				break;
			case 7:
				st_texel_size = 0.008f;
				ed_texel_size = 0.3f;
				break;
			case 8:
				st_texel_size = 0.009f;
				ed_texel_size = 0.4f;
				break;
			case 9:
				st_texel_size = 0.01f;
				ed_texel_size = 0.5f;
				break;
		}

		// ���Ԍo�߂𗣎U�I�ɍs��
		//float discrete_wipe_prg_float = (float)((int)(wipe_prg_float * 25)) / 25.0f;
		//if(discrete_wipe_prg_float < 0.5f)
		//	cut_per_texel_size = linear_limit<float, float>(discrete_wipe_prg_float, 0.0f, st_texel_size, 0.5f, ed_texel_size);
		//else
		//	cut_per_texel_size = linear_limit<float, float>(discrete_wipe_prg_float, 0.5f, ed_texel_size, 1.0f, st_texel_size);
		float discrete_wipe_prg_float = (float)((int)(wipe_prg_float * 50)) / 50.0f;
		int discrete_wipe_prg = (int)(discrete_wipe_prg_float * TNM_WIPE_PRG_MAX);
		if(discrete_wipe_prg < TNM_WIPE_PRG_MAX / 2)
			cut_per_texel_size = speed_up_limit(discrete_wipe_prg, 0, (int)(st_texel_size * 10000), TNM_WIPE_PRG_MAX / 2, (int)(ed_texel_size * 10000)) / 10000.0f;
		else {
			// �Ō�Ԃ؂�ɂȂ�̂�h��
			if(discrete_wipe_prg_float < 0.95f)
				cut_per_texel_size = speed_down_limit(discrete_wipe_prg, TNM_WIPE_PRG_MAX / 2, (int)(ed_texel_size * 10000), TNM_WIPE_PRG_MAX, (int)(st_texel_size * 10000)) / 10000.0f;
			else
				cut_per_texel_size = speed_down_limit(wipe_prg, TNM_WIPE_PRG_MAX / 2, (int)(ed_texel_size * 10000), TNM_WIPE_PRG_MAX, (int)(st_texel_size * 10000)) / 10000.0f;
		}
	}
	else if(Gp_wipe->m_type == 231) {
		//// ���G�t�F�[�h�A�E�g
		//if(Gp_wipe->m_option[1] == 0) {
		//	st_texel_size = 0.01f;
		//	ed_texel_size = 0.1f;
		//}
		//// ���G�t�F�[�h�C��
		//else {
		//	st_texel_size = 0.1f;
		//	ed_texel_size = 0.01f;
		//}

		// ���G�t�F�[�h�A�E�g����ɐݒ肷��
		switch(Gp_wipe->m_option[0]) {
			case 0:
				st_texel_size = 0.001f;
				ed_texel_size = 0.025f;
				break;
			case 1:
				st_texel_size = 0.002f;
				ed_texel_size = 0.04f;
				break;
			case 2:
				st_texel_size = 0.003f;
				ed_texel_size = 0.06f;
				break;
			case 3:
				st_texel_size = 0.004f;
				ed_texel_size = 0.08f;
				break;
			case 4:
				st_texel_size = 0.005f;
				ed_texel_size = 0.1f;
				break;
				// ���� 0.005
				// ���� 0.025 - 0.25 - 0.5
			case 5:
				st_texel_size = 0.006f;
				ed_texel_size = 0.15f;
				break;
			case 6:
				st_texel_size = 0.007f;
				ed_texel_size = 0.2f;
				break;
			case 7:
				st_texel_size = 0.008f;
				ed_texel_size = 0.3f;
				break;
			case 8:
				st_texel_size = 0.009f;
				ed_texel_size = 0.4f;
				break;
			case 9:
				st_texel_size = 0.01f;
				ed_texel_size = 0.5f;
				break;
		}

		// ���G�t�F�[�h�C���̏ꍇ��st��ed�����ւ���
		if(Gp_wipe->m_option[1] == 1) {
			float tmp = st_texel_size;
			st_texel_size = ed_texel_size;
			ed_texel_size = tmp;
		}

		// ���Ԍo�ߏ���
		if(Gp_wipe->m_option[1] == 0) {
			// ���Ԍo�߂𗣎U�I�ɍs��
			float discrete_wipe_prg_float = (float)((int)(wipe_prg_float * 25)) / 25.0f;
			cut_per_texel_size = linear_limit<float, float>(discrete_wipe_prg_float, 0.0f, st_texel_size, 1.0f, ed_texel_size);
		}
		else {
			// ���Ԍo�߂𗣎U�I�ɍs��
			float discrete_wipe_prg_float = (float)((int)(wipe_prg_float * 25)) / 25.0f;
			// �Ō�Ԃ؂�ɂȂ�̂�h��
			if(discrete_wipe_prg_float < 0.95f)
				cut_per_texel_size = linear_limit<float, float>(discrete_wipe_prg_float, 0.0f, st_texel_size, 1.0f, ed_texel_size);
			else
				cut_per_texel_size = linear_limit<float, float>(wipe_prg_float, 0.0f, st_texel_size, 1.0f, ed_texel_size);
		}
	}

	if(!sprite->get_texture(0))
		return;

	// �萔�ݒ�
	float final_cut_per_texel_size = cut_per_texel_size;

	// 2��n��e�N�X�`�����쐬�ł��Ȃ����ɑΉ������(�쐬�ł�����ł� * 1.0f �ɂȂ�̂Ŗ��Ȃ�)
	float cut_per_texel_size_ex = cut_per_texel_size * sprite->get_texture(0)->get_width() / sprite->get_texture(0)->get_width_ex();
	// �����`�ɂ���ׂ̏c����(�������ex�͕K�v�Ȃ�)
	float tex_rate_for_square = (float)sprite->get_texture(0)->get_width() / sprite->get_texture(0)->get_height();
	
	// ���ۂɒ萔��ݒ肷��
	sprite->set_effect_constant_f(0, cut_per_texel_size, tex_rate_for_square, 0, 0);

	// �e�N�j�b�N�ݒ�
	if(Gp_wipe->m_type == 230 || Gp_wipe->m_type == 231)	{
		sprite->set_effect_technique(m_view.m_default_effect.m_mosaic_technique);
	}
}