#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_input.h"
#include	"data/tnm_save_00.h"

#include	"element/elm_mwnd_sel.h"
#include	"element/elm_mwnd_moji.h"
#include	"element/elm_group_def.h"
#include	"element/elm_sound.h"

#include	"engine/eng_input.h"
#include	"engine/ifc_sound.h"
#include	"engine/ifc_stack.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_scene.h"


// ****************************************************************
// ������
// ================================================================
void C_elm_mwnd_select::init()
{
	// �e���v���[�g�ԍ�
	m_template_no = -1;

	// �e���v���[�g�̏�����
	m_def.moji_cnt = C_size(0, 0);
	m_def.moji_pos = C_point(0, 0);
	m_def.moji_size = 0;
	m_def.moji_space = C_size(0, 0);
	m_def.moji_color = 0;
	m_def.shadow_color = 0;
	m_def.fuchi_color = 0;

	// �����ϐ�������
	init_work_variable();
}

// ****************************************************************
// �����ϐ�������
// ================================================================
void C_elm_mwnd_select::init_work_variable()
{
	m_cur = m_def;		// �f�t�H���g�p�����[�^�����݂̃p�����[�^�փR�s�[����
	m_item_list.clear();
	m_disp_item_cnt = 0;
	m_cancel_enable_flag = false;
}

// ****************************************************************
// �e���v���[�g�̒l��ݒ�
// ================================================================
void C_elm_mwnd_select::set_template(int template_no)
{
	if (template_no == m_template_no)
		return;

	// �e���v���[�g�ԍ�
	m_template_no = template_no;

	// �e���v���[�g���č\�z
	restruct_template();

	// �����ϐ�������
	init_work_variable();
}

// ****************************************************************
// �e���v���[�g���č\�z
// ================================================================
void C_elm_mwnd_select::restruct_template()
{
	m_def.moji_cnt = Gp_ini->mwnd.sub[m_template_no].moji_cnt;
	m_def.moji_pos = C_point(0, 0);
	m_def.moji_size = Gp_ini->mwnd.sub[m_template_no].moji_size;
	m_def.moji_space = Gp_ini->mwnd.sub[m_template_no].moji_space;
	m_def.moji_color = Gp_ini->mwnd.sub[m_template_no].moji_color >= 0 ? Gp_ini->mwnd.sub[m_template_no].moji_color : Gp_ini->mwnd.moji_color;
	m_def.shadow_color = Gp_ini->mwnd.sub[m_template_no].shadow_color >= 0 ? Gp_ini->mwnd.sub[m_template_no].shadow_color : Gp_ini->mwnd.shadow_color;
	m_def.fuchi_color = Gp_ini->mwnd.sub[m_template_no].fuchi_color >= 0 ? Gp_ini->mwnd.sub[m_template_no].fuchi_color : Gp_ini->mwnd.fuchi_color;
}

// ****************************************************************
// �Z�[�u
// ================================================================
void C_elm_mwnd_select::save(C_tnm_save_stream& stream)
{
	// �e���v���[�g�ԍ�
	stream.save(m_template_no);

	// ���݂̃p�����[�^
	stream.save(m_cur);
	stream.save(m_disp_item_cnt);
	stream.save(m_cancel_enable_flag);

	// �A�C�e��
	stream.save<size_t>(m_item_list.size());
	for (int i = 0; i < (int)m_item_list.size(); i++)
		m_item_list[i].save(stream);
}

// ****************************************************************
// ���[�h
// ================================================================
void C_elm_mwnd_select::load(C_tnm_save_stream& stream)
{
	// �e���v���[�g�ԍ�
	stream.load(m_template_no);

	// �e���v���[�g���č\�z
	restruct_template();

	// ���݂̃p�����[�^
	stream.load(m_cur);
	stream.load(m_disp_item_cnt);
	stream.load(m_cancel_enable_flag);

	// �A�C�e��
	m_item_list.resize(stream.load_ret<size_t>());
	for (int i = 0; i < (int)m_item_list.size(); i++)	{
		m_item_list[i].init();	// �������I
		m_item_list[i].load(stream);
	}
}

// ****************************************************************
// �R�s�[
// ================================================================
void C_elm_mwnd_select::copy(C_elm_mwnd_select* src)
{
	// �e���v���[�g�ԍ�
	m_template_no = src->m_template_no;
		
	// �e���v���[�g���č\�z
	restruct_template();

	// ���݂̃p�����[�^
	m_cur = src->m_cur;
	m_disp_item_cnt = src->m_disp_item_cnt;
	m_cancel_enable_flag = src->m_cancel_enable_flag;
	
	// �A�C�e��
	m_item_list.resize(src->m_item_list.size());
	for (int i = 0; i < (int)m_item_list.size(); i++)
		m_item_list[i].copy(&src->m_item_list[i]);
}

