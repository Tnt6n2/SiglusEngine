#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_sorter.h"
#include	"element/elm_mwnd_key_icon.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// ������
// ================================================================
void C_elm_mwnd_key_icon::init()
{
	// �p�����[�^
	m_template_no = -1;
	m_icon_appear = false;
	m_icon_mode = 0;

	// ��ƕϐ�
	m_anime_start_flag = false;
	m_anime_start_time = 0;

	// �e���v���[�g���č\�z
	restruct_template();
}

// ****************************************************************
// �ď�����
// ================================================================
void C_elm_mwnd_key_icon::reinit()
{
	// ���
	release();

	// ������
	m_template_no = -1;
	m_icon_appear = false;
	m_icon_mode = 0;

	// ��ƕϐ�
	m_anime_start_flag = false;
	m_anime_start_time = 0;

	// �e���v���[�g���č\�z
	restruct_template();
}

// ****************************************************************
// ���
// ================================================================
void C_elm_mwnd_key_icon::release()
{
	// �X�v���C�g���N���A
	m_album_key_icon.reset();
	m_album_page_icon.reset();
	m_sprite_icon.release();
}

// ****************************************************************
// �e���v���[�g�̒l��ݒ�
// ================================================================
void C_elm_mwnd_key_icon::set_template(int template_no)
{
	// ���
	release();

	// �e���v���[�g�ԍ�
	m_template_no = template_no;

	// �e���v���[�g���č\�z
	restruct_template();
}

// ****************************************************************
// �Z�[�u
// ================================================================
void C_elm_mwnd_key_icon::save(C_tnm_save_stream& stream)
{
	// �e���v���[�g�ԍ�
	stream.save(m_template_no);

	// ���݂̃p�����[�^
	stream.save(m_icon_mode);
	stream.save(m_icon_pos);
}

// ****************************************************************
// ���[�h
// ================================================================
void C_elm_mwnd_key_icon::load(C_tnm_save_stream& stream)
{
	// �e���v���[�g�ԍ�
	stream.load(m_template_no);

	// �e���v���[�g���č\�z
	restruct_template();

	// �p�����[�^
	stream.load(m_icon_mode);
	stream.load(m_icon_pos);
}

// ****************************************************************
// �R�s�[
// ================================================================
void C_elm_mwnd_key_icon::copy(C_elm_mwnd_key_icon* src)
{
	// ���
	release();

	// �e���v���[�g�ԍ�
	m_template_no = src->m_template_no;

	// �e���v���[�g���č\�z
	restruct_template();

	// �p�����[�^
	m_icon_appear = src->m_icon_appear;
	m_icon_mode = src->m_icon_mode;
	m_icon_pos = src->m_icon_pos;

	// ��ƃp�����[�^
	m_anime_start_flag = src->m_anime_start_flag;
	m_anime_start_time = src->m_anime_start_time;
}

// ****************************************************************
// �e���v���[�g���č\�z�i�����p�F������Ȃ��j
// ================================================================
void C_elm_mwnd_key_icon::restruct_template()
{
	// �e���v���[�g�Ȃ�
	if (m_template_no < 0)
		return;

	// �L�[�҂��A�C�R��
	int key_icon_no = Gp_ini->mwnd_waku[m_template_no].icon_no;
	if (0 <= key_icon_no && key_icon_no < Gp_ini->icon_cnt)
	{
		// �A�C�R���̃t�@�C����
		TSTR file_name = Gp_ini->icon[key_icon_no].file_name;

		// �t�@�C�����̎w�肪����ꍇ�A�t�@�C����ǂݍ���
		if (!file_name.empty())
			m_album_key_icon = tnm_load_pct_d3d(file_name, false, false);
	}

	// �y�[�W�҂��A�C�R��
	int page_icon_no = Gp_ini->mwnd_waku[m_template_no].page_icon_no;
	if (0 <= page_icon_no && page_icon_no < Gp_ini->icon_cnt)
	{
		TSTR file_name = Gp_ini->icon[page_icon_no].file_name;
		if (!file_name.empty())
			m_album_page_icon = tnm_load_pct_d3d(file_name, false, false);
	}
}

// ****************************************************************
// �\���^��\����ݒ�
// ================================================================
void C_elm_mwnd_key_icon::set_appear(bool appear)
{
	m_icon_appear = appear;

	// ��\���ɂ����ꍇ�̓A�j�����~�߂�
	if (!appear)
	{
		m_anime_start_flag = false;
	}
}

// ****************************************************************
// �t���[������������
// ================================================================
void C_elm_mwnd_key_icon::frame_init()
{
	// �X�v���C�g��������
	m_sprite_icon.init();
}

