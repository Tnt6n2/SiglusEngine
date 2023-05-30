#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_array.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"data/tnm_sorter.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_btn_template.h"
#include	"data/tnm_color_table.h"
#include	"element/elm_stage.h"
#include	"element/elm_mwnd_msg.h"
#include	"element/elm_mwnd_moji.h"
#include	"engine/eng_config.h"
#include	"engine/eng_message.h"
#include	"engine/ifc_sound.h"
#include	"engine/ifc_error.h"
#include	"engine/flow_command.h"

// ****************************************************************
// ������
// ================================================================
void C_elm_mwnd_msg::init(S_element elm_group_list, C_elm_mwnd* p_parent)
{
	// �f�t�H���g�p�����[�^
	m_def_parent = p_parent;
	m_def_elm_group_list = elm_group_list;

	// �f�t�H���g�̋��ʃp�����[�^
	m_def.moji_cnt = C_size(0, 0);
	m_def.moji_pos = C_point(0, 0);
	m_def.moji_rep_pos = C_point(0, 0);
	m_def.moji_size = 0;
	m_def.moji_space = C_size(0, 0);
	m_def.moji_color = 0;
	m_def.shadow_color = 0;
	m_def.fuchi_color = 0;
	m_def.name_moji_size = 0;
	m_def.name_newline = 0;
	m_def.name_bracket = 0;

	// �p�����[�^������
	init_param();
}

// ****************************************************************
// �f�t�H���g�̒l��ݒ�
// ================================================================
void C_elm_mwnd_msg::set_def_param(C_size moji_cnt
	, int moji_size, C_size moji_space, int moji_color, int shadow_color, int fuchi_color, int ruby_size, int ruby_space
	, int name_moji_size, int name_newline, int name_bracket, C_rect talk_margin)
{
	// �e���v���[�g�ԍ��ɂ���ď�����
	m_def.moji_cnt = moji_cnt;
	m_def.moji_pos = C_point(0, 0);
	m_def.moji_rep_pos = C_point(0, 0);
	m_def.moji_size = moji_size;
	m_def.moji_space = moji_space;
	m_def.moji_color = moji_color;
	m_def.shadow_color = shadow_color;
	m_def.fuchi_color = fuchi_color;
	m_def.ruby_size = ruby_size;
	m_def.ruby_space = ruby_space;
	m_def.name_moji_size = name_moji_size;
	m_def.name_newline = name_newline;
	m_def.name_bracket = name_bracket;
	m_def.talk_margin = talk_margin;

	// �p�����[�^������
	init_param();
}

// ****************************************************************
// �����ϐ�������
// ================================================================
//		�f�t�H���g�̃p�����[�^�ɖ߂��܂��B
// ================================================================
void C_elm_mwnd_msg::init_param()
{
	m_cur = m_def;				// �f�t�H���g�p�����[�^�����݂̃p�����[�^�փR�s�[����
	m_cur.moji_color = -1;		// �F�ɂ��Ắu�f�t�H���g�ɏ]���v�ɐݒ肵�Ă����I
	m_cur.shadow_color = -1;	// �F�ɂ��Ắu�f�t�H���g�ɏ]���v�ɐݒ肵�Ă����I
	m_cur.fuchi_color = -1;		// �F�ɂ��Ắu�f�t�H���g�ɏ]���v�ɐݒ肵�Ă����I

	init_param_sub();
}

void C_elm_mwnd_msg::init_param_sub()
{
	m_chara_moji_color_no = -1;		// �f�t�H���g�ɏ]��
	m_chara_shadow_color_no = -1;	// �f�t�H���g�ɏ]��
	m_chara_fuchi_color_no = -1;	// �f�t�H���g�ɏ]��
	m_indent_pos = 0;
	m_indent_moji = 0;
	m_indent_cnt = 0;
	m_cur_msg_type = -1;				// �N���A��
	m_ruby_start_pos = C_point(0, 0);
	m_disp_moji_cnt = 0;
	m_hide_moji_cnt = 0;
	m_last_moji_disp_time = 0;
	m_btn_no = -1;
	m_btn_group_no = -1;
	m_btn_action_no = -1;
	m_btn_se_no = -1;

	m_cur_msg_type_decided = false;
	m_line_head_flag = true;
	m_ruby_start_ready = false;
	m_button_flag = false;
	m_progress_start_flag = false;

	m_debug_msg.clear();
	m_ruby.clear();
	m_moji_list.clear();
	m_msg_btn.clear();

	// ���b�Z�[�W�̈��傫���v�Z����i�����������͌��݂̒l���g���j
	if (Gp_ini->tateyoko_mode == 1)
	{
		m_msg_size.cx = (m_def.moji_size * m_cur.moji_cnt.row()) + (m_def.moji_space.row() * (m_cur.moji_cnt.row() - 1));
		m_msg_size.cy = (m_def.moji_size * m_cur.moji_cnt.column()) + (m_def.moji_space.column() * (m_cur.moji_cnt.column() - 1));
	}
	else
	{
		m_msg_size.cx = (m_def.moji_size * m_cur.moji_cnt.column()) + (m_def.moji_space.column() * (m_cur.moji_cnt.column() - 1));
		m_msg_size.cy = (m_def.moji_size * m_cur.moji_cnt.row()) + (m_def.moji_space.row() * (m_cur.moji_cnt.row() - 1));
	}
}

// ****************************************************************
// �Z�[�u
// ================================================================
void C_elm_mwnd_msg::save(C_tnm_save_stream& stream)
{
	// ���݂̃p�����[�^
	stream.save(m_cur);
	stream.save(m_chara_moji_color_no);
	stream.save(m_chara_shadow_color_no);
	stream.save(m_chara_fuchi_color_no);
	stream.save(m_indent_pos);
	stream.save(m_indent_moji);
	stream.save(m_indent_cnt);
	stream.save(m_cur_msg_type);
	stream.save(m_ruby_start_pos);
	stream.save(m_disp_moji_cnt);
	stream.save(m_hide_moji_cnt);
	stream.save(m_debug_msg);
	stream.save(m_ruby);
	stream.save(m_btn_no);
	stream.save(m_btn_group_no);
	stream.save(m_btn_action_no);
	stream.save(m_btn_se_no);

	stream.save(m_cur_msg_type_decided);
	stream.save(m_line_head_flag);
	stream.save(m_ruby_start_ready);
	stream.save(m_button_flag);

	// ����
	stream.save(m_moji_list);
}

