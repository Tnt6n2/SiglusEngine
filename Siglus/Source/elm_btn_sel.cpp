#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_input.h"
#include	"data/tnm_tool.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"data/tnm_lexer.h"

#include	"element/elm_group_def.h"
#include	"element/elm_btn_sel.h"
#include	"element/elm_btn_sel_item.h"
#include	"element/elm_mwnd_moji.h"
#include	"element/elm_sound.h"

#include	"engine/eng_input.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_scene.h"
#include	"engine/flow_script.h"
#include	"engine/ifc_stack.h"
#include	"engine/ifc_sound.h"
#include	"engine/eng_graphics.h"
#include	"engine/eng_fetch_point.h"


// ****************************************************************
// ������
// ================================================================
void C_elm_btn_select::init(S_element element, CTSTR& name)
{
	// �A�C�e�����X�g�̏�����
	m_item_list.init(element, FM_BTNSELITEMLIST, name, 0, true, NULL);

	// �ď�����
	reinit();
}

// ****************************************************************
// �ď�����
// ================================================================
void C_elm_btn_select::reinit()
{
	// �e���v���[�g�ԍ�
	m_template_no = -1;

	// �f�t�H���g�p�����[�^�̏�����
	m_def.base_pos = C_point(0, 0);
	m_def.rep_pos = C_point(0, 0);
	m_def.x_align = 0;
	m_def.y_align = 0;
	m_def.max_y_cnt = 0;
	m_def.line_width = 0;
	m_def.moji_cnt = 0;
	m_def.moji_pos = C_point(0, 0);
	m_def.moji_size = 0;
	m_def.moji_space = C_size(0, 0);
	m_def.moji_color = 0;
	m_def.moji_hit_color = 0;
	m_def.shadow_color = 0;
	m_def.fuchi_color = 0;
	m_def.btn_action_no = -1;
	m_def.open_anime_type = 0;
	m_def.open_anime_time = 0;
	m_def.close_anime_type = 0;
	m_def.close_anime_time = 0;
	m_def.decide_anime_type = 0;
	m_def.decide_anime_time = 0;
	m_def.sync_type = 0;

	// �����ϐ�������
	init_work_variable();
}

// ****************************************************************
// �{�^���I�����F�I��
// ================================================================
void C_elm_btn_select::finish()
{
	// �A�C�e�����X�g
	m_item_list.finish();
}

// ****************************************************************
// �{�^���I�����F�����ϐ�������
// ================================================================
void C_elm_btn_select::init_work_variable()
{
	m_cur = m_def;		// �f�t�H���g�p�����[�^�����݂̃p�����[�^�փR�s�[����

	m_open_anime_type = 0;
	m_open_anime_time = 0;
	m_open_anime_cur_time = 0;
	m_close_anime_type = 0;
	m_close_anime_time = 0;
	m_close_anime_cur_time = 0;
	m_decide_anime_type = 0;
	m_decide_anime_time = 0;
	m_decide_anime_cur_time = 0;
	m_decide_sel_no = -1;
	m_sync_type = 0;
	m_appear_flag = false;
	m_processing_flag_0 = false;
	m_processing_flag_1 = false;
	m_processing_flag_2 = false;
	m_cancel_enable_flag = false;
	m_capture_flag = false;
	m_sel_start_call_scn.clear();
	m_sel_start_call_z_no = -1;
	m_read_flag.scn_no = -1;
	m_read_flag.flag_no = -1;

	m_item_list.clear();

	m_capture_now_flag = false;
}

// ****************************************************************
// �e���v���[�g�̒l��ݒ�
// ================================================================
void C_elm_btn_select::set_template(int template_no)
{
	if (template_no == m_template_no)
		return;

	// �e���v���[�g�ԍ�
	m_template_no = template_no;

	// �e���v���[�g���č\�z
	restruct_template();

	// �A�C�e��
	for (int i = 0; i < (int)m_item_list.get_size(); i++)
		m_item_list[i].set_template(template_no);

	// �����ϐ�������
	init_work_variable();
}

