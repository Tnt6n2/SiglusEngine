#include	"pch.h"
#include	"../resource.h"

#include	"main.h"
#include	"data.h"
#include	"dialog.h"
#include	"project_setting_dialog.h"

// ****************************************************************
// �J���i�쐬�j
// ================================================================
bool C_tnm_project_setting_dialog::open(HWND parent_h_wnd, PROJECT_SETTING_MODE proc_mode)
{
	work.proc_mode = proc_mode;
	work.return_value = false;
	create(IDD_PROJECT_SETTING, _T("project_setting_dialog"), true, parent_h_wnd);
	return work.return_value;
}

// ****************************************************************
// �E�B���h�E�v���V�[�W��
// ================================================================
LRESULT	C_tnm_project_setting_dialog::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_INITDIALOG:		if (!on_init(wp, lp))		return FALSE;	break;
		case WM_CLOSE:			if (!on_close(wp, lp))		return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))	return FALSE;	break;
	}

	return C_modal_dialog::window_proc(msg, wp, lp);
}

// ****************************************************************
// ����������
// ================================================================

#define			DEFAULT_NEW_PROJECT_NAME_STR			_T("���V�K�v���W�F�N�g��")

bool C_tnm_project_setting_dialog::on_init(WPARAM wp, LPARAM lp)
{
	// �A�C�e���o�^
	bind_control(&ci.edt_project_name, IDC_EDT_PROJECT_NAME, _T(""), 8);
	bind_control(&ci.edt_scn_path, IDC_EDT_SCN_PATH, _T(""), 8);
	bind_control(&ci.edt_tmp_path, IDC_EDT_TMP_PATH, _T(""), 8);
	bind_control(&ci.edt_out_path, IDC_EDT_OUT_PATH, _T(""), 8);
	bind_control(&ci.edt_out_path_noangou, IDC_EDT_OUT_PATH_NOANGOU, _T(""), 8);
	bind_control(&ci.edt_exe_path, IDC_EDT_EXE_PATH, _T(""), 8);
	bind_control(&ci.edt_work_path, IDC_EDT_WORK_PATH, _T(""), 8);
	bind_control(&ci.edt_editor_path, IDC_EDT_EDITOR_PATH, _T(""), 8);
	bind_control(&ci.edt_manual_path, IDC_EDT_MANUAL_PATH, _T(""), 8);
	bind_control(&ci.edt_gameexe_ini, IDC_EDT_GAMEEXE_INI, _T(""), 8);
	bind_control(&ci.edt_extend_inc, IDC_EDT_EXTEND_INC, _T(""), 8);
	bind_control(&ci.edt_gameexe_dat, IDC_EDT_GAMEEXE_DAT, _T(""), 9);
	bind_control(&ci.edt_scene_pck, IDC_EDT_SCENE_PCK, _T(""), 9);
	bind_control(&ci.stc_dragdrop, IDC_STC_DRAGDROP, _T(""), 5);
	bind_control(&ci.chk_utf8, IDC_CHK_UTF8, _T(""), 1);
	bind_control(&ci.btn_ok, IDC_BTN_OK, _T(""), 2);
	bind_control(&ci.btn_cancel, IDC_BTN_CANCEL, _T(""), 1);

	// �A�C�e���ݒ�
	if (work.proc_mode == PROJECT_SETTING_MODE_CREATE)
	{
		// �V�K�v���W�F�N�g�̏ꍇ�̏����ݒ�
		ci.edt_project_name.set_text(DEFAULT_NEW_PROJECT_NAME_STR);
		ci.edt_gameexe_ini.set_text(_T("Gameexe.ini"));
		ci.edt_gameexe_dat.set_text(_T("Gameexe.dat"));
		ci.edt_scene_pck.set_text(_T("Scene.pck"));

		// �L���ȃe�L�X�g�G�f�B�^�[�̃p�X���ڐA����
		for (ARRAY<S_tnm_compiler_project_data>::iterator itr = G_data->project.begin(); itr != G_data->project.end(); ++itr)
		{
			if (file_get_type(itr->editor_path) == 1)
			{
				ci.edt_editor_path.set_text(itr->editor_path);
				break;
			}
		}

		// �L���ȃ}�j���A���̃p�X���ڐA����
		for (ARRAY<S_tnm_compiler_project_data>::iterator itr = G_data->project.begin(); itr != G_data->project.end(); ++itr)
		{
			if (file_get_type(itr->manual_path) != -1)
			{
				ci.edt_manual_path.set_text(itr->manual_path);
				break;
			}
		}
	}
	else
	{
		// �����̃v���W�F�N�g�̏ꍇ�̏����ݒ�
		ci.edt_project_name.set_text(G_data->project[G_data->cur_project_no].project_name);
		ci.edt_scn_path.set_text(G_data->project[G_data->cur_project_no].scn_path);
		ci.edt_tmp_path.set_text(G_data->project[G_data->cur_project_no].tmp_path);
		ci.edt_out_path.set_text(G_data->project[G_data->cur_project_no].out_path);
		ci.edt_out_path_noangou.set_text(G_data->project[G_data->cur_project_no].out_path_noangou);
		ci.edt_exe_path.set_text(G_data->project[G_data->cur_project_no].exe_path);
		ci.edt_work_path.set_text(G_data->project[G_data->cur_project_no].work_path);
		ci.edt_editor_path.set_text(G_data->project[G_data->cur_project_no].editor_path);
		ci.edt_manual_path.set_text(G_data->project[G_data->cur_project_no].manual_path);
		ci.edt_gameexe_ini.set_text(G_data->project[G_data->cur_project_no].gameexe_ini);
		ci.edt_extend_inc.set_text(G_data->project[G_data->cur_project_no].extend_inc);
		ci.edt_gameexe_dat.set_text(G_data->project[G_data->cur_project_no].gameexe_dat);
		ci.edt_scene_pck.set_text(G_data->project[G_data->cur_project_no].scene_pck);
		ci.chk_utf8.set_check(G_data->project[G_data->cur_project_no].utf8);
	}

	// �{�^���̕����ݒ�
	switch (work.proc_mode)
	{
		case PROJECT_SETTING_MODE_SETTING:
			ci.btn_ok.set_text(_T("�n�j �F �v���W�F�N�g�� �y�ύX�z ����"));
			set_text(_T("�v���W�F�N�g�ݒ�"));
			break;
		case PROJECT_SETTING_MODE_CREATE:
			ci.btn_ok.set_text(_T("�n�j �F �v���W�F�N�g�� �y�ǉ��z ����"));
			set_text(_T("�v���W�F�N�g�ǉ�"));
			break;
		case PROJECT_SETTING_MODE_DELETE:
			ci.btn_ok.set_text(_T("�n�j �F �v���W�F�N�g�� �y�폜�z ����"));
			set_text(_T("�v���W�F�N�g�폜"));
			// �A�C�e�����֎~��Ԃɂ���
			ci.edt_project_name.set_enable(false);
			ci.edt_scn_path.set_enable(false);
			ci.edt_tmp_path.set_enable(false);
			ci.edt_out_path.set_enable(false);
			ci.edt_out_path_noangou.set_enable(false);
			ci.edt_exe_path.set_enable(false);
			ci.edt_work_path.set_enable(false);
			ci.edt_editor_path.set_enable(false);
			ci.edt_manual_path.set_enable(false);
			ci.stc_dragdrop.set_enable(false);
			ci.chk_utf8.set_enable(false);
			break;
	}

	// �h���b�O�A���h�h���b�v�A�C�e���֐e�N���X��ݒ�
	ci.stc_dragdrop.p_parent_class = this;

	// �E�B���h�E��`���[�h
	load_ini_window_rect();

	// �E�B���h�E�\��
	update();
	set_show(true);

	return true;
}