// ****************************************************************
// ���[�h
// ================================================================
void C_elm_mwnd_msg::load(C_tnm_save_stream& stream)
{
	// ���݂̃p�����[�^
	stream.load(m_cur);
	stream.load(m_chara_moji_color_no);
	stream.load(m_chara_shadow_color_no);
	stream.load(m_chara_fuchi_color_no);
	stream.load(m_indent_pos);
	stream.load(m_indent_moji);
	stream.load(m_indent_cnt);
	stream.load(m_cur_msg_type);
	stream.load(m_ruby_start_pos);
	stream.load(m_disp_moji_cnt);
	stream.load(m_hide_moji_cnt);
	stream.load(m_debug_msg);
	stream.load(m_ruby);
	stream.load(m_btn_no);
	stream.load(m_btn_group_no);
	stream.load(m_btn_action_no);
	stream.load(m_btn_se_no);

	stream.load(m_cur_msg_type_decided);
	stream.load(m_line_head_flag);
	stream.load(m_ruby_start_ready);
	stream.load(m_button_flag);

	// ����
	stream.load(m_moji_list);

	// ���b�Z�[�W�̈��傫���v�Z����i�����������͌��݂̒l���g���j
	if (Gp_ini->tateyoko_mode == 1)
	{
		m_msg_size.cx = (m_def.moji_size * m_cur.moji_cnt.row()) + (m_def.moji_space.row() * (m_cur.moji_cnt.row() - 1));
		m_msg_size.cy = (m_def.moji_size * m_cur.moji_cnt.column()) + (m_def.moji_space.column() * (m_cur.moji_cnt.column() - 1));
	}
	else
	{
		m_msg_size.cx = (m_def.moji_size * m_cur.moji_cnt.column()) + (m_def.moji_space.column() * (m_cur.moji_cnt.column() - 1));
		m_msg_size.cy = (m_def.moji_size * m_cur.moji_cnt.row()) + (m_def.moji_space.row() * (m_cur.moji_cnt.row() - 1));
	}
}

// ****************************************************************
// �R�s�[
// ================================================================
void C_elm_mwnd_msg::copy(C_elm_mwnd_msg* src)
{
	// �f�t�H���g�̃p�����[�^���R�s�[
	m_def = src->m_def;

	// ���݂̃p�����[�^
	m_cur = src->m_cur;
	m_indent_pos = src->m_indent_pos;
	m_chara_moji_color_no = src->m_chara_moji_color_no;
	m_chara_shadow_color_no = src->m_chara_shadow_color_no;
	m_chara_fuchi_color_no = src->m_chara_fuchi_color_no;
	m_indent_moji = src->m_indent_moji;
	m_indent_cnt = src->m_indent_cnt;
	m_ruby_start_pos = src->m_ruby_start_pos;
	m_disp_moji_cnt = src->m_disp_moji_cnt;
	m_hide_moji_cnt = src->m_hide_moji_cnt;
	m_last_moji_disp_time = src->m_last_moji_disp_time;
	m_debug_msg = src->m_debug_msg;
	m_ruby = src->m_ruby;
	m_btn_no = src->m_btn_no;
	m_btn_group_no = src->m_btn_group_no;
	m_btn_action_no = src->m_btn_action_no;
	m_btn_se_no = src->m_btn_se_no;

	m_line_head_flag = src->m_line_head_flag;
	m_ruby_start_ready = src->m_ruby_start_ready;
	m_button_flag = src->m_button_flag;
	m_progress_start_flag = src->m_progress_start_flag;

	// ���b�Z�[�W�̈��傫���v�Z����i�����������͌��݂̒l���g���j
	if (Gp_ini->tateyoko_mode == 1)
	{
		m_msg_size.cx = (m_def.moji_size * m_cur.moji_cnt.row()) + (m_def.moji_space.row() * (m_cur.moji_cnt.row() - 1));
		m_msg_size.cy = (m_def.moji_size * m_cur.moji_cnt.column()) + (m_def.moji_space.column() * (m_cur.moji_cnt.column() - 1));
	}
	else
	{
		m_msg_size.cx = (m_def.moji_size * m_cur.moji_cnt.column()) + (m_def.moji_space.column() * (m_cur.moji_cnt.column() - 1));
		m_msg_size.cy = (m_def.moji_size * m_cur.moji_cnt.row()) + (m_def.moji_space.row() * (m_cur.moji_cnt.row() - 1));
	}

	// ����
	m_moji_list.resize(src->m_moji_list.size());
	for (int i = 0; i < (int)m_moji_list.size(); i++)
		m_moji_list[i] = src->m_moji_list[i];
}

// ****************************************************************
// �t���[������������
// ================================================================
void C_elm_mwnd_msg::frame_init()
{
	// ���ׂĂ̕����ɑ΂��ăt���[��������
	for (int i = 0; i < (int)m_moji_list.size(); i++)
		m_moji_list[i].frame_init();

	// �{�^��
	for (std::map<int, C_msg_btn>::iterator itr = m_msg_btn.begin(); itr != m_msg_btn.end(); ++itr)
		itr->second.m_button.frame_init();
}

