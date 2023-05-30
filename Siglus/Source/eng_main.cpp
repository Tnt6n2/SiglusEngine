#include	"pch.h"
#include	"tnm_eng.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_net.h"
#include	"data/tnm_dlg_manager.h"
#include	"data/tnm_gan_manager.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"

#include	"engine/eng_dialog.h"
#include	"engine/eng_config.h"
#include	"engine/ifc_sound.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// �v���g�^�C�v�錾
// ================================================================

// �G���W���F���b�Z�[�W���[�v
bool tnm_eng_message_loop();

// �G���W���F�ُ�I��
void tnm_eng_fatal_error();
void tnm_eng_fatal_error(CTSTR& str);

// ****************************************************************
// �O���[�o���ȃ|�C���^
// ================================================================
C_play_manager						G_pm;				// �v���C�}�l�[�W��
C_tnm_wnd*							Gp_wnd;				// �E�B���h�E
C_tnm_movie_wnd*					Gp_movie_wnd;		// ���[�r�[�E�B���h�E
C_tnm_net*							Gp_net;				// �l�b�g���[�N
C_tnm_dlg_manager*					Gp_dlg_mng;			// �_�C�A���O�}�l�[�W��
C_tnm_input*						Gp_input;			// ����
C_input_state*						Gp_cur_input;		// ����̓���
C_input_state*						Gp_last_input;		// �O��̓���
C_tnm_system_info*					Gp_system_info;		// �V�X�e�����
C_tnm_system_config*				Gp_system_config;	// �V�X�e���R���t�B�O
C_tnm_global_data*					Gp_global;			// �O���[�o���f�[�^
C_tnm_dir*							Gp_dir;				// �f�B���N�g��
C_tnm_config*						Gp_config;			// ���[�U�R���t�B�O
C_tnm_syscom*						Gp_syscom;			// �V�X�e���R�}���h
C_tnm_local_data*					Gp_local;			// �V�[���f�[�^
S_tnm_local_save*					Gp_local_save;		// ���[�J���Z�[�u
S_tnm_save_history*					Gp_save_history;	// �Z�[�u����
S_tnm_backlog_save*					Gp_backlog_save;	// �o�b�N���O�Z�[�u
C_tnm_scene_lexer*					Gp_lexer;			// ���N�T�[
C_tnm_stack*						Gp_stack;			// �X�^�b�N
C_tnm_timer*						Gp_timer;			// �^�C�}�[
C_tnm_icon*							Gp_icon;			// �A�C�R��
C_tnm_cursor_list*					Gp_cursor_list;		// �J�[�\��
C_tnm_font*							Gp_font;			// �t�H���g
C_font_name_list*					Gp_font_name_list;	// �t�H���g�����X�g
C_tnm_cg_table*						Gp_cg_table;		// �b�f�e�[�u��
C_tnm_bgm_table*					Gp_bgm_table;		// �a�f�l�e�[�u��
C_tnm_thumb_table*					Gp_thumb_table;		// �T���l�C���e�[�u��
ARRAY< ARRAY<BYTE> >*				Gp_read_flag;		// ���ǃt���O
C_tnm_namae*						Gp_namae;			// �y���O�z
ARRAY<C_tnm_chrkoe>*				Gp_chrkoe;			// �L�����N�^�[����
C_tnm_color_table*					Gp_color_table;		// �J���[�e�[�u��
C_tnm_tonecurve*					Gp_tonecurve;		// �g�[���J�[�u
C_tnm_fog*							Gp_fog;				// �t�H�O
C_tnm_wipe*							Gp_wipe;			// ���C�v
C_tnm_btn_mng*						Gp_btn_mng;			// �{�^���}�l�[�W��
C_tnm_btn_action_template_list*		Gp_btn_action_list;	// �{�^���A�N�V����
C_tnm_btn_se_template_list*			Gp_btn_se_list;		// �{�^���V�X�e����
C_tnm_script_input*					Gp_script_input;	// �X�N���v�g����
C_tnm_fetch_point*					Gp_fetch_point;		// �t�F�b�`�|�C���g
C_tnm_click_disable*				Gp_click_disable;	// �N���b�N�֎~
C_tnm_error*						Gp_error;			// �G���[
C_tnm_debug_dll*					Gp_debug_dll;		// �f�o�b�O dll
C_tnm_flag_ini*						Gp_flag_ini;		// Flag.ini
C_tnm_steam*						Gp_steam;			// Steam

