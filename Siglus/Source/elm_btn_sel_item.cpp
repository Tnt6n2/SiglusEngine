#include	"pch.h"
#include	"ifc_eng.h"
#include	"tnm_element_code.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_color_table.h"
#include	"data/tnm_btn_template.h"
#include	"element/elm_group_def.h"
#include	"element/elm_btn_sel_item.h"
#include	"element/elm_mwnd_moji.h"
#include	"element/elm_sound.h"
#include	"engine/eng_config.h"
#include	"engine/ifc_sound.h"
#include	"engine/eng_graphics.h"
#include	"engine/eng_message.h"
#include	"engine/ifc_error.h"

// ****************************************************************
// ������
// ================================================================
void C_elm_btn_select_item::init(S_element element, CTSTR& name)
{
	// ���N���X�̏�����
	C_tnm_element::init(element, FM_BTNSELITEM, name, NULL);

	// �I�u�W�F�N�g
	m_object_list.init(element + S_element(ELM_BTNSELITEM_OBJECT), name + _T(".object"), 0, true, false, false, false, NULL, S_element(), S_element());

	// �p�����[�^��������
	m_template_no = -1;
	m_base_file.clear();
	m_filter_file.clear();

	m_moji_list.clear();
	m_button.init();

	m_album_base.reset();
	m_album_filter.reset();
	m_spr_base.init();
	m_spr_filter.init();

	// �����ϐ�������
	init_work_variable();
}

// ****************************************************************
// ������
// ================================================================
void C_elm_btn_select_item::reinit(bool restruct_flag)
{
	// �I�u�W�F�N�g
	m_object_list.reinit(restruct_flag);
}

// ****************************************************************
// �e���v���[�g�̒l��ݒ�
// ================================================================
void C_elm_btn_select_item::set_template(int template_no)
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
void C_elm_btn_select_item::restruct_template()
{
	// �X�v���C�g���N���A
	m_album_base.reset();
	m_album_filter.reset();
	m_spr_base.init();
	m_spr_filter.init();

	// �e���v���[�g�Ȃ�
	if (m_template_no < 0)
		return;

	// �x�[�X�t�@�C��
	m_base_file = Gp_ini->sel_btn[m_template_no].base_file;

	// �t�@�C�����̎w�肪����ꍇ�A�t�@�C����ǂݍ���
	if (!m_base_file.empty())	{

		// �E�B���h�E�̃X�v���C�g���쐬
		m_spr_base.create_polygon_no_init();
		m_spr_base.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
		m_spr_base.set_index_buffer(G_rect_index_buffer);

		// �e�N�X�`����ǂݍ���
		TSTR base_file_path = m_base_file;
		m_album_base = tnm_load_pct_d3d(base_file_path, false, false);
		if (!m_album_base)	{	// �G���[�I
			tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, base_file_path + _T(" ���J���܂���ł����B"));
		}
		else	{
			m_spr_base.set_texture(m_album_base->get_texture(0));
		}

		// �A�C�e���T�C�Y
		if (m_album_base->get_texture(0))
			m_item_size = m_album_base->get_texture(0)->get_size();
	}
	else	{

		// �E�B���h�E�̃X�v���C�g���쐬
		m_spr_base.create_polygon_no_init();
		m_spr_base.set_vertex_buffer(G_rect_vertex_buffer_d2_filter);
		m_spr_base.set_index_buffer(G_rect_index_buffer);
	}

	// �t�B���^�[�t�@�C��
	m_filter_file = Gp_ini->sel_btn[m_template_no].filter_file;

	// �t�@�C�����̎w�肪����ꍇ�A�t�@�C����ǂݍ���
	if (!m_filter_file.empty())	{

		// �t�B���^�[�̃X�v���C�g���쐬
		m_spr_filter.create_polygon_no_init();
		m_spr_filter.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
		m_spr_filter.set_index_buffer(G_rect_index_buffer);

		// �e�N�X�`����ǂݍ���
		TSTR filter_file_path = m_filter_file;
		m_album_filter = tnm_load_pct_d3d(filter_file_path, false, false);
		if (m_album_filter)
			m_spr_filter.set_texture(m_album_filter->get_texture(0));
	}
	else	{

		// �t�B���^�[�̃X�v���C�g���쐬
		m_spr_filter.create_polygon_no_init();
		m_spr_filter.set_vertex_buffer(G_rect_vertex_buffer_d2_filter);
		m_spr_filter.set_index_buffer(G_rect_index_buffer);
	}
}

