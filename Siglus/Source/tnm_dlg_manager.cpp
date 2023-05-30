#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_flag_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_dlg_manager.h"
#include	"element/elm_stage.h"
#include	"engine/eng_scene.h"
#include	"../resource.h"

// ****************************************************************
// �_�C�A���O�E�B���h�E
// ================================================================

// �o�^
void C_tnm_dlg_manager::regist()
{
	// ���ݒ�x�[�X
	dlg_list.push_back(&cfg_wnd_config_base);					// �x�[�X
	dlg_list.push_back(&cfg_wnd_config_base_full_mode);			// �x�[�X�t�����[�h

	// ���ݒ�\��
	dlg_list.push_back(&cfg_wnd_solo_screen);					// �\���F��ʃ��[�h
	dlg_list.push_back(&cfg_wnd_solo_font_msgspd);				// �\���F���b�Z�[�W�i�t�H���g�A�������x�j
	dlg_list.push_back(&cfg_wnd_solo_font);						// �\���F�t�H���g
	dlg_list.push_back(&cfg_wnd_solo_msgspd);					// �\���F�������x
	dlg_list.push_back(&cfg_wnd_solo_volume_bgmfade);			// �\���F���ʁi���ʁA�a�f�l�t�F�[�h�j
	dlg_list.push_back(&cfg_wnd_solo_volume);					// �\���F����
	dlg_list.push_back(&cfg_wnd_solo_bgmfade);					// �\���F�a�f�l�t�F�[�h
	dlg_list.push_back(&cfg_wnd_solo_koemode_chrkoe);			// �\���F�����i�������[�h�A�L�����N�^�[�����j
	dlg_list.push_back(&cfg_wnd_solo_koemode);					// �\���F�������[�h
	dlg_list.push_back(&cfg_wnd_solo_chrkoe);					// �\���F�L�����N�^�[����
	dlg_list.push_back(&cfg_wnd_solo_mwndbk);					// �\���F�E�B���h�E�w�i�F
	dlg_list.push_back(&cfg_wnd_solo_automode);					// �\���F�I�[�g���[�h
	dlg_list.push_back(&cfg_wnd_solo_jitan);					// �\���F���Z�Đ�
	dlg_list.push_back(&cfg_wnd_solo_else);						// �\���F���̑�
	dlg_list.push_back(&cfg_wnd_solo_system);					// �\���F�V�X�e��
	dlg_list.push_back(&cfg_wnd_solo_movtype);					// �\���F���[�r�[�̍Đ����@

	// �Z�[�u�_�C�A���O
	dlg_list.push_back(&sys_wnd_solo_save);						// �Z�[�u�_�C�A���O
	// ���[�h�_�C�A���O
	dlg_list.push_back(&sys_wnd_solo_load);						// ���[�h�_�C�A���O
	// �c�C�[�g�_�C�A���O
	dlg_list.push_back(&sys_wnd_solo_tweet);					// �c�C�[�g�_�C�A���O

	// �f�o�b�O�E�B���h�E�i�f�o�b�O���̂݁j
	if (Gp_global->debug_flag)	{
		dlg_list.push_back(&db_wnd_info_debug);						// �f�o�b�O�ݒ�
		dlg_list.push_back(&db_wnd_info_scn_control);				// �V�[���R���g���[��
		dlg_list.push_back(&db_wnd_info_trace);						// �g���[�X
		dlg_list.push_back(&db_wnd_info_error);						// �G���[���
		dlg_list.push_back(&db_wnd_info_d3d_device);				// Direct3D �f�o�C�X���
		dlg_list.push_back(&db_wnd_info_d3d_resource);				// Direct3D ���\�[�X���
		dlg_list.push_back(&db_wnd_info_input);						// ���͏��
		dlg_list.push_back(&db_wnd_info_system);					// �V�X�e�����
		dlg_list.push_back(&db_wnd_info_dir);						// �f�B���N�g�����
		dlg_list.push_back(&db_wnd_info_script);					// �X�N���v�g���
		dlg_list.push_back(&db_wnd_info_stack);						// �X�^�b�N���
		dlg_list.push_back(&db_wnd_info_call_history);				// �Ăяo������
		dlg_list.push_back(&db_wnd_info_proc_stack);				// �v���Z�X�X�^�b�N���
		dlg_list.push_back(&db_wnd_info_namae);						// �y���O�z���
		dlg_list.push_back(&db_wnd_info_read_flag);					// ���ǃt���O���
		dlg_list.push_back(&db_wnd_info_cg_table);					// �b�f�e�[�u��
		dlg_list.push_back(&db_wnd_info_bgm_table);					// �a�f�l�e�[�u��
		dlg_list.push_back(&db_wnd_info_element);					// �G�������g���
		for (int i = 0; i < TNM_FLAG_CNT; i++)	{
			dlg_list.push_back(&db_wnd_info_flag[i]);					// �t���O���
		}
		for (int i = 0; i < 20; i++)	{
			dlg_list.push_back(&db_wnd_info_flag_ini[i]);				// Flag.ini ���
		}
		dlg_list.push_back(&db_wnd_info_user_flag);					// ���[�U�t���O���
		dlg_list.push_back(&db_wnd_info_user_call_flag);			// ���[�U�R�[���t���O���
		dlg_list.push_back(&db_wnd_info_namae_flag);				// ���O�t���O���
		dlg_list.push_back(&db_wnd_info_counter);					// �J�E���^���
		dlg_list.push_back(&db_wnd_info_btn);						// �{�^�����
		dlg_list.push_back(&db_wnd_info_group);						// �O���[�v���
		dlg_list.push_back(&db_wnd_info_world);						// ���[���h���
		for (int i = 0; i < 6; i++)	{
			dlg_list.push_back(&db_wnd_info_object[i]);					// �I�u�W�F�N�g���
		}
		dlg_list.push_back(&db_wnd_info_mwnd);						// ���b�Z�[�W�E�B���h�E���
		dlg_list.push_back(&db_wnd_info_sound);						// �T�E���h���
		dlg_list.push_back(&db_wnd_info_system_sound);				// �{�����[�����
		dlg_list.push_back(&db_wnd_info_koe);						// ���E�H�b�`���
		dlg_list.push_back(&db_wnd_info_save_history);				// �Z�[�u�|�C���g����
	}
}

