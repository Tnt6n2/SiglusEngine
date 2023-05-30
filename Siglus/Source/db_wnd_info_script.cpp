#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_syscom.h"
#include	"data/tnm_config_data.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_config.h"
#include	"dialog/db_wnd_info_script.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�X�N���v�g
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_script")

// ������
void C_db_wnd_info_script::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// �L���v�V�������o�^
	regist_caption_name(_T("�Q�[�����"));

	// ���X�g�r���[����
	listview_add_column(_T("state"), _T("�X�e�[�g"), 100);
	listview_add_column(_T("value"), _T("�l"), 100);
}

// �������t�@���N�V����
void C_db_wnd_info_script::on_init_func()
{
	// ���ڂ�ǉ�
	lv.add_item(0, _T("�V�[���ԍ�"));
	lv.add_item(0, _T("�s�ԍ�"));
	lv.add_item(0, _T("���݂̃v���Z�X"));

	lv.add_item(0, _T("�ꎞ�I�ɃV�X�e���R�}���h���j���[�֎~"));
	lv.add_item(0, _T("�ꎞ�I�ɃV���[�g�J�b�g�֎~"));
	lv.add_item(0, _T("�ꎞ�I�Ƀ��b�Z�[�W�E�B���h�E���B���֎~"));

	lv.add_item(0, _T("�ꎞ�I�ɃX�L�b�v�֎~"));
	lv.add_item(0, _T("�ꎞ�I�� Ctrl �X�L�b�v�֎~"));
	lv.add_item(0, _T("�ꎞ�I�ɃN���b�N���Ă��X�L�b�v���~�߂Ȃ�����"));
	lv.add_item(0, _T("�ꎞ�I�ɃN���b�N���Ă����b�Z�[�W���ꊇ�\�����Ȃ�����"));
	lv.add_item(0, _T("�ꎞ�I�ɖ��Ǖ��͂�������ł���悤�ɂ���"));
	lv.add_item(0, _T("�ꎞ�I�ɃI�[�g���[�h�ɂ���"));
	lv.add_item(0, _T("�ꎞ�I�Ƀ��b�Z�[�W�X�s�[�h��ݒ�"));
	lv.add_item(0, _T("�ꎞ�I�Ƀ��b�Z�[�W���m�[�E�F�C�g�ɂ���"));
	lv.add_item(0, _T("�ꎞ�I�Ƀ��b�Z�[�W��񓯊��ɂ���"));
	lv.add_item(0, _T("�ꎞ�I�Ƀ}���`���b�Z�[�W���g��"));
	lv.add_item(0, _T("�ꎞ�I�ɃX�L�b�v�g���K�["));
	lv.add_item(0, _T("�ꎞ�I�ɕ��͂��i��ł������~�߂Ȃ�����"));
	lv.add_item(0, _T("�ꎞ�I�ɕ��͂��i�񂾂琺���~�߂�悤�ɂ���"));

	lv.add_item(0, _T("�ꎞ�I�Ƀ��b�Z�[�W�E�B���h�E�{�^���֎~"));
	lv.add_item(0, _T("�ꎞ�I�Ƀ��b�Z�[�W�E�B���h�E�{�^���ɐG��Ȃ�����"));
	lv.add_item(0, _T("�ꎞ�I�Ƀ��b�Z�[�W�E�B���h�E�̃A�j�����s���悤�ɂ���"));
	lv.add_item(0, _T("�ꎞ�I�Ƀ��b�Z�[�W�E�B���h�E�̃A�j�����s��Ȃ�����"));
	lv.add_item(0, _T("�ꎞ�I�Ƀ��b�Z�[�W�E�B���h�E������"));

	lv.add_item(0, _T("�ꎞ�I�Ƀ��b�Z�[�W�o�b�N�֎~"));
	lv.add_item(0, _T("�ꎞ�I�Ƀ��b�Z�[�W�o�b�N���X�V���Ȃ�"));
	lv.add_item(0, _T("�ꎞ�I�Ƀ��b�Z�[�W�o�b�N���\���ɂ���"));

	lv.add_item(0, _T("�ꎞ�I�Ƀ}�E�X�J�[�\�����\���ɂ���"));
	lv.add_item(0, _T("�ꎞ�I�ɃL�[�{�[�h�ŃJ�[�\���𓮂����Ȃ�����"));

	lv.add_item(0, _T("�ꎞ�I�ɃN�G�C�N���~�߂�"));
	lv.add_item(0, _T("�ꎞ�I�ɂb�f�e�[�u���ɓo�^���Ȃ�����"));
	lv.add_item(0, _T("�ꎞ�I�ɂa�f�l�t�F�[�h���������ςȂ��ɂ���"));
	lv.add_item(0, _T("�ꎞ�I�ɃZ�[�u�|�C���g��ݒu���Ȃ�����"));
	lv.add_item(0, _T("�ꎞ�I�Ƀf�B�X�v���C�̐���������҂��Ȃ�����"));

	lv.add_item(0, _T("(���Ѻ����) ���Ǒ�����"));
	lv.add_item(0, _T("(���Ѻ����) ����������"));
	lv.add_item(0, _T("(���Ѻ����) �I�[�g���[�h"));
	lv.add_item(0, _T("(���Ѻ����) ���b�Z�[�W�E�B���h�E���B��"));
	lv.add_item(0, _T("(���Ѻ����) ���b�Z�[�W�o�b�N"));

	lv.add_item(0, _T("(���Ѻ�����ƭ-) ���Ǒ�����֎~"));
	lv.add_item(0, _T("(���Ѻ�����ƭ-) ����������֎~"));
	lv.add_item(0, _T("(���Ѻ�����ƭ-) �I�[�g���[�h�֎~"));
	lv.add_item(0, _T("(���Ѻ�����ƭ-) ���b�Z�[�W�E�B���h�E�������֎~"));
	lv.add_item(0, _T("(���Ѻ�����ƭ-) ���b�Z�[�W�o�b�N�֎~"));
	lv.add_item(0, _T("(���Ѻ�����ƭ-) �Z�[�u�֎~"));
	lv.add_item(0, _T("(���Ѻ�����ƭ-) ���[�h�֎~"));
	lv.add_item(0, _T("(���Ѻ�����ƭ-) �O�̑I�����ɖ߂�֎~"));

	lv.add_item(0, _T("(���Ѻ�����ƭ-) ���Ǒ������\��"));
	lv.add_item(0, _T("(���Ѻ�����ƭ-) �����������\��"));
	lv.add_item(0, _T("(���Ѻ�����ƭ-) �I�[�g���[�h��\��"));
	lv.add_item(0, _T("(���Ѻ�����ƭ-) ���b�Z�[�W�E�B���h�E��������\��"));
	lv.add_item(0, _T("(���Ѻ�����ƭ-) ���b�Z�[�W�o�b�N��\��"));
	lv.add_item(0, _T("(���Ѻ�����ƭ-) �Z�[�u��\��"));
	lv.add_item(0, _T("(���Ѻ�����ƭ-) ���[�h��\��"));
	lv.add_item(0, _T("(���Ѻ�����ƭ-) �O�̑I�����ɖ߂��\��"));
}