// ****************************************************************
// �{�^���I�����A�C�e���F�����ϐ�������
// ================================================================
void C_elm_btn_select_item::init_work_variable()
{
	m_msg.clear();

	m_param.item_type = TNM_SEL_ITEM_TYPE_ON;
}

// ****************************************************************
// �{�^���I�����A�C�e���F�I��
// ================================================================
void C_elm_btn_select_item::finish()
{
	// �I�u�W�F�N�g
	m_object_list.finish();
}

// ****************************************************************
// �{�^���I�����A�C�e���F�Z�[�u
// ================================================================
void C_elm_btn_select_item::save(C_tnm_save_stream& stream)
{
	// �p�����[�^
	stream.save(m_template_no);
	stream.save(m_base_file);
	stream.save(m_filter_file);
	stream.save(m_msg);
	stream.save(m_param);
	stream.save(m_pos);

	// ����
	stream.save(m_moji_list);
}

// ****************************************************************
// ���[�h
// ================================================================
void C_elm_btn_select_item::load(C_tnm_save_stream& stream)
{
	// �p�����[�^
	stream.load(m_template_no);
	stream.load(m_base_file);
	stream.load(m_filter_file);
	stream.load(m_msg);
	stream.load(m_param);
	stream.load(m_pos);

	// ����
	stream.load(m_moji_list);
}

// ****************************************************************
// �R�s�[
// ================================================================
void C_elm_btn_select_item::copy(C_elm_btn_select_item* src)
{
	// �e���v���[�g�ԍ�
	m_template_no = src->m_template_no;

	// �e���v���[�g�̍č\�z
	restruct_template();

	// ���݂̃p�����[�^
	m_msg = src->m_msg;
	m_param = src->m_param;
	m_pos = src->m_pos;
	
	// ����
	m_moji_list.resize(src->m_moji_list.size());
	for (int i = 0; i < (int)m_moji_list.size(); i++)
		m_moji_list[i] = src->m_moji_list[i];

	// �I�u�W�F�N�g
	m_object_list.resize(src->m_object_list.get_size());
	for (int i = 0; i < m_object_list.get_size(); i++)
		m_object_list[i].copy(&src->m_object_list[i], false);
}

// ****************************************************************
// �t���[��������
// ================================================================
void C_elm_btn_select_item::frame_init()
{
	// �X�v���C�g���t���[��������
	m_spr_base.rp.init();
	m_spr_filter.rp.init();

	// �{�^�����t���[��������
	m_button.frame_init();

	// �������t���[��������
	for (int i = 0; i < (int)m_moji_list.size(); i++)
		m_moji_list[i].frame_init();

	// �I�u�W�F�N�g
	for (int i = 0; i < m_object_list.get_size(); i++)
		m_object_list[i].frame_init();
}

// ****************************************************************
// ���Ԃ��X�V
// ================================================================
void C_elm_btn_select_item::update_time(int past_game_time, int past_real_time)
{
	// �I�u�W�F�N�g
	for (int i = 0; i < m_object_list.get_size(); i++)
		m_object_list[i].update_time(past_game_time, past_real_time, 0);
}

