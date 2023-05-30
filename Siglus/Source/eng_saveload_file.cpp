#include	"pch.h"

#include	"tnm_eng.h"
#include	"data/tnm_ini.h"

#include	<lzss.h>

bool	tnm_save_global_on_file_sub_global_data();		// �O���[�o���f�[�^���t�@�C���ɃZ�[�u
bool	tnm_load_global_on_file_sub_global_data();		// �O���[�o���f�[�^���t�@�C�����烍�[�h
bool	tnm_save_global_on_file_sub_system_data();		// �V�X�e���ݒ�f�[�^���t�@�C���ɃZ�[�u
bool	tnm_load_global_on_file_sub_system_data();		// �V�X�e���ݒ�f�[�^���t�@�C�����烍�[�h
bool	tnm_save_global_on_file_sub_movie_data();		// ���[�r�[�ݒ�f�[�^���t�@�C���ɃZ�[�u
bool	tnm_load_global_on_file_sub_movie_data();		// ���[�r�[�ݒ�f�[�^���t�@�C�����烍�[�h
bool	tnm_save_global_on_file_sub_read_data();		// ���ǃf�[�^���t�@�C���ɃZ�[�u
bool	tnm_load_global_on_file_sub_read_data();		// ���ǃf�[�^���t�@�C�����烍�[�h
bool	tnm_save_global_on_file_sub_config_data();		// ���ݒ�f�[�^���t�@�C���ɃZ�[�u
bool	tnm_load_global_on_file_sub_config_data();		// ���ݒ�f�[�^���t�@�C�����烍�[�h

// ****************************************************************
// �t�@�C���Z�[�u�i���ʁj
// ================================================================

// �o�C�i���f�[�^
bool tnm_save_to_file(CTSTR& file_path, BUFFER& write_data)
{
	// �t�@�C���̓ǂݎ���p���͂���
	file_set_read_only(file_path, false);

	// �t�@�C���ɏ�������
	if (!C_file::write_full_data(file_path, write_data))
		return false;

	Sleep(0);

	// �f�[�^�`�F�b�N�̂��߂ɍ��������񂾃f�[�^��ǂݍ���
	BUFFER read_data;
	if (!C_file::read_full_data(file_path, read_data))
		return false;

	// �������񂾃f�[�^�Ɠǂݍ��񂾃f�[�^���r����
	if (read_data.size() != write_data.size() || memcmp(read_data.get(), write_data.get(), read_data.size()) != 0)	{

		// ���j���f�[�^�Ȃ̂Ńt�@�C���������܂��B
		filesys_delete_file(file_path);

		// �ēx�Z�[�u�t�@�C���ɏ�������
		if (!C_file::write_full_data(file_path, write_data))
			return false;

		// �ēx�f�[�^�`�F�b�N
		BUFFER read_data;
		if (!C_file::read_full_data(file_path, read_data))
			return false;

		if (read_data.size() != write_data.size() || memcmp(read_data.get(), write_data.get(), read_data.size()) != 0)	{

			// ���j���f�[�^�Ȃ̂Ńt�@�C���������܂��B
			filesys_delete_file(file_path);

			return false;
		}
	}

	return true;
}

// �e�L�X�g�f�[�^
bool tnm_save_to_file(CTSTR& file_path, CTSTR& write_data)
{
	// �t�@�C���̓ǂݎ���p���͂���
	file_set_read_only(file_path, false);

	// �t�@�C���ɏ�������
	if (!C_file::write_full_text_in_MB(file_path, write_data))
		return false;

	// �f�[�^�`�F�b�N�̂��߂ɍ��������񂾃f�[�^��ǂݍ���
	TSTR read_data;
	if (!C_file::read_full_text(file_path, read_data))
		return false;

	// �������񂾃f�[�^�Ɠǂݍ��񂾃f�[�^���r����
	if (read_data != write_data)	{

		// ���j���f�[�^�Ȃ̂Ńt�@�C���������܂��B
		filesys_delete_file(file_path);

		// �ēx�Z�[�u�t�@�C���ɏ�������
		if (!C_file::write_full_text_in_MB(file_path, write_data))
			return false;

		// �ēx�f�[�^�`�F�b�N
		TSTR read_data;
		if (!C_file::read_full_text(file_path, read_data))
			return false;

		if (read_data != write_data)	{

			// ���j���f�[�^�Ȃ̂Ńt�@�C���������܂��B
			filesys_delete_file(file_path);

			return false;
		}
	}

	return true;
}

// ****************************************************************
// ���k���Í�
// ================================================================
bool tnm_pack_buffer(ARRAY<BYTE>& src)
{
	int rate = 5;	// �W���͂T�{�m��
	if (src.size() > 1024 * 1024)	{		// 1MB�𒴂��邨�����ȃf�[�^�̏ꍇ
		rate = 2;	// �Q�{�܂ŉ�����
	}

	ARRAY<BYTE> dst(src.size() * rate);		// �T�{�̃T�C�Y���m�ۂ���

	Clzss_pack lzss;
	lzss.ready(src.get(), (DWORD)src.size(), 0);
	while (1)	{
		// ���k�J�n�i�o�͐�o�b�t�@�̃|�C���^�ƃT�C�Y���w��j
		if (lzss.proc(dst.get(), (DWORD)dst.size()))	{
			dst.resize(lzss.pack_size);		// �]���ȗ̈���J�b�g
			break;	// ���k����
		}
		// �o�͐�o�b�t�@�̍Ċm��
		if (lzss.dst_buf_need_size > 0)	{
			dst.resize((DWORD)dst.size() + (int)src.size() * rate);		// ����ɂT�{�̃T�C�Y��ǉ��m�ۂ���
		}
	}

	// �Í�
	tpc_angou(dst.get(), (int)dst.size());

	src = dst;
	return true;
}

// ****************************************************************
// ��������
// ================================================================
bool tnm_unpack_buffer(ARRAY<BYTE>& src)
{
	// ����
	tpc_unangou(src.get(), (int)src.size());

	// ��
	int size = lzss_unpack(src.get(), NULL);
	ARRAY<BYTE> dst(size);				// �o�b�t�@�m��
	lzss_unpack(src.get(), dst.get());

	src = dst;
	return true;
}

