#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_save_cache.h"

// ****************************************************************
// �Z�[�u�f�[�^�L���b�V���F������
// ================================================================
void C_tnm_save_cache::init()
{
	cache_list.clear();
	cache_list.resize(Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt);

	// �L���b�V���̍ŐV�̃Z�[�u�f�[�^�ԍ���������
	clear_new_cache();
}

// ****************************************************************
// �Z�[�u�f�[�^�L���b�V���F�L��
// ================================================================
bool C_tnm_save_cache::memory_cache(int save_no, S_tnm_save_header* p_header)
{
	// �͈̓`�F�b�N
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// �L���b�V���ɋL��
	cache_list[save_no].save_header = BSP<S_tnm_save_header>(new S_tnm_save_header);
	*cache_list[save_no].save_header = *p_header;

	// �L���b�V���̍ŐV�̃Z�[�u�f�[�^�ԍ����L������i���ŐV�̃Z�[�u�f�[�^�ԍ����L������Ă���ꍇ�̂ݏ�������j

	// �P�D�ʏ�̃Z�[�u�f�[�^
	if (0 <= save_no && save_no < Gp_ini->save_cnt)	{
		if (new_save_data_no != -1 && new_save_data_no != save_no)	{
			if (*cache_list[new_save_data_no].save_header < *cache_list[save_no].save_header)	{
				new_save_data_no = save_no;
			}
		}
	}
	// �Q�D�N�C�b�N�Z�[�u�f�[�^
	if (Gp_ini->save_cnt <= save_no && save_no < Gp_ini->save_cnt + Gp_ini->quick_save_cnt)	{
		int quick_save_no = save_no - Gp_ini->save_cnt;
		if (new_quick_save_data_no != -1 && new_quick_save_data_no != quick_save_no)	{
			if (*cache_list[new_quick_save_data_no + Gp_ini->save_cnt].save_header < *cache_list[quick_save_no + Gp_ini->save_cnt].save_header)	{
				new_quick_save_data_no = quick_save_no;
			}
		}
	}

	return true;
}

// ****************************************************************
// �Z�[�u�f�[�^�L���b�V���F�N���A
// ================================================================
bool C_tnm_save_cache::clear_cache(int save_no)
{
	// �͈̓`�F�b�N
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// �L���b�V���N���A
	cache_list[save_no].save_header.reset();
	cache_list[save_no].data_none_flag = false;			// �f�[�^�����݂��Ȃ��ƌ������̃L���b�V�����Y�ꂸ�N���A����

	// �L���b�V���̍ŐV�̃Z�[�u�f�[�^�ԍ����N���A
	clear_new_cache();

	return true;
}

// ****************************************************************
// �Z�[�u�f�[�^�L���b�V���F�ŐV�̃Z�[�u�f�[�^�ԍ��N���A
// ================================================================
bool C_tnm_save_cache::clear_new_cache()
{
	new_save_data_no = -1;
	new_quick_save_data_no = -1;
	return true;
}

// ****************************************************************
// �Z�[�u�f�[�^�L���b�V���F�Z�[�u
// ================================================================
bool C_tnm_save_cache::save_cache(int save_no, S_tnm_save_header* p_header)
{
	if (p_header == NULL)
		return false;

	// �͈̓`�F�b�N
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// �L���b�V�����N���A
	clear_cache(save_no);

	// �p�X�����߂�
	TSTR path = Gp_dir->get_local_save_file_path(save_no);

	// �t�@�C�����J��
	C_file file;
	if (!file.open(path, _T("rb+")))	{
		cache_list[save_no].data_none_flag = true;		// �f�[�^�����݂��Ȃ��ƌ��������L���b�V���ɋL������
		return false;
	}

	// �t�@�C���ɏ�������
	file.write(p_header, sizeof(S_tnm_save_header));
	file.close();

	return true;
}

