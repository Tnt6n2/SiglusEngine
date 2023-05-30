#include	"pch.h"
#include	"dialog/db_wnd_info_listview.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�ėp���X�g�r���[
// ================================================================

#define		IDC_LISTVIEW		1000

// �J���i�쐬�j
void C_db_wnd_info_listview::open(HWND parent_h_wnd)
{
	if (m_h_wnd)
		return;

	// �_�C�A���O�e���v���[�g���g���ă_�C�A���O��g�ݗ��Ă�
	C_dialog_template dt;
	DWORD style = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | DS_SHELLFONT;
	dt.init_param(0, 0, 100, 160, caption_name_str, style, WS_EX_TOOLWINDOW, -1, 8, _T("MS Shell Dlg"));

	// ���X�g�r���[���쐬����
	style = WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_ALIGNLEFT;
	dt.add_control(0, 0, 100, 160, (PTR)_T("SysListView32"), IDC_LISTVIEW, style, 0, NULL);

	// �_�C�A���O���쐬����
	create_indirect(&dt, regist_state.window_name, regist_state.limit_min, regist_state.attach, parent_h_wnd);
}

// �E�B���h�E�v���V�[�W��
LRESULT C_db_wnd_info_listview::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init())		return FALSE;	break;
		case WM_CLOSE:			if (!on_close())	return FALSE;	break;
	}

	return C_db_wnd_info::window_proc(msg, wp, lp);
}

// ������
bool C_db_wnd_info_listview::on_init()
{
	// �t���[������t���O
	frame_first_flag = true;

	// �A�C�e���o�^
	bind_control(&lv, IDC_LISTVIEW, _T("listview"), 5);

	// ���X�g�r���[�̊ȈՐݒ�
	lv.easy_setting(true, true, false, true);

	// ���X�g�r���[�̃R�����ݒ�
	for (int i = 0; i < (int)lvc_list.size(); i++)	{
		lv.add_column(lvc_list[i].name, lvc_list[i].caption, lvc_list[i].width);
	}

	// �A�C�e���ݒ胍�[�h
	lv.load_ini();

	// �t�@���N�V����
	on_init_func();

	// �E�B���h�E��`�̓ǂݍ���
	load_ini_window_rect();

	// �E�B���h�E�\��
	update();
	set_show(true);

	return true;
}

// ����
bool C_db_wnd_info_listview::on_close()
{
	// �A�C�e���ݒ�Z�[�u
	lv.save_ini();

	// �t�@���N�V����
	on_close_func();

	// �E�B���h�E��`�̕ۑ�
	save_ini_window_rect();

	return true;
}

// ****************************************************************
// �L���v�V�������o�^
// ================================================================
void C_db_wnd_info_listview::regist_caption_name(CTSTR& name)
{
	caption_name_str = name;
}

// ****************************************************************
// ���X�g�r���[�̍��ڒǉ�
// ================================================================
void C_db_wnd_info_listview::listview_add_column(CTSTR& name, CTSTR& caption, int width)
{
	LISTVIEW_COLUMN lvc;
	lvc.name = name;
	lvc.caption = caption;
	lvc.width = width;
	lvc_list.push_back(lvc);
}


