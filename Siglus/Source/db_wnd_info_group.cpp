#include	"pch.h"
#include	"ifc_eng.h"
#include	"element/elm_group.h"
#include	"element/elm_stage.h"
#include	"element/elm_excall.h"
#include	"dialog/db_wnd_info_group.h"
#include	"dialog/db_wnd_info_element_lv.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�O���[�v
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_group")

// ������
void C_db_wnd_info_group::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// �L���v�V�������o�^
	regist_caption_name(_T("�O���[�v���"));
}

// �������t�@���N�V����
void C_db_wnd_info_group::on_init_func()
{
	// ���X�g�r���[��������
	tnm_init_lv_element(FM_GROUPLIST, true, get_lv());
}

// ����t�@���N�V����
void C_db_wnd_info_group::on_close_func()
{
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_group::frame(C_elm_group_list** p_group_list_list, int list_cnt)
{
	if (!m_h_wnd)
		return;

	// �t�����g�̃O���[�v����\������
	int item_no = 0;
	int stage_no = TNM_STAGE_FRONT;
	tnm_update_lv_list_table(&(*Gp_stage_list)[stage_no].m_group_list, get_lv(), &item_no, false);
	if (Gp_excall->is_ready())	{	// �d�w�R�[�����͂d�w�O���[�v���\��
		tnm_update_lv_list_table(&Gp_excall->m_stage_list[stage_no].m_group_list, get_lv(), &item_no, false);
	}
	tnm_reduce_item_cnt(get_lv(), item_no);
}