// ****************************************************************
// �e���v���[�g���č\�z
// ================================================================
void C_elm_btn_select::restruct_template()
{
	m_def.base_pos = Gp_ini->sel_btn[m_template_no].base_pos;
	m_def.rep_pos = Gp_ini->sel_btn[m_template_no].rep_pos;
	m_def.x_align = Gp_ini->sel_btn[m_template_no].x_align;
	m_def.y_align = Gp_ini->sel_btn[m_template_no].y_align;
	m_def.max_y_cnt = Gp_ini->sel_btn[m_template_no].max_y_cnt;
	m_def.line_width = Gp_ini->sel_btn[m_template_no].line_width;
	m_def.moji_cnt = Gp_ini->sel_btn[m_template_no].moji_cnt;
	m_def.moji_pos = Gp_ini->sel_btn[m_template_no].moji_pos;
	m_def.moji_size = Gp_ini->sel_btn[m_template_no].moji_size;
	m_def.moji_space = Gp_ini->sel_btn[m_template_no].moji_space;
	m_def.moji_x_align = Gp_ini->sel_btn[m_template_no].moji_x_align;
	m_def.moji_y_align = Gp_ini->sel_btn[m_template_no].moji_y_align;
	m_def.moji_color = Gp_ini->sel_btn[m_template_no].moji_color;
	m_def.moji_hit_color = Gp_ini->sel_btn[m_template_no].moji_hit_color;
	m_def.shadow_color = Gp_ini->mwnd.shadow_color;
	m_def.fuchi_color = Gp_ini->mwnd.fuchi_color;
	m_def.btn_action_no = Gp_ini->sel_btn[m_template_no].btn_action_no;
	m_def.open_anime_type = Gp_ini->sel_btn[m_template_no].open_anime_type;
	m_def.open_anime_time = Gp_ini->sel_btn[m_template_no].open_anime_time;
	m_def.close_anime_type = Gp_ini->sel_btn[m_template_no].close_anime_type;
	m_def.close_anime_time = Gp_ini->sel_btn[m_template_no].close_anime_time;
	m_def.decide_anime_type = Gp_ini->sel_btn[m_template_no].decide_anime_type;
	m_def.decide_anime_time = Gp_ini->sel_btn[m_template_no].decide_anime_time;
}

// ****************************************************************
// �Z�[�u
// ================================================================
void C_elm_btn_select::save(C_tnm_save_stream& stream)
{
	// �e���v���[�g�ԍ�
	stream.save(m_template_no);

	// ���݂̃p�����[�^
	stream.save(m_cur);
	stream.save(m_appear_flag);
	stream.save(m_processing_flag_0);
	stream.save(m_processing_flag_1);
	stream.save(m_processing_flag_2);
	stream.save(m_cancel_enable_flag);
	stream.save(m_capture_flag);
	stream.save(m_sel_start_call_scn);
	stream.save(m_sel_start_call_z_no);

	// �A�C�e��
	m_item_list.save(stream);
}

// ****************************************************************
// ���[�h
// ================================================================
void C_elm_btn_select::load(C_tnm_save_stream& stream)
{
	// �e���v���[�g�ԍ�
	stream.load(m_template_no);

	// �e���v���[�g���č\�z
	restruct_template();

	// ���݂̃p�����[�^
	stream.load(m_cur);
	stream.load(m_appear_flag);
	stream.load(m_processing_flag_0);
	stream.load(m_processing_flag_1);
	stream.load(m_processing_flag_2);
	stream.load(m_cancel_enable_flag);
	stream.load(m_capture_flag);
	stream.load(m_sel_start_call_scn);
	stream.load(m_sel_start_call_z_no);

	// �A�C�e��
	m_item_list.load(stream);
}

// ****************************************************************
// �R�s�[
// ================================================================
void C_elm_btn_select::copy(C_elm_btn_select* src)
{
	// �e���v���[�g�ԍ�
	m_template_no = src->m_template_no;

	// �e���v���[�g���č\�z
	restruct_template();

	// ���݂̃p�����[�^
	m_cur = src->m_cur;
	m_appear_flag = src->m_appear_flag;
	m_processing_flag_0 = src->m_processing_flag_0;
	m_processing_flag_1 = src->m_processing_flag_1;
	m_processing_flag_2 = src->m_processing_flag_2;
	m_cancel_enable_flag = src->m_cancel_enable_flag;
	m_capture_flag = src->m_capture_flag;
	m_sel_start_call_scn = src->m_sel_start_call_scn;
	m_sel_start_call_z_no = src->m_sel_start_call_z_no;
	
	// �A�C�e��
	m_item_list.resize(src->m_item_list.get_size());
	for (int i = 0; i < m_item_list.get_size(); i++)
		m_item_list[i] = src->m_item_list[i];
}

