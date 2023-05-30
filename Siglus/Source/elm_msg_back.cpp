#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_input.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"data/tnm_local_save.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_timer.h"
#include	"data/tnm_tool.h"
#include	"data/tnm_color_table.h"
#include	"element/elm_msg_back.h"
#include	"element/elm_mwnd_moji.h"
#include	"engine/eng_save.h"
#include	"engine/eng_input.h"
#include	"engine/eng_config.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_syscom.h"
#include	"engine/ifc_sound.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// ������
// ================================================================
void C_elm_msg_back::init()
{
	// �f�t�H���g�̃p�����[�^��ݒ肷��
	m_def.window_rect = C_rect::by_size(Gp_ini->msg_back.window_pos, Gp_ini->msg_back.window_size);
	m_def.disp_margin = Gp_ini->msg_back.disp_margin;
	m_def.msg_pos = Gp_ini->msg_back.msg_pos;
	m_def.koe_btn_pos = Gp_ini->msg_back.koe_btn_pos;
	m_def.load_btn_pos = Gp_ini->msg_back.load_btn_pos;
	m_history_cnt_max = Gp_ini->msg_back.history_cnt;

	// �q�X�g���[���\�z����
	m_history_list.clear();
	m_history_list.resize(m_history_cnt_max);
	for (int i = 0; i < m_history_cnt_max; i++)	{
		m_history_list[i].msg.init(S_element(), NULL);
		m_history_list[i].msg.set_def_param(Gp_ini->msg_back.moji_cnt
			, Gp_ini->msg_back.moji_size, Gp_ini->msg_back.moji_space, Gp_ini->mwnd.moji_color, Gp_ini->mwnd.shadow_color, Gp_ini->mwnd.fuchi_color, 10, 10
			, Gp_ini->msg_back.moji_size, 0, 0, C_rect(0, 0, 0, 0));
	}

	// �Z�p���[�^
	m_separator_list.clear();

	// �X�v���C�g���N���A
	m_spr_waku.init();
	m_spr_filter.init();

	// �A���o�����N���A
	m_album_waku.reset();
	m_album_filter.reset();
	m_album_separator.reset();
	m_album_separator_top.reset();
	m_album_separator_bottom.reset();

	// �����ϐ�������������
	init_work_variable();
}

// ****************************************************************
// �Z�[�u
// ================================================================
void C_elm_msg_back::save(C_tnm_save_stream& stream)
{
	stream.save(m_history_cnt);
	for (int i = 0; i < m_history_cnt; i++)
	{
		stream.save(m_history_list[i].pct_flag);
		stream.save(m_history_list[i].msg_str);
		stream.save(m_history_list[i].original_name);
		stream.save(m_history_list[i].disp_name);
		stream.save(m_history_list[i].pct_pos);
		stream.save(m_history_list[i].koe_no_list);
		stream.save(m_history_list[i].chr_no_list);
		stream.save(m_history_list[i].koe_play_no);
		stream.save(m_history_list[i].debug_msg);
		stream.save(m_history_list[i].scn_no);
		stream.save(m_history_list[i].line_no);
		stream.save(m_history_list[i].save_id);
		stream.save(m_history_list[i].save_id_check_flag);
	}
	stream.save(m_history_start_pos);
	stream.save(m_history_last_pos);
	stream.save(m_history_insert_pos);
	stream.save(m_new_msg_flag);
}

// ****************************************************************
// ���[�h
// ================================================================
void C_elm_msg_back::load(C_tnm_save_stream& stream)
{
	stream.load(m_history_cnt);
	for (int i = 0; i < m_history_cnt; i++)
	{
		stream.load(m_history_list[i].pct_flag);
		stream.load(m_history_list[i].msg_str);
		stream.load(m_history_list[i].original_name);
		stream.load(m_history_list[i].disp_name);
		stream.load(m_history_list[i].pct_pos);
		stream.load(m_history_list[i].koe_no_list);
		stream.load(m_history_list[i].chr_no_list);
		stream.load(m_history_list[i].koe_play_no);
		stream.load(m_history_list[i].debug_msg);
		stream.load(m_history_list[i].scn_no);
		stream.load(m_history_list[i].line_no);
		stream.load(m_history_list[i].save_id);
		stream.load(m_history_list[i].save_id_check_flag);
	}
	stream.load(m_history_start_pos);
	stream.load(m_history_last_pos);
	stream.load(m_history_insert_pos);
	stream.load(m_new_msg_flag);
}

// ****************************************************************
// ��������
// ================================================================
void C_elm_msg_back::remove()
{
	// �}�b�v���傫���Ȃ肷�����ꍇ�̂ݏ���
	if ((int)Gp_backlog_save->map.size() > Gp_ini->backlog_save_cnt)
	{
		// �V�����}�b�v
		std::map<S_tid, BSP<S_tnm_local_save>> new_map;

		// ���b�Z�[�W�o�b�N���t����`�F�b�N���Ă���
		for (int i = m_history_cnt - 1; i >= 0; i--)
		{
			int history_pos = (m_history_start_pos + i) % m_history_cnt_max;
			S_tid save_id = m_history_list[history_pos].save_id;

			if (save_id != S_tid())
			{
				// �Z�[�u�f�[�^�����ۂɑ��݂��邩���m�F
				std::map<S_tid, BSP<S_tnm_local_save>>::iterator save_find = Gp_backlog_save->map.find(save_id);

				// �Z�[�u�f�[�^�����݂����
				if (save_find != Gp_backlog_save->map.end())
				{
					// �V�����}�b�v�ɒǉ�
					new_map.insert(std::pair<S_tid, BSP<S_tnm_local_save>>(save_id, save_find->second));

					// �T�C�Y�����E�܂ŒB������I��
					if ((int)new_map.size() >= Gp_ini->backlog_save_cnt)
					{
						break;
					}

				}
			}
		}

		// �}�b�v������
		Gp_backlog_save->map = new_map;
	}
}

// ****************************************************************
// �����ϐ�������
// ================================================================
void C_elm_msg_back::init_work_variable()
{
	m_cur = m_def;	// �f�t�H���g�p�����[�^�����݂̃p�����[�^�փR�s�[����
	m_window_appear = false;

	m_history_cnt = 0;
	m_history_start_pos = 0;
	m_history_last_pos = 0;
	m_history_insert_pos = 0;
	m_new_msg_flag = true;

	m_msg_total_height = 0;
	m_target_no = -1;
	m_mouse_target_no = -1;

	m_scroll_pos = 0;
	m_slider_pos = Gp_ini->msg_back.slider_rect.top;
	m_slider_operating = false;
	m_slider_start_slider_pos = 0;
	m_slider_start_mouse_pos = 0;

	m_drag_ready_flag = false;
	m_drag_flag = false;
	m_drag_start_mouse_pos = C_point(0, 0);
	m_drag_start_scroll_pos = 0;

	m_drag_aftereffects_flag = false;
}

// ****************************************************************
// �t���[������������
// ================================================================
void C_elm_msg_back::frame_init()
{
	// �E�B���h�E����\���Ȃ�߂�
	if (!m_window_appear)
		return;
	
	// �X�v���C�g��������
	m_spr_waku.rp.init();
	m_spr_filter.rp.init();

	// �Z�p���[�^
	for (int i = 0; i < (int)m_separator_list.size(); i++)
		m_separator_list[i].sprite.rp.init();

	// �e�L�X�g
	for (int i = 0; i < m_history_cnt_max; i++)
		m_history_list[i].msg.frame_init();

	// ���{�^��
	for (int i = 0; i < m_history_cnt_max; i++)
		m_history_list[i].koe_btn.frame_init();

	// ���[�h�{�^��
	for (int i = 0; i < m_history_cnt_max; i++)
		m_history_list[i].load_btn.frame_init();

	// �X���C�_�[
	m_slider.frame_init();

	// �{�^��
	m_close_btn.frame_init();
	m_msg_up_btn.frame_init();
	m_msg_down_btn.frame_init();
	m_ex_btn_1.frame_init();
	m_ex_btn_2.frame_init();
	m_ex_btn_3.frame_init();
	m_ex_btn_4.frame_init();
}

