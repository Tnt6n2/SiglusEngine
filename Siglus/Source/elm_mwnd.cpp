#include	"pch.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_tool.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"data/tnm_read_flag.h"
#include	"element/elm_mwnd.h"
#include	"element/elm_mwnd_moji.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_config.h"
#include	"engine/eng_graphics.h"
#include	"engine/eng_etc.h"


// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F������
// ================================================================
void C_elm_mwnd::init(int mwnd_no, S_element element, CTSTR& name, C_elm_mwnd_list* p_parent, S_element elm_group_list)
{
	// ���N���X�̏�����
	C_tnm_element::init(element, FM_MWND, name, p_parent);

	// �Œ�p�����[�^
	m_mwnd_no = mwnd_no;
	m_def_elm_group_list = elm_group_list;

	// ���b�Z�[�W�g
	m_msg_waku.init(element, name);

	// ���O�g
	m_name_waku.init(element, name);

	// �ď�����
	reinit(true);
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F�ď�����
// ================================================================
void C_elm_mwnd::reinit(bool restruct_flag)
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	// �I���͕K�����Ă݂悤
	finish();

	// �e���v���[�g���č\�z
	restruct_template();

	// �����ϐ�������
	init_work_variable();

	// ���b�Z�[�W
	m_msg_list.resize(1);	// ���b�Z�[�W�͂P��
	for (int i = 0; i < (int)m_msg_list.size(); i++)
		m_msg_list[i].init(m_def_elm_group_list, this);

	// ���b�Z�[�W�Ƀf�t�H���g�̃p�����[�^��^����
	for (int i = 0; i < (int)m_msg_list.size(); i++)
		m_msg_list[i].set_def_param(p_ini_sub->moji_cnt
		, p_ini_sub->moji_size, p_ini_sub->moji_space
		, p_ini_sub->moji_color >= 0 ? p_ini_sub->moji_color : p_ini->moji_color
		, p_ini_sub->shadow_color >= 0 ? p_ini_sub->shadow_color : p_ini->shadow_color
		, p_ini_sub->fuchi_color >= 0 ? p_ini_sub->fuchi_color : p_ini->fuchi_color
		, p_ini_sub->ruby_size, p_ini_sub->ruby_space
		, p_ini_sub->name_moji_size, p_ini_sub->name_newline, p_ini_sub->name_bracket, p_ini_sub->talk_margin);

	// ���O
	m_name.init();

	// �I����
	m_sel.init();

	// ���b�Z�[�W�g
	m_msg_waku.reinit(false);	// �č\�z���Ȃ�

	// ���O�g
	m_name_waku.reinit(false);	// �č\�z���Ȃ�

	// �č\�z
	if (restruct_flag)	{

		// ���O�Ƀf�t�H���g�̃p�����[�^��^����
		m_name.set_template(m_mwnd_no);

		// �I�����Ƀf�t�H���g�̃p�����[�^��^����
		m_sel.set_template(m_mwnd_no);

		// �g�𐮂���
		set_waku(p_ini_sub->waku_no, p_ini_sub->name_waku_no, false);
	}
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F�I��
// ================================================================
void C_elm_mwnd::finish()
{
	// ���ǃt���O�𗧂Ă�I
	set_read_flag();

	// ���b�Z�[�W�g
	m_msg_waku.finish();

	// ���O�g
	m_name_waku.finish();
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F�����ϐ���������
// ================================================================
void C_elm_mwnd::init_work_variable()
{
	m_time = 0;
	m_msg_block_started_flag = false;
	m_auto_proc_ready_flag = false;
	m_window_appear = false;
	m_name_appear = false;
	m_clear_ready_flag = false;
	m_auto_mode_end_moji_cnt = 0;
	m_target_msg_no = 0;
	m_slide_msg = false;
	m_slide_time = 0;
	m_read_flag_stock_list.clear();
	m_koe_no = -1;
	m_koe_play_flag = false;
	m_koe_no_auto_mode_flag = false;
	m_open_anime_type = -1;
	m_open_anime_time = 0;
	m_open_anime_start_time = 0;
	m_close_anime_type = -1;
	m_close_anime_time = 0;
	m_close_anime_start_time = 0;
	m_slide_anime_type = -1;
	m_slide_anime_time = 0;
	m_slide_anime_start_time = 0;
	m_slide_pixel = 0;
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F�e���v���[�g���č\�z����
// ================================================================
void C_elm_mwnd::restruct_template()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	// Gameexe.ini ����f�t�H���g�̃p�����[�^���擾����
	m_param.novel_mode = p_ini_sub->novel_mode;
	m_param.sorter = S_tnm_sorter(p_ini->order, 0);
	m_param.world = -1;
	m_param.extend_type = p_ini_sub->extend_type;
	m_param.window_pos = p_ini_sub->window_pos;
	m_param.window_size = p_ini_sub->window_size;
	m_param.msg_pos = p_ini_sub->msg_pos;
	m_param.msg_margin = p_ini_sub->msg_margin;
	m_param.msg_moji_cnt = p_ini_sub->moji_cnt;
	m_param.name_disp_mode = p_ini_sub->name_disp_mode;
	m_param.name_bracket = p_ini_sub->name_bracket;
	m_param.name_extend_type = p_ini_sub->name_extend_type;
	m_param.name_window_align = p_ini_sub->name_window_align;
	m_param.name_window_pos = p_ini_sub->name_window_pos;
	m_param.name_window_size = p_ini_sub->name_window_size;
	m_param.name_msg_pos = p_ini_sub->name_msg_pos;
	m_param.name_msg_pos_rep = p_ini_sub->name_msg_pos_rep;
	m_param.name_msg_margin = p_ini_sub->name_msg_margin;
	m_param.over_flow_check_size = p_ini_sub->over_flow_check_size;
	m_param.face_hide_name = p_ini_sub->face_hide_name;
	m_param.open_anime_type = p_ini_sub->open_anime_type;
	m_param.open_anime_time = p_ini_sub->open_anime_time;
	m_param.close_anime_type = p_ini_sub->close_anime_type;
	m_param.close_anime_time = p_ini_sub->close_anime_time;
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F�Z�[�u
// ================================================================
void C_elm_mwnd::save(C_tnm_save_stream& stream)
{
	// ���݂̃p�����[�^
	stream.save(m_param);

	// �����ϐ�
	stream.save(m_time);
	stream.save(m_msg_block_started_flag);
	stream.save(m_auto_proc_ready_flag);
	stream.save(m_window_appear);
	stream.save(m_name_appear);
	stream.save(m_clear_ready_flag);
	stream.save(m_auto_mode_end_moji_cnt);
	stream.save(m_target_msg_no);
	stream.save(m_slide_msg);
	stream.save(m_slide_time);
	stream.save(m_koe_no);
	stream.save(m_koe_play_flag);
	stream.save(m_open_anime_type);
	stream.save(m_open_anime_time);
	stream.save(m_open_anime_start_time);
	stream.save(m_close_anime_type);
	stream.save(m_close_anime_time);
	stream.save(m_close_anime_start_time);

	// ���b�Z�[�W
	stream.save(m_msg_list);

	// ���b�Z�[�W�g
	m_msg_waku.save(stream);

	// ���O
	m_name.save(stream);

	// ���O�g
	m_name_waku.save(stream);

	// �I����
	m_sel.save(stream);
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F���[�h
// ================================================================
void C_elm_mwnd::load(C_tnm_save_stream& stream)
{
	// ���݂̃p�����[�^
	stream.load(m_param);

	// �����ϐ�
	stream.load(m_time);
	stream.load(m_msg_block_started_flag);
	stream.load(m_auto_proc_ready_flag);
	stream.load(m_window_appear);
	stream.load(m_name_appear);
	stream.load(m_clear_ready_flag);
	stream.load(m_auto_mode_end_moji_cnt);
	stream.load(m_target_msg_no);
	stream.load(m_slide_msg);
	stream.load(m_slide_time);
	stream.load(m_koe_no);
	stream.load(m_koe_play_flag);
	stream.load(m_open_anime_type);
	stream.load(m_open_anime_time);
	stream.load(m_open_anime_start_time);
	stream.load(m_close_anime_type);
	stream.load(m_close_anime_time);
	stream.load(m_close_anime_start_time);

	// ���b�Z�[�W
	stream.load(m_msg_list);

	// ���b�Z�[�W�g
	m_msg_waku.load(stream);

	// ���O
	m_name.load(stream);

	// ���O�g
	m_name_waku.load(stream);

	// �I����
	m_sel.load(stream);

	// ���傫���̍Đݒ�

	// ���b�Z�[�W�g�̍č\�z
	restruct_msg_waku();

	// ���O�g�̍č\�z
	restruct_name_waku();

	// �I�����̍č\�z
	restruct_sel();
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F�R�s�[
// ================================================================
void C_elm_mwnd::copy(C_elm_mwnd* src)
{
	// ���݂̃p�����[�^
	m_param = src->m_param;

	// �����ϐ�
	m_time = src->m_time;
	m_msg_block_started_flag = src->m_msg_block_started_flag;
	m_auto_proc_ready_flag = src->m_auto_proc_ready_flag;
	m_window_appear = src->m_window_appear;
	m_name_appear = src->m_name_appear;
	m_clear_ready_flag = src->m_clear_ready_flag;
	m_auto_mode_end_moji_cnt = src->m_auto_mode_end_moji_cnt;
	m_slide_msg = src->m_slide_msg;
	m_slide_time = src->m_slide_time;
	m_koe_no = src->m_koe_no;
	m_koe_play_flag = src->m_koe_play_flag;
	m_koe_no_auto_mode_flag = src->m_koe_no_auto_mode_flag;
	m_open_anime_type = src->m_open_anime_type;
	m_open_anime_time = src->m_open_anime_time;
	m_open_anime_start_time = src->m_open_anime_start_time;
	m_close_anime_type = src->m_close_anime_type;
	m_close_anime_time = src->m_close_anime_time;
	m_close_anime_start_time = src->m_close_anime_start_time;

	// ���b�Z�[�W
	m_msg_list.resize(src->m_msg_list.size());
	for (int i = 0; i < (int)m_msg_list.size(); i++)
		m_msg_list[i].copy(&src->m_msg_list[i]);

	// ���b�Z�[�W�g
	m_msg_waku.copy_all(&src->m_msg_waku, true);

	// ���O
	m_name.copy(&src->m_name);

	// ���O�g
	m_name_waku.copy_all(&src->m_name_waku, true);

	// �I����
	m_sel.copy(&src->m_sel);

	// ���傫���̍Đݒ�

	// ���b�Z�[�W�g�̍č\�z
	restruct_msg_waku();

	// ���O�g�̍č\�z
	restruct_name_waku();

	// �I�����̍č\�z
	restruct_sel();
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F�g��ݒ�
// ================================================================
void C_elm_mwnd::set_waku(int msg_waku_no, int name_waku_no, bool reinit_flag)
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	// �f�t�H���g�l��ݒ�
	if (msg_waku_no == TNM_USE_DEFAULT)
		msg_waku_no = p_ini_sub->waku_no;
	if (name_waku_no == TNM_USE_DEFAULT)
		name_waku_no = p_ini_sub->name_waku_no;

	// ���b�Z�[�W�g
	m_msg_waku.set_template(msg_waku_no, reinit_flag);

	// ���b�Z�[�W�g�̍č\�z
	restruct_msg_waku();

	// ���O�g
	m_name_waku.set_template(name_waku_no, reinit_flag);

	// ���O�g�̍č\�z
	restruct_name_waku();
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F�t���[������������
// ================================================================
void C_elm_mwnd::frame_init()
{
	// ���b�Z�[�W
	for (int i = 0; i < (int)m_msg_list.size(); i++)
		m_msg_list[i].frame_init();

	// ���b�Z�[�W�g
	m_msg_waku.frame_init();

	// ���O
	m_name.frame_init();

	// ���O�g
	m_name_waku.frame_init();

	// �I����
	m_sel.frame_init();
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F���Ԃ�i�߂�
// ================================================================
void C_elm_mwnd::update_time(int past_game_time, int past_real_time)
{
	// ������i�߂�
	m_time += past_game_time;

	// ���b�Z�[�W�g
	m_msg_waku.update_time(past_game_time, past_real_time);

	// ���O�g
	m_name_waku.update_time(past_game_time, past_real_time);
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F�t���[������
// ================================================================
bool C_elm_mwnd::frame(bool draw_futoku, bool draw_shadow)
{
	// ���b�Z�[�W��i�߂�
	msg_frame();

	// �E�B���h�E�̃A�j����Ԃ�������
	bool anime_open = true;
	int anime_type = 0;
	int anime_prog = 255;	// �A�j���i�s�x

	// �J���A�j���F�A�j���i�s�x�� 0 �� 255 �ɕω����܂�
	if (m_open_anime_type >= 0)	{
		anime_open = true;
		anime_type = m_open_anime_type;
		anime_prog = C_tnm_tool::time_table<int, int>(m_time, m_open_anime_start_time, 0, m_open_anime_start_time + m_open_anime_time, 255);
		if (anime_prog == 255)	{
			end_open_anime();		// �E�B���h�E�J���A�j���I��
		}
	}

	// ����A�j���F�A�j���i�s�x�� 255 �� 0 �ɕω����܂�
	if (m_close_anime_type >= 0)	{
		anime_open = false;
		anime_type = m_close_anime_type;
		anime_prog = C_tnm_tool::time_table<int, int>(m_time, m_close_anime_start_time, 255, m_close_anime_start_time + m_close_anime_time, 0);
		if (anime_prog == 0)	{
			end_close_anime();		// �E�B���h�E���A�j���I��
		}
	}

	// �X���C�h�A�j��
	int slide_pixel = 0;
	if (m_slide_anime_type >= 0)	{
		slide_pixel = C_tnm_tool::time_table<int, int>(m_time, m_slide_anime_start_time, m_slide_pixel, m_slide_anime_start_time + m_slide_anime_time, 0);
		if (m_time >= m_slide_anime_start_time + m_slide_anime_time)	{
			slide_pixel = 0;
			m_slide_anime_type = -1; 
		}
	}

	// �E�B���h�E���J���Ă��邩�A�E�B���h�E������A�j�����Ȃ�\�������ׂ�
	if (m_window_appear || m_close_anime_type >= 0)
	{
		// �E�B���h�E��`
		C_rect window_rect = C_rect::by_size(m_param.window_pos, m_param.window_size);

		// �E�B���h�E�A�j���ɂ��C���l
		C_point	pos_rep = C_point(0, 0);
		C_point	abs_center = C_point(0, 0);
		C_size	abs_scale = C_size(TNM_SCALE_UNIT, TNM_SCALE_UNIT);
		int		abs_rotate = 0;
		BYTE	tr_rep = 255;

		if (false);
		// �E�B���h�E�A�j���F�^�C�v�O�F�A�j���Ȃ�
		else if (anime_type == 0)	{
			// NOP
		}
		// �E�B���h�E�A�j���F���̑��̃^�C�v
		else	{
			// �E�B���h�E�Ɩ��O���͂���`�i�E�B���h�E�A�j���p�j
			C_rect win_name_rect = window_rect;
			if (m_name_appear && !m_name.is_empty())	{		// ���O���\������Ă���ꍇ
				C_rect name_rect = m_param.name_window_rect + m_param.name_window_pos + window_rect.left_top();
				if (win_name_rect.top > name_rect.top)			{	win_name_rect.top = name_rect.top;			}
				if (win_name_rect.bottom < name_rect.bottom)	{	win_name_rect.bottom = name_rect.bottom;	}
				if (win_name_rect.left > name_rect.left)		{	win_name_rect.left = name_rect.left;		}
				if (win_name_rect.right < name_rect.right)		{	win_name_rect.right = name_rect.right;		}
			}
			window_anime(anime_open, anime_type, anime_prog, &window_rect, &win_name_rect, &pos_rep, &abs_center, &abs_scale, &abs_rotate, &tr_rep);
		}

		// �Œ�E�B���h�E�̏ꍇ
		if (m_param.extend_type == 0)	{

			// �q�Ɉ����p�������_�[�p�����[�^
			S_tnm_render_param mwnd_tdp;
			mwnd_tdp.disp = true;
			mwnd_tdp.sorter = m_param.sorter;
			mwnd_tdp.center_x = (float)abs_center.x;
			mwnd_tdp.center_y = (float)abs_center.y;
			mwnd_tdp.scale_x = (float)abs_scale.cx / TNM_SCALE_UNIT;
			mwnd_tdp.scale_y = (float)abs_scale.cy / TNM_SCALE_UNIT;
			mwnd_tdp.rotate_z = D3DXToRadian((float)abs_rotate / TNM_ANGLE_UNIT);
			mwnd_tdp.tr = tr_rep;

			// ���b�Z�[�W�g�̃t���[������
			m_msg_waku.frame(m_time, window_rect + pos_rep, &mwnd_tdp, m_name_appear);

			// ���b�Z�[�W�̈ʒu���v�Z
			C_point msg_pos;
			if (Gp_ini->tateyoko_mode == 1)
			{
				msg_pos.x = window_rect.right - m_param.msg_pos.x - slide_pixel;
				msg_pos.y = window_rect.top + m_param.msg_pos.y;
			}
			else
			{
				msg_pos.x = window_rect.left + m_param.msg_pos.x;
				msg_pos.y = window_rect.top + m_param.msg_pos.y + slide_pixel;
			}

			// �X�N���[�����b�Z�[�W�ɔ����ăN���b�v�̈���쐬����
			// ��[�����ݒ�B���E���͉�ʂ����ς��܂Ŏ��
			if (m_slide_msg)
			{
				C_rect dst_clip(0, window_rect.top + m_param.msg_pos.y - Gp_ini->mwnd.sub[m_mwnd_no].moji_space.cy + pos_rep.y, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy);
				mwnd_tdp.dst_clip_use = true;
				mwnd_tdp.dst_clip_left = dst_clip.left;
				mwnd_tdp.dst_clip_top = dst_clip.top;
				mwnd_tdp.dst_clip_right = dst_clip.right;
				mwnd_tdp.dst_clip_bottom = dst_clip.bottom;
			}

			// ���b�Z�[�W�̃t���[������
			for (int i = 0; i < (int)m_msg_list.size(); i++)	{
				if (!m_msg_list[i].frame(&mwnd_tdp, msg_pos + pos_rep, 0, C_argb(0, 0, 0, 0), C_argb(0, 0, 0, 0), C_argb(0, 0, 0, 0), draw_futoku, draw_shadow))
					return false;
			}

			// �I�����̃t���[������
			m_sel.frame(&mwnd_tdp, msg_pos + pos_rep, draw_futoku, draw_shadow);
		}

		// �σE�B���h�E�̏ꍇ
		else	{

			// �q�Ɉ����p�������_�[�p�����[�^
			S_tnm_render_param mwnd_tdp;
			mwnd_tdp.disp = true;
			mwnd_tdp.sorter = m_param.sorter;
			mwnd_tdp.center_x = (float)abs_center.x;
			mwnd_tdp.center_y = (float)abs_center.y;
			mwnd_tdp.scale_x = (float)abs_scale.cx / TNM_SCALE_UNIT;
			mwnd_tdp.scale_y = (float)abs_scale.cy / TNM_SCALE_UNIT;
			mwnd_tdp.rotate_z = D3DXToRadian((float)abs_rotate / TNM_ANGLE_UNIT);
			mwnd_tdp.tr = tr_rep;

			// ���b�Z�[�W�g�̃t���[������
			m_msg_waku.frame(m_time, window_rect + pos_rep, &mwnd_tdp, m_name_appear);

			// ���b�Z�[�W��`���v�Z
			C_rect msg_rect = tnm_add_margin(window_rect, m_param.msg_margin);

			// �X�N���[�����b�Z�[�W�ɔ����ăN���b�v�̈���쐬����
			// ��[�����ݒ�B���E���͉�ʂ����ς��܂Ŏ��
			if (m_slide_msg)
			{
				C_rect dst_clip(0, window_rect.top + m_param.msg_pos.y - Gp_ini->mwnd.sub[m_mwnd_no].moji_space.cy + pos_rep.y, Gp_global->game_screen_size.cx, Gp_global->game_screen_size.cy);
				mwnd_tdp.dst_clip_left = dst_clip.left;
				mwnd_tdp.dst_clip_top = dst_clip.top;
				mwnd_tdp.dst_clip_right = dst_clip.right;
				mwnd_tdp.dst_clip_bottom = dst_clip.bottom;
				mwnd_tdp.dst_clip_use = true;
			}

			// ���b�Z�[�W�̈ʒu���v�Z
			C_point msg_pos(msg_rect.left + pos_rep.x, msg_rect.top + pos_rep.y);

			// ���b�Z�[�W�̃t���[������
			for (int i = 0; i < (int)m_msg_list.size(); i++)	{
				if (!m_msg_list[i].frame(&mwnd_tdp, msg_pos, 0, C_argb(0, 0, 0, 0), C_argb(0, 0, 0, 0), C_argb(0, 0, 0, 0), draw_futoku, draw_shadow))
					return false;
			}

			// �I�����̃t���[������
			m_sel.frame(&mwnd_tdp, msg_pos, draw_futoku, draw_shadow);
		}

		// ���O�E�B���h�E���\������Ă���ꍇ
		if (m_param.name_disp_mode == TNM_MWND_NAME_DISP_TYPE_NAME && m_name_appear && !m_name.is_empty())	{

			// �u��O����\�������Ƃ����O���B���v
			if (m_param.face_hide_name == 1 && m_msg_waku.is_exist_face())	{
				// NOP
			}
			else	{

				// ���O�E�B���h�E�ʒu
				C_point name_window_pos = m_param.name_window_pos + window_rect.left_top();

				// �Œ�E�B���h�E�̏ꍇ
				if (m_param.name_extend_type == 0)	{

					// �q�Ɉ����p�������_�[�p�����[�^
					S_tnm_render_param mwnd_tdp;
					mwnd_tdp.disp = true;
					mwnd_tdp.sorter = m_param.sorter;
					mwnd_tdp.center_x = (float)abs_center.x;
					mwnd_tdp.center_y = (float)abs_center.y;
					mwnd_tdp.scale_x = (float)abs_scale.cx / TNM_SCALE_UNIT;
					mwnd_tdp.scale_y = (float)abs_scale.cy / TNM_SCALE_UNIT;
					mwnd_tdp.rotate_z = D3DXToRadian((float)abs_rotate / TNM_ANGLE_UNIT);
					mwnd_tdp.tr = tr_rep;

					// ���b�Z�[�W�g�̃t���[������
					C_rect name_window_rect(m_param.name_window_rect + name_window_pos + pos_rep);
					m_name_waku.frame(m_time, name_window_rect, &mwnd_tdp, m_name_appear);

					// ���b�Z�[�W�̈ʒu���v�Z
					C_point name_msg_pos;

					if (false);
					else if (m_param.name_window_align == 0)	{	// ����
						name_msg_pos.x = name_window_pos.x + m_param.name_msg_pos.x + m_param.name_msg_pos_rep.x;
						name_msg_pos.y = name_window_pos.y + m_param.name_msg_pos.y + m_param.name_msg_pos_rep.y;
					}
					else if (m_param.name_window_align == 1)	{	// ������
						name_msg_pos.x = name_window_pos.x                          + m_param.name_msg_pos_rep.x;
						name_msg_pos.y = name_window_pos.y + m_param.name_msg_pos.y + m_param.name_msg_pos_rep.y;
					}
					else if (m_param.name_window_align == 2)	{	// �E��
						name_msg_pos.x = name_window_pos.x - m_param.name_msg_pos.x + m_param.name_msg_pos_rep.x;
						name_msg_pos.y = name_window_pos.y + m_param.name_msg_pos.y + m_param.name_msg_pos_rep.y;
					}

					// ���b�Z�[�W�̃t���[������
					m_name.frame(&mwnd_tdp, name_msg_pos + pos_rep, draw_futoku, draw_shadow);
				}

				// �σE�B���h�E�̏ꍇ
				else	{

					// �q�Ɉ����p�������_�[�p�����[�^
					S_tnm_render_param mwnd_tdp;
					mwnd_tdp.disp = true;
					mwnd_tdp.sorter = m_param.sorter;
					mwnd_tdp.center_x = (float)abs_center.x;
					mwnd_tdp.center_y = (float)abs_center.y;
					mwnd_tdp.scale_x = (float)abs_scale.cx / TNM_SCALE_UNIT;
					mwnd_tdp.scale_y = (float)abs_scale.cy / TNM_SCALE_UNIT;
					mwnd_tdp.rotate_z = (float)abs_rotate / TNM_ANGLE_UNIT;
					mwnd_tdp.tr = tr_rep;

					// ���b�Z�[�W�g�̃t���[������
					m_name_waku.frame(m_time, m_param.name_window_rect + name_window_pos + pos_rep, &mwnd_tdp, m_name_appear);

					// ���b�Z�[�W�̈ʒu���v�Z
					C_point name_msg_pos;

					if (false);
					else if (m_param.name_window_align == 0)	{	// ����
						name_msg_pos.x = name_window_pos.x + m_param.name_msg_margin.left  + m_param.name_msg_pos_rep.x;
						name_msg_pos.y = name_window_pos.y + m_param.name_msg_margin.top   + m_param.name_msg_pos_rep.y;
					}
					else if (m_param.name_window_align == 1)	{	// ������
						name_msg_pos.x = name_window_pos.x                                 + m_param.name_msg_pos_rep.x;
						name_msg_pos.y = name_window_pos.y + m_param.name_msg_margin.top   + m_param.name_msg_pos_rep.y;
					}
					else if (m_param.name_window_align == 2)	{	// �E��
						name_msg_pos.x = name_window_pos.x - m_param.name_msg_margin.right + m_param.name_msg_pos_rep.x;
						name_msg_pos.y = name_window_pos.y + m_param.name_msg_margin.top   + m_param.name_msg_pos_rep.y;
					}

					// ���b�Z�[�W�̃t���[������
					m_name.frame(&mwnd_tdp, name_msg_pos + pos_rep, draw_futoku, draw_shadow);
				}
			}
		}
	}

	return true;
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F�E�B���h�E�A�j������
// ================================================================
void C_elm_mwnd::window_anime(bool anime_open, int anime_type, int anime_prog, C_rect *window_rect, C_rect *win_name_rect, C_point *ret_pos_rep, C_point *ret_abs_center, C_size *ret_abs_scale, int *ret_abs_rotate, BYTE *ret_tr_rep)
{
	// �E�B���h�E�A�j���F�������p
	// anime_type = 39;

	C_point pos_rep = C_point(0, 0);
	C_point abs_center = C_point(window_rect->left + window_rect->width() / 2, window_rect->top + window_rect->height() / 2);
	C_size abs_scale = C_size(TNM_SCALE_UNIT, TNM_SCALE_UNIT);
	int abs_rotate = 0;
	BYTE tr_rep = 255;

	// --------------------------------------------------------------------------------------
	// �^�C�v�␳

	if (false);
	// �E�B���h�E�A�j���F�^�C�v�U�F�㉺�߂�������
	else if (anime_type == 6)	{
		// ���O�̗L�閳���ŕ������ɉe�����o�Ȃ��悤�A�E�B���h�E��`�Ōv�Z���܂�
		int type_2_length = window_rect->bottom;									// �ォ��
		int type_3_length = Gp_global->game_screen_size.cy - window_rect->top;		// ������
		anime_type = (type_2_length <= type_3_length) ? 2 : 3;
	}
	// �E�B���h�E�A�j���F�^�C�v�V�F���E�߂�������
	else if (anime_type == 7)	{
		// ���O�̗L�閳���ŕ������ɉe�����o�Ȃ��悤�A�E�B���h�E��`�Ōv�Z���܂�
		int type_4_length = window_rect->right;										// ������
		int type_5_length = Gp_global->game_screen_size.cx - window_rect->left;		// �E����
		anime_type = (type_4_length <= type_5_length) ? 4 : 5;
	}
	// �E�B���h�E�A�j���F�^�C�v�W�F�㉺���E�߂�������
	else if (anime_type == 8)	{
		// ���O�̗L�閳���ŕ������ɉe�����o�Ȃ��悤�A�E�B���h�E��`�Ōv�Z���܂�
		int type_2_length = window_rect->bottom;									// �ォ��
		int type_3_length = Gp_global->game_screen_size.cy - window_rect->top;		// ������
		int type_4_length = window_rect->right;										// ������
		int type_5_length = Gp_global->game_screen_size.cx - window_rect->left;		// �E����
		int type_ud, type_ud_length, type_lr, type_lr_length;
		if (type_2_length <= type_3_length)	{	type_ud = 2;	type_ud_length = type_2_length;	}
		else								{	type_ud = 3;	type_ud_length = type_3_length;	}
		if (type_4_length <= type_5_length)	{	type_lr = 4;	type_lr_length = type_4_length;	}
		else								{	type_lr = 5;	type_lr_length = type_5_length;	}
		anime_type = (type_ud_length <= type_lr_length) ? type_ud : type_lr;
	}

	// --------------------------------------------------------------------------------------
	// �g��k���n�A�j���̃p�����[�^

	bool	type_09_48_flag = false;
	int		ud_scale_mod = 0;		// 0=�Ȃ� 1=�g�� 2=�k��
	int		ud_scale_center = 0;	// 0=���S 1=��[ 2=���[ 3=��ʏ�[ 4=��ʉ��[
	int		lr_scale_mod = 0;		// 0=�Ȃ� 1=�g�� 2=�k��
	int		lr_scale_center = 0;	// 0=���S 1=���[ 2=�E�[ 3=��ʍ��[ 4=��ʉE�[

	if (false);
	// �E�B���h�E�A�j���F�^�C�v�O�X�F���S�ɑ΂��č��E�g��i�㉺�g��j
	// �E�B���h�E�A�j���F�^�C�v�P�O�F���S�ɑ΂��č��E�g��i�㉺�k���j
	// �E�B���h�E�A�j���F�^�C�v�P�P�F���S�ɑ΂��č��E�g��i�㉺���̂܂܁j
	// �E�B���h�E�A�j���F�^�C�v�P�Q�F���S�ɑ΂��č��E�k���i�㉺�g��j
	// �E�B���h�E�A�j���F�^�C�v�P�R�F���S�ɑ΂��č��E�k���i�㉺�k���j
	// �E�B���h�E�A�j���F�^�C�v�P�S�F���S�ɑ΂��č��E�k���i�㉺���̂܂܁j
	// �E�B���h�E�A�j���F�^�C�v�P�T�F���S�ɑ΂��č��E���̂܂܁i�㉺�g��j
	// �E�B���h�E�A�j���F�^�C�v�P�U�F���S�ɑ΂��č��E���̂܂܁i�㉺�k���j
	else if (anime_type == 9)	{	type_09_48_flag = true;		ud_scale_mod = 1;	ud_scale_center = 0;	lr_scale_mod = 1;	lr_scale_center = 0;	}
	else if (anime_type == 10)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 1;	lr_scale_center = 0;	}
	else if (anime_type == 11)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 1;	lr_scale_center = 0;	}
	else if (anime_type == 12)	{	type_09_48_flag = true;		ud_scale_mod = 1;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	}
	else if (anime_type == 13)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	}
	else if (anime_type == 14)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	}
	else if (anime_type == 15)	{	type_09_48_flag = true;		ud_scale_mod = 1;	ud_scale_center = 0;	lr_scale_mod = 0;	lr_scale_center = 0;	}
	else if (anime_type == 16)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 0;	lr_scale_center = 0;	}
	// �E�B���h�E�A�j���F�^�C�v�P�V�F���[�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�P�W�F�E�[�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�P�X�F��[�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�Q�O�F���[�ɑ΂��č��E�k��
	else if (anime_type == 17)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 1;	}
	else if (anime_type == 18)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 2;	}
	else if (anime_type == 19)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 1;	lr_scale_mod = 0;	lr_scale_center = 0;	}
	else if (anime_type == 20)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 2;	lr_scale_mod = 0;	lr_scale_center = 0;	}
	// �E�B���h�E�A�j���F�^�C�v�Q�P�F����p�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�Q�Q�F�E��p�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�Q�R�F�����p�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�Q�S�F�E���p�ɑ΂��č��E�k��
	else if (anime_type == 21)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 1;	lr_scale_mod = 2;	lr_scale_center = 1;	}
	else if (anime_type == 22)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 1;	lr_scale_mod = 2;	lr_scale_center = 2;	}
	else if (anime_type == 23)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 2;	lr_scale_mod = 2;	lr_scale_center = 1;	}
	else if (anime_type == 24)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 2;	lr_scale_mod = 2;	lr_scale_center = 2;	}
	// �E�B���h�E�A�j���F�^�C�v�Q�T�F��ʍ��[�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�Q�U�F��ʉE�[�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�Q�V�F��ʏ�[�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�Q�W�F��ʉ��[�ɑ΂��č��E�k��
	else if (anime_type == 25)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 3;	}
	else if (anime_type == 26)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 4;	}
	else if (anime_type == 27)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 3;	lr_scale_mod = 0;	lr_scale_center = 0;	}
	else if (anime_type == 28)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 4;	lr_scale_mod = 0;	lr_scale_center = 0;	}

	// --------------------------------------------------------------------------------------
	// ��]�n�A�j���̃p�����[�^

	int		rotate_cnt = 0;		// 4 �łP��]

	if (false);
	// �E�B���h�E�A�j���F�^�C�v�Q�X�F���S�ɑ΂��ďk�����Ȃ���E�ɂP��]
	// �E�B���h�E�A�j���F�^�C�v�R�O�F���S�ɑ΂��ďk�����Ȃ��獶�ɂP��]
	else if (anime_type == 29)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = -4;	}
	else if (anime_type == 30)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = 4;		}
	// �E�B���h�E�A�j���F�^�C�v�R�P�F���S�ɑ΂��ďk�����Ȃ���E�ɂQ��]
	// �E�B���h�E�A�j���F�^�C�v�R�Q�F���S�ɑ΂��ďk�����Ȃ��獶�ɂQ��]
	else if (anime_type == 31)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = -8;	}
	else if (anime_type == 32)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = 8;		}

	// �E�B���h�E�A�j���F�^�C�v�R�R�F���S�ɑ΂��Ċg�債�Ȃ���E�ɂP��]
	// �E�B���h�E�A�j���F�^�C�v�R�S�F���S�ɑ΂��Ċg�債�Ȃ��獶�ɂP��]
	else if (anime_type == 33)	{	type_09_48_flag = true;		ud_scale_mod = 1;	ud_scale_center = 0;	lr_scale_mod = 1;	lr_scale_center = 0;	rotate_cnt = -4;	}
	else if (anime_type == 34)	{	type_09_48_flag = true;		ud_scale_mod = 1;	ud_scale_center = 0;	lr_scale_mod = 1;	lr_scale_center = 0;	rotate_cnt = 4;		}
	// �E�B���h�E�A�j���F�^�C�v�R�T�F���S�ɑ΂��Ċg�債�Ȃ���E�ɂQ��]
	// �E�B���h�E�A�j���F�^�C�v�R�U�F���S�ɑ΂��Ċg�債�Ȃ��獶�ɂQ��]
	else if (anime_type == 35)	{	type_09_48_flag = true;		ud_scale_mod = 1;	ud_scale_center = 0;	lr_scale_mod = 1;	lr_scale_center = 0;	rotate_cnt = -8;	}
	else if (anime_type == 36)	{	type_09_48_flag = true;		ud_scale_mod = 1;	ud_scale_center = 0;	lr_scale_mod = 1;	lr_scale_center = 0;	rotate_cnt = 8;		}

	// �E�B���h�E�A�j���F�^�C�v�R�V�F���S�ɑ΂��ď㉺�k�����Ȃ���E�ɂP��]
	// �E�B���h�E�A�j���F�^�C�v�R�W�F���S�ɑ΂��ď㉺�k�����Ȃ��獶�ɂP��]
	else if (anime_type == 37)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 0;	lr_scale_center = 0;	rotate_cnt = -4;	}
	else if (anime_type == 38)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 0;	lr_scale_center = 0;	rotate_cnt = 4;		}
	// �E�B���h�E�A�j���F�^�C�v�R�X�F���S�ɑ΂��č��E�k�����Ȃ���E�ɂP��]
	// �E�B���h�E�A�j���F�^�C�v�S�O�F���S�ɑ΂��č��E�k�����Ȃ��獶�ɂP��]
	else if (anime_type == 39)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = -4;	}
	else if (anime_type == 40)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = 4;		}

	// �E�B���h�E�A�j���F�^�C�v�S�P�F���S�ɑ΂��ď㉺�k�����Ȃ���E�ɔ���]
	// �E�B���h�E�A�j���F�^�C�v�S�Q�F���S�ɑ΂��ď㉺�k�����Ȃ��獶�ɔ���]
	else if (anime_type == 41)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 0;	lr_scale_center = 0;	rotate_cnt = -2;	}
	else if (anime_type == 42)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 0;	lr_scale_center = 0;	rotate_cnt = 2;		}
	// �E�B���h�E�A�j���F�^�C�v�S�R�F���S�ɑ΂��č��E�k�����Ȃ���E�ɔ���]
	// �E�B���h�E�A�j���F�^�C�v�S�S�F���S�ɑ΂��č��E�k�����Ȃ��獶�ɔ���]
	else if (anime_type == 43)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = -2;	}
	else if (anime_type == 44)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = 2;		}

	// �E�B���h�E�A�j���F�^�C�v�S�T�F���S�ɑ΂��ď㉺�k�����Ȃ���E�Ɏl����]
	// �E�B���h�E�A�j���F�^�C�v�S�U�F���S�ɑ΂��ď㉺�k�����Ȃ��獶�Ɏl����]
	else if (anime_type == 45)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 0;	lr_scale_center = 0;	rotate_cnt = -1;	}
	else if (anime_type == 46)	{	type_09_48_flag = true;		ud_scale_mod = 2;	ud_scale_center = 0;	lr_scale_mod = 0;	lr_scale_center = 0;	rotate_cnt = 1;		}
	// �E�B���h�E�A�j���F�^�C�v�S�V�F���S�ɑ΂��č��E�k�����Ȃ���E�Ɏl����]
	// �E�B���h�E�A�j���F�^�C�v�S�W�F���S�ɑ΂��č��E�k�����Ȃ��獶�Ɏl����]
	else if (anime_type == 47)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = -1;	}
	else if (anime_type == 48)	{	type_09_48_flag = true;		ud_scale_mod = 0;	ud_scale_center = 0;	lr_scale_mod = 2;	lr_scale_center = 0;	rotate_cnt = 1;		}

	// --------------------------------------------------------------------------------------
	// �ʂ̃A�j������

	bool	tr_anime_flag = false;

	if (false);
	// �E�B���h�E�A�j���F�^�C�v�O�O�F�A�j���Ȃ�
	else if (anime_type == 0)	{
		// NOP
	}
	// �E�B���h�E�A�j���F�^�C�v�O�P�F�t�F�[�h
	else if (anime_type == 1)	{
		tr_rep = anime_prog;
	}
	// �E�B���h�E�A�j���F�^�C�v�O�Q�F�ォ��
	else if (anime_type == 2)	{
		tr_anime_flag = true;
		int length = -win_name_rect->bottom;
		if (anime_open)	{	pos_rep.y = speed_down_limit(anime_prog, 0, length, 255, 0);	}
		else			{	pos_rep.y = speed_up_limit(anime_prog, 0, length, 255, 0);		}
	}
	// �E�B���h�E�A�j���F�^�C�v�O�R�F������
	else if (anime_type == 3)	{
		tr_anime_flag = true;
		int length = Gp_global->game_screen_size.cy - win_name_rect->top;
		if (anime_open)	{	pos_rep.y = speed_down_limit(anime_prog, 0, length, 255, 0);	}
		else			{	pos_rep.y = speed_up_limit(anime_prog, 0, length, 255, 0);		}
	}
	// �E�B���h�E�A�j���F�^�C�v�O�S�F������
	else if (anime_type == 4)	{
		tr_anime_flag = true;
		int length = -win_name_rect->right;
		if (anime_open)	{	pos_rep.x = speed_down_limit(anime_prog, 0, length, 255, 0);	}
		else			{	pos_rep.x = speed_up_limit(anime_prog, 0, length, 255, 0);		}
	}
	// �E�B���h�E�A�j���F�^�C�v�O�T�F�E����
	else if (anime_type == 5)	{
		tr_anime_flag = true;
		int length = Gp_global->game_screen_size.cx - win_name_rect->left;
		if (anime_open)	{	pos_rep.x = speed_down_limit(anime_prog, 0, length, 255, 0);	}
		else			{	pos_rep.x = speed_up_limit(anime_prog, 0, length, 255, 0);		}
	}

	// �E�B���h�E�A�j���F�^�C�v�O�U�F�㉺�߂�������
	// �E�B���h�E�A�j���F�^�C�v�O�V�F���E�߂�������
	// �E�B���h�E�A�j���F�^�C�v�O�W�F�㉺���E�߂�������

	// �E�B���h�E�A�j���F�^�C�v�O�X�F���S�ɑ΂��č��E�g��i�㉺�g��j
	// �E�B���h�E�A�j���F�^�C�v�P�O�F���S�ɑ΂��č��E�g��i�㉺�k���j
	// �E�B���h�E�A�j���F�^�C�v�P�P�F���S�ɑ΂��č��E�g��i�㉺���̂܂܁j
	// �E�B���h�E�A�j���F�^�C�v�P�Q�F���S�ɑ΂��č��E�k���i�㉺�g��j
	// �E�B���h�E�A�j���F�^�C�v�P�R�F���S�ɑ΂��č��E�k���i�㉺�k���j
	// �E�B���h�E�A�j���F�^�C�v�P�S�F���S�ɑ΂��č��E�k���i�㉺���̂܂܁j
	// �E�B���h�E�A�j���F�^�C�v�P�T�F���S�ɑ΂��č��E���̂܂܁i�㉺�g��j
	// �E�B���h�E�A�j���F�^�C�v�P�U�F���S�ɑ΂��č��E���̂܂܁i�㉺�k���j
	// �E�B���h�E�A�j���F�^�C�v�P�V�F���[�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�P�W�F�E�[�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�P�X�F��[�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�Q�O�F���[�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�Q�P�F����p�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�Q�Q�F�E��p�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�Q�R�F�����p�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�Q�S�F�E���p�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�Q�T�F��ʍ��[�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�Q�U�F��ʉE�[�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�Q�V�F��ʏ�[�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�Q�W�F��ʉ��[�ɑ΂��č��E�k��
	// �E�B���h�E�A�j���F�^�C�v�Q�X�F���S�ɑ΂��ďk�����Ȃ���E�ɂP��]
	// �E�B���h�E�A�j���F�^�C�v�R�O�F���S�ɑ΂��ďk�����Ȃ��獶�ɂP��]
	// �E�B���h�E�A�j���F�^�C�v�R�P�F���S�ɑ΂��ďk�����Ȃ���E�ɂQ��]
	// �E�B���h�E�A�j���F�^�C�v�R�Q�F���S�ɑ΂��ďk�����Ȃ��獶�ɂQ��]
	// �E�B���h�E�A�j���F�^�C�v�R�R�F���S�ɑ΂��Ċg�債�Ȃ���E�ɂP��]
	// �E�B���h�E�A�j���F�^�C�v�R�S�F���S�ɑ΂��Ċg�債�Ȃ��獶�ɂP��]
	// �E�B���h�E�A�j���F�^�C�v�R�T�F���S�ɑ΂��Ċg�債�Ȃ���E�ɂQ��]
	// �E�B���h�E�A�j���F�^�C�v�R�U�F���S�ɑ΂��Ċg�債�Ȃ��獶�ɂQ��]
	// �E�B���h�E�A�j���F�^�C�v�R�V�F���S�ɑ΂��ď㉺�k�����Ȃ���E�ɂP��]
	// �E�B���h�E�A�j���F�^�C�v�R�W�F���S�ɑ΂��ď㉺�k�����Ȃ��獶�ɂP��]
	// �E�B���h�E�A�j���F�^�C�v�R�X�F���S�ɑ΂��č��E�k�����Ȃ���E�ɂP��]
	// �E�B���h�E�A�j���F�^�C�v�S�O�F���S�ɑ΂��č��E�k�����Ȃ��獶�ɂP��]
	// �E�B���h�E�A�j���F�^�C�v�S�P�F���S�ɑ΂��ď㉺�k�����Ȃ���E�ɔ���]
	// �E�B���h�E�A�j���F�^�C�v�S�Q�F���S�ɑ΂��ď㉺�k�����Ȃ��獶�ɔ���]
	// �E�B���h�E�A�j���F�^�C�v�S�R�F���S�ɑ΂��č��E�k�����Ȃ���E�ɔ���]
	// �E�B���h�E�A�j���F�^�C�v�S�S�F���S�ɑ΂��č��E�k�����Ȃ��獶�ɔ���]
	// �E�B���h�E�A�j���F�^�C�v�S�T�F���S�ɑ΂��ď㉺�k�����Ȃ���E�Ɏl����]
	// �E�B���h�E�A�j���F�^�C�v�S�U�F���S�ɑ΂��ď㉺�k�����Ȃ��獶�Ɏl����]
	// �E�B���h�E�A�j���F�^�C�v�S�V�F���S�ɑ΂��č��E�k�����Ȃ���E�Ɏl����]
	// �E�B���h�E�A�j���F�^�C�v�S�W�F���S�ɑ΂��č��E�k�����Ȃ��獶�Ɏl����]
	else if (type_09_48_flag)	{
		tr_anime_flag = true;
	}

	// �E�B���h�E�A�j���F�^�C�v�X�X�i�����Ă��Ɨp�j
	else if (anime_type == 99)	{
		pos_rep.x = (255 - anime_prog) * 800 / 255;
	}

	// --------------------------------------------------------------------------------------
	// �����x�A�j������
	if (tr_anime_flag)	{
		tr_rep = C_tnm_tool::time_table<int, int>(anime_prog, 0, 0, 224, 255);
	}

	// --------------------------------------------------------------------------------------
	// �g��k���n�̃A�j������

	// �㉺
	if (ud_scale_mod)	{	// 0=�Ȃ� 1=�g�� 2=�k��
		if (ud_scale_center == 0)	{		// ���S
			abs_center.y = window_rect->top + (window_rect->height() / 2);
		}
		else if (ud_scale_center == 1)	{	// ��[
			abs_center.y = window_rect->top;
		}
		else if (ud_scale_center == 2)	{	// ���[
			abs_center.y = window_rect->bottom;
		}
		else if (ud_scale_center == 3)	{	// ��ʏ�[
			abs_center.y = -(Gp_global->game_screen_size.cy / 16);
		}
		else if (ud_scale_center == 4)	{	// ��ʉ��[
			abs_center.y = Gp_global->game_screen_size.cy + (Gp_global->game_screen_size.cy / 16);
		}
		int start_scale = (ud_scale_mod == 1) ? (TNM_SCALE_UNIT * 3) : 0;
		if (anime_open)	{	abs_scale.cy = speed_down_limit(anime_prog, 0, start_scale, 255, TNM_SCALE_UNIT);	}
		else			{	abs_scale.cy = speed_up_limit(anime_prog, 0, start_scale, 255, TNM_SCALE_UNIT);	}
	}

	// ���E
	if (lr_scale_mod)	{	// 0=�Ȃ� 1=�g�� 2=�k��
		if (lr_scale_center == 0)	{		// ���S
			abs_center.x = window_rect->left + (window_rect->width() / 2);
		}
		else if (lr_scale_center == 1)	{	// ���[
			abs_center.x = window_rect->left;
		}
		else if (lr_scale_center == 2)	{	// �E�[
			abs_center.x = window_rect->right;
		}
		else if (lr_scale_center == 3)	{	// ��ʍ��[
			abs_center.x = -(Gp_global->game_screen_size.cx / 16);
		}
		else if (lr_scale_center == 4)	{	// ��ʉE�[
			abs_center.x = Gp_global->game_screen_size.cx + (Gp_global->game_screen_size.cx / 16);
		}
		int start_scale = (lr_scale_mod == 1) ? (TNM_SCALE_UNIT * 3) : 0;
		if (anime_open)	{	abs_scale.cx = speed_down_limit(anime_prog, 0, start_scale, 255, TNM_SCALE_UNIT);	}
		else			{	abs_scale.cx = speed_up_limit(anime_prog, 0, start_scale, 255, TNM_SCALE_UNIT);		}
	}

	// --------------------------------------------------------------------------------------
	// ��]�n�̃A�j������

	if (rotate_cnt)	{
		int start_rotate = rotate_cnt * 90 * TNM_ANGLE_UNIT;		// 4 �łP��]
		if (anime_open)	{	abs_rotate = speed_down_limit(anime_prog, 0, start_rotate, 255, 0);		}
		else			{	abs_rotate = speed_up_limit(anime_prog, 0, start_rotate, 255, 0);		}
	}

	// --------------------------------------------------------------------------------------
	// �p�����[�^�ڐA

	*ret_pos_rep += pos_rep;
	*ret_abs_center = abs_center;
	*ret_abs_scale = abs_scale;
	*ret_abs_rotate = abs_rotate;
	*ret_tr_rep = (BYTE)((int)*ret_tr_rep * (int)tr_rep / 255);
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F���b�Z�[�W��i�߂�
// ================================================================
void C_elm_mwnd::msg_frame()
{
	// ================================================================
	// ���b�Z�[�W�E�B���h�E�̏�������
	// ----------------------------------------------------------------
	//		�P�D���O��\������
	//		�Q�D�J���A�j�������s����
	//		�R�D���b�Z�[�W��\��
	// ----------------------------------------------------------------

	// ���������t���O�������Ă���ꍇ
	if (m_auto_proc_ready_flag)	{
		
		// �u��������v�܂��́u�����Ȃ��v�̏ꍇ�̓��b�Z�[�W�E�B���h�E��\������
		// �u�����̂݁v�ł��A�����Ȃ��܂��͐����Đ����Ȃ����b�Z�[�W�̏ꍇ�̓��b�Z�[�W�E�B���h�E��\������
		if (Gp_config->koe_mode == TNM_KOE_MODE_NORMAL || Gp_config->koe_mode == TNM_KOE_MODE_TEXTONLY || m_koe_no == -1 || !m_koe_play_flag)	{

			// ���O��\��
			m_name_appear = true;

			// �E�B���h�E���J��
			open();

			// �A�j�����I����Ă���΃��b�Z�[�W��\������
			if (m_open_anime_type == -1)	{

				// �X���C�h�A�j�����Ȃ牽�����Ȃ�
				if (m_slide_anime_type == -1)	{

					// �I����
					if (!m_sel.is_empty())	{
						// �I������\������
						m_sel.set_disp();
					}

					// �ʏ�̃��b�Z�[�W
					for (int i = 0; i < (int)m_msg_list.size(); i++)	{

						// ���b�Z�[�W�J�n����
						if (!m_msg_list[i].get_progress_start_flag())	{
							m_msg_list[i].set_progress_start_flag(true);

							// �Ō�ɕ\���������Ԃ����Z�b�g
							m_msg_list[i].set_last_moji_disp_time(m_time);
						}

						// ���Ԃ������玟�̕�����\������
						while (m_msg_list[i].check_not_appear_moji() && (m_time - (m_msg_list[i].get_last_moji_disp_time() + tnm_get_total_msg_speed()) >= 0))	{

							// �Ō�ɕ\���������Ԃ��o���Ă���
							m_msg_list[i].set_last_moji_disp_time(m_msg_list[i].get_last_moji_disp_time() + tnm_get_total_msg_speed());

							// �I�[�o�[�t���[���͕\�������ɔ�����
							//if (m_msg_list[i].check_over_flow())
							//	break;

							// �P�����\��
							m_msg_list[i].disp_one_moji();
						}
					}
				}
			}
		}
		// �u�����̂݁v�Ő������郁�b�Z�[�W�̏ꍇ�̓E�B���h�E��\�����Ȃ�
		else	{

			// ���O���\��
			m_name_appear = false;

			// �E�B���h�E�����
			close();

			// �A�j�����I����Ă��Ă���΃��b�Z�[�W��\������
			if (m_close_anime_type == -1)	{

				// ���b�Z�[�W�J�n
				for (int i = 0; i < (int)m_msg_list.size(); i++)	{
					m_msg_list[i].set_progress_start_flag(true);
				}

				// �ꊇ�\���i���ۂ͕\������Ȃ��j
				disp_all_moji();
			}
		}

		// �S�ĕ\�������玩�������𔲂���
		if (!check_not_appear_moji())
			m_auto_proc_ready_flag = false;
	}
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F���b�Z�[�W�E�F�C�g�܂ŏ�����i�߂�
// ================================================================
void C_elm_mwnd::jump_to_msg_wait_by_nowait()
{
	// ���������t���O�������Ă���ꍇ
	if (m_auto_proc_ready_flag)	{

		// �u��������v�܂��́u�����Ȃ��v�A�܂��͐����Ȃ��A�����Đ����Ȃ����b�Z�[�W�̏ꍇ�̓E�B���h�E��\������
		if (Gp_config->koe_mode == TNM_KOE_MODE_NORMAL || Gp_config->koe_mode == TNM_KOE_MODE_TEXTONLY || m_koe_no == -1 || !m_koe_play_flag)	{
			m_name_appear = true;		// ���O��\��
			open();						// �E�B���h�E���J��
//			if (m_open_anime_type == -1)	{
				end_open_anime();				// �J���A�j�����I������
				if (!is_slide_anime_doing())	{
					for (int i = 0; i < (int)m_msg_list.size(); i++)	{
						m_msg_list[i].set_progress_start_flag(true);	// ���b�Z�[�W�J�n
					}
					disp_all_moji();				// ���b�Z�[�W��\������

					// �S�ĕ\�������玩�������𔲂���
					if (!check_not_appear_moji())
						m_auto_proc_ready_flag = false;
				}
//			}
		}
		// �u�����̂݁v�Ő������郁�b�Z�[�W�̏ꍇ�̓E�B���h�E��\�����Ȃ�
		else	{
			m_name_appear = false;		// ���O���\��
			m_window_appear = false;	// �E�B���h�E���\��
			end_open_anime();			// �J���A�j�����I������
			end_close_anime();			// ���A�j�����I������

//			close();					// �E�B���h�E�����i��������Ɩ����Ăяo���ŋ����I���ɂȂ�j
//			if (m_close_anime_type == -1)	{
				end_close_anime();				// ���A�j�����I������
				if (!is_slide_anime_doing())	{
					for (int i = 0; i < (int)m_msg_list.size(); i++)	{
						m_msg_list[i].set_progress_start_flag(true);	// ���b�Z�[�W�J�n
					}
					disp_all_moji();				// ���b�Z�[�W��\������

					// �S�ĕ\�������玩�������𔲂���
					if (!check_not_appear_moji())
						m_auto_proc_ready_flag = false;
				}
//			}
		}
	}
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E�F�t���[���A�N�V��������
// ================================================================
void C_elm_mwnd::do_frame_action()
{
	// ���b�Z�[�W�g
	m_msg_waku.do_frame_action();

	// ���O�g
	m_name_waku.do_frame_action();
}

// ****************************************************************
// �I�u�W�F�N�g�F�X�v���C�g�c���[���\�z
// ================================================================
void C_elm_mwnd::get_sprite_tree(C_tree2<C_tnm_sprite *>& root)
{
	// �E�B���h�E���J���Ă��邩�A�E�B���h�E������A�j�����Ȃ�\�������ׂ�
	if (m_window_appear || (m_close_anime_type >= 0))	{

		// ���b�Z�[�W
		for (int i = 0; i < (int)m_msg_list.size(); i++)
			m_msg_list[i].get_sprite_tree(root);

		// ���b�Z�[�W�g
		m_msg_waku.get_sprite_tree(root);

		// ���O
		m_name.get_sprite_tree(root);

		// ���O�g
		m_name_waku.get_sprite_tree(root);

		// �I����
		m_sel.get_sprite_tree(root);
	}
}

// ****************************************************************
// �{�^���̓o�^����
// ================================================================
void C_elm_mwnd::regist_button()
{
	// �E�B���h�E���J���Ă��邩�A�E�B���h�E������A�j�����Ȃ�\�������ׂ�
	if (m_window_appear || (m_close_anime_type >= 0))	{

		// ���b�Z�[�W
		for (int i = 0; i < (int)m_msg_list.size(); i++)
			m_msg_list[i].regist_button();

		// �I����
		if (tnm_get_cur_proc()->type == TNM_PROC_TYPE_SEL)	{
			m_sel.regist_button();
		}

		// �g
		m_msg_waku.regist_button();
	}
}

// ****************************************************************
// �{�^���C�x���g����
// ================================================================
void C_elm_mwnd::button_event(bool wait_flag)
{
	if (m_window_appear)	{

		// ���b�Z�[�W
		for (int i = 0; i < (int)m_msg_list.size(); i++)
			m_msg_list[i].button_event();

		// �I����
		m_sel.button_event(wait_flag);

		// �g
		m_msg_waku.button_event();
	}
}

// ****************************************************************
// ���b�Z�[�W�u���b�N���J�n�����t���O��ݒ肷��
// ================================================================
void C_elm_mwnd::clear_msg_block_start_flag()
{
	m_msg_block_started_flag = false;
}
void C_elm_mwnd::set_msg_block_start_flag()
{
	m_msg_block_started_flag = true;
}
bool C_elm_mwnd::is_msg_block_started()
{
	return m_msg_block_started_flag;
}

// ****************************************************************
// ���������J�n
// ================================================================
void C_elm_mwnd::set_auto_proc_ready()
{
	m_auto_proc_ready_flag = true;
}

// ****************************************************************
// ���������I��
// ================================================================
void C_elm_mwnd::clear_auto_proc_ready()
{
	m_auto_proc_ready_flag = false;
}

// ****************************************************************
// �J��
// ================================================================
void C_elm_mwnd::open()
{
	// ���A�j���I��
	end_close_anime();

	// ���Ă���Ȃ�J���A�j���J�n
	if (!m_window_appear)	{
		m_window_appear = true;

		if (tnm_is_mwnd_anime_use())	{
			m_open_anime_type = m_param.open_anime_type;
			m_open_anime_time = m_param.open_anime_time;
			m_open_anime_start_time = m_time;
		}
	}
}

// ****************************************************************
// ����
// ================================================================
void C_elm_mwnd::close()
{
	// ���b�Z�[�W�������Ō�܂Ői�߂�
	jump_to_msg_wait_by_nowait();

	// ���A�j���I��
	end_open_anime();

	// �J���Ă���Ȃ���A�j���J�n
	if (m_window_appear)	{
		m_window_appear = false;

		if (tnm_is_mwnd_anime_use())	{
			m_close_anime_type = m_param.close_anime_type;
			m_close_anime_time = m_param.close_anime_time;
			m_close_anime_start_time = m_time;
		}
	}
}

// ****************************************************************
// ������I��
// ================================================================
void C_elm_mwnd::end_close()
{
	// ���A�j���I��
	end_close_anime();
}

// ****************************************************************
// ���̏���
// ================================================================

// ���̃N���A
void C_elm_mwnd::clear_koe()
{
	m_koe_no = -1;
	m_koe_play_flag = false;
}

// ����ݒ�
void C_elm_mwnd::set_koe(int koe_no, bool play_flag, bool no_auto_mode_flag)
{
	clear_koe();

	// ���O��ݒ肷��
	m_koe_no = koe_no;
	m_koe_play_flag = play_flag;
	m_koe_no_auto_mode_flag = no_auto_mode_flag;
}

// ****************************************************************
// ���O����
// ================================================================

// ���O�̃N���A
void C_elm_mwnd::clear_name()
{
	m_name.clear_name();
}

// ���O�̐ݒ�
void C_elm_mwnd::set_name(CTSTR& name)
{
	// ���O��ݒ肷��
	m_name.set_name(name);

	// ���O�g�̍č\�z
	restruct_name_waku();
}

// ���O�̎擾
TSTR C_elm_mwnd::get_name()
{
	return m_name.get_name();
}

// ****************************************************************
// ��O������
// ================================================================

// ��O���̃N���A
void C_elm_mwnd::clear_face()
{
	m_msg_waku.clear_face();
}

// ��O���̐ݒ�
void C_elm_mwnd::set_face(int face_no, CTSTR& face_path)
{
	// ���O��ݒ肷��
	m_msg_waku.set_face(face_no, face_path);
}

// ****************************************************************
// ���b�Z�[�W����
// ================================================================

// ���b�Z�[�W�̃N���A
void C_elm_mwnd::clear_msg()
{
	for (int i = 0; i < (int)m_msg_list.size(); i++)
		m_msg_list[i].clear_msg();

	// �^�[�Q�b�g���b�Z�[�W�����ɖ߂�
	m_target_msg_no = 0;

	// ���b�Z�[�W���P�ɖ߂�
	m_msg_list.resize(1);

	// �I�[�g���[�h�����܂ő҂����J�E���g�����Z�b�g
	m_auto_mode_end_moji_cnt = 0;

	// �L�[�҂��A�C�R��
	m_msg_waku.set_key_icon_appear(false);
}

// ���b�Z�[�W�����邩���`�F�b�N
bool C_elm_mwnd::add_msg_check(bool new_line_flag)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return false;

	// ���b�Z�[�W�����邩���`�F�b�N
	return m_msg_list[m_target_msg_no].add_msg_check(m_param.over_flow_check_size, new_line_flag);
}

// ���b�Z�[�W�̒ǉ�
void C_elm_mwnd::add_msg(CTSTR& msg, TSTR* ret_over_flow_msg)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	// ���b�Z�[�W��ǉ�����
	m_msg_list[m_target_msg_no].add_msg(msg, ret_over_flow_msg);

	// ���b�Z�[�W�g���č\�z����
	restruct_msg_waku();
}

// ���O�����b�Z�[�W�Ƃ��Ēǉ�
void C_elm_mwnd::add_name_to_msg(CTSTR& name, TSTR* ret_over_flow_msg)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	// ���b�Z�[�W��ǉ�����
	m_msg_list[m_target_msg_no].add_name_to_msg(name, ret_over_flow_msg);

	// ���b�Z�[�W�g���č\�z����
	restruct_msg_waku();
}

// ���������擾
int C_elm_mwnd::get_moji_cnt()
{
	return m_msg_list[0].get_moji_cnt();
}

// �\������
int C_elm_mwnd::get_disp_moji_cnt()
{
	return m_msg_list[0].get_disp_moji_cnt();
}

// �P�̕�����\��
void C_elm_mwnd::disp_one_moji()
{
	m_msg_list[0].disp_one_moji();
	m_sel.set_disp();
}

// �S�Ă̕�����\��
void C_elm_mwnd::disp_all_moji()
{
	for (int i = 0; i < (int)m_msg_list.size(); i++)
		m_msg_list[i].disp_all_moji();

	m_sel.set_disp();
}

// �s�̐擪�ɂ���
void C_elm_mwnd::set_line_head()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_line_head();
}
// ���s
void C_elm_mwnd::new_line_indent()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].new_line_indent();
}
void C_elm_mwnd::new_line_no_indent()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].new_line_no_indent();
}

