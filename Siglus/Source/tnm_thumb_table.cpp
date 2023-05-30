#include	"pch.h"
#include	"tnm_common_header.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_thumb_table.h"
#include	"engine/eng_dir.h"
#include	"engine/eng_graphics.h"
#include	"engine/ifc_error.h"

#include	<lzss_unpack.h>

// '\0' ���������ǂݍ���
TCHAR* get_str_null(TCHAR* p, TSTR& str)
{
	TCHAR* p_begin = p;
	for (; *p != _T('\0'); p++);

	str = TSTR(p_begin, p);
	return p + 1;	// '\0' �̎���
}

// ****************************************************************
// �T���l�C���e�[�u���F�e�[�u�����\�z
// ================================================================
bool C_tnm_thumb_table::create_table()
{
	// �t�@�C�������擾�A��Ȃ牽�����Ȃ�
	TSTR file_name = Gp_ini->thumb_table_file;
	if (file_name.empty())
		return true;

	// �t�@�C����ǂݍ���
	C_file file;
	ARRAY<BYTE> file_data;
	change_extension(file_name, _T("dat"));
	TSTR file_path = tnm_find_dat(file_name);
	if (!file.read_full_data(file_path, file_data))	{
		tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("�T���l�C���e�[�u�� ") + file_name + _T(" ��������܂���ł����B"));
		return false;
	}

	// �o�b�t�@�ւ̃|�C���^
	BYTE* p = file_data.get();

	// �w�b�_
	S_tnm_thumbnail_database_header* p_header = (S_tnm_thumbnail_database_header *)p;
	p += sizeof(S_tnm_thumbnail_database_header);

	// �f�[�^�������𓀂���
	int unpack_size = lzss_unpack(p, NULL);
	ARRAY<BYTE> unpack_data(unpack_size);
	lzss_unpack(p, unpack_data.get());
	p = unpack_data.get();

	// �f�[�^��ǂݍ���
	TSTR pct, thumb;
	for (int i = 0; i < p_header->data_cnt; i++)	{
		p = (BYTE *)get_str_null((TCHAR *)p, pct);
		p = (BYTE *)get_str_null((TCHAR *)p, thumb);
		str_to_lower(pct);		// ���������ɕϊ�
		str_to_lower(thumb);	// ���������ɕϊ�
		m_thumb_table.insert(std::pair<TSTR, TSTR>(pct, thumb));
	}

	return true;
}

// ****************************************************************
// �T���l�C���e�[�u���F�\������ׂ��T���l�C�����擾����
// ================================================================
TSTR C_tnm_thumb_table::calc_thumb_file_name()
{
	if (m_thumb_table.empty())
		return _T("");

	// ���\�[�X��Ԃ��擾����
	const S_tnm_d3d_resource_data& rd = G_rsm.pd();

	// �o�^����Ă���b�f�����\�[�X���猟������
	int rsc_cnt = (int)rd.resource_list.size();
	for (int i = 0; i < rsc_cnt; i++)	{
		// g00 ������ꂽ�A���o���ł���ꍇ
		if (rd.resource_list[i].type == E_tnm_d3d_resource_type_album_from_g00)	{
			// ���\�[�X�������Ă���K�v������܂��I
			if (rd.resource_list[i].album.lock())	{

				// �t�@�C������؂�o��
				TSTR file_name = get_file_name(rd.resource_list[i].name);
				erase_extension(file_name);
				str_to_lower(file_name);		// ���������ɕϊ�

				// �}�b�v���猟������
				std::map<TSTR, TSTR>::iterator find_itr = m_thumb_table.find(file_name);
				if (find_itr != m_thumb_table.end())	{

					// ���������I�I
					return find_itr->second;
				}
			}
		}
	}

	return _T("");
}




