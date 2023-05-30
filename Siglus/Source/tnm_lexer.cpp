#include	"pch.h"
#include	"lzss.h"

#include	"ifc_eng.h"

#include	"data/tnm_dir.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_lexer.h"

// ****************************************************************
// �ȈՈÍ��R�[�h
// ================================================================
#include	"tnm_exe_angou.h"
BYTE tnm_easy_angou_code[TNM_EASY_ANGOU_CODE_SIZE] = TNM_EASY_ANGOU_CODE;

// ****************************************************************
// �d�w�d�Z�b�g�Í��R�[�h
// ================================================================
volatile extern BYTE tnm_exe_angou_find_code00[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ���d�w�d�Z�b�g�Í��R�[�h
volatile extern BYTE tnm_exe_angou_find_code01[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ���d�w�d�Z�b�g�Í��R�[�h
volatile extern BYTE tnm_exe_angou_find_code02[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ���d�w�d�Z�b�g�Í��R�[�h
volatile extern BYTE tnm_exe_angou_find_code03[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ���d�w�d�Z�b�g�Í��R�[�h
volatile extern BYTE tnm_exe_angou_find_code04[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ���d�w�d�Z�b�g�Í��R�[�h
volatile extern BYTE tnm_exe_angou_find_code05[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ���d�w�d�Z�b�g�Í��R�[�h
volatile extern BYTE tnm_exe_angou_find_code06[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ���d�w�d�Z�b�g�Í��R�[�h
volatile extern BYTE tnm_exe_angou_find_code07[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ���d�w�d�Z�b�g�Í��R�[�h

// ****************************************************************
// �X�N���v�g���N�T�[
// ================================================================
C_tnm_scene_lexer::C_tnm_scene_lexer()
{
	m_cur_ptr = NULL;
	m_cur_scn_no = 0;
	m_cur_line_no = 0;
}

// ****************************************************************
// ���N�T�[�F������
// ================================================================
void C_tnm_scene_lexer::init()
{
	m_scn_data.clear();
	
	m_pack_scn_info.p_data_top = NULL;
	m_pack_scn_info.p_header = NULL;
	m_pack_scn_info.p_inc_prop_list = NULL;
	m_pack_scn_info.p_inc_prop_name_index_list = NULL;
	m_pack_scn_info.p_inc_prop_name_list = NULL;
	m_pack_scn_info.p_inc_cmd_name_index_list = NULL;
	m_pack_scn_info.p_inc_cmd_name_list = NULL;
	m_pack_scn_info.p_inc_cmd_list = NULL;
	m_pack_scn_info.p_scn_name_index_list = NULL;
	m_pack_scn_info.p_scn_name_list = NULL;
	m_pack_scn_info.p_scn_data_index_list = NULL;
	m_pack_scn_info.p_scn_data_list = NULL;
	m_pack_scn_info.scn_no_map.clear();
	m_pack_scn_info.inc_prop_no_map.clear();
	m_pack_scn_info.inc_cmd_no_map.clear();

	m_scn_info_list.clear();

	m_cur_ptr = NULL;
	m_cur_scn_no = 0;
	m_cur_line_no = 0;
}

// ****************************************************************
// ���N�T�[�F�V�[����ǂݍ���
// ================================================================
bool C_tnm_scene_lexer::load_scn(CTSTR& scn_pack_name)
{
	m_scn_data.clear();

	// �V�[���p�X
	TSTR scene_path = Gp_dir->exe_dir + _T("\\") + Gp_dir->append_dir + _T("\\") + scn_pack_name;

	// �t�@�C���̓ǂݍ���
	BUFFER tmp;
	if (!C_file::read_full_data(scene_path, tmp))
		return false;

	S_tnm_pack_scn_header *tmp_p_header = (S_tnm_pack_scn_header *)tmp.get();

	// �d�w�d�Z�b�g�Í��̃R�[�h���擾
	BYTE exe_angou_element[TNM_EXE_ANGOU_ELEMENT_CNT];
	if (tmp_p_header->scn_data_exe_angou_mod)
	{
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT00A] = tnm_exe_angou_find_code00[TNM_EXE_ANGOU_OFFSET00A];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT01A] = tnm_exe_angou_find_code01[TNM_EXE_ANGOU_OFFSET01A];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT02A] = tnm_exe_angou_find_code02[TNM_EXE_ANGOU_OFFSET02A];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT03A] = tnm_exe_angou_find_code03[TNM_EXE_ANGOU_OFFSET03A];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT04A] = tnm_exe_angou_find_code04[TNM_EXE_ANGOU_OFFSET04A];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT05A] = tnm_exe_angou_find_code05[TNM_EXE_ANGOU_OFFSET05A];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT06A] = tnm_exe_angou_find_code06[TNM_EXE_ANGOU_OFFSET06A];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT07A] = tnm_exe_angou_find_code07[TNM_EXE_ANGOU_OFFSET07A];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT00B] = tnm_exe_angou_find_code00[TNM_EXE_ANGOU_OFFSET00B];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT01B] = tnm_exe_angou_find_code01[TNM_EXE_ANGOU_OFFSET01B];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT02B] = tnm_exe_angou_find_code02[TNM_EXE_ANGOU_OFFSET02B];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT03B] = tnm_exe_angou_find_code03[TNM_EXE_ANGOU_OFFSET03B];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT04B] = tnm_exe_angou_find_code04[TNM_EXE_ANGOU_OFFSET04B];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT05B] = tnm_exe_angou_find_code05[TNM_EXE_ANGOU_OFFSET05B];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT06B] = tnm_exe_angou_find_code06[TNM_EXE_ANGOU_OFFSET06B];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT07B] = tnm_exe_angou_find_code07[TNM_EXE_ANGOU_OFFSET07B];
	}

	// �V�[���f�[�^�̍č\�z�i���k�Í������ꂽ�V�[���f�[�^�����̏�Ԃɖ߂��j
	m_scn_data.resize(tmp_p_header->scn_data_list_ofs);
	CopyMemory(m_scn_data.get(), tmp.get(), tmp_p_header->scn_data_list_ofs);
	C_index *tmp_p_scn_data_index_list = (C_index *)(tmp.get() + tmp_p_header->scn_data_index_list_ofs);
	C_index *scn_data_p_scn_data_index_list;
	int offset = tmp_p_scn_data_index_list[0].offset;
	int size, eac;
	BYTE *sp, *dp;
	for (int scn_no = 0; scn_no < tmp_p_header->scn_data_cnt; scn_no++)	{

		if (tmp_p_scn_data_index_list[scn_no].size > 0)	{

			// ���k�f�[�^�ւ̃|�C���^
			sp = tmp.get() + tmp_p_header->scn_data_list_ofs + tmp_p_scn_data_index_list[scn_no].offset;
			size = tmp_p_scn_data_index_list[scn_no].size;

			// �ȈՃ����N�łȂ��ꍇ�̓V�[���f�[�^����
			if (tmp_p_header->original_source_header_size > 0)	{

				// �d�w�d�Z�b�g�Í�����
				if (tmp_p_header->scn_data_exe_angou_mod)	{
					dp = sp;
					eac = 0;
					for (int i = 0; i < size; i++)	{
						*dp++ ^= exe_angou_element[eac++];
						eac %= TNM_EXE_ANGOU_ELEMENT_CNT;
					}
				}

				// �ȈՈÍ�����
				dp = sp;
				eac = 0;
				for (int i = 0; i < size; i++)	{
					*dp++ ^= tnm_easy_angou_code[eac++];
					eac %= TNM_EASY_ANGOU_CODE_SIZE;
				}

				// ��
				size = lzss_unpack(sp, NULL);
				m_scn_data.resize((int)m_scn_data.size() + size);				// �o�b�t�@�m��
				dp = m_scn_data.get() + tmp_p_header->scn_data_list_ofs + offset;
				lzss_unpack(sp, dp);
			}
			else	{

				// �ȈՃ����N�̏ꍇ�͂��̂܂܃R�s�[
				m_scn_data.resize((int)m_scn_data.size() + tmp_p_scn_data_index_list[scn_no].size);
				sp = tmp.get() + tmp_p_header->scn_data_list_ofs + tmp_p_scn_data_index_list[scn_no].offset;
				dp = m_scn_data.get() + tmp_p_header->scn_data_list_ofs + offset;
				memcpy(dp, sp, tmp_p_scn_data_index_list[scn_no].size);
			}
		}
		else	{
			size = 0;
		}

		scn_data_p_scn_data_index_list = (C_index *)(m_scn_data.get() + tmp_p_header->scn_data_index_list_ofs);
		scn_data_p_scn_data_index_list[scn_no].offset = offset;		// �V�����V�[���f�[�^�̃I�t�Z�b�g
		scn_data_p_scn_data_index_list[scn_no].size = size;			// �V�����V�[���f�[�^�̃T�C�Y
		offset += size;
	}

	// �p�b�N�V�[���f�[�^�̉��
	BYTE* p_pack_data_top = m_scn_data.get();
	m_pack_scn_info.p_data_top = m_scn_data.get();
	m_pack_scn_info.p_header = (S_tnm_pack_scn_header *)p_pack_data_top;																// �w�b�_
	m_pack_scn_info.p_inc_prop_list = (S_tnm_pack_scn_inc_prop *)(p_pack_data_top + m_pack_scn_info.p_header->inc_prop_list_ofs);		// �C���N�v���p�e�B
	m_pack_scn_info.p_inc_prop_name_index_list = (C_index *)(p_pack_data_top + m_pack_scn_info.p_header->inc_prop_name_index_list_ofs);	// �C���N�v���p�e�B��
	m_pack_scn_info.p_inc_prop_name_list = (TCHAR *)(p_pack_data_top + m_pack_scn_info.p_header->inc_prop_name_list_ofs);				// �C���N�v���p�e�B��
	m_pack_scn_info.p_inc_cmd_list = (S_tnm_pack_scn_inc_cmd *)(p_pack_data_top + m_pack_scn_info.p_header->inc_cmd_list_ofs);			// �C���N�R�}���h
	m_pack_scn_info.p_inc_cmd_name_index_list = (C_index *)(p_pack_data_top + m_pack_scn_info.p_header->inc_cmd_name_index_list_ofs);	// �C���N�R�}���h���g�b�v
	m_pack_scn_info.p_inc_cmd_name_list = (TCHAR *)(p_pack_data_top + m_pack_scn_info.p_header->inc_cmd_name_list_ofs);					// �C���N�R�}���h��
	m_pack_scn_info.p_scn_name_index_list = (C_index *)(p_pack_data_top + m_pack_scn_info.p_header->scn_name_index_list_ofs);			// �V�[�����g�b�v
	m_pack_scn_info.p_scn_name_list = (TCHAR *)(p_pack_data_top + m_pack_scn_info.p_header->scn_name_list_ofs);							// �V�[����
	m_pack_scn_info.p_scn_data_index_list = (C_index *)(p_pack_data_top + m_pack_scn_info.p_header->scn_data_index_list_ofs);			// �V�[���f�[�^�g�b�v
	m_pack_scn_info.p_scn_data_list = (BYTE *)(p_pack_data_top + m_pack_scn_info.p_header->scn_data_list_ofs);							// �V�[���f�[�^


	// �V�[���� �� �V�[���ԍ��̃}�b�v���쐬
	for (int i = 0; i < m_pack_scn_info.p_header->scn_name_cnt; i++)	{
		C_index* index = &m_pack_scn_info.p_scn_name_index_list[i];
		TCHAR* begin = &m_pack_scn_info.p_scn_name_list[index->offset];
		TCHAR* end = &m_pack_scn_info.p_scn_name_list[index->offset + index->size];
		m_pack_scn_info.scn_no_map.insert(std::pair<TSTR, int>(TSTR(begin, end), i));
	}

	// �C���N�v���p�e�B�� �� �C���N�v���p�e�B�ԍ��̃}�b�v���쐬
	for (int i = 0; i < m_pack_scn_info.p_header->inc_prop_name_cnt; i++)	{
		C_index* index = &m_pack_scn_info.p_inc_prop_name_index_list[i];
		TCHAR* begin = &m_pack_scn_info.p_inc_prop_name_list[index->offset];
		TCHAR* end = &m_pack_scn_info.p_inc_prop_name_list[index->offset + index->size];
		m_pack_scn_info.inc_prop_no_map.insert(std::pair<TSTR, int>(TSTR(begin, end), i));
	}

	// �C���N�R�}���h�� �� �C���N�R�}���h�ԍ��̃}�b�v���쐬
	for (int i = 0; i < m_pack_scn_info.p_header->inc_cmd_name_cnt; i++)	{
		C_index* index = &m_pack_scn_info.p_inc_cmd_name_index_list[i];
		TCHAR* begin = &m_pack_scn_info.p_inc_cmd_name_list[index->offset];
		TCHAR* end = &m_pack_scn_info.p_inc_cmd_name_list[index->offset + index->size];
		m_pack_scn_info.inc_cmd_no_map.insert(std::pair<TSTR, int>(TSTR(begin, end), i));
	}

	// �V�[���f�[�^�C���f�b�N�X����A�e�V�[���̐擪�̃|�C���^�����߂�
	m_scn_info_list.resize(m_pack_scn_info.p_header->scn_data_cnt);
	for (int scn_no = 0; scn_no < m_pack_scn_info.p_header->scn_data_cnt; scn_no++)	{
		BYTE* p_scn_data_top = m_pack_scn_info.p_scn_data_list + m_pack_scn_info.p_scn_data_index_list[scn_no].offset;

		// �e�V�[�������
		S_tnm_scn_header* p_scn_header						= (S_tnm_scn_header *)p_scn_data_top;
		m_scn_info_list[scn_no].p_header					= (S_tnm_scn_header *)p_scn_data_top;
		m_scn_info_list[scn_no].p_data_top					= p_scn_data_top;																// �g�b�v
		m_scn_info_list[scn_no].p_scn						= p_scn_data_top + p_scn_header->scn_ofs;										// �V�[��
		m_scn_info_list[scn_no].p_str_index_list			= (C_index *)(p_scn_data_top + p_scn_header->str_index_list_ofs);				// �����񃊃X�g�g�b�v
		m_scn_info_list[scn_no].p_str_list					= (TCHAR *)(p_scn_data_top + p_scn_header->str_list_ofs);						// �����񃊃X�g
		m_scn_info_list[scn_no].p_label_list				= (int *)(p_scn_data_top + p_scn_header->label_list_ofs);						// ���x�����X�g
		m_scn_info_list[scn_no].p_z_label_list				= (int *)(p_scn_data_top + p_scn_header->z_label_list_ofs);						// �y���x�����X�g
		m_scn_info_list[scn_no].p_scn_prop_list				= (S_tnm_scn_scn_prop *)(p_scn_data_top + p_scn_header->scn_prop_list_ofs);		// �V�[���v���p�e�B���X�g
		m_scn_info_list[scn_no].p_scn_prop_name_index_list	= (C_index *)(p_scn_data_top + p_scn_header->scn_prop_name_index_list_ofs);		// �V�[���v���p�e�B�����X�g�g�b�v
		m_scn_info_list[scn_no].p_scn_prop_name_list		= (TCHAR *)(p_scn_data_top + p_scn_header->scn_prop_name_list_ofs);				// �V�[���v���p�e�B�����X�g
		m_scn_info_list[scn_no].p_scn_cmd_list				= (S_tnm_scn_scn_cmd *)(p_scn_data_top + p_scn_header->scn_cmd_list_ofs);		// �V�[���R�}���h���X�g
		m_scn_info_list[scn_no].p_scn_cmd_name_index_list	= (C_index *)(p_scn_data_top + p_scn_header->scn_cmd_name_index_list_ofs);		// �V�[���R�}���h���C���f�b�N�X���X�g
		m_scn_info_list[scn_no].p_scn_cmd_name_list			= (TCHAR *)(p_scn_data_top + p_scn_header->scn_cmd_name_list_ofs);				// �V�[���R�}���h�����X�g
		m_scn_info_list[scn_no].p_call_prop_name_index_list	= (C_index *)(p_scn_data_top + p_scn_header->call_prop_name_index_list_ofs);	// �R�[���v���p�e�B�����X�g�g�b�v
		m_scn_info_list[scn_no].p_call_prop_name_list		= (TCHAR *)(p_scn_data_top + p_scn_header->call_prop_name_list_ofs);			// �R�[���v���p�e�B�����X�g
		m_scn_info_list[scn_no].p_namae_list				= (int *)(p_scn_data_top + p_scn_header->namae_list_ofs);						// �y���O�z���X�g
		m_scn_info_list[scn_no].p_read_flag_list			= (S_tnm_scn_read_flag *)(p_scn_data_top + p_scn_header->read_flag_list_ofs);	// ���ǃt���O���X�g

		// �V�[���v���p�e�B�� �� �V�[���v���p�e�B�ԍ��̃}�b�v���쐬
		for (int i = 0; i < m_scn_info_list[scn_no].p_header->scn_prop_name_cnt; i++)	{
			C_index* index = &m_scn_info_list[scn_no].p_scn_prop_name_index_list[i];
			TCHAR* begin = &m_scn_info_list[scn_no].p_scn_prop_name_list[index->offset];
			TCHAR* end = &m_scn_info_list[scn_no].p_scn_prop_name_list[index->offset + index->size];
			m_scn_info_list[scn_no].scn_prop_no_map.insert(std::pair<TSTR, int>(TSTR(begin, end), i));
		}

		// �V�[���R�}���h�� �� �V�[���R�}���h�ԍ��̃}�b�v���쐬
		for (int i = 0; i < m_scn_info_list[scn_no].p_header->scn_cmd_name_cnt; i++)	{
			C_index* index = &m_scn_info_list[scn_no].p_scn_cmd_name_index_list[i];
			TCHAR*  begin = &m_scn_info_list[scn_no].p_scn_cmd_name_list[index->offset];
			TCHAR*  end = &m_scn_info_list[scn_no].p_scn_cmd_name_list[index->offset + index->size];
			m_scn_info_list[scn_no].scn_cmd_no_map.insert(std::pair<TSTR, int>(TSTR(begin, end), i));
		}
	}

	return true;
}