// ****************************************************************
// �t���[������
// ================================================================
void C_elm_msg_back::frame(int time, bool draw_futoku, bool draw_shadow)
{
	// �E�B���h�E����\���Ȃ�߂�
	if (!m_window_appear)
		return;

	// �t�@�C�����g���ꍇ
	if (m_spr_waku.get_texture())
	{
		// �g�X�v���C�g�̃p�����[�^
		m_spr_waku.rpex.sorter = S_tnm_sorter(Gp_ini->msg_back_order, Gp_ini->mwnd.waku_layer_rep);
		m_spr_waku.rp.disp = true;
		m_spr_waku.rp.d2_rect = true;
		m_spr_waku.rp.alpha_test = true;
		m_spr_waku.rp.alpha_blend = true;
		m_spr_waku.rp.pos = tnm_d2to3(Gp_ini->msg_back.window_pos);
		m_spr_waku.rp.size_fit_to_texture = 0;
	}

	// �t�B���^�̃X�v���C�g�p�����[�^
	m_spr_filter.rpex.sorter = S_tnm_sorter(Gp_ini->msg_back_order, Gp_ini->mwnd.filter_layer_rep);
	m_spr_filter.rp.disp = true;
	m_spr_filter.rp.d2_rect = true;
	m_spr_filter.rp.alpha_test = true;
	m_spr_filter.rp.alpha_blend = true;
	m_spr_filter.rp.pos = tnm_d2to3(Gp_ini->msg_back.window_pos);
	m_spr_filter.rp.tr = Gp_config->filter_color.a;				// �����x
	m_spr_filter.rp.color_add_b = Gp_config->filter_color.b;	// �F���Z
	m_spr_filter.rp.color_add_g = Gp_config->filter_color.g;	// �F���Z
	m_spr_filter.rp.color_add_r = Gp_config->filter_color.r;	// �F���Z

	// �t�@�C�����g���ꍇ
	if (m_spr_filter.get_texture())
	{
		m_spr_filter.rp.size_fit_to_texture = 0;
	}
	// �t�@�C�����g��Ȃ��ꍇ
	else
	{
		C_point pos = Gp_ini->msg_back.window_pos + Gp_ini->msg_back.filter_margin.left_top();
		m_spr_filter.rp.pos = tnm_d2to3(pos);
		m_spr_filter.rp.size.x = (float)Gp_ini->msg_back.window_size.cx - Gp_ini->msg_back.filter_margin.left - Gp_ini->msg_back.filter_margin.right;
		m_spr_filter.rp.size.y = (float)Gp_ini->msg_back.window_size.cy - Gp_ini->msg_back.filter_margin.top - Gp_ini->msg_back.filter_margin.bottom;
		m_spr_filter.rp.filter.color = Gp_ini->msg_back.filter_color;	// �t�B���^�̐F
	}

	// �Z�p���[�^
	for (int i = 0; i < (int)m_separator_list.size(); i++)
	{
		m_separator_list[i].sprite.rpex.sorter = S_tnm_sorter(Gp_ini->msg_back_order, Gp_ini->mwnd.waku_layer_rep);
		m_separator_list[i].sprite.rp.disp = true;
		m_separator_list[i].sprite.rp.d2_rect = true;
		m_separator_list[i].sprite.rp.alpha_test = true;
		m_separator_list[i].sprite.rp.alpha_blend = true;
		m_separator_list[i].sprite.rp.pos = tnm_d2to3(C_point(m_cur.window_rect.left, m_cur.window_rect.top + m_separator_list[i].total_pos + m_scroll_pos));
		m_separator_list[i].sprite.rp.size_fit_to_texture = 0;

		C_rect clip = C_rect(C_point(m_cur.window_rect.left_top() + m_cur.disp_margin.left_top()), C_point(m_cur.window_rect.right_bottom() - m_cur.disp_margin.right_bottom()));
		m_separator_list[i].sprite.rp.dst_clip_use = true;
		m_separator_list[i].sprite.rp.dst_clip = clip;
	}

	// ���b�Z�[�W�̃t���[������
	if (m_history_cnt > 0)
	{
		// �e���b�Z�[�W�Ɋւ��Ĕ���
		for (int i = 0; i < m_history_cnt; i++)
		{
			int history_pos = (m_history_start_pos + i) % m_history_cnt_max;

			int pos_y = m_history_list[history_pos].total_pos;
			C_rect clip = C_rect(C_point(m_cur.window_rect.left_top() + m_cur.disp_margin.left_top()), C_point(m_cur.window_rect.right_bottom() - m_cur.disp_margin.right_bottom()));

			// ���b�Z�[�W���e�L�X�g��`���ɂ��邩�H
			bool is_in_rect = false;
			if (Gp_ini->tateyoko_mode == 1)
			{
				if (
					pos_y - m_scroll_pos < m_cur.window_rect.width() - m_cur.disp_margin.right &&			// ���b�Z�[�W�̍��[���e�L�X�g��`�ɓ����Ă���H
					pos_y - m_scroll_pos + m_history_list[history_pos].height > m_cur.disp_margin.left		// ���b�Z�[�W�̉E�[���e�L�X�g��`�ɓ����Ă���H
					)
				{
					is_in_rect = true;
				}
			}
			else
			{
				if (
					pos_y + m_scroll_pos + m_history_list[history_pos].height > m_cur.disp_margin.top &&	// ���b�Z�[�W�̉��[���e�L�X�g��`�ɓ����Ă���H
					pos_y + m_scroll_pos < m_cur.window_rect.height() - m_cur.disp_margin.bottom			// ���b�Z�[�W�̏�[���e�L�X�g��`�ɓ����Ă���H
					)
				{
					is_in_rect = true;
				}
			}
			if (is_in_rect)
			{
				// �^�[�Q�b�g�̏ꍇ�͐F��ς���
				C_argb color;
				C_argb shadow_color;
				C_argb fuchi_color;
				int color_rate = 0;
				if (Gp_global->debug_flag && history_pos == m_target_no)	{
					color_rate = 255;	// �f�o�b�O���̓^�[�Q�b�g�𐅐F�ɂ���
					color = Gp_color_table->get_color(Gp_ini->msg_back.debug_moji_color);
					shadow_color = Gp_color_table->get_color(Gp_ini->msg_back.debug_moji_shadow_color);
					fuchi_color = Gp_color_table->get_color(Gp_ini->msg_back.debug_moji_fuchi_color);
				}
				if (history_pos == m_mouse_target_no)	{
					color_rate = 255;	// �}�E�X�^�[�Q�b�g�����F�ɂ���
					color = Gp_color_table->get_color(Gp_ini->msg_back.active_moji_color);
					shadow_color = Gp_color_table->get_color(Gp_ini->msg_back.active_moji_shadow_color);
					fuchi_color = Gp_color_table->get_color(Gp_ini->msg_back.active_moji_fuchi_color);
				}

				// ���b�Z�[�W�Ɉ����p�������_�[�p�����[�^
				S_tnm_render_param msg_tdp;
				msg_tdp.disp = true;
				msg_tdp.sorter = S_tnm_sorter(Gp_ini->msg_back_order, 0);
				if (Gp_ini->tateyoko_mode == 1)
				{
					msg_tdp.pos_x = (float)m_cur.window_rect.right - pos_y + m_scroll_pos;
					msg_tdp.pos_y = (float)m_cur.window_rect.top + m_cur.msg_pos;
				}
				else
				{
					msg_tdp.pos_x = (float)m_cur.window_rect.left + m_cur.msg_pos;
					msg_tdp.pos_y = (float)m_cur.window_rect.top + pos_y + m_scroll_pos;
				}
				msg_tdp.dst_clip_use = true;
				msg_tdp.dst_clip_left = clip.left;
				msg_tdp.dst_clip_top = clip.top;
				msg_tdp.dst_clip_right = clip.right;
				msg_tdp.dst_clip_bottom = clip.bottom;

				// ���b�Z�[�W�̃t���[������
				m_history_list[history_pos].msg.frame(&msg_tdp, C_point(0, 0), color_rate, color, shadow_color, fuchi_color, draw_futoku, draw_shadow);
			}
			else
			{
				// ��`���ɂȂ��ꍇ�̓e�N�X�`��������i�O���t�B�b�N�������̐ߖ�̂��߁j
				m_history_list[history_pos].msg.clear_texture();
			}

			// �s�N�`���i���{�^�����g���܂킵�Ă܂��c�H�j
			if (m_history_list[history_pos].pct_flag)
			{
				C_point pos = m_cur.window_rect.left_top() + C_point(m_cur.msg_pos, pos_y + m_scroll_pos);
				m_history_list[history_pos].koe_btn.set_disp(1);
				m_history_list[history_pos].koe_btn.set_order(Gp_ini->msg_back_order);
				m_history_list[history_pos].koe_btn.set_pos_x(pos.x + m_history_list[history_pos].pct_pos.x);
				m_history_list[history_pos].koe_btn.set_pos_y(pos.y + m_history_list[history_pos].pct_pos.y);
				m_history_list[history_pos].koe_btn.set_clip_use(1);
				m_history_list[history_pos].koe_btn.set_clip_left(clip.left);
				m_history_list[history_pos].koe_btn.set_clip_top(clip.top);
				m_history_list[history_pos].koe_btn.set_clip_right(clip.right);
				m_history_list[history_pos].koe_btn.set_clip_bottom(clip.bottom);
				m_history_list[history_pos].koe_btn.frame(NULL, 0, 0);
			}

			// ���A�C�R��
			else if (!m_history_list[history_pos].koe_no_list.empty())
			{
				C_point pos;
				if (Gp_ini->tateyoko_mode == 1)
				{
					pos.x = m_cur.window_rect.right - m_history_list[history_pos].total_pos + m_scroll_pos;
					pos.y = m_cur.window_rect.top + m_cur.msg_pos;
				}
				else
				{
					pos.x = m_cur.window_rect.left + m_cur.msg_pos;
					pos.y = m_cur.window_rect.top + m_history_list[history_pos].total_pos + m_scroll_pos;
				}

				m_history_list[history_pos].koe_btn.set_disp(1);
				m_history_list[history_pos].koe_btn.set_order(Gp_ini->msg_back_order);
				m_history_list[history_pos].koe_btn.set_pos_x(pos.x + m_def.koe_btn_pos.x);
				m_history_list[history_pos].koe_btn.set_pos_y(pos.y + m_def.koe_btn_pos.y);
				m_history_list[history_pos].koe_btn.set_clip_use(1);
				m_history_list[history_pos].koe_btn.set_clip_left(clip.left);
				m_history_list[history_pos].koe_btn.set_clip_top(clip.top);
				m_history_list[history_pos].koe_btn.set_clip_right(clip.right);
				m_history_list[history_pos].koe_btn.set_clip_bottom(clip.bottom);
				m_history_list[history_pos].koe_btn.frame(NULL, 0, 0);
			}

			// ���[�h�{�^��
			if (m_history_list[history_pos].save_id != S_tid())
			{
				if (tnm_check_backlog_save(m_history_list[history_pos].save_id))
				{
					C_point pos;
					if (Gp_ini->tateyoko_mode == 1)
					{
						pos.x = m_cur.window_rect.right - m_history_list[history_pos].total_pos + m_scroll_pos;
						pos.y = m_cur.window_rect.top + m_cur.msg_pos;
					}
					else
					{
						pos.x = m_cur.window_rect.left + m_cur.msg_pos;
						pos.y = m_cur.window_rect.top + m_history_list[history_pos].total_pos + m_scroll_pos;
					}

					m_history_list[history_pos].load_btn.set_disp(1);
					m_history_list[history_pos].load_btn.set_order(Gp_ini->msg_back_order);
					m_history_list[history_pos].load_btn.set_pos_x(pos.x + m_def.load_btn_pos.x);
					m_history_list[history_pos].load_btn.set_pos_y(pos.y + m_def.load_btn_pos.y);
					m_history_list[history_pos].load_btn.set_clip_use(1);
					m_history_list[history_pos].load_btn.set_clip_left(clip.left);
					m_history_list[history_pos].load_btn.set_clip_top(clip.top);
					m_history_list[history_pos].load_btn.set_clip_right(clip.right);
					m_history_list[history_pos].load_btn.set_clip_bottom(clip.bottom);
					m_history_list[history_pos].load_btn.frame(NULL, 0, 0);
				}
			}
		}
	}

	// �X���C�_�[
	if (Gp_ini->tateyoko_mode == 1)
	{
		m_slider.set_pos_x(m_cur.window_rect.left + m_slider_pos);
		m_slider.set_pos_y(m_cur.window_rect.top + Gp_ini->msg_back.slider_rect.top);
	}
	else
	{
		m_slider.set_pos_x(m_cur.window_rect.left + Gp_ini->msg_back.slider_rect.left);
		m_slider.set_pos_y(m_cur.window_rect.top + m_slider_pos);
	}
	m_slider.frame(NULL, 0, 0);

	// �{�^��
	m_close_btn.frame(NULL, 0, 0);
	m_msg_up_btn.frame(NULL, 0, 0);
	m_msg_down_btn.frame(NULL, 0, 0);
	m_ex_btn_1.frame(NULL, 0, 0);
	m_ex_btn_2.frame(NULL, 0, 0);
	m_ex_btn_3.frame(NULL, 0, 0);
	m_ex_btn_4.frame(NULL, 0, 0);
}