// ****************************************************************
// �t���[������
// ================================================================
bool C_elm_mwnd_msg::frame(const S_tnm_render_param* parent_trp, C_point pos_rep, int color_rate, C_argb color, C_argb shadow_color, C_argb fuchi_color, bool draw_futoku, bool draw_shadow)
{
	// �������Ă�{�^����T��
	int active_btn_no = -1;
	int active_btn_state = TNM_BTN_STATE_NORMAL;

	// �O���[�v���擾
	C_elm_group* p_group = get_group();
	if (p_group && p_group->is_doing())
	{
		for (std::map<int, C_msg_btn>::iterator itr = m_msg_btn.begin(); itr != m_msg_btn.end(); ++itr)
		{
			// �A�N�e�B�u�ȏꍇ
			if (Gp_btn_mng->is_active(&itr->second.m_button))
			{
				active_btn_no = itr->second.m_btn_no;
				active_btn_state = Gp_btn_mng->get_hit_state(&itr->second.m_button);
				break;
			}
		}
	}

	// ���ׂĂ̕����ɑ΂��ăt���[������
	int moji_cnt = (int)m_moji_list.size();
	for (int i = 0; i < moji_cnt; i++)
	{
		C_point new_pos_rep = pos_rep;
		int this_color_rate = color_rate;
		C_argb this_color = color;
		C_argb this_shadow_color = shadow_color;
		C_argb this_fuchi_color = fuchi_color;

		// �c�����̏ꍇ�͋N�_���E�ォ�獶��ɕϊ�
		if (Gp_ini->tateyoko_mode == 1)
		{
			new_pos_rep.x -= m_moji_list[i].get_moji_size();
		}

		// �{�^���̏ꍇ
		std::map<int, C_msg_btn>::iterator itr = m_msg_btn.find(i);
		if (itr != m_msg_btn.end())
		{
			// �A�N�V������K�p
			if (active_btn_no != -1 && itr->second.m_btn_no == active_btn_no)
			{
				if (0 <= itr->second.m_action_no && itr->second.m_action_no < Gp_ini->msgbtn_cnt)
				{
					int color_no = Gp_ini->msgbtn[itr->second.m_action_no].state[active_btn_state].color_no;
					if (0 <= color_no && color_no < Gp_ini->color_table_cnt)
					{
						this_color_rate = 255;
						this_color = Gp_color_table->get_color(color_no);
						this_shadow_color = Gp_color_table->get_color(Gp_ini->mwnd.shadow_color);
						this_fuchi_color = Gp_color_table->get_color(Gp_ini->mwnd.fuchi_color);
					}
				}
			}
			else
			{
				if (0 <= itr->second.m_action_no && itr->second.m_action_no < Gp_ini->msgbtn_cnt)
				{
					int color_no = Gp_ini->msgbtn[itr->second.m_action_no].state[TNM_BTN_STATE_NORMAL].color_no;
					if (0 <= color_no && color_no < Gp_ini->color_table_cnt)
					{
						this_color_rate = 255;
						this_color = Gp_color_table->get_color(color_no);
						this_shadow_color = Gp_color_table->get_color(Gp_ini->mwnd.shadow_color);
						this_fuchi_color = Gp_color_table->get_color(Gp_ini->mwnd.fuchi_color);
					}
				}
			}
		}

		// �t���[������
		if (!m_moji_list[i].frame(parent_trp, new_pos_rep, 0, this_color_rate, this_color, this_shadow_color, this_fuchi_color, draw_futoku, draw_shadow))
			return false;
	}

	// �{�^������
	for (std::map<int, C_msg_btn>::iterator itr = m_msg_btn.begin(); itr != m_msg_btn.end(); ++itr)
	{
		C_elm_mwnd_moji* p_moji = &m_moji_list[itr->first];

		int x = (int)parent_trp->pos_x + pos_rep.x + p_moji->get_moji_pos().x;
		int y = (int)parent_trp->pos_y + pos_rep.y + p_moji->get_moji_pos().y;
		int size = p_moji->get_moji_size();

		itr->second.m_button.set_param(C_rect::by_size(x, y, size, size), S_tnm_sorter());
	}

	return true;
}

// ****************************************************************
// �e�N�X�`���̃N���A
// ================================================================
void C_elm_mwnd_msg::clear_texture()
{
	// ���ׂĂ̕����ɑ΂��ăN���A����
	for (int i = 0; i < (int)m_moji_list.size(); i++)
		m_moji_list[i].clear_texture();
}

// ****************************************************************
// �X�v���C�g�c���[���\�z
// ================================================================
void C_elm_mwnd_msg::get_sprite_tree(C_tree2<C_tnm_sprite *>& root)
{
	// ���ׂĂ̕����ɑ΂��ăX�v���C�g�c���[���\�z
	for (int i = 0; i < (int)m_moji_list.size(); i++)
		m_moji_list[i].get_sprite_tree(root, root);
}

// ****************************************************************
// �{�^����o�^
// ================================================================
void C_elm_mwnd_msg::regist_button()
{
	// ����
	for (std::map<int, C_msg_btn>::iterator itr = m_msg_btn.begin(); itr != m_msg_btn.end(); ++itr)
		Gp_btn_mng->regist_button(&itr->second.m_button);
}

// ****************************************************************
// �{�^����o�^
// ================================================================
void C_elm_mwnd_msg::button_event()
{
	// �O���[�v���擾
	C_elm_group* p_group = get_group();
	if (p_group && p_group->is_doing())
	{
		for (std::map<int, C_msg_btn>::iterator itr = m_msg_btn.begin(); itr != m_msg_btn.end(); ++itr)
		{
			// ��������
			if (Gp_btn_mng->is_hit_this_frame(&itr->second.m_button))
			{
				// ���ʉ�
				if (0 <= itr->second.m_se_no && itr->second.m_se_no < (int)Gp_btn_se_list->m_sub.size())
				{
					int se_no = Gp_btn_se_list->m_sub[itr->second.m_se_no].hit_no;
					tnm_play_se(se_no);
				}
			}
			// �������Ă���
			if (Gp_btn_mng->is_hit(&itr->second.m_button))
			{
				p_group->set_hit_button_no(itr->second.m_btn_no);
			}
			// �����ꂽ
			if (Gp_btn_mng->is_pushed_this_frame(&itr->second.m_button))
			{
				// ���ʉ�
				if (0 <= itr->second.m_se_no && itr->second.m_se_no < (int)Gp_btn_se_list->m_sub.size())
				{
					int se_no = Gp_btn_se_list->m_sub[itr->second.m_se_no].push_no;
					tnm_play_se(se_no);
				}
			}
			// ������Ă���
			if (Gp_btn_mng->is_pushed(&itr->second.m_button))
			{
				p_group->set_pushed_button_no(itr->second.m_btn_no);
			}
		}
	}
}

// ****************************************************************
// ��������O���[�v���擾
// ================================================================
C_elm_group* C_elm_mwnd_msg::get_group()
{
	if (!m_def_elm_group_list.is_empty())
	{
		if (m_btn_group_no >= 0)
		{
			S_element elm_group = m_def_elm_group_list + S_element(ELM_ARRAY, m_btn_group_no);

			// ���΃p�X
			if (elm_group.code[0] == ELM_UP)
			{
				S_tnm_command_proc_arg_struct arg;
				arg.disp_out_of_range_error = false;
				if (tnm_command_proc_mwnd(m_def_parent, elm_group.begin(), elm_group.begin(), elm_group.end(), &arg))
				{
					C_elm_group* p_group = (C_elm_group *)arg.p_ret_element;
					if (p_group)
					{
						return p_group;
					}
				}
			}
			// ��΃p�X
			else
			{
				C_elm_group* p_group = (C_elm_group *)tnm_get_element_ptr(elm_group.begin(), elm_group.end(), false);
				if (p_group)
				{
					return p_group;
				}
			}
		}
	}

	return NULL;
}
// ****************************************************************
// �󂩂ǂ�������
// ================================================================
bool C_elm_mwnd_msg::is_empty()
{
	return m_moji_list.empty();
}

// ****************************************************************
// ���b�Z�[�W�̃N���A
// ================================================================
void C_elm_mwnd_msg::clear_msg()
{
	m_cur.moji_pos = m_def.moji_pos;		// �f�t�H���g�̕����ʒu

	// �p�����[�^�̏������i�T�u�j
	init_param_sub();
}

