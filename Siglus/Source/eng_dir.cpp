#include	"pch.h"

#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_reg_ini.h"
#include	"data/tnm_cmd_line.h"

#include	"data/tnm_dir.h"
#include	"data/tnm_lexer.h"

#include	"engine/eng_graphics.h"

// ****************************************************************
// dat�i�t�@�C�����Ɋg���q���܂܂�Ă��܂��j
// ================================================================
TSTR tnm_dat_path(CTSTR& base_dir, CTSTR& append_dir, CTSTR& file_name)
{
	return base_dir + _T("\\") + append_dir + _T("\\dat\\") + file_name;
}

// ****************************************************************
// g00
// ================================================================
TSTR tnm_g00_dir(CTSTR& base_dir, CTSTR& append_dir)
{
	return base_dir + _T("\\") + append_dir + _T("\\g00");
}

TSTR tnm_g00_dir()
{
	return tnm_g00_dir(Gp_dir->base_dir, Gp_dir->append_dir);
}

TSTR tnm_g00_path(CTSTR& base_dir, CTSTR& append_dir, CTSTR& file_name, CTSTR& extension)
{
	if (extension.empty())
	{
		return tnm_g00_dir(base_dir, append_dir) + _T("\\") + file_name;
	}
	else
	{
		return tnm_g00_dir(base_dir, append_dir) + _T("\\") + file_name + _T(".") + extension;
	}
}

TSTR tnm_g00_path(CTSTR& file_name)
{
	return tnm_g00_path(Gp_dir->base_dir, Gp_dir->append_dir, file_name, _T("g00"));
}

// ****************************************************************
// gan
// ================================================================
TSTR tnm_gan_path(CTSTR& base_dir, CTSTR& append_dir, CTSTR& file_name, CTSTR& extension)
{
	return base_dir + _T("\\") + append_dir + _T("\\gan\\") + file_name + _T(".") + extension;
}

// ****************************************************************
// x
// ================================================================
TSTR tnm_x_path(CTSTR& base_dir, CTSTR& append_dir, CTSTR& file_name, CTSTR& extension)
{
	return base_dir + _T("\\") + append_dir + _T("\\x\\") + file_name + _T(".") + extension;
}

// ****************************************************************
// wav
// ================================================================
TSTR tnm_wav_path(CTSTR& base_dir, CTSTR& append_dir, CTSTR& file_name, CTSTR& extension)
{
	return base_dir + _T("\\") + append_dir + _T("\\wav\\") + file_name + _T(".") + extension;
}

// ****************************************************************
// mov
// ================================================================
TSTR tnm_mov_path(CTSTR& base_dir, CTSTR& append_dir, CTSTR& file_name, CTSTR& extension)
{
	return base_dir + _T("\\") + append_dir + _T("\\mov\\") + file_name + _T(".") + extension;
}

// ****************************************************************
// file
// ================================================================
TSTR tnm_file_path(CTSTR& base_dir, CTSTR& append_dir, CTSTR& sub_dir, CTSTR& file_name, CTSTR& extension)
{
	return base_dir + _T("\\") + append_dir + _T("\\") + sub_dir + _T("\\") + file_name + _T(".") + extension;
}

// ****************************************************************
// �t�@�C�����ƌ��݂̃A�y���h���� dat �̃t���p�X���擾����
// ================================================================
TSTR tnm_find_dat(CTSTR& file_name)
{
	// ���݂̃f�B���N�g���̃A�y���h�ԍ������߂�
	int find = -1;
	for (int i = 0; i < (int)Gp_select_ini->m_append_list.size(); i++)
	{
		if (Gp_select_ini->m_append_list[i].m_dir == Gp_dir->append_dir)
		{
			find = i;
			break;
		}
	}

	// ���������ԍ����珇�Ƀt�@�C����T��
	if (find >= 0)
	{
		for (int i = find; i < (int)Gp_select_ini->m_append_list.size(); i++)
		{
			TSTR append_dir = Gp_select_ini->m_append_list[i].m_dir;
			TSTR file_path = tnm_dat_path(Gp_dir->base_dir, append_dir, file_name);

			if (file_get_type(file_path) == 1)
			{
				return file_path;
			}
		}
	}

	return _T("");
}