// ****************************************************************
// �I���W�i���\�[�X����
// ================================================================
bool C_tnm_scene_lexer::is_exist_org_source()
{
	if (m_pack_scn_info.p_header)	{
		if (m_pack_scn_info.p_header->original_source_header_size > 0)	{
			return true;
		}
	}

	return false;
}

// ****************************************************************
// ���[�U�v���p�e�B�����烆�[�U�v���p�e�B�ԍ����擾
// ================================================================
int C_tnm_scene_lexer::get_user_prop_no(CTSTR& user_prop_name)
{
	return get_user_prop_no(m_cur_scn_no, user_prop_name);
}

int C_tnm_scene_lexer::get_user_prop_no(int scn_no, CTSTR& user_prop_name)
{
	if (scn_no < 0)
		return -1;

	// �t�@�C�����͏������Ŋi�[����Ă��邱�Ƃɒ��ӁI

	// �C���N�R�}���h��T��
	std::map<TSTR, int>::iterator itr_inc_prop = m_pack_scn_info.inc_prop_no_map.find(str_to_lower_ret(user_prop_name));
	if (itr_inc_prop != m_pack_scn_info.inc_prop_no_map.end())
		return itr_inc_prop->second;

	// �V�[���R�}���h��T��
	std::map<TSTR, int>::iterator itr_scn_prop = m_scn_info_list[scn_no].scn_prop_no_map.find(str_to_lower_ret(user_prop_name));
	if (itr_scn_prop != m_scn_info_list[scn_no].scn_prop_no_map.end())
		return itr_scn_prop->second + m_pack_scn_info.p_header->inc_prop_cnt;	// �V�[���v���p�e�BID �� ���[�U�v���p�e�BID

	return -1;
}