// �C���f���g�ݒ�
void C_elm_mwnd::set_indent()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_indent(0);
}

// �C���f���g���N���A
void C_elm_mwnd::clear_indent()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].clear_indent();
}

// �����̈ʒu��ݒ�
void C_elm_mwnd::set_moji_rep_pos(C_point pos)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_moji_rep_pos(pos);
}

// �����̈ʒu���f�t�H���g�ɖ߂�
void C_elm_mwnd::set_moji_rep_pos_default()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_moji_rep_pos_default();
}

// �����̑傫����ݒ�
void C_elm_mwnd::set_moji_size(int size)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_moji_size(size);
}

void C_elm_mwnd::set_moji_size_default()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_moji_size_default();
}

// �����̐F��ݒ�
void C_elm_mwnd::set_moji_color(int color_no)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_moji_color(color_no);
}
void C_elm_mwnd::set_moji_color_default()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_moji_color_default();
}

// �L�����N�^�����F���N���A
void C_elm_mwnd::clear_chara_moji_color()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].clear_chara_moji_color();

	// ���O�̐F
	m_name.clear_chara_moji_color();
}
void C_elm_mwnd::clear_chara_shadow_color()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].clear_chara_shadow_color();

	// ���O�̐F
	m_name.clear_chara_shadow_color();
}
void C_elm_mwnd::clear_chara_fuchi_color()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].clear_chara_fuchi_color();

	// ���O�̐F
	m_name.clear_chara_fuchi_color();
}