// ****************************************************************
// �{�^����o�^
// ================================================================
void C_elm_msg_back::regist_button(int min_order)
{
	// �E�B���h�E����\���Ȃ�߂�
	if (!m_window_appear)
		return;

	// ���b�Z�[�W�o�b�N���A�N�e�B�u�łȂ��Ȃ�߂�
	if (Gp_local->cur_proc.type != TNM_PROC_TYPE_MSG_BACK)
		return;

	// ���A�C�R��
	for (int i = 0; i < m_history_cnt_max; i++)
		m_history_list[i].koe_btn.regist_button(min_order);

	// ���[�h�A�C�R��
	for (int i = 0; i < m_history_cnt_max; i++)
		m_history_list[i].load_btn.regist_button(min_order);

	// �X���C�_�[
	m_slider.regist_button(min_order);

	// �{�^��
	m_close_btn.regist_button(min_order);
	m_msg_up_btn.regist_button(min_order);
	m_msg_down_btn.regist_button(min_order);
	m_ex_btn_1.regist_button(min_order);
	m_ex_btn_2.regist_button(min_order);
	m_ex_btn_3.regist_button(min_order);
	m_ex_btn_4.regist_button(min_order);
}

// ****************************************************************
// �{�^���C�x���g
// ================================================================

#define		TNM_MSG_BACK_AFTEREFFECTS_ALPHA		10

void C_elm_msg_back::msg_click_event()
{
	if (m_mouse_target_no >= 0)
	{
		S_history* p_hisotry = &m_history_list[m_mouse_target_no];

		if (Gp_ini->msg_back.msg_click_action == 1)
		{
			if (!p_hisotry->koe_no_list.empty())
			{
				// �Đ����鐺�ԍ���␳����
				if (p_hisotry->koe_play_no < 0 || (int)p_hisotry->koe_no_list.size() <= p_hisotry->koe_play_no)
					p_hisotry->koe_play_no = 0;

				// �����Đ��i���Z�Đ��L���j
				int koe_no = p_hisotry->koe_no_list[p_hisotry->koe_play_no];
				int chr_no = p_hisotry->chr_no_list[p_hisotry->koe_play_no];
				tnm_play_koe(koe_no, chr_no, Gp_config->jitan_msgbk_onoff ? Gp_config->jitan_speed : TNM_JITAN_RATE_NORMAL, 0, false, false, false, true, false);

				// ���̐��ɐi�߂�i�����̐�������ꍇ������܂��j
				p_hisotry->koe_play_no++;
			}
		}
		else if (Gp_ini->msg_back.msg_click_action == 2)
		{
			if (tnm_check_backlog_save(p_hisotry->save_id))
			{
				// ���[�h���s��
				load_call(p_hisotry->save_id);
			}
		}
	}
}

void C_elm_msg_back::button_event()
{
	// �E�B���h�E����\���Ȃ�߂�
	if (!m_window_appear)
		return;

	// ���b�Z�[�W�o�b�N���A�N�e�B�u�łȂ��Ȃ�߂�
	if (Gp_local->cur_proc.type != TNM_PROC_TYPE_MSG_BACK)
		return;

	// ���A�C�R��
	for (int i = 0; i < m_history_cnt_max; i++)
	{
		S_history* p_hisotry = &m_history_list[i];

		// �f�t�H���g�̃{�^���C�x���g
		p_hisotry->koe_btn.button_event();

		// ���A�C�R���̌��菈��
		if (p_hisotry->koe_btn.is_decided_this_frame())
		{
			if (!p_hisotry->koe_no_list.empty())
			{
				// �Đ����鐺�ԍ���␳����
				if (p_hisotry->koe_play_no < 0 || (int)p_hisotry->koe_no_list.size() <= p_hisotry->koe_play_no)
					p_hisotry->koe_play_no = 0;

				// �����Đ��i���Z�Đ��L���j
				int koe_no = p_hisotry->koe_no_list[p_hisotry->koe_play_no];
				int chr_no = p_hisotry->chr_no_list[p_hisotry->koe_play_no];
				tnm_play_koe(koe_no, chr_no, Gp_config->jitan_msgbk_onoff ? Gp_config->jitan_speed : TNM_JITAN_RATE_NORMAL, 0, false, false, false, true, false);

				// ���̐��ɐi�߂�i�����̐�������ꍇ������܂��j
				p_hisotry->koe_play_no ++;
			}
		}
	}

	// ���[�h�{�^��
	for (int i = 0; i < m_history_cnt_max; i++)
	{
		S_history* p_hisotry = &m_history_list[i];

		if (p_hisotry->save_id != S_tid())
		{
			// �f�t�H���g�̃{�^���C�x���g
			p_hisotry->load_btn.button_event();

			// ���[�h�{�^���̌��菈��
			if (p_hisotry->load_btn.is_decided_this_frame())
			{
				// ���[�h���s��
				load_call(p_hisotry->save_id);
			}
		}
	}

	// �X���C�_�[
	m_slider.button_event();

	// �X���C�_�[�̌��菈��
	if (m_slider.is_pushed_this_frame())
	{
		m_slider_operating = true;
		m_slider_start_slider_pos = m_slider_pos;
		if (Gp_ini->tateyoko_mode == 1)
		{
			m_slider_start_mouse_pos = Gp_cur_input->mouse.pos.x;
		}
		else
		{
			m_slider_start_mouse_pos = Gp_cur_input->mouse.pos.y;
		}
	}
	if (m_slider.is_decided_this_frame())
	{
		m_slider_operating = false;

		// �X�N���[������X���C�_�[�ʒu���X�V�i�X���C�_�[�����������Ɉ�x�����X���C�_�[�ʒu�𒲐�����F�o�b�N���O���P�����Ȃ������ꍇ�ɃX���C�_�[�̈ʒu�������ɔz�u����ׂ̕␳�����ł��j
		calc_slider_pos_from_scroll();
	}
	if (!m_slider.is_pushed())
	{
		m_slider_operating = false;
	}

	// �X���C�_�[�𓮂���������
	if (m_slider_operating && m_history_cnt > 0)
	{
		// �X���C�_�[�̈ʒu�����߂�
		if (Gp_ini->tateyoko_mode == 1)
		{
			m_slider_pos = m_slider_start_slider_pos + (Gp_cur_input->mouse.pos.x - m_slider_start_mouse_pos);
			m_slider_pos = ::limit((int)Gp_ini->msg_back.slider_rect.left, m_slider_pos, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_x(0));
		}
		else
		{
			m_slider_pos = m_slider_start_slider_pos + (Gp_cur_input->mouse.pos.y - m_slider_start_mouse_pos);
			m_slider_pos = ::limit((int)Gp_ini->msg_back.slider_rect.top, m_slider_pos, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_y(0));
		}

		// �X���C�_�[�ʒu���X�V����
		update_pos_from_slider();
	}

	// ����{�^��
	m_close_btn.button_event();

	// ����{�^���̌��菈��
	if (m_close_btn.is_decided_this_frame())
	{
		// ���b�Z�[�W�o�b�N�����
		tnm_syscom_close_msg_back();
	}

	// ���b�Z�[�W�A�b�v�{�^��
	m_msg_up_btn.button_event();

	// ���b�Z�[�W�A�b�v�{�^���̌��菈��
	if (m_page_up_key || m_msg_up_btn.is_pushed())
	{
		target_up();
	}
	else
	{
		m_target_up_start_time = 0;
	}
	m_page_up_key = false;

	// ���b�Z�[�W�_�E���{�^��
	m_msg_down_btn.button_event();

	// ���b�Z�[�W�_�E���{�^���̌��菈��
	if (m_page_dw_key || m_msg_down_btn.is_pushed())
	{
		target_down();
	}
	else
	{
		m_target_dw_start_time = 0;
	}
	m_page_dw_key = false;

	// �ėp�{�^���P
	m_ex_btn_1.button_event();

	// �ėp�{�^���P�̌��菈��
	if (m_ex_btn_1.is_decided_this_frame())
	{
		ex_btn_call(1);
	}

	// �ėp�{�^���Q
	m_ex_btn_2.button_event();

	// �ėp�{�^���Q�̌��菈��
	if (m_ex_btn_2.is_decided_this_frame())
	{
		ex_btn_call(2);
	}

	// �ėp�{�^���R
	m_ex_btn_3.button_event();

	// �ėp�{�^���R�̌��菈��
	if (m_ex_btn_3.is_decided_this_frame())
	{
		ex_btn_call(3);
	}

	// �ėp�{�^���S
	m_ex_btn_4.button_event();

	// �ėp�{�^���S�̌��菈��
	if (m_ex_btn_4.is_decided_this_frame())
	{
		ex_btn_call(4);
	}

	// �h���b�O����
	if (Gp_input->now.mouse.left.use_down_stock())
	{
		// �h���b�O�����J�n
		m_drag_ready_flag = true;
		m_drag_start_mouse_pos = Gp_cur_input->mouse.pos;
		m_drag_start_scroll_pos = m_scroll_pos;

		// �h���b�O�̗]�g
		m_drag_aftereffects_flag = false;
		m_drag_aftereffects_start_time = ::timeGetTime();
		if (Gp_ini->tateyoko_mode == 1)
		{
			m_drag_aftereffects_mouse_pos = Gp_cur_input->mouse.pos.x;
		}
		else
		{
			m_drag_aftereffects_mouse_pos = Gp_cur_input->mouse.pos.y;
		}
		m_drag_aftereffects_distance = 0;
	}

	// �����ł������΃h���b�O�J�n
	if (m_drag_ready_flag && m_drag_start_mouse_pos != Gp_cur_input->mouse.pos)
	{
		m_drag_flag = true;
	}

	// �h���b�O�I��
	if (Gp_input->now.mouse.left.use_up_stock())
	{
		// �h���b�O�����������Ăăh���b�O���Ă��Ȃ��ꍇ�A�����Đ�����
		if (m_drag_ready_flag && !m_drag_flag && m_mouse_target_no >= 0)
		{
			msg_click_event();
		}

		// �h���b�O�̗]�g
		if (m_drag_flag && m_drag_aftereffects_distance != 0)
		{
			m_drag_aftereffects_flag = true;						// �]�g���������܂����B
			int power = m_drag_aftereffects_distance * TNM_MSG_BACK_AFTEREFFECTS_ALPHA;			// * TNM_MSG_BACK_AFTEREFFECTS_ALPHA = �X�N���[�����鋗���Ɋւ��W��
			m_drag_aftereffects_start_scroll_pos = m_scroll_pos;
			m_drag_aftereffects_end_scroll_pos = m_scroll_pos + power;
			m_drag_aftereffects_start_time = ::timeGetTime();
		}

		// �h���b�O�I��
		m_drag_ready_flag = false;
		m_drag_flag = false;
	}

	// ����L�[
	if (tnm_input_use_key_down_up(VK_EX_KEYBOARD_DECIDE))
	{
		// �����Đ�����
		if (m_mouse_target_no >= 0)	{
			msg_click_event();
		}
	}

	// �h���b�O��
	if (m_drag_flag)
	{
		// �h���b�O�̗]�g
		DWORD now_time = ::timeGetTime();
		int keika = now_time - m_drag_aftereffects_start_time;
		if (keika >= 25)	{			// 25�_�b�P�ʂŗ]�g���v�����Ă��܂��B
			m_drag_aftereffects_start_time = now_time;
			if (Gp_ini->tateyoko_mode == 1)
			{
				m_drag_aftereffects_distance = Gp_cur_input->mouse.pos.x - m_drag_aftereffects_mouse_pos;
				m_drag_aftereffects_mouse_pos = Gp_cur_input->mouse.pos.x;
			}
			else
			{
				m_drag_aftereffects_distance = Gp_cur_input->mouse.pos.y - m_drag_aftereffects_mouse_pos;
				m_drag_aftereffects_mouse_pos = Gp_cur_input->mouse.pos.y;
			}
		}

		// �X�N���[��
		if (Gp_ini->tateyoko_mode == 1)
		{
			m_scroll_pos = m_drag_start_scroll_pos - (m_drag_start_mouse_pos.x - Gp_cur_input->mouse.pos.x);
		}
		else
		{
			m_scroll_pos = m_drag_start_scroll_pos - (m_drag_start_mouse_pos.y - Gp_cur_input->mouse.pos.y);
		}

		// �X�N���[������ʒu���X�V����
		update_pos_from_scroll();
	}

	// �h���b�O�̗]�g�̒��f�i�}�E�X��������܂����j
	if (Gp_input->now.mouse.left.is_down() || Gp_input->now.mouse.right.is_down() || Gp_input->now.keyboard.key[VK_PRIOR].is_down() || Gp_input->now.keyboard.key[VK_NEXT].is_down() || Gp_input->now.mouse.wheel.use_up_stock() || Gp_input->now.mouse.wheel.use_down_stock())
	{
		m_drag_aftereffects_flag = false;
	}

	// �h���b�O�̗]�g
	if (m_drag_aftereffects_flag)
	{
		DWORD now_time = ::timeGetTime();
		int keika = now_time - m_drag_aftereffects_start_time;

		// �X�N���[���i175 * TNM_MSG_BACK_AFTEREFFECTS_ALPHA �_�b�ŃX�N���[�����܂��j
		m_scroll_pos = speed_down_limit(keika, 0, m_drag_aftereffects_start_scroll_pos, 175 * TNM_MSG_BACK_AFTEREFFECTS_ALPHA, m_drag_aftereffects_end_scroll_pos);

		// �X�N���[������ʒu���X�V����
		update_pos_from_scroll();

		// �X�N���[���ʒu�܂œ��B�����̂ŏ��������I
		if (m_scroll_pos == m_drag_aftereffects_end_scroll_pos)	{
			m_drag_aftereffects_flag = false;
		}
	}

	// �}�E�X�^�[�Q�b�g
	m_mouse_target_no = -1;

	if (Gp_ini->msg_back.msg_click_action > 0)
	{
		if (!m_drag_flag)
		{
			for (int i = 0; i < m_history_cnt; i++)
			{
				// �^�[�Q�b�g�ɂȂ邩�ǂ����̔���
				bool is_target_enable = false;
				if (Gp_ini->msg_back.msg_click_action == 1)			// �����Đ�����ꍇ
				{
					// �����o�^����Ă���ꍇ�̂݃^�[�Q�b�g�ɂȂ�
					if (!m_history_list[i].koe_no_list.empty())
					{
						is_target_enable = true;
					}
				}
				else if (Gp_ini->msg_back.msg_click_action == 2)	// ���[�h���s���ꍇ
				{
					// ���Ȃ炸�^�[�Q�b�g�ɂȂ�
					is_target_enable = true;
				}

				// �^�[�Q�b�g�ɂȂ�ꍇ�͓����蔻��
				if (is_target_enable)
				{
					if (Gp_ini->tateyoko_mode == 1)
					{
						int right = m_cur.window_rect.right - m_history_list[i].total_pos + m_scroll_pos;
						int left = right - m_history_list[i].height;
						if (left <= Gp_cur_input->mouse.pos.x && Gp_cur_input->mouse.pos.x < right)
						{
							m_mouse_target_no = i;
							break;
						}
					}
					else
					{
						int top = m_cur.window_rect.top + m_history_list[i].total_pos + m_scroll_pos;
						int bottom = top + m_history_list[i].height;
						if (top <= Gp_cur_input->mouse.pos.y && Gp_cur_input->mouse.pos.y < bottom)
						{
							m_mouse_target_no = i;
							break;
						}
					}
				}
			}
		}
	}
}

