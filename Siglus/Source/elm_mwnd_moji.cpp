#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_sorter.h"
#include	"data/tnm_moji_manager.h"
#include	"data/tnm_color_table.h"
#include	"element/elm_mwnd_moji.h"
#include	"engine/eng_config.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// �R���X�g���N�^
// ================================================================
C_elm_mwnd_moji::C_elm_mwnd_moji()
{
	m_moji.type = TNM_MOJI_TYPE_MOJI;
	m_moji.code = 0;
	m_moji.size = 0;
	m_moji.color = 0;
	m_moji.shadow = 0;
	m_moji.fuchi = 0;
	m_appeared_flag = false;
	m_ruby_flag = false;
}

// ****************************************************************
// ������
// ================================================================
void C_elm_mwnd_moji::init()
{
	// �p�����[�^��������
	m_moji.type = TNM_MOJI_TYPE_MOJI;
	m_moji.code = 0;
	m_moji.size = 0;
	m_moji.color = 0;
	m_moji.shadow = 0;
	m_moji.fuchi = 0;
	m_appeared_flag = false;
	m_ruby_flag = false;

	// ���\�[�X��������
	m_spr_moji.init();
	m_spr_shadow.init();
	m_spr_fuchi.init();
}

// ****************************************************************
// �Z�[�u
// ================================================================
void C_elm_mwnd_moji::save(C_tnm_save_stream& stream)
{
	// �p�����[�^��ۑ�
	stream.save(m_moji);
	stream.save(m_appeared_flag);
	stream.save(m_ruby_flag);
}

// ****************************************************************
// ���[�h
// ================================================================
void C_elm_mwnd_moji::load(C_tnm_save_stream& stream)
{
	// �p�����[�^��ǂݍ���
	stream.load(m_moji);
	stream.load(m_appeared_flag);
	stream.load(m_ruby_flag);

	// �������č\�z
	set_moji(m_moji.type, m_moji.code, m_moji.size, m_moji.color, m_moji.shadow, m_moji.fuchi, m_moji.pos);
}

// ****************************************************************
// �t���[������������
// ================================================================
void C_elm_mwnd_moji::frame_init()
{
	// �X�v���C�g������������
	m_spr_moji.init();
	m_spr_shadow.init();
	m_spr_fuchi.init();
}