// ****************************************************************
// ���[�U�R�}���h�����烆�[�U�R�}���h�ԍ����擾
// ================================================================
int C_tnm_scene_lexer::get_user_cmd_no(int scn_no, CTSTR& user_cmd_name)
{
	if (scn_no < 0)
		return -1;

	// �t�@�C�����͏������Ŋi�[����Ă��邱�Ƃɒ��ӁI

	// �C���N�R�}���h��T��
	std::map<TSTR, int>::iterator itr_inc_cmd = m_pack_scn_info.inc_cmd_no_map.find(str_to_lower_ret(user_cmd_name));
	if (itr_inc_cmd != m_pack_scn_info.inc_cmd_no_map.end())
		return itr_inc_cmd->second;

	// �V�[���R�}���h��T��
	std::map<TSTR, int>::iterator itr_scn_cmd = m_scn_info_list[scn_no].scn_cmd_no_map.find(str_to_lower_ret(user_cmd_name));
	if (itr_scn_cmd != m_scn_info_list[scn_no].scn_cmd_no_map.end())
		return itr_scn_cmd->second + m_pack_scn_info.p_header->inc_cmd_cnt;	// �V�[���R�}���hID �� ���[�U�R�}���hID

	return -1;
}

// ****************************************************************
// ���[�U�R�}���h�ԍ����烆�[�U�R�}���h�����擾
// ================================================================
TSTR C_tnm_scene_lexer::get_user_cmd_name(int scn_no, int user_cmd_no)
{
	// �C���N�R�}���h�̏ꍇ
	if (user_cmd_no < m_pack_scn_info.p_header->inc_cmd_cnt)	{
		
		C_index* index = &m_pack_scn_info.p_inc_cmd_name_index_list[user_cmd_no];
		TCHAR*  start = &m_pack_scn_info.p_inc_cmd_name_list[index->offset];
		TCHAR*  end = &m_pack_scn_info.p_inc_cmd_name_list[index->offset + index->size];
		return TSTR(start, end);
	}

	// �V�[���R�}���h�̏ꍇ
	else	{

		// ���[�U�R�}���h�ԍ� �� �V�[���R�}���h�ԍ�
		int scn_cmd_no = user_cmd_no - m_pack_scn_info.p_header->inc_cmd_cnt;

		C_index* index = &m_scn_info_list[scn_no].p_scn_cmd_name_index_list[scn_cmd_no];
		TCHAR*  start = &m_scn_info_list[scn_no].p_scn_cmd_name_list[index->offset];
		TCHAR*  end = &m_scn_info_list[scn_no].p_scn_cmd_name_list[index->offset + index->size];
		return TSTR(start, end);
	}
}

