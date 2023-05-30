#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_lexer.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_etc.h"
#include	"dialog/db_wnd_info_trace.h"
#include	"../resource.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�g���[�X
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_trace")

#define		FONT_SIZE_LIST_MAX		12
static int font_size_list[FONT_SIZE_LIST_MAX] = {8, 10, 12, 16, 18, 20, 22, 24, 26, 28, 30, 32,};

// ������
void C_db_wnd_info_trace::init()
{
	regist(REGIST_WINDOW_NAME, false, true, IDD_DB_WIN_INFO_TRACE);
}

// �E�B���h�E�v���V�[�W��
LRESULT C_db_wnd_info_trace::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	if (p_text_window.get())	{	p_text_window->message_proc(msg, wp, lp);	}

	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init())				return FALSE;	break;
		case WM_CLOSE:			if (!on_close())			return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
		case WM_NOTIFY:			if (!on_notify(wp, lp))		return FALSE;	break;
	}
	return C_db_wnd_info::window_proc(msg, wp, lp);
}

// ������
bool C_db_wnd_info_trace::on_init()
{
	work.page_cnt = 10;
	work.next_click_state = false;
	work.next_key_state = false;

	// ���݂̏���������
	work.cd.scn_no = -1;		// �V�[���ԍ�
	work.cd.line_no = -1;		// �s�ԍ�;
	work.cd.is_breaking = 0;	// �u���[�N���;

	// �O��̏���������
	work.ld = work.cd;

	// �A�C�e���o�^
	bind_control(&ci.chk_break, IDC_CHK_BREAK, _T(""), 3);
	bind_control(&ci.chk_click, IDC_CHK_CLICK, _T("CHK_CLICK"), 3);
	bind_control(&ci.chk_trace, IDC_CHK_TRACE, _T("CHK_TRACE"), 3);
	bind_control(&ci.cmb_font_size, IDC_CMB_FONT_SIZE, _T("CMB_FONT_SIZE"), 3);
	bind_control(&ci.btn_next, IDC_BTN_NEXT, _T(""), 1);
	bind_control(&ci.tab_text, IDC_TAB, _T(""), 5);

	// �t�H���g�T�C�Y�̃R���{�{�b�N�X�̃��X�g�쐬
	for (int i = 0; i < FONT_SIZE_LIST_MAX; i++)	{
		ci.cmb_font_size.insert_item(tostr(font_size_list[i]), i, false);
	}

	// �^�u�ɐe�̃N���X�|�C���^�[��ݒ�
	ci.tab_text.parent_class = this;

	// �^�u��ǉ�
	for (int i = 0; i < work.page_cnt; i++)	{
		ci.tab_text.add_item(_T("?"));
	}

	// �Z�p���[�^�o�^

	// �A�C�e���ݒ胍�[�h
	ci.chk_click.load_ini(true);
	ci.chk_trace.load_ini(true);
	ci.cmb_font_size.load_ini_sel_no(2);

	// �Z�p���[�^�ݒ胍�[�h

	// �E�B���h�E��`�̓ǂݍ���
	load_ini_window_rect();
	
	// �A�C�e���t�H�[�J�X

	// �u���[�N��Ԃ�ݒ�
	set_break(Gp_global->is_breaking);

	// �E�B���h�E�\��
	update();
	set_show(true);

	return true;
}

// ����
bool C_db_wnd_info_trace::on_close()
{
	// �X�e�[�^�X�̎擾�ƕۑ�

	// �A�C�e���ݒ�Z�[�u
	ci.chk_click.save_ini();
	ci.chk_trace.save_ini();
	ci.cmb_font_size.save_ini_sel_no();

	// �Z�p���[�^�ݒ�Z�[�u

	// �E�B���h�E��`�̕ۑ�
	save_ini_window_rect();

	p_text_window.reset();

	return true;
}

// �R�}���h
bool C_db_wnd_info_trace::on_command(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{
		case IDC_CHK_BREAK:
			Gp_global->is_breaking = ci.chk_break.get_check();
			set_focus_draw_target();
			break;
		case IDC_CHK_CLICK:
			set_focus_draw_target();
			break;
		case IDC_CHK_TRACE:
			set_focus_draw_target();
			break;
		case IDC_BTN_NEXT:
			next_step();
			set_focus_draw_target();
			break;
		case IDC_CMB_FONT_SIZE:
			switch (HIWORD(wp))	{
				case CBN_SELCHANGE:
					draw_text_for_change_font_size(font_size_list[ci.cmb_font_size.get_sel_no()]);
					set_focus_draw_target();
					break;
				case CBN_CLOSEUP:
					set_focus_draw_target();
					break;
			}
			break;
	}
	return true;
}

