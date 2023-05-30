#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_syscom.h"
#include	"data/tnm_save_cache.h"
#include	"engine/eng_config.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_save.h"
#include	"dialog/cfg_wnd_func_mod.h"
#include	"dialog/sys_wnd_func_saveload.h"

#include	"localize/localize.h"
#include	"../resource.h"

// ****************************************************************
// �V�X�e���E�B���h�E�F�t�@���N�V�����F�Z�[�u���[�h
// ================================================================

#define		SAVELOAD_MODAL_DIALOG_INI_NAME		_T("SAVELOAD_MODAL_DIALOG")

// ������
void C_sys_wnd_func_saveload::on_init_funcfunc()
{
	select_return_value = -1;

	// �A�C�e���o�^
	p_dlg->bind_control(&ci.tab_saveload, IDC_TAB_SAVELOAD, _T("TAB_SAVELOAD"), 5);
	p_dlg->bind_control(&ci.lst_saveload, IDC_LST_SAVELOAD, _T("LST_SAVELOAD"), 5);
	p_dlg->bind_control(&ci.chk_warning, IDC_CHK_WARNING, _T(""), 1);
	p_dlg->bind_control(&ci.chk_dblclick, IDC_CHK_DBLCLICK, _T(""), 1);
	p_dlg->bind_control(&ci.btn_new, IDC_BTN_NEW, _T(""), 3);
	p_dlg->bind_control(&ci.btn_saveload, IDC_BTN_SAVELOAD, _T(""), 3);
	p_dlg->bind_control(&ci.btn_close, IDC_BTN_CLOSE, _T(""), 3);

	ci.tab_saveload.parent_class = this;
	ci.lst_saveload.parent_class = this;
	ci.chk_warning.parent_class = this;
	ci.chk_dblclick.parent_class = this;
	ci.btn_new.parent_class = this;
	ci.btn_saveload.parent_class = this;
	ci.btn_close.parent_class = this;

// �����������F�Z�[�u���[�h�_�C�A���O������
// ���N�C�b�N�f�[�^�ɖ��Ή��i�N�C�b�N�f�[�^�^�u���K�v�H�j

	// �A�C�e���̐ݒ�

	// �f�[�^�̐ݒ�
	work.data_cnt_par_page = Gp_ini->config.saveload_dialog.data_cnt_par_page;
	work.page_cnt = (Gp_ini->save_cnt + (work.data_cnt_par_page - 1)) / work.data_cnt_par_page;
	if (Gp_ini->save_cnt <= 100)		{	work.data_index_keta_mode = 0;	}
	else if (Gp_ini->save_cnt <= 1000)	{	work.data_index_keta_mode = 1;	}
	else								{	work.data_index_keta_mode = 2;	}

	// �Z�[�u���[�h�̃{�^���̐ݒ�
	if (func_mod == SYS_WND_FUNC_MOD_SOLO_SAVE)	{	ci.btn_saveload.set_text(Gp_ini->config.save_info.dlgwnd_deside_button_str);	}
	else										{	ci.btn_saveload.set_text(Gp_ini->config.load_info.dlgwnd_deside_button_str);	}

	// ����̃{�^���̐ݒ�
	if (func_mod == SYS_WND_FUNC_MOD_SOLO_SAVE)	{ ci.btn_close.set_text(LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR); }
	else										{ ci.btn_close.set_text(LCL_STR_MODAL_DIALOG_CLOSE_BTN_STR); }

	// �x���̃`�F�b�N�{�b�N�X�̐ݒ�
	if (func_mod == SYS_WND_FUNC_MOD_SOLO_SAVE)	{	ci.chk_warning.set_text(Gp_ini->config.save_info.dlgwnd_warning_chkbox_str);	}
	else										{	ci.chk_warning.set_text(Gp_ini->config.load_info.dlgwnd_warning_chkbox_str);	}
	ci.chk_warning.adjust_width_from_text(20);

	// �_�u���N���b�N�ł̃`�F�b�N�{�b�N�X�̐ݒ�
	if (func_mod == SYS_WND_FUNC_MOD_SOLO_SAVE)	{	ci.chk_dblclick.set_text(Gp_ini->config.save_info.dlgwnd_dblclick_chkbox_str);	}
	else										{	ci.chk_dblclick.set_text(Gp_ini->config.load_info.dlgwnd_dblclick_chkbox_str);	}
	ci.chk_dblclick.adjust_width_from_text(20);

	// �ŐV�̃Z�[�u�f�[�^�ԍ�
	int new_save_no = tnm_get_new_save_no();
	work.new_tab_page_no = -1;
	if (0 <= new_save_no && new_save_no < Gp_ini->save_cnt)	{
		int tab_no = new_save_no / work.data_cnt_par_page;
		if (0 <= tab_no && tab_no < work.page_cnt)	{
			work.new_tab_page_no = tab_no;
		}
	}

	// �^�u�̐ݒ�
	TSTR tab_str;
	for (int i = 0; i < work.page_cnt; i++)	{
		tab_str = _T(" ") + tostr(i + 1);
		if (i == work.new_tab_page_no)	{
			tab_str += _T(" *");
		}
		ci.tab_saveload.add_item(tab_str);
	}

	// ���X�g�r���[�̐ݒ�i�^�u�̐ݒ�̌�ŏ������鎖�j
	C_rect rect = p_dlg->screen_to_client(ci.tab_saveload.get_display_rect());	// �^�u�̕\���̈�����߂�
	rect.left -= 2;		// ������
	ci.lst_saveload.set_window_rect(rect);
	ci.lst_saveload.set_def_rect_from_now_rect();
	ci.lst_saveload.add_column(_T("new"), _T("New"), 30);
	ci.lst_saveload.add_column(_T("data"), _T("Data"), 10);
	ci.lst_saveload.easy_setting(true, false, false, false);
	ci.lst_saveload.set_column_width_just(1, false);

	// ���ݑI������Ă���^�u�̐ݒ�
	int tab_select_no;
	if (modal_mode_flag)	{
		tab_select_no = C_ini_file::body().load_int(_T("TAB_SELECT_NO"), 0, SAVELOAD_MODAL_DIALOG_INI_NAME);
	}
	else	{
		tab_select_no = ci.tab_saveload.load_ini_int(_T("TAB_SELECT_NO"), 0);
	}
	if (!(0 <= tab_select_no && tab_select_no < work.page_cnt))	{	tab_select_no = 0;	}
	ci.tab_saveload.set_selected_item_no(tab_select_no);

	// ���X�g�̍\�z
	work.now_page_no = -1;
	create_data_list();

	// ���ݑI������Ă��郊�X�g�̐ݒ�
	int list_top_no;
	int list_select_no;
	if (modal_mode_flag)	{
		list_top_no = C_ini_file::body().load_int(_T("LIST_TOP_NO"), -1, SAVELOAD_MODAL_DIALOG_INI_NAME);
		list_select_no = C_ini_file::body().load_int(_T("LIST_SELECT_NO"), -1, SAVELOAD_MODAL_DIALOG_INI_NAME);
	}
	else	{
		list_top_no = ci.lst_saveload.load_ini_int(_T("LIST_TOP_NO"), -1);
		list_select_no = ci.lst_saveload.load_ini_int(_T("LIST_SELECT_NO"), -1);
	}
	if (0 <= list_top_no && list_top_no < ci.lst_saveload.get_item_cnt())	{
		ci.lst_saveload.set_item_visible_top(list_top_no);
	}
	if (0 <= list_select_no && list_select_no < ci.lst_saveload.get_item_cnt())	{
		ci.lst_saveload.set_item_focus(list_select_no, true);
		ci.lst_saveload.set_item_select(list_select_no, true);
	}

	// �E�B���h�E�^�C�g���ݒ�
	if (func_mod == SYS_WND_FUNC_MOD_SOLO_SAVE)	{	p_dlg->set_text(Gp_ini->config.save_info.dlgwnd_caption_title_str);	}
	else										{	p_dlg->set_text(Gp_ini->config.load_info.dlgwnd_caption_title_str);	}

	// �Z�[�u�{�^���̋֎~�E����
	set_save_bnt_enable();

	// �t�H�[�J�X
	ci.lst_saveload.set_focus();

	// �_�C�A���O���X�V
	update_dialog();
}