// ****************************************************************
// �t�@�C�����ƌ��݂̃A�y���h���� g00 �̃t���p�X���擾����
// ================================================================
TSTR tnm_find_g00_sub(CTSTR& base_dir, CTSTR& append_dir, CTSTR& sub_dir, CTSTR& file_name, int* p_ret_pct_type)
{
	int pct_type;
	TSTR file_path;

	// g00 ������
	pct_type = 1;
	file_path = tnm_file_path(base_dir, append_dir, sub_dir, file_name, _T("g00"));
	if (file_get_type(file_path) == 1)	{
		goto find_pct_success;
	}

	// bmp ������
	pct_type = 2;
	file_path = tnm_file_path(base_dir, append_dir, sub_dir, file_name, _T("bmp"));
	if (file_get_type(file_path) == 1)	{
		goto find_pct_success;
	}

	// png ������
	pct_type = 3;
	file_path = tnm_file_path(base_dir, append_dir, sub_dir, file_name, _T("png"));
	if (file_get_type(file_path) == 1)	{
		goto find_pct_success;
	}

	// jpg ������
	pct_type = 4;
	file_path = tnm_file_path(base_dir, append_dir, sub_dir, file_name, _T("jpg"));
	if (file_get_type(file_path) == 1)	{
		goto find_pct_success;
	}

	// dds ������
	pct_type = 5;
	file_path = tnm_file_path(base_dir, append_dir, sub_dir, file_name, _T("dds"));
	if (file_get_type(file_path) == 1)	{
		goto find_pct_success;
	}

	// ������Ȃ�����
	return _T("");

find_pct_success:

	if (p_ret_pct_type)
	{
		*p_ret_pct_type = pct_type;
	}

	return file_path;
}

TSTR tnm_find_g00(CTSTR& base_dir, CTSTR& sub_dir, CTSTR& file_name, int* p_ret_pct_type, bool find_append)
{
	int pct_type = 0;
	TSTR file_path;

	int find = -1;

	// �A�y���h��T���ꍇ
	if (find_append)
	{
		// ���݂̃f�B���N�g���̃A�y���h�ԍ������߂�
		for (int i = 0; i < (int)Gp_select_ini->m_append_list.size(); i++)
		{
			if (Gp_select_ini->m_append_list[i].m_dir == Gp_dir->append_dir)
			{
				find = i;
				break;
			}
		}

		// ���������ԍ����珇�ɉ摜��T��
		if (find >= 0)
		{
			for (int i = find; i < (int)Gp_select_ini->m_append_list.size(); i++)
			{
				TSTR append_dir = Gp_select_ini->m_append_list[i].m_dir;

				file_path = tnm_find_g00_sub(base_dir, append_dir, sub_dir, file_name, &pct_type);

				if (!file_path.empty())
				{
					goto tnm_find_g00_success;
				}
			}
		}
	}
	// �A�y���h��T���Ȃ��ꍇ
	else
	{
		file_path = tnm_find_g00_sub(base_dir, _T(""), sub_dir, file_name, &pct_type);

		if (!file_path.empty())
		{
			goto tnm_find_g00_success;
		}
	}

	return _T("");

	// �ȉ��͐������[�`��

tnm_find_g00_success:

	if (p_ret_pct_type)
	{
		*p_ret_pct_type = pct_type;
	}

	return file_path;
}

TSTR tnm_find_g00(CTSTR& file_name, int* p_ret_type)
{
	return tnm_find_g00(Gp_dir->base_dir, _T("g00"), file_name, p_ret_type, true);
}

// ****************************************************************
// �t�@�C�����ƌ��݂̃A�y���h���� gan �̃t���p�X���擾����
// ================================================================
TSTR tnm_find_gan(CTSTR& file_name)
{
	TSTR base_dir = Gp_dir->base_dir;
	TSTR extension = _T("gan");

	int find = -1;

	// ���݂̃f�B���N�g���̃A�y���h�ԍ������߂�
	for (int i = 0; i < (int)Gp_select_ini->m_append_list.size(); i++)
	{
		if (Gp_select_ini->m_append_list[i].m_dir == Gp_dir->append_dir)
		{
			find = i;
			break;
		}
	}

	// ���������ԍ����珇�Ƀt�@�C����T��
	if (find >= 0)
	{
		for (int i = find; i < (int)Gp_select_ini->m_append_list.size(); i++)
		{
			TSTR append_dir = Gp_select_ini->m_append_list[i].m_dir;
			TSTR file_path = tnm_gan_path(base_dir, append_dir, file_name, extension);

			if (file_get_type(file_path) == 1)
			{
				return file_path;
			}
		}
	}

	return _T("");
}

