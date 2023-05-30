#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_tool.h"
#include	"data/tnm_input.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_timer.h"
#include	"engine/eng_input.h"
#include	"engine/eng_fetch_point.h"
#include	"engine/eng_etc.h"

class	C_tnm_fetch_point;
extern	C_tnm_fetch_point*	Gp_fetch_point;

// �t�F�b�`���N���A����
void tnm_init_fetch_point()
{
	Gp_fetch_point->m_fetch_point.clear();
	Gp_fetch_point->m_fetch_rect.clear();
	Gp_fetch_point->m_fetch_start_pos = C_point(0, 0);
	Gp_fetch_point->m_fetch_end_pos = C_point(0, 0);
	Gp_fetch_point->m_fetch_last_pos = C_point(0, 0);
	Gp_fetch_point->m_fetch_last_no = 0;
	Gp_fetch_point->m_fetch_start_time = 0;
	Gp_fetch_point->m_fetch_time_len = 0;
	Gp_fetch_point->m_key_repeat_key = 0;
	Gp_fetch_point->m_key_repeat_start_time = 0;
	Gp_fetch_point->m_key_repeat_cnt = 0;
}

// �t�F�b�`�|�C���g��ǉ�����
void tnm_add_fetch_point(C_point fetch_pos, C_rect fetch_rect)
{
	Gp_fetch_point->m_fetch_point.push_back(fetch_pos);
	Gp_fetch_point->m_fetch_rect.push_back(fetch_rect);
}

// �t�F�b�`����ړ�
bool tnm_fetch_proc_initial_move()
{
#if 1

	// -1: �܂��ǂ��ɂ��t�F�b�`���Ă��Ȃ�
	Gp_fetch_point->m_fetch_last_no = -1;

#else

	// �O�ԂɃX�[�b���Ĉړ�����^�C�v


	// �O�ԂɃt�F�b�`����
	Gp_fetch_point->m_fetch_last_no = 0;
	Gp_fetch_point->m_fetch_time_len = 250;

	// �t�F�b�`�|�C���g���X�N���[�����W�ɒ���
	C_point fetch_point = Gp_fetch_point->m_fetch_point[Gp_fetch_point->m_fetch_last_no];
	fetch_point.x = fetch_point.x * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx + Gp_global->total_game_screen_pos.x;
	fetch_point.y = fetch_point.y * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
	fetch_point = Gp_wnd->client_to_screen(fetch_point);

	// �i����j�X�N���[���n�ł̃}�E�X���W���擾
	C_point mp = get_mouse_pos();

	// �t�F�b�`�J�n
	Gp_fetch_point->m_fetch_start_pos = mp;
	Gp_fetch_point->m_fetch_last_pos = mp;
	Gp_fetch_point->m_fetch_end_pos = fetch_point;
	Gp_fetch_point->m_fetch_start_time = Gp_timer->real_time;
#endif
	return true;
}

// �t�F�b�`�����F�t�F�b�`�|�C���g���X�N���[�����W�ɒ���
C_point tnm_fetch_proc_fetch_point_client_to_screen(C_point fetch_point)
{
	fetch_point.x = fetch_point.x * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx + Gp_global->total_game_screen_pos.x;
	fetch_point.y = fetch_point.y * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
	fetch_point = Gp_wnd->client_to_screen(fetch_point);
	return fetch_point;
}

// �t�F�b�`�����F�t�F�b�`��`���X�N���[�����W�ɒ���
C_rect tnm_fetch_proc_fetch_rect_client_to_screen(C_rect fetch_rect)
{
	fetch_rect.left = fetch_rect.left * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx + Gp_global->total_game_screen_pos.x;
	fetch_rect.top = fetch_rect.top * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
	fetch_rect.right = fetch_rect.right * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx + Gp_global->total_game_screen_pos.x;
	fetch_rect.bottom = fetch_rect.bottom * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
	fetch_rect = Gp_wnd->client_to_screen(fetch_rect);
	return fetch_rect;
}