// ****************************************************************
// �t���[������������
// ================================================================
void C_elm_btn_select::frame_init()
{
	// �A�C�e��
	for (int i = 0; i < m_item_list.get_size(); i++)
		m_item_list[i].frame_init();
}

// ****************************************************************
// ���Ԃ�i�߂�
// ================================================================
void C_elm_btn_select::update_time(int past_game_time, int past_real_time)
{
	// ���Ԃ�i�߂�
	m_open_anime_cur_time += past_game_time;
	m_close_anime_cur_time += past_game_time;
	m_decide_anime_cur_time += past_game_time;

	// �A�C�e��
	for (int i = 0; i < m_item_list.get_size(); i++)
		m_item_list[i].update_time(past_game_time, past_real_time);

	// �o�����Ă��邩���A�j�����̂ݐi�߂�
	if (m_appear_flag || is_close_anime_doing())	{

		int open_anime_end_time = m_open_anime_time;
		int close_anime_end_time = m_close_anime_time;
		int decide_anime_end_time = m_decide_anime_time;

		// �c�����̃A�C�e�������v�Z���Ă���
		int y_cnt;
		if (m_cur.max_y_cnt > 0)	{
			y_cnt = std::min(m_cur.max_y_cnt, m_item_list.get_size());
		}
		else	{
			y_cnt = m_item_list.get_size();
		}

		// �A�j���ɂ���Ă͏I��鎞�Ԃ����������
		if (m_open_anime_type == 2 || m_open_anime_type == 3 || m_open_anime_type == 4 || m_open_anime_type == 5 || m_open_anime_type == 6)	{

			// �c�����̃A�C�e�����~50ms
			open_anime_end_time += y_cnt * 50;
		}
		if (m_close_anime_type == 2 || m_close_anime_type == 3 || m_close_anime_type == 4 || m_close_anime_type == 5 || m_close_anime_type == 6)	{

			// �c�����̃A�C�e�����~50ms
			close_anime_end_time += y_cnt * 50;
		}

		// �L���v�`���[���������ꍇ�̓L���v�`���[�I��
		if (is_capture_doing())	{
			end_capture();
		}
		// ���Ԃ����ĂΊJ���A�j���I��
		if (is_open_anime_doing() && m_open_anime_cur_time - open_anime_end_time >= 0)	{
			end_open_anime();
		}
		// ���Ԃ����ĂΕ��A�j���I��
		if (is_close_anime_doing() && m_close_anime_cur_time - close_anime_end_time >= 0)	{
			end_close_anime();
		}
		// ���Ԃ����ĂΌ���A�j���I��
		if (is_decide_anime_doing() && m_decide_anime_cur_time - decide_anime_end_time >= 0)	{
			end_decide_anime();
		}
	}
}

