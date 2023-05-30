#include	"g00_pch.h"
#include	"g00.h"
#include	"unpack.h"
#include	"jpeg.h"

std::vector<BYTE> C_g00::m_unpack_data(2 * 1024 * 1024);		// 2MB ���炢�Ƃ��Ƃ�
std::vector<BYTE> C_g00_chip::m_unpack_buffer(2 * 1024 * 1024);	// 2MB ���炢�Ƃ��Ƃ�

// ****************************************************************
// G00 �w�b�_�\����
// ================================================================

// psd �ł̌��ʒu�B�r���[�A�Ȃǂŕ�������Ƃ��Ɏg�p�B
// �Q�[����ł͂܂��g��Ȃ��B
struct G00_CUT_DATABASE
{
	int		sx;
	int		sy;
	int		ex;
	int		ey;
	int		cx;
	int		cy;
};

// �J�b�g�f�[�^�̃w�b�_
struct G00_CUT_HEADER_STRUCT
{
	BYTE	type;		// 0:�x�^ 1:�C���f�b�N�X�J���[ 2:�J�b�g
	WORD	count;		// �J�b�g��
	int		x;			// �\���̈�w
	int		y;			// �\���̈�x
	int		disp_xl;	// �\���̈�̕�
	int		disp_yl;	// �\���̈�̍���
	int		xc;			// ���S�w
	int		yc;			// ���S�x
	int		cut_xl;		// �J�b�g�S�̂̕�
	int		cut_yl;		// �J�b�g�S�̂̍���
	int		keep[20];
};

// �`�b�v�f�[�^�̃w�b�_
struct G00_CHIP_HEADER_STRUCT
{
	WORD	x;			// �`�b�v�w�i���_�F�J�b�g�S�́j
	WORD	y;			// �`�b�v�w�i���_�F�J�b�g�S�́j
	BYTE	type;		// 0:�x�^ 1:���߂���
	WORD	xl;			// �`�b�v�̕�
	WORD	yl;			// �`�b�v�̍���
	int		keep[20];
};

// ****************************************************************
// g00
// ================================================================
C_g00::C_g00()
{
	m_g00_type = 0;
}

// ****************************************************************
// g00�F�f�[�^�Z�b�g
// ================================================================
bool C_g00::set_data(BYTE* file_data, int data_size)
{
	// �T���|�C���^�̃Z�b�g
	BYTE* p = file_data;
	// G00 �̃^�C�v��ǂݍ���
	m_g00_type = *p++;

	// �^�C�v�ɂ���ĕ���
	if (false);
	// �^�C�v�O�F�x�^�f�[�^
	else if (m_g00_type == 0)	{

		// �J�b�g�͂P��
		m_cut_list.resize(1);
		// �J�b�g�̍쐬
		m_cut_list[0].set_data(m_g00_type, p, data_size - 1);
	}
	// �^�C�v�P�F�C���f�b�N�X�J���[
	else if (m_g00_type == 1)	{

		// �J�b�g�͂P��
		m_cut_list.resize(1);
		// �J�b�g�̍쐬
		m_cut_list[0].set_data(m_g00_type, p, data_size - 1);
	}
	// �^�C�v�Q�F�J�b�g�f�[�^
	else if (m_g00_type == 2)	{

		int width = *(WORD *)p;		p += 2;			// ��
		int height = *(WORD *)p;	p += 2;			// ����
		int cut_cnt = *(int *)p;	p += 4;			// �J�b�g��
		p += sizeof(G00_CUT_DATABASE) * cut_cnt;	// �J�b�g�����p�w�b�_

		int unpack_size = LzssUnPack(p, NULL);		// �W�J�T�C�Y
		if ((int)m_unpack_data.size() < unpack_size + 64)	{
			m_unpack_data.resize(unpack_size + 64);		// �W�J�o�b�t�@�m�ہi+64�F�\���j
		}
		LzssUnPack(p, &m_unpack_data[0]);			// �W�J
		p = &m_unpack_data[0];

		// �J�b�g���X�g���쐬
		m_cut_list.resize(cut_cnt);
		for (int cut_no = 0; cut_no < cut_cnt; cut_no++)	{

			// �J�b�g�f�[�^�̐擪���擾
			int size = 0;
			BYTE* p_cut_data = get_cut_data_point(p, cut_no, &size);

			// �J�b�g�̏�����
			if (p_cut_data)
				m_cut_list[cut_no].set_data(m_g00_type, p_cut_data, size);
		}
	}
	// �^�C�v�R�FJpeg �f�[�^
	else if (m_g00_type == 3)	{

		// �J�b�g�͂P��
		m_cut_list.resize(1);
		// �J�b�g�̍쐬
		m_cut_list[0].set_data(m_g00_type, p, data_size - 1);
	}

	return true;
}