// �t�F�b�`�����F�������Ă���t�F�b�`��`�����߂�
int tnm_fetch_proc_to_search_cur_fetch_point()
{
	int fetch_no = -1;
	for (int i = 0; i < (int)Gp_fetch_point->m_fetch_rect.size(); i++)	{
		// �t�F�b�`��`���X�N���[�����W�ɒ���
		C_rect fetch_rect = tnm_fetch_proc_fetch_rect_client_to_screen(Gp_fetch_point->m_fetch_rect[i]);
		// �}�E�X���������Ă���Ίm��
		if (fetch_rect.left <= Gp_cur_input->mouse.pos.x && Gp_cur_input->mouse.pos.x < fetch_rect.right)	{
			if (fetch_rect.top <= Gp_cur_input->mouse.pos.y && Gp_cur_input->mouse.pos.y < fetch_rect.bottom)	{
				fetch_no = i;
				break;
			}
		}
	}

	return fetch_no;
}

// �t�F�b�`�����F���Ɉړ�
bool tnm_fetch_proc_to_down()
{
#if 1
	// ���ݓ������Ă���t�F�b�`��`��T��
	C_rect fetch_rect;
	int fetch_no = tnm_fetch_proc_to_search_cur_fetch_point();

	// �t�F�b�`�ԍ���i�߂�
	if (fetch_no == -1)	{
		fetch_no = 0;
	}
	else	{
		fetch_no ++;
		if (fetch_no >= (int)Gp_fetch_point->m_fetch_point.size())
			fetch_no = 0;
	}

	// �t�F�b�`�ʒu�����߂�
	C_point fetch_point = Gp_fetch_point->m_fetch_point[fetch_no];
	fetch_point = tnm_fetch_proc_fetch_point_client_to_screen(fetch_point);

	// �}�E�X�ʒu��ݒ�
	Gp_cur_input->mouse.pos = fetch_point;
	::SetCursorPos(Gp_cur_input->mouse.pos.x, Gp_cur_input->mouse.pos.y);

	return true;

#else

	// �t�F�b�`�ԍ��ƈړ����Ԃ����߂�
	if (Gp_fetch_point->m_fetch_last_no == -1)	{
		Gp_fetch_point->m_fetch_last_no = 0;
	}
	else	{
		Gp_fetch_point->m_fetch_last_no ++;
		if (Gp_fetch_point->m_fetch_last_no >= (int)Gp_fetch_point->m_fetch_point.size())
			Gp_fetch_point->m_fetch_last_no = 0;
	}

	// �t�F�b�`�|�C���g���X�N���[�����W�ɒ���
	C_point fetch_point = Gp_fetch_point->m_fetch_point[Gp_fetch_point->m_fetch_last_no];
	fetch_point.x = fetch_point.x * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx + Gp_global->total_game_screen_pos.x;
	fetch_point.y = fetch_point.y * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
	fetch_point = Gp_wnd->client_to_screen(fetch_point);

	// �}�E�X�����ɓ������Ă���ꍇ�͂����P���ɐi��
	if (Gp_cur_input->mouse.pos == fetch_point)	{
		Gp_fetch_point->m_fetch_last_no ++;
		if (Gp_fetch_point->m_fetch_last_no >= (int)Gp_fetch_point->m_fetch_point.size())
			Gp_fetch_point->m_fetch_last_no = 0;

		// �t�F�b�`�|�C���g���X�N���[�����W�ɒ���
		fetch_point = Gp_fetch_point->m_fetch_point[Gp_fetch_point->m_fetch_last_no];
		fetch_point.x = fetch_point.x * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx + Gp_global->total_game_screen_pos.x;
		fetch_point.y = fetch_point.y * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
		fetch_point = Gp_wnd->client_to_screen(fetch_point);
	}

	// �}�E�X�ʒu��ݒ�
	Gp_cur_input->mouse.pos = fetch_point;
	::SetCursorPos(Gp_cur_input->mouse.pos.x, Gp_cur_input->mouse.pos.y);

	return true;
#endif
}

