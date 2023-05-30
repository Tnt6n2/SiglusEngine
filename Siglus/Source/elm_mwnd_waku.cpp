#include	"pch.h"

#include	"tnm_element_code.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_mwnd_waku.h"
#include	"engine/eng_dir.h"
#include	"engine/eng_config.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// ������
// ================================================================
void C_elm_mwnd_waku::init(S_element element, CTSTR& name)
{
	// �p�����[�^���N���A
	m_template_no = -1;
	m_extend_type = 0;

	// �f�t�H���g�̐ݒ�
	m_element = element;
	m_name = name;
	m_def.filter_margin = C_rect(0, 0, 0, 0);
	m_def.filter_color = C_argb(0, 0, 0, 0);
	m_def.filter_config_color = false;
	m_def.filter_config_tr = false;

	// �����ϐ�������
	init_work_variable();

	// �L�[�҂��A�C�R��
	m_key_icon.init();

	// �{�^��
	m_btn_list.init(m_element + S_element(ELM_MWND_BUTTON), m_name + _T(".button"), Gp_ini->mwnd_waku_btn_cnt, false, false, false, false, NULL, S_element(), S_element());

	// ��O��
	m_face_list.init(m_element + S_element(ELM_MWND_FACE), m_name + _T(".face"), Gp_ini->mwnd_waku_face_cnt, false, false, false, false, NULL, S_element(), S_element());

	// �I�u�W�F�N�g
	m_object_list.init(m_element + S_element(ELM_MWND_OBJECT), m_name + _T(".object"), Gp_ini->mwnd_waku_object_cnt, false, false, false, false, NULL, S_element(), S_element());
}

// ****************************************************************
// �ď�����
// ================================================================
void C_elm_mwnd_waku::reinit(bool restruct_flag)
{
	// �X�v���C�g���N���A
	m_spr_waku.init();
	m_spr_filter.init();

	// �A���o�����N���A
	m_album_waku.reset();
	m_album_filter.reset();

	// �p�����[�^���N���A
	m_extend_type = 0;
	m_waku_file.clear();

	// �{�^��
	m_btn_list.reinit(restruct_flag);

	// ��O��
	m_face_list.reinit(restruct_flag);

	// �I�u�W�F�N�g
	m_object_list.reinit(restruct_flag);

	// �L�[�҂��A�C�R��
	m_key_icon.reinit();
}

// ****************************************************************
// �I��
// ================================================================
void C_elm_mwnd_waku::finish()
{
	// �{�^��
	m_btn_list.finish();

	// ��O��
	m_face_list.finish();

	// �I�u�W�F�N�g
	m_object_list.finish();
}

// ****************************************************************
// �����ϐ�������
// ================================================================
void C_elm_mwnd_waku::init_work_variable()
{
	m_cur = m_def;	// �f�t�H���g�p�����[�^�����݂̃p�����[�^�փR�s�[����
}

// ****************************************************************
// �e���v���[�g�̒l��ݒ�
// ================================================================
void C_elm_mwnd_waku::set_template(int template_no, bool reinit_flag)
{
	// �ď�����
	if (reinit_flag)
		reinit(false);

	// �e���v���[�g�ԍ�
	m_template_no = template_no;

	// �e���v���[�g�Ȃ�
	if (m_template_no < 0)
		return;

	// �e���v���[�g�p�����[�^
	m_waku_file = Gp_ini->mwnd_waku[m_template_no].waku_file;
	m_filter_file = Gp_ini->mwnd_waku[m_template_no].filter_file;

	// �e���v���[�g���č\�z
	restruct_template();

	// �����ϐ�������
	init_work_variable();

	// �L�[�҂��A�C�R��
	m_key_icon.set_template(template_no);
}