C_elm_flag*							Gp_flag;				// �t���O
C_elm_user_inc_prop*				Gp_user_inc_prop;		// ���[�U�C���N�v���p�e�B
C_elm_user_scn_prop_list*			Gp_user_scn_prop_list;	// ���[�U�V�[���v���p�e�B
C_elm_database_list*				Gp_database_list;		// �f�[�^�x�[�X
C_elm_counter_list*					Gp_counter_list;		// �J�E���^
C_elm_frame_action*					Gp_frame_action;		// �t���[���A�N�V����
C_elm_frame_action_list*			Gp_frame_action_ch;		// �t���[���A�N�V�����`�����l��
C_elm_g00_buf_list*					Gp_g00_buf_list;		// �f�O�O�o�b�t�@
C_elm_mask_list*					Gp_mask_list;			// �}�X�N
C_elm_stage_list*					Gp_stage_list;			// �X�e�[�W
C_elm_msg_back*						Gp_msg_back;			// ���b�Z�[�W�o�b�N
C_elm_screen*						Gp_screen;				// �X�N���[��
C_elm_sound*						Gp_sound;				// �T�E���h
C_elm_pcm_event_list*				Gp_pcm_event_list;		// ���ʉ��C�x���g
C_elm_editbox_list*					Gp_editbox_list;		// �G�f�B�b�g�{�b�N�X
C_elm_call_list*					Gp_call_list;			// �R�[��
C_elm_excall*						Gp_excall;				// �d�w�R�[��

// ****************************************************************
// �G���W���F�R���X�g���N�^
// ================================================================
C_tnm_eng::C_tnm_eng()
{
	m_init_success_flag = false;

	// �O���[�o���ȃ|�C���^�̐ݒ�
	Gp_wnd = &m_wnd;
	Gp_movie_wnd = &m_movie_wnd;
	Gp_net = &m_net;
	Gp_dlg_mng = &m_dlg_mng;
	Gp_input = &m_input;
	Gp_cur_input = &m_cur_input;
	Gp_last_input = &m_last_input;
	Gp_system_info = &m_system_info;
	Gp_system_config = &m_system_config;
	Gp_global = &m_global;
	Gp_dir = &m_dir;
	Gp_config = &m_config;
	Gp_syscom = &m_syscom;
	Gp_local = &m_local;
	Gp_local_save = &m_local_save;
	Gp_save_history = &m_save_history;
	Gp_backlog_save = &m_backlog_save;
	Gp_lexer = &m_lexer;
	Gp_stack = &m_stack;
	Gp_timer = &m_timer;
	Gp_icon = &m_icon;
	Gp_cursor_list = &m_cursor_list;
	Gp_font = &m_font;
	Gp_font_name_list = &m_font_name_list;
	Gp_cg_table = &m_cg_table;
	Gp_bgm_table = &m_bgm_table;
	Gp_thumb_table = &m_thumb_table;
	Gp_read_flag = &m_read_flag_list;
	Gp_namae = &m_namae;
	Gp_chrkoe = &m_chrkoe;
	Gp_color_table = &m_color_table;
	Gp_tonecurve = &m_tonecurve;
	Gp_fog = &m_fog;
	Gp_wipe = &m_wipe;
	Gp_btn_mng = &m_btn_mng;
	Gp_btn_action_list = &m_btn_action_template_list;
	Gp_btn_se_list = &m_btn_se_template_list;
	Gp_script_input = &m_script_input;
	Gp_fetch_point = &m_fetch_point;
	Gp_click_disable = &m_click_disable;
	Gp_error = &m_error;
	Gp_debug_dll = &m_debug_dll;
	Gp_flag_ini = &m_flag_ini;

	Gp_flag = &m_flag;
	Gp_user_inc_prop = &m_user_inc_prop;
	Gp_user_scn_prop_list = &m_user_scn_prop_list;
	Gp_database_list = &m_database_list;
	Gp_counter_list = &m_counter_list;
	Gp_frame_action = &m_frame_action;
	Gp_frame_action_ch = &m_frame_action_ch_list;
	Gp_g00_buf_list = &m_g00_buf_list;
	Gp_mask_list = &m_mask_list;
	Gp_stage_list = &m_stage_list;
	Gp_msg_back = &m_msg_back;
	Gp_screen = &m_screen;
	Gp_sound = &m_sound;
	Gp_pcm_event_list = &m_pcm_event_list;
	Gp_editbox_list = &m_editbox_list;
	Gp_call_list = &m_call_list;
	Gp_excall = &m_excall;

#if ___USE_STEAM
	Gp_steam = &m_steam;
#endif
}