// ****************************************************************
// �t���[������
// ================================================================
void C_elm_btn_select::frame(bool draw_futoku, bool draw_shadow)
{
	// �o�����Ă��邩���A�j�����̂ݕ\������
	if (m_appear_flag || is_close_anime_doing())	{

		// �c�����̃A�C�e�������v�Z���Ă���
		int y_cnt;
		if (m_cur.max_y_cnt > 0)	{
			y_cnt = std::min(m_cur.max_y_cnt, m_item_list.get_size());
		}
		else	{
			y_cnt = m_item_list.get_size();
		}

		// �A�C�e���̃t���[������
		for (int i = 0; i < m_item_list.get_size(); i++)	{
			int tr_rep = 255;
			int x_rep = 0, y_rep = 0;

			// �J���A�j���ɂ��␳
			if (false);
			else if (m_open_anime_type == 1)	{
				tr_rep = linear_limit(m_open_anime_cur_time, 0, 0, m_open_anime_time, 255);
			}
			else if (m_open_anime_type == 2)	{
				int time = m_open_anime_cur_time - 50 * (y_cnt - (i % y_cnt));				// 50�ؕb���x�点��
				tr_rep = linear_limit(time, 0, 0, m_open_anime_time, 255);
				y_rep = speed_down_limit(time, 0, - (int)Gp_global->game_screen_size.cy, m_open_anime_time, 0);
			}
			else if (m_open_anime_type == 3)	{
				int time = m_open_anime_cur_time - 50 * (i % y_cnt);						// 50�ؕb���x�点��
				tr_rep = linear_limit(time, 0, 0, m_open_anime_time, 255);
				y_rep = speed_down_limit(time, 0, (int)Gp_global->game_screen_size.cy, m_open_anime_time, 0);
			}
			else if (m_open_anime_type == 4)	{
				int time = m_open_anime_cur_time - 50 * (i % y_cnt);						// 50�ؕb���x�点��
				tr_rep = linear_limit(time, 0, 0, m_open_anime_time, 255);
				x_rep = speed_down_limit(time, 0, - (int)Gp_global->game_screen_size.cx, m_open_anime_time, 0);
			}
			else if (m_open_anime_type == 5)	{
				int time = m_open_anime_cur_time - 50 * (i % y_cnt);						// 50�ؕb���x�点��
				tr_rep = linear_limit(time, 0, 0, m_open_anime_time, 255);
				x_rep = speed_down_limit(time, 0, (int)Gp_global->game_screen_size.cx, m_open_anime_time, 0);
			}
			else if (m_open_anime_type == 6)	{
				int time = m_open_anime_cur_time - 50 * (i % y_cnt);						// 50�ؕb���x�点��
				tr_rep = linear_limit(time, 0, 0, m_open_anime_time, 255);
				if ((i % y_cnt) % 2 == 0)	x_rep = speed_down_limit(time, 0, (int)Gp_global->game_screen_size.cx, m_open_anime_time, 0);
				else						x_rep = speed_down_limit(time, 0, - (int)Gp_global->game_screen_size.cx, m_open_anime_time, 0);
			}

			// ����A�j���ɂ��␳
			if (m_decide_sel_no >= 0 && i != m_decide_sel_no)	{

				if (false);
				else if (m_decide_anime_type == 1)	{
					tr_rep = linear_limit(m_decide_anime_cur_time, 0, 255, m_decide_anime_time, 0);
				}
				else if (m_decide_anime_type == 2)	{
					C_point decided_pos = m_item_list[m_decide_sel_no].get_pos();
					C_point my_pos = m_item_list[i].get_pos();
					C_point rep_pos = decided_pos - my_pos;

					tr_rep = linear_limit(m_decide_anime_cur_time, 0, 255, m_decide_anime_time, 0);
					x_rep = speed_down_limit(m_decide_anime_cur_time, 0, 0, m_decide_anime_time, rep_pos.x);
					y_rep = speed_down_limit(m_decide_anime_cur_time, 0, 0, m_decide_anime_time, rep_pos.y);
				}
			}

			// ����A�j��������ꍇ�A�I�����ꂽ�A�C�e���ȊO�͏����Ă���͂�
			if (m_close_anime_type >= 1 && m_cur.decide_anime_type >= 1 && m_decide_sel_no >= 0 && i != m_decide_sel_no)	{
				tr_rep = 0;
			}
			else	{

				// ���A�j���ɂ��␳
				if (false);
				else if (m_close_anime_type == 1)	{
					tr_rep = linear_limit(m_close_anime_cur_time, 0, 255, m_close_anime_time, 0);
				}
				else if (m_close_anime_type == 2)	{
					int time = m_close_anime_cur_time - 50 * (i % y_cnt);						// 50�ؕb���x�点��
					tr_rep = linear_limit(time, 0, 255, m_close_anime_time, 0);
					y_rep = speed_up_limit(time, 0, 0, m_close_anime_time, - (int)Gp_global->game_screen_size.cy);
				}
				else if (m_close_anime_type == 3)	{
					int time = m_close_anime_cur_time - 50 * (y_cnt - (i % y_cnt));				// 50�ؕb���x�点��
					tr_rep = linear_limit(time, 0, 255, m_close_anime_time, 0);
					y_rep = speed_up_limit(time, 0, 0, m_close_anime_time, (int)Gp_global->game_screen_size.cy);
				}
				else if (m_close_anime_type == 4)	{
					int time = m_close_anime_cur_time - 50 * (i % y_cnt);						// 50�ؕb���x�点��
					tr_rep = linear_limit(time, 0, 255, m_close_anime_time, 0);
					x_rep = speed_up_limit(time, 0, 0, m_close_anime_time, - (int)Gp_global->game_screen_size.cx);
				}
				else if (m_close_anime_type == 5)	{
					int time = m_close_anime_cur_time - 50 * (i % y_cnt);						// 50�ؕb���x�点��
					tr_rep = linear_limit(time, 0, 255, m_close_anime_time, 0);
					x_rep = speed_up_limit(time, 0, 0, m_close_anime_time, (int)Gp_global->game_screen_size.cx);
				}
				else if (m_close_anime_type == 6)	{
					int time = m_close_anime_cur_time - 50 * (i % y_cnt);						// 50�ؕb���x�点��
					tr_rep = linear_limit(time, 0, 255, m_close_anime_time, 0);
					if ((i % y_cnt) % 2 == 0)	x_rep = speed_up_limit(time, 0, 0, m_close_anime_time, (int)Gp_global->game_screen_size.cx);
					else						x_rep = speed_up_limit(time, 0, 0, m_close_anime_time, - (int)Gp_global->game_screen_size.cx);
				}
			}

			// �A�C�e���Ɉ����p�������_�[�p�����[�^
			S_tnm_render_param item_trp;
			item_trp.disp = true;
			item_trp.sorter = S_tnm_sorter(Gp_ini->mwnd.order, 0);
			item_trp.tr = tr_rep;
			
			// �A�C�e���̃t���[������
			m_item_list[i].frame(m_cur.base_pos + C_point(x_rep, y_rep), &item_trp, m_cur.moji_hit_color, m_cur.btn_action_no, draw_futoku, draw_shadow);
		}
	}
}