// ****************************************************************
// ���[�h���s��
// ================================================================
void C_elm_msg_back::load_call(S_tid save_id)
{
	if (tnm_check_backlog_save(save_id))
	{
		if (!Gp_ini->msg_back.load_call_scene_name.empty())
		{
			int scene_no = Gp_lexer->get_scn_no(Gp_ini->msg_back.load_call_scene_name);
			int command_no = Gp_lexer->get_user_cmd_no(scene_no, Gp_ini->msg_back.load_call_command_name);
			if (scene_no < 0)
			{
				tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("�w�肳�ꂽ�V�[����������܂���B"));
			}
			else if (command_no < 0)
			{
				tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("�w�肳�ꂽ�R�}���h��������܂���B"));
			}
			else
			{
				Gp_global->msg_back_load_tid = save_id;		// �X�N���v�g����Ăяo����悤�� ID ���L�����Ă���

				// �w��̃R�}���h���R�[��
				tnm_scene_proc_call_user_cmd(scene_no, command_no, FM_VOID, true, false);
			}
		}
		else
		{
			tnm_syscom_msgbk_load(save_id, true, true, true);
		}
	}
}

// ****************************************************************
// �ėp�{�^��
// ================================================================
void C_elm_msg_back::ex_btn_call(int btn_no)
{
	TSTR scene_name;
	TSTR command_name;

	switch (btn_no)
	{
	case 1:
		scene_name = Gp_ini->msg_back.ex_btn_1_scene_name;
		command_name = Gp_ini->msg_back.ex_btn_1_command_name;
		break;
	case 2:
		scene_name = Gp_ini->msg_back.ex_btn_2_scene_name;
		command_name = Gp_ini->msg_back.ex_btn_2_command_name;
		break;
	case 3:
		scene_name = Gp_ini->msg_back.ex_btn_3_scene_name;
		command_name = Gp_ini->msg_back.ex_btn_3_command_name;
		break;
	case 4:
		scene_name = Gp_ini->msg_back.ex_btn_4_scene_name;
		command_name = Gp_ini->msg_back.ex_btn_4_command_name;
		break;
	}

	if (!scene_name.empty())
	{
		int scene_no = Gp_lexer->get_scn_no(scene_name);
		int command_no = Gp_lexer->get_user_cmd_no(scene_no, command_name);
		if (scene_no < 0)
		{
			tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("�w�肳�ꂽ�V�[����������܂���B"));
		}
		else if (command_no < 0)
		{
			tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("�w�肳�ꂽ�R�}���h��������܂���B"));
		}
		else
		{
			// �w��̃R�}���h���R�[��
			tnm_scene_proc_call_user_cmd(scene_no, command_no, FM_VOID, true, false);
		}
	}
}

// ****************************************************************
// �X�N���[���̗]�g���������Ă��邩�ǂ����𔻒肷��
// ================================================================
bool C_elm_msg_back::check_aftereffects()
{
	return m_drag_aftereffects_flag;
}

// ****************************************************************
// �X�v���C�g�c���[���\�z
// ================================================================
void C_elm_msg_back::get_sprite_tree(C_tree2<C_tnm_sprite *>& root)
{
	// �E�B���h�E����\���Ȃ�߂�
	if (!m_window_appear)
		return;

	// �w�i
	root.add_child(&m_spr_waku);
	root.add_child(&m_spr_filter);

	// �Z�p���[�^�[
	for (int i = 0; i < (int)m_separator_list.size(); i++)
		root.add_child(&m_separator_list[i].sprite);

	// �e�L�X�g
	for (int i = 0; i < (int)m_history_list.size(); i++)
		m_history_list[i].msg.get_sprite_tree(root);

	// ���A�C�R��
	for (int i = 0; i < (int)m_history_list.size(); i++)
		m_history_list[i].koe_btn.get_sprite_tree(root);

	// ���[�h�{�^��
	for (int i = 0; i < (int)m_history_list.size(); i++)
		m_history_list[i].load_btn.get_sprite_tree(root);

	// �X���C�_�[
	m_slider.get_sprite_tree(root);

	// �{�^��
	m_close_btn.get_sprite_tree(root);
	m_msg_up_btn.get_sprite_tree(root);
	m_msg_down_btn.get_sprite_tree(root);
	m_ex_btn_1.get_sprite_tree(root);
	m_ex_btn_2.get_sprite_tree(root);
	m_ex_btn_3.get_sprite_tree(root);
	m_ex_btn_4.get_sprite_tree(root);
}