// ****************************************************************
// �G���W���F���C������
// ================================================================
bool C_tnm_eng::main()
{
	// �G���W��������������
	try	{
		if (!init())	{
			tnm_eng_fatal_error();
		}
	}
	// ��O���󂯎���ĕ\������
	catch (TCHAR* str)	{
		tnm_eng_fatal_error(_T("�Q�[���̏����������ŗ�O���������܂����B�Q�[�����I�����܂��B\n") + TSTR(str));
	}
	catch (TSTR str)	{
		tnm_eng_fatal_error(_T("�Q�[���̏����������ŗ�O���������܂����B�Q�[�����I�����܂��B\n") + str);
	}
	catch (const std::exception& e)	{
		tnm_eng_fatal_error(_T("�Q�[���̏����������ŗ�O���������܂����B�Q�[�����I�����܂��B\n") + MBSTR_to_TSTR(e.what()));
	}

	// �G���[���N���Ă��Ȃ���΃G���W�����J�n
	if (!Gp_global->game_end_flag)	{

		// �G���W���̊J�n����
		try	{
			if (!start())	{
				tnm_eng_fatal_error(_T("�Q�[���̊J�n�����ŃG���[���������܂����B�Q�[�����I�����܂��B\n") + get_last_error_msg());
			}
		}
		// ��O���󂯎���ĕ\������
		catch (TCHAR* str)	{
			tnm_eng_fatal_error(_T("�Q�[���̊J�n�����ŗ�O���������܂����B�Q�[�����I�����܂��B\n") + TSTR(str));
		}
		catch (TSTR str)	{
			tnm_eng_fatal_error(_T("�Q�[���̊J�n�����ŗ�O���������܂����B�Q�[�����I�����܂��B\n") + str);
		}
		catch (const std::exception& e)	{
			tnm_eng_fatal_error(_T("�Q�[���̊J�n�����ŗ�O���������܂����B�Q�[�����I�����܂��B\n") + MBSTR_to_TSTR(e.what()));
		}
	}

	// �E�B���h�E���������Q�[�����[�v�����s��������
	while (Gp_wnd->get_handle())	{

		// �Q�[���I������i�O�̂��߂����ł��j
		if (Gp_wnd->get_handle() && Gp_global->game_end_flag)	{
			Gp_wnd->close();
		}

		// ���b�Z�[�W���[�v���s���BWM_QUIT �Ŕ�����
		if (!tnm_eng_message_loop())
			break;

		// �Q�[���I������i���̈ʒu�͕K�{�ł��BWM_CLOSE �ȂǂŃt���O�������Ƃ�����܂��B�j
		if (Gp_wnd->get_handle() && Gp_global->game_end_flag)	{
			Gp_wnd->close();
		}

		// �L���v�`���[�p�̋󃋁[�v
		while (1)	{

			// �Q�[���I������i�t���[�������Ńt���O�����ꍇ�̏����j
			if (Gp_global->game_end_flag)
				break;

			// �G���W���̃t���[������
			try	{
				if (!frame())	{
					tnm_eng_fatal_error(_T("�Q�[���̃t���[�������ŃG���[���������܂����B�Q�[�����I�����܂��B\n") + get_last_error_msg());
				}
			}
			// ��O���󂯎���ĕ\������
			catch (TCHAR* str)	{
				tnm_eng_fatal_error(_T("�Q�[���̃t���[�������ŗ�O���������܂����B�Q�[�����I�����܂��B\n") + TSTR(str));
			}
			catch (TSTR str)	{
				tnm_eng_fatal_error(_T("�Q�[���̃t���[�������ŗ�O���������܂����B�Q�[�����I�����܂��B\n") + str);
			}
			catch (const std::exception& e)	{
				tnm_eng_fatal_error(_T("�Q�[���̃t���[�������ŗ�O���������܂����B�Q�[�����I�����܂��B\n") + MBSTR_to_TSTR(e.what()));
			}

			// �Q�[���I������i�t���[�������Ńt���O�����ꍇ�̏����j
			if (Gp_global->game_end_flag)
				break;

			// �`�揈��
			if (Gp_global->disp_flag)	{

				// �G���W���̕`�揈��
				try	{
					if (!Gp_wnd->disp_proc())	{
						tnm_eng_fatal_error(_T("�Q�[���̕`�揈���ŃG���[���������܂����B�Q�[�����I�����܂��B\n") + get_last_error_msg());
					}
				}
				// ��O���󂯎���ĕ\������
				catch (TCHAR* str)	{
					tnm_eng_fatal_error(_T("�Q�[���̕`�揈���ŗ�O���������܂����B�Q�[�����I�����܂��B\n") + TSTR(str));
				}
				catch (TSTR str)	{
					tnm_eng_fatal_error(_T("�Q�[���̕`�揈���ŗ�O���������܂����B�Q�[�����I�����܂��B\n") + str);
				}
				catch (const std::exception& e)	{
					tnm_eng_fatal_error(_T("�Q�[���̕`�揈���ŗ�O���������܂����B�Q�[�����I�����܂��B\n") + MBSTR_to_TSTR(e.what()));
				}
			}

			// �L���v�`���[�̌㏈��
			if (Gp_global->capture_type == TNM_CAPTURE_TYPE_THUMB)	{
				tnm_stack_push_int(Gp_global->capture_time);	// �L���v�`���[�R�}���h�̖߂�l��ς�
			}
			// �L���v�`���[�I��
			Gp_global->capture_type = TNM_CAPTURE_TYPE_NONE;

			// �Q�[���I������i�`�揈���Ńt���O�����ꍇ�̏����j
			if (Gp_global->game_end_flag)
				break;

			// ��ʍX�V���s���Ȃ甲����
			if (Gp_global->present_flag)
				break;
		}

		// �Q�[���I������i��̋󃋁[�v���Ńt���O�����ꍇ�̏����j
		if (Gp_wnd->get_handle() && Gp_global->game_end_flag)	{
			Gp_wnd->close();
		}

		if (!Gp_global->game_end_flag)	{

			// ��ʂ��X�V����
			Gp_wnd->invalidate_rect(NULL, false);

			// �L���v�V�������X�V����
			Gp_wnd->update_caption();

			// �_�C�A���O�}�l�[�W���̏���
			Gp_dlg_mng->frame();

			// ���\�[�X�}�l�[�W���𐮗�����
			static DWORD last_time = ::timeGetTime();
			DWORD now_time = ::timeGetTime();
			if ((int)(now_time - last_time) >= 1000)	{
				G_rsm.organize();
				G_gan_manager.organize();		// �f�`�m�f�[�^�̃��\�[�X�}�l�[�W��
				last_time = now_time;
			}

			// �����[�r�[�Đ����̓X���[�v���͂���
			if (tnm_is_movie_playing())	{
				Sleep(100);
			}
			// ���ŏ������̓X���[�v���͂���
			else if (IsIconic(G_app.h_wnd))	{
				Sleep(100);
			}
			// ���ݒ�ɂ���Ă̓X���[�v���͂���
			else if (Gp_config->system.sleep_flag)	{
				Sleep(10);
			}
		}
	}

	// �E�B���h�E���J���Ă���΃E�B���h�E�����
	//Gp_wnd->close();

	// �G���W�����I��������
	try	{
		if (!end())	{
			tnm_eng_fatal_error(_T("�Q�[���̏I�������ŃG���[���������܂����B\n") + get_last_error_msg());
		}
	}
	// ��O���󂯎���ĕ\������
	catch (TCHAR* str)	{
		tnm_eng_fatal_error(_T("�Q�[���̏I�������ŗ�O���������܂����B\n") + TSTR(str));
	}
	catch (TSTR str)	{
		tnm_eng_fatal_error(_T("�Q�[���̏I�������ŗ�O���������܂����B\n") + str);
	}
	catch (const std::exception& e)	{
		tnm_eng_fatal_error(_T("�Q�[���̏I�������ŗ�O���������܂����B\n") + MBSTR_to_TSTR(e.what()));
	}

	return true;
}