// ****************************************************************
// �V�[��������V�[���ԍ����擾
// ================================================================
int C_tnm_scene_lexer::get_scn_no(CTSTR& scn_name)
{
	// �t�@�C�����͏������Ŋi�[����Ă��邱�Ƃɒ��ӁI

	// �V�[����T��
	std::map<TSTR, int>::iterator itr = m_pack_scn_info.scn_no_map.find(str_to_lower_ret(scn_name));
	if (itr == m_pack_scn_info.scn_no_map.end())
		return -1;

	return itr->second;
}

// ****************************************************************
// �V�[���ԍ�����V�[�������擾
// ================================================================
TSTR C_tnm_scene_lexer::get_scn_name(int scn_no)
{
	C_index* index = &m_pack_scn_info.p_scn_name_index_list[scn_no];
	TCHAR* begin = &m_pack_scn_info.p_scn_name_list[index->offset];
	TCHAR* end = &m_pack_scn_info.p_scn_name_list[index->offset + index->size];

	return TSTR(begin, end);
}

// ****************************************************************
// �V�[�����ݔ���
// ================================================================
bool C_tnm_scene_lexer::is_exist_scn(CTSTR& scn_name)
{
	// �t�@�C�����͏������Ŋi�[����Ă���I
	return m_pack_scn_info.scn_no_map.find(str_to_lower_ret(scn_name)) != m_pack_scn_info.scn_no_map.end();
}