// ****************************************************************
// �J��
// ================================================================
void C_elm_msg_back::open()
{
	m_window_appear = true;

	// �X�v���C�g��������
	m_spr_waku.init();
	m_spr_filter.init();
	m_separator_list.clear();

	// �E�B���h�E�֘A�̏��
	TSTR waku_file_name = Gp_ini->msg_back.waku_file;

	// �t�@�C�����̎w�肪����ꍇ�A�t�@�C����ǂݍ���
	if (!waku_file_name.empty())
	{
		// �E�B���h�E�̃X�v���C�g���쐬
		m_spr_waku.create_polygon_no_init();
		m_spr_waku.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
		m_spr_waku.set_index_buffer(G_rect_index_buffer);

		// �e�N�X�`����ǂݍ���
		m_album_waku = tnm_load_pct_d3d(waku_file_name, false, false);
		if (m_album_waku)
			m_spr_waku.set_texture(m_album_waku->get_texture(0));
	}
	else
	{
		// �E�B���h�E�̃X�v���C�g���쐬
		m_spr_waku.create_polygon_no_init();
		m_spr_waku.set_vertex_buffer(G_rect_vertex_buffer_d2_filter);
		m_spr_waku.set_index_buffer(G_rect_index_buffer);
	}

	// �t�B���^�[�֘A�̏��
	TSTR filter_file_name = Gp_ini->msg_back.filter_file;
	C_rect filter_margin = Gp_ini->msg_back.filter_margin;
	C_argb filter_color = Gp_ini->msg_back.filter_color;

	// �t�@�C�����̎w�肪����ꍇ�A�t�@�C����ǂݍ���
	if (!filter_file_name.empty())
	{
		// �t�B���^�[�̃X�v���C�g���쐬
		m_spr_filter.create_polygon_no_init();
		m_spr_filter.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
		m_spr_filter.set_index_buffer(G_rect_index_buffer);

		// �e�N�X�`����ǂݍ���
		TSTR filter_file_path = filter_file_name;
		m_album_filter = tnm_load_pct_d3d(filter_file_path, false, false);
		if (m_album_filter)
			m_spr_filter.set_texture(m_album_filter->get_texture(0));
	}
	else
	{
		// �t�B���^�[�̃X�v���C�g���쐬
		m_spr_filter.create_polygon_no_init();
		m_spr_filter.set_vertex_buffer(G_rect_vertex_buffer_d2_filter);
		m_spr_filter.set_index_buffer(G_rect_index_buffer);
	}

	// ���b�Z�[�W
	for (int i = 0; i < m_history_cnt_max; i++)
	{
		m_history_list[i].msg.set_def_param(Gp_ini->msg_back.moji_cnt
			, Gp_ini->msg_back.moji_size, Gp_ini->msg_back.moji_space, Gp_ini->mwnd.moji_color, Gp_ini->mwnd.shadow_color, Gp_ini->mwnd.fuchi_color, 0, 0
			, Gp_ini->msg_back.moji_size, 0, 0, C_rect(0, 0, 0, 0));

		// ���A�C�R��
		m_history_list[i].koe_btn.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());

		// ���[�h�{�^��
		m_history_list[i].load_btn.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());
	}

	// �X���C�_�[
	m_slider.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());
	if (!Gp_ini->msg_back.slider_file.empty())
	{
		m_slider.create_pct(Gp_ini->msg_back.slider_file);
		m_slider.set_order(Gp_ini->msg_back_order);
		m_slider.set_layer(Gp_ini->mwnd.moji_layer_rep);
		m_slider.set_disp(1);
		m_slider.set_button_param(0, 0, Gp_ini->msg_back.slider_action_no, Gp_ini->msg_back.slider_se_no);
		m_slider.set_button_push_keep(1);
	}

	// ����{�^��
	m_close_btn.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());
	if (!Gp_ini->msg_back.close_btn_file.empty())
	{
		m_close_btn.create_pct(Gp_ini->msg_back.close_btn_file);
		m_close_btn.set_order(Gp_ini->msg_back_order);
		m_close_btn.set_layer(Gp_ini->mwnd.moji_layer_rep);
		m_close_btn.set_pos_x(Gp_ini->msg_back.window_pos.x + Gp_ini->msg_back.close_btn_pos.x);
		m_close_btn.set_pos_y(Gp_ini->msg_back.window_pos.y + Gp_ini->msg_back.close_btn_pos.y);
		m_close_btn.set_disp(1);
		m_close_btn.set_button_param(0, 0, Gp_ini->msg_back.close_btn_action_no, Gp_ini->msg_back.close_btn_se_no);
	}

	// ���b�Z�[�W�A�b�v�{�^��
	m_msg_up_btn.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());
	if (!Gp_ini->msg_back.msg_up_btn_file.empty())
	{
		m_msg_up_btn.create_pct(Gp_ini->msg_back.msg_up_btn_file);
		m_msg_up_btn.set_order(Gp_ini->msg_back_order);
		m_msg_up_btn.set_layer(Gp_ini->mwnd.moji_layer_rep);
		m_msg_up_btn.set_pos_x(Gp_ini->msg_back.window_pos.x + Gp_ini->msg_back.msg_up_btn_pos.x);
		m_msg_up_btn.set_pos_y(Gp_ini->msg_back.window_pos.y + Gp_ini->msg_back.msg_up_btn_pos.y);
		m_msg_up_btn.set_disp(1);
		m_msg_up_btn.set_button_param(0, 0, Gp_ini->msg_back.msg_up_btn_action_no, Gp_ini->msg_back.msg_up_btn_se_no);
	}

	// ���b�Z�[�W�_�E���{�^��
	m_msg_down_btn.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());
	if (!Gp_ini->msg_back.msg_down_btn_file.empty())
	{
		m_msg_down_btn.create_pct(Gp_ini->msg_back.msg_down_btn_file);
		m_msg_down_btn.set_order(Gp_ini->msg_back_order);
		m_msg_down_btn.set_layer(Gp_ini->mwnd.moji_layer_rep);
		m_msg_down_btn.set_pos_x(Gp_ini->msg_back.window_pos.x + Gp_ini->msg_back.msg_down_btn_pos.x);
		m_msg_down_btn.set_pos_y(Gp_ini->msg_back.window_pos.y + Gp_ini->msg_back.msg_down_btn_pos.y);
		m_msg_down_btn.set_disp(1);
		m_msg_down_btn.set_button_param(0, 0, Gp_ini->msg_back.msg_down_btn_action_no, Gp_ini->msg_back.msg_down_btn_se_no);
	}

	// �ėp�{�^���P
	m_ex_btn_1.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());
	if (!Gp_ini->msg_back.ex_btn_1_file.empty())
	{
		m_ex_btn_1.create_pct(Gp_ini->msg_back.ex_btn_1_file);
		m_ex_btn_1.set_order(Gp_ini->msg_back_order);
		m_ex_btn_1.set_layer(Gp_ini->mwnd.moji_layer_rep);
		m_ex_btn_1.set_pos_x(Gp_ini->msg_back.window_pos.x + Gp_ini->msg_back.ex_btn_1_pos.x);
		m_ex_btn_1.set_pos_y(Gp_ini->msg_back.window_pos.y + Gp_ini->msg_back.ex_btn_1_pos.y);
		m_ex_btn_1.set_disp(1);
		m_ex_btn_1.set_button_param(0, 0, Gp_ini->msg_back.ex_btn_1_action_no, Gp_ini->msg_back.ex_btn_1_se_no);
	}

	// �ėp�{�^���Q
	m_ex_btn_2.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());
	if (!Gp_ini->msg_back.ex_btn_2_file.empty())
	{
		m_ex_btn_2.create_pct(Gp_ini->msg_back.ex_btn_2_file);
		m_ex_btn_2.set_order(Gp_ini->msg_back_order);
		m_ex_btn_2.set_layer(Gp_ini->mwnd.moji_layer_rep);
		m_ex_btn_2.set_pos_x(Gp_ini->msg_back.window_pos.x + Gp_ini->msg_back.ex_btn_2_pos.x);
		m_ex_btn_2.set_pos_y(Gp_ini->msg_back.window_pos.y + Gp_ini->msg_back.ex_btn_2_pos.y);
		m_ex_btn_2.set_disp(1);
		m_ex_btn_2.set_button_param(0, 0, Gp_ini->msg_back.ex_btn_2_action_no, Gp_ini->msg_back.ex_btn_2_se_no);
	}

	// �ėp�{�^���R
	m_ex_btn_3.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());
	if (!Gp_ini->msg_back.ex_btn_3_file.empty())
	{
		m_ex_btn_3.create_pct(Gp_ini->msg_back.ex_btn_3_file);
		m_ex_btn_3.set_order(Gp_ini->msg_back_order);
		m_ex_btn_3.set_layer(Gp_ini->mwnd.moji_layer_rep);
		m_ex_btn_3.set_pos_x(Gp_ini->msg_back.window_pos.x + Gp_ini->msg_back.ex_btn_3_pos.x);
		m_ex_btn_3.set_pos_y(Gp_ini->msg_back.window_pos.y + Gp_ini->msg_back.ex_btn_3_pos.y);
		m_ex_btn_3.set_disp(1);
		m_ex_btn_3.set_button_param(0, 0, Gp_ini->msg_back.ex_btn_3_action_no, Gp_ini->msg_back.ex_btn_3_se_no);
	}

	// �ėp�{�^���S
	m_ex_btn_4.init(S_element(), _T(""), true, false, false, false, -1, NULL, S_element(), S_element());
	if (!Gp_ini->msg_back.ex_btn_4_file.empty())
	{
		m_ex_btn_4.create_pct(Gp_ini->msg_back.ex_btn_4_file);
		m_ex_btn_4.set_order(Gp_ini->msg_back_order);
		m_ex_btn_4.set_layer(Gp_ini->mwnd.moji_layer_rep);
		m_ex_btn_4.set_pos_x(Gp_ini->msg_back.window_pos.x + Gp_ini->msg_back.ex_btn_4_pos.x);
		m_ex_btn_4.set_pos_y(Gp_ini->msg_back.window_pos.y + Gp_ini->msg_back.ex_btn_4_pos.y);
		m_ex_btn_4.set_disp(1);
		m_ex_btn_4.set_button_param(0, 0, Gp_ini->msg_back.ex_btn_4_action_no, Gp_ini->msg_back.ex_btn_4_se_no);
	}

	// �Z�p���[�^�[
	if (!Gp_ini->msg_back.separator_file.empty())
	{
		// �e�N�X�`����ǂݍ���
		m_album_separator = tnm_load_pct_d3d(Gp_ini->msg_back.separator_file, false, false);
	}
	if (!Gp_ini->msg_back.separator_top_file.empty())
	{
		// �e�N�X�`����ǂݍ���
		m_album_separator_top = tnm_load_pct_d3d(Gp_ini->msg_back.separator_top_file, false, false);
	}
	if (!Gp_ini->msg_back.separator_bottom_file.empty())
	{
		// �e�N�X�`����ǂݍ���
		m_album_separator_bottom = tnm_load_pct_d3d(Gp_ini->msg_back.separator_bottom_file, false, false);
	}

	// ���b�Z�[�W���쐬����
	create_msg();

	// �����̃X���C�_�[�ʒu�����߂�
	if (Gp_ini->tateyoko_mode == 1)
	{
		m_slider_pos = (int)Gp_ini->msg_back.slider_rect.left;
	}
	else
	{
		m_slider_pos = (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_y(0);
	}

	// �X���C�_�[����ʒu���v�Z����
	update_pos_from_slider();

	// ���߂ăX�N���[������X���C�_�[�ʒu���v�Z�i���b�Z�[�W���P�̂Ƃ��ɃX���C�_�[�̈ʒu�𒲐����邽�߁j
	update_pos_from_scroll();

	// �h���b�O�̗]�g������������
	m_drag_ready_flag = false;
	m_drag_flag = false;
	m_drag_aftereffects_flag = false;

	// PageUp / PageDown �L�[�������ꂽ��������������
	m_page_up_key = false;
	m_page_dw_key = false;

	// PageUp / PageDown �̃��s�[�g�^�C�}�[������������
	m_target_up_start_time = 0;
	m_target_dw_start_time = 0;

	// �^�[�Q�b�g�����肷��
	m_target_no = m_history_last_pos;
}