// �t�F�b�`�����F�O�Ɉړ�
bool tnm_fetch_proc_to_up()
{

#if 1
	// ���ݓ������Ă���t�F�b�`��`��T��
	C_rect fetch_rect;
	int fetch_no = tnm_fetch_proc_to_search_cur_fetch_point();

	// �t�F�b�`�ԍ���߂�
	if (fetch_no == -1)	{
		fetch_no = (int)Gp_fetch_point->m_fetch_point.size() - 1;
	}
	else	{
		fetch_no --;
		if (fetch_no < 0)
			fetch_no = (int)Gp_fetch_point->m_fetch_point.size() - 1;
	}

	// �t�F�b�`�ʒu�����߂�
	C_point fetch_point = Gp_fetch_point->m_fetch_point[fetch_no];
	fetch_point = tnm_fetch_proc_fetch_point_client_to_screen(fetch_point);

	// �}�E�X�ʒu��ݒ�
	Gp_cur_input->mouse.pos = fetch_point;
	::SetCursorPos(Gp_cur_input->mouse.pos.x, Gp_cur_input->mouse.pos.y);

	return true;

#else
	// �t�F�b�`�ԍ��ƈړ����Ԃ����߂�
	if (Gp_fetch_point->m_fetch_last_no == -1)	{
		Gp_fetch_point->m_fetch_last_no = (int)Gp_fetch_point->m_fetch_point.size() - 1;
	}
	else	{
		Gp_fetch_point->m_fetch_last_no --;
		if (Gp_fetch_point->m_fetch_last_no < 0)
			Gp_fetch_point->m_fetch_last_no = (int)Gp_fetch_point->m_fetch_point.size() - 1;
	}

	// �t�F�b�`�|�C���g���X�N���[�����W�ɒ���
	C_point fetch_point = Gp_fetch_point->m_fetch_point[Gp_fetch_point->m_fetch_last_no];
	fetch_point.x = fetch_point.x * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx + Gp_global->total_game_screen_pos.x;
	fetch_point.y = fetch_point.y * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
	fetch_point = Gp_wnd->client_to_screen(fetch_point);

	// �}�E�X�����ɓ������Ă���ꍇ�͂����P���ɐi��
	if (Gp_cur_input->mouse.pos == fetch_point)	{
		Gp_fetch_point->m_fetch_last_no --;
		if (Gp_fetch_point->m_fetch_last_no < 0)
			Gp_fetch_point->m_fetch_last_no = (int)Gp_fetch_point->m_fetch_point.size() - 1;

		// �t�F�b�`�|�C���g���X�N���[�����W�ɒ���
		fetch_point = Gp_fetch_point->m_fetch_point[Gp_fetch_point->m_fetch_last_no];
		fetch_point.x = fetch_point.x * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx + Gp_global->total_game_screen_pos.x;
		fetch_point.y = fetch_point.y * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
		fetch_point = Gp_wnd->client_to_screen(fetch_point);
	}

	// �}�E�X�ʒu��ݒ�
	Gp_cur_input->mouse.pos = fetch_point;
	::SetCursorPos(Gp_cur_input->mouse.pos.x, Gp_cur_input->mouse.pos.y);

	return true;
#endif
}