bool C_tnm_scene_lexer::is_exist_z(int scn_no, int z_no)
{
	// �y�ԍ��͈̔͂𔻒�
	if (z_no < 0 || TNM_Z_LABEL_CNT <= z_no)
		return false;

	// �y���x���̌���
	return m_scn_info_list[scn_no].p_z_label_list[z_no] > 0;
}

// ****************************************************************
// �y���O�z�̌����擾
// ================================================================
int C_tnm_scene_lexer::get_namae_cnt(int scn_no)
{
	return m_scn_info_list[scn_no].p_header->namae_cnt;
}

// ****************************************************************
// �y���O�z�̕�������擾
// ================================================================
TSTR C_tnm_scene_lexer::get_namae_str(int scn_no, int namae_no)
{
	int str_index = m_scn_info_list[scn_no].p_namae_list[namae_no];
	return get_string_with_scn_no(scn_no, str_index);
}

// ****************************************************************
// ���ǃt���O�����擾
// ================================================================
int C_tnm_scene_lexer::get_read_flag_cnt(int scn_no)
{
	return m_scn_info_list[scn_no].p_header->read_flag_cnt;
}

// ****************************************************************
// ���ǃt���O�̍s�ԍ����擾
// ================================================================
int C_tnm_scene_lexer::get_read_flag_line_no(int scn_no, int rf_no)
{
	return m_scn_info_list[scn_no].p_read_flag_list[rf_no].line_no;
}