// ****************************************************************
// �G���W���F���b�Z�[�W���[�v
// ================================================================
bool tnm_eng_message_loop()
{
	MSG msg;

	// ���b�Z�[�W�����o��
	while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))	{

		// WM_QUIT �������� false
		if (!GetMessage(&msg, NULL, 0, 0))
			return false;
		// ���b�Z�[�W����
		if (!G_app.default_message_proc(&msg))
			return false;
	}

	// ���b�Z�[�W�����Ȃ�������߂�
	return true;
}

// ****************************************************************
// �G���W���F�ُ�I������
// ================================================================
void tnm_eng_fatal_error()
{
	// �ُ�I������t���O
	Gp_global->game_end_flag = true;
	Gp_global->game_end_no_warning = true;
	Gp_global->game_end_save_done = true;
}

void tnm_eng_fatal_error(CTSTR& str)
{
	// �ُ�I���̏ꍇ�̓��b�Z�[�W�{�b�N�X���o��
	// MB_ICONERROR ���Ɖ����S���Ɉ����̂ŁAMB_ICONWARNING �ɂ��Ă���
	message_box(str, Gp_ini->game_name, MB_ICONWARNING);

	// �ُ�I������t���O
	Gp_global->game_end_flag = true;
	Gp_global->game_end_no_warning = true;
	Gp_global->game_end_save_done = true;
}