// ****************************************************************
// �t���[������������
// ================================================================
void C_elm_mwnd_select::frame_init()
{
	// �A�C�e��
	for (int i = 0; i < (int)m_item_list.size(); i++)
		m_item_list[i].frame_init();
}

// ****************************************************************
// �t���[������
// ================================================================
void C_elm_mwnd_select::frame(const S_tnm_render_param* parent_trp, C_point pos_rep, bool draw_futoku, bool draw_shadow)
{
	// �A�C�e��
	for (int i = 0; i < (int)m_item_list.size(); i++)
		m_item_list[i].frame(parent_trp, pos_rep, 0, draw_futoku, draw_shadow);
}

// ****************************************************************
// �X�v���C�g�c���[���\�z
// ================================================================
void C_elm_mwnd_select::get_sprite_tree(C_tree2<C_tnm_sprite *>& root)
{
	// �A�C�e��
	for (int i = 0; i < (int)m_item_list.size(); i++)
		m_item_list[i].get_sprite_tree(root);
}

// ****************************************************************
// �{�^����o�^
// ================================================================
void C_elm_mwnd_select::regist_button()
{
	// �A�C�e��
	for (int i = 0; i < (int)m_item_list.size(); i++)
		m_item_list[i].regist_button();
}

// ****************************************************************
// �{�^���C�x���g����
// ================================================================
void C_elm_mwnd_select::button_event(bool wait_flag)
{
	int sel_no = TNM_GROUP_NOT_DECIDED;

	// �L�����Z�����ꂽ�ꍇ
	if (m_cancel_enable_flag && tnm_input_use_key_down_up(VK_EX_CANCEL))
	{
		sel_no = TNM_GROUP_CANCELED;
	}
	else
	{
		// �A�C�e��
		for (int i = 0; i < (int)m_item_list.size(); i++)
		{
			if (m_item_list[i].button_event(wait_flag))
			{
				sel_no = i;
			}
		}
	}

	// ����܂��̓L�����Z�����ꂽ�I
	if (sel_no == TNM_GROUP_CANCELED || sel_no >= 0)
	{
		// ���ʉ�
		if (sel_no == TNM_GROUP_CANCELED)
		{
			tnm_play_se(TNM_SE_TYPE_CANCEL);

			Gp_global->last_sel_msg = _T("�i�L�����Z���j");

		}
		else
		{
			tnm_play_se(TNM_SE_TYPE_DECIDE);

			Gp_global->last_sel_msg = m_item_list[sel_no].get_msg();
		}

		// ����҂��̏ꍇ
		if (wait_flag)
		{
			tnm_stack_push_int(sel_no);	// ���ʂ��X�^�b�N�ɐς�
			tnm_pop_proc();				// ���̃v���Z�X��
		}

		// �I�����|�C���g�𗧂Ă�
		tnm_set_sel_point();
	}
}

// ****************************************************************
// �󂩂ǂ�������
// ================================================================
bool C_elm_mwnd_select::is_empty()
{
	return m_item_list.empty();
}

// ****************************************************************
// �I�����̃N���A
// ================================================================
void C_elm_mwnd_select::clear_sel()
{
	m_cur.moji_pos = C_point(0, 0);
	m_item_list.clear();
	m_disp_item_cnt = 0;
	m_cancel_enable_flag = false;
}

// ****************************************************************
// �I�����̏����ʒu��ݒ肷��
// ================================================================
void C_elm_mwnd_select::set_def_pos(C_point pos)
{
	m_def.moji_pos = pos;
	m_cur.moji_pos = pos;
}

// ****************************************************************
// �I������ǉ�
// ================================================================
void C_elm_mwnd_select::add_sel(CTSTR& msg, int item_type, int color)
{
	// �����ŐF��ϊ�
	color = (color >= 0) ? color : m_def.moji_color;

	// �A�C�e�����쐬
	C_elm_mwnd_select_item item;
	item.init();	// �������I
	item.set_msg(msg, item_type, m_cur.moji_size, m_cur.moji_space, color, m_cur.shadow_color, m_cur.fuchi_color);

	// �A�C�e�����X�g�ɒǉ�
	m_item_list.push_back(item);
}

// ****************************************************************
// �L�����Z���\�t���O��ݒ�
// ================================================================
void C_elm_mwnd_select::set_cancel_enable(bool enable)
{
	m_cancel_enable_flag = enable;
}