// ****************************************************************
// �V�[���W�����v
// ================================================================
bool C_tnm_scene_lexer::jump_to_scn(int scn_no, int z_no)
{
	// �y�ԍ��͈̔͂𔻒�
	if (z_no < 0 || TNM_Z_LABEL_CNT <= z_no)
		return false;

	// ���݂̃V�[���ԍ���ݒ�
	m_cur_scn_no = scn_no;

	// �V�[���擪����̃I�t�Z�b�g���擾
	int z_label_offset = m_scn_info_list[m_cur_scn_no].p_z_label_list[z_no];

	// �y���x���̈ʒu�Ɉړ�
	m_cur_ptr = m_scn_info_list[m_cur_scn_no].p_scn + z_label_offset;

	return true;
}

// ****************************************************************
// ���x���ɃW�����v
// ================================================================
bool C_tnm_scene_lexer::jump_to_label(int label_no)
{
	// ���x���͈̔͂𔻒�
	if (label_no >= m_scn_info_list[m_cur_scn_no].p_header->label_cnt)
		return false;

	// �V�[���擪����̃I�t�Z�b�g���擾
	int label_offset = m_scn_info_list[m_cur_scn_no].p_label_list[label_no];

	// ���x���̈ʒu�Ɉړ�
	m_cur_ptr = m_scn_info_list[m_cur_scn_no].p_scn + label_offset;

	return true;
}