// ****************************************************************
// �X�v���C�g�c���[���\�z
// ================================================================
void C_elm_btn_select::get_sprite_tree(C_tree2<C_tnm_sprite *>& root)
{
	// �o�����Ă��邩���A�j�����̂ݕ\������
	if (m_appear_flag || is_close_anime_doing())	{

		// �I����
		for (int i = 0; i < m_item_list.get_size(); i++)
			m_item_list[i].get_sprite_tree(root);
	}
}

// ****************************************************************
// �{�^����o�^
// ================================================================
void C_elm_btn_select::regist_button()
{
	// �o�����Ă��ĊJ���A�j�����I����ăL���v�`���[���I�����Ă���ꍇ�̂ݏ���
	if (m_appear_flag && !is_open_anime_doing() && !is_decide_anime_doing() && !m_capture_now_flag)	{

		// �I����
		for (int i = 0; i < m_item_list.get_size(); i++)
			m_item_list[i].regist_button();
	}
}

// ****************************************************************
// �{�^���C�x���g����
// ================================================================
void C_elm_btn_select::button_event()
{
	// �o�����Ă��ĊJ���A�j�����I����Ă���ꍇ�̂ݏ���
	if (m_appear_flag && !is_open_anime_doing() && !is_decide_anime_doing())
	{
		int sel_no = TNM_GROUP_NOT_DECIDED;	// �܂����肳��Ă��Ȃ�

		// �L�����Z�����ꂽ�ꍇ	
		if (m_cancel_enable_flag && tnm_input_use_key_down_up(VK_EX_CANCEL))
		{
			sel_no = TNM_GROUP_CANCELED;
		}
		else
		{
			// �L�����Z������ĂȂ��̂Ŋe�I�����̏������s���܂�
			for (int i = 0; i < m_item_list.get_size(); i++)
			{
				if (m_item_list[i].button_event())
					sel_no = i;
			}
		}

		// �I����������܂��̓L�����Z�����ꂽ�I
		if (sel_no >= -1)
		{
			// ����
			decide(sel_no);
			// �X�N���v�g�擾�p�̃��b�Z�[�W
			if (sel_no == TNM_GROUP_CANCELED)
			{
				Gp_global->last_sel_msg = _T("�i�L�����Z���j");
			}
			else
			{
				Gp_global->last_sel_msg = m_item_list[sel_no].get_msg();
			}

			// �o�^���ꂽ���ǃt���O�𗧂Ă�
			set_read_flag();
		}
	}
}