// �L�����N�^�����F��ݒ�
void C_elm_mwnd::set_chara_moji_color(int color_no)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_chara_moji_color(color_no);

	// ���O�̐F
	m_name.set_chara_moji_color(color_no);
}
void C_elm_mwnd::set_chara_shadow_color(int color_no)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_chara_shadow_color(color_no);

	// ���O�̐F
	m_name.set_chara_shadow_color(color_no);
}
void C_elm_mwnd::set_chara_fuchi_color(int color_no)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].set_chara_fuchi_color(color_no);

	// ���O�̐F
	m_name.set_chara_fuchi_color(color_no);
}

// ���r��ݒ�
void C_elm_mwnd::start_ruby(CTSTR& ruby)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].start_ruby(ruby);
}
void C_elm_mwnd::end_ruby()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].end_ruby();
}

// �{�^����ݒ�
void C_elm_mwnd::start_button(int btn_id, int group_no, int action_no, int se_no)
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].start_button(btn_id, group_no, action_no, se_no);
}
void C_elm_mwnd::end_button()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].end_button();
}

// �Ō�ɕ\���������Ԃ�ݒ肷��
void C_elm_mwnd::set_last_moji_disp_time()
{
	for (int i = 0; i < (int)m_msg_list.size(); i++)
		m_msg_list[i].set_last_moji_disp_time(m_time);
}