// ****************************************************************
// �t���[������
// ================================================================
void C_elm_btn_select_item::frame(C_point window_pos, const S_tnm_render_param* parent_trp, int hit_color, int action_template_no, bool draw_futoku, bool draw_shadow)
{
	if (m_param.item_type == TNM_SEL_ITEM_TYPE_ON || m_param.item_type == TNM_SEL_ITEM_TYPE_READ)	{

		C_rect hit_rect;

		// �{�^���A�N�V�����ɂ��␳�p�����[�^�i�����蔻��ɂ͉e�����Ȃ��j
		S_tnm_btn_action_template_pat btn_action;
		int btn_state = Gp_btn_mng->get_hit_state(&m_button);
		if (m_param.item_type == TNM_SEL_ITEM_TYPE_READ)	{
			btn_state = TNM_BTN_STATE_DISABLE;
		}

		// �{�^���A�N�V�����ɂ��␳���v�Z����
		if (0 <= action_template_no && action_template_no < (int)Gp_btn_action_list->m_sub.size())	{
			C_tnm_btn_action_template* p_action_templete = &Gp_btn_action_list->m_sub[action_template_no];
			S_tnm_btn_action_template_pat* p_action = p_action_templete->get_action(btn_state);
			if (p_action)	{
				btn_action = *p_action;
			}
		}

		// �x�[�X�X�v���C�g�̃p�����[�^
		if (m_spr_base.get_texture())	{

			S_tnm_render_param base_trp;
			base_trp.disp = true;
			base_trp.pat_no = btn_action.rep_pat_no;
			base_trp.sorter = S_tnm_sorter(0, Gp_ini->mwnd.waku_layer_rep);
			base_trp.pos_x = (float)(window_pos.x + m_pos.x + btn_action.rep_pos.x);
			base_trp.pos_y = (float)(window_pos.y + m_pos.y + btn_action.rep_pos.y);
			base_trp.tr = btn_action.rep_tr;
			base_trp.bright = btn_action.rep_bright;
			base_trp.dark = btn_action.rep_dark;

			// �e�̃����_�[�p�����[�^��������
			tnm_add_parent_trp(&base_trp, parent_trp);

			// �x�[�X�X�v���C�g�̃����_�[�p�����[�^
			tnm_trp_to_rp(&base_trp, &m_spr_base.rpex, &m_spr_base.rp);
			m_spr_base.rp.d2_rect = true;

			// �p�^�[���ԍ��ɂ���ăe�N�X�`����ݒ�
			if (m_album_base && m_album_base->get_texture(base_trp.pat_no))	{
				m_spr_base.set_texture(m_album_base->get_texture(base_trp.pat_no));
				m_spr_base.rp.size_fit_to_texture = 0;
			}

			// �������`�̓E�B���h�E���
			if (m_spr_base.get_texture())	{
				C_point total_pos;
				total_pos.x = (int)(m_spr_base.rp.pos.x - m_spr_base.rp.center.x) - m_spr_base.get_texture()->get_center().x;
				total_pos.y = (int)(m_spr_base.rp.pos.y - m_spr_base.rp.center.y) - m_spr_base.get_texture()->get_center().y;
				total_pos -= btn_action.rep_pos;		// �{�^���A�N�V�����������ɖ߂�
				m_button.set_param(C_rect::by_size(total_pos, m_spr_base.get_texture()->get_size()), base_trp.sorter);
			}
		}

		// �t�B���^�̃X�v���C�g�p�����[�^
		if (m_spr_filter.get_texture())	{

			S_tnm_render_param filter_trp;
			filter_trp.disp = true;
			filter_trp.pos_x = (float)(window_pos.x + m_pos.x + btn_action.rep_pos.x);
			filter_trp.pos_y = (float)(window_pos.y + m_pos.y + btn_action.rep_pos.y);
			filter_trp.bright = btn_action.rep_bright;
			filter_trp.dark = btn_action.rep_dark;
			filter_trp.tr = Gp_config->filter_color.a;				// �����x
			filter_trp.color_add_b = Gp_config->filter_color.b;		// �F���Z
			filter_trp.color_add_g = Gp_config->filter_color.g;		// �F���Z
			filter_trp.color_add_r = Gp_config->filter_color.r;		// �F���Z
			filter_trp.blend_type = TNM_BLEND_TYPE_NORMAL;

			// �e�̃����_�[�p�����[�^��������
			tnm_add_parent_trp(&filter_trp, parent_trp);

			// �x�[�X�X�v���C�g�̃����_�[�p�����[�^
			tnm_trp_to_rp(&filter_trp, &m_spr_filter.rpex, &m_spr_filter.rp);
			m_spr_filter.rp.d2_rect = true;
			m_spr_filter.rp.size_fit_to_texture = 0;
		}
		// �t�@�C�����g��Ȃ��ꍇ
		else	{

			S_tnm_render_param filter_trp;
			filter_trp.disp = true;

			// �e�̃����_�[�p�����[�^��������
			tnm_add_parent_trp(&filter_trp, parent_trp);

			// �t�B���^�X�v���C�g�̃����_�[�p�����[�^
			tnm_trp_to_rp(&filter_trp, &m_spr_filter.rpex, &m_spr_filter.rp);
			int width = 100;//window_rect.width();// - m_cur.filter_margin.left - m_cur.filter_margin.right;
			int height = 100;//window_rect.height();// - m_cur.filter_margin.top - m_cur.filter_margin.bottom;
			m_spr_filter.rp.size = C_float2((float)width, (float)height);					// �t�B���^�̃T�C�Y
			m_spr_filter.rp.filter.color = C_argb(0, 0, 0, 0);//m_cur.filter_color;		// �t�B���^�̐F
		}

		// �e�̃X�v���C�g�p�����[�^�ƌ���
		//tnm_affect_sp_and_rp(NULL, NULL, &m_spr_filter.rp, p_parent_rp);

		// ���������Ƃ��̐F�𒲐�
		if (hit_color == -1)	{
			hit_color = m_param.moji_color;
		}

		// ����
		int color = ((btn_state == TNM_BTN_STATE_NORMAL || btn_state == TNM_BTN_STATE_DISABLE) ? m_param.moji_color : hit_color);
		for (int i = 0; i < (int)m_moji_list.size(); i++)	{
			m_moji_list[i].frame(parent_trp, window_pos + m_pos + btn_action.rep_pos, 0
				, 255, Gp_color_table->get_color(color), Gp_color_table->get_color(Gp_ini->mwnd.shadow_color), Gp_color_table->get_color(Gp_ini->mwnd.fuchi_color)
				, draw_futoku, draw_shadow
				);
		}

		// �I�u�W�F�N�g�Ɉ����p�������_�[�p�����[�^
		S_tnm_render_param object_trp;
		object_trp.disp = true;
		object_trp.pos_x = (float)(window_pos.x + m_pos.x + btn_action.rep_pos.x);
		object_trp.pos_y = (float)(window_pos.y + m_pos.y + btn_action.rep_pos.y);

		// �e�̃����_�[�p�����[�^��������
		tnm_add_parent_trp(&object_trp, parent_trp);

		// �I�u�W�F�N�g�̃t���[������
		for (int i = 0; i < m_object_list.get_size(); i++)	{
			m_object_list[i].frame(&object_trp, 0, 0);
		}
	}
}