// ****************************************************************
// �t���[������
// ================================================================
void C_elm_mwnd_key_icon::frame(int cur_time, C_rect window_rect, const S_tnm_render_param* parent_trp)
{
	// �e���v���[�g���Ȃ���Ή������Ȃ�
	if (m_template_no < 0 || Gp_ini->mwnd_waku_cnt <= m_template_no)
		return;

	// �o�����Ă��Ȃ��ꍇ�͉������Ȃ�
	if (!m_icon_appear)
		return;

	// �A�j�����X�^�[�g���Ă��Ȃ��ꍇ�̓X�^�[�g����
	if (!m_anime_start_flag)
	{
		m_anime_start_flag = true;
		m_anime_start_time = cur_time;
	}

	// ���[�h�ɂ���ăA�C�R���ԍ����ς��
	int icon_no = (m_icon_mode == 0 ? Gp_ini->mwnd_waku[m_template_no].icon_no : Gp_ini->mwnd_waku[m_template_no].page_icon_no);
	if (icon_no < 0 || Gp_ini->icon_cnt <= icon_no)
		return;

	// �p�^�[���ԍ��Ƒ��x�����߂�
	int anime_speed = Gp_ini->icon[icon_no].anime_speed;
	int anime_pat_cnt = Gp_ini->icon[icon_no].anime_pat_cnt;
	if (anime_speed == 0 || anime_pat_cnt == 0)
		return;

	int pat_no = (cur_time - m_anime_start_time) / anime_speed % anime_pat_cnt;

	// ���[�h�ɂ���Ďg���A���o����ς���
	C_d3d_album* p_album = (m_icon_mode == 0 ? m_album_key_icon.get() : m_album_page_icon.get());

	// �t�@�C��������ꍇ
	if (p_album && p_album->get_texture(pat_no))
	{
		// �A�C�R���̈ʒu�����߂�i�ǂ̊p�ɂ��������ŏꍇ�킯�j
		C_point pos = C_point(0, 0);
		if (Gp_ini->mwnd_waku[m_template_no].icon_pos_type == 0)
		{
			switch (Gp_ini->mwnd_waku[m_template_no].icon_pos_base)
			{
				case 0:
					pos.x = window_rect.left + Gp_ini->mwnd_waku[m_template_no].icon_pos.x;
					pos.y = window_rect.top + Gp_ini->mwnd_waku[m_template_no].icon_pos.y;
					break;
				case 1:
					pos.x = window_rect.right - Gp_ini->mwnd_waku[m_template_no].icon_pos.x - p_album->get_texture(pat_no)->get_original_width();
					pos.y = window_rect.top + Gp_ini->mwnd_waku[m_template_no].icon_pos.y;
					break;
				case 2:
					pos.x = window_rect.left + Gp_ini->mwnd_waku[m_template_no].icon_pos.x;
					pos.y = window_rect.bottom - Gp_ini->mwnd_waku[m_template_no].icon_pos.y - p_album->get_texture(pat_no)->get_original_height();
					break;
				case 3:
					pos.x = window_rect.right - Gp_ini->mwnd_waku[m_template_no].icon_pos.x - p_album->get_texture(pat_no)->get_original_width();
					pos.y = window_rect.bottom - Gp_ini->mwnd_waku[m_template_no].icon_pos.y - p_album->get_texture(pat_no)->get_original_height();
					break;
			}
		}
		else if (Gp_ini->mwnd_waku[m_template_no].icon_pos_type == 1)
		{
			if (Gp_ini->tateyoko_mode == 1)
			{
				pos.x = window_rect.right - m_icon_pos.x - p_album->get_texture(pat_no)->get_original_width();
				pos.y = window_rect.top + m_icon_pos.y;
			}
			else
			{
				pos.x = window_rect.left + m_icon_pos.x;
				pos.y = window_rect.top + m_icon_pos.y;
			}
		}

		// �����_�[�p�����[�^
		S_tnm_render_param icon_trp;
		icon_trp.disp = true;
		icon_trp.sorter = S_tnm_sorter(0, Gp_ini->mwnd.waku_layer_rep);
		icon_trp.pos_x = (float)pos.x;
		icon_trp.pos_y = (float)pos.y;

		// �e�̃����_�[�p�����[�^��������
		tnm_add_parent_trp(&icon_trp, parent_trp);

		// �X�v���C�g���쐬
		m_sprite_icon.create_polygon_no_init();

		// �X�v���C�g�̃����_�[�p�����[�^
		tnm_trp_to_rp(&icon_trp, &m_sprite_icon.rpex, &m_sprite_icon.rp);
		m_sprite_icon.rp.d2_rect = true;
		m_sprite_icon.rp.size_fit_to_texture = 0;	// �O�ԃe�N�X�`���Ƀt�B�b�g
		m_sprite_icon.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
		m_sprite_icon.set_index_buffer(G_rect_index_buffer);
		m_sprite_icon.set_texture(p_album->get_texture(pat_no));
	}
}

// ****************************************************************
// �I�u�W�F�N�g�F�X�v���C�g�c���[���\�z
// ================================================================
void C_elm_mwnd_key_icon::get_sprite_tree(C_tree2<C_tnm_sprite *>& root)
{
	root.add_child(&m_sprite_icon);
}