// �t�F�b�`����
bool tnm_fetch_proc()
{
	// ���̏����ł͍��W�̓O���[�o���Ȉʒu�ōs����

	// �O�񂩂�}�E�X�������Ă���΃t�F�b�`���I������
	if (Gp_fetch_point->m_fetch_time_len > 0 && Gp_cur_input->mouse.pos != Gp_fetch_point->m_fetch_last_pos)	{
		Gp_fetch_point->m_fetch_time_len = 0;
	}

	// �t�F�b�`����
	if (Gp_fetch_point->m_fetch_time_len > 0)	{

		int ct = Gp_timer->real_time;
		int st = Gp_fetch_point->m_fetch_start_time;
		int et = st + Gp_fetch_point->m_fetch_time_len;
		int svx = Gp_fetch_point->m_fetch_start_pos.x;
		int svy = Gp_fetch_point->m_fetch_start_pos.y;
		int evx = Gp_fetch_point->m_fetch_end_pos.x;
		int evy = Gp_fetch_point->m_fetch_end_pos.y;

		// �����v�Z
		int cvx = speed_down_limit(ct, st, svx, et, evx);
		int cvy = speed_down_limit(ct, st, svy, et, evy);

		// �}�E�X�ʒu��ݒ�
		Gp_cur_input->mouse.pos = C_point(cvx, cvy);
		::SetCursorPos(Gp_cur_input->mouse.pos.x, Gp_cur_input->mouse.pos.y);

		// �ݒ肵���l���L�����Ă���
		Gp_fetch_point->m_fetch_last_pos = C_point(cvx, cvy);

		// �I���������z������t�F�b�`�I��
		if (ct - et >= 0)	{
			Gp_fetch_point->m_fetch_time_len = 0;
		}
	}

	// �L�[�{�[�h�ŃJ�[�\���ړ��i�t�F�b�`�ړ����͈ړ����Ȃ��j
	if (!Gp_fetch_point->m_fetch_point.empty() && Gp_fetch_point->m_fetch_time_len == 0)	{

		// ���L�[�^���L�[
		if (tnm_input_use_key_down(VK_DOWN) || tnm_input_use_key_down(VK_RIGHT))	{

			// ���Ɉړ�
			tnm_fetch_proc_to_down();

			// �L�[���s�[�g�J�n
			Gp_fetch_point->m_key_repeat_key = 1;
			Gp_fetch_point->m_key_repeat_start_time = Gp_timer->real_time;
			Gp_fetch_point->m_key_repeat_cnt = 0;
		}
		// ���L�[�^���L�[
		else if (tnm_input_use_key_down(VK_UP) || tnm_input_use_key_down(VK_LEFT))	{

			// ��Ɉړ�
			tnm_fetch_proc_to_up();

			// �L�[���s�[�g�J�n
			Gp_fetch_point->m_key_repeat_key = 2;
			Gp_fetch_point->m_key_repeat_start_time = Gp_timer->real_time;
			Gp_fetch_point->m_key_repeat_cnt = 0;
		}
	}

	// �L�[���s�[�g
	if (Gp_fetch_point->m_key_repeat_key > 0)	{

		// �L�[���s�[�g�I������
		if (Gp_fetch_point->m_key_repeat_key == 1 && !tnm_input_is_key_down(VK_DOWN) &&  !tnm_input_is_key_down(VK_RIGHT))	{
			Gp_fetch_point->m_key_repeat_key = 0;
		}
		else if (Gp_fetch_point->m_key_repeat_key == 2 && !tnm_input_is_key_down(VK_UP) && !tnm_input_is_key_down(VK_LEFT))	{
			Gp_fetch_point->m_key_repeat_key = 0;
		}
		else	{

			// ���s�[�g�񐔂��玟�̈ړ��������擾
			int next_time;
			if (Gp_fetch_point->m_key_repeat_cnt == 0)	{
				next_time = Gp_fetch_point->m_key_repeat_start_time + 500;
			}
			else	{
				next_time = Gp_fetch_point->m_key_repeat_start_time + 500 + Gp_fetch_point->m_key_repeat_cnt * 50;
			}

			// �ړ������𒴂��Ă���ړ�
			if (Gp_timer->real_time - next_time > 0)	{
				if (Gp_fetch_point->m_key_repeat_key == 1)	{
					tnm_fetch_proc_to_down();
				}
				else	{
					tnm_fetch_proc_to_up();
				}

				// �L�[���s�[�g�񐔂𑝂₷
				Gp_fetch_point->m_key_repeat_cnt++;
			}
		}
	}

	return true;
}

// ****************************************************************
// �t�F�b�`�������s�����𔻒�
// ================================================================
bool tnm_is_fetch_proc()
{
	// �d�w�R�[���F�u�d�w�R�[���v���̓t�F�b�`�������s��Ȃ�
	if (tnm_excall_is_excall())
		return false;

	// �d�w�R�[���F�u���b�Z�[�W�o�b�N�v���̓t�F�b�`�������s��Ȃ�
	if (tnm_excall_is_msg_back())
		return false;

	// �d�w�R�[���F�u���b�Z�[�W�E�B���h�E���B���v���̓t�F�b�`�������s��Ȃ�
	if (tnm_excall_is_hide_mwnd())
		return false;

	// �t�F�b�`�|�C���g���Ȃ���΃t�F�b�`�������s��Ȃ�
	if (Gp_fetch_point->m_fetch_point.empty())
		return false;

	return true;
}