// ����
void C_sys_wnd_func_saveload::on_close_funcfunc()
{
	ci.tab_saveload.save_ini_int(_T("TAB_SELECT_NO"), ci.tab_saveload.get_selected_item_no());
	ci.lst_saveload.save_ini_int(_T("LIST_TOP_NO"), ci.lst_saveload.get_item_top_no());
	ci.lst_saveload.save_ini_int(_T("LIST_SELECT_NO"), ci.lst_saveload.get_selected_item_no());
	if (modal_mode_flag)	{
		C_ini_file::body().save_int(_T("TAB_SELECT_NO"), ci.tab_saveload.get_selected_item_no(), SAVELOAD_MODAL_DIALOG_INI_NAME);
		C_ini_file::body().save_int(_T("LIST_TOP_NO"), ci.lst_saveload.get_item_top_no(), SAVELOAD_MODAL_DIALOG_INI_NAME);
		C_ini_file::body().save_int(_T("LIST_SELECT_NO"), ci.lst_saveload.get_selected_item_no(), SAVELOAD_MODAL_DIALOG_INI_NAME);
	}
}

// �R�}���h
bool C_sys_wnd_func_saveload::on_command_funcfunc(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{
		case IDC_CHK_WARNING:
			Gp_config->system.saveload_alert_flag = ci.chk_warning.get_check();
			return true;

		case IDC_CHK_DBLCLICK:
			Gp_config->system.saveload_dblclick_flag = ci.chk_dblclick.get_check();
			return true;

		case IDC_BTN_NEW:
			{
				int new_save_no = tnm_get_new_save_no();	// �ŐV�̃Z�[�u�f�[�^�ԍ�
				if (0 <= new_save_no && new_save_no < Gp_ini->save_cnt)	{
					int tab_no = new_save_no / work.data_cnt_par_page;
					if (0 <= tab_no && tab_no < work.page_cnt)	{
						ci.tab_saveload.set_selected_item_no(tab_no);
						create_data_list();
						set_save_bnt_enable();		// �Z�[�u�{�^���̋֎~�E����
						int list_no = new_save_no - work.data_top_no;
						ci.lst_saveload.set_item_focus(list_no, true);
						ci.lst_saveload.set_item_select(list_no, true);
						ci.lst_saveload.set_item_visible(list_no);
					}
				}
			}
			return true;

		case IDC_BTN_SAVELOAD:
			execute_saveload();
			return true;

		case IDC_BTN_CLOSE:
			p_dlg->close();
			return true;
	}
	return false;
}