// ****************************************************************
// �X�v���C�g�c���[���\�z
// ================================================================
void C_elm_btn_select_item::get_sprite_tree(C_tree2<C_tnm_sprite *>& root)
{
	if (m_spr_base.rp.disp)
		root.add_child(&m_spr_base);

	if (m_spr_filter.rp.disp)
		root.add_child(&m_spr_filter);

	for (int i = 0; i < (int)m_moji_list.size(); i++)
		m_moji_list[i].get_sprite_tree(root, root);

	// �I�u�W�F�N�g
	for (int i = 0; i < m_object_list.get_size(); i++)
		m_object_list[i].get_sprite_tree(root);
}

// ****************************************************************
// �{�^����o�^
// ================================================================
void C_elm_btn_select_item::regist_button()
{
	if (m_param.item_type == TNM_SEL_ITEM_TYPE_ON)
		Gp_btn_mng->regist_button(&m_button);
}

// ****************************************************************
// �{�^���C�x���g����
// ================================================================
bool C_elm_btn_select_item::button_event()
{
	// ��������
	if (Gp_btn_mng->is_hit_this_frame(&m_button))	{

		// ���ʉ�
		tnm_play_se(TNM_SE_TYPE_SELECT);
	}

	// ���肳�ꂽ
	if (Gp_btn_mng->is_decided_this_frame(&m_button))	{

		// ���肳�ꂽ
		return true;
	}

	return false;
}