// ****************************************************************
// �t���[������
// ================================================================
bool C_elm_mwnd_moji::frame(const S_tnm_render_param* parent_trp, C_point pos_rep, int dark_rep, int color_rate, C_argb color, C_argb shadow_color, C_argb fuchi_color, bool draw_futoku, bool draw_shadow)
{
	if (!m_appeared_flag)
		return true;	// �܂��o�����Ȃ�

	// ����

	// �����X�v���C�g���쐬����
	m_spr_moji.create_polygon_no_init();
	m_spr_moji.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
	m_spr_moji.set_index_buffer(G_rect_index_buffer);

	// �����e�N�X�`���i�Ǝ��̕����}�l�[�W������擾����j
	// �K�����ō쐬����
	MOJI_FACE_TYPE moji_face_type = (draw_futoku) ? MOJI_FACE_TYPE_4 : MOJI_FACE_TYPE_NONE;
	BSP<C_d3d_texture> texture_moji = G_moji_manager.load_moji(m_moji.type, m_moji.code, m_moji.size, C_argb(255, 255, 255, 255), moji_face_type, Gp_ini->tateyoko_mode == 1, 0);
	m_spr_moji.set_texture(texture_moji);
	if (!texture_moji)
		return false;

	// �����e�N�X�`�����ǂ߂Ă��Ȃ���Ύ��s
	if (m_spr_moji.get_texture())
	{
		S_tnm_render_param moji_trp;
		moji_trp.disp = true;
		moji_trp.sorter = S_tnm_sorter(0, Gp_ini->mwnd.moji_layer_rep);
		moji_trp.pos_x = (float)(m_moji.pos.x + pos_rep.x);
		moji_trp.pos_y = (float)(m_moji.pos.y + pos_rep.y);
		moji_trp.dark = dark_rep;

		// �e�̃����_�[�p�����[�^��������
		tnm_add_parent_trp(&moji_trp, parent_trp);

		// �����X�v���C�g�̃����_�[�p�����[�^
		tnm_trp_to_rp(&moji_trp, &m_spr_moji.rpex, &m_spr_moji.rp);
		m_spr_moji.rp.d2_rect = true;
		m_spr_moji.rp.size_fit_to_texture = 0;

		// �G�����^�C�v�`�̏ꍇ�͐F��ύX���Ȃ�
		if (m_moji.type != TNM_MOJI_TYPE_EMOJI_A)
		{
			// �F���w�肳��Ă���΂��̐F��
			if (color_rate > 0)
			{
				m_spr_moji.rp.color_b = color.b;
				m_spr_moji.rp.color_g = color.g;
				m_spr_moji.rp.color_r = color.r;
				m_spr_moji.rp.color_rate = color_rate;
			}
			else
			{
				C_argb org_color = Gp_color_table->get_color(m_moji.color);

				// �ʏ�̕����ŐF�����Ȃ�F�ω����Ȃ��i�������̂��߁j
				if (!(m_moji.type == TNM_MOJI_TYPE_MOJI && org_color == C_argb(255, 255, 255, 255)))
				{
					m_spr_moji.rp.color_b = org_color.b;
					m_spr_moji.rp.color_g = org_color.g;
					m_spr_moji.rp.color_r = org_color.r;
					m_spr_moji.rp.color_rate = 255;
				}
			}
		}
	}

	// �e
	if (Gp_global->total_font_shadow == TNM_FONT_SHADOW_MODE_SHADOW || Gp_global->total_font_shadow == TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW)
	{
		// �X�L�b�v���͉e��\�����Ȃ�
		if (draw_shadow)
		{
			// �e�X�v���C�g���쐬����
			m_spr_shadow.create_polygon_no_init();
			m_spr_shadow.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
			m_spr_shadow.set_index_buffer(G_rect_index_buffer);

			// ���^�C�v
			MOJI_FACE_TYPE moji_face_type;
			if (draw_futoku && m_moji.type == TNM_MOJI_TYPE_MOJI)	// �G�����̏ꍇ�͑����Ȃ�Ȃ�
			{
				if (Gp_global->total_font_shadow == TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW)
				{
					moji_face_type = MOJI_FACE_TYPE_16;
				}
				else
				{
					moji_face_type = MOJI_FACE_TYPE_4;
				}
			}
			else
			{
				if (Gp_global->total_font_shadow == TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW)
				{
					moji_face_type = MOJI_FACE_TYPE_9;
				}
				else
				{
					moji_face_type = MOJI_FACE_TYPE_NONE;
				}
			}

			// �e�e�N�X�`���i�Ǝ��̕����}�l�[�W������擾����j
			BSP<C_d3d_texture> texture_shadow = G_moji_manager.load_moji(m_moji.type, m_moji.code, m_moji.size, C_argb(255, 255, 255, 255), moji_face_type, Gp_ini->tateyoko_mode == 1, 1);
			m_spr_shadow.set_texture(texture_shadow);
			if (!texture_shadow)
				return false;

			// �e�e�N�X�`�����ǂ߂Ă��Ȃ���Ύ��s
			if (m_spr_shadow.get_texture())
			{
				float rep_x = 0.0;
				float rep_y = 0.0;

				if (Gp_global->total_font_shadow == TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW)
				{
					rep_x = linear_limit(m_moji.size, 0, 0.5f, 32, 1.5f) - 1.0f;	// 2.0f �� 1.5f �ɏC��
					rep_y = linear_limit(m_moji.size, 0, 0.5f, 32, 1.5f) - 1.0f;	// 2.0f �� 1.5f �ɏC��
				}
				else
				{
					rep_x = linear_limit(m_moji.size, 0, 0.5f, 32, 2.0f);
					rep_y = linear_limit(m_moji.size, 0, 0.5f, 32, 2.0f);
				}

				S_tnm_render_param shadow_trp;
				shadow_trp.disp = true;
				shadow_trp.sorter = S_tnm_sorter(0, Gp_ini->mwnd.shadow_layer_rep);
				shadow_trp.pos_x = (float)(m_moji.pos.x + pos_rep.x) + rep_x;
				shadow_trp.pos_y = (float)(m_moji.pos.y + pos_rep.y) + rep_y;
				shadow_trp.dark = dark_rep;

				// �e�̃����_�[�p�����[�^��������
				tnm_add_parent_trp(&shadow_trp, parent_trp);

				// �����X�v���C�g�̃����_�[�p�����[�^
				tnm_trp_to_rp(&shadow_trp, &m_spr_shadow.rpex, &m_spr_shadow.rp);
				m_spr_shadow.rp.d2_rect = true;
				m_spr_shadow.rp.size_fit_to_texture = 0;

				// �F���w��

				// �F���w�肳��Ă���΂��̐F��
				if (color_rate > 0)
				{
					m_spr_shadow.rp.color_b = shadow_color.b;
					m_spr_shadow.rp.color_g = shadow_color.g;
					m_spr_shadow.rp.color_r = shadow_color.r;
					m_spr_shadow.rp.color_rate = color_rate;
				}
				else
				{
					C_argb color = Gp_color_table->get_color(m_moji.shadow);
					m_spr_shadow.rp.color_b = color.b;
					m_spr_shadow.rp.color_g = color.g;
					m_spr_shadow.rp.color_r = color.r;
					m_spr_shadow.rp.color_rate = 255;
				}
			}
		}
	}

	// ��
	if (Gp_global->total_font_shadow == TNM_FONT_SHADOW_MODE_FUTIDORU || Gp_global->total_font_shadow == TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW)
	{
		// �X�L�b�v���͉e��\�����Ȃ�
		if (draw_shadow)
		{
			// ���X�v���C�g���쐬����
			m_spr_fuchi.create_polygon_no_init();
			m_spr_fuchi.set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
			m_spr_fuchi.set_index_buffer(G_rect_index_buffer);

			// ���e�N�X�`���i�Ǝ��̕����}�l�[�W������擾����j
			MOJI_FACE_TYPE moji_face_type;
			if (draw_futoku && m_moji.type == TNM_MOJI_TYPE_MOJI)	// �G�����̏ꍇ�͑����Ȃ�Ȃ�
			{
				moji_face_type = MOJI_FACE_TYPE_16;
			}
			else
			{
				moji_face_type = MOJI_FACE_TYPE_9;
			}

			BSP<C_d3d_texture> texture_fuchi = G_moji_manager.load_moji(m_moji.type, m_moji.code, m_moji.size, C_argb(255, 255, 255, 255), moji_face_type, Gp_ini->tateyoko_mode == 1, 1);
			m_spr_fuchi.set_texture(texture_fuchi);
			if (!texture_fuchi)
				return false;

			// ���e�N�X�`�����ǂ߂Ă��Ȃ���Ύ��s
			if (m_spr_fuchi.get_texture())
			{
				float rep_x = -1.0;
				float rep_y = -1.0;

				S_tnm_render_param fuchi_trp;
				fuchi_trp.disp = true;
				fuchi_trp.sorter = S_tnm_sorter(0, Gp_ini->mwnd.fuchi_layer_rep);
				fuchi_trp.pos_x = (float)(m_moji.pos.x + pos_rep.x) + rep_x;
				fuchi_trp.pos_y = (float)(m_moji.pos.y + pos_rep.y) + rep_y;
				fuchi_trp.dark = dark_rep;

				// �e�̃����_�[�p�����[�^��������
				tnm_add_parent_trp(&fuchi_trp, parent_trp);

				// �����X�v���C�g�̃����_�[�p�����[�^
				tnm_trp_to_rp(&fuchi_trp, &m_spr_fuchi.rpex, &m_spr_fuchi.rp);
				m_spr_fuchi.rp.d2_rect = true;
				m_spr_fuchi.rp.size_fit_to_texture = 0;

				// �F���w��

				// �F���w�肳��Ă���΂��̐F��
				if (color_rate > 0)
				{
					m_spr_fuchi.rp.color_b = fuchi_color.b;
					m_spr_fuchi.rp.color_g = fuchi_color.g;
					m_spr_fuchi.rp.color_r = fuchi_color.r;
					m_spr_fuchi.rp.color_rate = color_rate;
				}
				else
				{
					C_argb color = Gp_color_table->get_color(m_moji.fuchi);
					m_spr_fuchi.rp.color_b = color.b;
					m_spr_fuchi.rp.color_g = color.g;
					m_spr_fuchi.rp.color_r = color.r;
					m_spr_fuchi.rp.color_rate = 255;
				}
			}
		}
	}

	return true;
}

