#include	"pch.h"
#include	"ifc_eng.h"
#include	"tnm_eng.h"			// ������

#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_timer.h"
#include	"data/tnm_moji_manager.h"
#include	"data/tnm_gan_manager.h"
#include	"element/elm_mov.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_scene.h"
#include	"engine/ifc_stack.h"
#include	"engine/ifc_sound.h"
#include	"dialog/db_wnd_info_system.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�V�X�e��
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_system")

// ������
void C_db_wnd_info_system::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// �L���v�V�������o�^
	regist_caption_name(_T("�V�X�e�����"));

	// ���X�g�r���[����
	listview_add_column(_T("state"), _T("�X�e�[�g"), 100);
	listview_add_column(_T("value"), _T("�l"), 100);
}

// �������t�@���N�V����
void C_db_wnd_info_system::on_init_func()
{
	// ���ڂ�ǉ�
	lv.add_item(0, _T("�t���[�����[�g"));
	lv.add_item(0, _T("�Z�[�u�|�C���g"));
	lv.add_item(0, _T("�Z�[�u�L���v�`���["));
	lv.add_item(0, _T("�I�����|�C���g"));
	lv.add_item(0, _T("�V�X�e���R�[��"));
	lv.add_item(0, _T("�V�X�e�����C�v"));
	lv.add_item(0, _T("�Q�[���^�C�}�["));
	lv.add_item(0, _T("�X�^�b�N�T�C�Y"));
	lv.add_item(0, _T("�G�������g�X�^�b�N�T�C�Y"));
	lv.add_item(0, _T("���b�Z�[�W�o�b�N�Z�[�u�T�C�Y"));

	lv.add_item(0, _T("���݂̎�����"));
	lv.add_item(0, _T("���[�J��������"));
	lv.add_item(0, _T("���[�J���Q�[������"));
	lv.add_item(0, _T("���[�J�����C�v����"));
	lv.add_item(0, _T("Ctrl�X�L�b�v"));
	lv.add_item(0, _T("�X�L�b�v���x"));
	lv.add_item(0, _T("�X�L�b�v���~�߂�"));

	lv.add_item(0, _T("�A�N�e�B�u"));
	lv.add_item(0, _T("�t�H�[�J�X"));
	lv.add_item(0, _T("�t�H�[�J�X�i�G�f�B�b�g�{�b�N�X�j"));

	lv.add_item(0, _T("�ŏI�a�f�l�t�F�[�h�l"));
	lv.add_item(0, _T("�ŏI�a�f�l�t�F�[�h�l�Q"));
	lv.add_item(0, _T("�ŏI�a�f�l�{�����[��"));
	lv.add_item(0, _T("�ŏI���{�����[��"));
	lv.add_item(0, _T("�ŏI���ʉ��{�����[��"));
	lv.add_item(0, _T("�ŏI�V�X�e�����{�����[��"));
	lv.add_item(0, _T("�ŏI���[�r�[�{�����[��"));
	lv.add_item(0, _T("���[�r�[�{�����[���P�i�e�X�g���j"));
	lv.add_item(0, _T("���[�r�[�{�����[���Q�i�e�X�g���j"));
	lv.add_item(0, _T("�o�b�N�O���E���h�Ŗ������Đ�����"));

	lv.add_item(0, _T("�r�f�I�������g�p��"));
	lv.add_item(0, _T("�ő咸�_�o�b�t�@�T�C�Y"));
	lv.add_item(0, _T("�����m�ې�"));
	lv.add_item(0, _T("����������"));
	lv.add_item(0, _T("������"));
	lv.add_item(0, _T("�f�`�m�f�[�^�m�ې�"));

	lv.add_item(0, _T("�e�X�g�P"));
	lv.add_item(0, _T("�e�X�g�Q"));
	lv.add_item(0, _T("�e�X�g�R"));
	lv.add_item(0, _T("�e�X�g�S"));
	lv.add_item(0, _T("�e�X�g�T"));
	lv.add_item(0, _T("�e�X�g�U"));
	lv.add_item(0, _T("�e�X�g�V"));
	lv.add_item(0, _T("�e�X�g�W"));
	lv.add_item(0, _T("�e�X�g�X"));
}

