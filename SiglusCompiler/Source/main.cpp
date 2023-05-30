#include	"pch.h"
#include	"tona3_main.h"
#include	"data.h"
#include	"dialog.h"
#include	"project_setting_dialog.h"
#include	"main.h"
#include	"exe_angou.h"
#include	"tnm_common_header.h"
#include	"BS.h"

#include <boost/algorithm/string.hpp>

#include	"../resource.h"

C_tnm_compiler_main*	G_main;
C_tnm_compiler_data*	G_data;
C_tnm_compiler_dialog*	G_dlg;

#ifdef __SIGLUS
const TCHAR* const TNMC_WINDOW_TITLE_STR = _T("Siglus �X�N���v�g�R���p�C��");
const TCHAR* const TNMC_WINDOW_CLASS_STR = _T("visualarts_siglus_compiler_main_dialog_class");
#endif
#ifdef __KINETIC
const TCHAR* const TNMC_WINDOW_TITLE_STR = _T("Flix �X�N���v�g�R���p�C��");
const TCHAR* const TNMC_WINDOW_CLASS_STR = _T("visualarts_siglus_compiler_main_dialog_class");	// �N���X Siglus / Flix ����
#endif

// ****************************************************************
// ���C��
// ================================================================
int tona_main()
{
	// �����ݒ�
	G_app.app_name = _T("siglus_compiler");		// �A�v������ Siglus / Flix ����

	// GUI �t���O
	bool cl_cui_set = false;
	bool cl_project_no_set = false;
	bool cl_log_set = false;
	bool cl_full_set = false;
	int cl_project_no = -1;
	TSTR cl_log;

	// �R�}���h���C�����烂�[�h���擾
	C_str_analizer cmd_anl(G_app.command_line.begin(), G_app.command_line.end());
	while (1)	{
		TSTR str;
		int num = 0;
		if (cmd_anl.check_moji(_T('/')))	{
			if (cmd_anl.check_str(_T("cui")))	{
				cl_cui_set = true;
			}
			else if (cmd_anl.check_str(_T("project_no")))	{
				if (cmd_anl.check_moji(_T('=')))	{
					if (cmd_anl.get_integer(&num))	{
						cl_project_no_set = true;
						cl_project_no = num;
					}
				}
			}
			else if (cmd_anl.check_str(_T("log")))	{
				if (cmd_anl.check_moji(_T('=')))	{
					if (cmd_anl.get_dquote_str(str, _T('\\')))	{
						cl_log_set = true;
						cl_log = str;
					}
				}
			}
			else if (cmd_anl.check_str(_T("full")))	{
				cl_full_set = true;
			}
			else	{
				break;
			}
		}
		else	{
			break;
		}
	}

	// �~���[�e�b�N�X�𗘗p���đ��d�N���h�~
	C_mutex mutex;
	if (!mutex.create_mutex(_T("siglus_compiler")))	{
		message_box(_T("�~���[�e�b�N�X�̍쐬�Ɏ��s���܂����B"), TNMC_WINDOW_TITLE_STR, MB_ICONWARNING);
		return EXIT_FAILURE;
	}
	else if (::GetLastError() == ERROR_ALREADY_EXISTS)	{

		// CUI ���[�h�̏ꍇ�̓G���[
		if (cl_cui_set)	{

			message_box(_T("SiglusCompiler.exe �͊��ɋN�����Ă��܂��B"), TNMC_WINDOW_TITLE_STR, MB_ICONWARNING);
			return EXIT_FAILURE;
		}
		// GUI ���[�h�̏ꍇ�́A�R���p�C���������ăA�N�e�B�u�ɂ���
		else	{

			// ���ɋN�����Ă��� Siglus �R���p�C������������
			HWND another_h_wnd = ::FindWindow(TNMC_WINDOW_CLASS_STR, NULL);
			if (another_h_wnd)	{
				SetForegroundWindow(another_h_wnd);

				// ���g�͏I������
				return EXIT_FAILURE;
			}
		}
	}

	// �^�C�}�[�̐��x���グ��
	TIMECAPS caps = {0};
	timeGetDevCaps(&caps, sizeof(caps));
	timeBeginPeriod(caps.wPeriodMin);

	// �����ݒ�
	C_ini_file::body().load_ini_file();

	// ���C���̍\�z
	C_tnm_compiler_main main;		G_main = &main;
	C_tnm_compiler_data data;		G_data = &data;
	C_tnm_compiler_dialog dialog;	G_dlg = &dialog;

	// GUI �t���O��ݒ�
	G_data->gui_flag = !cl_cui_set;
	G_data->cl_project_no_set = cl_project_no_set;
	G_data->cl_project_no = cl_project_no;
	G_data->cl_log_set = cl_log_set;
	G_data->cl_log = cl_log;
	G_data->cl_full_set = cl_full_set;

	// �Ǝ��̃_�C�A���O�N���X��o�^����
	WNDCLASS wc;
	::GetClassInfo(G_app.h_inst, _T("#32770"), &wc);	// �_�C�A���O�̃N���X�����擾����
	wc.lpszClassName = TNMC_WINDOW_CLASS_STR;			// �N���X�������ς����N���X��
	if (!::RegisterClass(&wc))	{						// �Ǝ��̃_�C�A���O�N���X�Ƃ��ēo�^����I
		set_last_error_win(_T("RegisterClass"));
		error_box(_T("���C���_�C�A���O�̃N���X�̓o�^�Ɏ��s���܂����B\n") + get_last_error_msg());
	}
	else	{

		// �_�C�A���O�̍쐬
		if (!G_dlg->create(IDD_TNM_COMPILER, _T("main_dialog"), true, true))	{
			error_box(_T("���C���_�C�A���O�̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		}
		else	{

			// ���b�Z�[�W���[�v
			while (G_app.default_message_loop())
				Sleep(1);

			// �I������
			C_ini_file::body().save_ini_file();
		}

		// �Ǝ��̃_�C�A���O�N���X�𖕏�����
		::UnregisterClass(TNMC_WINDOW_CLASS_STR, G_app.h_inst);
	}

	// �^�C�}�[�̐��x�����ɖ߂�
	timeEndPeriod(caps.wPeriodMin);

	return EXIT_SUCCESS;
}

// ****************************************************************
// �R���X�g���N�^
// ================================================================
C_tnm_compiler_main::C_tnm_compiler_main()
{
	// �v���Z�X�t���O
	proc_execut_flag = false;	// �v���Z�X���s���t���O
	proc_cancel_flag = false;	// �v���Z�X�L�����Z���t���O
}

// ****************************************************************
// ini �Z�[�u
// ================================================================
void C_tnm_compiler_main::save_ini()
{
	// �v���W�F�N�g�֘A�̃A�C�e����Ԃ��擾
	get_project_item_state();

	// �v���W�F�N�g�̐ݒ�
	G_dlg->save_ini_int(_T("project_cnt"), (int)G_data->project.size());
	G_dlg->save_ini_int(_T("cur_project_no"), G_data->cur_project_no);
	for (int i = 0; i < (int)G_data->project.size(); ++i)
	{
		G_dlg->save_ini_str(_T("project_name"), G_data->project[i].project_name, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("scn_path"), G_data->project[i].scn_path, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("tmp_path"), G_data->project[i].tmp_path, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("out_path"), G_data->project[i].out_path, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("out_path_noangou"), G_data->project[i].out_path_noangou, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("exe_path"), G_data->project[i].exe_path, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("work_path"), G_data->project[i].work_path, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("editor_path"), G_data->project[i].editor_path, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("manual_path"), G_data->project[i].manual_path, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("build_after"), G_data->project[i].build_after, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("angou_str"), G_data->project[i].angou_str, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("gameexe_ini"), G_data->project[i].gameexe_ini, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("extend_inc"), G_data->project[i].extend_inc, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("gameexe_dat"), G_data->project[i].gameexe_dat, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("scene_pck"), G_data->project[i].scene_pck, _T("project_") + tostr(i));
		G_dlg->save_ini_str(_T("start_scene"), G_data->project[i].start_scene, _T("project_") + tostr(i));

		G_dlg->save_ini_int(_T("start_z_no"), G_data->project[i].start_z_no, _T("project_") + tostr(i));

		G_dlg->save_ini_bool(_T("start_check"), G_data->project[i].start_check, _T("project_") + tostr(i));
		G_dlg->save_ini_bool(_T("angou_check"), G_data->project[i].angou_check, _T("project_") + tostr(i));
		G_dlg->save_ini_bool(_T("utf8"), G_data->project[i].utf8, _T("project_") + tostr(i));
	}

	// �E�B���h�E���W
	G_dlg->save_ini_window_rect();

	// �_�C�A���O
	G_dlg->ci.dlgsep00.save_ini();
	G_dlg->ci.dlgsep01.save_ini();
	G_dlg->ci.dlgsep02.save_ini();
	G_dlg->ci.chk_debug_on.save_ini();
	G_dlg->ci.chk_debug_off.save_ini();
	G_dlg->ci.chk_execute.save_ini();
	G_dlg->ci.lv_scn.save_ini();
}

// ****************************************************************
// ini ���[�h
// ================================================================
void C_tnm_compiler_main::load_ini()
{
	// �v���W�F�N�g�̐ݒ�����[�h
	G_data->project.resize(G_dlg->load_ini_int(_T("project_cnt"), 0));
	G_data->cur_project_no = G_dlg->load_ini_int(_T("cur_project_no"), -1);
	for (int i = 0; i < (int)G_data->project.size(); ++i)
	{
		G_data->project[i].project_name = G_dlg->load_ini_str(_T("project_name"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].scn_path = G_dlg->load_ini_str(_T("scn_path"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].tmp_path = G_dlg->load_ini_str(_T("tmp_path"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].out_path = G_dlg->load_ini_str(_T("out_path"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].out_path_noangou = G_dlg->load_ini_str(_T("out_path_noangou"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].exe_path = G_dlg->load_ini_str(_T("exe_path"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].work_path = G_dlg->load_ini_str(_T("work_path"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].editor_path = G_dlg->load_ini_str(_T("editor_path"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].manual_path = G_dlg->load_ini_str(_T("manual_path"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].build_after = G_dlg->load_ini_str(_T("build_after"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].angou_str = G_dlg->load_ini_str(_T("angou_str"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].start_scene = G_dlg->load_ini_str(_T("start_scene"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].gameexe_ini = G_dlg->load_ini_str(_T("gameexe_ini"), _T("Gameexe.ini"), _T("project_") + tostr(i));
		G_data->project[i].extend_inc = G_dlg->load_ini_str(_T("extend_inc"), _T(""), _T("project_") + tostr(i));
		G_data->project[i].gameexe_dat = G_dlg->load_ini_str(_T("gameexe_dat"), _T("Gameexe.dat"), _T("project_") + tostr(i));
		G_data->project[i].scene_pck = G_dlg->load_ini_str(_T("scene_pck"), _T("Scene.pck"), _T("project_") + tostr(i));

		G_data->project[i].start_z_no = G_dlg->load_ini_int(_T("start_z_no"), 0, _T("project_") + tostr(i));

		G_data->project[i].start_check = G_dlg->load_ini_bool(_T("start_check"), false, _T("project_") + tostr(i));
		G_data->project[i].angou_check = G_dlg->load_ini_bool(_T("angou_check"), false, _T("project_") + tostr(i));
		G_data->project[i].utf8 = G_dlg->load_ini_bool(_T("utf8"), false, _T("project_") + tostr(i));
	}

	// �R�}���h���C���̏����㏑��
	if (G_data->cl_project_no_set)
	{
		G_data->cur_project_no = G_data->cl_project_no;
	}

	// �E�B���h�E���W
	G_dlg->load_ini_window_rect();

	// �_�C�A���O�̏������[�h
	G_dlg->ci.dlgsep00.load_ini();
	G_dlg->ci.dlgsep01.load_ini();
	G_dlg->ci.dlgsep02.load_ini();
	G_dlg->ci.chk_debug_on.load_ini(true);		// �f�t�H���g�Ńf�o�b�O�n�m
	G_dlg->ci.chk_debug_off.load_ini(false);
	G_dlg->ci.chk_execute.load_ini(true);
	G_dlg->ci.lv_scn.load_ini();

	// �J�����g�v���W�F�N�g��␳����
	if (G_data->project.empty())
	{
		G_data->cur_project_no = -1;	// �v���W�F�N�g����Ȃ� -1 �ɏ���������
	}
	else if (!check_current_project(false))
	{
		G_data->cur_project_no = 0;		// �ړI�̃v���W�F�N�g��������� 0 �ɕ␳����B
	}
}

// ****************************************************************
// �v���W�F�N�g�֘A�̃A�C�e����Ԃ��擾
// ================================================================
void C_tnm_compiler_main::get_project_item_state()
{
	if (!(0 <= G_data->cur_project_no && G_data->cur_project_no < (int)G_data->project.size()))
		return;

	// �X�^�[�g�V�[���̃A�C�e����Ԃ��擾
	G_data->project[G_data->cur_project_no].start_check = G_dlg->ci.chk_start_scene.get_check();
	G_data->project[G_data->cur_project_no].start_scene = G_dlg->ci.edt_start_scene.get_text();
	G_data->project[G_data->cur_project_no].start_z_no = str_to_int(G_dlg->ci.edt_start_z_no.get_text(), 0);
}

// ****************************************************************
// �v���W�F�N�g�֘A�̃A�C�e����Ԃ�ݒ�
// ================================================================
void C_tnm_compiler_main::set_project_item_state()
{
	if (!(0 <= G_data->cur_project_no && G_data->cur_project_no < (int)G_data->project.size()))
		return;

	// �X�^�[�g�V�[���̃A�C�e����Ԃ�ݒ�
	G_dlg->ci.chk_start_scene.set_check(G_data->project[G_data->cur_project_no].start_check);
	G_dlg->ci.edt_start_scene.set_text(G_data->project[G_data->cur_project_no].start_scene);
	G_dlg->ci.edt_start_z_no.set_text(tostr(G_data->project[G_data->cur_project_no].start_z_no));
}

// ****************************************************************
// �_�C�A���O���X�V
// ================================================================

// �E�B���h�E�^�C�g���� ss �̃p�X��\��
void C_tnm_compiler_main::update_dialog_for_window_title()
{
	if (G_data->cur_project_no < 0)
		return;

	TSTR ss_path_str = _T(" ( ss = ") + G_data->project[G_data->cur_project_no].scn_path + _T(" )");
	if (0 <= G_data->cur_project_no && G_data->cur_project_no < (int)G_data->project.size())
		ss_path_str = _T(" ( SS �t�H���_ �F ") + G_data->project[G_data->cur_project_no].scn_path + _T(" )");

	G_dlg->set_text(TNMC_WINDOW_TITLE_STR + ss_path_str);
}

// �S���{�^���i�Č����������I��or�S�I�����r���h�����s�j
void C_tnm_compiler_main::update_dialog_for_all_btn(bool is_cancel)
{
	if (is_cancel)	{
		G_dlg->ci.btn_all_sel_all.set_text(_T("�L�����Z��"));
		G_dlg->ci.btn_all_sel_auto.set_text(_T("�L�����Z��"));
	}
	else	{
		if (G_dlg->ci.chk_execute.get_check())	{
			G_dlg->ci.btn_all_sel_all.set_text(_T("�S���߲�\n���s"));
			G_dlg->ci.btn_all_sel_auto.set_text(_T("�X�V����\n�R���p�C��\n���s"));
		}
		else	{
			G_dlg->ci.btn_all_sel_all.set_text(_T("�S���߲�"));
			G_dlg->ci.btn_all_sel_auto.set_text(_T("�X�V����\n�R���p�C��"));
		}
	}
}

// �X�^�[�g�֘A�̃A�C�e��
void C_tnm_compiler_main::update_dialog_for_start_item()
{
	bool check_state = G_dlg->ci.chk_start_scene.get_check();
	G_dlg->ci.edt_start_scene.set_enable(check_state);
	G_dlg->ci.edt_start_z_no.set_enable(check_state);
	G_dlg->ci.spn_start_z_no.set_enable(check_state);
}

void C_tnm_compiler_main::update_dialog()
{
	// �v���W�F�N�g�I���R���{
	G_dlg->ci.cmb_project.clear_item_list();
	for (int i = 0; i < (int)G_data->project.size(); ++i)
		G_dlg->ci.cmb_project.insert_item_to_bottom(G_data->project[i].project_name + _T("�@�@�iSS̫��� �F ") + G_data->project[i].scn_path + _T("�j"), false);

	if (G_data->cur_project_no >= 0)
		G_dlg->ci.cmb_project.set_sel_no(G_data->cur_project_no);

	// �v���W�F�N�g�֘A�̃A�C�e����Ԃ�ݒ�
	set_project_item_state();

	// �E�B���h�E�^�C�g���� ss �̃p�X��\��
	update_dialog_for_window_title();

	// �X�^�[�g�֘A�̃A�C�e��
	update_dialog_for_start_item();

	// �S���{�^���i�Č����������I��or�S�I�����r���h�����s�j
	update_dialog_for_all_btn(false);

	// �V�[�����X�g�X�V
	update_scn_list();		// �i�_�C�A���O�̍X�V����̌Ăяo���j
}

// ****************************************************************
// ����F�v���W�F�N�g��ύX
// ================================================================
void C_tnm_compiler_main::change_project(int no)
{
	// �v���W�F�N�g�֘A�̃A�C�e����Ԃ��擾
	get_project_item_state();

	// �v���W�F�N�g�ԍ���ݒ�
	G_data->cur_project_no = no;

	// �_�C�A���O���X�V
	update_dialog();			// �i�v���W�F�N�g��ύX����̌Ăяo���j
}

// ****************************************************************
// ����F�v���W�F�N�g�ݒ�
// ================================================================
bool C_tnm_compiler_main::on_setting_project()
{
	set_project(PROJECT_SETTING_MODE_SETTING);		// �v���W�F�N�g��ݒ�
	return true;
}

// ****************************************************************
// ����F�v���W�F�N�g�쐬�i�V�K�v���W�F�N�g�j
// ================================================================
bool C_tnm_compiler_main::on_create_project()
{
	set_project(PROJECT_SETTING_MODE_CREATE);	// �v���W�F�N�g��ݒ�
	return true;
}

// ****************************************************************
// ����F�v���W�F�N�g�폜
// ================================================================
bool C_tnm_compiler_main::on_delete_project()
{
	set_project(PROJECT_SETTING_MODE_DELETE);		// �v���W�F�N�g��ݒ�
	return true;
}

// ****************************************************************
// ����̑O����
// ================================================================
bool C_tnm_compiler_main::on_command_befor_proc(int mod)
{
	// �v���Z�X�t���O
	proc_execut_flag = true;	// �v���Z�X���s���t���O
	proc_cancel_flag = false;	// �v���Z�X�L�����Z���t���O

	// ���O�̃N���A
	G_dlg->ci.lv_log.clear();
	G_data->error_jump_list.clear();

	// �����N���A
	G_dlg->ci.edt_detail1.clear_text();
	G_dlg->ci.edt_detail2.clear_text();

	// �_�C�A���O�̑���𖳌��ɂ���
	G_dlg->set_enable_control(false);

	// �S���{�^�����L�����Z���{�^���ɂ���
	update_dialog_for_all_btn(true);

	return true;
}

// ****************************************************************
// ����̌㏈��
// ================================================================
bool C_tnm_compiler_main::on_command_after_proc(int mod)
{
	// �S���{�^�������ɖ߂�
	update_dialog_for_all_btn(false);

	// �_�C�A���O�̑����L���ɂ���
	G_dlg->set_enable_control(true);

	// �v���Z�X�t���O
	proc_execut_flag = false;	// �v���Z�X���s���t���O
	proc_cancel_flag = false;	// �v���Z�X�L�����Z���t���O

	return true;
}

// ****************************************************************
// ����F�Č���
// ================================================================
bool C_tnm_compiler_main::on_command_find()
{
	on_command_befor_proc(0);
	update_scn_list();		// �V�[�����X�g�X�V�i�Č�������̌Ăяo���j
	on_command_after_proc(0);
	return true;
}

// ****************************************************************
// ����F�S�I��
// ================================================================
bool C_tnm_compiler_main::on_command_sel_all()
{
	on_command_befor_proc(0);
	proc_sel_all(true);			// �S�I��
	on_command_after_proc(0);
	return true;
}

// ****************************************************************
// ����F�S����
// ================================================================
bool C_tnm_compiler_main::on_command_unsel_all()
{
	on_command_befor_proc(0);
	proc_sel_all(false);		// �S����
	on_command_after_proc(0);
	return true;
}

// ****************************************************************
// ����F�����I��
// ================================================================
bool C_tnm_compiler_main::on_command_sel_auto()
{
	on_command_befor_proc(0);
	proc_sel_auto();		// �����I��
	on_command_after_proc(0);
	return true;
}

// ****************************************************************
// ����F�r���h
// ================================================================
bool C_tnm_compiler_main::on_command_build()
{
	on_command_befor_proc(0);
	proc_build();			// �r���h
	on_command_after_proc(0);
	return true;
}

// ****************************************************************
// ����F���s
// ================================================================
bool C_tnm_compiler_main::on_command_run()
{
	proc_run();				// ���s
	return true;
}

// ****************************************************************
// ����F�S���i�Č����������I��or�S�I�����r���h�����s�j
// ================================================================
bool C_tnm_compiler_main::on_command_all(bool is_sel_auto)
{
	// �v���Z�X�̃L�����Z��
	if (proc_execut_flag)	{	
		proc_cancel_flag = true;
		return false;
	}

	on_command_befor_proc(0);
	proc_all(is_sel_auto);	// �S���i�Č����������I��or�S�I�����r���h�����s�j
	on_command_after_proc(0);

	return true;
}

// ****************************************************************
// ����F�}�j���A��
// ================================================================
bool C_tnm_compiler_main::on_command_manual()
{
	// �v���W�F�N�g���I������Ă��Ȃ���Ύ��s
	if (!check_current_project(true))
		return false;

	// �}�j���A���p�X�̎擾
	TSTR manual_path = G_data->project[G_data->cur_project_no].manual_path;
	// �}�j���A�����J��
	shell_execute_open(manual_path);
	return true;
}

// ****************************************************************
// �����F��������
// ================================================================
bool C_tnm_compiler_main::start_auto_process()
{
	//
	// CUI ���[�h�ł̂݌Ă΂�鑀��ł��B
	//

	bool compile_ok_flag = false;

	if (G_data->cl_full_set)	{
		compile_ok_flag = proc_all(false);	// �S���i�Č������S�I�����r���h�j
	}
	else	{
		compile_ok_flag = proc_all(true);	// �S���i�Č����������I�����r���h�j
	}

	// ���O���o�͂���
	if (G_data->cl_log_set && !G_data->cl_log.empty())	{
		if (!::create_dir_from_dir_path(G_data->cl_log))	{
			error_box(G_data->cl_log + _T(" ���쐬�ł��܂���B���O���o�͂ł��܂���ł����B\n") + get_last_error_msg());
		}
		else	{

			TSTR log_path = G_data->cl_log + _T("\\log.txt");
			TSTR error_path = G_data->cl_log + _T("\\error.dat");

			// ���O�t�@�C�����폜����
			if (file_get_type(log_path) == 1)	{
				if (!::filesys_delete_file(log_path))	{
					error_box(log_path + _T(" ���폜�ł��܂���B���O���o�͂ł��܂���ł����B\n") + get_last_error_msg());
					goto output_end;
				}
			}
			if (file_get_type(error_path) == 1)	{
				if (!::filesys_delete_file(error_path))	{
					error_box(error_path + _T(" ���폜�ł��܂���B���O���o�͂ł��܂���ł����B\n") + get_last_error_msg());
					goto output_end;
				}
			}

			// ���O���o�͂���
			if (!C_file::write_full_text_in_MB(log_path, G_data->cui_full_log))	{
				error_box(log_path + _T(" �ɏ������߂܂���B���O���o�͂ł��܂���ł����B\n") + get_last_error_msg());
				goto output_end;
			}
			if (!compile_ok_flag)	{	// �R���p�C���Ɏ��s�����ꍇ error.dat ���o��
				if (!C_file::write_full_text_in_MB(error_path, _T("")))	{
					error_box(error_path + _T(" �ɏ������߂܂���B���O���o�͂ł��܂���ł����B\n") + get_last_error_msg());
					goto output_end;
				}
			}
		}
	}

output_end:

	// �_�C�A���O����ďI������
	G_dlg->close();

	return true;
}

// ****************************************************************
// �����F�v���W�F�N�g�̐ݒ�
// ================================================================
bool C_tnm_compiler_main::set_project(PROJECT_SETTING_MODE proc_mode)
{
	// �v���W�F�N�g���I������Ă��Ȃ���Ύ��s
	if ((proc_mode == PROJECT_SETTING_MODE_SETTING || proc_mode == PROJECT_SETTING_MODE_DELETE) && !check_current_project(true))
		return false;

	// �v���W�F�N�g�֘A�̃A�C�e����Ԃ��擾
	get_project_item_state();

	// �v���W�F�N�g�ݒ�_�C�A���O���J��
	C_tnm_project_setting_dialog psdlg;
	if (!psdlg.open(G_dlg->get_handle(), proc_mode))
		return false;	// �L�����Z�����ꂽ�炱���Ŗ߂�

	// �_�C�A���O���X�V
	update_dialog();					// �i�v���W�F�N�g�̐ݒ肩��̌Ăяo���j
	return true;
}

// ****************************************************************
// �t�@�C�����X�g�̃\�[�g�֐�
// ================================================================

struct FN_STRUCT
{
	TSTR	extend_name;
	TSTR	file_name;
	TSTR	sort_str;

	TSTR	file_path()
	{
		return (extend_name.empty() ? _T("") : extend_name + _T("\\")) + file_name;
	}
};

bool fn_list_sort_func(FN_STRUCT& lhs, FN_STRUCT& rhs)
{
	if (lhs.sort_str < rhs.sort_str)			{	return true;	}
	else if (lhs.sort_str > rhs.sort_str)		{	return false;	}
	return false;
}

void make_fn_list(CTSTR& path, CTSTR& extend_name, CTSTR& extension, ARRAY<FN_STRUCT>* fn_list)
{
	fn_list->clear();

	// �t�@�C�����X�g���쐬����
	{
		C_file_find ff;
		ff.find(path, _T("*"), extension, false, true, false);
		int file_cnt = ff.get_file_cnt();
		if (file_cnt <= 0)
			return;

		fn_list->resize(file_cnt);
		ARRAY<FN_STRUCT>::iterator itr = fn_list->begin();
		for (int i = 0; i < file_cnt; i++)	{
			itr->file_name = ff[i].rltv_path;
			itr->sort_str = str_to_lower_ret(itr->file_path());
			itr++;
		}
	}

	// �G�N�X�e���h�̃t�@�C�����X�g���쐬����
	if (!extend_name.empty())	{

		C_file_find ff;
		ff.find(path + _T("\\") + extend_name, _T("*"), extension, false, true, false);
		int file_cnt = ff.get_file_cnt();
		if (file_cnt > 0)	{

			int old_size = fn_list->size();
			fn_list->resize(old_size + file_cnt);
			ARRAY<FN_STRUCT>::iterator itr = fn_list->begin() + old_size;
			for (int i = 0; i < file_cnt; i++)	{
				itr->extend_name = extend_name;
				itr->file_name = ff[i].rltv_path;
				itr->sort_str = str_to_lower_ret(itr->file_path());
				itr++;
			}
		}
	}

	// �t�@�C�������\�[�g����
	std::sort(fn_list->begin(), fn_list->end(), fn_list_sort_func);
}

// ****************************************************************
// �����F�V�[�����X�g�X�V
// ================================================================
bool C_tnm_compiler_main::update_scn_list()
{
	G_dlg->ci.lv_ini.clear();
	G_dlg->ci.lv_inc.clear();
	G_dlg->ci.lv_scn.clear();
	G_data->ini_list.clear();
	G_data->inc_list.clear();
	G_data->scn_list.clear();

	// �v���W�F�N�g���I������Ă��Ȃ���Ύ��s
	if (!check_current_project(true))	{	return false;	}

	// �X�N���v�g�t�H���_�̎擾
	TSTR scn_dir_path = G_data->project[G_data->cur_project_no].scn_path;
	if (scn_dir_path.empty())
		return false;

	// �G�N�X�e���h���̎擾�i��ł����Ȃ��j
	TSTR extend_name = _T("");

	ARRAY<FN_STRUCT> fn_list;
	int row;

	// ini �t�@�C�����X�g�̎擾
	make_fn_list(scn_dir_path, extend_name, _T("ini"), &fn_list);
	for (int i = 0; i < (int)fn_list.size(); i++)
	{
		row = G_dlg->ci.lv_ini.add_item();
		G_dlg->ci.lv_ini.set_cell_text(row, 1, fn_list[i].file_path());
		G_data->ini_list.push_back(fn_list[i].file_path());
	}

	// dat �t�@�C�����X�g�̎擾
	make_fn_list(scn_dir_path, extend_name, _T("dat"), &fn_list);
	for (int i = 0; i < (int)fn_list.size(); i++)
	{
		row = G_dlg->ci.lv_ini.add_item();
		G_dlg->ci.lv_ini.set_cell_text(row, 1, fn_list[i].file_path());
		G_data->ini_list.push_back(fn_list[i].file_path());
	}

	// txt �t�@�C�����X�g�̎擾
	make_fn_list(scn_dir_path, extend_name, _T("txt"), &fn_list);
	for (int i = 0; i < (int)fn_list.size(); i++)
	{
		row = G_dlg->ci.lv_ini.add_item();
		G_dlg->ci.lv_ini.set_cell_text(row, 1, fn_list[i].file_path());
		G_data->ini_list.push_back(fn_list[i].file_path());
	}

	// inc �t�@�C�����X�g�̎擾
	make_fn_list(scn_dir_path, extend_name, _T("inc"), &fn_list);
	for (int i = 0; i < (int)fn_list.size(); i++)
	{
		row = G_dlg->ci.lv_inc.add_item();
		G_dlg->ci.lv_inc.set_cell_text(row, 1, fn_list[i].file_path());
		G_data->inc_list.push_back(fn_list[i].file_path());
	}
	
	// �g�� inc
	TSTR extend_inc = G_data->project[G_data->cur_project_no].extend_inc;
	std::list<TSTR> extend_inc_list;
	boost::split(extend_inc_list, extend_inc, boost::is_any_of(_T(";")));
	for (std::list<TSTR>::iterator itr = extend_inc_list.begin(); itr != extend_inc_list.end(); ++itr)
	{
		G_data->inc_list.push_back(*itr);
	}

	// SSID �d���`�F�b�N�f�[�^
	std::map<int, ARRAY<TSTR>> ssid_cheku_map;

	// ss �t�@�C�����X�g�̎擾
	make_fn_list(scn_dir_path, extend_name, _T("ss"), &fn_list);
	for (int i = 0; i < (int)fn_list.size(); i++)	{

		// SSID ���擾����
		// // #SCENE_SCRIPT_ID = 0001 �i�X�N���v�g�h�c�F���̍s�͍폜������A�ύX�����肵�Ȃ��ŉ������j
		int ssid = SCN_ID_NONE;	// ID�s��
		TSTR file_path = scn_dir_path + _T("\\") + fn_list[i].file_path();
		C_file file;
		if (file.open(file_path, _T("rt")))	{
			char buf[1024] = {0};
			if (fgets(buf, 1024, file.get_fp()))	{
				if (strlen(buf) >= 26)	{
					if (strncmp(buf, "// #SCENE_SCRIPT_ID = ", 22) == 0)	{
						ssid = str_to_int(MBSTR_to_TSTR(buf + 22, 4), SCN_ID_NONE);
					}
				}
			}
		}

		row = G_dlg->ci.lv_scn.add_item();
		G_dlg->ci.lv_scn.set_cell_text(row, 1, ssid == SCN_ID_NONE ? _T("---") : str_format(_T("%04d"), ssid));
		G_dlg->ci.lv_scn.set_cell_text(row, 2, fn_list[i].file_path());
		G_data->scn_list.push_back(S_scn_info(fn_list[i].file_path(), ssid));

		// ssid �d���`�F�b�N�p�}�b�v�ɓ����
		if (ssid != SCN_ID_NONE)	{
			ssid_cheku_map[ssid].push_back(fn_list[i].file_path());
		}
	}

	// ssid �d���`�F�b�N
	TSTR error_str;
	for (std::map<int, ARRAY<TSTR>>::iterator itr = ssid_cheku_map.begin(); itr != ssid_cheku_map.end(); ++itr)	{
		if (itr->second.size() >= 2)	{
			error_str += _T("��SSID = ") + tostr(itr->first) + _T("\n");
			for (int i = 0; i < (int)itr->second.size(); i++)	{
				error_str += itr->second[i] + _T(" ");
			}
			error_str += _T("\n\n");
		}
	}
	if (!error_str.empty())	{
		error_box(_T("SSID �̏d����������܂����I\n\n") + error_str + _T("���� ss �ő�{���쐬���邱�Ƃ͂ł��܂���B"), MB_OK);
	}

	return true;
}

// ****************************************************************
// �����F�S�I���^�S����
// ================================================================
bool C_tnm_compiler_main::proc_sel_all(bool mod)
{
	G_dlg->ci.lv_scn.set_item_select_all(mod);

	return true;
}

// ****************************************************************
// �����F�����I��
// ================================================================
bool C_tnm_compiler_main::proc_sel_auto()
{
	FILETIME tmp;
	bool all_update_flag = false;

	// inc �t�@�C����͊J�n
	set_log(_T("- �X�V���ꂽ�t�@�C���������I�����܂��B"));

	// �v���W�F�N�g���I������Ă��Ȃ���Ύ��s
	if (!check_current_project(true))
		return false;

	TSTR scn_dir_path = G_data->project[G_data->cur_project_no].scn_path;
	TSTR tmp_dir_path = G_data->project[G_data->cur_project_no].tmp_path;
	TSTR out_dir_path = G_data->project[G_data->cur_project_no].out_path;

	// �I����S����
	G_dlg->ci.lv_scn.set_item_select_all(false);

	// �X�V���ꂽ inc �t�@�C����T���i���������ꍇ�� ss ��S�I������j
	FILETIME inc_update, tmp_update;
	TSTR inc_name, inc_path, tmp_path;
	for (int i = 0; i < (int)G_data->inc_list.size(); i++)	{
		inc_name = erase_extension_ret(G_data->inc_list[i]);
		inc_path = scn_dir_path + _T("\\") + G_data->inc_list[i];
		tmp_path = tmp_dir_path + _T("\\inc\\") + inc_name + _T(".txt");
		if (file_get_type(tmp_path) == 1)	{
			file_get_time(inc_path, &tmp, &tmp, &inc_update);
			file_get_time(tmp_path, &tmp, &tmp, &tmp_update);
			if (inc_update.dwHighDateTime > tmp_update.dwHighDateTime || (inc_update.dwHighDateTime == tmp_update.dwHighDateTime && inc_update.dwLowDateTime > tmp_update.dwLowDateTime))	{
				all_update_flag = true;		// �S�I���t���O
				break;
			}
		}
		else	{
			all_update_flag = true;		// �S�I���t���O
			break;
		}
	}
	if (all_update_flag)	{
		set_log(_T("inc �t�@�C�����X�V���ꂽ�̂őS�I�����܂����B"));
		G_dlg->ci.lv_scn.set_item_select_all(true);	// �S�I��
		return true;
	}

	// �X�V���ꂽ ss �̂݃R���p�C�����s���i�X�V���t���r���đI������j
	FILETIME scn_update, dat_update;
	TSTR scn_name, scn_path, dat_path;
	for (int i = 0; i < (int)G_data->scn_list.size(); i++)	{
		scn_name = erase_extension_ret(G_data->scn_list[i].scn_name);
		scn_path = scn_dir_path + _T("\\") + scn_name + _T(".ss");
		if (G_dlg->ci.chk_easy_link.get_check())	{
			dat_path = tmp_dir_path + _T("\\bs\\") + scn_name + _T(".dat");
		}
		else	{
			dat_path = tmp_dir_path + _T("\\bs\\") + scn_name + _T(".lzss");
		}
		// dat �����݂���ꍇ
		if (file_get_type(dat_path) == 1)	{
			// �X�V���Ԃ��擾
			file_get_time(scn_path, &tmp, &tmp, &scn_update);
			file_get_time(dat_path, &tmp, &tmp, &dat_update);
			// scn ��� dat �̕����X�V���Ԃ��Â��̂őI��
			if (scn_update.dwHighDateTime > dat_update.dwHighDateTime || (scn_update.dwHighDateTime == dat_update.dwHighDateTime && scn_update.dwLowDateTime > dat_update.dwLowDateTime))	{
				G_dlg->ci.lv_scn.set_item_select(i, true);
			}
		}
		// dat �����݂��Ȃ��ꍇ
		else	{
			// �K���I������
			G_dlg->ci.lv_scn.set_item_select(i, true);
		}
	}
	return true;
}

// ****************************************************************
// �����F�r���h
// ================================================================
bool C_tnm_compiler_main::proc_build()
{
	bool error = false;
	int error_cnt = 0;

	int start_time = ::GetTickCount();

	// �v���W�F�N�g���I������Ă��Ȃ���Ύ��s
	if (!check_current_project(true))
		return false;

	// �t�H���_�̎擾
	TSTR scn_dir_path = G_data->project[G_data->cur_project_no].scn_path;
	if (scn_dir_path.empty())	{
		error_box(_T("scn �t�H���_���w�肵�Ă��������B"));
		return false;
	}
	TSTR tmp_dir_path = G_data->project[G_data->cur_project_no].tmp_path;
	if (tmp_dir_path.empty())	{
		error_box(_T("tmp �t�H���_���w�肵�Ă��������B"));
		return false;
	}
	TSTR out_dir_path = G_data->project[G_data->cur_project_no].out_path;
	if (out_dir_path.empty())	{
		error_box(_T("�o�̓t�H���_���w�肵�Ă��������B"));
		return false;
	}

	// Gameexe.ini ���
	if (!proc_gei())	{
		set_log(_T("Gameexe.ini �̉�͂Ɏ��s���܂����B"));
		return false;
	}

	// inc �t�@�C�����
	S_tnms_ia_data ia_data;
	if (!proc_inc(&ia_data))	{
		set_log(_T("inc �t�@�C���̉�͂Ɏ��s���܂����B"));
		return false;
	}

	// �R���p�C��
	if (!proc_compile(&ia_data, &error_cnt))	{
		set_log(_T("�R���p�C���Ɏ��s���܂����B"));
		error = true;	// �G���[�t���O�𗧂Ă邾���I
	}

	// �L�����Z��
	if (proc_cancel_flag)	{
		set_log(_T("�L�����Z�����܂����B"));
		return false;
	}

	// �����N
	if (!proc_link(&ia_data))	{
		set_log(_T("�����������N�Ɏ��s���܂����I������"));
		return false;
	}

	if (error)	{
		set_log(_T("������") + tostr(error_cnt) + _T("�̃t�@�C���ɃG���[��������܂����I������"));
		return false;
	}

	DWORD past_time = ::GetTickCount() - start_time;

	set_log(_T("�S�Ă̏���������ɐ������܂����B�i�o�ߎ��ԁF") + tostr(past_time) + _T("�j"));

	// �����F�r���h��̏���
	if (!proc_build_after())	{
		set_log(_T("�r���h��̏����Ɏ��s���܂����B"));
		return false;
	}

	return true;
}

// ****************************************************************
// �����F�r���h��̏���
// ================================================================
bool C_tnm_compiler_main::proc_build_after()
{
	if (!G_data->project[G_data->cur_project_no].build_after.empty())	{
		::shell_execute_open(G_data->project[G_data->cur_project_no].build_after);
		set_log(_T("�r���h��̏������s���܂����B"));
	}

	return true;
}

// ****************************************************************
// \ �� \\ �ɒu������
// ================================================================
TSTR escape_yen(CTSTR& str)
{
	TSTR ret_str;

	for (TSTR::const_iterator itr = str.begin(); itr != str.end(); ++itr)	{
		if (*itr == _T('\\'))
			ret_str += _T("\\\\");
		else
			ret_str += *itr;
	}
	
	return ret_str;
}

// ****************************************************************
// �����F���s
// ================================================================
bool C_tnm_compiler_main::proc_run(CTSTR& start_scene_str, int z_no)
{
	// �v���W�F�N�g���I������Ă��Ȃ���Ύ��s
	if (!check_current_project(true))
		return false;

	// �t�H���_�̎擾
	TSTR exe_path = G_data->project[G_data->cur_project_no].exe_path;
	if (exe_path.empty())	{
		error_box(_T("�g�p���� exe ���w�肵�Ă��������B"));
		return false;
	}

	// �R�}���h���C���̎w��
	TSTR command_line;
	if (G_dlg->ci.chk_debug_on.get_check())	{
		command_line += _T(" /debug_on");
		command_line += _T(" /dp=\"aQpbZF$ijgDSLL19hY#)An36;Ij@ln+G\"");
		command_line += _T(" /dk=\"7hG)o!bNa>tvV{wcc-D>EXpPfq7Bn*sU\"");
	}
	if (G_dlg->ci.chk_debug_off.get_check())	{
		command_line += _T(" /debug_off");
	}

	if (!G_data->project[G_data->cur_project_no].work_path.empty())	{
		command_line += _T(" /work_dir=\"") + escape_yen(G_data->project[G_data->cur_project_no].work_path) + _T("\"");
	}

	if (!start_scene_str.empty())	{
		command_line += _T(" /start=\"") + start_scene_str + _T("\"");
		command_line += _T(" /z_no=") + tostr(z_no);
	}
	else if (G_dlg->ci.chk_start_scene.get_check() && !G_dlg->ci.edt_start_scene.get_text().empty())	{
		command_line += _T(" /start=\"") + G_dlg->ci.edt_start_scene.get_text() + _T("\"");
		command_line += _T(" /z_no=") + G_dlg->ci.edt_start_z_no.get_text();
	}

	if (G_dlg->ci.chk_no_interval.get_check())	{
		command_line += _T(" /no_interval");
	}
	if (G_dlg->ci.chk_overflow_error_off.get_check())	{
		command_line += _T(" /overflow_error_off");
	}

	command_line += _T(" /end_start");

	// exe ���s
	open_process(exe_path + command_line);

	return true;
}

// ****************************************************************
// �����F�S���i�Č����������I��or�S�I�����r���h�����s�j
// ================================================================
bool C_tnm_compiler_main::proc_all(bool is_sel_auto)
{
#if 0
	G_name_cntr.clear();
#endif

	// �����F�V�[�����X�g�X�V
	if (!update_scn_list())		// �i�S�������̍Č�������̌Ăяo���j
		return false;

	// �����F�����I��
	if (is_sel_auto)	{
		if (!proc_sel_auto())
			return false;
	}
	// �����F�S�I��
	else	{
		proc_sel_all(true);
	}

	// �����F�r���h
	if (!proc_build())
		return false;

	// �L�����Z��
	if (proc_cancel_flag)
		return false;

#if 0
	// ���O�}�b�v�̓��e��f���o��
	TSTR scr_dir_path = G_data->project[G_data->cur_project_no].scn_path;
	if (scr_dir_path.empty())
		return false;

	C_file file;
	if (!file.open(scr_dir_path + _T("\\___namae.csv"), _T("wt")))	{
		message_box(_T("open error"), _T(""), MB_OK);
	}
	for (std::map<TSTR, int>::iterator itr = G_name_cntr.begin(); itr != G_name_cntr.end(); ++itr)	{
		TSTR str = itr->first + _T(",") + tostr(itr->second) + _T("\n");
		file.write_in_MB(str);
	}
	file.close();
#endif

	// �����F���s�iGUI �̂݁j
	if (G_data->gui_flag && G_dlg->ci.chk_execute.get_check())	{
		if (!proc_run())
			return false;
	}

	return true;
}

// ****************************************************************
// ���O�̐ݒ�
// ================================================================
void C_tnm_compiler_main::set_log(CTSTR& text)
{
	set_log_jump(text, _T(""), _T(""), -1);
}

void C_tnm_compiler_main::set_log_jump(CTSTR& text, CTSTR& detail_file_path, CTSTR& jump_file_path, int line_no)
{
	// �W�����v���X�g�ɒǉ�
	S_error_jump error_jump;
	error_jump.detail_file_path = detail_file_path;
	error_jump.jump_file_path = jump_file_path;
	error_jump.line_no = line_no;
	G_data->error_jump_list.push_back(error_jump);

	// ���X�g�r���[�ɃA�C�e����ǉ�
	int line = G_dlg->ci.lv_log.add_item();
	G_dlg->ci.lv_log.set_cell_text_visible(line, 1, text);

	// ���O��ۑ�
	if (!G_data->gui_flag && G_data->cl_log_set)	{
		G_data->cui_full_log += text + _T("\n");
	}

	// ���b�Z�[�W���[�v�ɂP��܂킷
	G_app.default_message_loop();
}

// ****************************************************************
// ini �r���[���� ini ���J��
// ================================================================
void C_tnm_compiler_main::open_ini_by_ini_lv(int row_no)
{
	if (!(0 <= row_no && row_no < (int)G_data->ini_list.size()))
		return;

	if (!check_current_project(true))	{	return;	}		// �v���W�F�N�g���I������Ă��Ȃ���Ύ��s
	TSTR file_path = G_data->project[G_data->cur_project_no].scn_path + _T("\\") + G_data->ini_list[row_no];
	TSTR editor_path = G_data->project[G_data->cur_project_no].editor_path;
	open_file_by_editor(file_path, 0, editor_path);
}

// ****************************************************************
// inc �r���[���� inc ���J��
// ================================================================
void C_tnm_compiler_main::open_inc_by_inc_lv(int row_no)
{
	if (!(0 <= row_no && row_no < (int)G_data->inc_list.size()))
		return;

	if (!check_current_project(true))
		return;		// �v���W�F�N�g���I������Ă��Ȃ���Ύ��s

	TSTR file_path = G_data->project[G_data->cur_project_no].scn_path + _T("\\") + G_data->inc_list[row_no];
	TSTR editor_path = G_data->project[G_data->cur_project_no].editor_path;
	open_file_by_editor(file_path, 0, editor_path);
}

// ****************************************************************
// scn �r���[���� scn ���J��
// ================================================================
void C_tnm_compiler_main::open_scn_by_scn_lv(int row_no)
{
	if (!(0 <= row_no && row_no < (int)G_data->scn_list.size()))
		return;
	if (!check_current_project(true))
		return;		// �v���W�F�N�g���I������Ă��Ȃ���Ύ��s

	TSTR file_path = G_data->project[G_data->cur_project_no].scn_path + _T("\\") + G_data->scn_list[row_no].scn_name;
	TSTR editor_path = G_data->project[G_data->cur_project_no].editor_path;
	open_file_by_editor(file_path, 0, editor_path);
}

// ****************************************************************
// ���O�r���[���� scn ���J��
// ================================================================
void C_tnm_compiler_main::open_scn_by_log_lv(int row_no)
{
	if (row_no < 0 || (int)G_data->error_jump_list.size() <= row_no)
		return;

	if (G_data->error_jump_list[row_no].jump_file_path.empty())
		return;

	if (!check_current_project(true))
		return;		// �v���W�F�N�g���I������Ă��Ȃ���Ύ��s

	TSTR editor_path = G_data->project[G_data->cur_project_no].editor_path;
	open_file_by_editor(G_data->error_jump_list[row_no].jump_file_path, G_data->error_jump_list[row_no].line_no, editor_path);
}

// ****************************************************************
// �t�H���_���J��
// ================================================================

// �t�H���_�I�[�v��
bool C_tnm_compiler_main::open_folder(CTSTR& path)
{
	if (file_get_type(path) != 0)	{
		error_box(_T("�t�H���_��������܂���B\n\n") + path, MB_OK);
		return false;
	}
	shell_execute_open(path);
	return true;
}

// scn �p�X�̃t�H���_���J��
bool C_tnm_compiler_main::open_folder_from_scn_path()
{
	if (!check_current_project(true))
		return false;

	TSTR path = G_data->project[G_data->cur_project_no].scn_path;
	return open_folder(path);
}

// tmp �p�X�̃t�H���_���J��
bool C_tnm_compiler_main::open_folder_from_tmp_path()
{
	if (!check_current_project(true))
		return false;

	TSTR path = G_data->project[G_data->cur_project_no].tmp_path;
	return open_folder(path);
}

// out �p�X�̃t�H���_���J��
bool C_tnm_compiler_main::open_folder_from_out_path()
{
	if (!check_current_project(true))
		return false;

	TSTR path = G_data->project[G_data->cur_project_no].out_path;
	return open_folder(path);
}

// exe �p�X�̃t�H���_���J��
bool C_tnm_compiler_main::open_folder_from_exe_path()
{
	if (!check_current_project(true))
		return false;

	TSTR path = get_dir_path(G_data->project[G_data->cur_project_no].exe_path);
	return open_folder(path);
}

// work �p�X�̃t�H���_���J��
bool C_tnm_compiler_main::open_folder_from_work_path()
{
	if (!check_current_project(true))
		return false;

	TSTR path = G_data->project[G_data->cur_project_no].work_path;
	return open_folder(path);
}

// editor �p�X�̃t�H���_���J��
bool C_tnm_compiler_main::open_folder_from_editor_path()
{
	if (!check_current_project(true))
		return false;

	TSTR path = get_dir_path(G_data->project[G_data->cur_project_no].editor_path);
	return open_folder(path);
}

// manual �p�X�̃t�H���_���J��
bool C_tnm_compiler_main::open_folder_from_manual_path()
{
	if (!check_current_project(true))
		return false;

	TSTR path = get_dir_path(G_data->project[G_data->cur_project_no].manual_path);
	return open_folder(path);
}

// ****************************************************************
// �|�b�v�A�b�v���j���[����
// ================================================================

const int	COMPILER_POPUP_MENU_SEL_START_SCENE = 10;

const int	COMPILER_POPUP_MENU_RUN_START_SCENE = 20;
const int	COMPILER_POPUP_MENU_RUN_START_Z_NO = 21;

const int	COMPILER_POPUP_MENU_SET_START_SCENE = 30;
const int	COMPILER_POPUP_MENU_SET_START_Z_NO = 31;

const int	COMPILER_POPUP_MENU_ALL_SELECT = 40;
const int	COMPILER_POPUP_MENU_ALL_UNSELECT = 41;

const int	COMPILER_POPUP_MENU_OPEN_FOLDER_SCN = 50;
const int	COMPILER_POPUP_MENU_OPEN_FOLDER_TMP = 51;
const int	COMPILER_POPUP_MENU_OPEN_FOLDER_OUT = 52;
const int	COMPILER_POPUP_MENU_OPEN_FOLDER_EXE = 53;
const int	COMPILER_POPUP_MENU_OPEN_FOLDER_WORK = 54;
const int	COMPILER_POPUP_MENU_OPEN_FOLDER_MANUAL = 55;

const int	COMPILER_POPUP_MENU_DELETE_SAVEDATA = 60;


const int	COMPILER_POPUP_MENU_RUN_Z_NO_TOP = 1000;
const int	COMPILER_POPUP_MENU_SET_Z_NO_TOP = 2000;


void C_tnm_compiler_main::popup_menu_proc(CTSTR& start_scene_str)
{
	C_popup_menu menu;
	menu.create();

	bool project_selected = check_current_project(false);

	// �X�^�[�g�V�[���֘A�̏���
	TSTR sel_start_scene_column_str;
	bool scene_enable;
	if (start_scene_str.empty())	{
		sel_start_scene_column_str = _T("No Select");
		scene_enable = false;
	}
	else	{
		sel_start_scene_column_str = start_scene_str;
		scene_enable = true;
	}

	// �y�ԍ����X�g�쐬
	ARRAY<int> z_list;
	if (!start_scene_str.empty() && project_selected)	{
		TSTR path = G_data->project[G_data->cur_project_no].tmp_path + _T("\\bs\\") + start_scene_str + _T(".dat");
		if (file_get_type(path) == 1)	{
			BUFFER buf;
			if (C_file::read_full_data(path, buf))	{
				S_tnm_scn_header *header = (S_tnm_scn_header *)buf.get();
				if (header->z_label_cnt > 0)	{
					int *label = (int *)(buf.get() + header->z_label_list_ofs);
					for (int i = 0; i < header->z_label_cnt; i++)	{
						if (label[i] > 0)	{	// 0 �͖���`�̃��x���ł��B
							z_list.push_back(i);
						}
					}
					// #Z00 �����Ȃ��Ȃ烊�X�g���N���A����
					if ((int)z_list.size() == 1 && z_list[0] == 0)	{
						z_list.clear();
					}
				}
				buf.clear();
			}
		}
	}

	// �u�Z�[�u�f�[�^���폜����v�̏���
	TSTR savedata_path;
	bool savedata_delete = false;
	if (project_selected)	{
		if (file_get_type(G_data->project[G_data->cur_project_no].work_path) == 0)	{
			savedata_path = G_data->project[G_data->cur_project_no].work_path + _T("\\savedata");
			if (file_get_type(savedata_path) == 0)	{
				savedata_delete = true;
			}
		}
		else if (file_get_type(G_data->project[G_data->cur_project_no].exe_path) == 1)	{
			savedata_path = get_dir_path(G_data->project[G_data->cur_project_no].exe_path) + _T("\\savedata");
			if (file_get_type(savedata_path) == 0)	{
				savedata_delete = true;
			}
		}
	}

	// �u�y�ԍ��v���w�肵�Ď��s����
	C_popup_menu menu_run_z_no;
	menu_run_z_no.create();
	for (int i = 0; i < (int)z_list.size(); i++)	{
		menu_run_z_no.add_item(COMPILER_POPUP_MENU_RUN_Z_NO_TOP + i, _T("#Z") + str_format(_T("%02d"), z_list[i]));
	}

	// �X�^�[�g�V�[���Ɂu�y�ԍ��v���ݒ肷��
	C_popup_menu menu_set_z_no;
	menu_set_z_no.create();
	for (int i = 0; i < (int)z_list.size(); i++)	{
		menu_set_z_no.add_item(COMPILER_POPUP_MENU_SET_Z_NO_TOP + i, _T("#Z") + str_format(_T("%02d"), z_list[i]));
	}

//	menu.add_item(COMPILER_POPUP_MENU_SEL_START_SCENE, sel_start_scene_column_str, false);
//	menu.add_separator();
	menu.add_item(COMPILER_POPUP_MENU_RUN_START_SCENE, _T("���s���� (#Z00)"), scene_enable);
	menu.add_sub_menu(COMPILER_POPUP_MENU_RUN_START_Z_NO, _T("�@�� �y�ԍ����w�� ��"), menu_run_z_no.get_handle(), z_list.empty() ? false : scene_enable);
	menu.add_separator();
	menu.add_item(COMPILER_POPUP_MENU_SET_START_SCENE, _T("[ ���ļ�� ] �ɐݒ肷�� (#Z00)"), scene_enable);
	menu.add_sub_menu(COMPILER_POPUP_MENU_SET_START_Z_NO, _T("�@�� �y�ԍ����w�� ��"), menu_set_z_no.get_handle(), z_list.empty() ? false : scene_enable);
	menu.add_separator();
	menu.add_item(COMPILER_POPUP_MENU_ALL_SELECT, _T("�r�r�̑S�I��"), project_selected);
	menu.add_item(COMPILER_POPUP_MENU_ALL_UNSELECT, _T("�r�r�̑S����"), project_selected);
	menu.add_separator();
	menu.add_item(COMPILER_POPUP_MENU_OPEN_FOLDER_SCN, _T("�r�r�t�H���_���J��"), project_selected);
	menu.add_item(COMPILER_POPUP_MENU_OPEN_FOLDER_TMP, _T("�s�l�o�t�H���_���J��"), project_selected);
	menu.add_item(COMPILER_POPUP_MENU_OPEN_FOLDER_OUT, _T("�o�̓t�H���_���J��"), project_selected);
	menu.add_item(COMPILER_POPUP_MENU_OPEN_FOLDER_EXE, _T("�d�w�d�t�H���_���J��"), project_selected);
	menu.add_item(COMPILER_POPUP_MENU_OPEN_FOLDER_WORK, _T("��ƃt�H���_���J��"), project_selected);
	menu.add_item(COMPILER_POPUP_MENU_OPEN_FOLDER_MANUAL, _T("�}�j���A���t�H���_���J��"), project_selected);
	menu.add_separator();
	menu.add_item(COMPILER_POPUP_MENU_DELETE_SAVEDATA, _T("�Z�[�u�f�[�^���폜����"), savedata_delete);

	C_point mp = get_mouse_pos();

	int res = menu.select(G_dlg->get_handle(), mp.x, mp.y);

	switch (res)	{
		case COMPILER_POPUP_MENU_SEL_START_SCENE:		// �w�I������Ă���V�[���x
			break;
		case COMPILER_POPUP_MENU_RUN_START_SCENE:		// ���s����
			proc_run(start_scene_str, 0);
			break;
		case COMPILER_POPUP_MENU_SET_START_SCENE:		// �X�^�[�g�V�[���ɐݒ肷��
			G_dlg->ci.chk_start_scene.set_check(true);
			G_dlg->ci.edt_start_scene.set_text(start_scene_str);
			G_dlg->ci.edt_start_z_no.set_text(_T("0"));
			G_main->update_dialog_for_start_item();
			break;
		case COMPILER_POPUP_MENU_ALL_SELECT:			// �S�I��
			G_main->on_command_sel_all();
			break;
		case COMPILER_POPUP_MENU_ALL_UNSELECT:			// �S����
			G_main->on_command_unsel_all();
			break;
		case COMPILER_POPUP_MENU_OPEN_FOLDER_SCN:		// �r�r�t�H���_���J��
			G_main->open_folder_from_scn_path();
			break;
		case COMPILER_POPUP_MENU_OPEN_FOLDER_TMP:		// �s�l�o�t�H���_���J��
			G_main->open_folder_from_tmp_path();
			break;
		case COMPILER_POPUP_MENU_OPEN_FOLDER_OUT:		// �o�̓t�H���_���J��
			G_main->open_folder_from_out_path();
			break;
		case COMPILER_POPUP_MENU_OPEN_FOLDER_EXE:		// �d�w�d�t�H���_���J��
			G_main->open_folder_from_exe_path();
			break;
		case COMPILER_POPUP_MENU_OPEN_FOLDER_WORK:		// ��ƃt�H���_���J��
			G_main->open_folder_from_work_path();
			break;
		case COMPILER_POPUP_MENU_OPEN_FOLDER_MANUAL:	// �}�j���A���t�H���_���J��
			G_main->open_folder_from_manual_path();
			break;
		case COMPILER_POPUP_MENU_DELETE_SAVEDATA:
			if (info_box(_T("�Z�[�u�f�[�^���폜���Ă���낵���ł����H\n\n") + savedata_path, MB_YESNO) == IDYES)	{
				filesys_remove_directory_with_contain_files(savedata_path, true, true, true);
			}
			break;
		default:
			// �y�ԍ����w�肵�ăX�^�[�g�V�[���ɐݒ肷��
			if (res >= COMPILER_POPUP_MENU_SET_Z_NO_TOP)	{
				int list_no = res - COMPILER_POPUP_MENU_SET_Z_NO_TOP;
				int z_no = 0;
				if (0 <= list_no && list_no < (int)z_list.size())	{
					z_no = z_list[list_no];
				}
				G_dlg->ci.chk_start_scene.set_check(true);
				G_dlg->ci.edt_start_scene.set_text(start_scene_str);
				G_dlg->ci.edt_start_z_no.set_text(tostr(z_no));
				G_main->update_dialog_for_start_item();
			}
			// �y�ԍ����w�肵�Ď��s����
			else if (res >= COMPILER_POPUP_MENU_RUN_Z_NO_TOP)	{
				int list_no = res - COMPILER_POPUP_MENU_RUN_Z_NO_TOP;
				int z_no = 0;
				if (0 <= list_no && list_no < (int)z_list.size())	{
					z_no = z_list[list_no];
				}
				proc_run(start_scene_str, z_no);
			}
			break;
	}

	menu_set_z_no.destroy();
	menu_run_z_no.destroy();
	menu.destroy();
}

// ****************************************************************
// �G���[�̏o���s�̏ڍׂ��X�e�[�^�X�o�[�ɕ\��
// ================================================================
void C_tnm_compiler_main::show_err_detail(int row_no)
{
	if (row_no < 0 || (int)G_data->error_jump_list.size() <= row_no)
		return;

	bool utf8 = G_data->project[G_data->cur_project_no].utf8;

	G_dlg->ci.edt_detail1.clear_text();
	G_dlg->ci.edt_detail2.clear_text();

	if (G_data->error_jump_list[row_no].detail_file_path.empty())
		return;

	ARRAY<TSTR> ca_text;
	utf8 ? C_file::read_full_text_UTF8(G_data->error_jump_list[row_no].detail_file_path, ca_text) : C_file::read_full_text(G_data->error_jump_list[row_no].detail_file_path, ca_text);
	int line_no = G_data->error_jump_list[row_no].line_no;
	if (line_no - 1 < 0 || (int)ca_text.size() <= line_no - 1)
		return;

	TSTR text = ca_text[line_no - 1];

	// �^�u���X�y�[�X�ɕϊ�
	str_replace(text, _T("\t"), _T(" "));

	// �A���X�y�[�X��u��
	str_replace(text, _T("  "), _T(" "));

	// �X�e�[�^�X�o�[�ɐݒ�
	G_dlg->set_detail1_text(text);
	G_dlg->set_detail2_text(G_dlg->ci.lv_log.get_cell_text(row_no, 1));
}

// ****************************************************************
// �v���W�F�N�g���I������Ă��邩�ǂ������肷��
// ================================================================
bool C_tnm_compiler_main::check_current_project(bool is_warning)
{
	if (G_data->cur_project_no == -1)
		return false;

	if (0 <= G_data->cur_project_no && G_data->cur_project_no < (int)G_data->project.size())
		return true;

	if (is_warning)	{	error_box(_T("�v���W�F�N�g��I�����Ă��������B"));	}
	return false;
}

// ****************************************************************
// �E���ɂ��Ă̐���
// ================================================================
bool C_tnm_compiler_main::explain_under()
{
	MessageBox(G_dlg->get_handle(),
		_T("������������҂��Ȃ�\n")
		_T("�����e�X�g�p�ł��B\n")
		_T("\n")
		_T("�����k���Ȃ�\n")
		_T("Scene.pck �����k���܂���B�R���p�C���������Ȃ�܂��B\n")
		_T("���k���Ȃ��ꍇ�A�Q�[���E�B���h�E�̃L���v�V������ [�������k��] �ƕ\������܂��B�i�f�o�b�O���݂̂ł��B�j\n")
		_T("���i�łł͕K���`�F�b�N���͂����ĉ������B\n")
		_T("\n")
		_T("�����̃`�F�b�N�́A�R���p�C�����N������x�ɃI�t�ɖ߂�܂��B")
		, _T("�E���̃{�^���ɂ��Ă̐���"), MB_OK);

	return true;
}

// ****************************************************************
// �Í�.dat �̓ǂݍ���
// ================================================================
bool C_tnm_compiler_main::load_exe_angou_dat(TSTR& angou_str)
{
	bool utf8 = G_data->project[G_data->cur_project_no].utf8;

	angou_str.clear();
	TSTR exe_angou_dat_path = G_data->project[G_data->cur_project_no].scn_path + _T("\\�Í�.dat");
	TSTR exe_angou_dat;
	if (!(utf8 ? C_file::read_full_text_UTF8(exe_angou_dat_path, exe_angou_dat) : C_file::read_full_text(exe_angou_dat_path, exe_angou_dat)))
	{
		return false;
	}

	TSTR::iterator itr = exe_angou_dat.begin();
	TSTR::iterator end = exe_angou_dat.end();
	while (1)	{
		if (itr == end)	{	break;	}
		if (*itr == _T('\n'))	{	break;	}
		angou_str += *itr++;
	}
	return true;
}

// ****************************************************************
// �Í��ɂ��Ă̐���
// ================================================================
void C_tnm_compiler_main::manual_exe_angou()
{
	MessageBox(G_dlg->get_handle(),
		_T("������\n\n")
		_T("�@�C�x���g�f�[�^ ( Scene.pck ) �Ǝ��s�d�w�d ( SiglusEngine.exe ) ���Í������܂��B\n")
		_T("�@����ɂ��A�Í�����v���Ȃ���Γ����Ȃ��Ȃ�܂��B\n\n")
		_T("���ړI\n\n")
		_T("�@�R�s�[�v���e�N�g���{�������i�ɁA�v���e�N�g�̂������Ă��Ȃ� SiglusEngine.exe ���R�s�[����ƁA���ʂɓ��삵�Ă��܂��܂��B\n")
		_T("�@�܂��A���i�ł̏C���p�b�`���o�����Ƃ��A�u�̌��łɁv�u���i�ł̏C���p�b�`�𓖂Ă�v���ƂŁA�����őS�ẴV�i���I���ǂ߂Ă��܂����Ƃ�����܂��B\n")
		_T("�@�i�g�ݕ��ɂ����܂��B�ʏ�́A�t�@�C���G���[�Ȃǂ��o�܂���̂ŃQ�[���ɂȂ�Ȃ��Ǝv���܂��B�j\n")
		_T("�@������h�����߂ł��B\n")
		_T("�@(2011/9/14) �v���e�N�g�������Ȃ��^�C�g���̏ꍇ���A�Í������K�{�ɂȂ�܂����B\n\n")
		_T("�@�K���e�^�C�g�����ƂɈÍ��̕�����͕ς��ĉ������B\n\n")
		_T("�@�����z�z����̌��łȂǂ͈Í�������K�v�͂���܂���B\n")
		_T("�@�i���Ă����͂Ȃ��ł����A���i�łƂ͈قȂ�Í��ɂ��ĉ������B�j\n\n")
		_T("���菇\n\n")
		_T("�@�P�Dss �t�@�C���Ɠ����ꏊ�ɈÍ��t�@�C���i�Í�.dat�j���쐬���ĉ������B\n\n")
		_T("�@�@�@�e�L�X�g�t�@�C����V�K�쐬�����O�� �u �Í�.dat �v �ɕύX���ĉ������B\n\n")
		_T("�@�@�@�쐬�����t�@�C���̂P�s�ڂɈÍ��̕�������L�q���ĉ������B\n")
		_T("�@�@�@�S�p�Ȃ�S�����ȏ�A���p�Ȃ�W�����ȏ�A�S�p���p�͍��݂��Ă���肠��܂���B\n\n")
		_T("�@�@�@�������A�K���e�^�C�g�����ƂɈÍ��̕�����͕ς��ĉ������B\n\n")
		_T("�@�@�@��F �T���v��Project\n\n")
		_T("�@�Q�D�C�x���g�f�[�^���Í�������\n\n")
		_T("�@�@�@�R���p�C�����ɈÍ��t�@�C���̑��݂̗L���Ŏ����I�ɈÍ������܂��B\n\n")
		_T("�@�@�@�Í������Ȃ��ꍇ�͈Í��t�@�C���i�Í�.dat�j���폜�������͖��O��ύX���ĉ������B\n\n")
		_T("�@�R�D�d�w�d���Í�������\n\n")
		_T("�@�@�@���j���[���ڂ� [ �d�w�d���Í������� ] ��I�����ĉ������B\n\n")
		_T("�@�@�@���P�x�Í������ꂽ�d�w�d���ĈÍ������鎖�͂ł��܂���B\n\n")
		_T("�@�@�@�Í������ɖ߂������ꍇ �� �Ⴄ�Í��ɂ������ꍇ�́A�Í�������Ă��Ȃ����̂d�w�d�������Ă��ăR�s�[���ĉ������B\n\n")
		_T("���⑫\n\n")
		_T("�@�Í�������Ă��Ȃ��C�x���g�f�[�^�́A�Í������ꂽ�d�w�d�ł������܂��B\n")
		_T("�@�Í������ꂽ�C�x���g�f�[�^���A�Í�������Ă��Ȃ��d�w�d�œ������Ɨ\�����ʓ���łd�w�d�������I�����܂��B\n")
		, _T("�Í����ɂ��Ă̐���"), MB_OK);
}

// ****************************************************************
// �d�w�d���Í�������
// ================================================================
void C_tnm_compiler_main::func_exe_angou()
{
	TSTR exe_angou_str;
	if (!G_main->load_exe_angou_dat(exe_angou_str))	{
		MessageBox(G_dlg->get_handle(),
			_T("�Í��̕����񂪂���܂���B\n\n")
			_T("ss �t�@�C���Ɠ����ꏊ�ɈÍ��t�@�C���i�Í�.dat�j��������܂���B\n")
			, _T("�G���["), MB_OK | MB_ICONWARNING);
	}
	else if(exe_angou_str.empty())	{
		MessageBox(G_dlg->get_handle(),
			_T("�Í��̕����񂪂���܂���B\n\n")
			_T("�Í��t�@�C���i�Í�.dat�j�̂P�s�ڂɈÍ��̕����񂪋L�q����Ă��܂���B\n")
			, _T("�G���["), MB_OK | MB_ICONWARNING);
	}
	else	{
		TSTR exe_path = G_data->project[G_data->cur_project_no].exe_path;
		MBSTR mb_exe_angou_str = TSTR_to_MBSTR(exe_angou_str);
		if((int)mb_exe_angou_str.size() < 8)	{
			TSTR mess = 
				_T("�Í��̕����񂪒Z�߂��܂��B\n\n")
				_T("��Í��̕������\n\n")
				_T("�@�@") + exe_angou_str + _T("\n\n")
				_T("�S�p�Ȃ�S�����ȏ�A���p�Ȃ�W�����ȏ�A�S�p���p�͍��݂��Ă���肠��܂���B\n\n")
				_T("��F ��������޼ޭ��Art's\n\n");
			MessageBox(G_dlg->get_handle(), mess.c_str(), _T("�G���["), MB_OK | MB_ICONWARNING);
		}
		else if(exe_path.empty())	{
			MessageBox(G_dlg->get_handle(), _T("�v���W�F�N�g�̐ݒ�Łu���s�t�@�C���v �ɂd�w�d�t�@�C�����w�肵�ĉ������B\n"), _T("�G���["), MB_OK | MB_ICONWARNING);
		}
		else if(file_get_type(exe_path) != 1)	{
			TSTR mess = 
				_T("�d�w�d�t�@�C����������܂���B\n\n")
				_T("�u���s�t�@�C���v �Ɏw�肳��Ă���d�w�d�̃p�X���m�F���ĉ������B\n\n")
				_T("�@") + exe_path + _T("�@�@\n");
			MessageBox(G_dlg->get_handle(), mess.c_str(), _T("�G���["), MB_OK | MB_ICONWARNING);
		}
		else	{
			TSTR mess =
				_T("�u���s�t�@�C���v �Ɏw�肳��Ă���d�w�d�t�@�C�����Í������܂��B\n\n")
				_T("��d�w�d�̃p�X��\n\n")
				_T("�@�@") + exe_path + _T("�@�@\n\n")
				_T("��Í��̕������\n\n")
				_T("�@�@") + exe_angou_str + _T("�@�@\n\n")
				_T("���P�x�Í������ꂽ�d�w�d�t�@�C�����ĈÍ������鎖�͂ł��܂���B\n\n")
				_T("�Í������ɖ߂������ꍇ �� �Ⴄ�Í��ɂ������ꍇ�́A\n")
				_T("�Í�������Ă��Ȃ����̂d�w�d�t�@�C���������Ă��ăR�s�[���ĉ������B\n");
			int res = MessageBox(G_dlg->get_handle(), mess.c_str(), _T("�d�w�d�̈Í���"), MB_OKCANCEL | MB_ICONINFORMATION);
			if (res == IDOK)	{
				 C_tnms_exe_angou::angou(G_dlg->get_handle(), exe_path, exe_angou_str);
			}
		}
	}
}