// ****************************************************************
// �t�@�C�����ƌ��݂̃A�y���h���� x �̃t���p�X���擾����
// ================================================================
TSTR tnm_find_x(CTSTR& file_name)
{
	TSTR base_dir = Gp_dir->base_dir;
	TSTR extension = _T("x");

	// ���݂̃f�B���N�g���̃A�y���h�ԍ������߂�
	int find = -1;
	for (int i = 0; i < (int)Gp_select_ini->m_append_list.size(); i++)
	{
		if (Gp_select_ini->m_append_list[i].m_dir == Gp_dir->append_dir)
		{
			find = i;
			break;
		}
	}

	// ���������ԍ����珇�Ƀt�@�C����T��
	if (find >= 0)
	{
		for (int i = find; i < (int)Gp_select_ini->m_append_list.size(); i++)
		{
			TSTR append_dir = Gp_select_ini->m_append_list[i].m_dir;
			TSTR file_path = tnm_x_path(base_dir, append_dir, file_name, extension);

			if (file_get_type(file_path) == 1)
			{
				return file_path;
			}
		}
	}

	return _T("");
}

// ****************************************************************
// �t�@�C�����ƌ��݂̃A�y���h���� wav �̃t���p�X���擾����
// ================================================================
TSTR tnm_find_wav(CTSTR& base_dir, CTSTR& sub_dir, CTSTR& file_name, int* p_ret_type)
{
	int sound_type = 0;
	TSTR file_path;

	int find_append_dir = -1;

	// ���݂̃f�B���N�g���̃A�y���h�ԍ������߂�
	for (int i = 0; i < (int)Gp_select_ini->m_append_list.size(); i++)
	{
		if (Gp_select_ini->m_append_list[i].m_dir == Gp_dir->append_dir)
		{
			find_append_dir = i;
			break;
		}
	}

	// ���������ԍ����珇�Ƀt�@�C����T��
	if (find_append_dir >= 0)
	{
		bool find_file = false;

		for (int i = find_append_dir; i < (int)Gp_select_ini->m_append_list.size(); i++)
		{
			TSTR append_dir = Gp_select_ini->m_append_list[i].m_dir;

			// wav ������
			sound_type = 1;
			file_path = tnm_file_path(base_dir, append_dir, sub_dir, file_name, _T("wav"));
			if (file_get_type(file_path) == 1)	{
				find_file = true;
				break;
			}

			// nwa ������
			sound_type = 2;
			file_path = tnm_file_path(base_dir, append_dir, sub_dir, file_name, _T("nwa"));
			if (file_get_type(file_path) == 1)	{
				find_file = true;
				break;
			}

			// ogg ������
			sound_type = 3;
			file_path = tnm_file_path(base_dir, append_dir, sub_dir, file_name, _T("ogg"));
			if (file_get_type(file_path) == 1)	{
				find_file = true;
				break;
			}

			// owp ������
			sound_type = 4;
			file_path = tnm_file_path(base_dir, append_dir, sub_dir, file_name, _T("owp"));
			if (file_get_type(file_path) == 1)	{
				find_file = true;
				break;
			}
		}

		if (find_file)
		{
			if (p_ret_type)
			{
				*p_ret_type = sound_type;
			}

			return file_path;
		}
	}

	return _T("");
}

// ****************************************************************
// �t�@�C�����ƌ��݂̃A�y���h���� koe �̃t���p�X���擾����
// ================================================================
TSTR tnm_find_koe(CTSTR& base_dir, CTSTR& sub_dir, int koe_no, int* p_ret_type)
{
	int sound_type = 0;
	TSTR file_name;
	TSTR file_path;

	int find_append_dir = -1;

	// ���ԍ�����V�[���ԍ������߂�
	int scn_no = koe_no / 100000;

	// ���݂̃f�B���N�g���̃A�y���h�ԍ������߂�
	for (int i = 0; i < (int)Gp_select_ini->m_append_list.size(); i++)
	{
		if (Gp_select_ini->m_append_list[i].m_dir == Gp_dir->append_dir)
		{
			find_append_dir = i;
			break;
		}
	}

	// ���������ԍ����珇�Ƀt�@�C����T��
	if (find_append_dir >= 0)
	{
		bool find_file = false;

		for (int i = find_append_dir; i < (int)Gp_select_ini->m_append_list.size(); i++)
		{
			TSTR append_dir = Gp_select_ini->m_append_list[i].m_dir;

			// wav ������
			sound_type = 1;
			file_name = str_format(_T("%04d\\z%09d"), scn_no, koe_no);
			file_path = tnm_file_path(base_dir, append_dir, sub_dir, file_name, _T("wav"));
			if (file_get_type(file_path) == 1)	{
				find_file = true;
				break;
			}

			// nwa ������
			sound_type = 2;
			file_name = str_format(_T("%04d\\z%09d"), scn_no, koe_no);
			file_path = tnm_file_path(base_dir, append_dir, sub_dir, file_name, _T("nwa"));
			if (file_get_type(file_path) == 1)	{
				find_file = true;
				break;
			}

			// ovk ������
			sound_type = 3;
			file_name = str_format(_T("z%04d"), scn_no);
			file_path = tnm_file_path(base_dir, append_dir, sub_dir, file_name, _T("ovk"));
			if (file_get_type(file_path) == 1)	{
				find_file = true;
				break;
			}
		}

		if (find_file)
		{
			if (p_ret_type)
			{
				*p_ret_type = sound_type;
			}

			return file_path;
		}
	}

	return _T("");
}

