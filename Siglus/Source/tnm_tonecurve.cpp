#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_tonecurve.h"
#include	"engine/eng_graphics.h"
#include	"engine/eng_dir.h"
#include	"engine/ifc_error.h"

// ****************************************************************
// �g�[���J�[�u�F�萔
// ================================================================
const int		TNM_TONE_CURVE_MAX = 1000;

const int		TNM_TCTYPE_TONE_CURVE = 0;
const int		TNM_TCTYPE_RGB_SAT = 1;

// ****************************************************************
// �g�[���J�[�u�F�\����
// ================================================================
struct S_tnm_tonecurve_header
{
	int		max;
	int		cnt;
	int		offset[TNM_TONE_CURVE_MAX];
};

struct S_tnm_tonecurve_sub_header
{
	int		type;
	int		data_size;
	int		keep[14];
};

// ****************************************************************
// �g�[���J�[�u�F������
// ================================================================
void C_tnm_tonecurve::init()
{
	TSTR file_name = Gp_ini->tonecurve_file;

	// �t�@�C��������Ȃ牽�����Ȃ�
	if (file_name.empty())
		return;

	TSTR file_path = tnm_find_dat(file_name);

	// �t�@�C����ǂݍ���
	ARRAY<BYTE> file_data;
	if (!C_file::read_full_data(file_path, file_data))	{
		tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, file_name + _T(" ���J���܂���ł����B"));
		return;
	}

	// �w�b�_���擾
	S_tnm_tonecurve_header* p_header = (S_tnm_tonecurve_header *)file_data.get();

	// �������i�V�V�X�e���ł� 256 �܂ł����ǂ߂Ȃ��j
	if (p_header->cnt > 256)	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�g�[���J�[�u�� 256 �܂ł����쐬�ł��܂���B"));
		return;
	}

	// �T�u���쐬
	m_sub_list.resize(256);

	// �e�N�X�`�����쐬
	// ������ 256 �Œ�
	// �~�b�v�}�b�v���쐬���Ȃ��i�X�e�[�W�P�ɐݒ肷��̂ō쐬���Ă��g���܂���j
	D3DLOCKED_RECT rect;
	m_texture = G_rsm.create_texture(_T("İݶ���"), 256, 256, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, false);
	m_texture->lock_rect(0, &rect, NULL, 0);

	// �t�@�C���f�[�^���R�s�[���Ă���
	for (int i = 0; i < 256; i++)	{
		if (p_header->offset[i] != 0)	{
			S_tnm_tonecurve_sub_header* p_sub_header = (S_tnm_tonecurve_sub_header *)(file_data.get() + p_header->offset[i]);
			BYTE* p = file_data.get() + p_header->offset[i] + sizeof(S_tnm_tonecurve_sub_header);
			BYTE* src_r = p;
			BYTE* src_g = p + 256;
			BYTE* src_b = p + 512;
			C_argb* dst = (C_argb *)((BYTE *)rect.pBits + rect.Pitch * i);

			// �T�u�̓ǂݍ���
			switch (p_sub_header->type)	{
				case TNM_TCTYPE_TONE_CURVE:
					for (int j = 0; j < 256; j++)	{
						dst->r = *src_r++;
						dst->g = *src_g++;
						dst->b = *src_b++;
						dst->a = 255;		dst++;
					}
					m_sub_list[i].exist = 1;	// ���݂����t���O
					break;
				case TNM_TCTYPE_RGB_SAT:
					for (int j = 0; j < 256; j++)	{
						dst->r = *src_r++;
						dst->g = *src_g++;
						dst->b = *src_b++;
						dst->a = 255;		dst++;
					}
					m_sub_list[i].sat = *(int *)(p + 768);
					m_sub_list[i].exist = 1;	// ���݂����t���O
					break;
			}
		}
	}

	// �e�N�X�`�����A�����b�N
	m_texture->unlock_rect(0);
}

// ****************************************************************
// �g�[���J�[�u�F�f�[�^�̑��݃`�F�b�N
// ================================================================
bool C_tnm_tonecurve::check_tonecurve_no(int tonecurve_no)
{
	return (0 <= tonecurve_no && tonecurve_no < (int)m_sub_list.size()) && m_sub_list[tonecurve_no].exist;
}

// ****************************************************************
// �g�[���J�[�u�F�e�N�X�`�����擾
// ================================================================
BSP<C_d3d_texture> C_tnm_tonecurve::get_texture()
{
	return m_texture;
}

// ****************************************************************
// �g�[���J�[�u�F�ʓx���擾
// ================================================================
int C_tnm_tonecurve::get_sat(int tonecurve_no)
{
	if (tonecurve_no < 0 || (int)m_sub_list.size() <= tonecurve_no)
		return 0;

	return m_sub_list[tonecurve_no].sat;
}