// ****************************************************************
// ����
// ================================================================
void C_elm_msg_back::close()
{
	m_window_appear = false;

	// ���b�Z�[�W���N���A����
	for (int i = 0; i < m_history_cnt_max; i++)
		m_history_list[i].msg.clear_msg();

	// ���{�^��
	for (int i = 0; i < m_history_cnt_max; i++)
		m_history_list[i].koe_btn.reinit(true);

	// ���[�h�{�^��
	for (int i = 0; i < m_history_cnt_max; i++)
		m_history_list[i].load_btn.reinit(true);

	// �e��{�^��
	m_slider.reinit(true);
	m_close_btn.reinit(true);
	m_msg_up_btn.reinit(true);
	m_msg_down_btn.reinit(true);
	m_ex_btn_1.reinit(true);
	m_ex_btn_2.reinit(true);
	m_ex_btn_3.reinit(true);
	m_ex_btn_4.reinit(true);

	// �Z�p���[�^�[
	m_separator_list.clear();

	// �A���o�����N���A
	m_album_waku.reset();
	m_album_filter.reset();
	m_album_separator.reset();
	m_album_separator_top.reset();
	m_album_separator_bottom.reset();

	// �X�v���C�g���N���A
	m_spr_waku.init();
	m_spr_filter.init();
}

// ****************************************************************
// ���b�Z�[�W��ǉ�����
// ================================================================
void C_elm_msg_back::ready_msg()
{
	// ���b�Z�[�W���쐬����

	// �܂����b�Z�[�W�����ӂ�Ă��Ȃ��ꍇ
	if (m_history_cnt < m_history_cnt_max)
	{
		// �V�K���b�Z�[�W�̏ꍇ�̓J�E���g�𑝂₷
		if (m_new_msg_flag)
		{
			m_new_msg_flag = false;
			m_history_cnt ++;

			// ���̈ʒu�̃��b�Z�[�W���N���A����
			m_history_list[m_history_insert_pos].save_id = S_tid();
			m_history_list[m_history_insert_pos].save_id_check_flag = false;
			m_history_list[m_history_insert_pos].pct_flag = false;
			m_history_list[m_history_insert_pos].original_name.clear();
			m_history_list[m_history_insert_pos].disp_name.clear();
			m_history_list[m_history_insert_pos].msg_str.clear();
			m_history_list[m_history_insert_pos].debug_msg.clear();
			m_history_list[m_history_insert_pos].scn_no = -1;
			m_history_list[m_history_insert_pos].line_no = -1;
			m_history_list[m_history_insert_pos].koe_no_list.clear();
			m_history_list[m_history_insert_pos].chr_no_list.clear();
			m_history_list[m_history_insert_pos].koe_play_no = 0;
		}
	}
	// ���b�Z�[�W�����ӂ�Ă���ꍇ
	else
	{
		// �V�K���b�Z�[�W�̏ꍇ�̓X�^�[�g�ʒu�����炷
		if (m_new_msg_flag)
		{
			m_new_msg_flag = false;
			m_history_start_pos = (m_history_start_pos + 1) % m_history_cnt_max;

			// ���̈ʒu�̃��b�Z�[�W���N���A����
			m_history_list[m_history_insert_pos].save_id = S_tid();
			m_history_list[m_history_insert_pos].save_id_check_flag = false;
			m_history_list[m_history_insert_pos].pct_flag = false;
			m_history_list[m_history_insert_pos].original_name.clear();
			m_history_list[m_history_insert_pos].disp_name.clear();
			m_history_list[m_history_insert_pos].msg_str.clear();
			m_history_list[m_history_insert_pos].debug_msg.clear();
			m_history_list[m_history_insert_pos].scn_no = -1;
			m_history_list[m_history_insert_pos].line_no = -1;
			m_history_list[m_history_insert_pos].koe_no_list.clear();
			m_history_list[m_history_insert_pos].chr_no_list.clear();
			m_history_list[m_history_insert_pos].koe_play_no = 0;
		}
	}
}

// ****************************************************************
// �Z�[�u ID ��ݒ�
// ================================================================
void C_elm_msg_back::set_save_id(S_tid save_id)
{
	if (m_history_insert_pos < 0 || m_history_cnt <= m_history_insert_pos)
		return;

	m_history_list[m_history_insert_pos].save_id = save_id;
}

void C_elm_msg_back::set_save_id_check_flag(bool save_id_check_flag)
{
	if (m_history_insert_pos < 0 || m_history_cnt <= m_history_insert_pos)
		return;

	m_history_list[m_history_insert_pos].save_id_check_flag = save_id_check_flag;
}

bool C_elm_msg_back::get_save_id_check_flag()
{
	if (m_history_insert_pos < 0 || m_history_cnt <= m_history_insert_pos)
		return false;

	return m_history_list[m_history_insert_pos].save_id_check_flag;
}

// ****************************************************************
// ����ǉ�
// ================================================================
bool C_elm_msg_back::add_koe(int koe_no, int chr_no, int scn_no, int line_no)
{
	if (koe_no < 0)
		return true;

	// ���b�Z�[�W��ǉ���������
	ready_msg();

	// ���b�Z�[�W��ǉ�����
	m_history_list[m_history_insert_pos].koe_no_list.push_back(koe_no);
	m_history_list[m_history_insert_pos].chr_no_list.push_back(chr_no);
	m_history_list[m_history_insert_pos].scn_no = scn_no;
	m_history_list[m_history_insert_pos].line_no = line_no;

	// ���������b�Z�[�W�̍Ō�
	m_history_last_pos = m_history_insert_pos;

	return true;
}

// ****************************************************************
// ���O��ǉ�
// ================================================================
bool C_elm_msg_back::add_name(CTSTR& original_name, CTSTR& disp_name, int scn_no, int line_no)
{
	if (disp_name.empty())
		return true;

	// ���b�Z�[�W��ǉ���������
	ready_msg();

	// ���b�Z�[�W��ǉ�����
	m_history_list[m_history_insert_pos].original_name = original_name;
	m_history_list[m_history_insert_pos].disp_name = disp_name;
	m_history_list[m_history_insert_pos].scn_no = scn_no;
	m_history_list[m_history_insert_pos].line_no = line_no;

	// ���������b�Z�[�W�̍Ō�
	m_history_last_pos = m_history_insert_pos;

	return true;
}

// ****************************************************************
// ���b�Z�[�W��ǉ�
// ================================================================
bool C_elm_msg_back::add_msg(CTSTR& msg, CTSTR& debug_msg, int scn_no, int line_no)
{
	if (msg.empty())
		return true;

	// ���b�Z�[�W��ǉ���������
	ready_msg();

	// ���b�Z�[�W��ǉ�����
	m_history_list[m_history_insert_pos].msg_str += msg;
	m_history_list[m_history_insert_pos].debug_msg = debug_msg;
	m_history_list[m_history_insert_pos].scn_no = scn_no;
	m_history_list[m_history_insert_pos].line_no = line_no;

	// ���������b�Z�[�W�̍Ō�
	m_history_last_pos = m_history_insert_pos;

	return true;
}

// ****************************************************************
// ���s��ǉ�
// ================================================================
bool C_elm_msg_back::add_new_line_indent(int scn_no, int line_no)
{
	// ���b�Z�[�W��ǉ���������
	ready_msg();

	// ���b�Z�[�W��ǉ�����i�f�o�b�O���b�Z�[�W�ɂ͉��s�͓���Ȃ����Ƃɒ��Ӂj
	m_history_list[m_history_insert_pos].msg_str += _T("\n");
	m_history_list[m_history_insert_pos].scn_no = scn_no;
	m_history_list[m_history_insert_pos].line_no = line_no;

	// ���������b�Z�[�W�̍Ō�
	m_history_last_pos = m_history_insert_pos;

	return true;
}
bool C_elm_msg_back::add_new_line_no_indent(int scn_no, int line_no)
{
	// ���b�Z�[�W��ǉ���������
	ready_msg();

	// ���b�Z�[�W��ǉ�����
	m_history_list[m_history_insert_pos].msg_str += _T("\a");
	//m_history_list[m_history_insert_pos].debug_msg = debug_msg;	// �f�o�b�O���b�Z�[�W�ɂ͉��s�͓���Ȃ�
	m_history_list[m_history_insert_pos].scn_no = scn_no;
	m_history_list[m_history_insert_pos].line_no = line_no;

	// ���������b�Z�[�W�̍Ō�
	m_history_last_pos = m_history_insert_pos;

	return true;
}