// ****************************************************************
// �󂩂ǂ�������
// ================================================================
bool C_elm_btn_select::is_empty()
{
	return m_item_list.is_empty();
}

// ****************************************************************
// �I�����̏����ʒu��ݒ肷��
// ================================================================
void C_elm_btn_select::set_def_pos(C_point pos)
{
	m_def.moji_pos = pos;
	m_cur.moji_pos = pos;
}

// ****************************************************************
// �I������ǉ�
// ================================================================
void C_elm_btn_select::add_sel(CTSTR& msg, int item_type, int color)
{
	// �F���f�t�H���g�̏ꍇ�͎w�肷��
	if (color == -1)
		color = m_cur.moji_color;

	// �A�C�e�����쐬
	int old_cnt = m_item_list.get_size();
	m_item_list.resize(old_cnt + 1);
	m_item_list[old_cnt].set_template(m_template_no);
	m_item_list[old_cnt].set_msg(msg, item_type, m_cur.moji_size, m_cur.moji_pos, m_cur.moji_space, m_cur.moji_x_align, m_cur.moji_y_align, color, m_cur.shadow_color, m_cur.fuchi_color);
}

// ****************************************************************
// �I�������č\�z
// ================================================================
void C_elm_btn_select::restruct_item()
{
	// ================================================================
	// �c�ɂ���[���ƕ��ׂ܂��B�S�̋�`���v�Z���܂��B
	// ----------------------------------------------------------------

	// �I�����̐�
	int item_cnt = m_item_list.get_size();
	if (!m_item_list.is_empty())	{

		// �A�C�e���̈ʒu���v�Z����
		C_point offset;
		C_point max_offset;
		int org_offset_x = 0;
		for (int i = 0, y_cnt = 0; i < item_cnt; i++)	{

			// type == off �̏ꍇ�͉������Ȃ�
			if (m_item_list[i].get_item_type() != TNM_SEL_ITEM_TYPE_OFF)	{

				m_item_list[i].set_pos(offset);
				offset += m_cur.rep_pos;

				if (offset.x > max_offset.x)	{
					max_offset.x = offset.x;
				}
				if (offset.y > max_offset.y)	{
					max_offset.y = offset.y;
				}

				y_cnt++;

				// �x�̍ő�l���w�肳��Ă���ꍇ�͉��s�i����H�j
				if (m_cur.max_y_cnt > 0 && y_cnt >= m_cur.max_y_cnt)	{
					offset.x = org_offset_x + m_cur.line_width;
					offset.y = 0;
					org_offset_x = offset.x;
					y_cnt = 0;
				}
			}
		}

		// �A�C�e���̑傫�����擾����
		C_size item_size = m_item_list[0].get_item_size();

		// �I�����S�̂̑傫�����v�Z����
		int total_x = max_offset.x - m_cur.rep_pos.x + item_size.cx;
		int total_y = max_offset.y - m_cur.rep_pos.y + item_size.cy;

		// �A�C�e���̈ʒu���Čv�Z����
		for (int i = 0; i < item_cnt; i++)	{

			// type == off �̏ꍇ�͉������Ȃ�
			if (m_item_list[i].get_item_type() != TNM_SEL_ITEM_TYPE_OFF)	{
				int rep_x = 0, rep_y = 0;

				if (false);
				else if (m_cur.x_align == 1)	{	rep_x -= total_x / 2;	}
				else if (m_cur.x_align == 2)	{	rep_x -= total_x;		}

				if (false);
				else if (m_cur.y_align == 1)	{	rep_y -= total_y / 2;	}
				else if (m_cur.y_align == 2)	{	rep_y -= total_y;		}

				m_item_list[i].set_pos(m_item_list[i].get_pos() + C_point(rep_x, rep_y));
			}
		}
	}
}

// ****************************************************************
// �L�����Z���\��ݒ肷��
// ================================================================
void C_elm_btn_select::set_cancel_enable(bool enable)
{
	m_cancel_enable_flag = enable;
}