// ****************************************************************
// ���[�U�R�}���h�ɃW�����v
// ================================================================
bool C_tnm_scene_lexer::jump_to_user_cmd(int scn_no, int user_cmd_no)
{
	// �C���N�R�}���h�̏ꍇ
	if (user_cmd_no < m_pack_scn_info.p_header->inc_cmd_cnt)	{

		// �R�}���h�̎擾
		S_tnm_pack_scn_inc_cmd* p_user_cmd = &m_pack_scn_info.p_inc_cmd_list[user_cmd_no];

		// �R�}���h�����݂���V�[���Ɉړ�
		m_cur_scn_no = p_user_cmd->scn_no;

		// �R�}���h�̈ʒu�Ɉړ�
		m_cur_ptr = m_scn_info_list[m_cur_scn_no].p_scn + p_user_cmd->offset;
	}

	// �V�[���R�}���h�̏ꍇ
	else	{

		// ���[�U�R�}���h�ԍ� �� �V�[���R�}���h�ԍ�
		int scn_cmd_no = user_cmd_no - m_pack_scn_info.p_header->inc_cmd_cnt;

		// �R�}���h�̎擾
		S_tnm_scn_scn_cmd* p_scn_cmd = &m_scn_info_list[scn_no].p_scn_cmd_list[scn_cmd_no];

		// �R�}���h�����݂���V�[���Ɉړ�
		m_cur_scn_no = scn_no;

		// �R�}���h�̈ʒu�Ɉړ�
		m_cur_ptr = m_scn_info_list[scn_no].p_scn + p_scn_cmd->offset;
	}

	return true;
}

// ****************************************************************
// �v���O�����J�E���^���擾
// ================================================================
//		�V�[���̐擪����̃I�t�Z�b�g���v�Z���܂��B
// ================================================================
int C_tnm_scene_lexer::get_prg_cntr()
{
	return (int)((PTR)m_cur_ptr - (PTR)m_scn_info_list[m_cur_scn_no].p_scn);
}