// ****************************************************************
// �e���v���[�g���č\�z
// ================================================================
void C_elm_mwnd_waku::restruct_template()
{
	// �e���v���[�g�Ȃ�
	if (m_template_no < 0)
		return;

	// �g�֘A�̏��
	m_extend_type = Gp_ini->mwnd_waku[m_template_no].extend_type;

	// �g���č\�z
	restruct_waku();

	// �t�B���^�[�֘A�̏��
	m_def.filter_margin = Gp_ini->mwnd_waku[m_template_no].filter_margin;
	m_def.filter_color = Gp_ini->mwnd_waku[m_template_no].filter_color;
	m_def.filter_config_color = Gp_ini->mwnd_waku[m_template_no].filter_config_color;
	m_def.filter_config_tr = Gp_ini->mwnd_waku[m_template_no].filter_config_tr;

	// �t�B���^�[���č\�z
	restruct_filter();

	// �{�^��
	int btn_cnt = (int)m_btn_list.get_size();
	for (int i = 0; i < btn_cnt; i++)	{

		// �t�@�C�������w�肵�Ă���΃{�^���ƔF������
		if (!Gp_ini->mwnd_waku[m_template_no].btn[i].btn_file.empty())	{
			m_btn_list[i].create_pct(Gp_ini->mwnd_waku[m_template_no].btn[i].btn_file);
			m_btn_list[i].set_layer(Gp_ini->mwnd.moji_layer_rep);
			m_btn_list[i].set_disp(1);
			m_btn_list[i].set_button_cut_no(Gp_ini->mwnd_waku[m_template_no].btn[i].cut_no);
			m_btn_list[i].set_button_param(0, 0, Gp_ini->mwnd_waku[m_template_no].btn[i].action_no, Gp_ini->mwnd_waku[m_template_no].btn[i].se_no);
			m_btn_list[i].set_button_sys_type(Gp_ini->mwnd_waku[m_template_no].btn[i].sys_type, Gp_ini->mwnd_waku[m_template_no].btn[i].sys_type_opt);
			m_btn_list[i].set_button_mode(Gp_ini->mwnd_waku[m_template_no].btn[i].btn_mode);
			m_btn_list[i].set_frame_action(-1, 0, Gp_ini->mwnd_waku[m_template_no].btn[i].frame_action_scn_name, Gp_ini->mwnd_waku[m_template_no].btn[i].frame_action_cmd_name, C_tnm_prop_list());
			if (!Gp_ini->mwnd_waku[m_template_no].btn[i].scn_name.empty() && !Gp_ini->mwnd_waku[m_template_no].btn[i].cmd_name.empty())
				m_btn_list[i].set_button_decided_action(Gp_ini->mwnd_waku[m_template_no].btn[i].scn_name, Gp_ini->mwnd_waku[m_template_no].btn[i].cmd_name);
			else if (!Gp_ini->mwnd_waku[m_template_no].btn[i].scn_name.empty() && Gp_ini->mwnd_waku[m_template_no].btn[i].z_no >= 0)
				m_btn_list[i].set_button_decided_action(Gp_ini->mwnd_waku[m_template_no].btn[i].scn_name, Gp_ini->mwnd_waku[m_template_no].btn[i].z_no);
		}
	}
}

// ****************************************************************
// �Z�[�u
// ================================================================
void C_elm_mwnd_waku::save(C_tnm_save_stream& stream)
{
	// �e���v���[�g�ԍ�
	stream.save(m_template_no);

	// �e���v���[�g�p�����[�^
	stream.save(m_waku_file);
	stream.save(m_filter_file);

	// ���݂̃p�����[�^
	stream.save(m_cur);

	// �L�[�҂��A�C�R��
	m_key_icon.save(stream);

	// �{�^���̓Z�[�u���Ȃ�

	// ��O��
	m_face_list.save(stream);

	// �I�u�W�F�N�g
	m_object_list.save(stream);
}

// ****************************************************************
// ���[�h
// ================================================================
void C_elm_mwnd_waku::load(C_tnm_save_stream& stream)
{
	// ���������ς݂�O��Ƃ��Ă��܂��B

	// �e���v���[�g�ԍ�
	stream.load(m_template_no);

	// �e���v���[�g�p�����[�^
	stream.load(m_waku_file);
	stream.load(m_filter_file);

	// �e���v���[�g���č\�z
	restruct_template();

	// ���݂̃p�����[�^
	stream.load(m_cur);

	// �L�[�҂��A�C�R��
	m_key_icon.load(stream);

	// �{�^���̓��[�h���Ȃ�

	// ��O��
	m_face_list.load(stream);

	// �I�u�W�F�N�g
	m_object_list.load(stream);
}

