#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_global_data.h"
#include	"element/elm_object.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// �I�u�W�F�N�g�F���l�p�����[�^�̏�����
// ================================================================
void C_elm_object::init_number_param()
{
	m_op.number.number = 0;				// ���l
	m_op.number.keta_max = 0;			// �ő包��
	m_op.number.disp_zero = false;		// �擪�� 0 ��\��
	m_op.number.disp_sign = false;		// ������\��
	m_op.number.tumeru_sign = false;	// �������߂ĕ\��
	m_op.number.space_mod = 0;			// �Ԋu�^�C�v
	m_op.number.space = 0;				// �Ԋu
}

// ****************************************************************
// �I�u�W�F�N�g�F���l�p�����[�^�̐ݒ�
// ================================================================
void C_elm_object::set_number_param(int keta, int disp_zero, int disp_sign, int tumeru_sign, int space_mod, int space)
{
	m_op.number.keta_max = keta;			// ����
	m_op.number.disp_zero = disp_zero;		// �O��\��
	m_op.number.disp_sign = disp_sign;		// ������\��
	m_op.number.tumeru_sign = tumeru_sign;	// �������l�߂�
	m_op.number.space_mod = space_mod;		// �X�y�[�X�̃��[�h
	m_op.number.space = space;				// �X�y�[�X
}

// ****************************************************************
// �I�u�W�F�N�g�F���l�I�u�W�F�N�g�̍쐬
// ================================================================
bool C_elm_object::create_number(TSTR file_path)
{
	// ���ɍ���Ă���ꍇ�͉������Ȃ�
	if (m_op.type == TNM_OBJECT_TYPE_NUMBER && m_op.file_path == file_path)
		return true;

	// �^�C�v�ʂ̃p�����[�^������������
	init_type(true);

	// �p�����[�^��ݒ�
	m_op.type = TNM_OBJECT_TYPE_NUMBER;
	m_op.file_path = file_path;

	// ���l�I�u�W�F�N�g���č\�z����
	if (!restruct_number())
		return false;

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F���l�I�u�W�F�N�g���č\�z
// ================================================================
bool C_elm_object::restruct_number()
{
	// �e�N�X�`����ǂݍ���
	m_album = tnm_load_pct_d3d(m_op.file_path, false, m_op_def.set_cg_table);
	if (!m_album)	{
		m_op.file_path.clear();		// ���s�����̂Ńt�@�C�������N���A
		return false;
	}

	// �X�v���C�g���쐬
	m_sprite_list.resize(m_sprite_cnt_number);
	for (int i = 0; i < m_sprite_cnt_number; i++)	{
		if (!m_sprite_list[i].create_polygon_no_init())
			return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���l�I�u�W�F�N�g�̍쐬�Ɏ��s���܂����B"));

		m_sprite_list[i].set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
		m_sprite_list[i].set_index_buffer(G_rect_index_buffer);
	}

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F���l�I�u�W�F�N�g�̃t���[������
// ================================================================
void C_elm_object::number_frame()
{
	int pat_no[m_sprite_cnt_number];

	// �X�v���C�g�̐ݒ�
	for (int i = 0; i < m_sprite_cnt_number; i++)	{
		trp_to_rp(&m_sprite_list[i].rpex, &m_sprite_list[i].rp);
		m_sprite_list[i].rp.disp = false;	// ��U��\���ɂ���
		pat_no[i] = 0;						// �p�^�[���ԍ���������
	}

	// ���l�̐ݒ�
	if (m_trp.disp && m_album)	{
		int num = abs(m_op.number.number);
		int sign = get_sign(m_op.number.number);
		bool disp_sign = m_op.number.disp_sign || sign == -1;
		bool tumeru_sign = m_op.number.tumeru_sign && !m_op.number.disp_zero;
		int keta_max = m_op.number.keta_max;
		int keta = get_keta(num);			// ���������߂�
		BSA<int> digit(new int[keta]);		// ������p�̃o�b�t�@
		get_keta(num, digit.get(), keta);	// ������

		// 0 ��\������ꍇ�́A��� 0 �Ŗ��߂�
		if (m_op.number.disp_zero)	{
			for (int i = 0; i < keta_max; i++)	{
				pat_no[i] = 0;
				m_sprite_list[i].rp.disp = true;
			}
		}
		// �����Ɛ��l�̈ʒu�����߂�
		int num_pos, sign_pos = -1;
		num_pos = std::max(keta_max - keta, 0);
		if (disp_sign)	{
			if (tumeru_sign)	sign_pos = std::max(num_pos - 1, 0);
			else				sign_pos = 0;
			// ���l�̈ʒu��␳
			num_pos = std::max(num_pos, sign_pos + 1);
		}
		// ���l�X�v���C�g�̐ݒ�
		for (int i = 0; i < keta; i++)	{
			pat_no[num_pos + i] = digit[i];
			m_sprite_list[num_pos + i].rp.disp = true;
		}
		// �����X�v���C�g�̐ݒ�
		if (disp_sign)	{
			if (false);
			else if (sign == 0)		pat_no[sign_pos] = 12;
			else if (sign == 1)		pat_no[sign_pos] = 11;
			else if (sign == -1)	pat_no[sign_pos] = 10;
			m_sprite_list[sign_pos].rp.disp = true;
		}
		// �p�^�[���ԍ����m�肷��
		for (int i = 0; i < m_sprite_cnt_number; i++)	{
			pat_no[i] += m_trp.pat_no;
		}
		// �p�^�[���ԍ�����e�N�X�`����ݒ肷��
		for (int i = 0; i < m_sprite_cnt_number; i++)	{
			if (m_sprite_list[i].rp.disp)	{
				m_sprite_list[i].set_texture(m_album->get_texture(pat_no[i]));
				m_sprite_list[i].rp.size_fit_to_texture = 0;
			}
		}
		// �X�v���C�g�̈ʒu�̐ݒ�
		int offset = 0;
		for (int i = 0; i < m_sprite_cnt_number; i++)	{
			m_sprite_list[i].rp.center.x -= offset;
			offset += m_op.number.space;
			if (m_op.number.space_mod == 0)	{		// �摜�T�C�Y���g��
				if (pat_no[i] >= 0 && m_sprite_list[i].get_texture(pat_no[i]))	{
					offset += m_album->get_texture(pat_no[i])->get_original_width();
				}
				else if (m_album->get_texture(0))	{
					offset += m_album->get_texture(0)->get_original_width();		// �\�����Ȃ��ꍇ�͂O�ԃe�N�X�`���̃T�C�Y���g��
				}
			}
		}
	}
}