// ****************************************************************
// ���b�Z�[�W�̒ǉ�
// ================================================================
bool is_kinsoku_moji(TCHAR moji)
{
	if (
		moji == _T('�B') || moji == _T('�A') || 
		moji == _T('�I') || moji == _T('�H') || 
		moji == _T('�F') || moji == _T('�G') || 
		moji == _T('�v') || moji == _T('�x') || 
		moji == _T('�j') || moji == _T('�z') || 
		moji == _T('�p') || moji == _T('�n') || 
		moji == _T('�r') || moji == _T('�t') || 
		moji == _T('�h') || moji == _T('�f') || moji == _T('��') || 
		moji == _T('�`') || moji == _T('�[') || 
		moji == _T('�c') || moji == _T('�d') || moji == _T('�E') || 
		moji == _T('�D') || moji == _T('�C') || 
		moji == _T('�J') || moji == _T('�K') ||	// �� �u�J�v�u�K�vRealLive �ɂȂ�
		moji == _T('��') || moji == _T('��') || moji == _T('��') || moji == _T('��') || moji == _T('��') || 
		moji == _T('��') || moji == _T('��') || moji == _T('��') || moji == _T('��') || moji == _T('��') || 
		moji == _T('�@') || moji == _T('�B') || moji == _T('�D') || moji == _T('�F') || moji == _T('�H') || 
		moji == _T('�b') || moji == _T('��') || moji == _T('��') || moji == _T('��') || moji == _T('��') ||
		moji == _T('��') || moji == _T('��') ||
		moji == _T('�')  || moji == _T('�')  ||
		moji == _T('�')  || moji == _T('�')  ||
		moji == _T('!')  || moji == _T('?')  ||
		moji == _T(':')  || moji == _T(';')  ||
		moji == _T('�')  || moji == _T(')')  ||
		moji == _T(']')  || moji == _T('>')  || moji == _T('}') ||
		moji == _T('\'') || moji == _T('"')  ||  // �� �u"�vRealLive �ɂȂ�
		moji == _T('�')  || moji == _T('�')  ||
		moji == _T('.')  || moji == _T(',')  ||
		moji == _T('�')  || moji == _T('�')  || moji == _T('�') || moji == _T('�') || moji == _T('�') || 
		moji == _T('�')  || moji == _T('�')  || moji == _T('�') || moji == _T('�')
	)	{
		return true;
	}

	return false;
}