// ****************************************************************
// �R�s�[�i�S�āj
// ================================================================
void C_elm_mwnd_waku::copy_all(C_elm_mwnd_waku* src, bool reinit_flag)
{
	// �ď�����
	if (reinit_flag)
		reinit(false);

	// �e���v���[�g�ԍ�
	m_template_no = src->m_template_no;

	// �e���v���[�g�p�����[�^
	m_waku_file = src->m_waku_file;
	m_filter_file = src->m_filter_file;

	// �e���v���[�g���č\�z
	restruct_template();

	// ���݂̃p�����[�^
	m_cur = src->m_cur;

	// �L�[�҂��A�C�R��
	m_key_icon.copy(&src->m_key_icon);

	// �{�^��
	int btn_cnt = m_btn_list.get_size();
	for (int i = 0; i < btn_cnt; i++)
		m_btn_list[i].copy(&src->m_btn_list[i], false);

	// ��O��
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)
		m_face_list[i].copy(&src->m_face_list[i], false);

	// �I�u�W�F�N�g
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
		m_object_list[i].copy(&src->m_object_list[i], false);
}

// ****************************************************************
// �R�s�[�i�g�j
// ================================================================
void C_elm_mwnd_waku::copy_waku(C_elm_mwnd_waku* src, bool reinit_flag)
{
	// �ď�����
	if (reinit_flag)
		reinit(false);

	// �e���v���[�g�ԍ�
	m_template_no = src->m_template_no;

	// �e���v���[�g�p�����[�^
	m_waku_file = src->m_waku_file;
	m_filter_file = src->m_filter_file;

	// �e���v���[�g���č\�z
	restruct_template();

	// ���݂̃p�����[�^
	m_cur = src->m_cur;
}

// ****************************************************************
// �R�s�[�i�L�[�҂��A�C�R���j
// ================================================================
void C_elm_mwnd_waku::copy_key_icon(C_elm_mwnd_waku* src)
{
	// �L�[�҂��A�C�R��
	m_key_icon.copy(&src->m_key_icon);
}

// ****************************************************************
// �R�s�[�i��O���j
// ================================================================
void C_elm_mwnd_waku::copy_face(C_elm_mwnd_waku* src)
{
	// ��O��
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)
		m_face_list[i].copy(&src->m_face_list[i], false);
}

// ****************************************************************
// �R�s�[�i�I�u�W�F�N�g�j
// ================================================================
void C_elm_mwnd_waku::copy_object(C_elm_mwnd_waku* src)
{
	// �I�u�W�F�N�g
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
		m_object_list[i].copy(&src->m_object_list[i], false);
}

// ****************************************************************
// �t���[������������
// ================================================================
void C_elm_mwnd_waku::frame_init()
{
	// �X�v���C�g��������
	m_spr_waku.rp.init();
	m_spr_filter.rp.init();

	// �L�[�҂��A�C�R��
	m_key_icon.frame_init();

	// �{�^��
	int bnt_cnt = m_btn_list.get_size();
	for (int i = 0; i < bnt_cnt; i++)
		m_btn_list[i].frame_init();

	// ��O��
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)	{
		m_face_list[i].frame_init();
		m_face_list[i].set_disp(0);		// ��\���ɂ���
	}

	// �I�u�W�F�N�g
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
		m_object_list[i].frame_init();
}

// ****************************************************************
// ���Ԃ�i�߂�
// ================================================================
void C_elm_mwnd_waku::update_time(int past_game_time, int past_real_time)
{
	// �{�^��
	int btn_cnt = m_btn_list.get_size();
	for (int i = 0; i < btn_cnt; i++)
		m_btn_list[i].update_time(past_game_time, past_real_time, 0);

	// ��O��
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)
		m_face_list[i].update_time(past_game_time, past_real_time, 0);

	// �I�u�W�F�N�g
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
		m_object_list[i].update_time(past_game_time, past_real_time, 0);
}

