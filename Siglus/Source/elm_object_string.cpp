#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_def.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_color_table.h"
#include	"data/tnm_moji.h"
#include	"data/tnm_moji_manager.h"
#include	"element/elm_object.h"
#include	"engine/eng_config.h"
#include	"engine/eng_graphics.h"
#include	"engine/eng_message.h"

// ****************************************************************
// �I�u�W�F�N�g�F������I�u�W�F�N�g���쐬
// ================================================================
bool C_elm_object::create_string(TSTR str)
{
	// ���ɍ���Ă���ꍇ�͉������Ȃ�
	if (m_op.type == TNM_OBJECT_TYPE_STRING && m_op.str == str)
		return true;

	// �^�C�v�ʂ̃p�����[�^������������
	init_type(true);

	// �p�����[�^��ݒ�
	m_op.type = TNM_OBJECT_TYPE_STRING;
	m_op.str = str;

	// ������I�u�W�F�N�g���\�z����
	if (!restruct_string())
		return false;

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F�������ݒ�
// ================================================================
bool C_elm_object::set_string(TSTR str)
{
	// ������I�u�W�F�N�g�łȂ������牽�����Ȃ�
	if (m_op.type != TNM_OBJECT_TYPE_STRING)
		return false;

	// �ύX����Ă��Ȃ��ꍇ�͉������Ȃ�
	if (m_op.str == str)
		return true;

	// �^�C�v�ʂ̃p�����[�^���������i�����̂݁j
	free_type(false);

	// �p�����[�^��ݒ�
	m_op.str = str;

	// ������I�u�W�F�N�g���\�z����
	if (!restruct_string())
		return false;

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F������p�����[�^��ݒ�
// ================================================================
bool C_elm_object::set_string_param(int moji_size, int moji_space_x, int moji_space_y, int moji_cnt, int moji_color, int shadow_color, int fuchi_color, int shadow_mode)
{
	// ������I�u�W�F�N�g�łȂ������牽�����Ȃ�
	if (m_op.type != TNM_OBJECT_TYPE_STRING)
		return true;

	// �ύX����Ă��Ȃ��ꍇ�͉������Ȃ�
	if (m_op.string.moji_size == moji_size && m_op.string.moji_space_x == moji_space_x && m_op.string.moji_space_y == moji_space_y && m_op.string.moji_cnt == moji_cnt && m_op.string.moji_color == moji_color && m_op.string.shadow_color == shadow_color && m_op.string.fuchi_color == fuchi_color && m_op.string.shadow_mode == shadow_mode)
		return true;

	// �^�C�v�ʂ̃p�����[�^���������i�����̂݁j
	free_type(false);

	// �p�����[�^��ݒ�
	m_op.string.moji_size = moji_size;
	m_op.string.moji_space_x = moji_space_x;
	m_op.string.moji_space_y = moji_space_y;
	m_op.string.moji_cnt = moji_cnt;
	m_op.string.moji_color = moji_color;
	m_op.string.shadow_color = shadow_color;
	m_op.string.fuchi_color = fuchi_color;
	m_op.string.shadow_mode = shadow_mode;

	// ������I�u�W�F�N�g���\�z����
	if (!restruct_string())
		return false;

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F������I�u�W�F�N�g���č\�z
// ================================================================
bool C_elm_object::restruct_string()
{
	// �������X�g���쐬
	int num = 0, cnt = 0, cur_x = 0, cur_y = 0, color_no = m_op.string.moji_color, size = m_op.string.moji_size;
	for (TSTR::const_iterator itr = m_op.str.begin(), end = m_op.str.end(); itr != m_op.str.end(); )	{
		int moji_type = TNM_MOJI_TYPE_MOJI;
		int moji_code = 0;

		// �R�}���h
		if (str_analize_check_moji(itr, end, _T('#')))	{
			if (str_analize_check_moji(itr, end, _T('#')))	{
				moji_code = _T('#');		// ## �� # ���o�͂���BRealLive �ƈقȂ�̂Œ��ӁI
			}
			else if (str_analize_check_moji(itr, end, _T('D')))	{		// ���s
				cnt = 0;

				if (Gp_ini->tateyoko_mode == 1)
				{
					cur_x -= size + m_op.string.moji_space_y;
					cur_y = 0;
				}
				else
				{
					cur_x = 0;
					cur_y += size + m_op.string.moji_space_y;
				}
				continue;
			}
			else if (str_analize_get_integer(itr, end, &num))	{

				if (false);
				else if (str_analize_check_moji(itr, end, _T('C')))	{
					color_no = num;		// �F��ݒ�
					continue;
				}
				else if (str_analize_check_moji(itr, end, _T('S')))	{
					size = num;			// �T�C�Y��ݒ�
					continue;
				}
				else if (str_analize_check_str(itr, end, _T("RX")))	{
					cur_x += num;		// �w���W���w��
					continue;
				}
				else if (str_analize_check_moji(itr, end, _T('X')))	{
					cur_x = num;		// �w���W���w��
					continue;
				}
				else if (str_analize_check_str(itr, end, _T("RY")))	{
					cur_y += num;		// �x���W���w��
					continue;
				}
				else if (str_analize_check_moji(itr, end, _T('Y')))	{
					cur_y = num;		// �x���W���w��
					continue;
				}
			}
		}
		// �ʏ�̕����܂��͊G����
		else	{
			tnm_msg_proc_expand_name_flag(itr, end, &moji_type, &moji_code);
		}

		// ���肫��Ȃ���Ή��s����im_op.string.moji_cnt <= 0 �̂Ƃ��͉��s���Ȃ��j
		if (m_op.string.moji_cnt > 0)	{
			int new_cnt = 2;
			if (moji_type == TNM_MOJI_TYPE_MOJI && is_hankaku((TCHAR)moji_code))	{
				new_cnt = 1;	// �����ł͔��p���P�����Ɛ����Ă܂�
			}
			if (cnt + new_cnt > m_op.string.moji_cnt * 2)	{
				cnt = 0;

				if (Gp_ini->tateyoko_mode == 1)
				{
					cur_x -= size + m_op.string.moji_space_y;
					cur_y = 0;
				}
				else
				{
					cur_x = 0;
					cur_y += size + m_op.string.moji_space_y;
				}
			}
		}

		// �p�����[�^�̐ݒ�
		C_elm_object_param_moji moji_param;
		moji_param.type = moji_type;	// �����^�C�v
		moji_param.code = moji_code;	// �����R�[�h
		moji_param.pos.x = cur_x;		// �ʒu�w
		moji_param.pos.y = cur_y;		// �ʒu�x
		moji_param.color = Gp_color_table->get_color(color_no);	// �F
		moji_param.size = size;			// �傫��

		// ���݈ʒu��i�߂�
		if (Gp_ini->tateyoko_mode == 1)
		{
			if (moji_type != TNM_MOJI_TYPE_MOJI)	{ cnt += 2;	cur_y += size + m_op.string.moji_space_x; }
			if (is_hankaku((TCHAR)moji_code))		{ cnt += 1;	cur_y += (size + 1) / 2 + m_op.string.moji_space_x; }
			else									{ cnt += 2;	cur_y += size + m_op.string.moji_space_x; }
		}
		else
		{
			if (moji_type != TNM_MOJI_TYPE_MOJI)	{ cnt += 2;	cur_x += size + m_op.string.moji_space_x; }
			if (is_hankaku((TCHAR)moji_code))		{ cnt += 1;	cur_x += (size + 1) / 2 + m_op.string.moji_space_x; }
			else									{ cnt += 2;	cur_x += size + m_op.string.moji_space_x; }
		}

		// �������X�g�ɒǉ�
		m_moji_list.push_back(moji_param);
	}

	// �X�v���C�g�̍쐬
	m_sprite_list.resize(m_moji_list.size() * 3);	// * 3: �e
	for (int i = 0; i < (int)m_moji_list.size() * 3; i++)	{
		if (!m_sprite_list[i].create_polygon_no_init())
			return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("������I�u�W�F�N�g�̍쐬�Ɏ��s���܂����B"));

		m_sprite_list[i].set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
		m_sprite_list[i].set_index_buffer(G_rect_index_buffer);
	}

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F������I�u�W�F�N�g�̃t���[������
// ================================================================
void C_elm_object::string_frame()
{
	// �����̐ݒ�
	int moji_cnt = (int)m_moji_list.size();
	for (int i = 0; i < moji_cnt; i++)
	{
		// ��{�p�����[�^��ݒ�
		trp_to_rp(&m_sprite_list[i].rpex, &m_sprite_list[i].rp);									// �e
		m_sprite_list[i].rp.size_fit_to_texture = 0;
		trp_to_rp(&m_sprite_list[i + moji_cnt].rpex, &m_sprite_list[i + moji_cnt].rp);				// ��
		m_sprite_list[i + moji_cnt].rp.size_fit_to_texture = 0;
		trp_to_rp(&m_sprite_list[i + moji_cnt * 2].rpex, &m_sprite_list[i + moji_cnt * 2].rp);		// ����
		m_sprite_list[i + moji_cnt * 2].rp.size_fit_to_texture = 0;

		int shadow_mode = (m_op.string.shadow_mode == -1) ? Gp_global->total_font_shadow : m_op.string.shadow_mode;

		// �e�e�N�X�`����ǂݍ��ށi�e�^���{�e�j
		if (shadow_mode == TNM_FONT_SHADOW_MODE_SHADOW || shadow_mode == TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW)
		{
			// �����̃^�C�v�����߂�i�ׂ��F�܂Ȃ�X�����A�e�Ȃ�P�����^�����F�܂Ȃ�P�U�����A�e�Ȃ�S�����j
			MOJI_FACE_TYPE moji_face_type;
			if (Gp_global->total_font_futoku)
			{
				if (shadow_mode == TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW)
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
				if (shadow_mode == TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW)
				{
					moji_face_type = MOJI_FACE_TYPE_9;
				}
				else
				{
					moji_face_type = MOJI_FACE_TYPE_NONE;
				}
			}

			BSP<C_d3d_texture> texture_shadow = G_moji_manager.load_moji(m_moji_list[i].type, m_moji_list[i].code, m_moji_list[i].size, Gp_color_table->get_color(m_op.string.shadow_color), moji_face_type, Gp_ini->tateyoko_mode == 1, 1);
			if (texture_shadow)
			{
				float rep_x = 0.0;
				float rep_y = 0.0;

				if (shadow_mode == TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW)
				{
					rep_x = linear_limit(m_moji_list[i].size, 0, 0.5f, 32, 1.5f) - 1.0f;	// 2.0f �� 1.5f �ɏC��
					rep_y = linear_limit(m_moji_list[i].size, 0, 0.5f, 32, 1.5f) - 1.0f;	// 2.0f �� 1.5f �ɏC��
				}
				else
				{
					rep_x = linear_limit(m_moji_list[i].size, 0, 0.5f, 32, 2.0f);
					rep_y = linear_limit(m_moji_list[i].size, 0, 0.5f, 32, 2.0f);
				}

				// �ʒu�����߂�
				//float rep_x = linear_limit(m_moji_list[i].size, 0, 0.5f, 32, 2.0f);
				//float rep_y = rep_x;
				
				// �c�����̏ꍇ�͋N�_���E�ォ�獶��ɕϊ�
				if (Gp_ini->tateyoko_mode == 1)
				{
					rep_x -= (float)m_moji_list[i].size;
				}

				// �e�N�X�`����ݒ�
				m_sprite_list[i].set_texture(texture_shadow);
				m_sprite_list[i].rp.center.x -= (float)m_moji_list[i].pos.x + rep_x;
				m_sprite_list[i].rp.center.y -= (float)m_moji_list[i].pos.y + rep_y;
			}
		}

		// ���e�N�X�`����ǂݍ���
		if (shadow_mode == TNM_FONT_SHADOW_MODE_FUTIDORU || shadow_mode == TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW)
		{
			// �����̃^�C�v�����߂�i�ׂ��F�܂Ȃ�X�����A�e�Ȃ�P�����^�����F�܂Ȃ�P�U�����A�e�Ȃ�S�����j
			MOJI_FACE_TYPE moji_face_type;
			if (Gp_global->total_font_futoku)
			{
				moji_face_type = MOJI_FACE_TYPE_16;
			}
			else
			{
				moji_face_type = MOJI_FACE_TYPE_9;
			}

			BSP<C_d3d_texture> texture_shadow = G_moji_manager.load_moji(m_moji_list[i].type, m_moji_list[i].code, m_moji_list[i].size, Gp_color_table->get_color(m_op.string.fuchi_color), moji_face_type, Gp_ini->tateyoko_mode == 1, 1);
			if (texture_shadow)	{

				// �ʒu�����߂�
				float rep_x = -1.0f;
				float rep_y = rep_x;

				// �c�����̏ꍇ�͋N�_���E�ォ�獶��ɕϊ�
				if (Gp_ini->tateyoko_mode == 1)
				{
					rep_x -= (float)m_moji_list[i].size;
				}

				// �e�N�X�`����ݒ�
				m_sprite_list[i + moji_cnt].set_texture(texture_shadow);
				m_sprite_list[i + moji_cnt].rp.center.x -= (float)m_moji_list[i].pos.x + rep_x;
				m_sprite_list[i + moji_cnt].rp.center.y -= (float)m_moji_list[i].pos.y + rep_y;
			}
		}

		// �����e�N�X�`����ǂݍ���
		{
			// �����̃^�C�v�����߂�i�ׂ��F�P�����^�����F�S�����j
			MOJI_FACE_TYPE moji_face_type = Gp_global->total_font_futoku ? MOJI_FACE_TYPE_4 : MOJI_FACE_TYPE_NONE;

			// �����e�N�X�`����ǂݍ���
			BSP<C_d3d_texture> texture_moji = G_moji_manager.load_moji(m_moji_list[i].type, m_moji_list[i].code, m_moji_list[i].size, m_moji_list[i].color, moji_face_type, Gp_ini->tateyoko_mode == 1, 0);
			if (texture_moji)	{

				// �ʒu�����߂�
				float rep_x = 0.0f;
				float rep_y = 0.0f;

				// �c�����̏ꍇ�͋N�_���E�ォ�獶��ɕϊ�
				if (Gp_ini->tateyoko_mode == 1)
				{
					rep_x -= (float)m_moji_list[i].size;
				}

				// �e�N�X�`����ݒ�
				m_sprite_list[i + moji_cnt * 2].set_texture(texture_moji);
				m_sprite_list[i + moji_cnt * 2].rp.center.x -= m_moji_list[i].pos.x + rep_x;
				m_sprite_list[i + moji_cnt * 2].rp.center.y -= m_moji_list[i].pos.y + rep_y;
			}
		}
	}
}