void C_elm_mwnd_msg::add_msg_sub(CTSTR& msg, TSTR* ret_over_flow_msg)
{
	if (msg.empty())
		return;

	// �����̗̈��\�񂵂Ă����i�m�ۂł͂Ȃ��I�j
	m_moji_list.reserve(m_moji_list.size() + msg.size());

	for (TSTR::const_iterator itr = msg.begin(); itr != msg.end();)
	{
		int moji_type = TNM_MOJI_TYPE_MOJI;
		int moji_code = -1;

		// ���݂̃C�e���[�^��ۑ����Ă���
		TSTR::const_iterator old_itr = itr;

		// �G��������
		tnm_msg_proc_expand_name_flag(itr, msg.end(), &moji_type, &moji_code);

		// �\�����镶���̏ꍇ
		if (moji_code == _T('\n'))
		{
			new_line_indent();			// �����I�ɉ��s�i�C���f���g����j
		}
		else if (moji_code == _T('\a'))
		{
			new_line_no_indent();		// �����I�ɉ��s�i�C���f���g�Ȃ��j
		}
		else if (!(moji_type == TNM_MOJI_TYPE_MOJI && moji_code == 0))
		{
			bool auto_indent_flag = false;

			// �����̑傫�����擾
			int this_moji_size = 0;
			if (moji_type != TNM_MOJI_TYPE_MOJI)
			{
				this_moji_size = m_cur.moji_size;
			}
			else
			{
				this_moji_size = is_hankaku((TCHAR)moji_code) ? m_cur.moji_size / 2 : m_cur.moji_size;
			}

			// �������s�̔���p�̃T�C�Y
			int this_check_size = this_moji_size + m_cur.moji_space.column();

#if defined (__SIGLUS_EN) || defined (__SIGLUS_ES) || defined (__SIGLUS_DE) || defined (__SIGLUS_ID) || defined (__SIGLUS_FR)

			// �p�P��ɂ�鎩�����s
			if (moji_code == _T(' '))
			{
				for (TSTR::const_iterator itr_2 = itr; itr_2 != msg.end(); ++itr_2)
				{
					if (is_hankaku(*itr_2) && *itr_2 != _T(' '))
					{
						this_check_size += m_cur.moji_size / 2 + m_cur.moji_space.column();
					}
					else
					{
						break;
					}
				}
			}
#endif

			// �s�ɓ��肫��Ȃ��ꍇ�͉��s
			if (Gp_ini->tateyoko_mode == 1)
			{
				if (m_cur.moji_pos.y + this_check_size > m_msg_size.cy + m_def.moji_size)
				{
					new_line_indent();			// �֑������щz�����̂ŕK�����s
					auto_indent_flag = true;	// �����C���f���g�t���O
				}
				else if (m_cur.moji_pos.y + this_check_size > m_msg_size.cy)
				{
					if (!is_kinsoku_moji(moji_code))
					{
						new_line_indent();			// �֑������łȂ������̂ŉ��s
						auto_indent_flag = true;	// �����C���f���g�t���O
					}
				}
			}
			else
			{
				if (m_cur.moji_pos.x + this_check_size > m_msg_size.cx + m_def.moji_size)
				{
					new_line_indent();			// �֑������щz�����̂ŕK�����s
					auto_indent_flag = true;	// �����C���f���g�t���O
				}
				else if (m_cur.moji_pos.x + this_check_size > m_msg_size.cx)
				{
					if (!is_kinsoku_moji(moji_code))
					{
						new_line_indent();			// �֑������łȂ������̂ŉ��s
						auto_indent_flag = true;	// �����C���f���g�t���O
					}
				}
			}

			// �����C���f���g����̃X�y�[�X�͖�������
			if (auto_indent_flag)
			{
				if (moji_type == TNM_MOJI_TYPE_MOJI && (moji_code == _T(' ') || moji_code == _T('�@')))
				{
					continue;
				}
			}

			// �s���𒴂����ꍇ�́u���y�[�W�v�i�I�[�o�[�t���[�j
			if (Gp_ini->tateyoko_mode == 1)
			{
				if (m_cur.moji_pos.x <= -m_msg_size.cx)
				{
					// �I�[�o�[�t���[���b�Z�[�W��ݒ�
					if (ret_over_flow_msg)
					{
						*ret_over_flow_msg = TSTR(old_itr, msg.end());	// �\�����Ȃ������̂ŌÂ��C�e���[�^���w��
					}

					break;
				}
			}
			else
			{
				if (m_cur.moji_pos.y >= m_msg_size.cy)
				{
					// �I�[�o�[�t���[���b�Z�[�W��ݒ�
					if (ret_over_flow_msg)
					{
						*ret_over_flow_msg = TSTR(old_itr, msg.end());	// �\�����Ȃ������̂ŌÂ��C�e���[�^���w��
					}

					break;
				}
			}

			// �������쐬
			C_elm_mwnd_moji moji;
			int moji_color_no = m_cur.moji_color != -1 ? m_cur.moji_color : (Gp_config->message_chrcolor_flag && m_chara_moji_color_no != -1) ? m_chara_moji_color_no : m_def.moji_color;
			int shadow_color_no = m_cur.shadow_color != -1 ? m_cur.shadow_color : (Gp_config->message_chrcolor_flag && m_chara_shadow_color_no != -1) ? m_chara_shadow_color_no : m_def.shadow_color;
			int fuchi_color_no = m_cur.fuchi_color != -1 ? m_cur.fuchi_color : (Gp_config->message_chrcolor_flag && m_chara_fuchi_color_no != -1) ? m_chara_fuchi_color_no : m_def.fuchi_color;
			moji.set_moji(moji_type, moji_code, m_cur.moji_size, moji_color_no, shadow_color_no, fuchi_color_no, m_cur.moji_pos + m_cur.moji_rep_pos);

			// ���r�̊J�n�ʒu��ݒ�
			if (m_ruby_start_ready)
			{
				m_ruby_start_pos = m_cur.moji_pos;
				m_ruby_start_ready = false;
			}

			// ���̕����ʒu�̐ݒ�
			if (Gp_ini->tateyoko_mode == 1)
			{
				m_cur.moji_pos.y += this_moji_size + m_cur.moji_space.column();
			}
			else
			{
				m_cur.moji_pos.x += this_moji_size + m_cur.moji_space.column();
			}

			// ���ʂ̏ꍇ�͎����ŃC���f���g��ݒ�
			if (moji_type == TNM_MOJI_TYPE_MOJI && (moji_code == _T('�u') || moji_code == _T('�w') || moji_code == _T('�i')))
			{
				// �P�����ڂ̏ꍇ�͎��ۂɃC���f���g��ݒ�
				if (m_line_head_flag)	{
					set_indent(moji_code);
				}
				// ���݂̃C���f���g�����Ɠ��������̏ꍇ�̓C���f���g�J�E���^�𑝂₷
				else if (moji_code == m_indent_moji)	{
					m_indent_cnt ++;
				}
			}

			// �C���f���g���������̔���
			if (m_indent_cnt > 0)
			{
				if (moji_type == TNM_MOJI_TYPE_MOJI && m_indent_moji == _T('�u') && moji_code == _T('�v'))	{
					m_indent_cnt --;
				}
				if (moji_type == TNM_MOJI_TYPE_MOJI && m_indent_moji == _T('�w') && moji_code == _T('�x'))	{
					m_indent_cnt --;
				}
				if (moji_type == TNM_MOJI_TYPE_MOJI && m_indent_moji == _T('�i') && moji_code == _T('�j'))	{
					m_indent_cnt --;
				}

				// �C���f���g�J�E���g���O�ɂȂ�΃C���f���g����������
				if (m_indent_cnt == 0)	{
					clear_indent();
				}
			}

			// �{�^���Ƃ��ēo�^
			if (m_button_flag)
			{
				C_msg_btn btn;
				btn.m_button.create_guid();
				btn.m_btn_no = m_btn_no;
				btn.m_action_no = m_btn_action_no;
				btn.m_se_no = m_btn_se_no;
				m_msg_btn.insert(std::pair<int, C_msg_btn>((int)m_moji_list.size(), btn));
			}

			// �������X�g�ɒǉ�
			m_moji_list.push_back(moji);

			// �f�o�b�O�������X�g�ɒǉ�
			if (moji_type == TNM_MOJI_TYPE_MOJI)
			{
				if (moji_code != _T('\n') && moji_code != _T('\a'))
				{
					m_debug_msg.push_back(moji_code);		// ���b�Z�[�W�ɕ�����ۑ�
				}
			}
			else
			{
				m_debug_msg += TSTR(old_itr, itr);		// �G�����͓W�J�����ɕۑ�
			}

			// �s���t���O�����낷
			m_line_head_flag = false;
		}
	}
}

// ���b�Z�[�W�����邩���`�F�b�N
// �m�x�����[�h�ɂ����ĉ��y�[�W���s�����̔���Ɏg��
// new_line_flag: ���s�������Ĕ��肷��i�L�[�҂��A�C�R���^�y�[�W�҂��A�C�R���̔���Ɏg���j
bool C_elm_mwnd_msg::add_msg_check(int check_size, bool new_line_flag)
{
	if (Gp_ini->tateyoko_mode == 1)
	{
		int cur_x = m_cur.moji_pos.x;
		int cur_y = m_cur.moji_pos.y;

		// �s�ɓ��肫��Ȃ��ꍇ�͉��s
		if (cur_y + m_cur.moji_size > m_msg_size.cy + m_def.moji_size)	{
			cur_x -= m_cur.moji_size + m_cur.moji_space.row();
		}

		// ���s�����Ĕ��肷��
		if (new_line_flag)	{
			cur_x -= m_cur.moji_size + m_cur.moji_space.row();
		}

		// �s���𒴂����ꍇ�͓��肫��Ȃ��I�i���߂ɔ��肷��I�j
		if (cur_x <= - m_msg_size.cx + check_size)	{
			return false;
		}
	}
	else
	{
		int cur_x = m_cur.moji_pos.x;
		int cur_y = m_cur.moji_pos.y;

		// �s�ɓ��肫��Ȃ��ꍇ�͉��s
		if (cur_x + m_cur.moji_size > m_msg_size.cx + m_def.moji_size)	{
			cur_y += m_cur.moji_size + m_cur.moji_space.cy;
		}

		// ���s�����Ĕ��肷��
		if (new_line_flag)	{
			cur_y += m_cur.moji_size + m_cur.moji_space.cy;
		}

		// �s���𒴂����ꍇ�͓��肫��Ȃ��I�i���߂ɔ��肷��I�j
		if (cur_y >= m_msg_size.cy - check_size)	{
			return false;
		}
	}

	return true;
}