// ������
void C_tnm_dlg_manager::init()
{
	for (int i = 0; i < (int)dlg_list.size(); i++)	{
		dlg_list[i]->init();
	}

	// �t���O���_�C�A���O�̓y�[�W�ԍ����w��
	TSTR flag_name[] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E"), _T("F"), _T("X"), _T("S"), _T("G"), _T("Z"), _T("M"), _T("L"), _T("K")};
	for (int i = 0; i < TNM_FLAG_CNT; i++)	{
		db_wnd_info_flag[i].regist(str_format(_T("db_wnd_info_flag_%s"), flag_name[i].c_str()), false, true, -1);
		db_wnd_info_flag[i].regist_caption_name(str_format(_T("�t���O%s"), flag_name[i].c_str()));
		db_wnd_info_flag[i].set_page_no(i);
	}
	// Flag.ini ���_�C�A���O�̓y�[�W�ԍ����w��
	for (int i = 0; i < 20; i++)	{
		db_wnd_info_flag_ini[i].regist(str_format(_T("db_wnd_info_flag_ini_%02d"), i), false, true, -1);
		if (Gp_flag_ini->page[i].title.empty())	{
			db_wnd_info_flag_ini[i].regist_caption_name(str_format(_T("�t���O%02d"), i));
		}
		else	{
			db_wnd_info_flag_ini[i].regist_caption_name(Gp_flag_ini->page[i].title);
		}
		db_wnd_info_flag_ini[i].set_page_no(i);
	}
	// �I�u�W�F�N�g���_�C�A���O�̓y�[�W�ԍ����w��
	TSTR regist_name[] = {_T("back_object"), _T("front_object"), _T("next_object"), _T("excall_back_object"), _T("excall_front_object"), _T("excall_next_object")};
	TSTR caption_name[] = {_T("back.object"), _T("front.object"), _T("next.object"), _T("excall.back.object"), _T("excall.front.object"), _T("excall.next.object")};
	for (int i = 0; i < 6; i++)	{
		db_wnd_info_object[i].regist(str_format(_T("db_wnd_info_object_%s"), regist_name[i].c_str()), false, true, -1);
		db_wnd_info_object[i].regist_caption_name(_T("�I�u�W�F�N�g���i") + caption_name[i] + _T("�j"));
		db_wnd_info_object[i].set_page_no(i);
	}
}