// ****************************************************************
// �N���[�Y����
// ================================================================
bool C_tnm_project_setting_dialog::on_close(WPARAM wp, LPARAM lp)
{
	// �n�j�ŏI��
	if (work.return_value)
	{
		bool update_flag = true;

		// �v���W�F�N�g�쐬�̏ꍇ
		if (work.proc_mode == PROJECT_SETTING_MODE_CREATE)
		{
			if (
				ci.edt_project_name.get_text() == DEFAULT_NEW_PROJECT_NAME_STR &&
				ci.edt_scn_path.get_text() == _T("") &&
				ci.edt_tmp_path.get_text() == _T("") &&
				ci.edt_out_path.get_text() == _T("") &&
				ci.edt_out_path_noangou.get_text() == _T("") &&
				ci.edt_exe_path.get_text() == _T("") &&
				ci.edt_work_path.get_text() == _T("")
			)	{
				// �ݒ肪�����̂ŃL�����Z������
				update_flag = false;	// �X�V���Ȃ�
			}
			else
			{
				// �v���W�F�N�g��ǉ����đI������
				G_data->project.resize(G_data->project.size() + 1);
				G_data->cur_project_no = (int)G_data->project.size() - 1;
			}
		}

		// �v���W�F�N�g�폜�̏ꍇ
		if (work.proc_mode == PROJECT_SETTING_MODE_DELETE)
		{
			// �v���W�F�N�g���폜���ĂP�O��I������
			G_dlg->delete_ini_project(_T(""), _T("project_") + tostr(G_data->cur_project_no));	// ini ����폜
			G_data->project.erase(G_data->project.begin() + G_data->cur_project_no);
			G_data->cur_project_no--;
			update_flag = false;		// �X�V���Ȃ�
		}

		// �v���W�F�N�g�̍X�V
		if (update_flag)
		{
			G_data->project[G_data->cur_project_no].project_name = ci.edt_project_name.get_text();
			G_data->project[G_data->cur_project_no].scn_path = ci.edt_scn_path.get_text();
			G_data->project[G_data->cur_project_no].tmp_path = ci.edt_tmp_path.get_text();
			G_data->project[G_data->cur_project_no].out_path = ci.edt_out_path.get_text();
			G_data->project[G_data->cur_project_no].out_path_noangou = ci.edt_out_path_noangou.get_text();
			G_data->project[G_data->cur_project_no].exe_path = ci.edt_exe_path.get_text();
			G_data->project[G_data->cur_project_no].work_path = ci.edt_work_path.get_text();
			G_data->project[G_data->cur_project_no].editor_path = ci.edt_editor_path.get_text();
			G_data->project[G_data->cur_project_no].manual_path = ci.edt_manual_path.get_text();
			G_data->project[G_data->cur_project_no].gameexe_ini = ci.edt_gameexe_ini.get_text();
			G_data->project[G_data->cur_project_no].extend_inc = ci.edt_extend_inc.get_text();
			G_data->project[G_data->cur_project_no].gameexe_dat = ci.edt_gameexe_dat.get_text();
			G_data->project[G_data->cur_project_no].scene_pck = ci.edt_scene_pck.get_text();
			G_data->project[G_data->cur_project_no].utf8 = ci.chk_utf8.get_check();

			if (work.proc_mode == PROJECT_SETTING_MODE_CREATE)
			{
				G_data->project[G_data->cur_project_no].start_check = false;
				G_data->project[G_data->cur_project_no].start_scene = _T("");
			}
		}
	}

	// �E�B���h�E��`�Z�[�u
	save_ini_window_rect();

	return true;
}