void C_elm_mwnd_msg::add_msg(CTSTR& msg, TSTR* ret_over_flow_msg)
{
	// �O��̃��b�Z�[�W�^�C�v�Ɣ�r���ĈႦ�΃X�y�[�X���󂯂�
	if (!m_cur_msg_type_decided)	{
		if (m_cur_msg_type == 1)	{	// ���O���胁�b�Z�[�W�������I
			m_cur.moji_pos.y += m_def.talk_margin.bottom;
		}
	}

	// ���b�Z�[�W��ǉ�
	add_msg_sub(msg, ret_over_flow_msg);

	// ���b�Z�[�W�^�C�v�F�g����
	if (!m_cur_msg_type_decided)	{
		m_cur_msg_type = 0;
		m_cur_msg_type_decided = true;
	}
}

void C_elm_mwnd_msg::add_name_to_msg(CTSTR& name, TSTR* ret_over_flow_msg)
{
	// �O��̃��b�Z�[�W�^�C�v�Ɣ�r���ĈႦ�΃X�y�[�X���󂯂�
	if (!m_cur_msg_type_decided)	{
		if (m_cur_msg_type == 0)	{	// ���O�Ȃ����b�Z�[�W�������I
			m_cur.moji_pos.y += m_def.talk_margin.top;
		}
	}

	// ���O�����b�Z�[�W�Ƃ��Ēǉ�
	int old_moji_size = m_cur.moji_size;
	m_cur.moji_size = m_cur.name_moji_size;
	add_msg_sub(name, ret_over_flow_msg);
	if (m_def.name_newline == 1)	{
		m_cur.moji_pos.x = m_def.talk_margin.left;
		set_indent(0);			// ���b�Z�[�W�̃C���f���g��ݒ肷��
		new_line_indent();		// ���O�̌�ɉ��s����
	}
	m_cur.moji_size = old_moji_size;

	// ���b�Z�[�W�^�C�v�F���O���胁�b�Z�[�W
	if (!m_cur_msg_type_decided)	{
		m_cur_msg_type = 1;
		m_cur_msg_type_decided = true;
	}
}

// ****************************************************************
// ���������擾
// ================================================================
int C_elm_mwnd_msg::get_moji_cnt()
{
	return (int)m_moji_list.size();
}
int C_elm_mwnd_msg::get_disp_moji_cnt()
{
	return m_disp_moji_cnt;
}
int C_elm_mwnd_msg::get_hide_moji_cnt()
{
	return m_hide_moji_cnt;
}

// ****************************************************************
// ���̂P�����\��
// ================================================================
void C_elm_mwnd_msg::disp_one_moji()
{
	if (m_disp_moji_cnt < (int)m_moji_list.size())	{
		m_moji_list[m_disp_moji_cnt].set_appear(true);
		m_disp_moji_cnt ++;		// �\������������

		// �������r�Ȃ炳��ɕ\������
		while (m_disp_moji_cnt < (int)m_moji_list.size() && m_moji_list[m_disp_moji_cnt].get_ruby_flag())	{
			m_moji_list[m_disp_moji_cnt].set_appear(true);
			m_disp_moji_cnt ++;		// �\������������
		}
	}
}

// ****************************************************************
// �S�Ă̕�����\��
// ================================================================
void C_elm_mwnd_msg::disp_all_moji()
{
	// �\�����Ă��Ȃ����������ɕ\������
	for (int i = m_disp_moji_cnt; i < (int)m_moji_list.size(); i++)	{
		m_moji_list[i].set_appear(true);
		m_disp_moji_cnt ++;
	}
}

// ****************************************************************
// �s�̐擪�ɂ���
// ================================================================
//		�C���f���g�����̔��肪���߂čs����悤�ɂȂ�܂��B
// ================================================================
void C_elm_mwnd_msg::set_line_head()
{
	m_line_head_flag = true;
}

// ****************************************************************
// ���s�i�C���f���g����j
// ================================================================
//		�K�����s���܂��B�C���f���g���s���܂��B
// ================================================================
void C_elm_mwnd_msg::new_line_indent()
{
	if (Gp_ini->tateyoko_mode == 1)
	{
		// �x���W���C���f���g�̈ʒu�ɐݒ�
		m_cur.moji_pos.y = m_indent_pos;
		// �w���W�͎��̍s��
		m_cur.moji_pos.x -= m_cur.moji_size + m_cur.moji_space.row();

		// ���b�Z�[�W�Ƃ��ĕۑ�
		m_debug_msg += _T("\n") + TSTR(_T("�@")) * ((m_cur.moji_pos.y - m_def.moji_pos.y) / m_def.moji_size);
	}
	else
	{
		// �w���W���C���f���g�̈ʒu�ɐݒ�
		m_cur.moji_pos.x = m_indent_pos;
		// �x���W�͎��̍s��
		m_cur.moji_pos.y += m_cur.moji_size + m_cur.moji_space.row();

		// ���b�Z�[�W�Ƃ��ĕۑ�
		m_debug_msg += _T("\n") + TSTR(_T("�@")) * ((m_cur.moji_pos.x - m_def.moji_pos.x) / m_def.moji_size);
	}
}

// ****************************************************************
// ���s�i�C���f���g�Ȃ��j
// ================================================================
//		�K�����s���܂��B�C���f���g���s���܂���B
// ================================================================
void C_elm_mwnd_msg::new_line_no_indent()
{
	clear_indent();		// �C���f���g�������Ă���
	new_line_indent();	// ���s����

	// ���s����Ȃ̂ōs���t���O�𗧂Ă܂��B
	m_line_head_flag = true;
}

// ****************************************************************
// ���s�i�擪�łȂ��ꍇ�̂݁j
// ================================================================
//		�I�������߂̒��O�ȂǂɎg���܂��B
//		���݈ʒu���擪�ɂȂ��ꍇ�̂݉��s���s���܂��B
//		�C���f���g���s���܂���B
// ================================================================
void C_elm_mwnd_msg::new_line_if_not_line_head()
{
	if (Gp_ini->tateyoko_mode == 1)
	{
		// �擪�łȂ��ꍇ
		if (m_cur.moji_pos.y != m_def.moji_pos.y)
			new_line_no_indent();
	}
	else
	{
		// �擪�łȂ��ꍇ
		if (m_cur.moji_pos.x != m_def.moji_pos.x)
			new_line_no_indent();
	}
}

// ****************************************************************
// �C���f���g��ݒ�
// ================================================================
void C_elm_mwnd_msg::set_indent(TCHAR moji)
{
	if (Gp_ini->tateyoko_mode == 1)
	{
		m_indent_pos = m_cur.moji_pos.y;
		m_indent_moji = moji;
		m_indent_cnt = 1;
	}
	else
	{
		m_indent_pos = m_cur.moji_pos.x;
		m_indent_moji = moji;
		m_indent_cnt = 1;
	}
}