// �ʒm
bool C_db_wnd_info_trace::on_notify(WPARAM wp, LPARAM lp)
{
	NMHDR* hdr = (NMHDR *)lp;

	// �^�u�R���g���[��
	if (hdr->hwndFrom == ci.tab_text.get_handle())	{
		switch (hdr->code)	{
			case TCN_SELCHANGE:
				draw_scn_for_change_select_tab(ci.tab_text.get_selected_item_no());
				break;
		}
	}

	return true;
}

// ****************************************************************
// �X�e�[�^�X�ǂݍ���
// ================================================================
void C_db_wnd_info_trace::load_state()
{
}

// ****************************************************************
// �X�e�[�^�X�ۑ�
// ================================================================
void C_db_wnd_info_trace::save_state()
{
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_trace::frame()
{
	if (!m_h_wnd)
		return;

	// �e�L�X�g�E�B���h�E�쐬
	if (!p_text_window.get())	{
		C_rect rect = screen_to_client(ci.tab_text.get_display_rect());	// �^�u�̕\���̈�����߂�
		p_text_window = BSP<C_db_wnd_info_trace_text_window>(new C_db_wnd_info_trace_text_window);
		p_text_window->create(get_handle(), rect, work.page_cnt);
	}

	// ���̍X�V
	work.cd.scn_no = Gp_lexer->get_cur_scn_no();
	work.cd.line_no = Gp_lexer->get_cur_line_no();
	work.cd.is_breaking = Gp_global->is_breaking;

	// �u���[�N��Ԃ�ݒ�i�O���� Gp_local->is_breaking �̒l���ύX�����j
	if (work.ld.is_breaking != work.cd.is_breaking)	{
		set_break(Gp_global->is_breaking);
	}

	// �g���[�X���̃e�L�X�g�\��
	if (ci.chk_trace.get_check() || ci.chk_break.get_check())	{
		if (work.ld.scn_no != work.cd.scn_no || work.ld.line_no != work.cd.line_no)	{
			draw_scn_for_trace(work.cd.scn_no, work.cd.line_no);
			set_tab_str();
		}
	}

	// ���̃X�e�b�v�֐i�߂�
	if (Gp_global->is_breaking)	{
		bool next_step_flag = false;
		// ��ʂ��N���b�N����
		if (ci.chk_click.get_check())	{
			if (work.next_click_state)	{
				if (!is_key_down(VK_LBUTTON))	{
					// �Q�[���E�B���h�E���t�H�[�J�X����Ă��āA�E�B���h�E���Ń}�E�X�������ꂽ
					if (GetFocus() == Gp_wnd->get_handle() && Gp_wnd->get_window_rect().is_in(get_mouse_pos()))	{
						next_step();
						next_step_flag = true;
					}
				}
			}
			work.next_click_state = is_key_down(VK_LBUTTON);
		}
		// �u�I�P�ʁv�L�[�������ꂽ
		if (next_step_flag == false)	{
			if (work.next_key_state)	{
				if (!is_key_down(_T('1')))	{
					// �Q�[���E�B���h�E�������̓g���[�X�E�B���h�E���t�H�[�J�X����Ă���
					if (GetFocus() == Gp_wnd->get_handle() || GetFocus() == m_h_wnd || check_focus_draw_target())	{
						next_step();
						next_step_flag = true;
					}
				}
			}
			work.next_key_state = is_key_down(_T('1'));
		}
	}

	// ���̃R�s�[
	work.ld = work.cd;
}

// ****************************************************************
// �e�L�X�g�^�u�̃E�B���h�E�v���V�[�W��
// ================================================================
LRESULT C_db_wnd_info_trace::Ctab_text::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_SIZE:
		case WM_MOVE:
			if (parent_class->p_text_window.get())	{
				C_rect rect = parent_class->screen_to_client(get_display_rect());	// �^�u�̕\���̈�����߂�
				parent_class->p_text_window->resize_text_window_rect(rect);
				parent_class->p_text_window->re_draw_seen_file();				// ���V�[���t�@�C����`��
			}
			break;
	}
	return Cc_tab::window_proc(msg, wp, lp);
}