// �f�o�b�O���b�Z�[�W���擾
TSTR C_elm_mwnd::get_debug_msg()
{
	TSTR msg;
	for (int i = 0; i < (int)m_msg_list.size(); i++)
		msg += m_msg_list[i].get_debug_msg();

	msg += m_sel.get_msg();

	return msg;
}

// ****************************************************************
// �}���`���b�Z�[�W
// ================================================================

// ���̃��b�Z�[�W��
void C_elm_mwnd::next_msg()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &Gp_ini->mwnd.sub[m_mwnd_no];

	// �^�[�Q�b�g���ړ�����
	m_target_msg_no ++;

	// ���b�Z�[�W�������m�ۂ���
	if (m_target_msg_no >= (int)m_msg_list.size())	{
		m_msg_list.resize(m_target_msg_no + 1);
		m_msg_list[m_target_msg_no].init(m_def_elm_group_list, this);
		m_msg_list[m_target_msg_no].set_def_param(p_ini_sub->moji_cnt
			, p_ini_sub->moji_size, p_ini_sub->moji_space, p_ini->moji_color, p_ini->shadow_color, p_ini->fuchi_color, p_ini_sub->ruby_size, p_ini_sub->ruby_space
			, p_ini_sub->name_moji_size, p_ini_sub->name_newline, p_ini_sub->name_bracket, p_ini_sub->talk_margin);
	}

	// �ʒu��������������
	C_point msg_pos = m_msg_list[m_target_msg_no - 1].get_cur_moji_pos();
	m_msg_list[m_target_msg_no].set_cur_moji_pos(msg_pos);
}