// �ʒm
bool C_sys_wnd_func_saveload::on_notify_funcfunc(WPARAM wp, LPARAM lp)
{
	NMHDR* hdr = (NMHDR *)lp;

	// �^�u�R���g���[��
	if (hdr->hwndFrom == ci.tab_saveload.get_handle())	{
		switch (hdr->code)	{
			case TCN_SELCHANGE:
				create_data_list();
				set_save_bnt_enable();		// �Z�[�u�{�^���̋֎~�E����
				return true;
		}
	}

	// ���X�g�r���[
	else if (hdr->hwndFrom == ci.lst_saveload.get_handle())	{
		NMLISTVIEW* lv = (NMLISTVIEW *)lp;
		if (lv->hdr.code == LVN_ITEMCHANGED)	{
		}
		else if (lv->hdr.code == NM_DBLCLK)	{
			if (!work.data_header_list.empty() && Gp_config->system.saveload_dblclick_flag)	{
				execute_saveload_func(work.data_top_no + lv->iItem);
			}
		}
		else if (lv->hdr.code == NM_CLICK)	{
			set_save_bnt_enable();			// �Z�[�u�{�^���̋֎~�E����
		}
	}

	return false;
}

// �}�E�X�z�C�[��
bool C_sys_wnd_func_saveload::on_mousewheel_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �g�X�N���[��
bool C_sys_wnd_func_saveload::on_h_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �u�X�N���[��
bool C_sys_wnd_func_saveload::on_v_scroll_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return false;
}

// �X�^�e�B�b�N�R���g���[���̃J���[�ύX
LRESULT C_sys_wnd_func_saveload::on_ctlcolorstatic_funcfunc(WPARAM wp, LPARAM lp)
{
//	if (false);
	return NULL;
}

// �t���[��
void C_sys_wnd_func_saveload::frame_funcfunc()
{
	if (!h_wnd)	{	return;	}

	// �Z�[�u�ł��Ȃ����́A�Z�[�u�{�^�����֎~��Ԃɂ���
	set_save_bnt_enable();

	// �O���ŕύX���ꂽ
	if (Gp_config->system.saveload_alert_flag != ci.chk_warning.get_check())	{
		ci.chk_warning.set_check(Gp_config->system.saveload_alert_flag);
	}
	if (Gp_config->system.saveload_dblclick_flag != ci.chk_dblclick.get_check())	{
		ci.chk_dblclick.set_check(Gp_config->system.saveload_dblclick_flag);
	}

	// �ŐV�̃Z�[�u�f�[�^�ԍ�
	int new_save_no = tnm_get_new_save_no();

	// new �{�^��
	bool btn_new_enable = (new_save_no != -1) ? true : false;
	if (ci.btn_new.get_enable() != btn_new_enable)	{
		ci.btn_new.set_enable(btn_new_enable);
	}

	// �f�[�^���X�g�̍X�V
	if (work.data_header_list.empty())	{	return;	}
	TSTR str;
	S_tnm_save_header header;
	int data_no = work.data_top_no;
	for (int list_index = 0; list_index < (int)work.data_header_list.size(); list_index++)	{
		if (tnm_load_save_header(data_no, &header))	{
			// �f�[�^�����݂���
			if (work.data_header_exist[list_index] == false || work.data_header_list[list_index] != header)	{
				str = create_data_list_str(data_no, &header);
				ci.lst_saveload.set_cell_text(list_index, 1, str);
				work.data_header_list[list_index] = header;
				work.data_header_exist[list_index] = true;
			}
		}
		else	{
			// �f�[�^�����݂��Ȃ�
			if (work.data_header_exist[list_index] == true)	{
				str = create_data_list_str(data_no, NULL);
				ci.lst_saveload.set_cell_text(list_index, 1, str);
				work.data_header_exist[list_index] = false;
			}
		}
		data_no++;
	}

	// �ŐV�̃Z�[�u�f�[�^�ԍ�
	if (work.new_save_data_no != new_save_no)	{
		if (work.new_save_data_no != -1)	{
			if (work.data_top_no <= work.new_save_data_no && work.new_save_data_no < work.data_end_no)	{
				ci.lst_saveload.set_cell_text(work.new_save_data_no - work.data_top_no, 0, _T(""));
			}
		}
		work.new_save_data_no = new_save_no;
		if (work.new_save_data_no != -1)	{
			if (work.data_top_no <= work.new_save_data_no && work.new_save_data_no < work.data_end_no)	{
				ci.lst_saveload.set_cell_text(work.new_save_data_no - work.data_top_no, 0, _T("new"));
			}
		}
	}

	// �ŐV�̃^�u�ԍ�
	int new_tab_no = -1;
	if (0 <= work.new_save_data_no && work.new_save_data_no < Gp_ini->save_cnt)	{
		int tab_no = work.new_save_data_no / work.data_cnt_par_page;
		if (0 <= tab_no && tab_no < work.page_cnt)	{
			new_tab_no = tab_no;
		}
	}
	if (work.new_tab_page_no != new_tab_no)	{
		if (work.new_tab_page_no != -1)	{
			ci.tab_saveload.set_item_text(work.new_tab_page_no, _T(" ") + tostr(work.new_tab_page_no + 1));
		}
		work.new_tab_page_no = new_tab_no;
		if (work.new_tab_page_no != -1)	{
			ci.tab_saveload.set_item_text(work.new_tab_page_no, _T(" ") + tostr(work.new_tab_page_no + 1) + _T(" *"));
		}
	}
}

// ������Ԃɖ߂�
void C_sys_wnd_func_saveload::init_state()
{
	tnm_init_config_system_saveload_alert();
	tnm_init_config_system_saveload_dblclick();
	update_dialog();
}

// �_�C�A���O�X�V
void C_sys_wnd_func_saveload::update_dialog()
{
	if (!h_wnd)	{	return;	}
	ci.chk_warning.set_check(Gp_config->system.saveload_alert_flag);
	ci.chk_dblclick.set_check(Gp_config->system.saveload_dblclick_flag);
}

// ****************************************************************
// �f�[�^���X�g�쐬
// ================================================================
void C_sys_wnd_func_saveload::create_data_list()
{
	int tab_no = ci.tab_saveload.get_selected_item_no();
	if (!(0 <= tab_no && tab_no < work.page_cnt))	{	tab_no = 0;	}

	if (!work.data_header_list.empty() && work.now_page_no == tab_no)	{	return;	}
	work.now_page_no = tab_no;
	ci.lst_saveload.clear();
	work.data_header_list.clear();
	work.data_header_exist.clear();

	work.data_top_no = tab_no * work.data_cnt_par_page;
	work.data_end_no = work.data_top_no + work.data_cnt_par_page;

	if (work.data_end_no > Gp_ini->save_cnt)	{	work.data_end_no = Gp_ini->save_cnt;	}

	work.data_header_list.resize(work.data_end_no - work.data_top_no);
	work.data_header_exist.resize(work.data_end_no - work.data_top_no);

	// �ŐV�̃Z�[�u�f�[�^�ԍ�
	work.new_save_data_no = tnm_get_new_save_no();

	TSTR str;
	S_tnm_save_header *p_header;
	int list_index = 0;
	for (int data_no = work.data_top_no; data_no < work.data_end_no; data_no++)	{
		p_header = &work.data_header_list[list_index];
		if (tnm_load_save_header(data_no, p_header))	{
			// �f�[�^�����݂���
			str = create_data_list_str(data_no, p_header);
			work.data_header_exist[list_index] = true;
		}
		else	{
			// �f�[�^�����݂��Ȃ�
			str = create_data_list_str(data_no, NULL);
			work.data_header_exist[list_index] = false;
		}
		ci.lst_saveload.add_item(0, _T(""));
		if (data_no == work.new_save_data_no)	{
			ci.lst_saveload.set_cell_text(list_index, 0, _T("New"));
		}
		ci.lst_saveload.set_cell_text(list_index, 1, str);
		list_index++;
	}
}

// ****************************************************************
// �Z�[�u�{�^���̋֎~�E����
// ================================================================
void C_sys_wnd_func_saveload::set_save_bnt_enable()
{
	int list_no = ci.lst_saveload.get_selected_item_no();
	bool enable_flag = false;
	if (list_no != -1)	{
		if (func_mod == SYS_WND_FUNC_MOD_SOLO_SAVE)	{
			enable_flag = tnm_syscom_open_save_is_enable();
		}
		else	{
			int data_no = work.data_top_no + list_no;
			enable_flag = (tnm_check_save_file(data_no)) ? true : false;
		}
	}
	if (ci.btn_saveload.get_enable() != enable_flag)	{
		ci.btn_saveload.set_enable(enable_flag);
	}
}

// ****************************************************************
// �f�[�^���X�g�̕�����쐬
// ================================================================

TSTR date_time_str(S_tnm_save_header* p_header)
{
	return LCL_DATE_FORMAT;
}

TSTR C_sys_wnd_func_saveload::create_data_list_str(int data_no, S_tnm_save_header *p_header)
{
	TSTR str;
	if (func_mod == SYS_WND_FUNC_MOD_SOLO_SAVE)	{	str = Gp_ini->config.save_info.dlgwnd_datalist_nameheader_str;	}
	else										{	str = Gp_ini->config.load_info.dlgwnd_datalist_nameheader_str;	}
	switch (work.data_index_keta_mode)	{
		case 0:		str += str_format(_T(" [ %02d ] �F "), data_no);	break;
		case 1:		str += str_format(_T(" [ %03d ] �F "), data_no);	break;
		default:	str += str_format(_T(" [ %04d ] �F "), data_no);	break;
	}
	if (p_header)	{
		str += date_time_str(p_header);
		str += str_format(_T(" %s"), p_header->title);
	}
	else	{
		str += Gp_ini->save_no_data_str;
	}
	return str;
}

// ****************************************************************
// �Z�[�u���[�h�̎��s
// ================================================================

void C_sys_wnd_func_saveload::execute_saveload()
{
	if (!work.data_header_list.empty())	{
		int list_no = ci.lst_saveload.get_selected_item_no();
		if (list_no != -1)	{
			execute_saveload_func(work.data_top_no + list_no);
		}
	}
}

void C_sys_wnd_func_saveload::execute_saveload_func(int data_no)
{
	if (data_no < 0 || data_no >= Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt)	{	return;	}

	if (modal_mode_flag)	{
		select_return_value = data_no;
		p_dlg->send_message(WM_CLOSE, 0, 0);
	}
	else	{
		if (func_mod == SYS_WND_FUNC_MOD_SOLO_SAVE)	{	tnm_syscom_save(data_no, true, true);			}
		else										{	tnm_syscom_load(data_no, true, true, true);		}
	}
}

// ****************************************************************
// ���X�g���t�H�[�J�X
// ================================================================
void C_sys_wnd_func_saveload::set_focus_list(bool up_down_flag)
{
	int now_list_no = ci.lst_saveload.get_selected_item_no();
	int list_no = now_list_no;
	if (list_no == -1)	{
		list_no = 0;
	}
	else	{
		if (up_down_flag)	{
			list_no++;
			if (list_no >= ci.lst_saveload.get_item_cnt())	{
				list_no = 0;
			}
		}
		else	{
			list_no--;
			if (list_no < 0)	{
				list_no = ci.lst_saveload.get_item_cnt() - 1;
			}
		}
	}
	if (now_list_no != list_no)	{
		ci.lst_saveload.set_item_focus(list_no, true);
		ci.lst_saveload.set_item_select(list_no, true);
		ci.lst_saveload.set_item_visible(list_no);
	}
	if (GetFocus() != ci.lst_saveload.get_handle())	{
		ci.lst_saveload.set_focus();

		// ���{�^���̋����\���������i�{�^���Ƀt�H�[�J�X�������Ԃ���A�ʂ̃A�C�e���Ƀt�H�[�J�X���ڂ��ƁA�t�H�[�J�X��Ԃɂ������{�^���������\�����ꂽ�܂܂ɂȂ�����������ׁj
		ci.btn_new.send_message(BM_SETSTYLE, BS_PUSHBUTTON, TRUE);
		ci.btn_saveload.send_message(BM_SETSTYLE, BS_PUSHBUTTON, TRUE);
		ci.btn_close.send_message(BM_SETSTYLE, BS_PUSHBUTTON, TRUE);
	}
}

// ****************************************************************
// �^�u���t�H�[�J�X
// ================================================================
void C_sys_wnd_func_saveload::set_focus_tab(bool left_right_flag)
{
	int now_tab_no = ci.tab_saveload.get_selected_item_no();
	int tab_no = now_tab_no;
	if (tab_no == -1)	{
		tab_no = 0;
	}
	else	{
		if (left_right_flag)	{
			tab_no++;
			if (tab_no >= ci.tab_saveload.get_item_cnt())	{
				tab_no = 0;
			}
		}
		else	{
			tab_no--;
			if (tab_no < 0)	{
				tab_no = ci.tab_saveload.get_item_cnt() - 1;
			}
		}
	}
	if (now_tab_no != tab_no)	{
		ci.tab_saveload.set_selected_item_no(tab_no);
		create_data_list();
		set_save_bnt_enable();		// �Z�[�u�{�^���̋֎~�E����
	}
	if (GetFocus() != ci.tab_saveload.get_handle())	{
		ci.tab_saveload.set_focus();

		// ���{�^���̋����\���������i�{�^���Ƀt�H�[�J�X�������Ԃ���A�ʂ̃A�C�e���Ƀt�H�[�J�X���ڂ��ƁA�t�H�[�J�X��Ԃɂ������{�^���������\�����ꂽ�܂܂ɂȂ�����������ׁj
		ci.btn_new.send_message(BM_SETSTYLE, BS_PUSHBUTTON, TRUE);
		ci.btn_saveload.send_message(BM_SETSTYLE, BS_PUSHBUTTON, TRUE);
		ci.btn_close.send_message(BM_SETSTYLE, BS_PUSHBUTTON, TRUE);
	}
}

// ****************************************************************
// �G�X�P�[�v�L�[
// ================================================================
void C_sys_wnd_func_saveload::escape_key_func()
{
	if (modal_mode_flag)	{
		p_dlg->close();
	}
}

// ****************************************************************
// �J�[�\���R���g���[��
// ================================================================
void C_sys_wnd_func_saveload::cursor_control(WPARAM wp)
{
	if (wp == VK_UP)	{	set_focus_list(false);	return;	}
	if (wp == VK_DOWN)	{	set_focus_list(true);	return;	}
	if (wp == VK_LEFT)	{	set_focus_tab(false);	return;	}
	if (wp == VK_RIGHT)	{	set_focus_tab(true);	return;	}
}

// ****************************************************************
// GETDLGCODE
// ================================================================
bool C_sys_wnd_func_saveload::on_getdlgcode(WPARAM wp, LPARAM lp, LRESULT *ret)
{
	if (lp == NULL)	{	return false;	}

	MSG *msg = (MSG *)lp;
	if (msg->message == WM_KEYDOWN)	{
		if (msg->wParam == VK_UP || msg->wParam == VK_DOWN || msg->wParam == VK_LEFT || msg->wParam == VK_RIGHT)	{
			cursor_control(msg->wParam);
			*ret = DLGC_WANTARROWS;
			return true;
		}
	}
	return false;
}

// ****************************************************************
// �Z�[�u���[�h�^�u�F�E�B���h�E�v���V�[�W��
// ================================================================
LRESULT C_sys_wnd_func_saveload::Cc_tab_saveload::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_KEYDOWN:
			if (wp == VK_UP || wp == VK_DOWN || wp == VK_LEFT || wp == VK_RIGHT)	{
				parent_class->cursor_control(wp);
				return TRUE;
			}
			break;
		case WM_KEYUP:
			if (wp == VK_ESCAPE)	{
				parent_class->escape_key_func();
			}
			break;
	}
	return Cc_tab::window_proc(msg, wp, lp);
}