// ****************************************************************
// �u���[�N��Ԃ�ݒ�
// ================================================================
void C_db_wnd_info_trace::set_break(bool is_breaking)
{
	if (is_breaking)	{
		// �u���[�N��
		set_text(_T("�g���[�X�i�u���[�N���j"));
		ci.chk_break.set_text(_T("�u���[�N����"));
	}
	else	{
		// �u���[�N���Ă��Ȃ�
		set_text(_T("�g���[�X"));
		ci.chk_break.set_text(_T("�u���[�N"));
	}
	if (is_breaking != ci.chk_break.get_check())	{	ci.chk_break.set_check(is_breaking);	}
	Gp_global->is_breaking = is_breaking;
	work.cd.is_breaking = is_breaking;
}

// ****************************************************************
// ���̃X�e�b�v�֐i��
// ================================================================
void C_db_wnd_info_trace::next_step()
{
	Gp_global->break_step_flag = true;		// �X�e�b�v���s
}

// ****************************************************************
// �e�L�X�g�`��i�g���[�X���j
// ================================================================
void C_db_wnd_info_trace::draw_scn_for_trace(int scn_no, int line_no)
{
	if (!p_text_window.get())	{	return;	}
	ci.tab_text.set_selected_item_no(0);		// �O�Ԃ������I�ɑI������
	p_text_window->draw_seen_file(0, scn_no, line_no, line_no, _T("�l�r �S�V�b�N"), font_size_list[ci.cmb_font_size.get_sel_no()], true);
}

// ****************************************************************
// �e�L�X�g�`��i�^�u�ύX���j
// ================================================================
void C_db_wnd_info_trace::draw_scn_for_change_select_tab(int tab_no)
{
	if (!p_text_window.get())	{	return;	}
	p_text_window->change_draw_seen_file(tab_no);
}

// ****************************************************************
// �e�L�X�g�`��i�����T�C�Y�ύX���j
// ================================================================
void C_db_wnd_info_trace::draw_text_for_change_font_size(int font_size)
{
	if (!p_text_window.get())	{	return;	}
	p_text_window->re_draw_seen_file(_T("�l�r �S�V�b�N"), font_size);
}

// ****************************************************************
// �`��^�[�Q�b�g���t�H�[�J�X����
// ================================================================
void C_db_wnd_info_trace::set_focus_draw_target()
{
	if (!p_text_window.get())	{	return;	}
	p_text_window->set_focus_draw_target();
}

// ****************************************************************
// �`��^�[�Q�b�g���t�H�[�J�X����Ă��邩���肷��
// ================================================================
bool C_db_wnd_info_trace::check_focus_draw_target()
{
	if (!p_text_window.get())	{	return false;	}
	return p_text_window->check_focus_draw_target();
}

// ****************************************************************
// �^�u�̕�����ύX
// ================================================================
void C_db_wnd_info_trace::set_tab_str()
{
	if (!p_text_window.get())	{	return;	}

#if 0		// ���̏ꍇ�A�Ȃ����`��^�[�Q�b�g�̃N���C�A���g�G�b�W��������Ă��܂��B
	int scn_no;
	for (int i = 0; i < work.page_cnt; i++)	{
		scn_no = p_text_window->get_scn_no(i);
		if (scn_no >= 0)	{	ci.tab_text.set_item_text(i, Gp_eng->get_scn_name(scn_no));	}
		else				{	ci.tab_text.set_item_text(i, TSTR(_T("?")));					}
	}
#else
	int scn_no;
	TSTR next, now;
	bool str_change = false;
	for (int i = 0; i < work.page_cnt; i++)	{
		scn_no = p_text_window->get_scn_no(i);
		next = (scn_no >= 0) ? tnm_get_scn_name(scn_no) : TSTR(_T("?"));
		now = ci.tab_text.get_item_text(i);
		if (next != now)	{
			ci.tab_text.set_item_text(i, next);
			str_change = true;
		}
	}
	if (str_change)	{
		// �S��ʍX�V����i���A�C�e���P�ʂł͂��܂������Ȃ��c�j
		invalidate_rect(NULL, false);
	}
#endif
}