// ****************************************************************
// �Z�[�u�f�[�^�L���b�V���F���[�h
// ================================================================
bool C_tnm_save_cache::load_cache(int save_no, S_tnm_save_header* p_header)
{
	if (p_header == NULL)
		return false;

	// �͈̓`�F�b�N
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// �L���b�V�����烍�[�h
	if (cache_list[save_no].save_header)	{
		*p_header = *cache_list[save_no].save_header;
		return true;
	}
	else if (cache_list[save_no].data_none_flag)	{
		return false;		// �f�[�^�����݂��Ȃ�
	}

	// �p�X�����߂�
	TSTR path = Gp_dir->get_local_save_file_path(save_no);

	// �t�@�C�����J��
	C_file file;
	if (!file.open(path, _T("rb")))	{
		cache_list[save_no].data_none_flag = true;		// �f�[�^�����݂��Ȃ��ƌ��������L���b�V���ɋL������
		return false;
	}

	// �t�@�C������ǂݍ���
	file.read(p_header, sizeof(S_tnm_save_header));
	file.close();

	// �L���b�V���ɋL��
	memory_cache(save_no, p_header);
	return true;
}

// ****************************************************************
// �Z�[�u�f�[�^�L���b�V���F�t�@�C���`�F�b�N
// ================================================================
bool C_tnm_save_cache::check_file(int save_no)
{
	// �͈̓`�F�b�N
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// �L���b�V��������Α��݂���
	if (cache_list[save_no].save_header)	{
		return true;
	}
	else if (cache_list[save_no].data_none_flag)	{
		return false;		// �f�[�^�����݂��Ȃ�
	}

	// �p�X�����߂�
	TSTR path = Gp_dir->get_local_save_file_path(save_no);

	// �t�@�C�����J��
	C_file file;
	if (!file.open(path, _T("rb")))	{
		cache_list[save_no].data_none_flag = true;		// �f�[�^�����݂��Ȃ��ƌ��������L���b�V���ɋL������
		return false;
	}

	// �t�@�C������ǂݍ���
	S_tnm_save_header header;
	file.read(&header, sizeof(S_tnm_save_header));
	file.close();

	// �L���b�V���ɋL��
	memory_cache(save_no, &header);
	return true;
}

// ****************************************************************
// �ŐV�̃Z�[�u�f�[�^�ԍ����擾
// ================================================================
int C_tnm_save_cache::get_new_save_data_no()
{
	// �L���b�V���ɋL������Ă���ŐV�f�[�^�ԍ���߂�
	if (new_save_data_no != -1)
	{
		return new_save_data_no;
	}

	// �L���b�V����ǂ݂Ȃ���ŐV�̃Z�[�u�f�[�^��T��
	int new_no = -1;
	S_tnm_save_header header;
	for (int i = 0; i < Gp_ini->save_cnt; i++)
	{
		if (load_cache(i, &header))
		{
			// �ŐV�i���t���ł��傫���j�̃Z�[�u�f�[�^���擾����
			if (new_no == -1 || *cache_list[new_no].save_header < header)
			{
				new_no = i;
			}
		}
	}

	// �L���b�V���ɍŐV�̃Z�[�u�f�[�^�ԍ����L������
	new_save_data_no = new_no;

	return new_save_data_no;
}

// ****************************************************************
// �ŐV�̃N�C�b�N�Z�[�u�f�[�^�ԍ����擾
// ================================================================
int C_tnm_save_cache::get_new_quick_save_data_no()
{
	// �L���b�V���ɋL������Ă���ŐV�f�[�^�ԍ���߂�
	if (new_quick_save_data_no != -1)
	{
		return new_quick_save_data_no;
	}

	// �L���b�V����ǂ݂Ȃ���ŐV�̃Z�[�u�f�[�^��T��
	int new_no = -1;
	S_tnm_save_header header;
	for (int i = 0; i < Gp_ini->quick_save_cnt; i++)
	{
		if (load_cache(i + Gp_ini->save_cnt, &header))
		{
			// �ŐV�i���t���ł��傫���j�̃Z�[�u�f�[�^���擾����
			if (new_no == -1 || *cache_list[new_no + Gp_ini->save_cnt].save_header < header)
			{
				new_no = i;
			}
		}
	}

	// �L���b�V���ɍŐV�̃Z�[�u�f�[�^�ԍ����L������
	new_quick_save_data_no = new_no;

	return new_quick_save_data_no;
}