// ****************************************************************
// �Z�[�u���[�h���X�g�F�E�B���h�E�v���V�[�W��
// ================================================================
LRESULT C_sys_wnd_func_saveload::Cc_lst_saveload::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{

		// case WM_SIZE:
		// ���d�v�Ȏ��Ȃ̂ŁA�����Ƃ��Ďc���Ă����܂��B
		// �����ڃT�C�Y�̒����� WM_SIZE �ŏ�������ׂ����Ǝv���̂ł����A�Ƃ�������Ń��X�g�̕\�����o�O��܂��B
		// ���Č����@�F���X�g�����܂ŃX�N���[�������遨�E�B���h�E�̏c�T�C�Y���L���遨�Ȃ������X�g�̃g�b�v�|�W�V�������������Ȏ��Ɂc
		// �������F���L�̂悤�� WM_PAINT �ŏ������ĉ������B
		// ���R���p�C���� dialog.cpp �ł����l�̕��@�Ŗ���������Ă��܂��B

		case WM_PAINT:
			set_column_width_just(1, false);
			break;
		case WM_KEYDOWN:
			if (wp == VK_UP || wp == VK_DOWN || wp == VK_LEFT || wp == VK_RIGHT)	{
				parent_class->cursor_control(wp);
				return TRUE;
			}
			break;
		case WM_KEYUP:
			if (wp == VK_RETURN)	{
				parent_class->execute_saveload();
			}
			else if (wp == VK_ESCAPE)	{
				parent_class->escape_key_func();
			}
			break;
	}
	return Cc_listview::window_proc(msg, wp, lp);
}

// ****************************************************************
// �Z�[�u���[�h�֘A�̃`�F�b�N�{�b�N�X�F�E�B���h�E�v���V�[�W��
// ================================================================
LRESULT C_sys_wnd_func_saveload::Cc_chk_saveload::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_GETDLGCODE:
			{
				LRESULT res;
				if (parent_class->on_getdlgcode(wp, lp, &res))	{	return res;	}
			}
			break;

		case WM_KEYUP:
			if (wp == VK_ESCAPE)	{
				parent_class->escape_key_func();
			}
			break;
	}
	return Cc_check::window_proc(msg, wp, lp);
}


// ****************************************************************
// �Z�[�u���[�h�֘A�̃{�^���F�E�B���h�E�v���V�[�W��
// ================================================================
LRESULT C_sys_wnd_func_saveload::Cc_btn_saveload::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_GETDLGCODE:
			{
				LRESULT res;
				if (parent_class->on_getdlgcode(wp, lp, &res))	{	return res;	}
			}
			break;

		case WM_KEYUP:
			if (wp == VK_ESCAPE)	{
				parent_class->escape_key_func();
			}
			break;
	}
	return Cc_button::window_proc(msg, wp, lp);
}