// ����ifree �� WM_CLOSE ����������Aon_close ���ŃX�e�[�^�X���ۑ������j
void C_tnm_dlg_manager::free()
{
	for (int i = 0; i < (int)dlg_list.size(); i++)	{
		dlg_list[i]->free();
	}
}

// �����X�e�[�^�X�ǂݍ���
void C_tnm_dlg_manager::load_initial_state()
{
	for (int i = 0; i < (int)dlg_list.size(); i++)	{
		dlg_list[i]->load_state();
	}
}

// �����X�e�[�^�X�ۑ�
void C_tnm_dlg_manager::save_initial_state()
{
	for (int i = 0; i < (int)dlg_list.size(); i++)	{
		dlg_list[i]->save_state();
	}
}

// �����I�[�v��
void C_tnm_dlg_manager::initial_open()
{
	for (int i = 0; i < (int)dlg_list.size(); i++)	{
		dlg_list[i]->initial_open();
	}
}

// �֎~�^����ݒ�
void C_tnm_dlg_manager::set_enable(bool flag)
{
	for (int i = 0; i < (int)dlg_list.size(); i++)	{
		if (dlg_list[i]->get_handle())	{
			dlg_list[i]->set_enable(flag);
		}
	}
}

// �}�E�X�J�[�\�����������Ă���^�������Ă��Ȃ����擾
bool C_tnm_dlg_manager::check_mouse_over()
{
	C_point mp = get_mouse_pos();
	for (int i = 0; i < (int)dlg_list.size(); i++)	{
		if (dlg_list[i]->get_handle() && dlg_list[i]->get_enable())	{
			C_rect wr = dlg_list[i]->get_window_rect();
			if (wr.left <= mp.x && mp.x < wr.right && wr.top <= mp.y && mp.y < wr.bottom)	{
				return true;	// ��ɏ���Ă���I
			}
		}
	}

	// �G�������g���E�B���h�E���ʂɃ`�F�b�N
	if (db_wnd_info_element.check_mouse_over(mp))
		return true;	// ��ɏ���Ă���I

	// ��ɏ���Ă��Ȃ�
	return false;
}