// ****************************************************************
// ���b�Z�[�W���x�v�Z�֘A
// ================================================================
void C_elm_mwnd::set_progress_start_flag(bool value)
{
	for (int i = 0; i < (int)m_msg_list.size(); i++)
		m_msg_list[i].set_progress_start_flag(value);
}

// ****************************************************************
// �I��������
// ================================================================

// �I�������N���A
void C_elm_mwnd::clear_sel()
{
	m_sel.clear_sel();
}

// �I����������
void C_elm_mwnd::ready_sel()
{
	// ���݈ʒu���擪�łȂ��ꍇ�͉��s����
	m_msg_list[0].new_line_if_not_line_head();

	// ���݂̕����ʒu���ŏ��̑I������u���ʒu�ɂȂ�
	m_sel.set_def_pos(m_msg_list[0].get_cur_moji_pos());
}

// �I������ǉ�
void C_elm_mwnd::add_sel(CTSTR& sel, int item_type, int color)
{
	// �I�����̌��|�P�A���s������K�v������
	if (m_sel.is_empty())	{
		m_sel.add_sel(sel, item_type, color);
	}
	else	{
		m_sel.add_sel(sel, item_type, color);
		m_msg_list[m_target_msg_no].new_line_no_indent();
	}
}

// �L�����Z���\�t���O��ݒ�
void C_elm_mwnd::set_sel_cancel_enable(bool enable)
{
	m_sel.set_cancel_enable(enable);
}