// ****************************************************************
// �v���O�����J�E���^��ݒ�
// ================================================================
//		�V�[���̐擪����̃I�t�Z�b�g�Ŏw�肵�܂��B
// ================================================================
void C_tnm_scene_lexer::set_prg_cntr(int prg_cntr)
{
	m_cur_ptr = m_scn_info_list[m_cur_scn_no].p_scn + prg_cntr;
}

// ****************************************************************
// ��������擾
// ================================================================
//		������ԍ�����C���f�b�N�X���擾���A�������Ԃ�
// ================================================================
TSTR C_tnm_scene_lexer::get_string_with_scn_no(int scn_no, int str_index)
{
	C_index* index = &m_scn_info_list[scn_no].p_str_index_list[str_index];
	TCHAR* start = &m_scn_info_list[scn_no].p_str_list[index->offset];
	TCHAR* end = &m_scn_info_list[scn_no].p_str_list[index->offset + index->size];

	boost::scoped_array<TCHAR> buf(new TCHAR[index->size]);
	for (int i = 0; i < index->size; i++)	{
		buf[i] = start[i] ^ (28807 * str_index);
	}

	return TSTR(buf.get(), buf.get() + index->size);
}

TSTR C_tnm_scene_lexer::get_string(int str_index)
{
	C_index* index = &m_scn_info_list[m_cur_scn_no].p_str_index_list[str_index];
	TCHAR* start = &m_scn_info_list[m_cur_scn_no].p_str_list[index->offset];
	TCHAR* end = &m_scn_info_list[m_cur_scn_no].p_str_list[index->offset + index->size];

	boost::scoped_array<TCHAR> buf(new TCHAR[index->size]);
	for (int i = 0; i < index->size; i++)	{
		buf[i] = start[i] ^ (28807 * str_index);
	}

	return TSTR(buf.get(), buf.get() + index->size);
}

// ****************************************************************
// ���[�U�C���N�v���p�e�B�����擾
// ================================================================
//		���[�U�C���N�v���p�e�B�̖��O���擾����
// ================================================================
TSTR C_tnm_scene_lexer::get_user_inc_prop_name(int prop_id)
{
	C_index* index = &m_pack_scn_info.p_inc_prop_name_index_list[prop_id];
	TCHAR* start = &m_pack_scn_info.p_inc_prop_name_list[index->offset];
	TCHAR* end = &m_pack_scn_info.p_inc_prop_name_list[index->offset + index->size];

	return TSTR(start, end);
}

// ****************************************************************
// ���[�U�V�[���v���p�e�B�����擾
// ================================================================
//		���[�U�V�[���v���p�e�B�̖��O���擾����
// ================================================================
TSTR C_tnm_scene_lexer::get_user_scn_prop_name(int scn_no, int prop_id)
{
	C_index* index = &m_scn_info_list[scn_no].p_scn_prop_name_index_list[prop_id];
	TCHAR* start = &m_scn_info_list[scn_no].p_scn_prop_name_list[index->offset];
	TCHAR* end = &m_scn_info_list[scn_no].p_scn_prop_name_list[index->offset + index->size];

	return TSTR(start, end);
}

// ****************************************************************
// ���[�U�R�[���v���p�e�B�����擾
// ================================================================
//		���[�U�R�[���v���p�e�B�̖��O���擾����
// ================================================================
TSTR C_tnm_scene_lexer::get_user_call_prop_name(int scn_no, int prop_id)
{
	C_index* index = &m_scn_info_list[scn_no].p_call_prop_name_index_list[prop_id];
	TCHAR* start = &m_scn_info_list[scn_no].p_call_prop_name_list[index->offset];
	TCHAR* end = &m_scn_info_list[scn_no].p_call_prop_name_list[index->offset + index->size];

	return TSTR(start, end);
}

// ****************************************************************
// ���[�h�i���[�J���j
// ================================================================
bool C_tnm_scene_lexer::load_local(int scn_no, int line_no, int prg_cntr)
{
	m_cur_scn_no = scn_no;
	m_cur_line_no = line_no;

	// �v���O�����J�E���^��ݒ肷��
	set_prg_cntr(prg_cntr);

	return true;
}