// ����t�@���N�V����
void C_db_wnd_info_system::on_close_func()
{
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_system::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

	// ���݂̒l���擾����
	cd.frame_rate = Gp_global->frame_rate_100msec_total;
	cd.save_point = Gp_eng->m_local_save.save_msg;
	cd.sel_point_exist = Gp_eng->m_sel_save.list.empty() ? false : true;
	cd.capture_exist_flag = Gp_wnd->m_capture_texture_for_save_thumb ? true : false;
	cd.ex_call_flag = Gp_global->ex_call_flag;
	cd.game_timer_move_flag = Gp_global->game_timer_move_flag;
	cd.system_wipe_flag = Gp_global->system_wipe_flag;
	cd.stack_size = Gp_stack->int_now_cnt;
	cd.element_stack_size = (int)Gp_stack->stack_point_list.size();
	cd.msg_back_save_size = (int)Gp_backlog_save->map.size();

	cd.real_time = Gp_timer->real_time;
	cd.local_real_time = Gp_timer->local_real_time;
	cd.local_game_time = Gp_timer->local_game_time;
	cd.local_wipe_time = Gp_timer->local_wipe_time;
	cd.ctrl_skip_flag = Gp_global->ctrl_skip_flag;
	cd.skip_speed = Gp_global->disp_because_msg_wait_cnt_max;
	cd.cant_auto_skip_before_click = Gp_global->cant_auto_skip_before_click;

	cd.active_flag = Gp_global->active_flag;
	cd.focused_flag = Gp_global->focused_flag;
	cd.focused_flag_on_edit_box = Gp_global->focused_flag_on_edit_box;

	cd.total_bgm_fade = Gp_global->bgmfade_total_volume;
	cd.total_bgm_fade2 = Gp_global->bgmfade2_total_volume;
	cd.total_bgm_volume = Gp_global->bgm_buf_total_volume;
	cd.total_koe_volume = Gp_global->koe_buf_total_volume;
	cd.total_pcm_volume = Gp_global->pcm_buf_total_volume;
	cd.total_se_volume = Gp_global->se_buf_total_volume;
	cd.total_mov_volume = Gp_global->mov_buf_total_volume;
	cd.mov_volume = Gp_sound->m_mov.get_volume();
	cd.mov_volume_real = Gp_sound->m_mov.get_volume_real();
	cd.play_silent_sound = Gp_global->play_silent_sound;

	cd.video_memory_use_size = Gp_global->video_memory_use_size;
	cd.video_memory_use_max_size = Gp_global->video_memory_use_max_size;
	cd.max_vertex_buffer_size = Gp_global->max_vertex_buffer_size;
	cd.moji_map_use_cnt = G_moji_manager.get_moji_map_use_cnt();
	cd.moji_map_now_cnt = G_moji_manager.get_moji_map_now_cnt();
	cd.moji_map_max_cnt = G_moji_manager.get_moji_map_max_cnt();
	cd.moji_memory_use_size = (int)(G_moji_manager.get_moji_memory_use_size() / 1024);
	cd.moji_memory_now_size = (int)(G_moji_manager.get_moji_memory_now_size() / 1024);
	cd.moji_memory_max_size = (int)(G_moji_manager.get_moji_memory_max_size() / 1024);
	cd.moji_kind_useful_cnt = G_moji_manager.get_moji_kind_useful_cnt();
	cd.moji_kind_unuseful_cnt = G_moji_manager.get_moji_kind_unuseful_cnt();
	cd.gan_data_map_now_cnt = G_gan_manager.get_map_now_cnt();
	cd.gan_data_map_max_cnt = G_gan_manager.get_map_max_cnt();

	cd.test1 = G_test1;
	cd.test2 = G_test2;
	cd.test3 = G_test3;
	cd.test4 = G_test4;
	cd.test5 = G_test5;
	cd.test6 = G_test6;
	cd.test7 = G_test7;
	cd.test8 = G_test8;
	cd.test9 = G_test9;

	// ���ڂ����ɐݒ肷��
	int item_no = -1;

	// �t���[�����[�g
	item_no ++;
	if (frame_first_flag || cd.frame_rate != m_ld.frame_rate)
		lv.set_cell_text(item_no, 1, tostr(cd.frame_rate));

	// �Z�[�u�|�C���g
	item_no ++;
	if (frame_first_flag || cd.save_point != m_ld.save_point)
		lv.set_cell_text(item_no, 1, cd.save_point);

	// �L���v�`���[
	item_no ++;
	if (frame_first_flag || cd.capture_exist_flag != m_ld.capture_exist_flag)
		lv.set_cell_text(item_no, 1, cd.capture_exist_flag ? _T("���݂���") : _T("--"));

	// �I�����|�C���g
	item_no ++;
	if (frame_first_flag || cd.sel_point_exist != m_ld.sel_point_exist)
		lv.set_cell_text(item_no, 1, cd.sel_point_exist ? _T("�L") : _T("��"));

	// �d�w�R�[����
	item_no ++;
	if (frame_first_flag || cd.ex_call_flag != m_ld.ex_call_flag)
		lv.set_cell_text(item_no, 1, cd.ex_call_flag ? _T("�V�X�e���R�[����") : _T("--"));

	// �V�X�e�����C�v
	item_no ++;
	if (frame_first_flag || cd.system_wipe_flag != m_ld.system_wipe_flag)
		lv.set_cell_text(item_no, 1, cd.system_wipe_flag ? _T("�V�X�e�����C�v��") : _T("--"));

	// �Q�[���^�C�}�[
	item_no ++;
	if (frame_first_flag || cd.game_timer_move_flag != m_ld.game_timer_move_flag)
		lv.set_cell_text(item_no, 1, cd.game_timer_move_flag ? _T("�����Ă���") : _T("�����Ă��Ȃ�"));

	// �X�^�b�N�T�C�Y
	item_no ++;
	if (frame_first_flag || cd.stack_size != m_ld.stack_size)
		lv.set_cell_text(item_no, 1, tostr(cd.stack_size));

	// �G�������g�X�^�b�N�T�C�Y
	item_no ++;
	if (frame_first_flag || cd.element_stack_size != m_ld.element_stack_size)
		lv.set_cell_text(item_no, 1, tostr(cd.element_stack_size));

	// ���b�Z�[�W�o�b�N�Z�[�u�T�C�Y
	item_no++;
	if (frame_first_flag || cd.msg_back_save_size != m_ld.msg_back_save_size)
		lv.set_cell_text(item_no, 1, tostr(cd.msg_back_save_size) + _T(" / ") + tostr(Gp_ini->backlog_save_cnt));

	// ���݂̎�����
	item_no ++;
	if (frame_first_flag || cd.real_time != m_ld.real_time)
		lv.set_cell_text(item_no, 1, tostr(cd.real_time));

	// ���[�J���̎�����
	item_no ++;
	if (frame_first_flag || cd.local_real_time != m_ld.local_real_time)
		lv.set_cell_text(item_no, 1, tostr(cd.local_real_time));

	// ���[�J���̃Q�[������
	item_no ++;
	if (frame_first_flag || cd.local_game_time != m_ld.local_game_time)
		lv.set_cell_text(item_no, 1, tostr(cd.local_game_time));

	// ���[�J���̃��C�v����
	item_no ++;
	if (frame_first_flag || cd.local_game_time != m_ld.local_wipe_time)
		lv.set_cell_text(item_no, 1, tostr(cd.local_wipe_time));

	// Ctrl �X�L�b�v
	item_no ++;
	if (frame_first_flag || cd.ctrl_skip_flag != m_ld.ctrl_skip_flag)
		lv.set_cell_text(item_no, 1, cd.ctrl_skip_flag ? _T("��") : _T("�~"));

	// �X�L�b�v���x
	item_no ++;
	if (frame_first_flag || cd.skip_speed != m_ld.skip_speed)
		lv.set_cell_text(item_no, 1, tostr(cd.skip_speed));

	// �X�L�b�v���~�߂�
	item_no ++;
	if (frame_first_flag || cd.cant_auto_skip_before_click != m_ld.cant_auto_skip_before_click)
		lv.set_cell_text(item_no, 1, cd.cant_auto_skip_before_click ? _T("��") : _T("�~"));

	// �E�B���h�E���A�N�e�B�u
	item_no ++;
	if (frame_first_flag || cd.active_flag != m_ld.active_flag)
		lv.set_cell_text(item_no, 1, cd.active_flag ? _T("��") : _T("�~"));

	// �E�B���h�E�Ƀt�H�[�J�X������
	item_no ++;
	if (frame_first_flag || cd.focused_flag != m_ld.focused_flag)
		lv.set_cell_text(item_no, 1, cd.focused_flag ? _T("��") : _T("�~"));

	// �G�f�B�b�g�{�b�N�X�Ƀt�H�[�J�X������
	item_no ++;
	if (frame_first_flag || cd.focused_flag_on_edit_box != m_ld.focused_flag_on_edit_box)
		lv.set_cell_text(item_no, 1, cd.focused_flag_on_edit_box ? _T("��") : _T("�~"));

	// �ŏI�a�f�l�t�F�[�h�l
	item_no ++;
	if (frame_first_flag || cd.total_bgm_fade != m_ld.total_bgm_fade)
		lv.set_cell_text(item_no, 1, tostr(cd.total_bgm_fade));

	// �ŏI�a�f�l�t�F�[�h�l�Q
	item_no ++;
	if (frame_first_flag || cd.total_bgm_fade2 != m_ld.total_bgm_fade2)
		lv.set_cell_text(item_no, 1, tostr(cd.total_bgm_fade2));

	// �ŏI�a�f�l�{�����[��
	item_no ++;
	if (frame_first_flag || cd.total_bgm_volume != m_ld.total_bgm_volume)
		lv.set_cell_text(item_no, 1, tostr(cd.total_bgm_volume));

	// �ŏI���{�����[��
	item_no ++;
	if (frame_first_flag || cd.total_koe_volume != m_ld.total_koe_volume)
		lv.set_cell_text(item_no, 1, tostr(cd.total_koe_volume));

	// �ŏI���ʉ��{�����[��
	item_no ++;
	if (frame_first_flag || cd.total_pcm_volume != m_ld.total_pcm_volume)
		lv.set_cell_text(item_no, 1, tostr(cd.total_pcm_volume));

	// �ŏI�V�X�e�����{�����[��
	item_no ++;
	if (frame_first_flag || cd.total_se_volume != m_ld.total_se_volume)
		lv.set_cell_text(item_no, 1, tostr(cd.total_se_volume));

	// �ŏI���[�r�[�{�����[��
	item_no ++;
	if (frame_first_flag || cd.total_mov_volume != m_ld.total_mov_volume)
		lv.set_cell_text(item_no, 1, tostr(cd.total_mov_volume));

	// ���[�r�[�{�����[��
	item_no ++;
	if (frame_first_flag || cd.mov_volume != m_ld.mov_volume)
		lv.set_cell_text(item_no, 1, tostr(cd.mov_volume));
	item_no ++;
	if (frame_first_flag || cd.mov_volume_real != m_ld.mov_volume_real)
		lv.set_cell_text(item_no, 1, tostr(cd.mov_volume_real));

	// �o�b�N�O���E���h�Ŗ������Đ�����
	item_no ++;
	if (frame_first_flag || cd.play_silent_sound != m_ld.play_silent_sound)
		lv.set_cell_text(item_no, 1, cd.play_silent_sound ? _T("��") : _T("�~"));

	// �r�f�I�������g�p�T�C�Y
	item_no ++;
	if (frame_first_flag || cd.video_memory_use_size != m_ld.video_memory_use_size || cd.video_memory_use_max_size != m_ld.video_memory_use_max_size)
		lv.set_cell_text(item_no, 1, tostr(cd.video_memory_use_size) + _T("MB / ") + tostr(cd.video_memory_use_max_size) + _T("MB"));

	// ���_�o�b�t�@�T�C�Y
	item_no ++;
	if (frame_first_flag || cd.max_vertex_buffer_size != m_ld.max_vertex_buffer_size)
		lv.set_cell_text(item_no, 1, tostr(cd.max_vertex_buffer_size) + _T(" Byte"));

	// �����m�ې�
	item_no ++;
	if (frame_first_flag || cd.moji_map_use_cnt != m_ld.moji_map_use_cnt || cd.moji_map_now_cnt != m_ld.moji_map_now_cnt || cd.moji_map_max_cnt != m_ld.moji_map_max_cnt)
		lv.set_cell_text(item_no, 1, tostr(cd.moji_map_use_cnt) + _T(" / ") + tostr(cd.moji_map_now_cnt) + _T(" / ") + tostr(cd.moji_map_max_cnt));

	// ����������
	item_no ++;
	if (frame_first_flag || cd.moji_memory_use_size != m_ld.moji_memory_use_size || cd.moji_memory_now_size != m_ld.moji_memory_now_size || cd.moji_memory_max_size != m_ld.moji_memory_max_size)
		lv.set_cell_text(item_no, 1, tostr(cd.moji_memory_use_size) + _T("KB / ") + tostr(cd.moji_memory_now_size) + _T("KB / ") + tostr(cd.moji_memory_max_size) + _T("KB"));

	// ������
	item_no ++;
	if (frame_first_flag || cd.moji_kind_useful_cnt != m_ld.moji_kind_useful_cnt || cd.moji_kind_unuseful_cnt != m_ld.moji_kind_unuseful_cnt)
		lv.set_cell_text(item_no, 1, tostr(cd.moji_kind_useful_cnt) + _T(" / ") + tostr(cd.moji_kind_unuseful_cnt));

	// �f�`�m�f�[�^�m�ې�
	item_no ++;
	if (frame_first_flag || cd.gan_data_map_now_cnt != m_ld.gan_data_map_now_cnt || cd.gan_data_map_max_cnt != m_ld.gan_data_map_max_cnt)
		lv.set_cell_text(item_no, 1, tostr(cd.gan_data_map_now_cnt) + _T(" / ") + tostr(cd.gan_data_map_max_cnt));

	// �e�X�g
	item_no ++;
	if (frame_first_flag || cd.test1 != m_ld.test1)
		lv.set_cell_text(item_no, 1, tostr(cd.test1));
	item_no ++;
	if (frame_first_flag || cd.test2 != m_ld.test2)
		lv.set_cell_text(item_no, 1, tostr(cd.test2));
	item_no ++;
	if (frame_first_flag || cd.test3 != m_ld.test3)
		lv.set_cell_text(item_no, 1, tostr(cd.test3));
	item_no ++;
	if (frame_first_flag || cd.test4 != m_ld.test4)
		lv.set_cell_text(item_no, 1, tostr(cd.test4));
	item_no ++;
	if (frame_first_flag || cd.test5 != m_ld.test5)
		lv.set_cell_text(item_no, 1, tostr(cd.test5));
	item_no ++;
	if (frame_first_flag || cd.test6 != m_ld.test6)
		lv.set_cell_text(item_no, 1, tostr(cd.test6));
	item_no ++;
	if (frame_first_flag || cd.test7 != m_ld.test7)
		lv.set_cell_text(item_no, 1, tostr(cd.test7));
	item_no ++;
	if (frame_first_flag || cd.test8 != m_ld.test8)
		lv.set_cell_text(item_no, 1, tostr(cd.test8));
	item_no ++;
	if (frame_first_flag || cd.test9 != m_ld.test9)
		lv.set_cell_text(item_no, 1, tostr(cd.test9));

	// �f�[�^���L��
	m_ld = cd;

	// ����t���O�����낷
	frame_first_flag = false;
}