// ****************************************************************
// �t���[������
// ================================================================
void C_elm_mwnd_waku::frame(int cur_time, C_rect window_rect, const S_tnm_render_param* parent_trp, bool face_appear_flag)
{
	if (m_template_no < 0)
		return;

	// �g�t�@�C�����g���ꍇ
	if (m_spr_waku.get_texture())	{

		// �g�̃����_�[�p�����[�^
		S_tnm_render_param waku_trp;
		waku_trp.disp = true;
		waku_trp.sorter = S_tnm_sorter(0, Gp_ini->mwnd.waku_layer_rep);
		waku_trp.pos_x = (float)window_rect.left;
		waku_trp.pos_y = (float)window_rect.top;

		// �e�̃����_�[�p�����[�^��������
		tnm_add_parent_trp(&waku_trp, parent_trp);

		// �g�X�v���C�g�̃����_�[�p�����[�^
		tnm_trp_to_rp(&waku_trp, &m_spr_waku.rpex, &m_spr_waku.rp);
		m_spr_waku.rp.d2_rect = true;
		m_spr_waku.rp.size_fit_to_texture = 0;	// �O�ԃe�N�X�`���Ƀt�B�b�g
	}

	// �t�B���^�̃����_�[�p�����[�^
	{
		S_tnm_render_param filter_trp;
		filter_trp.disp = true;
		filter_trp.sorter = S_tnm_sorter(0, Gp_ini->mwnd.filter_layer_rep);
		filter_trp.pos_x = (float)window_rect.left + m_cur.filter_margin.left;
		filter_trp.pos_y = (float)window_rect.top + m_cur.filter_margin.top;
		if (m_def.filter_config_tr)	{
			filter_trp.tr = Gp_config->filter_color.a;				// �����x
		}
		if (m_def.filter_config_color)	{
			filter_trp.color_add_b = Gp_config->filter_color.b;		// �F���Z
			filter_trp.color_add_g = Gp_config->filter_color.g;		// �F���Z
			filter_trp.color_add_r = Gp_config->filter_color.r;		// �F���Z
		}

		// �e�̃����_�[�p�����[�^��������
		tnm_add_parent_trp(&filter_trp, parent_trp);

		// �t�B���^�[�X�v���C�g�̃����_�[�p�����[�^
		tnm_trp_to_rp(&filter_trp, &m_spr_filter.rpex, &m_spr_filter.rp);
		m_spr_filter.rp.d2_rect = true;

		// �t�@�C�����g���ꍇ
		if (m_spr_filter.get_texture())	{
			m_spr_filter.rp.size_fit_to_texture = 0;	// �O�ԃe�N�X�`���Ƀt�B�b�g
		}
		// �t�@�C�����g��Ȃ��ꍇ
		else	{
			int width = window_rect.width() - m_cur.filter_margin.left - m_cur.filter_margin.right;
			int height = window_rect.height() - m_cur.filter_margin.top - m_cur.filter_margin.bottom;
			m_spr_filter.rp.size = C_float2((float)width, (float)height);	// �t�B���^�̃T�C�Y
			m_spr_filter.rp.filter.color = m_cur.filter_color;				// �t�B���^�̐F
		}
	}

	// �L�[�҂��A�C�R��
	m_key_icon.frame(cur_time, window_rect, parent_trp);

	// �{�^��
	int btn_cnt = m_btn_list.get_size();
	for (int i = 0; i < btn_cnt; i++)	{

		// �q�Ɉ����p�����߂̃����_�[�p�����[�^
		S_tnm_render_param trp;
		trp.disp = true;
		trp.pos_x = (float)window_rect.left;
		trp.pos_y = (float)window_rect.top;

		// �ʒu���v�Z����
		switch (Gp_ini->mwnd_waku[m_template_no].btn[i].pos_base)	{
			case 0:
				trp.pos_x += Gp_ini->mwnd_waku[m_template_no].btn[i].pos.x;
				trp.pos_y += Gp_ini->mwnd_waku[m_template_no].btn[i].pos.y;
				break;
			case 1:
				trp.pos_x += window_rect.width() - Gp_ini->mwnd_waku[m_template_no].btn[i].pos.x;
				trp.pos_y += Gp_ini->mwnd_waku[m_template_no].btn[i].pos.y;
				break;
			case 2:
				trp.pos_x += Gp_ini->mwnd_waku[m_template_no].btn[i].pos.x;
				trp.pos_y += window_rect.height() - Gp_ini->mwnd_waku[m_template_no].btn[i].pos.y;
				break;
			case 3:
				trp.pos_x += window_rect.width() - Gp_ini->mwnd_waku[m_template_no].btn[i].pos.x;
				trp.pos_y += window_rect.height() - Gp_ini->mwnd_waku[m_template_no].btn[i].pos.y;
				break;
		}

		// �ݒ�ɂ���ă{�^����s���ɂ���
		if (Gp_local->pod.mwnd_btn_disable_all || Gp_local->mwnd_btn_disable[i].value)
			trp.button_state = TNM_BTN_STATE_DISABLE;

		// �␳���C���[�l�ݒ�
		trp.sorter = S_tnm_sorter(0, Gp_ini->mwnd.waku_layer_rep);

		// �e�̃����_�[�p�����[�^��������
		tnm_add_parent_trp(&trp, parent_trp);

		// �t���[������
		m_btn_list[i].frame(&trp, 0, 0);
	}

	// ��O��
	if (face_appear_flag)	{	// �\������ꍇ

		int face_cnt = m_face_list.get_size();
		for (int i = 0; i < face_cnt; i++)	{

			// �q�Ɉ����p�����߂̃����_�[�p�����[�^
			S_tnm_render_param trp;
			trp.disp = true;
			trp.pos_x = (float)window_rect.left + Gp_ini->mwnd_waku[m_template_no].face_pos[i].x;
			trp.pos_y = (float)window_rect.top + Gp_ini->mwnd_waku[m_template_no].face_pos[i].y;

			// �\������
			m_face_list[i].set_disp(1);

			// �␳���C���[�l�ݒ�
			trp.sorter = S_tnm_sorter(0, Gp_ini->mwnd.face_layer_rep);

			// �e�̃����_�[�p�����[�^��������
			tnm_add_parent_trp(&trp, parent_trp);

			// �t���[������
			m_face_list[i].frame(&trp, 0, 0);
		}
	}

	// �I�u�W�F�N�g
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)	{

		// �q�Ɉ����p�����߂̃����_�[�p�����[�^
		S_tnm_render_param trp;
		trp.disp = true;
		trp.pos_x = (float)window_rect.left;
		trp.pos_y = (float)window_rect.top;

		// �e�̃����_�[�p�����[�^��������
		tnm_add_parent_trp(&trp, parent_trp);

		// �t���[������
		m_object_list[i].frame(&trp, 0, 0);
	}
}