// ****************************************************************
// g00�F�J�b�g�f�[�^�̐擪�|�C���^���擾
// ================================================================
BYTE* C_g00::get_cut_data_point(BYTE* g00_data, int cut_no, int* ret_size)
{
	if (g00_data == NULL)
		return false;

	DWORD offset;
	int size;

	// �T���|�C���^�iUINT)
	UINT* uip = (UINT *)g00_data;

	int cut_cnt = *uip++;
	if (0 <= cut_no && cut_no < cut_cnt)	{
		offset = *(uip + cut_no * 2);
		size = *(uip + cut_no * 2 + 1);

		if (offset > 0 && size != 0)		// size ���}�C�i�X�̎��͓��f�[�^�̃����N
		{
			if (ret_size)
			{
				*ret_size = size;
			}

			return g00_data + offset;
		}
	}

	return NULL;
}

// ****************************************************************
// g00�F���̎擾
// ================================================================
void C_g00::get_info(S_g00_info* info)
{
	info->cut_cnt = (int)m_cut_list.size();
}

// ****************************************************************
// g00�F�J�b�g�f�[�^�̎擾
// ================================================================
C_g00_cut* C_g00::get_cut(int cut_no)
{
	return &m_cut_list[cut_no];
}

// ****************************************************************
// g00 �J�b�g�F������
// ================================================================
void C_g00_cut::init()
{
	m_g00_type = 0;
	m_width = 0;
	m_height = 0;
	m_chip_list.clear();
}

// ****************************************************************
// g00 �J�b�g�F�f�[�^�Z�b�g
// ================================================================
void C_g00_cut::set_data(int g00_type, BYTE* cut_data, int data_size)
{
	init();

	m_g00_type = g00_type;

	// �T���|�C���^�̃Z�b�g
	BYTE* p = cut_data;

	if (false);
	else if (m_g00_type == 0 || m_g00_type == 1 || m_g00_type == 3)	{

		// ���̐ݒ�
		m_width = *(WORD *)p;	p += 2;				// ��
		m_height = *(WORD *)p;	p += 2;				// ����
		m_center.x = 0;
		m_center.y = 0;
		m_disp_rect.left = 0;
		m_disp_rect.top = 0;
		m_disp_rect.right = m_width;
		m_disp_rect.bottom = m_height;

		// �`�b�v�͂P��
		m_chip_list.resize(1);
		m_chip_list[0].set_data_type0or1(m_g00_type, p, data_size - 4, m_width, m_height);
	}
	else if (m_g00_type == 2)	{

		// �J�b�g�f�[�^�w�b�_�̎擾
		G00_CUT_HEADER_STRUCT* cut_header = (G00_CUT_HEADER_STRUCT *)p;
		p += sizeof(G00_CUT_HEADER_STRUCT);

		// ���̐ݒ�
		m_width = cut_header->cut_xl;
		m_height = cut_header->cut_yl;
		m_center.x = cut_header->xc;
		m_center.y = cut_header->yc;
		m_disp_rect.left = cut_header->x;
		m_disp_rect.top = cut_header->y;
		m_disp_rect.right = cut_header->x + cut_header->disp_xl;
		m_disp_rect.bottom = cut_header->y +  cut_header->disp_yl;

		// �`�b�v�̏����擾
		int chip_cnt = cut_header->count;
		m_chip_list.resize(chip_cnt);
		for (int chip_no = 0; chip_no < chip_cnt; chip_no++)
			m_chip_list[chip_no].set_data_type2(m_g00_type, p, &p);
	}
}

// ****************************************************************
// g00 �J�b�g�F�f�[�^�𔻒�
// ================================================================
bool C_g00_cut::is_data()
{
	return m_width > 0 && m_height > 0;
}

// ****************************************************************
// g00 �J�b�g�F���̎擾
// ================================================================
void C_g00_cut::get_info(S_g00_cut_info* info)
{
	info->width = m_width;
	info->height = m_height;
	info->center = m_center;
	info->disp_rect = m_disp_rect;
	info->chip_cnt = (int)m_chip_list.size();
}

// ****************************************************************
// g00 �J�b�g�F�`�b�v�̎擾
// ================================================================
C_g00_chip* C_g00_cut::get_chip(int chip_no)
{
	return &m_chip_list[chip_no];
}

// ****************************************************************
// g00 �`�b�v�F������
// ================================================================
void C_g00_chip::init()
{
	m_g00_type = 0;
	m_pos_x = 0;
	m_pos_y = 0;
	m_width = 0;
	m_height = 0;
	m_data = NULL;
	m_data_size = 0;
	m_sprite = true;
}

// ****************************************************************
// g00 �`�b�v�F�f�[�^�Z�b�g�i�^�C�v�O�A�^�C�v�P�j
// ================================================================
void C_g00_chip::set_data_type0or1(int g00_type_, BYTE* chip_data_, int data_size_, int width_, int height_)
{
	init();

	m_g00_type = g00_type_;
	m_pos_x = 0;
	m_pos_y = 0;
	m_width = width_;
	m_height = height_;
	m_data = chip_data_;
	m_data_size = data_size_;
	m_sprite = false;			// ���߂Ȃ�
}