// ****************************************************************
// �R�}���h����
// ================================================================
bool C_tnm_project_setting_dialog::on_command(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))	{
		case IDC_STC_DRAGDROP:
			break;
		case IDC_BTN_OK:
			work.return_value = true;
			close();
			break;
		case IDC_BTN_CANCEL:
			close();
			break;
	}
	return true;
}

// ****************************************************************
// �h���b�O�A���h�h���b�v�ňꊇ�p�X�ݒ�
// ================================================================
void C_tnm_project_setting_dialog::set_path_easy(TSTR path)
{
	if (file_get_type(path) != 0)	{	return;	}

	// �r�r�p�X
	TSTR find_path;
	find_path = path + _T("\\ss");
	if (file_get_type(find_path) == 0)	{
		ci.edt_scn_path.set_text(find_path);
	}

	// �s�l�o�p�X
	find_path = path + _T("\\tmp");
	if (file_get_type(find_path) == 0)	{
		ci.edt_tmp_path.set_text(find_path);
	}

	// �o�̓p�X
	find_path = path + _T("\\exe\\setupdata\\gamedata");
	if (file_get_type(find_path) == 0)	{
		ci.edt_out_path.set_text(find_path);
		set_path_easy_for_exe_path(find_path);			// �d�w�d�p�X
	}
	else	{
		find_path = path + _T("\\exe");
		if (file_get_type(find_path) == 0)	{
			ci.edt_out_path.set_text(find_path);
			set_path_easy_for_exe_path(find_path);		// �d�w�d�p�X
		}
	}

	// ��ƃp�X
	find_path = path + _T("\\work");
	if (file_get_type(find_path) == 0)	{
		ci.edt_work_path.set_text(find_path);
	}
}