// ****************************************************************
// �L���v�`���[�t���O��ݒ肷��
// ================================================================
void C_elm_btn_select::set_capture_flag(bool value)
{
	m_capture_flag = value;
}

// ****************************************************************
// �I���J�n���ɌĂ΂��R�[����ݒ肷��
// ================================================================
void C_elm_btn_select::set_sel_start_call(CTSTR& scn_name, int z_no)
{
	m_sel_start_call_scn = scn_name;
	m_sel_start_call_z_no = z_no;
}

// ****************************************************************
// �I���J�n���ɌĂ΂��R�[�����s��
// ================================================================
void C_elm_btn_select::do_sel_start_call()
{
	// �V�[�����R�[��
	if (!tnm_scene_proc_farcall(m_sel_start_call_scn, m_sel_start_call_z_no, FM_VOID, false, true))
		return;

	// �ēx�X�N���v�g�����s����
	tnm_proc_script();
}

// ****************************************************************
// �I�����J�n����
// ================================================================
void C_elm_btn_select::start_sel(int sync_type)
{
	m_cur.sync_type = sync_type;

	// �J���A�j����ݒ�
	m_open_anime_type = m_cur.open_anime_type;
	m_open_anime_time = m_cur.open_anime_time;
	m_open_anime_cur_time = 0;

	// �I�����o��
	m_appear_flag = true;
	m_processing_flag_0 = true;
	m_processing_flag_1 = true;
	m_processing_flag_2 = true;
	m_sync_type = sync_type;

	// �A�C�e����\������
	for (int i = 0; i < m_item_list.get_size(); i++)	{
		m_item_list[i].set_disp();
	}

	// �t�F�b�`�|�C���g������������
	tnm_init_fetch_point();

	// �t�F�b�`�̏C�����W���v�Z
	C_point rep_pos;
	if (!m_item_list.is_empty())	{
		rep_pos.y = m_item_list[0].get_item_size().cy / 2;
		rep_pos.x = rep_pos.y;
	}

	// �t�F�b�`�|�C���g��ǉ�����
	for (int i = 0; i < m_item_list.get_size(); i++)	{
		if (m_item_list[i].get_item_type() == TNM_SEL_ITEM_TYPE_ON)	{	// �I���ł�����̂����ݒ�
			C_point pos = m_cur.base_pos + m_item_list[i].get_pos() + rep_pos;
			C_rect rect = C_rect::by_size(m_cur.base_pos + m_item_list[i].get_pos(), m_item_list[i].get_item_size());
			tnm_add_fetch_point(pos, rect);
		}
	}

	// �t�F�b�`�����ړ�
	tnm_fetch_proc_initial_move();

	// �I�[�g���[�h���ɏ����Ă����J�[�\���𕜋A������
	Gp_global->auto_mode_cursor_off = false;
}

// ****************************************************************
// ���A�j�����J�n����
// ================================================================
void C_elm_btn_select::close()
{
	// �o���t���O���~�낷
	m_appear_flag = false;

	// ���A�j����ݒ�
	m_close_anime_type = m_cur.close_anime_type;
	m_close_anime_time = m_cur.close_anime_time;
	m_close_anime_cur_time = 0;

	// ����A�j��������ꍇ�͕��A�j�����ύX�����
	if (m_decide_anime_type >= 1 && m_decide_sel_no >= 0 && m_close_anime_type >= 1)
		m_close_anime_type = 1;

	// ���A�j�����Ȃ��ꍇ�͕��A�j���I��
	if (m_close_anime_type == 0)	{
		end_close_anime();
	}

	// �������t���O�����낷�i���̂Q�j
	m_processing_flag_1 = false;
}

// ****************************************************************
// ���肷��
// ================================================================
void C_elm_btn_select::decide(int sel_no)
{
	// ���肵���ԍ���ۑ�����
	m_decide_sel_no = sel_no;

	// ���ʉ�
	tnm_play_se(m_decide_sel_no >= 0 ? TNM_SE_TYPE_DECIDE : TNM_SE_TYPE_CANCEL);

	// ���ʂ��X�^�b�N�ɐς�
	tnm_stack_push_int(sel_no);

	// �I�����|�C���g�𗧂Ă�
	tnm_set_sel_point();

	// �������t���O�������i���̂R�j
	m_processing_flag_2 = false;

	// ����A�j����ݒ�
	if (m_decide_sel_no >= 0 && m_cur.decide_anime_type >= 1)	{
		m_decide_anime_type = m_cur.decide_anime_type;
		m_decide_anime_time = m_cur.decide_anime_time;
		m_decide_anime_cur_time = 0;
	}
	// �L�����Z���A�������͌���A�j�����Ȃ��ꍇ�͂�������
	else	{
		close();
	}

	// �t�F�b�`�|�C���g������������
	tnm_init_fetch_point();
}