// ****************************************************************
// �摜��ǉ�
// ================================================================
bool C_elm_msg_back::add_pct(CTSTR& file_name, C_point pos)
{
	// �t�@�C�������݂��邩�𔻒肷��
	if (!tnm_check_pct(file_name, true))
		return false;

	// �K�����̃��b�Z�[�W��
	next_msg();

	// ���b�Z�[�W��ǉ���������
	ready_msg();

	// ���b�Z�[�W�ɉ摜����ݒ肷��
	m_history_list[m_history_insert_pos].pct_flag = true;
	m_history_list[m_history_insert_pos].pct_pos = pos;
	m_history_list[m_history_insert_pos].msg_str = file_name;

	// ���������b�Z�[�W�̍Ō�
	m_history_last_pos = m_history_insert_pos;

	// ����Ɏ����b�Z�[�W��
	next_msg();

	return true;
}

// ****************************************************************
// ���b�Z�[�W���N���A
// ================================================================
void C_elm_msg_back::clear_msg()
{
	m_history_list.clear();
	m_history_list.resize(m_history_cnt_max);
	for (int i = 0; i < m_history_cnt_max; i++)	{
		m_history_list[i].msg.init(S_element(), NULL);
		m_history_list[i].msg.set_def_param(Gp_ini->msg_back.moji_cnt
			, Gp_ini->msg_back.moji_size, Gp_ini->msg_back.moji_space, Gp_ini->mwnd.moji_color, Gp_ini->mwnd.shadow_color, Gp_ini->mwnd.fuchi_color, 10, 10
			, Gp_ini->msg_back.moji_size, 0, 0, C_rect(0, 0, 0, 0));
	}

	m_history_cnt = 0;
	m_history_start_pos = 0;
	m_history_last_pos = 0;
	m_history_insert_pos = 0;
	m_new_msg_flag = true;
}

// ****************************************************************
// ���̈ʒu��
// ================================================================
void C_elm_msg_back::next_msg()
{
	if (m_new_msg_flag)
		return;

	// ���b�Z�[�W����Ȃ牽�����Ȃ�
	if (m_history_list[m_history_insert_pos].pct_flag == 0 && m_history_list[m_history_insert_pos].msg_str.empty())
		return;

	// ���̃��b�Z�[�W��
	m_history_insert_pos = (m_history_insert_pos + 1) % m_history_cnt_max;

	// ���b�Z�[�W�����������鏀��
	m_new_msg_flag = true;
}

// ****************************************************************
// ���b�Z�[�W���\�z
// ================================================================
void C_elm_msg_back::create_msg()
{
	int last_margin = 0;

	m_msg_total_height = 0;

	// �Z�p���[�^�[�i�ŏ��j
	if (m_history_cnt > 0 && m_album_separator_top && m_album_separator_top->get_texture(0))
	{
		S_separator separator;
		separator.total_pos = - m_album_separator_top->get_texture(0)->get_original_height();
		separator.height = m_album_separator_top->get_texture(0)->get_original_height();
		separator.sprite.create_polygon_no_init();
		separator.sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
		separator.sprite.set_index_buffer(G_rect_index_buffer);
		separator.sprite.set_texture(m_album_separator_top->get_texture(0));
		m_separator_list.push_back(separator);

		//m_msg_total_height += separator.height;
	}

	// ���b�Z�[�W
	for (int i = 0; i < m_history_cnt; i++)
	{
		int history_pos = (m_history_start_pos + i) % m_history_cnt_max;

		// ���b�Z�[�W���\�z���Ă���
		m_history_list[history_pos].msg.clear_msg();
		if (m_history_list[history_pos].pct_flag)
		{
			// koe_btn ���g���܂킵�ĉ摜��ǂݍ���
			m_history_list[history_pos].koe_btn.create_pct(m_history_list[history_pos].msg_str);
			m_history_list[history_pos].koe_btn.set_disp(1);
			m_history_list[history_pos].koe_btn.set_layer(Gp_ini->mwnd.moji_layer_rep);
			m_history_list[history_pos].total_pos = m_msg_total_height;
			m_history_list[history_pos].height = m_history_list[history_pos].koe_btn.get_size_y(0);

			m_msg_total_height += m_history_list[history_pos].height;	// ���͂��̂��̂̍���
			last_margin = 0;											// ���̃}�[�W��
		}
		else
		{
			if (!m_history_list[history_pos].disp_name.empty())		// ���O
			{
				m_history_list[history_pos].msg.add_msg(m_history_list[history_pos].disp_name, NULL);
				m_history_list[history_pos].msg.new_line_no_indent();
			}
			if (!m_history_list[history_pos].msg_str.empty())		// ���b�Z�[�W
			{
				m_history_list[history_pos].msg.add_msg(m_history_list[history_pos].msg_str, NULL);
				m_history_list[history_pos].msg.new_line_no_indent();
			}
			m_history_list[history_pos].msg.disp_all_moji();
			m_history_list[history_pos].total_pos = m_msg_total_height + last_margin;
			m_history_list[history_pos].height = m_history_list[history_pos].msg.get_msg_height();

			// koe_btn �ɃA�C�R����ǂݍ���
			m_history_list[history_pos].koe_btn.create_pct(Gp_ini->msg_back.koe_btn_file);
			m_history_list[history_pos].koe_btn.set_disp(1);
			m_history_list[history_pos].koe_btn.set_layer(Gp_ini->mwnd.moji_layer_rep);
			m_history_list[history_pos].koe_btn.set_button_param(0, 0, Gp_ini->msg_back.koe_btn_action_no, Gp_ini->msg_back.koe_btn_se_no);

			// load_btn �ɃA�C�R����ǂݍ���
			m_history_list[history_pos].load_btn.create_pct(Gp_ini->msg_back.load_btn_file);
			m_history_list[history_pos].load_btn.set_disp(1);
			m_history_list[history_pos].load_btn.set_layer(Gp_ini->mwnd.moji_layer_rep);
			m_history_list[history_pos].load_btn.set_button_param(0, 0, Gp_ini->msg_back.load_btn_action_no, Gp_ini->msg_back.load_btn_se_no);

			m_msg_total_height += last_margin;							// ���͂̊Ԋu��
			m_msg_total_height += m_history_list[history_pos].height;	// ���͂��̂��̂̍���
			last_margin = Gp_ini->msg_back.moji_size;					// ���̃}�[�W��
		}

		// �Z�p���[�^�[�i�ԁj
		if (i < m_history_cnt - 1)
		{
			if (m_album_separator && m_album_separator->get_texture(0))
			{
				S_separator separator;
				separator.total_pos = m_msg_total_height;
				separator.height = m_album_separator->get_texture(0)->get_original_height();
				separator.sprite.create_polygon_no_init();
				separator.sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
				separator.sprite.set_index_buffer(G_rect_index_buffer);
				separator.sprite.set_texture(m_album_separator->get_texture(0));
				m_separator_list.push_back(separator);

				m_msg_total_height += separator.height;

				last_margin = 0;									// ���̃}�[�W��
			}
		}
		// �Z�p���[�^�[�i�Ō�j
		else
		{
			if (m_album_separator_bottom && m_album_separator_bottom->get_texture(0))
			{
				S_separator separator;
				separator.total_pos = m_msg_total_height;
				separator.height = m_album_separator_bottom->get_texture(0)->get_height();
				separator.sprite.create_polygon_no_init();
				separator.sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
				separator.sprite.set_index_buffer(G_rect_index_buffer);
				separator.sprite.set_texture(m_album_separator_bottom->get_texture(0));
				m_separator_list.push_back(separator);

				m_msg_total_height += separator.height;
			}
		}
	}
}

// ****************************************************************
// �f�o�b�O���b�Z�[�W���擾
// ================================================================

int C_elm_msg_back::get_koe_no()
{
	if (m_target_no < 0 || m_history_cnt <= m_target_no)
		return -1;

	if (m_history_list[m_target_no].koe_no_list.empty())
		return -1;

	return m_history_list[m_target_no].koe_no_list[0];
}

TSTR C_elm_msg_back::get_disp_name()
{
	if (m_target_no < 0 || m_history_cnt <= m_target_no)
		return _T("");

	return m_history_list[m_target_no].disp_name;
}

TSTR C_elm_msg_back::get_original_name()
{
	if (m_target_no < 0 || m_history_cnt <= m_target_no)
		return _T("");

	return m_history_list[m_target_no].original_name;
}

TSTR C_elm_msg_back::get_debug_msg()
{
	if (m_target_no < 0 || m_history_cnt <= m_target_no)
		return _T("");

	return m_history_list[m_target_no].debug_msg;
}

int C_elm_msg_back::get_scn_no()
{
	if (m_target_no < 0 || m_history_cnt <= m_target_no)
		return -1;

	return m_history_list[m_target_no].scn_no;
}

int C_elm_msg_back::get_line_no()
{
	if (m_target_no < 0 || m_history_cnt <= m_target_no)
		return -1;

	return m_history_list[m_target_no].line_no;
}

// ****************************************************************
// PageUp / PageDown �L�[�������ꂽ
// ================================================================
void C_elm_msg_back::page_up_key_down()
{
	m_page_up_key = true;
}

void C_elm_msg_back::page_dw_key_down()
{
	m_page_dw_key = true;
}

// ****************************************************************
// �^�[�Q�b�g���ړ�
// ================================================================
void C_elm_msg_back::target_up_sub(int cnt)
{
	if (m_target_no >= m_history_start_pos)	{
		m_target_no -= cnt;
		if (m_target_no < m_history_start_pos)
			m_target_no = m_history_start_pos;
	}
	else	{
		m_target_no -= cnt;
		if (m_target_no < 0)
			m_target_no += m_history_cnt;
	}
}