// ****************************************************************
// �t���[���A�N�V��������
// ================================================================
void C_elm_mwnd_waku::do_frame_action()
{
	if (m_template_no < 0)
		return;

	// �{�^��
	int btn_cnt = m_btn_list.get_size();
	for (int i = 0; i < btn_cnt; i++)
		m_btn_list[i].do_frame_action();

	// ��O��
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)
		m_face_list[i].do_frame_action();

	// �I�u�W�F�N�g
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
		m_object_list[i].do_frame_action();
}

// ****************************************************************
// �X�v���C�g�c���[���\�z
// ================================================================
void C_elm_mwnd_waku::get_sprite_tree(C_tree2<C_tnm_sprite *>& root)
{
	if (m_template_no < 0)
		return;

	root.add_child(&m_spr_waku);
	root.add_child(&m_spr_filter);

	// �L�[�҂��A�C�R��
	m_key_icon.get_sprite_tree(root);

	// �{�^��
	int btn_cnt = m_btn_list.get_size();
	for (int i = 0; i < btn_cnt; i++)
		m_btn_list[i].get_sprite_tree(root);

	// ��O��
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)
		m_face_list[i].get_sprite_tree(root);

	// �I�u�W�F�N�g
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)
		m_object_list[i].get_sprite_tree(root);
}

// ****************************************************************
// �{�^����o�^
// ================================================================
void C_elm_mwnd_waku::regist_button()
{
	// �{�^��
	int btn_cnt = m_btn_list.get_size();
	for (int i = 0; i < btn_cnt; i++)	{
		m_btn_list[i].regist_button(INT_MIN);
	}

	// ��O��
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)	{
		m_face_list[i].regist_button(INT_MIN);
	}

	// �I�u�W�F�N�g
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)	{
		m_object_list[i].regist_button(INT_MIN);
	}
}