// ****************************************************************
// �I�������č\�z
// ================================================================
void C_elm_mwnd_select::restruct_item(int extend_type)
{
	// ================================================================
	// �Œ�g�̏ꍇ
	// ----------------------------------------------------------------
	//		�������ĕ��ׂ܂��B
	// ----------------------------------------------------------------
	//		�P�D������			�S�D����
	//		�Q�D������������	�T�D��������
	//		�R�D��������
	// ----------------------------------------------------------------
	if (extend_type == 0)	{

		// �e�L�X�g��`���v�Z
		C_size text_area_size;
		text_area_size.cx = (m_def.moji_size * m_def.moji_cnt.cx) + (m_def.moji_space.cx * (m_def.moji_cnt.cx - 1));
		text_area_size.cy = (m_def.moji_size * m_def.moji_cnt.cy) + (m_def.moji_space.cy * (m_def.moji_cnt.cy - 1));

		// �I�����̐�
		int item_cnt = (int)m_item_list.size();

		// �c�ɕ��ԍ��ڐ��̌v�Z
		int line_cnt = (text_area_size.cy - m_cur.moji_pos.y + m_cur.moji_space.cy) / (m_cur.moji_size + m_cur.moji_space.cy);
		line_cnt = std::max(1, line_cnt);

		// ���ɕ��ԍ��ڐ��̌v�Z
		int column_cnt = (item_cnt - 1) / line_cnt + 1;

		// �P�̑I�����̑傫���̌v�Z
		int sel_width = text_area_size.cx / column_cnt;
		int sel_height = m_cur.moji_size + m_cur.moji_space.cy;

		// �I����`�ƕ\�����W���Đݒ�
		int pos_no = 0;
		for (int i = 0; i < (int)m_item_list.size(); i++)	{

			// type == off �̏ꍇ�͉������Ȃ�
			if (m_item_list[i].get_item_type() != TNM_SEL_ITEM_TYPE_OFF)	{
				int sel_rect_x = pos_no / line_cnt * sel_width;
				int sel_rect_y = pos_no % line_cnt * sel_height + m_def.moji_pos.y;
				m_item_list[i].set_pos(C_point(sel_rect_x, sel_rect_y));
				m_item_list[i].set_hit_rect(C_rect::by_size(sel_rect_x, sel_rect_y, sel_width, sel_height));
					// ��U�A�e�L�X�g���W�œ����蔻��̈���v�Z���܂��B

				// ���̈ʒu��
				pos_no ++;
			}
		}
	}

	// ================================================================
	// �Ϙg�̏ꍇ
	// ----------------------------------------------------------------
	// �c�ɂ���[���ƕ��ׂ܂��B�S�̋�`���v�Z���܂��B
	// ----------------------------------------------------------------
	else if (extend_type == 1)	{

		// �I�����̐�
		int item_cnt = (int)m_item_list.size();

		// ���b�Z�[�W�T�C�Y���v�Z����i�Ϙg�̃T�C�Y�����Ɏg���܂��j
		m_msg_size = C_size(0, m_def.moji_pos.y - m_cur.moji_space.cy);
		for (int i = 0; i < item_cnt; i++)	{

			// type == off �̏ꍇ�͉������Ȃ�
			if (m_item_list[i].get_item_type() != TNM_SEL_ITEM_TYPE_OFF)	{
				m_msg_size.cx = std::max(m_msg_size.cx, m_item_list[i].get_msg_size().cx);
				m_msg_size.cy += m_item_list[i].get_msg_size().cy + m_cur.moji_space.cy;
			}
		}

		// �I����`�ƕ\�����W���Đݒ�
		int offset_y = m_def.moji_pos.y;
		for (int i = 0; i < (int)m_item_list.size(); i++)	{

			// type == off �̏ꍇ�͉������Ȃ�
			if (m_item_list[i].get_item_type() != TNM_SEL_ITEM_TYPE_OFF)	{

				// ��: �ő�̃e�L�X�g���A����: ���̃A�C�e���̍����{�X�y�[�X
				m_item_list[i].set_pos(C_point(0, offset_y));
				m_item_list[i].set_hit_rect(C_rect::by_size(0, offset_y, m_msg_size.cx, m_item_list[i].get_msg_size().cy + m_cur.moji_space.cy));
				offset_y += m_item_list[i].get_msg_size().cy + m_cur.moji_space.cy;
			}
		}
	}
}

// ****************************************************************
// �I������\��
// ================================================================
void C_elm_mwnd_select::set_disp()
{
	for (ARRAY<C_elm_mwnd_select_item>::iterator itr = m_item_list.begin(); itr != m_item_list.end(); ++itr)	{
		itr->set_disp();
	}

	m_disp_item_cnt = (int)m_item_list.size();
}

// ****************************************************************
// ���b�Z�[�W���擾
// ================================================================
TSTR C_elm_mwnd_select::get_msg()
{
	return m_msg;
}

// ****************************************************************
// ���b�Z�[�W�T�C�Y���擾
// ================================================================
C_size C_elm_mwnd_select::get_msg_size()
{
	return m_msg_size;
}

// ****************************************************************
// �܂��o�����Ă��Ȃ����������݂��邩���肷��
// ================================================================
bool C_elm_mwnd_select::check_not_appear_moji()
{
	return (int)m_item_list.size() > m_disp_item_cnt;
}