// �I�����𐮂���
void C_elm_mwnd::restruct_sel()
{
	// �I�����𐮂���
	m_sel.restruct_item(m_param.extend_type);

	// �σE�B���h�E�̏ꍇ�͋�`���Đ�������I�I
	if (m_param.extend_type == 1)	{

		// ���b�Z�[�W�T�C�Y���擾
		C_size msg_size = m_sel.get_msg_size();

		// �E�B���h�E�T�C�Y�����߂�
		m_param.window_size.cx = msg_size.cx + m_param.msg_margin.left + m_param.msg_margin.right;
		m_param.window_size.cy = msg_size.cy + m_param.msg_margin.top + m_param.msg_margin.bottom;

		// �E�B���h�E��`�����߂�
		C_rect window_rect = C_rect::by_size(m_param.window_pos, m_param.window_size);

		// �E�B���h�E�g���X�V
		m_msg_waku.set_extend_waku(&window_rect);

		// �E�B���h�E�T�C�Y���X�V����
		m_param.window_size = window_rect.size();
	}
}

// ****************************************************************
// �g�̏���
// ================================================================

// ���b�Z�[�W�g�̍č\�z
bool C_elm_mwnd::restruct_msg_waku()
{
	if (false);
	else if (m_param.extend_type == 0)	{

		// �E�B���h�E��`�����߂�
		C_rect window_rect = C_rect::by_size(m_param.window_pos, m_param.window_size);

		// ���b�Z�[�W�g���쐬�i�E�B���h�E�T�C�Y���ς�邱�Ƃ�����܂��j
		if (!m_msg_waku.set_extend_waku(&window_rect))
			return false;

		// �E�B���h�E�T�C�Y���X�V����
		m_param.window_size = window_rect.size();
	}
	else if (m_param.extend_type == 1)	{

		// �E�B���h�E�T�C�Y��ݒ�
		m_param.window_size.cx = m_msg_list[0].get_msg_size().cx + m_param.msg_margin.left + m_param.msg_margin.right;
		m_param.window_size.cy = m_msg_list[0].get_msg_size().cy + m_param.msg_margin.top + m_param.msg_margin.bottom;

		// �E�B���h�E��`�����߂�
		C_rect window_rect = C_rect::by_size(m_param.window_pos, m_param.window_size);

		// ���b�Z�[�W�g���쐬�i�E�B���h�E�T�C�Y���ς�邱�Ƃ�����܂��j
		if (!m_msg_waku.set_extend_waku(&window_rect))
			return false;

		// �E�B���h�E�T�C�Y���X�V����
		m_param.window_size = window_rect.size();
	}

	return true;
}