// ****************************************************************
// �C���f���g���N���A
// ================================================================
void C_elm_mwnd_msg::clear_indent()
{
	m_indent_pos = 0;
	m_indent_moji = 0;
	m_indent_cnt = 0;
}

// ****************************************************************
// �����␳�ʒu��ݒ�
// ================================================================
void C_elm_mwnd_msg::set_moji_rep_pos(C_point pos)
{
	m_cur.moji_rep_pos = pos;
}
void C_elm_mwnd_msg::set_moji_rep_pos_default()
{
	m_cur.moji_rep_pos = m_def.moji_rep_pos;
}

// ****************************************************************
// �����T�C�Y��ݒ�
// ================================================================
void C_elm_mwnd_msg::set_moji_size(int size)
{
	m_cur.moji_size = size;
}
void C_elm_mwnd_msg::set_moji_size_default()
{
	m_cur.moji_size = m_def.moji_size;
}

// ****************************************************************
// �����F��ݒ�
// ================================================================
void C_elm_mwnd_msg::set_moji_color(int color_no)
{
	m_cur.moji_color = color_no;
}
void C_elm_mwnd_msg::set_moji_color_default()
{
	m_cur.moji_color = -1;		// �f�t�H���g�ɏ]��
}

// ****************************************************************
// �L�����N�^�����F���N���A
// ================================================================
void C_elm_mwnd_msg::clear_chara_moji_color()
{
	m_chara_moji_color_no = -1;
}
void C_elm_mwnd_msg::clear_chara_shadow_color()
{
	m_chara_shadow_color_no = -1;
}
void C_elm_mwnd_msg::clear_chara_fuchi_color()
{
	m_chara_fuchi_color_no = -1;
}

// ****************************************************************
// �L�����N�^�����F��ݒ�
// ================================================================
void C_elm_mwnd_msg::set_chara_moji_color(int color_no)
{
	m_chara_moji_color_no = color_no;
}
void C_elm_mwnd_msg::set_chara_shadow_color(int color_no)
{
	m_chara_shadow_color_no = color_no;
}
void C_elm_mwnd_msg::set_chara_fuchi_color(int color_no)
{
	m_chara_fuchi_color_no = color_no;
}

// ****************************************************************
// ���݂̕����ʒu���擾
// ================================================================
C_point C_elm_mwnd_msg::get_cur_moji_pos()
{
	return m_cur.moji_pos;
}

// ****************************************************************
// ���݂̕����ʒu��ݒ�
// ================================================================
void C_elm_mwnd_msg::set_cur_moji_pos(C_point pos)
{
	m_cur.moji_pos = pos;
}

// ****************************************************************
// �{�^�����J�n
// ================================================================
void C_elm_mwnd_msg::start_button(int btn_no, int group_no, int action_no, int se_no)
{
	m_button_flag = true;
	m_btn_no = btn_no;
	m_btn_group_no = group_no;
	m_btn_action_no = action_no;
	m_btn_se_no = se_no;
}

// ****************************************************************
// �{�^�����I��
// ================================================================
void C_elm_mwnd_msg::end_button()
{
	m_button_flag = false;
}

// ****************************************************************
// ���r���J�n
// ================================================================
void C_elm_mwnd_msg::start_ruby(CTSTR& str)
{
	m_ruby = str;

	m_ruby_start_ready = true;
}

