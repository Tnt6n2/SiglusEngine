#include	"pch.h"
#include	"ifc_eng.h"
#include	"element/elm_counter.h"
#include	"element/elm_excall.h"
#include	"dialog/db_wnd_info_counter.h"
#include	"dialog/db_wnd_info_element_lv.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�J�E���^
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_counter")

// ������
void C_db_wnd_info_counter::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// �L���v�V�������o�^
	regist_caption_name(_T("�J�E���^���"));
}

// �������t�@���N�V����
void C_db_wnd_info_counter::on_init_func()
{
	// ���X�g�r���[����
	tnm_init_lv_element(FM_COUNTERLIST, true, &lv);
}

// ����t�@���N�V����
void C_db_wnd_info_counter::on_close_func()
{
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_counter::frame()
{
	if (!m_h_wnd)
		return;

	// ���X�g�r���[���X�V
	int item_no = 0;
	tnm_update_lv_list_table(Gp_counter_list, &lv, &item_no, false);
	if (Gp_excall->is_ready())	{	// �d�w�R�[�����͂d�w�J�E���^���\��
		tnm_update_lv_list_table(&Gp_excall->m_counter_list, &lv, &item_no, false);
	}
	tnm_reduce_item_cnt(get_lv(), item_no);
}