// ****************************************************************
// g00 �`�b�v�F�f�[�^�Z�b�g�i�^�C�v�Q�j
// ================================================================
void C_g00_chip::set_data_type2(int g00_type_, BYTE* chip_data, BYTE** next_chip_data)
{
	init();

	m_g00_type = g00_type_;

	// �T���|�C���^�̃Z�b�g
	BYTE* p = chip_data;

	// �w�b�_�̉��
	G00_CHIP_HEADER_STRUCT* chip_header = (G00_CHIP_HEADER_STRUCT *)p;
	m_pos_x = chip_header->x;
	m_pos_y = chip_header->y;
	m_width = chip_header->xl;
	m_height = chip_header->yl;
	m_sprite = chip_header->type == 1 ? true : false;
	p += sizeof(G00_CHIP_HEADER_STRUCT);

	m_data = p;
	p += m_width * m_height * 4;

	// ���̃`�b�v�f�[�^�̈ʒu��Ԃ�
	*next_chip_data = p;
}

// ****************************************************************
// g00 �`�b�v�F���̎擾
// ================================================================
void C_g00_chip::get_info(S_g00_chip_info* info)
{
	info->x = m_pos_x;
	info->y = m_pos_y;
	info->width = m_width;
	info->height = m_height;
	info->sprite = m_sprite;
}

// ****************************************************************
// g00 �`�b�v�F�f�[�^�̎擾
// ================================================================

#define			GET_DATA_OLD_PROCESS		0

std::vector<BYTE>* G__buf;

bool C_g00_chip::get_data(BYTE* dst, int dst_pitch)
{
	if (m_data == NULL)
		return false;

	if (false);
	else if (m_g00_type == 0)	{

		// �k�y�r�r�W�J
		int unpack_size = LzssUnPack32(m_data, NULL);
		if ((int)m_unpack_buffer.size() < unpack_size)	{
			m_unpack_buffer.resize(unpack_size);
		}
		LzssUnPack32(m_data, &m_unpack_buffer[0]);
		
		// �`�b�v��ǂݍ���

#if GET_DATA_OLD_PROCESS
		for (int y = 0; y < height; y++)
			memcpy(dst + dst_pitch * y, &unpack_buffer[0] + width * 4 * y, width * 4);
#else
		BYTE* src = &m_unpack_buffer[0];
		int draw_cnt_x = m_width;
		int draw_cnt_y = m_height;
		int add_dst_addr = dst_pitch - (m_width * 4);

		// �`��{��
		_asm{
			cld
			mov		edi, dst
			mov		esi, src
			mov		ecx, draw_cnt_y
			mov		eax, draw_cnt_x
			mov		edx, add_dst_addr
		g00_type_0_loop_y:
			mov		ebx, ecx		// push ecx
			mov		ecx, eax
			rep		movsd
			add		edi, edx
			mov		ecx, ebx		// pop ecx
			loop	g00_type_0_loop_y
		}
#endif
	}
	else if (m_g00_type == 1)	{

		// �k�y�r�r�W�J
		int unpack_size = LzssUnPack(m_data, NULL);
		if ((int)m_unpack_buffer.size() < unpack_size)	{
			m_unpack_buffer.resize(unpack_size);
		}
		LzssUnPack(m_data, &m_unpack_buffer[0]);
		BYTE* p = &m_unpack_buffer[0];

		// �p���b�g�̓ǂݍ���
		int pal_cnt = *(WORD *)p;	p += 2;
		DWORD* pal = (DWORD *)p;	p += pal_cnt * 4;

		// �`�b�v��ǂݍ���
		for (int y = 0; y < m_height; y++)	{
			DWORD* dst_4 = (DWORD *)(dst + dst_pitch * y);
			for (int x = 0; x < m_width; x++)	{
				*dst_4++ = *(pal + *p++);
			}
		}
	}
	else if (m_g00_type == 2)	{

		// �`�b�v��ǂݍ���

#if GET_DATA_OLD_PROCESS
		for (int y = 0; y < height; y++)
			memcpy(dst + dst_pitch * y, data + width * 4 * y, width * 4);
#else
		BYTE* src = m_data;
		int draw_cnt_x = m_width;
		int draw_cnt_y = m_height;
		int add_dst_addr = dst_pitch - (m_width * 4);

		// �`��{��
		_asm{
			cld
			mov		edi, dst
			mov		esi, src
			mov		ecx, draw_cnt_y
			mov		eax, draw_cnt_x
			mov		edx, add_dst_addr
		g00_type_2_loop_y:
			mov		ebx, ecx		// push ecx
			mov		ecx, eax
			rep		movsd
			add		edi, edx
			mov		ecx, ebx		// pop ecx
			loop	g00_type_2_loop_y
		}
#endif
	}
	else if (m_g00_type == 3)	{

		Cva_jpeg jpeg_expander;
		std::vector<BYTE> buffer(m_width * m_height * 4);
		int buffer_w = 0, buffer_h = 0;

		G__buf = &buffer;
		
		if (!jpeg_expander.f_expand_ready(m_data, m_data_size, &buffer, &buffer_w, &buffer_h, 0))	{
			return false;
		}
		jpeg_expander.f_expand_proc();

		for (int y = 0; y < buffer_h; y++) {
			memcpy(dst + dst_pitch * y, &buffer[buffer_w * 4 * y], buffer_w * 4);
		}

		return true;
	}

	return true;
}