// ****************************************************************
// ���r���I��
// ================================================================
void C_elm_mwnd_msg::end_ruby()
{
	// ���r���������烋�r�𐶐��I
	if (!m_ruby.empty())
	{
		// ���r�J�n�������������܂܂̏ꍇ
		if (m_ruby_start_ready)
		{
			tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("���r�̑Ώۂ̕����񂪋�ł��B"));
		}
		else
		{
			C_point ruby_start_pos = m_ruby_start_pos;
			C_point ruby_end_pos = m_cur.moji_pos;

			// ���r�͂����c�����^�������Ŋ��S�ɕ������܂�
			if (Gp_ini->tateyoko_mode == 1)
			{
				// ���r�����s�ɂ܂������Ă����ꍇ
				if (ruby_start_pos.x != ruby_end_pos.x)
				{
					tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("���r�̓r���ŉ��s����Ă��܂��B���r�͕\������܂���B"));
				}
				else
				{
					// ���r���ϓ����肷��I
					int msg_height = ruby_end_pos.y - ruby_start_pos.y;
					int ruby_moji_cnt = (int)m_ruby.size();
					int ruby_moji_height = m_cur.ruby_size;
					int ruby_moji_space_y = (msg_height - (ruby_moji_height * ruby_moji_cnt)) / (ruby_moji_cnt + 1);

					// ���r�̈ʒu��ݒ肷��
					C_point pos = ruby_start_pos + C_point(m_cur.ruby_size + m_cur.ruby_space, ruby_moji_space_y);

					// �����X�y�[�X�� 0 �ȉ��ɂȂ�����␳����
					if (ruby_moji_space_y < 0)
					{
						ruby_moji_space_y = 0;
						int ruby_total_height = ruby_moji_cnt * ruby_moji_height;
						pos.y = ruby_start_pos.y + (msg_height / 2) - (ruby_moji_height / 2);

						// ��[���z�������l��
						if (pos.y < 0)
							pos.y = ruby_start_pos.y;

						// ���[���z�����牺�l��
						if (pos.y + ruby_total_height >= m_msg_size.cy + m_def.moji_size)
							pos.y = ruby_start_pos.y + msg_height - ruby_total_height;
					}

					m_debug_msg += _T(" [");
					for (TSTR::const_iterator itr = m_ruby.begin(); itr != m_ruby.end(); ++itr)
					{
						TCHAR moji_code = *itr;

						// �������쐬
						C_elm_mwnd_moji moji;
						int hankaku_rep_y = is_hankaku(moji_code) ? ruby_moji_height / 4 : 0;	// ���p�Ȃ�Z���^�����O
						int moji_color_no = (m_cur.moji_color != -1 ? m_cur.moji_color : m_chara_moji_color_no != -1 ? m_chara_moji_color_no : m_def.moji_color);
						int shadow_color_no = (m_cur.shadow_color != -1 ? m_cur.shadow_color : m_chara_shadow_color_no != -1 ? m_chara_shadow_color_no : m_def.shadow_color);
						int fuchi_color_no = (m_cur.fuchi_color != -1 ? m_cur.fuchi_color : m_chara_fuchi_color_no != -1 ? m_chara_fuchi_color_no : m_def.fuchi_color);
						moji.set_moji(TNM_MOJI_TYPE_MOJI, moji_code, ruby_moji_height, moji_color_no, shadow_color_no, fuchi_color_no, pos + C_point(0, hankaku_rep_y));
						moji.set_ruby_flag(true);

						// �������X�g�ɒǉ�
						m_moji_list.push_back(moji);
						m_debug_msg.push_back(moji_code);

						// ���̈ʒu��
						pos += C_point(0, ruby_moji_height + ruby_moji_space_y);
					}
					m_debug_msg += _T("] ");
				}
			}
			else
			{
				// ���r�����s�ɂ܂������Ă����ꍇ
				if (ruby_start_pos.y != ruby_end_pos.y)
				{
					tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("���r�̓r���ŉ��s����Ă��܂��B���r�͕\������܂���B"));
				}
				else
				{
					// ���r���ϓ����肷��I
					int msg_width = ruby_end_pos.x - ruby_start_pos.x;
					int ruby_moji_cnt = (int)m_ruby.size();
					int ruby_moji_width = m_cur.ruby_size;
					int ruby_moji_space_x = (msg_width - (ruby_moji_width * ruby_moji_cnt)) / (ruby_moji_cnt + 1);

					// ���r�̈ʒu��ݒ肷��
					C_point pos = ruby_start_pos + C_point(ruby_moji_space_x, -m_cur.ruby_size - m_cur.ruby_space);

					// �����X�y�[�X�� 0 �ȉ��ɂȂ�����␳����
					if (ruby_moji_space_x < 0)
					{
						ruby_moji_space_x = 0;
						int ruby_total_width = ruby_moji_cnt * ruby_moji_width;
						pos.x = ruby_start_pos.x + (msg_width / 2) - (ruby_total_width / 2);

						// ���[���z�����獶�l��
						if (pos.x < 0)
							pos.x = ruby_start_pos.x;

						// �E�[���z������E�l��
						if (pos.x + ruby_total_width >= m_msg_size.cx + m_def.moji_size)
							pos.x = ruby_start_pos.x + msg_width - ruby_total_width;
					}

					m_debug_msg += _T(" [");
					for (TSTR::const_iterator itr = m_ruby.begin(); itr != m_ruby.end(); ++itr)
					{
						TCHAR moji_code = *itr;

						// �������쐬
						C_elm_mwnd_moji moji;
						int hankaku_rep_x = is_hankaku(moji_code) ? ruby_moji_width / 4 : 0;	// ���p�Ȃ�Z���^�����O
						int moji_color_no = (m_cur.moji_color != -1 ? m_cur.moji_color : m_chara_moji_color_no != -1 ? m_chara_moji_color_no : m_def.moji_color);
						int shadow_color_no = (m_cur.shadow_color != -1 ? m_cur.shadow_color : m_chara_shadow_color_no != -1 ? m_chara_shadow_color_no : m_def.shadow_color);
						int fuchi_color_no = (m_cur.fuchi_color != -1 ? m_cur.fuchi_color : m_chara_fuchi_color_no != -1 ? m_chara_fuchi_color_no : m_def.fuchi_color);
						moji.set_moji(TNM_MOJI_TYPE_MOJI, moji_code, ruby_moji_width, moji_color_no, shadow_color_no, fuchi_color_no, pos + C_point(hankaku_rep_x, 0));
						moji.set_ruby_flag(true);

						// �������X�g�ɒǉ�
						m_moji_list.push_back(moji);
						m_debug_msg.push_back(moji_code);

						// ���̈ʒu��
						pos += C_point(ruby_moji_width + ruby_moji_space_x, 0);
					}
					m_debug_msg += _T("] ");
				}
			}
		}
	}

	// ���r���N���A
	m_ruby.clear();

	m_ruby_start_pos = C_point(0, 0);
}

// ****************************************************************
// �X���C�h���b�Z�[�W
// ================================================================
void C_elm_mwnd_msg::slide_msg()
{
	int slide_pixel = m_def.moji_size + m_def.moji_space.cy;

	// ���ɕ\�������S���������炷
	int moji_cnt = m_moji_list.size();
	for (int i = 0; i < moji_cnt; i++)
	{
		C_point moji_pos = m_moji_list[i].get_moji_pos();
		m_moji_list[i].set_moji_pos(moji_pos - C_point(0, slide_pixel));
	}

	// ���݂̕����ʒu�����炷
	m_cur.moji_pos.y -= slide_pixel;
}

// ****************************************************************
// �܂��o�����Ă��Ȃ����������݂��邩���肷��
// ================================================================
bool C_elm_mwnd_msg::check_not_appear_moji()
{
	return (int)m_moji_list.size() > m_disp_moji_cnt;
}

// ****************************************************************
// ���b�Z�[�W�T�C�Y�̎擾
// ================================================================
C_size C_elm_mwnd_msg::get_msg_size()
{
	return m_msg_size;
}

// ****************************************************************
// �\���������b�Z�[�W�̍��������߂�
// ================================================================
//		���b�Z�[�W�o�b�N�̍ہA
//		�ǂ��܂ŕ��͂�\�����邩�𔻒肷��̂Ɏg���܂��B
// ================================================================
int C_elm_mwnd_msg::get_msg_height()
{
	if (Gp_ini->tateyoko_mode == 1)
	{
		return -(m_cur.moji_pos.x + m_def.moji_space.cx);
	}
	else
	{
		return m_cur.moji_pos.y - m_def.moji_space.cy;
	}
}

// ****************************************************************
// �f�o�b�O���b�Z�[�W�̎擾
// ================================================================
TSTR C_elm_mwnd_msg::get_debug_msg()
{
	return m_debug_msg;
}

// ****************************************************************
// �p�����[�^�����F���������������^�擾�^�ݒ�
// ================================================================
void C_elm_mwnd_msg::init_moji_cnt()
{
	m_cur.moji_cnt = m_def.moji_cnt;
}
void C_elm_mwnd_msg::set_moji_cnt(C_size moji_cnt)
{
	m_cur.moji_cnt = moji_cnt;

	// ���b�Z�[�W�̈��傫���v�Z����i�����������͌��݂̒l���g���j
	m_msg_size.cx = (m_def.moji_size * m_cur.moji_cnt.cx) + (m_def.moji_space.cx * (m_cur.moji_cnt.cx - 1));
	m_msg_size.cy = (m_def.moji_size * m_cur.moji_cnt.cy) + (m_def.moji_space.cy * (m_cur.moji_cnt.cy - 1));
}


