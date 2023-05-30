#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_local_save.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_scene.h"
#include	"dialog/db_wnd_info_save_history.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�V�X�e��
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_save_history")

// ������
void C_db_wnd_info_save_history::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// �L���v�V�������o�^
	regist_caption_name(_T("�Z�[�u�|�C���g����"));

	// ���X�g�r���[����
	listview_add_column(_T("state"), _T(""), 25);
	listview_add_column(_T("save_pont"), _T("�Z�[�u�|�C���g"), 160);
}

// �E�B���h�E�v���V�[�W��
LRESULT C_db_wnd_info_save_history::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_NOTIFY:			if (!on_notify(wp, lp))		return FALSE;	break;
	}
	return C_db_wnd_info_listview::window_proc(msg, wp, lp);
}

// �������t�@���N�V����
void C_db_wnd_info_save_history::on_init_func()
{
}

// ����t�@���N�V����
void C_db_wnd_info_save_history::on_close_func()
{
}

// �m�n�s�h�e�x
bool C_db_wnd_info_save_history::on_notify(WPARAM wp, LPARAM lp)
{
	NMHDR* nm = (NMHDR*)lp;
	if (nm->code == NM_DBLCLK)	{
		NM_LISTVIEW* nmlv = (NM_LISTVIEW*)lp;
		if (nmlv->hdr.hwndFrom == lv.get_handle())	{
			load(nmlv->iItem);
		}
	}
	return true;
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_save_history::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

	// ���݂̒l���擾����
	cd.m_save_history_top = Gp_save_history->top;
	cd.m_save_history_cnt = Gp_save_history->cnt;

	// ���ڂ����ɐݒ肷��
	int item_no = -1;

	// �������X�g�X�V
	item_no ++;
	if (frame_first_flag || Gp_save_history->update_flag || cd.m_save_history_top != m_ld.m_save_history_top || cd.m_save_history_cnt != m_ld.m_save_history_cnt)	{

		// ���X�g������
		if (lv.get_item_cnt() != cd.m_save_history_cnt)	{
			lv.set_item_cnt(cd.m_save_history_cnt);
		}
		else if (cd.m_save_history_top != m_ld.m_save_history_top)	{
			lv.delete_item(0);
			lv.add_item();
		}

		// ���X�g�ݒ�
		if (cd.m_save_history_cnt > 0)	{
			int index = cd.m_save_history_top;
			SYSTEMTIME *time;
			for (int i = 0; i < cd.m_save_history_cnt; i++)	{
				switch (Gp_save_history->item[index].flag)	{
					case 0:		lv.set_cell_text(i, 0, _T(""));		break;
					case 1:		lv.set_cell_text(i, 0, _T("��"));	break;
					case 2:		lv.set_cell_text(i, 0, _T("��"));	break;
				}
				time = &Gp_save_history->item[index].systemtime;
				TSTR str = str_format(_T("%04d/%02d/%02d (%02d:%02d:%02d)"), time->wYear, time->wMonth, time->wDay, time->wHour, time->wMinute, time->wSecond);
				if (!Gp_save_history->item[index].local_save.save_msg.empty())	{
					str += _T("�@") + Gp_save_history->item[index].local_save.save_msg;
				}
				lv.set_cell_text(i, 1, str);
				index++;
				if (index >= cd.m_save_history_cnt)	{
					index = 0;
				}
			}
			index--;
			if (index < 0)	{
				index = cd.m_save_history_cnt - 1;
			}
			lv.set_item_visible(cd.m_save_history_cnt - 1);
		}

		Gp_save_history->update_flag = false;
	}

	// �f�[�^���L��
	m_ld = cd;

	// ����t���O�����낷
	frame_first_flag = false;
}

// ****************************************************************
// ���[�h
// ================================================================
void C_db_wnd_info_save_history::load(int click_index)
{
	if (Gp_save_history->cnt <= 0 || click_index < 0 || Gp_save_history->cnt <= click_index)
		return;

	// ���[�h���闚��ԍ�
	int index = (Gp_save_history->top + click_index) % Gp_save_history->cnt;

	// �t���O����
	if (Gp_save_history->item[index].flag != 2)	{
		for (int i = 0; i < Gp_save_history->cnt; i++)	{
			if (Gp_save_history->item[i].flag > 0)	{
				Gp_save_history->item[i].flag--;
			}
		}
	}

	// �����Z�b�g�t���O�𗧂Ă�
	Gp_save_history->update_flag = true;
	Gp_save_history->item[index].flag = 2;

	// ���[�h�i�f�o�b�O�p�F�ȈՔŁj
	tnm_saveload_proc_load_save_history(&Gp_save_history->item[index].local_save);

	// �Q�D�ȈՃ��[�h�̌㏈��
	tnm_push_proc(TNM_PROC_TYPE_EASY_LOAD);

	// �P�D�`����s��
	tnm_push_proc(TNM_PROC_TYPE_DISP);
}