// ****************************************************************
// �t�@�C�����[�h�i�V�X�e���f�[�^�j
// ================================================================
bool tnm_load_system_on_file()
{
	// �V�X�e���ݒ�f�[�^��ǂݍ���
	tnm_load_global_on_file_sub_system_data();

	return true;
}

// ****************************************************************
// �t�@�C���Z�[�u�i�O���[�o���j
// ================================================================
bool tnm_save_global_on_file()
{
	// �����������I����ĂȂ��ꍇ�͉������Ȃ��i�N���Ɏ��s�����Ƃ��Ȃǁj
	if (Gp_eng->m_init_success_flag)	{
		
		// �O���[�o���f�[�^
		tnm_save_global_on_file_sub_global_data();

		// �V�X�e���ݒ�f�[�^
		tnm_save_global_on_file_sub_system_data();

		// ���ǃf�[�^
		tnm_save_global_on_file_sub_read_data();

		// ���ݒ�f�[�^
		tnm_save_global_on_file_sub_config_data();
	}

	return true;
}

// ****************************************************************
// �t�@�C�����[�h�i�O���[�o���j
// ================================================================
bool tnm_load_global_on_file()
{
	// �O���[�o���f�[�^
	if (!tnm_load_global_on_file_sub_global_data())
		return false;

	// ���ǃf�[�^
	if (!tnm_load_global_on_file_sub_read_data())
		return false;

	// ���ݒ�f�[�^
	if (!tnm_load_global_on_file_sub_config_data())
		return false;

	return true;
}