// ****************************************************************
// �{�^���C�x���g
// ================================================================
void C_elm_mwnd_waku::button_event()
{
	// �{�^��
	int btn_cnt = m_btn_list.get_size();
	for (int i = 0; i < btn_cnt; i++)	{
		int parent_button_state = TNM_BTN_STATE_NORMAL;

		// �ݒ�ɂ���ă{�^����s���ɂ���
		if (Gp_local->pod.mwnd_btn_disable_all || Gp_local->mwnd_btn_disable[i].value)	{
			parent_button_state = TNM_BTN_STATE_DISABLE;
		}

		// disable �łȂ���΃C�x���g
		if (parent_button_state != TNM_BTN_STATE_DISABLE)	{
			m_btn_list[i].button_event();
		}
	}

	// ��O��
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)	{
		m_face_list[i].button_event();
	}

	// �I�u�W�F�N�g
	int object_cnt = m_object_list.get_size();
	for (int i = 0; i < object_cnt; i++)	{
		m_object_list[i].button_event();
	}
}

// ****************************************************************
// �Ϙg�̐ݒ�
// ================================================================
//		�e�L�X�g�̓��e����A�Ϙg�̑傫�����v�Z���A�X�v���C�g���쐬���܂��B
// ================================================================
bool C_elm_mwnd_waku::set_extend_waku(C_rect* window_rect)
{
	// �Œ�g�̏ꍇ
	if (m_extend_type == 0)	{

		// �摜���g���Ă���ꍇ�͉摜�T�C�Y
		if (m_album_waku && m_album_waku->get_texture(0))	{
			*window_rect = C_rect(C_point(0, 0), m_album_waku->get_texture(0)->get_original_size());

			// �X�v���C�g�Ƀe�N�X�`����ݒ肷��
			m_spr_waku.set_texture(m_album_waku->get_texture(0));
		}
	}

	// �Ϙg�̏ꍇ
	else if (m_extend_type == 1)	{

		// �g�t�@�C�����w�肳��Ă���ꍇ
		if (!m_waku_file.empty())
		{
			// g00 ��������
			int pct_type = 0;
			TSTR file_path = tnm_find_g00(m_waku_file, &pct_type);

			if (file_path.empty())
			{
				tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("�g�t�@�C�� ") + m_waku_file + _T(" ��������܂���ł����B"));
				return false;
			}
			if (pct_type != 1)
			{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�g�t�@�C���� g00 �����Ή����Ă��܂���B\n") + m_waku_file);
				return false;
			}

			// g00 ����g��ǂݍ���
			BSP<C_d3d_texture> texture = G_rsm.create_texture_from_mwnd(file_path, m_extend_type, window_rect->size(), false);
			if (!texture)
			{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�g�t�@�C�� ") + m_waku_file + _T(" ���ǂݍ��߂܂���ł����B") + get_last_error_msg());
				return false;
			}

			*window_rect = C_rect(C_point(0, 0), texture->get_size());

			// �X�v���C�g�Ƀe�N�X�`����ݒ肷��
			m_spr_waku.set_texture(texture);
		}
	}

	return true;
}

// ****************************************************************
// �g�t�@�C����������
// ================================================================
void C_elm_mwnd_waku::init_waku_file()
{
	// �X�v���C�g�ƃA���o�������
	m_spr_waku.init();
	m_album_waku.reset();

	// �e���v���[�g�Ȃ�
	if (m_template_no < 0)
		return;

	// �g�t�@�C����ݒ�
	m_waku_file = Gp_ini->mwnd_waku[m_template_no].waku_file;

	// �g���č\�z
	restruct_waku();
}

// ****************************************************************
// �g�t�@�C����ύX
// ================================================================
void C_elm_mwnd_waku::set_waku_file(CTSTR& waku_file)
{
	// �g���ς��Ȃ��ꍇ�͉������Ȃ�
	if (waku_file == m_waku_file)
		return;

	// �X�v���C�g�ƃA���o�������
	m_spr_waku.init();
	m_album_waku.reset();

	// �g�t�@�C����ݒ�
	m_waku_file = waku_file;
	
	// �g���č\�z
	restruct_waku();
}