// �t���[������
void C_tnm_dlg_manager::frame()
{
	C_elm_stage* p_back = &(*Gp_stage_list)[TNM_STAGE_BACK];
	C_elm_stage* p_front = &(*Gp_stage_list)[TNM_STAGE_FRONT];
	C_elm_stage* p_next = &(*Gp_stage_list)[TNM_STAGE_NEXT];

	C_elm_group_list* group_list[] = {&p_back->m_group_list, &p_front->m_group_list, &p_next->m_group_list};
	C_elm_mwnd_list* mwnd_list[] = {&p_back->m_mwnd_list, &p_front->m_mwnd_list, &p_next->m_mwnd_list};

	// ���ݒ�x�[�X
	cfg_wnd_config_base.frame();			// �x�[�X
	cfg_wnd_config_base_full_mode.frame();	// �x�[�X�t�����[�h

	// ���ݒ�\��
	cfg_wnd_solo_screen.frame();			// �\���F��ʃ��[�h
	cfg_wnd_solo_font_msgspd.frame();		// �\���F���b�Z�[�W�i�t�H���g�A�������x�j
	cfg_wnd_solo_font.frame();				// �\���F�t�H���g
	cfg_wnd_solo_msgspd.frame();			// �\���F�������x
	cfg_wnd_solo_volume_bgmfade.frame();	// �\���F���ʁi���ʁA�a�f�l�t�F�[�h�j
	cfg_wnd_solo_volume.frame();			// �\���F����
	cfg_wnd_solo_bgmfade.frame();			// �\���F�a�f�l�t�F�[�h
	cfg_wnd_solo_koemode_chrkoe.frame();	// �\���F�����i�������[�h�A�L�����N�^�[�����j
	cfg_wnd_solo_koemode.frame();			// �\���F�������[�h
	cfg_wnd_solo_chrkoe.frame();			// �\���F�L�����N�^�[����
	cfg_wnd_solo_mwndbk.frame();			// �\���F�E�B���h�E�w�i�F
	cfg_wnd_solo_automode.frame();			// �\���F�I�[�g���[�h
	cfg_wnd_solo_jitan.frame();				// �\���F���Z�Đ�
	cfg_wnd_solo_else.frame();				// �\���F���̑�
	cfg_wnd_solo_system.frame();			// �\���F�V�X�e��
	cfg_wnd_solo_movtype.frame();			// �\���F���[�r�[�̍Đ����@

	// �Z�[�u�_�C�A���O
	sys_wnd_solo_save.frame();				// �Z�[�u�_�C�A���O
	// ���[�h�_�C�A���O
	sys_wnd_solo_load.frame();				// ���[�h�_�C�A���O
	// �c�C�[�g�_�C�A���O
	sys_wnd_solo_tweet.frame();				// �c�C�[�g�_�C�A���O

	// �f�o�b�O�E�B���h�E�i�f�o�b�O���̂݁j
	if (Gp_global->debug_flag)	{
		db_wnd_info_debug.frame();
		db_wnd_info_scn_control.frame();
		db_wnd_info_trace.frame();
		db_wnd_info_error.frame();
		db_wnd_info_d3d_device.frame();
		db_wnd_info_d3d_resource.frame();
		db_wnd_info_input.frame();
		db_wnd_info_system.frame();
		db_wnd_info_dir.frame();
		db_wnd_info_script.frame();
		db_wnd_info_stack.frame();
		db_wnd_info_call_history.frame();
		db_wnd_info_proc_stack.frame();
		db_wnd_info_namae.frame();
		db_wnd_info_cg_table.frame();
		db_wnd_info_bgm_table.frame();
		db_wnd_info_read_flag.frame();
		db_wnd_info_element.frame();
		for (int i = 0; i < TNM_FLAG_CNT; i++)	{
			db_wnd_info_flag[i].frame();
		}
		for (int i = 0; i < 20; i++)	{
			db_wnd_info_flag_ini[i].frame();
		}
		db_wnd_info_user_flag.frame();
		db_wnd_info_user_call_flag.frame();
		db_wnd_info_namae_flag.frame();
		db_wnd_info_counter.frame() ;
		db_wnd_info_world.frame();
		db_wnd_info_btn.frame();
		db_wnd_info_group.frame(group_list, sizeof_array(group_list));
		for (int i = 0; i < 6; i++)	{
			db_wnd_info_object[i].frame();
		}
		db_wnd_info_mwnd.frame(mwnd_list, sizeof_array(mwnd_list));
		db_wnd_info_sound.frame();
		db_wnd_info_system_sound.frame();
		db_wnd_info_koe.frame();
		db_wnd_info_save_history.frame();
	}

	// �E�B���h�E���W��␳����
	for (int i = 0; i < (int)dlg_list.size(); i++)	{
		if (dlg_list[i]->get_handle())	{
			dlg_list[i]->rep_window_pos_outside_desktop();
		}
	}
}
