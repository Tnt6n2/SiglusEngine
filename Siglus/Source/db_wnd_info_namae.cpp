#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_namae.h"
#include	"dialog/db_wnd_info_namae.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�y���O�z
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_namae")

// ������
void C_db_wnd_info_namae::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// �L���v�V�������o�^
	regist_caption_name(_T("�y���O�z���"));

	// ���X�g�r���[����
	listview_add_column(_T("namae_str"), _T("���O"), 100);
	listview_add_column(_T("change_str"), _T("�ϊ���"), 100);
	listview_add_column(_T("exist"), _T("����"), 60);
	listview_add_column(_T("moji_color_mod"), _T("���[�h"), 60);
	listview_add_column(_T("moji_color"), _T("�����F"), 60);
	listview_add_column(_T("shadow_color"), _T("�e�F"), 60);
}

// �������t�@���N�V����
void C_db_wnd_info_namae::on_init_func()
{
	if (Gp_namae->namae_data_base_list.empty())
		return;

	int list_cnt = (int)Gp_namae->namae_data_base_list.size();
	for (int i = 0; i < list_cnt; i++)	{

		// �o�^��
		lv.add_item(0, Gp_namae->namae_data_base_list[i].regist_name_str);

		// �o�^���
		lv.set_cell_text(i, 1, Gp_namae->namae_data_base_list[i].regist_flag ? Gp_namae->namae_data_base_list[i].change_name_str : _T("�i�����o�^�j"));

		// ����
		lv.set_cell_text(i, 2, Gp_namae->namae_data_base_list[i].exist_flag ? _T("��") : _T("�i�����݂��Ȃ��j"));

		// �o�^���
		if (Gp_namae->namae_data_base_list[i].regist_flag)	{
			lv.set_cell_text(i, 3, tostr(Gp_namae->namae_data_base_list[i].color_mod));
			lv.set_cell_text(i, 4, tostr(Gp_namae->namae_data_base_list[i].moji_color_no));
			lv.set_cell_text(i, 5, tostr(Gp_namae->namae_data_base_list[i].shadow_color_no));
		}
	}
}

// ����t�@���N�V����
void C_db_wnd_info_namae::on_close_func()
{
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_namae::frame()
{
	if (!m_h_wnd)
		return;

	// ����t���O�����낷
	frame_first_flag = false;
}