// ****************************************************************
// �e�N�X�`���[�N���A
// ================================================================
void C_elm_mwnd_moji::clear_texture()
{
	m_spr_moji.clear_texture();
	m_spr_shadow.clear_texture();
	m_spr_fuchi.clear_texture();
}

// ****************************************************************
// �X�v���C�g�c���[���擾
// ================================================================
void C_elm_mwnd_moji::get_sprite_tree(C_tree2<C_tnm_sprite *>& moji_root, C_tree2<C_tnm_sprite *>& shadow_root)
{
	if (m_spr_moji.rp.disp)	{
		moji_root.add_child(&m_spr_moji);
	}
	if (m_spr_shadow.rp.disp)	{
		shadow_root.add_child(&m_spr_shadow);
	}
	if (m_spr_fuchi.rp.disp)	{
		shadow_root.add_child(&m_spr_fuchi);
	}
}

// ****************************************************************
// �����̐ݒ�
// ================================================================
void C_elm_mwnd_moji::set_moji(int moji_type, int moji_code, int moji_size, int moji_color, int shadow_color, int fuchi_color, C_point moji_pos)
{
	// �`�^�C�v�̊G�����ɂ͐F�͂��Ȃ�
	if (moji_type == TNM_MOJI_TYPE_EMOJI_A)	{
		moji_color = 0;
	}

	m_moji.type = moji_type;		// �����^�C�v
	m_moji.code = moji_code;		// �����R�[�h
	m_moji.size = moji_size;		// �����T�C�Y
	m_moji.color = moji_color;		// �����F
	m_moji.shadow = shadow_color;	// �e�F
	m_moji.fuchi = fuchi_color;		// ���F
	m_moji.pos = moji_pos;			// �����ʒu
}

// ****************************************************************
// �o���t���O��ݒ�
// ================================================================
void C_elm_mwnd_moji::set_appear(bool flag)
{
	m_appeared_flag = flag;
}