// ****************************************************************
// �h���b�O�A���h�h���b�v�ňꊇ�p�X�ݒ�i�d�w�d�p�X�p�j
// ================================================================
void C_tnm_project_setting_dialog::set_path_easy_for_exe_path(TSTR path)
{
		TSTR exe_path;
#ifdef _DEBUG
		exe_path = path + _T("\\SiglusEngine_d.exe");
		if (file_get_type(exe_path) == 1)	{
			ci.edt_exe_path.set_text(exe_path);
		}
		else	{
			exe_path = path + _T("\\SiglusEngine.exe");
			if (file_get_type(exe_path) == 1)	{
				ci.edt_exe_path.set_text(exe_path);
			}
			else	{
				exe_path = path + _T("\\SiglusEngine_t.exe");
				if (file_get_type(exe_path) == 1)	{
					ci.edt_exe_path.set_text(exe_path);
				}
			}
		}
#elif _TEST
		exe_path = path + _T("\\SiglusEngine_t.exe");
		if (file_get_type(exe_path) == 1)	{
			ci.edt_exe_path.set_text(exe_path);
		}
		else	{
			exe_path = path + _T("\\SiglusEngine.exe");
			if (file_get_type(exe_path) == 1)	{
				ci.edt_exe_path.set_text(exe_path);
			}
			else	{
				exe_path = path + _T("\\SiglusEngine_d.exe");
				if (file_get_type(exe_path) == 1)	{
					ci.edt_exe_path.set_text(exe_path);
				}
			}
		}
#else
		exe_path = path + _T("\\SiglusEngine.exe");
		if (file_get_type(exe_path) == 1)	{
			ci.edt_exe_path.set_text(exe_path);
		}
		else	{
			exe_path = path + _T("\\SiglusEngine_d.exe");
			if (file_get_type(exe_path) == 1)	{
				ci.edt_exe_path.set_text(exe_path);
			}
			else	{
				exe_path = path + _T("\\SiglusEngine_t.exe");
				if (file_get_type(exe_path) == 1)	{
					ci.edt_exe_path.set_text(exe_path);
				}
			}
		}
#endif
}

// ****************************************************************
// �E�B���h�E�v���V�[�W��
// ================================================================
LRESULT	C_tnm_project_setting_dialog::Cc_stc_dragdrop::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_DROPFILES:
			p_parent_class->set_path_easy(get_droped_link_path(wp));
			break;
	}

	return Cc_static::window_proc(msg, wp, lp);
}