// ****************************************************************
// ������ݒ�
// ================================================================
void C_elm_btn_select_item::set_msg(CTSTR& msg, int item_type, int moji_size, C_point moji_pos, C_size moji_space, int x_align, int y_align, int moji_color, int shadow_color, int fuchi_color)
{
	m_msg = msg;
	m_param.item_type = item_type;
	m_param.moji_color = moji_color;

	// �A�C�e�����X�g���쐬
	C_point pos(0, 0);
	for (TSTR::const_iterator itr = msg.begin(); itr != msg.end();)	{
		int moji_type = TNM_MOJI_TYPE_MOJI;
		int moji_code = -1;

		// �G��������
		tnm_msg_proc_expand_name_flag(itr, msg.end(), &moji_type, &moji_code);

		// �������쐬
		C_elm_mwnd_moji moji;
		moji.set_moji(moji_type, moji_code, moji_size, moji_color, shadow_color, fuchi_color, moji_pos + pos);

		// �������X�g�ɒǉ�
		m_moji_list.push_back(moji);

		// ���̕����ʒu�̐ݒ�
		int this_moji_size = 0;
		if (moji_type != TNM_MOJI_TYPE_MOJI)	this_moji_size = moji_size + moji_space.cx;
		else if (is_hankaku((TCHAR)moji_code))	this_moji_size = (moji_size + moji_space.cx) / 2;
		else									this_moji_size = moji_size + moji_space.cx;
		pos.x += this_moji_size;
	}

	// ������`�̑傫�����擾
	int total_x = pos.x - moji_space.cx;
	int total_y = moji_size;

	// �����ʒu���Đݒ�
	for (int i = 0; i < (int)m_moji_list.size(); i++)	{
		int rep_x = 0, rep_y = 0;

		if (false);
		else if (x_align == 1)	{	rep_x -= total_x / 2;	}
		else if (x_align == 2)	{	rep_x -= total_x;		}

		if (false);
		else if (y_align == 1)	{	rep_y -= total_y / 2;	}
		else if (y_align == 2)	{	rep_y -= total_y;		}

		m_moji_list[i].set_moji_pos(m_moji_list[i].get_moji_pos() + C_point(rep_x, rep_y));
	}
}

// ****************************************************************
// �I������\��
// ================================================================
void C_elm_btn_select_item::set_disp()
{
	for (ARRAY<C_elm_mwnd_moji>::iterator itr = m_moji_list.begin(); itr != m_moji_list.end(); ++itr)	{
		itr->set_appear(true);
	}
}

// ****************************************************************
// ���b�Z�[�W�T�C�Y���擾
// ================================================================
C_size C_elm_btn_select_item::get_item_size()
{
	return m_item_size;
}

// ****************************************************************
// �{�^���I�����A�C�e�����X�g�F�I��
// ================================================================
void C_elm_btn_select_item_list::finish()
{
	int item_cnt = (int)m_sub.size();
	for (int i = 0; i < item_cnt; i++)
		m_sub[i].finish();
}

// ****************************************************************
// �{�^���I�����A�C�e�����X�g�F�T�u�̏�����
// ================================================================
void C_elm_btn_select_item_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"));
}

// ****************************************************************
// �{�^���I�����A�C�e�����X�g�F�T�u�̍ď�����
// ================================================================
void C_elm_btn_select_item_list::_reinit(int begin, int end, bool restruct_flag)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit(restruct_flag);
}

// ****************************************************************
// �{�^���I�����A�C�e�����X�g�F�T�u�̃Z�[�u
// ================================================================
void C_elm_btn_select_item_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// �{�^���I�����A�C�e�����X�g�F�T�u�̃��[�h
// ================================================================
void C_elm_btn_select_item_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}