// ****************************************************************
// �t�@�C���Z�[�u�i�O���[�o���f�[�^�j
// ================================================================
bool tnm_save_global_on_file_sub_global_data()
{
	// �O���[�o���f�[�^
	C_tnm_save_stream global_data;
	Gp_eng->save_global(global_data);

	// ���k����
	tnm_pack_buffer(global_data.get_buffer());

	// �w�b�_���쐬����
	S_tnm_global_save_header global_header;
	global_header.major_version = 2;
	global_header.minor_version = 0;
	global_header.global_data_size = (int)global_data.size();

	// �S�������݃f�[�^�𓝍�����
	BUFFER global_total_data;
	global_total_data.insert(global_total_data.end(), (BYTE *)&global_header, (BYTE *)&global_header + sizeof(global_header));
	global_total_data.insert(global_total_data.end(), global_data.begin(), global_data.end());

	// �p�X�����߂ăt�H���_���쐬����
	TSTR global_save_file_path = Gp_dir->get_global_save_file_path();
	create_dir_from_file_path(global_save_file_path);
	if (!tnm_save_to_file(global_save_file_path, global_total_data))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�O���[�o���f�[�^�̕ۑ��Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	// ����
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�O���[�o���f�[�^��ۑ����܂����B"));

	return true;
}

// ****************************************************************
// �t�@�C�����[�h�i�O���[�o���f�[�^�j
// ================================================================
bool tnm_load_global_on_file_sub_global_data()
{
	// �O���[�o���Z�[�u�̃p�X�����߂�
	TSTR global_save_file_path = Gp_dir->get_global_save_file_path();

	// �t�@�C���̑��ݔ���
	if (file_get_type(global_save_file_path) == 1)	{

		C_tnm_save_stream global_data;

		// �t�@�C�����J��
		C_file file;
		if (!file.open(global_save_file_path, _T("rb")))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�O���[�o���f�[�^���J���܂���ł����B\n") + get_last_error_msg());
			return false;			// �ǂ߂Ȃ������̂ł����ŏI��
		}
		
		// �w�b�_��ǂݍ���
		S_tnm_global_save_header global_header;
		file.read(&global_header, sizeof(global_header));
		if (global_header.major_version == 2 && global_header.minor_version == 0)	{

			// �f�[�^��ǂݍ���
			global_data.resize(global_header.global_data_size);
			file.read(global_data.get(), (int)global_data.size());
			file.close();

			// �𓀂���
			tnm_unpack_buffer(global_data.get_buffer());

			// �V�[�N�����Z�b�g
			global_data.set_seek(0);

			// ����
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�O���[�o���f�[�^��ǂݍ��݂܂����B"));

			// �G���W���{�̂��Ăяo��
			Gp_eng->load_global(global_data);

			// ����
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�O���[�o���f�[�^�̍č\�z�ɐ������܂����B"));
		}
		else	{

			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�Z�[�u�f�[�^�̃o�[�W�������Â��ł��B\n�Z�[�u�f�[�^���폜���Ă���ēx�Q�[�����N�����ĉ������B"));
			return false;			// �ǂ߂Ȃ������̂ł����ŏI��
		}
	}

	return true;
}

// ****************************************************************
// �t�@�C���Z�[�u�i�V�X�e���ݒ�f�[�^�j
// ================================================================
#if 0
const TSTR system_ini_data =
	_T("// ****************************************************************\n")
	_T("// SiglusEngine �V�X�e���ݒ�\n")
	_T("// ****************************************************************\n")
	_T("\n")
	_T("// Direct3D: �g�p����A�_�v�^�ԍ�\n")
	_T("#D3D.ADAPTER_NO = 0\n")
	_T("\n")
	_T("// Direct3D: ����������҂�\n")
	_T("#D3D.PRESENT_INTERVAL = 1\n")
	_T("\n")
	_T("// Direct3D: �~�b�v�}�b�v�̎����������s��\n")
	_T("#D3D.AUTOGENMIPMAP = 1\n")
	_T("\n")
	_T("// Direct3D: ���_�������n�[�h�E�F�A�ōs��\n")
	_T("#D3D.HARDWARE_VERTEX = 1\n")
	_T("\n")
	_T("// Direct3D: ���C�v�o�b�t�@�̖���\n")
	_T("#D3D.WIPE_BUFFER_COUNT = 2\n")
	_T("\n")
	_T("// DirectSound: �o�b�N�O���E���h�Ŗ������Đ���������\n")
	_T("#DS.PLAY_SILENT_SOUND = 0\n")
;
bool tnm_save_global_on_file_sub_system_data()
{
	// �V�X�e���ݒ�f�[�^�̃p�X�����߂�
	TSTR system_ini_file_path = Gp_dir->get_system_ini_file_path();

	// �t�@�C�����Ȃ������ꍇ�̂ݐV�K�쐬�i�t�@�C�����������ꍇ�͉������Ȃ��j
	if (file_get_type(system_ini_file_path) == -1)	{

		// �V�X�e���ݒ�f�[�^����������
		create_dir_from_file_path(system_ini_file_path);
		if (!tnm_save_file(system_ini_file_path, system_ini_data))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�V�X�e���ݒ�f�[�^�̕ۑ��Ɏ��s���܂����B\n") + get_last_error_msg());
			return false;
		}
	}

	return true;
}
#endif

// ****************************************************************
// �t�@�C���Z�[�u�i�V�X�e���ݒ�f�[�^�j
// ================================================================
bool tnm_save_global_on_file_sub_system_data()
{
	ARRAY<TSTR> system_ini_data;
	system_ini_data.push_back(_T("#D3D.ADAPTER_NO = ") + tostr(Gp_system_config->d3d_adapter_no));
	system_ini_data.push_back(_T("#D3D.HARDWARE_VERTEX = ") + tostr(Gp_system_config->d3d_hardware_vertex_flag ? 1 : 0));
	system_ini_data.push_back(_T("#D3D.PRESENT_INTERVAL = ") + tostr(Gp_system_config->d3d_present_interval));
	system_ini_data.push_back(_T("#D3D.USE_MIPMAP = ") + tostr(Gp_system_config->d3d_use_mipmap_flag ? 1 : 0));
	system_ini_data.push_back(_T("#D3D.WIPE_BUFFER_COUNT = ") + tostr(Gp_system_config->d3d_wipe_buffer_cnt));
	system_ini_data.push_back(_T("#DS.PLAY_SILENT_SOUND = ") + tostr(Gp_system_config->ds_play_silent_sound_flag ? 1 : 0));
	system_ini_data.push_back(_T("#MOVIE_TYPE = ") + tostr(Gp_system_config->movie_type));

	TSTR system_ini_file_path = Gp_dir->get_system_ini_file_path();
	if (!::create_dir_from_file_path(system_ini_file_path))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�Z�[�u�t�H���_�̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
	}
	else if (!C_file::write_full_text_in_MB(system_ini_file_path, system_ini_data))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("system.ini �̏������݂Ɏ��s���܂����B\n") + get_last_error_msg());
	}

	return true;
}

// ****************************************************************
// �t�@�C�����[�h�i�V�X�e���ݒ�f�[�^�j
// ================================================================
bool tnm_load_global_on_file_sub_system_data()
{
	// �V�X�e���ݒ�̃p�X�����߂�
	TSTR system_ini_file_path = Gp_dir->get_system_ini_file_path();

	// �t�@�C���̑��ݔ���
	if (file_get_type(system_ini_file_path) == 1)	{

		// �t�@�C����ǂݍ���
		ARRAY<TSTR> system_ini_data;
		if (!C_file::read_full_text(system_ini_file_path, system_ini_data))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�V�X�e���ݒ�f�[�^���ǂݍ��߂܂���ł����B\n") + get_last_error_msg());
			return false;			// �ǂ߂Ȃ������̂ł����ŏI��
		}

		// �t�@�C������͂���
		for (int i = 0; i < (int)system_ini_data.size(); i++)	{
			TSTR line_data = system_ini_data[i];
			C_str_analizer anl(line_data.begin(), line_data.end());
			int num[4];

			if (false);
			else if (anl.get_format(_T("//")))	{
				// NOP
			}
			else if (anl.get_format(_T("#D3D.ADAPTER_NO = %d"), &num[0]))	{
				Gp_system_config->d3d_adapter_no = num[0];
			}
			else if (anl.get_format(_T("#D3D.HARDWARE_VERTEX = %d"), &num[0]))	{
				Gp_system_config->d3d_hardware_vertex_flag = num[0] ? true : false;
			}
			else if (anl.get_format(_T("#D3D.PRESENT_INTERVAL = %d"), &num[0]))	{
				Gp_system_config->d3d_present_interval = limit(0, num[0], 1);	// 0or1
			}
			else if (anl.get_format(_T("#D3D.USE_MIPMAP = %d"), &num[0]))	{
				Gp_system_config->d3d_use_mipmap_flag = num[0] ? true : false;
			}
			else if (anl.get_format(_T("#D3D.WIPE_BUFFER_COUNT = %d"), &num[0]))	{
				Gp_system_config->d3d_wipe_buffer_cnt = limit(1, num[0], 2);	// 1or2��
			}
			else if (anl.get_format(_T("#DS.PLAY_SILENT_SOUND = %d"), &num[0]))	{
				Gp_system_config->ds_play_silent_sound_flag = num[0] ? true : false;
			}
			else if (anl.get_format(_T("#MOVIE_TYPE = %d"), &num[0]))	{
				Gp_system_config->movie_type = limit(0, num[0], 1);
			}
		}

		// ����
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�V�X�e���ݒ�f�[�^��ǂݍ��݂܂����B"));

		// ����
		if (Gp_system_config->d3d_adapter_no != 0)
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- �f�B�X�v���C�A�_�v�^") + tostr(Gp_system_config->d3d_adapter_no) + _T("�Ԃ��g���܂��B"));
		if (Gp_system_config->d3d_hardware_vertex_flag == false)
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ���_�������\�t�g�E�F�A�ōs���܂��B"));
		if (Gp_system_config->d3d_present_interval == 0)
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- �`��̍ۂɁA�������������܂���B"));
		if (Gp_system_config->d3d_use_mipmap_flag == false)
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- �~�b�v�}�b�v���g���܂���B"));
		if (Gp_system_config->d3d_wipe_buffer_cnt == 1)
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ���C�v�o�b�t�@���P���Ɍ��炵�܂��B"));
		if (Gp_system_config->ds_play_silent_sound_flag)
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- �o�b�N�O���E���h�Ŗ������Đ��������܂��B"));
		if (Gp_system_config->movie_type == TNM_MOVIE_PLAY_TYPE_MCI)
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- MCI �Ń��[�r�[���Đ����܂��B"));
	}

	return true;
}

// ****************************************************************
// �t�@�C���Z�[�u�i���ǃf�[�^�j
// ================================================================
bool tnm_save_global_on_file_sub_read_data()
{
	C_tnm_save_stream read_data;

	if (!Gp_lexer->get_cut_ptr())
		return true;	// �G���[�ł͂Ȃ�

	// �V�[�������܂߂ăZ�[�u���Ă݂悤�B
	int scn_cnt = Gp_lexer->get_scn_cnt();
	for (int s = 0; s < scn_cnt; s++)	{
		TSTR scn_name = Gp_lexer->get_scn_name(s);
		read_data.save(scn_name);						// �V�[����
		read_data.save((*Gp_read_flag)[s].size());		// ���ǃt���O�̌�
		read_data.save((*Gp_read_flag)[s].get(), (int)(*Gp_read_flag)[s].size());
	}

	// ���ǃf�[�^�̈Í���
	tnm_pack_buffer(read_data.get_buffer());

	// �w�b�_�̍\�z
	S_tnm_read_save_header header;
	header.major_version = 1;
	header.minor_version = 0;
	header.read_data_size = (int)read_data.size();
	header.scn_cnt = scn_cnt;

	// �S�������݃f�[�^����������
	BUFFER total_data;
	total_data.insert(total_data.end(), (BYTE *)&header, (BYTE *)&header + sizeof(header));
	total_data.insert(total_data.end(), read_data.begin(), read_data.end());

	// �p�X�����߂ăt�H���_���쐬����
	TSTR read_save_file_path = Gp_dir->get_read_save_file_path();
	create_dir_from_file_path(read_save_file_path);
	if (!tnm_save_to_file(read_save_file_path, total_data))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���ǃf�[�^�̕ۑ��Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	// ����
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���ǃf�[�^��ۑ����܂����B"));

	return true;
}

// ****************************************************************
// �t�@�C�����[�h�i���ǃf�[�^�j
// ================================================================
bool tnm_load_global_on_file_sub_read_data()
{
	C_tnm_save_stream read_data;

	TSTR file_path = Gp_dir->get_read_save_file_path();

	// �t�@�C���̑��ݔ���
	if (file_get_type(file_path) != 1)
		return true;			// �t�@�C�����Ȃ��ꍇ�͉������Ȃ�

	// �t�@�C�����J��
	C_file file;
	if (!file.open(file_path, _T("rb")))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���ǃt�@�C�����J���܂���ł����B\n") + get_last_error_msg());
		return false;			// �ǂ߂Ȃ������̂ł����ŏI��
	}

	// �t�@�C����ǂݍ���
	S_tnm_read_save_header header;
	file.read(&header, sizeof(header));
	read_data.resize(header.read_data_size);
	file.read(read_data.get(), header.read_data_size);

	// ���ǃf�[�^����
	tnm_unpack_buffer(read_data.get_buffer());

	// �V�[�N�����Z�b�g
	read_data.set_seek(0);

	// �ǂݍ��݊J�n
	for (int i = 0; i < header.scn_cnt; i++)	{
		TSTR scn_name;
		read_data.load(scn_name);							// �V�[����
		int save_flag_cnt = read_data.load_ret<int>();		// ���ǃt���O�̌�
		int scn_no = Gp_lexer->get_scn_no(scn_name);
		if (scn_no >= 0)	{
			int real_flag_cnt = Gp_lexer->get_read_flag_cnt(scn_no);

			// �Z�[�u�������ǃt���O�Ǝ��ۂ̊��ǃt���O�̌�����������ΓK�p
			if (save_flag_cnt == real_flag_cnt)	{
				read_data.load((*Gp_read_flag)[scn_no].get(), save_flag_cnt);
			}
			// �T�C�Y���Ⴆ�Γǂݔ�΂�
			else	{
				read_data.add_seek(save_flag_cnt);
			}
		}
		// �V�[�����Ȃ���Γǂݔ�΂�
		else	{
			read_data.add_seek(save_flag_cnt);
		}
	}

	// ����
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���ǃf�[�^��ǂݍ��݂܂����B"));

	return true;
}

// ****************************************************************
// �t�@�C���Z�[�u�i���ݒ�f�[�^�j
// ================================================================
bool tnm_save_global_on_file_sub_config_data()
{
	// �R���t�B�O�f�[�^
	C_tnm_save_stream config_data;
	Gp_eng->save_config(config_data);

	// ���k����
	tnm_pack_buffer(config_data.get_buffer());

	// �w�b�_���쐬����
	S_tnm_config_save_header config_header;
	config_header.major_version = 1;
	config_header.minor_version = 3;
	config_header.config_data_size = (int)config_data.size();

	// �S�������݃f�[�^�𓝍�����
	BUFFER config_total_data;
	config_total_data.insert(config_total_data.end(), (BYTE *)&config_header, (BYTE *)&config_header + sizeof(config_header));
	config_total_data.insert(config_total_data.end(), config_data.begin(), config_data.end());

	// �p�X�����߂ăt�H���_���쐬����
	TSTR config_save_file_path = Gp_dir->get_config_save_file_path();
	create_dir_from_file_path(config_save_file_path);
	if (!tnm_save_to_file(config_save_file_path, config_total_data))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���ݒ�f�[�^�̕ۑ��Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	// ����
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���ݒ�f�[�^��ۑ����܂����B"));

	return true;
}

// ****************************************************************
// �t�@�C�����[�h�i���ݒ�f�[�^�j
// ================================================================
bool tnm_load_global_on_file_sub_config_data()
{
	// ���ݒ�Z�[�u�̃p�X�����߂�
	TSTR config_save_file_path = Gp_dir->get_config_save_file_path();

	// �t�@�C���̑��ݔ���
	if (file_get_type(config_save_file_path) == 1)	{

		C_tnm_save_stream config_data;

		// �t�@�C�����J��
		C_file file;
		if (!file.open(config_save_file_path, _T("rb")))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���ݒ�f�[�^���J���܂���ł����B\n") + get_last_error_msg());
			return false;			// �ǂ߂Ȃ������̂ł����ŏI��
		}
		
		// �f�[�^��ǂݍ���
		S_tnm_config_save_header config_header;
		file.read(&config_header, sizeof(config_header));
		if (config_header.major_version == 1 && config_header.minor_version >= 2)
		{
			config_data.resize(config_header.config_data_size);
			file.read(config_data.get(), (int)config_data.size());
			file.close();

			// �𓀂���
			tnm_unpack_buffer(config_data.get_buffer());

			// �V�[�N�����Z�b�g
			config_data.set_seek(0);

			// ����
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���ݒ�f�[�^��ǂݍ��݂܂����B"));

			// �G���W���{�̂��Ăяo��
			Gp_eng->load_config(config_data, config_header.major_version, config_header.minor_version);
		}
		else
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���ݒ�f�[�^�̃o�[�W�������Â��ł��B\n�Z�[�u�f�[�^���폜���Ă���ēx�Q�[�����N�����ĉ������B"));
			return false;			// �ǂ߂Ȃ������̂ł����ŏI��
		}
	}

	return true;
}

// ****************************************************************
// �t�@�C���Z�[�u�i���[�J���j
// ================================================================

// �t�@�C���ɃZ�[�u
bool tnm_save_local_on_file(int save_no)
{
	// �Z�[�u�f�[�^����Ȃ牽�����Ȃ�
	if (Gp_eng->m_local_save.save_stream.empty())
		return false;

	// �͈̓`�F�b�N
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// �L���b�V�����N���A����
	Gp_eng->m_save_cache.clear_cache(save_no);

	// ���[�J���ݒ�f�[�^��ۑ�
	Gp_eng->save_local_ex();

	// �X�g���[���Ƀf�[�^���\�z���Ă���
	C_stream data_stream;
	{
		// ���[�J���Z�[�u�f�[�^
		data_stream.push(Gp_eng->m_local_save.save_id);
		data_stream.push_str_len(Gp_eng->m_local_save.append_dir);
		data_stream.push_str_len(Gp_eng->m_local_save.append_name);
		data_stream.push_str_len(Gp_eng->m_local_save.save_scene_title);
		data_stream.push_str_len(Gp_eng->m_local_save.save_msg);
		data_stream.push_str_len(Gp_eng->m_local_save.save_full_msg);
		data_stream.push(Gp_eng->m_local_save.save_stream.size());
		data_stream.push(Gp_eng->m_local_save.save_stream.get(), Gp_eng->m_local_save.save_stream.size());
		data_stream.push(Gp_eng->m_local_save.save_stream_ex.size());
		data_stream.push(Gp_eng->m_local_save.save_stream_ex.get(), Gp_eng->m_local_save.save_stream_ex.size());

		// �I�����Z�[�u�f�[�^
		data_stream.push(Gp_eng->m_sel_save.map.size());
		for (std::map<S_tid, BSP<S_tnm_local_save>>::iterator itr = Gp_eng->m_sel_save.map.begin(); itr != Gp_eng->m_sel_save.map.end(); ++itr)	{

			data_stream.push(itr->second->save_id);
			data_stream.push_str_len(itr->second->append_dir);
			data_stream.push_str_len(itr->second->append_name);
			data_stream.push_str_len(itr->second->save_scene_title);
			data_stream.push_str_len(itr->second->save_msg);
			data_stream.push_str_len(itr->second->save_full_msg);
			data_stream.push(itr->second->save_stream.size());
			data_stream.push(itr->second->save_stream.get(), itr->second->save_stream.size());
			data_stream.push(itr->second->save_stream_ex.size());
			data_stream.push(itr->second->save_stream_ex.get(), itr->second->save_stream_ex.size());
		}
	}

	// �f�[�^�����k
	BUFFER data_buffer_angou = data_stream.get_buffer();

	tnm_pack_buffer(data_buffer_angou);

	// �������擾
	SYSTEMTIME system_time;
	GetLocalTime(&system_time);
	
	// �w�b�_���\�z�iver 1.0�j
	S_tnm_save_header header = {0};
	header.major_version = 1;
	header.minor_version = 0;
	header.year = system_time.wYear;
	header.month = system_time.wMonth;
	header.day = system_time.wDay;
	header.weekday = system_time.wDayOfWeek;
	header.hour = system_time.wHour;
	header.minute = system_time.wMinute;
	header.second = system_time.wSecond;
	header.millisecond = system_time.wMilliseconds;
	ZeroMemory(header.append_dir, sizeof(header.append_dir));		// �A�y���h�f�B���N�g��
	_tcsncpy_s(header.append_dir, TNM_SAVE_APPEND_DIR_MAX_LEN, Gp_eng->m_local_save.append_dir.c_str(), TNM_SAVE_APPEND_DIR_MAX_LEN - 1);
	ZeroMemory(header.append_name, sizeof(header.append_name));		// �A�y���h��
	_tcsncpy_s(header.append_name, TNM_SAVE_APPEND_NAME_MAX_LEN, Gp_eng->m_local_save.append_name.c_str(), TNM_SAVE_APPEND_NAME_MAX_LEN - 1);
	ZeroMemory(header.title, sizeof(header.title));					// �^�C�g��
	_tcsncpy_s(header.title, TNM_SAVE_TITLE_MAX_LEN, Gp_eng->m_local_save.save_scene_title.c_str(), TNM_SAVE_TITLE_MAX_LEN - 1);
	ZeroMemory(header.message, sizeof(header.message));				// ���b�Z�[�W
	_tcsncpy_s(header.message, TNM_SAVE_MESSAGE_MAX_LEN, Gp_eng->m_local_save.save_msg.c_str(), TNM_SAVE_MESSAGE_MAX_LEN - 1);
	ZeroMemory(header.full_message, sizeof(header.full_message));				// �t�����b�Z�[�W
	_tcsncpy_s(header.full_message, TNM_SAVE_FULL_MESSAGE_MAX_LEN, Gp_eng->m_local_save.save_full_msg.c_str(), TNM_SAVE_FULL_MESSAGE_MAX_LEN - 1);
	ZeroMemory(header.comment, sizeof(header.comment));				// �R�����g�P�i�\��j
	ZeroMemory(header.comment2, sizeof(header.comment2));			// �R�����g�Q�i�\��j
	ZeroMemory(header.flag, sizeof(int) * TNM_SAVE_FLAG_MAX_CNT);	// �t���O�i�\��j
	header.data_size = (int)data_buffer_angou.size();				// �f�[�^�T�C�Y�i�Í����ς݁j

	// �S�������݃f�[�^����������
	BUFFER total_data;
	total_data.insert(total_data.end(), (BYTE *)&header, (BYTE *)&header + sizeof(header));
	total_data.insert(total_data.end(), data_buffer_angou.begin(), data_buffer_angou.end());

	// �p�X�����߂ăt�H���_���쐬����
	TSTR save_path = Gp_dir->get_local_save_file_path(save_no);
	if (!create_dir_from_file_path(save_path))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�Z�[�u�t�H���_�̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}
	if (!tnm_save_to_file(save_path, total_data))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�Z�[�u�f�[�^�̕ۑ��Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	// ����
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�Z�[�u�f�[�^��ۑ����܂����B"));

	// �Z�[�u�T���l�C�����g��Ȃ��ꍇ�͂����ŏI��
	if (!Gp_ini->use_save_thumb)	{
		return true;
	}

	// �Â��Z�[�u�T���l�C�����폜����
	TSTR thumb_path = Gp_dir->get_save_thumb_file_path(save_no);
	if (file_get_type(thumb_path) == 1)	{
		file_set_read_only(thumb_path, false);
		if (!filesys_delete_file(thumb_path))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�Â��Z�[�u�T���l�C�����폜�ł��܂���ł����B\n�i�f�[�^�̃Z�[�u�͐���ɍs���܂����B�j\n") + get_last_error_msg());
			return false;
		}
	}

	// �Z�[�u�T���l�C��������Ă��Ȃ��ꍇ�̓G���[
	if (!Gp_wnd->m_capture_texture_for_save_thumb)	{
		tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("�Z�[�u�T���l�C��������Ă��܂���B"));
		return false;
	}

	// ================================================================
	// �L���v�`���[�o�b�t�@����s�N�Z���f�[�^�����o���܂��B
	// ================================================================
	BSP<C_d3d_texture> p_texture = Gp_wnd->m_capture_texture_for_save_thumb;
	int capture_prior = Gp_global->capture_prior;
	int width = p_texture->get_width();
	int height = p_texture->get_height();
	D3DLOCKED_RECT rect;
	ARRAY<BYTE> bmp_buffer;

	// png �̏ꍇ
	if (Gp_ini->save_thumb_type == TNM_SAVE_THUMB_TYPE_PNG)	{

		// �e�N�X�`�������b�N����
		if (!p_texture->lock_rect(0, &rect, NULL, 0))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�T���l�C���p�e�N�X�`���̃��b�N�Ɏ��s���܂����B\n") + get_last_error_msg());
			return false;
		}

		// png �ɏ�������
		change_extension(thumb_path, _T("png"));
		if (!tnm_create_png_from_texture_and_save_to_file(thumb_path, width, height, &rect, false))	{		// �A���t�@�l���g��Ȃ�
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�Z�[�u�T���l�C�����t�@�C���ɕۑ��ł��܂���ł����B\n") + get_last_error_msg());
			return false;
		}

		// �e�N�X�`�����A�����b�N����
		if (!p_texture->unlock_rect(0))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�T���l�C���p�e�N�X�`���̃A�����b�N�Ɏ��s���܂����B\n") + get_last_error_msg());
			return false;
		}
	}
	// bmp �̏ꍇ
	else	{

		// �e�N�X�`�������b�N����
		if (!p_texture->lock_rect(0, &rect, NULL, 0))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�T���l�C���p�e�N�X�`���̃��b�N�Ɏ��s���܂����B\n") + get_last_error_msg());
			return false;
		}

		// bmp �ɗ��Ƃ�����
		if (!tnm_create_bmp_from_texture(bmp_buffer, width, height, &rect))
			return false;

		// �e�N�X�`�����A�����b�N����
		if (!p_texture->unlock_rect(0))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�T���l�C���p�e�N�X�`���̃A�����b�N�Ɏ��s���܂����B\n") + get_last_error_msg());
			return false;
		}

		// �t�@�C���ɕۑ�
		if (!tnm_save_to_file(thumb_path, bmp_buffer))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�Z�[�u�T���l�C�����t�@�C���ɕۑ��ł��܂���ł����B\n") + get_last_error_msg());
			return false;
		}
	}

	// ����
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�Z�[�u�T���l�C����ۑ����܂����B"));

	return true;
}

// ****************************************************************
// �t�@�C�����烍�[�h�i���[�J���j
// ================================================================
bool tnm_load_local_on_file(int save_no)
{
	// �͈̓`�F�b�N
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)	{
		return false;
	}

	// �Z�[�u�f�[�^���N���A����
	Gp_eng->m_local_save.save_id = S_tid();
	Gp_eng->m_local_save.append_dir.clear();
	Gp_eng->m_local_save.append_name.clear();
	Gp_eng->m_local_save.save_scene_title.clear();
	Gp_eng->m_local_save.save_msg.clear();
	Gp_eng->m_local_save.save_full_msg.clear();
	Gp_eng->m_local_save.save_stream.clear();
	Gp_eng->m_local_save.save_stream_ex.clear();
	Gp_eng->m_sel_save.list.clear();
	Gp_eng->m_sel_save.map.clear();

	// �p�X�����߂�
	TSTR save_path = Gp_dir->get_local_save_file_path(save_no);

	// �t�@�C�����J��
	C_file file;
	if (!file.open(save_path, _T("rb")))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�Z�[�u�f�[�^���ǂݍ��߂܂���ł����B\n") + get_last_error_msg());
		return false;
	}

	// �t�@�C������ǂݍ���
	S_tnm_save_header header;
	file.read(&header, sizeof(header));
	Gp_eng->m_local_save.append_dir = header.append_dir;
	Gp_eng->m_local_save.append_name = header.append_name;
	Gp_eng->m_local_save.save_scene_title = header.title;
	Gp_eng->m_local_save.save_msg = header.message;
	Gp_eng->m_local_save.save_full_msg = header.full_message;

	C_stream data_stream;
	data_stream.resize(header.data_size);
	file.read(data_stream.get(), header.data_size);
	tnm_unpack_buffer(data_stream.get_buffer());

	// �X�g���[������f�[�^���\�z���Ă���
	{
		data_stream.set_seek();

		// ���[�J���Z�[�u�f�[�^
		Gp_eng->m_local_save.save_id = data_stream.pop_ret<S_tid>();
		Gp_eng->m_local_save.append_dir = data_stream.pop_str_len_ret<TSTR>();
		Gp_eng->m_local_save.append_name = data_stream.pop_str_len_ret<TSTR>();
		Gp_eng->m_local_save.save_scene_title = data_stream.pop_str_len_ret<TSTR>();
		Gp_eng->m_local_save.save_msg = data_stream.pop_str_len_ret<TSTR>();
		Gp_eng->m_local_save.save_full_msg = data_stream.pop_str_len_ret<TSTR>();
		int local_save_size = data_stream.pop_ret<int>();
		Gp_eng->m_local_save.save_stream.resize(local_save_size);
		data_stream.pop(Gp_eng->m_local_save.save_stream.get(), local_save_size);
		int local_save_ex_size = data_stream.pop_ret<int>();
		Gp_eng->m_local_save.save_stream_ex.resize(local_save_ex_size);
		data_stream.pop(Gp_eng->m_local_save.save_stream_ex.get(), local_save_ex_size);

		// �I�����f�[�^
		int sel_save_cnt = data_stream.pop_ret<int>();
		for (int i = 0; i < sel_save_cnt; i++)	{

			BSP<S_tnm_local_save> sel_save(new S_tnm_local_save);
			sel_save->save_id = data_stream.pop_ret<S_tid>();
			sel_save->append_dir = data_stream.pop_str_len_ret<TSTR>();
			sel_save->append_name = data_stream.pop_str_len_ret<TSTR>();
			sel_save->save_scene_title = data_stream.pop_str_len_ret<TSTR>();
			sel_save->save_msg = data_stream.pop_str_len_ret<TSTR>();
			sel_save->save_full_msg = data_stream.pop_str_len_ret<TSTR>();
			int sel_save_size = data_stream.pop_ret<int>();
			sel_save->save_stream.resize(sel_save_size);
			data_stream.pop(sel_save->save_stream.get(), sel_save_size);
			int sel_save_ex_size = data_stream.pop_ret<int>();
			sel_save->save_stream_ex.resize(sel_save_ex_size);
			data_stream.pop(sel_save->save_stream_ex.get(), sel_save_ex_size);

			Gp_eng->m_sel_save.map[sel_save->save_id] = sel_save;
		}
	}

	// �t�@�C�������
	file.close();

	// ����
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�Z�[�u�f�[�^��ǂݍ��݂܂����B"));

	return true;
}

// ****************************************************************
// �t�@�C�����폜
// ================================================================
bool tnm_delete_save_file(int save_no)
{
	// �͈̓`�F�b�N
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// �L���b�V������`�F�b�N
	if (!Gp_eng->m_save_cache.check_file(save_no))
		return false;

	// �L���b�V�����N���A����
	Gp_eng->m_save_cache.clear_cache(save_no);

	// �p�X�����߂�
	TSTR save_path = Gp_dir->get_local_save_file_path(save_no);
	TSTR thumb_path = Gp_dir->get_save_thumb_file_path(save_no);

	// �t�@�C������������
	file_set_read_only(save_path, false);
	if (!filesys_delete_file(save_path))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�Z�[�u�t�@�C���̍폜�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	// �T���l�C������������
	if (Gp_ini->use_save_thumb)	{
		file_set_read_only(thumb_path, false);
		if (!filesys_delete_file(thumb_path))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�Z�[�u�T���l�C���̍폜�Ɏ��s���܂����B\n") + get_last_error_msg());
			return false;
		}
	}

	// ����
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�Z�[�u�t�@�C�����폜���܂����B"));

	return true;
}

// ****************************************************************
// �t�@�C�����R�s�[
// ================================================================
bool tnm_copy_save_file(int save_no_from, int save_no_to)
{
	// �͈̓`�F�b�N
	if (save_no_from < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no_from)
		return false;
	if (save_no_to < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no_to)
		return false;

	// �����t�@�C���Ȃ玸�s
	if (save_no_from == save_no_to)
		return false;

	// �L���b�V�����N���A����
	Gp_eng->m_save_cache.clear_cache(save_no_to);

	// �p�X�����߂�
	TSTR save_path_from = Gp_dir->get_local_save_file_path(save_no_from);
	TSTR save_path_to = Gp_dir->get_local_save_file_path(save_no_to);
	TSTR thumb_path_from = Gp_dir->get_save_thumb_file_path(save_no_from);
	TSTR thumb_path_to = Gp_dir->get_save_thumb_file_path(save_no_to);

	// �t�@�C���O���t�@�C���P�ɃR�s�[
	file_set_read_only(save_path_to, false);
	if (!filesys_copy_file(save_path_from, save_path_to))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�t�@�C���̃R�s�[�Ɏ��s���܂����B%d �� %d"), save_no_from, save_no_to) + _T("\n") + get_last_error_msg());
		return false;
	}

	// �T���l�C���O���T���l�C���P�ɃR�s�[
	if (Gp_ini->use_save_thumb)	{
		file_set_read_only(thumb_path_to, false);
		if (!filesys_copy_file(thumb_path_from, thumb_path_to))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�T���l�C���̃R�s�[�Ɏ��s���܂����B%d �� %d"), save_no_from, save_no_to) + _T("\n") + get_last_error_msg());
			return false;
		}
	}

	return true;
}

// ****************************************************************
// �t�@�C��������
// ================================================================
bool tnm_change_save_file(int save_no_0, int save_no_1)
{
	// �͈̓`�F�b�N
	if (save_no_0 < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no_0)
		return false;
	if (save_no_1 < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no_1)
		return false;

	// �����t�@�C���Ȃ玸�s
	if (save_no_0 == save_no_1)
		return false;

	// ���݂��ɔ��肵�Ă���
	bool save_exist_0 = Gp_eng->m_save_cache.check_file(save_no_0);
	bool save_exist_1 = Gp_eng->m_save_cache.check_file(save_no_1);

	// �L���b�V�����N���A����
	Gp_eng->m_save_cache.clear_cache(save_no_0);
	Gp_eng->m_save_cache.clear_cache(save_no_1);

	// �p�X�����߂�
	TSTR save_path_0 = Gp_dir->get_local_save_file_path(save_no_0);
	TSTR save_path_1 = Gp_dir->get_local_save_file_path(save_no_1);
	TSTR thumb_path_0 = Gp_dir->get_save_thumb_file_path(save_no_0);
	TSTR thumb_path_1 = Gp_dir->get_save_thumb_file_path(save_no_1);

	if (false);
	// �t�@�C���O�݂̂�����ꍇ
	else if (save_exist_0 && !save_exist_1)	{

		// �t�@�C���O���t�@�C���P�Ƀ��l�[��
		if (!filesys_rename_file(save_path_0, save_path_1))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�t�@�C�����̕ύX�Ɏ��s���܂����B%d �� %d"), save_no_0, save_no_1) + _T("\n") + get_last_error_msg());
			return false;
		}
		// �T���l�C���O���T���l�C���P�Ƀ��l�[��
		if (Gp_ini->use_save_thumb)	{
			if (!filesys_rename_file(thumb_path_0, thumb_path_1))	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�T���l�C�����̕ύX�Ɏ��s���܂����B%d �� %d"), save_no_0, save_no_1) + _T("\n") + get_last_error_msg());
				return false;
			}
		}

		// ����
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("�t�@�C������ύX���܂����B%d �� %d"), save_no_0, save_no_1));
	}
	// �t�@�C���P�݂̂�����ꍇ
	else if (!save_exist_0 && save_exist_1)	{

		// �t�@�C���P���t�@�C���O�Ƀ��l�[��
		if (!filesys_rename_file(save_path_1, save_path_0))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�t�@�C�����̕ύX�Ɏ��s���܂����B%d �� %d"), save_no_0, save_no_1) + _T("\n") + get_last_error_msg());
			return false;
		}
		// �T���l�C���P���T���l�C���O�Ƀ��l�[��
		if (Gp_ini->use_save_thumb)	{
			if (!filesys_rename_file(thumb_path_1, thumb_path_0))	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�T���l�C�����̕ύX�Ɏ��s���܂����B%d �� %d"), save_no_0, save_no_1) + _T("\n") + get_last_error_msg());
				return false;
			}
		}

		// ����
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("�t�@�C������ύX���܂����B%d �� %d"), save_no_0, save_no_1));
	}
	// �t�@�C���O���t�@�C���P���Ȃ��ꍇ
	else if (!save_exist_0 && !save_exist_1)	{

		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("��������t�@�C�������ɑ��݂��܂���ł����B")));
		return false;
	}
	// �t�@�C���O���t�@�C���P������ꍇ
	else if (save_exist_0 && save_exist_1)	{

		TSTR tmp_save_path_0 = save_path_0 + _T("__tmp");
		TSTR tmp_save_path_1 = save_path_1 + _T("__tmp");
		TSTR tmp_thumb_path_0 = thumb_path_0 + _T("__tmp");
		TSTR tmp_thumb_path_1 = thumb_path_1 + _T("__tmp");

		// �e���|�����t�@�C�����폜����
		filesys_delete_file(tmp_save_path_0);
		filesys_delete_file(tmp_save_path_1);
		filesys_delete_file(tmp_thumb_path_0);
		filesys_delete_file(tmp_thumb_path_1);

		// �t�@�C���O���e���|�����t�@�C���Ƀ��l�[��
		if (!filesys_rename_file(save_path_0, tmp_save_path_0))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�t�@�C�����̕ύX�Ɏ��s���܂����B%d �� tmp"), save_no_0) + _T("\n") + get_last_error_msg());
			return false;
		}

		// �t�@�C���O�����ɖ߂�
		filesys_rename_file(tmp_save_path_0, save_path_0);

		// �t�@�C���P���e���|�����t�@�C���Ƀ��l�[��
		if (!filesys_rename_file(save_path_1, tmp_save_path_1))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�t�@�C�����̕ύX�Ɏ��s���܂����B%d �� tmp"), save_no_1) + _T("\n") + get_last_error_msg());
			return false;
		}

		// �t�@�C���P�����ɖ߂�
		filesys_rename_file(tmp_save_path_1, save_path_1);

		// �T���l�C�����g���ꍇ�̏���
		if (Gp_ini->use_save_thumb)	{

			// �T���l�C���O���e���|�����t�@�C���Ƀ��l�[��
			if (!filesys_rename_file(thumb_path_0, tmp_thumb_path_0))	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�T���l�C�����̕ύX�Ɏ��s���܂����B%d �� tmp"), save_no_0) + _T("\n") + get_last_error_msg());
				return false;
			}

			// �T���l�C���O�����ɖ߂�
			filesys_rename_file(tmp_thumb_path_0, thumb_path_0);

			// �T���l�C���P���e���|�����t�@�C���Ƀ��l�[��
			if (!filesys_rename_file(thumb_path_1, tmp_thumb_path_1))	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�T���l�C�����̕ύX�Ɏ��s���܂����B%d �� tmp"), save_no_1) + _T("\n") + get_last_error_msg());
				return false;
			}

			// �T���l�C���P�����ɖ߂�
			filesys_rename_file(tmp_thumb_path_1, thumb_path_1);
		}

		// ������ŁA�����̃t�@�C�������l�[���\���ƌ������Ƃ����������I

		// �t�@�C���O���e���|�����t�@�C���Ƀ��l�[��
		filesys_rename_file(save_path_0, tmp_save_path_0);
		// �t�@�C���P���e���|�����t�@�C���Ƀ��l�[��
		filesys_rename_file(save_path_1, tmp_save_path_1);
		// �t�@�C���O���t�@�C���P�Ƀ��l�[��
		filesys_rename_file(tmp_save_path_0, save_path_1);
		// �t�@�C���P���t�@�C���O�Ƀ��l�[��
		filesys_rename_file(tmp_save_path_1, save_path_0);

		// �T���l�C�����g���ꍇ�̏���
		if (Gp_ini->use_save_thumb)	{

			// �T���l�C���O���e���|�����t�@�C���Ƀ��l�[��
			filesys_rename_file(thumb_path_0, tmp_thumb_path_0);
			// �T���l�C���P���e���|�����t�@�C���Ƀ��l�[��
			filesys_rename_file(thumb_path_1, tmp_thumb_path_1);
			// �T���l�C���O���t�@�C���P�Ƀ��l�[��
			filesys_rename_file(tmp_thumb_path_0, thumb_path_1);
			// �T���l�C���P���t�@�C���O�Ƀ��l�[��
			filesys_rename_file(tmp_thumb_path_1, thumb_path_0);
		}

		// ����
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("�t�@�C������ύX���܂����B%d �� %d"), save_no_0, save_no_1));
	}

	return true;
}