void C_elm_msg_back::target_down_sub(int cnt)
{
	int history_end_pos = (m_history_start_pos + m_history_cnt - 1) % m_history_cnt_max;

	if (m_target_no <= history_end_pos)	{
		m_target_no += cnt;
		if (m_target_no > history_end_pos)
			m_target_no = history_end_pos;
	}
	else	{
		m_target_no += cnt;
		if (m_target_no >= m_history_cnt)
			m_target_no -= m_history_cnt;
	}
}


void C_elm_msg_back::target_up()
{
	if (m_target_up_start_time == 0)	{
		m_target_up_start_time = ::timeGetTime();
	}
	else	{
		DWORD past_time = ::timeGetTime() - m_target_up_start_time;
		if (past_time <= 200)	{	return;	}
	}

	// �^�[�Q�b�g���P��Ɉړ�����
	target_up_sub(1);

	// �^�[�Q�b�g����ʒu���X�V����
	update_pos_from_target();
}

void C_elm_msg_back::target_down()
{
	if (m_target_dw_start_time == 0)	{
		m_target_dw_start_time = ::timeGetTime();
	}
	else	{
		DWORD past_time = ::timeGetTime() - m_target_dw_start_time;
		if (past_time <= 200)	{	return;	}
	}

	// �^�[�Q�b�g���P���Ɉړ�����
	target_down_sub(1);

	// �^�[�Q�b�g�ԍ�����X�N���[���ʒu�����߂�
	if (0 <= m_target_no && m_target_no < m_history_cnt)
		m_scroll_pos = m_cur.window_rect.height() / 2 - (m_history_list[m_target_no].total_pos + m_history_list[m_target_no].height / 2);

	// �^�[�Q�b�g����ʒu���X�V����
	update_pos_from_target();
}

// ****************************************************************
// �^�[�Q�b�g���P��Ɂi�z�C�[���p�j
// ================================================================
void C_elm_msg_back::target_up_for_wheel()
{
	// �^�[�Q�b�g���P��Ɉړ�����
	target_up_sub(1);

	// �^�[�Q�b�g����ʒu���X�V����
	update_pos_from_target();
}

void C_elm_msg_back::target_down_for_wheel()
{
	// �^�[�Q�b�g���P���Ɉړ�����
	target_down_sub(1);

	// �^�[�Q�b�g����ʒu���X�V����
	update_pos_from_target();
}

// ****************************************************************
// �^�[�Q�b�g����ʒu���X�V
// ================================================================
void C_elm_msg_back::update_pos_from_target()
{
	// �^�[�Q�b�g����X�N���[���ʒu�����߂�
	calc_scroll_pos_from_target();

	// �X�N���[������X���C�_�[�ʒu�����߂�
	calc_slider_pos_from_scroll();
}

// ****************************************************************
// �X�N���[������ʒu���X�V
// ================================================================
void C_elm_msg_back::update_pos_from_scroll()
{
	// �X�N���[������^�[�Q�b�g�ԍ������߂�
	calc_target_no_from_scroll();
	
	// �X�N���[������X���C�_�[�ʒu�����߂�
	calc_slider_pos_from_scroll();
}

// ****************************************************************
// �X���C�_�[����ʒu���X�V
// ================================================================
void C_elm_msg_back::update_pos_from_slider()
{
	// �X���C�_�[����X�N���[���ʒu�����߂�
	calc_scroll_pos_from_slider();

	// �X�N���[������^�[�Q�b�g�ԍ������߂�
	calc_target_no_from_scroll();
}

// ****************************************************************
// �^�[�Q�b�g����X�N���[���ʒu���X�V
// ================================================================
void C_elm_msg_back::calc_scroll_pos_from_target()
{
	// �^�[�Q�b�g��␳����
	m_target_no = limit(0, m_target_no, m_history_cnt - 1);

	// �X�N���[���ʒu�����߂�
	if (Gp_ini->tateyoko_mode == 1)
	{
		m_scroll_pos = - m_cur.window_rect.width() / 2 + (m_history_list[m_target_no].total_pos + m_history_list[m_target_no].height / 2);
	}
	else
	{
		m_scroll_pos = m_cur.window_rect.height() / 2 - (m_history_list[m_target_no].total_pos + m_history_list[m_target_no].height / 2);
	}
}

// ****************************************************************
// �X���C�_�[����X�N���[���ʒu���X�V
// ================================================================
void C_elm_msg_back::calc_scroll_pos_from_slider()
{
	if (m_history_cnt <= 0)
		return;

	if (Gp_ini->tateyoko_mode == 1)
	{
		// �q�X�g���[�̉E�[�ƍ��[�����߂�
		int histsp = m_history_start_pos;
		int histep = (m_history_start_pos + m_history_cnt - 1) % m_history_cnt_max;
		int msgsh = m_history_list[histsp].height;
		int msgeh = m_history_list[histep].height;
		int wind_width = m_cur.window_rect.width();		// �g�̕�
		int msgsp = - (wind_width / 2) + (msgsh / 2);
		int msgep = - (wind_width / 2) - (msgeh / 2) + m_msg_total_height;
		if (m_history_cnt == 1)	{ msgep = msgsp; }

		// ��ɃX���C�_�[�ʒu��␳����
		m_slider_pos = ::limit((int)Gp_ini->msg_back.slider_rect.left, m_slider_pos, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_x(0));

		// ��[�Ɖ��[����X�N���[���̈ʒu�����߂�
		m_scroll_pos = ::linear(m_slider_pos, (int)Gp_ini->msg_back.slider_rect.left, msgep, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_x(0), msgsp);
		m_scroll_pos = ::limit(msgsp, m_scroll_pos, msgep);
	}
	else
	{
		// �q�X�g���[�̏�[�Ɖ��[�����߂�
		int histsp = m_history_start_pos;
		int histep = (m_history_start_pos + m_history_cnt - 1) % m_history_cnt_max;
		int msgsh = m_history_list[histsp].height;
		int msgeh = m_history_list[histep].height;
		int wind_height = m_cur.window_rect.height();	// �g�̍���
		int msgsp = (wind_height / 2) - (msgsh / 2);
		int msgep = (wind_height / 2) + (msgeh / 2) - m_msg_total_height;
		if (m_history_cnt == 1)	{ msgep = msgsp; }

		// ��ɃX���C�_�[�ʒu��␳����
		m_slider_pos = ::limit((int)Gp_ini->msg_back.slider_rect.top, m_slider_pos, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_y(0));

		// ��[�Ɖ��[����X�N���[���̈ʒu�����߂�
		m_scroll_pos = ::linear(m_slider_pos, (int)Gp_ini->msg_back.slider_rect.top, msgsp, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_y(0), msgep);
		m_scroll_pos = ::limit(msgep, m_scroll_pos, msgsp);
	}
}

// ****************************************************************
// �X�N���[������^�[�Q�b�g�ԍ����X�V
// ================================================================
void C_elm_msg_back::calc_target_no_from_scroll()
{
	m_target_no = (m_history_start_pos + m_history_cnt - 1 + m_history_cnt_max) % m_history_cnt_max;

	if (Gp_ini->tateyoko_mode == 1)
	{
		for (int i = m_history_cnt - 1; i >= 0; i--)	{
			int index = (i + m_history_start_pos) % m_history_cnt_max;
			if (m_cur.window_rect.width() + m_scroll_pos - m_history_list[index].total_pos - m_history_list[index].height <= m_cur.window_rect.width() / 2)	{
				m_target_no = index;
			}
		}
	}
	else
	{
		for (int i = m_history_cnt - 1; i >= 0; i--)	{
			int index = (i + m_history_start_pos) % m_history_cnt_max;
			if (m_scroll_pos + m_history_list[index].total_pos + m_history_list[index].height >= m_cur.window_rect.height() / 2)	{
				m_target_no = index;
			}
		}
	}
}

// ****************************************************************
// �X�N���[������X���C�_�[�ʒu���X�V
// ================================================================
void C_elm_msg_back::calc_slider_pos_from_scroll()
{
	if (m_history_cnt <= 0)
		return;

	if (Gp_ini->tateyoko_mode == 1)
	{
		// �q�X�g���[�̉E�[�ƍ��[�����߂�
		int histsp = m_history_start_pos;
		int histep = (m_history_start_pos + m_history_cnt - 1) % m_history_cnt_max;
		int msgsh = m_history_list[histsp].height;
		int msgeh = m_history_list[histep].height;
		int wind_width = m_cur.window_rect.width();		// �g�̕�
		int msgsp = - (wind_width / 2) + (msgsh / 2);
		int msgep = - (wind_width / 2) - (msgeh / 2) + m_msg_total_height;
		if (m_history_cnt == 1)	{ msgep = msgsp; }

		// ��ɃX�N���[���ʒu��␳����
		m_scroll_pos = ::limit(msgsp, m_scroll_pos, msgep);

		// �E�[�ƍ��[����X���C�_�[�̈ʒu�����߂�
		m_slider_pos = ::linear(m_scroll_pos, msgsp, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_x(0), msgep, (int)Gp_ini->msg_back.slider_rect.left);
		m_slider_pos = ::limit((int)Gp_ini->msg_back.slider_rect.left, m_slider_pos, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_x(0));
	}
	else
	{
		// �q�X�g���[�̏�[�Ɖ��[�����߂�
		int histsp = m_history_start_pos;
		int histep = (m_history_start_pos + m_history_cnt - 1) % m_history_cnt_max;
		int msgsh = m_history_list[histsp].height;
		int msgeh = m_history_list[histep].height;
		int wind_height = m_cur.window_rect.height();	// �g�̍���
		int msgsp = (wind_height / 2) - (msgsh / 2);
		int msgep = (wind_height / 2) + (msgeh / 2) - m_msg_total_height;
		if (m_history_cnt == 1)	{ msgep = msgsp; }

		// ��ɃX�N���[���ʒu��␳����
		m_scroll_pos = ::limit(msgep, m_scroll_pos, msgsp);

		// ��[�Ɖ��[����X���C�_�[�̈ʒu�����߂�
		m_slider_pos = ::linear(m_scroll_pos, msgep, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_y(0), msgsp, (int)Gp_ini->msg_back.slider_rect.top);
		m_slider_pos = ::limit((int)Gp_ini->msg_back.slider_rect.top, m_slider_pos, (int)Gp_ini->msg_back.slider_rect.bottom - m_slider.get_size_y(0));
	}
}

