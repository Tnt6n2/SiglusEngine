#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_input.h"
#include	"engine/eng_input.h"
#include	"dialog/db_wnd_info_input.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F����
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_input")

// ****************************************************************
// �f�o�b�O���E�B���h�E�F����
// ================================================================

// ������
void C_db_wnd_info_input::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// �L���v�V�������o�^
	regist_caption_name(_T("���͏��"));

	// ���X�g�r���[����
	listview_add_column(_T("state"), _T("�X�e�[�g"), 100);
	listview_add_column(_T("value"), _T("�l"), 100);
}

// �������t�@���N�V����
void C_db_wnd_info_input::on_init_func()
{
	lv.add_item(0, _T("�i�V�X�e���j�}�E�X�ʒu"));
	lv.add_item(0, _T("�i�V�X�e���j�}�E�X��������"));
	lv.add_item(0, _T("�i�V�X�e���j�}�E�X��������"));
	lv.add_item(0, _T("�i�V�X�e���j�}�E�X�������ĕ�����"));
	lv.add_item(0, _T("�i�V�X�e���j�}�E�X�������Ă���"));
	lv.add_item(0, _T("�i�V�X�e���j�}�E�X�E������"));
	lv.add_item(0, _T("�i�V�X�e���j�}�E�X�E������"));
	lv.add_item(0, _T("�i�V�X�e���j�}�E�X�E�����ĕ�����"));
	lv.add_item(0, _T("�i�V�X�e���j�}�E�X�E�����Ă���"));
	lv.add_item(0, _T("�i�X�N���v�g�j�}�E�X�ʒu"));
	lv.add_item(0, _T("�i�X�N���v�g�j�}�E�X��������"));
	lv.add_item(0, _T("�i�X�N���v�g�j�}�E�X��������"));
	lv.add_item(0, _T("�i�X�N���v�g�j�}�E�X�������ĕ�����"));
	lv.add_item(0, _T("�i�X�N���v�g�j�}�E�X�������Ă���"));
	lv.add_item(0, _T("�i�X�N���v�g�j�}�E�X�E������"));
	lv.add_item(0, _T("�i�X�N���v�g�j�}�E�X�E������"));
	lv.add_item(0, _T("�i�X�N���v�g�j�}�E�X�E�����ĕ�����"));
	lv.add_item(0, _T("�i�X�N���v�g�j�}�E�X�E�����Ă���"));
}