// ���O�g�̍č\�z
bool C_elm_mwnd::restruct_name_waku()
{
	if (false);
	else if (m_param.name_extend_type == 0)	{

		// ���O�E�B���h�E��`�����߂�
		m_param.name_window_rect = C_rect(C_point(0, 0), m_param.name_window_size);
		
		// ���O�g���쐬�i�E�B���h�E�T�C�Y���ς�邱�Ƃ�����܂��j
		if (!m_name_waku.set_extend_waku(&m_param.name_window_rect))
			return false;

		// �����ʒu�ɂ��낦��
		if (false);
		else if (m_param.name_window_align == 0)	{	// ����
			// NOP
		}
		else if (m_param.name_window_align == 1)	{	// ������
			m_param.name_window_rect -= C_point(m_param.name_window_rect.width() / 2, 0);
		}
		else if (m_param.name_window_align == 2)	{	// �E��
			m_param.name_window_rect -= C_point(m_param.name_window_rect.width(), 0);
		}
	}
	else if (m_param.name_extend_type == 1)	{

		// ���O��`���擾
		C_rect name_msg_rect = m_name.get_msg_rect();

		// ���O�g��`���v�Z
		m_param.name_window_rect.left = name_msg_rect.left - m_param.msg_margin.left;
		m_param.name_window_rect.top = name_msg_rect.top - m_param.msg_margin.top;
		m_param.name_window_rect.right = name_msg_rect.right + m_param.msg_margin.right;
		m_param.name_window_rect.bottom = name_msg_rect.bottom + m_param.msg_margin.bottom;

		// ���O�g���쐬�i�E�B���h�E�T�C�Y���ς�邱�Ƃ�����܂��j
		if (!m_name_waku.set_extend_waku(&m_param.name_window_rect))
			return false;
	}

	return true;
}