// ****************************************************************
// �g���č\�z
// ================================================================
void C_elm_mwnd_waku::restruct_waku()
{
	// �t�@�C�����̎w�肪����ꍇ�A�t�@�C����ǂݍ���
	if (!m_waku_file.empty())	{

		// �E�B���h�E�̃X�v���C�g���쐬
		m_spr_waku.create_polygon_no_init();
		m_spr_waku.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
		m_spr_waku.set_index_buffer(G_rect_index_buffer);

		// �e�N�X�`����ǂݍ���
		m_album_waku = tnm_load_pct_d3d(m_waku_file, false, false);
		//if (m_album_waku)
		//	m_spr_waku.set_texture(m_album_waku->get_texture(0));
		// �����Ԃ�Ϙg�̏ꍇ���l���ăR�����g�A�E�g�H�H�H
	}
	// �t�@�C�������Ȃ��ꍇ�̓t�B���^�[
	else	{

		// �E�B���h�E�̃X�v���C�g���쐬
		m_spr_waku.create_polygon_no_init();
		m_spr_waku.set_vertex_buffer(G_rect_vertex_buffer_d2_filter);
		m_spr_waku.set_index_buffer(G_rect_index_buffer);
	}
}

// ****************************************************************
// �t�B���^�[�t�@�C����������
// ================================================================
void C_elm_mwnd_waku::init_filter_file()
{
	// �X�v���C�g�ƃA���o�������
	m_spr_filter.init();
	m_album_filter.reset();

	// �e���v���[�g�Ȃ�
	if (m_template_no < 0)
		return;

	// �t�B���^�[�t�@�C����ݒ�
	m_filter_file = Gp_ini->mwnd_waku[m_template_no].filter_file;

	// �t�B���^�[���č\�z
	restruct_filter();
}

// ****************************************************************
// �t�B���^�[�t�@�C����ύX
// ================================================================
void C_elm_mwnd_waku::set_filter_file(CTSTR& filter_file)
{
	// �t�B���^�[���ς��Ȃ��ꍇ�͉������Ȃ�
	if (filter_file == m_filter_file)
		return;

	// �X�v���C�g�ƃA���o�������
	m_spr_filter.init();
	m_album_filter.reset();

	// �t�B���^�[�t�@�C����ݒ�
	m_filter_file = filter_file;
	
	// �t�B���^�[���č\�z
	restruct_filter();
}

// ****************************************************************
// �t�B���^�[���č\�z
// ================================================================
void C_elm_mwnd_waku::restruct_filter()
{
	// �t�@�C�����̎w�肪����ꍇ�A�t�@�C����ǂݍ���
	if (!m_filter_file.empty())	{

		// �t�B���^�[�̃X�v���C�g���쐬
		m_spr_filter.create_polygon_no_init();
		m_spr_filter.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
		m_spr_filter.set_index_buffer(G_rect_index_buffer);

		// �e�N�X�`����ǂݍ���
		m_album_filter = tnm_load_pct_d3d(m_filter_file, false, false);
		if (m_album_filter)
			m_spr_filter.set_texture(m_album_filter->get_texture(0));
	}
	// �t�@�C�������Ȃ��ꍇ�̓t�B���^�[
	else	{

		// �t�B���^�[�̃X�v���C�g���쐬
		m_spr_filter.create_polygon_no_init();
		m_spr_filter.set_vertex_buffer(G_rect_vertex_buffer_d2_filter);
		m_spr_filter.set_index_buffer(G_rect_index_buffer);
	}

}

// ****************************************************************
// ��O�����N���A
// ================================================================
void C_elm_mwnd_waku::clear_face()
{
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)
		m_face_list[i].reinit(true);
}

// ****************************************************************
// ��O���̐ݒ�
// ================================================================
void C_elm_mwnd_waku::set_face(int face_no, CTSTR& file_path)
{
	if (face_no < 0 || m_face_list.get_size() <= face_no)
		return;

	m_face_list[face_no].create_pct(file_path);
}

// ****************************************************************
// ��O���̑��ݔ���
// ================================================================
bool C_elm_mwnd_waku::is_exist_face()
{
	int face_cnt = m_face_list.get_size();
	for (int i = 0; i < face_cnt; i++)	{
		if (m_face_list[i].get_type() == TNM_OBJECT_TYPE_PCT)	{
			return true;
		}
	}

	return false;
}