// ****************************************************************
// �t�@�C�����ƌ��݂̃A�y���h���� omv �̃t���p�X���擾����
// ================================================================
TSTR tnm_find_omv(CTSTR& file_name)
{
	TSTR base_dir = Gp_dir->base_dir;
	TSTR extension = _T("omv");

	int find = -1;

	// ���݂̃f�B���N�g���̃A�y���h�ԍ������߂�
	for (int i = 0; i < (int)Gp_select_ini->m_append_list.size(); i++)
	{
		if (Gp_select_ini->m_append_list[i].m_dir == Gp_dir->append_dir)
		{
			find = i;
			break;
		}
	}

	// ���������ԍ����珇�Ƀt�@�C����T��
	if (find >= 0)
	{
		for (int i = find; i < (int)Gp_select_ini->m_append_list.size(); i++)
		{
			TSTR append_dir = Gp_select_ini->m_append_list[i].m_dir;
			TSTR file_path = tnm_mov_path(base_dir, append_dir, file_name, extension);

			if (file_get_type(file_path) == 1)
			{
				return file_path;
			}
		}
	}

	return _T("");
}

// ****************************************************************
// �t�@�C�����ƌ��݂̃A�y���h���� psb �̃t���p�X���擾����
// ================================================================
TSTR tnm_find_psb(CTSTR& file_name)
{
	TSTR base_dir = Gp_dir->base_dir;
	TSTR extension = _T("psb");

	int find = -1;

	// ���݂̃f�B���N�g���̃A�y���h�ԍ������߂�
	for (int i = 0; i < (int)Gp_select_ini->m_append_list.size(); i++)
	{
		if (Gp_select_ini->m_append_list[i].m_dir == Gp_dir->append_dir)
		{
			find = i;
			break;
		}
	}

	// ���������ԍ����珇�Ƀt�@�C����T��
	if (find >= 0)
	{
		for (int i = find; i < (int)Gp_select_ini->m_append_list.size(); i++)
		{
			TSTR append_dir = Gp_select_ini->m_append_list[i].m_dir;
			TSTR file_path = tnm_dat_path(base_dir, append_dir, file_name + _T(".") + extension);

			if (file_get_type(file_path) == 1)
			{
				return file_path;
			}
		}
	}

	return _T("");
}

// ****************************************************************
// �t�@�C�����ƌ��݂̃A�y���h���� mov �̃t���p�X���擾����
// ================================================================
TSTR tnm_find_mov(CTSTR& base_dir, CTSTR& sub_dir, CTSTR& file_name, int* p_ret_type)
{
	int movie_type = 0;
	TSTR file_path;

	int find_append_dir = -1;

	// ���݂̃f�B���N�g���̃A�y���h�ԍ������߂�
	for (int i = 0; i < (int)Gp_select_ini->m_append_list.size(); i++)
	{
		if (Gp_select_ini->m_append_list[i].m_dir == Gp_dir->append_dir)
		{
			find_append_dir = i;
			break;
		}
	}

	// ���������ԍ����珇�Ƀt�@�C����T��
	if (find_append_dir >= 0)
	{
		bool find_file = false;

		for (int i = find_append_dir; i < (int)Gp_select_ini->m_append_list.size(); i++)
		{
			TSTR append_dir = Gp_select_ini->m_append_list[i].m_dir;

			// wmv ������
			movie_type = TNM_MOVIE_FILE_TYPE_WMV;
			file_path = tnm_file_path(base_dir, append_dir, sub_dir, file_name, _T("wmv"));
			if (file_get_type(file_path) == 1)	{
				find_file = true;
				break;
			}

			// mpg ������
			movie_type = TNM_MOVIE_FILE_TYPE_MPG;
			file_path = tnm_file_path(base_dir, append_dir, sub_dir, file_name, _T("mpg"));
			if (file_get_type(file_path) == 1)	{
				find_file = true;
				break;
			}

			// avi ������
			movie_type = TNM_MOVIE_FILE_TYPE_AVI;
			file_path = tnm_file_path(base_dir, append_dir, sub_dir, file_name, _T("avi"));
			if (file_get_type(file_path) == 1)	{
				find_file = true;
				break;
			}
		}

		if (find_file)
		{
			if (p_ret_type)
			{
				*p_ret_type = movie_type;
			}

			return file_path;
		}
	}

	return _T("");
}