// ����t�@���N�V����
void C_db_wnd_info_input::on_close_func()
{
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_input::frame()
{
	if (!m_h_wnd)
		return;

	// ���݂̃f�[�^�����
	STATE cd;
	cd.mouse_pos = Gp_cur_input->mouse.pos;
	cd.mouse_left_down = Gp_input->now.mouse.left.get_down_stock();
	cd.mouse_left_up = Gp_input->now.mouse.left.get_up_stock();
	cd.mouse_left_down_up = Gp_input->now.mouse.left.get_down_up_stock();
	cd.mouse_left_state = Gp_input->now.mouse.left.get_real_state();
	cd.mouse_right_down = Gp_input->now.mouse.right.get_down_stock();
	cd.mouse_right_up = Gp_input->now.mouse.right.get_up_stock();
	cd.mouse_right_down_up = Gp_input->now.mouse.right.get_down_up_stock();
	cd.mouse_right_state = Gp_input->now.mouse.right.get_real_state();
	cd.script_mouse_pos = Gp_script_input->now.mouse.pos;
	cd.script_mouse_left_down = Gp_script_input->now.mouse.left.get_down_stock();
	cd.script_mouse_left_up = Gp_script_input->now.mouse.left.get_up_stock();
	cd.script_mouse_left_down_up = Gp_script_input->now.mouse.left.get_down_up_stock();
	cd.script_mouse_left_state = Gp_script_input->now.mouse.left.get_real_state();
	cd.script_mouse_right_down = Gp_script_input->now.mouse.right.get_down_stock();
	cd.script_mouse_right_up = Gp_script_input->now.mouse.right.get_up_stock();
	cd.script_mouse_right_down_up = Gp_script_input->now.mouse.right.get_down_up_stock();
	cd.script_mouse_right_state = Gp_script_input->now.mouse.right.get_real_state();

	int item_no = -1;

	// �}�E�X
	item_no ++;
	if (frame_first_flag || cd.mouse_pos != m_ld.mouse_pos)
		lv.set_cell_text(item_no, 1, tostr(cd.mouse_pos));
	item_no ++;
	if (frame_first_flag || cd.mouse_left_down != m_ld.mouse_left_down)
		lv.set_cell_text(item_no, 1, tostr(cd.mouse_left_down));
	item_no ++;
	if (frame_first_flag || cd.mouse_left_up != m_ld.mouse_left_up)
		lv.set_cell_text(item_no, 1, tostr(cd.mouse_left_up));
	item_no ++;
	if (frame_first_flag || cd.mouse_left_down_up != m_ld.mouse_left_down_up)
		lv.set_cell_text(item_no, 1, tostr(cd.mouse_left_down_up));
	item_no ++;
	if (frame_first_flag || cd.mouse_left_state != m_ld.mouse_left_state)
		lv.set_cell_text(item_no, 1, tostr(cd.mouse_left_state));
	item_no ++;
	if (frame_first_flag || cd.mouse_right_down != m_ld.mouse_right_down)
		lv.set_cell_text(item_no, 1, tostr(cd.mouse_right_down));
	item_no ++;
	if (frame_first_flag || cd.mouse_right_up != m_ld.mouse_right_up)
		lv.set_cell_text(item_no, 1, tostr(cd.mouse_right_up));
	item_no ++;
	if (frame_first_flag || cd.mouse_right_down_up != m_ld.mouse_right_down_up)
		lv.set_cell_text(item_no, 1, tostr(cd.mouse_right_down_up));
	item_no ++;
	if (frame_first_flag || cd.mouse_right_state != m_ld.mouse_right_state)
		lv.set_cell_text(item_no, 1, tostr(cd.mouse_right_state));

	// �X�N���v�g�̃}�E�X
	item_no ++;
	if (frame_first_flag || cd.script_mouse_pos != m_ld.script_mouse_pos)
		lv.set_cell_text(item_no, 1, tostr(cd.script_mouse_pos));
	item_no ++;
	if (frame_first_flag || cd.script_mouse_left_down != m_ld.script_mouse_left_down)
		lv.set_cell_text(item_no, 1, tostr(cd.script_mouse_left_down));
	item_no ++;
	if (frame_first_flag || cd.script_mouse_left_up != m_ld.script_mouse_left_up)
		lv.set_cell_text(item_no, 1, tostr(cd.script_mouse_left_up));
	item_no ++;
	if (frame_first_flag || cd.script_mouse_left_down_up != m_ld.script_mouse_left_down_up)
		lv.set_cell_text(item_no, 1, tostr(cd.script_mouse_left_down_up));
	item_no ++;
	if (frame_first_flag || cd.script_mouse_left_state != m_ld.script_mouse_left_state)
		lv.set_cell_text(item_no, 1, tostr(cd.script_mouse_left_state));
	item_no ++;
	if (frame_first_flag || cd.script_mouse_right_down != m_ld.script_mouse_right_down)
		lv.set_cell_text(item_no, 1, tostr(cd.script_mouse_right_down));
	item_no ++;
	if (frame_first_flag || cd.script_mouse_right_up != m_ld.script_mouse_right_up)
		lv.set_cell_text(item_no, 1, tostr(cd.script_mouse_right_up));
	item_no ++;
	if (frame_first_flag || cd.script_mouse_right_down_up != m_ld.script_mouse_right_down_up)
		lv.set_cell_text(item_no, 1, tostr(cd.script_mouse_right_down_up));
	item_no ++;
	if (frame_first_flag || cd.script_mouse_right_state != m_ld.script_mouse_right_state)
		lv.set_cell_text(item_no, 1, tostr(cd.script_mouse_right_state));

	// �f�[�^���L��
	m_ld = cd;

	// ����t���O�����낷
	frame_first_flag = false;
}