// ****************************************************************
// �L�[�҂��A�C�R���̕\���ݒ�
// ================================================================
void C_elm_mwnd::set_key_icon_appear(bool appear)
{
	// �\���t���O
	m_msg_waku.set_key_icon_appear(appear);

	// �A�C�R���̈ʒu�𒲂ׂ�
	C_point icon_pos(0, 0);
	if (!m_msg_list.empty())
	{
		icon_pos = m_msg_list.back().get_cur_moji_pos();

		// �����ʒu�ƃA�C�R���ʒu�͌������Ⴄ�悤�ł�
		if (Gp_ini->tateyoko_mode == 1)
		{
			icon_pos.x *= -1;
		}
	}

	// �A�C�R���̈ʒu��ݒ�
	if (Gp_ini->tateyoko_mode == 1)
	{
		m_msg_waku.set_key_icon_pos(m_param.msg_pos + icon_pos);
	}
	else
	{
		m_msg_waku.set_key_icon_pos(m_param.msg_pos + icon_pos);
	}
}
void C_elm_mwnd::set_key_icon_mode(int mode)
{
	// ���[�h
	m_msg_waku.set_key_icon_mode(mode);
}

// ****************************************************************
// �N���A
// ================================================================
void C_elm_mwnd::clear()
{
	// �N���A
	clear_face();		// ��O�����N���A
	clear_koe();		// �����N���A
	clear_name();		// ���O���N���A
	clear_msg();		// ���b�Z�[�W���N���A
	clear_sel();		// �I�������N���A

	// �N���A�����t���O���~�낷
	m_clear_ready_flag = false;
}

// ****************************************************************
// �m�x���ɑ΂��ă��b�Z�[�W�P�ʂ̃N���A����
// ================================================================
void C_elm_mwnd::clear_for_novel_one_msg()
{
	// �L�����N�^�����F���N���A
	clear_chara_moji_color();
	clear_chara_shadow_color();
	clear_chara_fuchi_color();

	// ���ԍ����N���A
	clear_koe();

	// ���b�Z�[�W�J�n�����t���O���~�낷
	for (int i = 0; i < (int)m_msg_list.size(); i++)	{
		m_msg_list[i].set_progress_start_flag(false);
	}

	// ���b�Z�[�W�^�C�v�m��t���O���~�낷
	for (int i = 0; i < (int)m_msg_list.size(); i++)	{
		m_msg_list[i].set_cur_msg_type_decided(false);
	}
}

// ****************************************************************
// ���b�Z�[�W���󂩂ǂ����𒲂ׂ�
// ================================================================
bool C_elm_mwnd::is_empty()
{
	for (int i = 0; i < (int)m_msg_list.size(); i++)	{
		if (!m_msg_list[i].is_empty())
			return false;
	}

	if (!m_name.is_empty())
		return false;

	if (!m_sel.is_empty())
		return false;

	return true;
}

// ****************************************************************
// �܂��o�����Ă��Ȃ����������݂��邩���肷��
// ================================================================
bool C_elm_mwnd::check_not_appear_moji()
{
	for (int i = 0; i < (int)m_msg_list.size(); i++)	{
		if (m_msg_list[i].check_not_appear_moji())
			return true;
	}

	if (m_sel.check_not_appear_moji())
		return true;

	return false;
}

// ****************************************************************
// ���ǃt���O��ǉ�����
// ================================================================
void C_elm_mwnd::add_read_flag(int scn_no, int flag_no)
{
	S_tnm_read_flag read_flag;
	read_flag.scn_no = scn_no;
	read_flag.flag_no = flag_no;
	m_read_flag_stock_list.push_back(read_flag);
}

// ****************************************************************
// ���ǃt���O���N���A����
// ================================================================
void C_elm_mwnd::clear_read_flag()
{
	m_read_flag_stock_list.clear();
}

// ****************************************************************
// �o�^�������ǃt���O��ݒ肷��
// ================================================================
void C_elm_mwnd::set_read_flag()
{
	for (int i = 0; i < (int)m_read_flag_stock_list.size(); i++)	{
		int scn_no = m_read_flag_stock_list[i].scn_no;		// ���ǃt���O�̃V�[���ԍ�
		int flag_no = m_read_flag_stock_list[i].flag_no;	// ���ǃt���O�̃t���O�ԍ�
		if (scn_no >= 0 && flag_no >= 0)
			(*Gp_read_flag)[scn_no][flag_no] = 1;				// ���ǁI
	}

	// ���ǃt���O�̓o�^���N���A
	m_read_flag_stock_list.clear();
}

// ****************************************************************
// �A�j�����I��点��
// ================================================================
void C_elm_mwnd::end_open_anime()
{
	m_open_anime_type = -1;
}

void C_elm_mwnd::end_close_anime()
{
	m_close_anime_type = -1;
}

void C_elm_mwnd::end_slide_anime()
{
	m_slide_anime_type = -1;
}

// ****************************************************************
// �A�j�����𔻒�
// ================================================================
bool C_elm_mwnd::is_open_anime_doing()
{
	return m_open_anime_type >= 0;
}

bool C_elm_mwnd::is_close_anime_doing()
{
	return m_close_anime_type >= 0;
}

bool C_elm_mwnd::is_slide_anime_doing()
{
	return m_slide_anime_type >= 0;
}

// ****************************************************************
// ������X���C�h
// ================================================================
void C_elm_mwnd::slide_msg()
{
	if (m_target_msg_no < 0 || (int)m_msg_list.size() <= m_target_msg_no)
		return;

	m_msg_list[m_target_msg_no].slide_msg();

	// �X���C�h�A�j���̏���ݒ�
	m_slide_pixel = m_msg_list[m_target_msg_no].get_slide_pixel();
	m_slide_anime_type = 0;
	m_slide_anime_time = m_slide_time;
	m_slide_anime_start_time = m_time;
}

// ****************************************************************
// �p�����[�^��������
// ================================================================
void C_elm_mwnd::init_window_pos()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	m_param.window_pos = p_ini_sub->window_pos;
}
void C_elm_mwnd::init_window_size()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	m_param.window_size = p_ini_sub->window_size;
}
void C_elm_mwnd::init_open_anime_type()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	m_param.open_anime_type = p_ini_sub->open_anime_type;
}
void C_elm_mwnd::init_open_anime_time()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	m_param.open_anime_time = p_ini_sub->open_anime_time;
}
void C_elm_mwnd::init_close_anime_type()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	m_param.close_anime_type = p_ini_sub->close_anime_type;
}
void C_elm_mwnd::init_close_anime_time()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	m_param.close_anime_time = p_ini_sub->close_anime_time;
}

// ****************************************************************
// �f�t�H���g�p�����[�^���擾
// ================================================================
int C_elm_mwnd::get_default_open_anime_type()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	return p_ini_sub->open_anime_type;
}
int C_elm_mwnd::get_default_open_anime_time()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	return p_ini_sub->open_anime_time;
}
int C_elm_mwnd::get_default_close_anime_type()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	return p_ini_sub->close_anime_type;
}
int C_elm_mwnd::get_default_close_anime_time()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	return p_ini_sub->close_anime_time;
}

// ****************************************************************
// ���b�Z�[�W�p�����[�^
// ================================================================
void C_elm_mwnd::init_window_moji_cnt()
{
	C_tnm_ini::S_mwnd* p_ini = &Gp_ini->mwnd;
	C_tnm_ini::S_mwnd::S_mwnd_sub* p_ini_sub = &p_ini->sub[m_mwnd_no];

	m_param.msg_moji_cnt = p_ini_sub->moji_cnt;

	// ���b�Z�[�W�ɔ��f
	int msg_cnt = (int)m_msg_list.size();
	for (int i = 0; i < msg_cnt; i++)	{
		m_msg_list[i].init_moji_cnt();
	}
}
void C_elm_mwnd::set_window_moji_cnt(C_size moji_cnt)
{
	m_param.msg_moji_cnt = moji_cnt;

	// ���b�Z�[�W�ɔ��f
	int msg_cnt = (int)m_msg_list.size();
	for (int i = 0; i < msg_cnt; i++)	{
		m_msg_list[i].set_moji_cnt(moji_cnt);
	}
}
C_size C_elm_mwnd::get_window_moji_cnt()
{
	return m_param.msg_moji_cnt;
}


// ****************************************************************
// ���b�Z�[�W�E�B���h�E���X�g�F������
// ================================================================
void C_elm_mwnd_list::init(S_element element, CTSTR& name, int size, bool extend_enable, C_tnm_element* p_parent, S_element elm_group_list)
{
	// �Œ�p�����[�^
	m_elm_group_list = elm_group_list;

	// ���N���X�̏�����
	C_elm_list_ex<C_elm_mwnd>::init(element, FM_MWNDLIST, name, size, extend_enable, p_parent);
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E���X�g�F�I��
// ================================================================
void C_elm_mwnd_list::finish()
{
	int mwnd_cnt = (int)m_sub.size();
	for (int i = 0; i < mwnd_cnt; i++)
		m_sub[i].finish();
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E���X�g�F�T�u�̏�����
// ================================================================
void C_elm_mwnd_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(i, get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"), this, tnm_set_element_down(m_elm_group_list));
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E���X�g�F�T�u�̍ď�����
// ================================================================
void C_elm_mwnd_list::_reinit(int begin, int end, bool restruct_flag)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit(restruct_flag);
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E���X�g�F�T�u�̃Z�[�u
// ================================================================
void C_elm_mwnd_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// ���b�Z�[�W�E�B���h�E���X�g�F�T�u�̃��[�h
// ================================================================
void C_elm_mwnd_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}