// ****************************************************************
// �L���v�`���[���I������
// ================================================================
void C_elm_btn_select::end_capture()
{
	m_capture_now_flag = false;

	// �I�����J�n�R�[�����s��
	if (!m_sel_start_call_scn.empty() && m_sel_start_call_z_no >= 0)	{
		Gp_global->sel_start_call_flag = true;
	}
}

// ****************************************************************
// �J���A�j�����I������
// ================================================================
void C_elm_btn_select::end_open_anime()
{
	m_open_anime_type = 0;

	// �L���v�`���[���s��
	if (m_capture_flag)	{
		m_capture_now_flag = true;
		tnm_syscom_create_capture(TNM_CAPTURE_PRIOR_SAVE, false);
	}
}

// ****************************************************************
// ���A�j�����I������
// ================================================================
void C_elm_btn_select::end_close_anime()
{
	m_close_anime_type = 0;

	// �L���v�`���[���������
	if (m_capture_flag)	{
		tnm_syscom_free_capture(TNM_CAPTURE_PRIOR_SAVE);
	}

	// �A�C�e�����X�g���N���A
	m_item_list.clear();

	// �������t���O���~�낷
	// �O�̂��ߑS�Ẵt���O���~�낵�܂��B����ň��S�B
	m_processing_flag_0 = false;
	m_processing_flag_1 = false;
	m_processing_flag_2 = false;
}

// ****************************************************************
// ����A�j�����I������
// ================================================================
void C_elm_btn_select::end_decide_anime()
{
	// ����i���� m_decide_anime_type ���g���Ă�̂Ő�ɍs���I�j
	close();

	// ���Ƀt���O���~�낷
	m_decide_anime_type = 0;
}

// ****************************************************************
// �L���v�`���[���𔻒肷��
// ================================================================
bool C_elm_btn_select::is_capture_doing()
{
	return m_capture_now_flag;
}

// ****************************************************************
// �J���A�j�����𔻒肷��
// ================================================================
bool C_elm_btn_select::is_open_anime_doing()
{
	return m_open_anime_type > 0;
}

// ****************************************************************
// ���A�j�����𔻒肷��
// ================================================================
bool C_elm_btn_select::is_close_anime_doing()
{
	return m_close_anime_type > 0;
}

// ****************************************************************
// ����A�j�����𔻒肷��
// ================================================================
bool C_elm_btn_select::is_decide_anime_doing()
{
	return m_decide_anime_type > 0;
}

// ****************************************************************
// ���������ǂ����𔻒肷��
// ================================================================
bool C_elm_btn_select::is_processing()
{
	switch (m_sync_type)	{
		case 0:		return m_processing_flag_0;
		case 1:		return m_processing_flag_1;
		case 2:		return m_processing_flag_2;
	}

	return false;
}

// ****************************************************************
// ���ǃt���O��o�^����
// ================================================================
void C_elm_btn_select::regist_read_flag(int scn_no, int flag_no)
{
	m_read_flag.scn_no = scn_no;
	m_read_flag.flag_no = flag_no;
}

// ****************************************************************
// �o�^�������ǃt���O�𗧂Ă�
// ================================================================
void C_elm_btn_select::set_read_flag()
{
	if (0 <= m_read_flag.scn_no && m_read_flag.scn_no < (int)Gp_read_flag->size())	{
		if (0 <= m_read_flag.flag_no && m_read_flag.flag_no < (int)(*Gp_read_flag)[m_read_flag.scn_no].size())	{
			(*Gp_read_flag)[m_read_flag.scn_no][m_read_flag.flag_no] = 1;
		}
	}

	m_read_flag.scn_no = -1;
	m_read_flag.flag_no = -1;
}