// ����t�@���N�V����
void C_db_wnd_info_script::on_close_func()
{
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_script::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

	// ���݂̒l���擾����
	cd.cur_scn_no = Gp_lexer->get_cur_scn_no();
	cd.cur_line_no = Gp_lexer->get_cur_line_no();
	cd.proc_type = Gp_local->cur_proc.type;

	cd.syscom_menu_disable = Gp_local->pod.syscom_menu_disable;
	cd.hide_mwnd_disable = Gp_local->pod.hide_mwnd_disable;
	cd.shortcut_disable = Gp_local->pod.shortcut_disable;

	cd.skip_disable = Gp_local->pod.skip_disable;
	cd.ctrl_disable = Gp_local->pod.ctrl_disable;
	cd.not_stop_skip_by_click = Gp_local->pod.not_stop_skip_by_click;
	cd.not_skip_msg_by_click = Gp_local->pod.not_skip_msg_by_click;
	cd.skip_unread_message = Gp_local->pod.skip_unread_message;
	cd.auto_mode_flag = Gp_local->pod.auto_mode_flag;
	cd.msg_speed = Gp_local->pod.msg_speed;
	cd.msg_nowait = Gp_local->pod.msg_nowait;
	cd.async_msg_mode = Gp_local->pod.async_msg_mode;
	cd.multi_msg_mode = Gp_local->pod.multi_msg_mode;
	cd.skip_trigger = Gp_local->pod.skip_trigger;
	cd.koe_dont_stop_on_flag = Gp_local->pod.koe_dont_stop_on_flag;
	cd.koe_dont_stop_off_flag = Gp_local->pod.koe_dont_stop_off_flag;

	cd.mwnd_btn_disable_all = Gp_local->pod.mwnd_btn_disable_all;
	cd.mwnd_btn_touch_disable = Gp_local->pod.mwnd_btn_touch_disable;
	cd.mwnd_anime_on_flag = Gp_local->pod.mwnd_anime_on_flag;
	cd.mwnd_anime_off_flag = Gp_local->pod.mwnd_anime_off_flag;
	cd.mwnd_disp_off_flag = Gp_local->pod.mwnd_disp_off_flag;

	cd.msg_back_disable = Gp_local->pod.msg_back_disable;
	cd.msg_back_off = Gp_local->pod.msg_back_off;
	cd.msg_back_disp_off = Gp_local->pod.msg_back_disp_off;

	cd.cursor_disp_off = Gp_local->pod.cursor_disp_off;
	cd.cursor_move_by_key_disable = Gp_local->pod.cursor_move_by_key_disable;

	cd.quake_stop_flag = Gp_local->pod.quake_stop_flag;
	cd.cg_table_off_flag = Gp_local->pod.cg_table_off_flag;
	cd.bgmfade_flag = Gp_local->pod.bgmfade_flag;
	cd.dont_set_save_point = Gp_local->pod.dont_set_save_point;
	cd.wait_display_vsync_off_flag = Gp_local->pod.wait_display_vsync_off_flag;

	cd.syscom_read_skip = Gp_syscom->read_skip_flag;
	cd.syscom_auto_skip = Gp_syscom->auto_skip_flag;
	cd.syscom_auto_mode = Gp_config->auto_mode_onoff;
	cd.syscom_hide_mwnd = Gp_syscom->hide_mwnd_flag;
	cd.syscom_msg_back = Gp_syscom->msg_back_flag;

	cd.syscom_read_skip_disable = !Gp_syscom->syscommenu.read_skip.enable;
	cd.syscom_auto_skip_disable = !Gp_syscom->syscommenu.auto_skip.enable;
	cd.syscom_auto_mode_disable = !Gp_syscom->syscommenu.auto_mode.enable;
	cd.syscom_save_disable = !Gp_syscom->syscommenu.save.enable;
	cd.syscom_load_disable = !Gp_syscom->syscommenu.load.enable;
	cd.syscom_return_to_sel_disable = !Gp_syscom->syscommenu.return_to_sel.enable;
	cd.syscom_hide_mwnd_disable = !Gp_syscom->syscommenu.hide_mwnd.enable;
	cd.syscom_msg_back_disable = !Gp_syscom->syscommenu.msg_back.enable;

	cd.syscom_read_skip_off = !Gp_syscom->syscommenu.read_skip.exist;
	cd.syscom_auto_skip_off = !Gp_syscom->syscommenu.auto_skip.exist;
	cd.syscom_auto_mode_off = !Gp_syscom->syscommenu.auto_mode.exist;
	cd.syscom_save_off = !Gp_syscom->syscommenu.save.exist;
	cd.syscom_load_off = !Gp_syscom->syscommenu.load.exist;
	cd.syscom_return_to_sel_off = !Gp_syscom->syscommenu.return_to_sel.exist;
	cd.syscom_hide_mwnd_off = !Gp_syscom->syscommenu.hide_mwnd.exist;
	cd.syscom_msg_back_off = !Gp_syscom->syscommenu.msg_back.exist;

	// ���ڂ����ɐݒ肷��
	int item_no = -1;

	// �V�[���ԍ�
	item_no ++;
	if (frame_first_flag || cd.cur_scn_no != m_ld.cur_scn_no)
		lv.set_cell_text(item_no, 1, tostr(cd.cur_scn_no));

	// �s�ԍ�
	item_no ++;
	if (frame_first_flag || cd.cur_line_no != m_ld.cur_line_no)
		lv.set_cell_text(item_no, 1, tostr(cd.cur_line_no));

	// ���݂̃v���Z�X
	item_no ++;
	if (frame_first_flag || cd.proc_type != m_ld.proc_type)	{
		if (0 <= cd.proc_type && cd.proc_type < TNM_PROC_TYPE_MAX)	{
			lv.set_cell_text(item_no, 1, TNM_PROC_STR[cd.proc_type]);
		}
		else	{
			lv.set_cell_text(item_no, 1, _T("???"));
		}
	}

	// �V�X�e���R�}���h���j���[���֎~
	item_no ++;
	if (frame_first_flag || cd.syscom_menu_disable != m_ld.syscom_menu_disable)
		lv.set_cell_text(item_no, 1, cd.syscom_menu_disable ? _T("�I��") : _T("--"));

	// �V���[�g�J�b�g���֎~
	item_no ++;
	if (frame_first_flag || cd.shortcut_disable != m_ld.shortcut_disable)
		lv.set_cell_text(item_no, 1, cd.shortcut_disable ? _T("�I��") : _T("--"));

	// ���b�Z�[�W�E�B���h�E���B�����֎~
	item_no ++;
	if (frame_first_flag || cd.hide_mwnd_disable != m_ld.hide_mwnd_disable)
		lv.set_cell_text(item_no, 1, cd.hide_mwnd_disable ? _T("�I��") : _T("--"));


	// �X�L�b�v���֎~
	item_no ++;
	if (frame_first_flag || cd.skip_disable != m_ld.skip_disable)
		lv.set_cell_text(item_no, 1, cd.skip_disable ? _T("�I��") : _T("--"));

	// �X�L�b�v�֎~�iCtrl �܂ށj
	item_no ++;
	if (frame_first_flag || cd.ctrl_disable != m_ld.ctrl_disable)
		lv.set_cell_text(item_no, 1, cd.ctrl_disable ? _T("�I��") : _T("--"));

	// �N���b�N���Ă��X�L�b�v���~�߂Ȃ�
	item_no ++;
	if (frame_first_flag || cd.not_stop_skip_by_click != m_ld.not_stop_skip_by_click)
		lv.set_cell_text(item_no, 1, cd.not_stop_skip_by_click ? _T("�I��") : _T("--"));

	// �N���b�N���Ă����b�Z�[�W���ꊇ�\�����Ȃ�
	item_no ++;
	if (frame_first_flag || cd.not_skip_msg_by_click != m_ld.not_skip_msg_by_click)
		lv.set_cell_text(item_no, 1, cd.not_skip_msg_by_click ? _T("�I��") : _T("--"));

	// �ꎞ�I�ɖ��Ǖ��͂�������ł���悤�ɂ���
	item_no ++;
	if (frame_first_flag || cd.skip_unread_message != m_ld.skip_unread_message)
		lv.set_cell_text(item_no, 1, cd.skip_unread_message ? _T("�I��") : _T("--"));

	// �I�[�g���[�h�t���O
	item_no ++;
	if (frame_first_flag || cd.auto_mode_flag != m_ld.auto_mode_flag)
		lv.set_cell_text(item_no, 1, cd.auto_mode_flag ? _T("�I��") : _T("--"));

	// ���b�Z�[�W�X�s�[�h
	item_no ++;
	if (frame_first_flag || cd.msg_speed != m_ld.msg_speed)
		lv.set_cell_text(item_no, 1, cd.msg_speed >= 0 ? tostr(cd.msg_speed) : _T("--"));

	// �m�[�E�F�C�g
	item_no ++;
	if (frame_first_flag || cd.msg_nowait != m_ld.msg_nowait)
		lv.set_cell_text(item_no, 1, cd.msg_nowait ? _T("�I��") : _T("--"));

	// �񓯊����b�Z�[�W
	item_no ++;
	if (frame_first_flag || cd.async_msg_mode != m_ld.async_msg_mode)
		lv.set_cell_text(item_no, 1, cd.async_msg_mode ? _T("�I��") : _T("--"));

	// �}���`���b�Z�[�W
	item_no ++;
	if (frame_first_flag || cd.multi_msg_mode != m_ld.multi_msg_mode)
		lv.set_cell_text(item_no, 1, cd.multi_msg_mode ? _T("�I��") : _T("--"));

	// �X�L�b�v�g���K�[
	item_no ++;
	if (frame_first_flag || cd.skip_trigger != m_ld.skip_trigger)
		lv.set_cell_text(item_no, 1, cd.skip_trigger ? _T("�I��") : _T("--"));

	// �����~�߂Ȃ�
	item_no ++;
	if (frame_first_flag || cd.koe_dont_stop_on_flag != m_ld.koe_dont_stop_on_flag)
		lv.set_cell_text(item_no, 1, cd.koe_dont_stop_on_flag ? _T("�I��") : _T("--"));

	// �����~�߂�
	item_no ++;
	if (frame_first_flag || cd.koe_dont_stop_off_flag != m_ld.koe_dont_stop_off_flag)
		lv.set_cell_text(item_no, 1, cd.koe_dont_stop_off_flag ? _T("�I��") : _T("--"));



	// ���b�Z�[�W�E�B���h�E�{�^�����֎~
	item_no ++;
	if (frame_first_flag || cd.mwnd_btn_disable_all != m_ld.mwnd_btn_disable_all)
		lv.set_cell_text(item_no, 1, cd.mwnd_btn_disable_all ? _T("�I��") : _T("--"));

	// ���b�Z�[�W�E�B���h�E�{�^���ɐG��Ȃ�
	item_no ++;
	if (frame_first_flag || cd.mwnd_btn_touch_disable != m_ld.mwnd_btn_touch_disable)
		lv.set_cell_text(item_no, 1, cd.mwnd_btn_touch_disable ? _T("�I��") : _T("--"));

	// ���b�Z�[�W�E�B���h�E�̃A�j�����s��
	item_no ++;
	if (frame_first_flag || cd.mwnd_anime_on_flag != m_ld.mwnd_anime_on_flag)
		lv.set_cell_text(item_no, 1, cd.mwnd_anime_on_flag ? _T("�I��") : _T("--"));

	// ���b�Z�[�W�E�B���h�E�̃A�j�����s��Ȃ�
	item_no ++;
	if (frame_first_flag || cd.mwnd_anime_off_flag != m_ld.mwnd_anime_off_flag)
		lv.set_cell_text(item_no, 1, cd.mwnd_anime_off_flag ? _T("�I��") : _T("--"));

	// ���b�Z�[�W�E�B���h�E������
	item_no ++;
	if (frame_first_flag || cd.mwnd_disp_off_flag != m_ld.mwnd_disp_off_flag)
		lv.set_cell_text(item_no, 1, cd.mwnd_disp_off_flag ? _T("�I��") : _T("--"));

	
	
	// ���b�Z�[�W�o�b�N���֎~
	item_no ++;
	if (frame_first_flag || cd.msg_back_disable != m_ld.msg_back_disable)
		lv.set_cell_text(item_no, 1, cd.msg_back_disable ? _T("�I��") : _T("--"));

	// ���b�Z�[�W�o�b�N���g��Ȃ�
	item_no ++;
	if (frame_first_flag || cd.msg_back_off != m_ld.msg_back_off)
		lv.set_cell_text(item_no, 1, cd.msg_back_off ? _T("�I��") : _T("--"));

	// ���b�Z�[�W�o�b�N���\���ɂ���
	item_no ++;
	if (frame_first_flag || cd.msg_back_disp_off != m_ld.msg_back_disp_off)
		lv.set_cell_text(item_no, 1, cd.msg_back_disp_off ? _T("�I��") : _T("--"));


	// �J�[�\�����\��
	item_no ++;
	if (frame_first_flag || cd.cursor_disp_off != m_ld.cursor_disp_off)
		lv.set_cell_text(item_no, 1, cd.cursor_disp_off ? _T("�I��") : _T("--"));

	// �L�[�{�[�h�ŃJ�[�\���𓮂����Ȃ�
	item_no ++;
	if (frame_first_flag || cd.cursor_move_by_key_disable != m_ld.cursor_move_by_key_disable)
		lv.set_cell_text(item_no, 1, cd.cursor_move_by_key_disable ? _T("�I��") : _T("--"));



	// �N�G�C�N���~�߂�
	item_no ++;
	if (frame_first_flag || cd.quake_stop_flag != m_ld.quake_stop_flag)
		lv.set_cell_text(item_no, 1, cd.quake_stop_flag ? _T("�I��") : _T("--"));

	// �b�f�e�[�u���ɓo�^���Ȃ�
	item_no ++;
	if (frame_first_flag || cd.cg_table_off_flag != m_ld.cg_table_off_flag)
		lv.set_cell_text(item_no, 1, cd.cg_table_off_flag ? _T("�I��") : _T("--"));

	// �a�f�l�t�F�[�h���������ςȂ�
	item_no ++;
	if (frame_first_flag || cd.bgmfade_flag != m_ld.bgmfade_flag)
		lv.set_cell_text(item_no, 1, cd.bgmfade_flag ? _T("�I��") : _T("--"));

	// �Z�[�u�|�C���g��ݒu���Ȃ�
	item_no ++;
	if (frame_first_flag || cd.dont_set_save_point != m_ld.dont_set_save_point)
		lv.set_cell_text(item_no, 1, cd.dont_set_save_point ? _T("�I��") : _T("--"));

	// �f�B�X�v���C�̐���������҂��Ȃ�
	item_no ++;
	if (frame_first_flag || cd.wait_display_vsync_off_flag != m_ld.wait_display_vsync_off_flag)
		lv.set_cell_text(item_no, 1, cd.wait_display_vsync_off_flag ? _T("�I��") : _T("--"));



	// ���Ѻ���ށF���Ǒ�����
	item_no ++;
	if (frame_first_flag || cd.syscom_read_skip != m_ld.syscom_read_skip)
		lv.set_cell_text(item_no, 1, cd.syscom_read_skip ? _T("�I��") : _T("--"));

	// ���Ѻ���ށF����������
	item_no ++;
	if (frame_first_flag || cd.syscom_auto_skip != m_ld.syscom_auto_skip)
		lv.set_cell_text(item_no, 1, cd.syscom_auto_skip ? _T("�I��") : _T("--"));

	// ���Ѻ���ށF�I�[�g���[�h
	item_no ++;
	if (frame_first_flag || cd.syscom_auto_mode != m_ld.syscom_auto_mode)
		lv.set_cell_text(item_no, 1, cd.syscom_auto_mode ? _T("�I��") : _T("--"));

	// ���Ѻ���ށF���b�Z�[�W�E�B���h�E���B��
	item_no ++;
	if (frame_first_flag || cd.syscom_hide_mwnd != m_ld.syscom_hide_mwnd)
		lv.set_cell_text(item_no, 1, cd.syscom_hide_mwnd ? _T("�I��") : _T("--"));

	// ���Ѻ���ށF���b�Z�[�W�o�b�N
	item_no ++;
	if (frame_first_flag || cd.syscom_msg_back != m_ld.syscom_msg_back)
		lv.set_cell_text(item_no, 1, cd.syscom_msg_back ? _T("�I��") : _T("--"));

	// ���Ѻ�����ƭ��F���Ǒ�����F�֎~
	item_no ++;
	if (frame_first_flag || cd.syscom_read_skip_disable != m_ld.syscom_read_skip_disable)
		lv.set_cell_text(item_no, 1, cd.syscom_read_skip_disable ? _T("�I��") : _T("--"));

	// ���Ѻ�����ƭ��F����������F�֎~
	item_no ++;
	if (frame_first_flag || cd.syscom_auto_skip_disable != m_ld.syscom_auto_skip_disable)
		lv.set_cell_text(item_no, 1, cd.syscom_auto_skip_disable ? _T("�I��") : _T("--"));

	// ���Ѻ�����ƭ��F�I�[�g���[�h�F�֎~
	item_no ++;
	if (frame_first_flag || cd.syscom_auto_mode_disable != m_ld.syscom_auto_mode_disable)
		lv.set_cell_text(item_no, 1, cd.syscom_auto_mode_disable ? _T("�I��") : _T("--"));

	// ���Ѻ�����ƭ��F���b�Z�[�W�E�B���h�E���B���F�֎~
	item_no ++;
	if (frame_first_flag || cd.syscom_hide_mwnd_disable != m_ld.syscom_hide_mwnd_disable)
		lv.set_cell_text(item_no, 1, cd.syscom_hide_mwnd_disable ? _T("�I��") : _T("--"));

	// ���Ѻ�����ƭ��F���b�Z�[�W�o�b�N�F�֎~
	item_no ++;
	if (frame_first_flag || cd.syscom_msg_back_disable != m_ld.syscom_msg_back_disable)
		lv.set_cell_text(item_no, 1, cd.syscom_msg_back_disable ? _T("�I��") : _T("--"));

	// ���Ѻ�����ƭ��F�Z�[�u�F�֎~
	item_no ++;
	if (frame_first_flag || cd.syscom_save_disable != m_ld.syscom_save_disable)
		lv.set_cell_text(item_no, 1, cd.syscom_save_disable ? _T("�I��") : _T("--"));

	// ���Ѻ�����ƭ��F���[�h�F�֎~
	item_no ++;
	if (frame_first_flag || cd.syscom_load_disable != m_ld.syscom_load_disable)
		lv.set_cell_text(item_no, 1, cd.syscom_load_disable ? _T("�I��") : _T("--"));

	// ���Ѻ�����ƭ��F�O�̑I�����ɖ߂�F�֎~
	item_no ++;
	if (frame_first_flag || cd.syscom_return_to_sel_disable != m_ld.syscom_return_to_sel_disable)
		lv.set_cell_text(item_no, 1, cd.syscom_return_to_sel_disable ? _T("�I��") : _T("--"));

	// ���Ѻ�����ƭ��F���Ǒ�����F��\��
	item_no ++;
	if (frame_first_flag || cd.syscom_read_skip_off != m_ld.syscom_read_skip_off)
		lv.set_cell_text(item_no, 1, cd.syscom_read_skip_off ? _T("�I��") : _T("--"));

	// ���Ѻ�����ƭ��F����������F��\��
	item_no ++;
	if (frame_first_flag || cd.syscom_auto_skip_off != m_ld.syscom_auto_skip_off)
		lv.set_cell_text(item_no, 1, cd.syscom_auto_skip_off ? _T("�I��") : _T("--"));

	// ���Ѻ�����ƭ��F�I�[�g���[�h�F��\��
	item_no ++;
	if (frame_first_flag || cd.syscom_auto_mode_off != m_ld.syscom_auto_mode_off)
		lv.set_cell_text(item_no, 1, cd.syscom_auto_mode_off ? _T("�I��") : _T("--"));

	// ���Ѻ�����ƭ��F���b�Z�[�W�E�B���h�E���B���F��\��
	item_no ++;
	if (frame_first_flag || cd.syscom_hide_mwnd_off != m_ld.syscom_hide_mwnd_off)
		lv.set_cell_text(item_no, 1, cd.syscom_hide_mwnd_off ? _T("�I��") : _T("--"));

	// ���Ѻ�����ƭ��F���b�Z�[�W�o�b�N�F��\��
	item_no ++;
	if (frame_first_flag || cd.syscom_msg_back_off != m_ld.syscom_msg_back_off)
		lv.set_cell_text(item_no, 1, cd.syscom_msg_back_off ? _T("�I��") : _T("--"));

	// ���Ѻ�����ƭ��F�Z�[�u�F��\��
	item_no ++;
	if (frame_first_flag || cd.syscom_save_off != m_ld.syscom_save_off)
		lv.set_cell_text(item_no, 1, cd.syscom_save_off ? _T("�I��") : _T("--"));

	// ���Ѻ�����ƭ��F���[�h�F��\��
	item_no ++;
	if (frame_first_flag || cd.syscom_load_off != m_ld.syscom_load_off)
		lv.set_cell_text(item_no, 1, cd.syscom_load_off ? _T("�I��") : _T("--"));

	// ���Ѻ�����ƭ��F�O�̑I�����ɖ߂�F��\��
	item_no ++;
	if (frame_first_flag || cd.syscom_return_to_sel_off != m_ld.syscom_return_to_sel_off)
		lv.set_cell_text(item_no, 1, cd.syscom_return_to_sel_off ? _T("�I��") : _T("--"));

	// �f�[�^���L��
	m_ld = cd;

	// ����t���O�����낷
	frame_first_flag = false;
}
