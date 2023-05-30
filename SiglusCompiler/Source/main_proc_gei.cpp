#include	"pch.h"
#include	"tnm_common_header.h"
#include	"data.h"
#include	"main.h"
#include	"ini.h"
#include	"lzss.h"
#include	"exe_angou.h"

BYTE tns_gameexe_dat_angou_code[TNM_GAMEEXE_DAT_ANGOU_CODE_SIZE] = TNM_GAMEEXE_DAT_ANGOU_CODE;

// ****************************************************************
// �����FGameexe.ini ���
// ================================================================
bool C_tnm_compiler_main::proc_gei()
{
	// �v���W�F�N�g���I������Ă��Ȃ���Ύ��s
	if (!check_current_project(true))	{	return false;	}

	TSTR scn_dir_path = G_data->project[G_data->cur_project_no].scn_path;
	TSTR tmp_dir_path = G_data->project[G_data->cur_project_no].tmp_path;
	TSTR out_dir_path = G_data->project[G_data->cur_project_no].out_path;
	TSTR out_dir_path_noangou = G_data->project[G_data->cur_project_no].out_path_noangou;
	TSTR gameexe_ini = G_data->project[G_data->cur_project_no].gameexe_ini;
	TSTR gameexe_dat = G_data->project[G_data->cur_project_no].gameexe_dat;
	bool utf8 = G_data->project[G_data->cur_project_no].utf8;

	// ���ʂ̃e�L�X�g
	TSTR ged_text;

	// Gameexe.ini ����
	TSTR gei_text;
	TSTR gei_path = scn_dir_path + _T("\\") + gameexe_ini;
	if (!(utf8 ? C_file::read_full_text_UTF8(gei_path, gei_text) : C_file::read_full_text(gei_path, gei_text)))
	{
		set_log(gameexe_ini + _T(" ��������܂���ł����B��� Gameexe.dat ���쐬���܂��B"));
	}
	else
	{
		// inc �t�@�C����͊J�n
		set_log(_T("- Gameexe.ini ����͂��܂��B"));

		// Gameexe.ini ���
		C_ini_file_analizer inia;
		if (!inia.analize(gei_text, ged_text))
		{
			set_log_jump(gei_path + _T(" line(") + tostr(inia.get_error_line()) + _T("): ") + inia.get_error_str(), gei_path, gei_path, inia.get_error_line());
			return false;
		}
	}

	// �Í��R�[�h�쐬
	TSTR exe_angou_str;
	BYTE exe_angou_element[TNM_EXE_ANGOU_ELEMENT_CNT];
	int exe_angou_mode = 0;
	if (G_main->load_exe_angou_dat(exe_angou_str))
	{
		if (!exe_angou_str.empty())
		{
			MBSTR mb_exe_angou_str = TSTR_to_MBSTR(exe_angou_str);
			if ((int)mb_exe_angou_str.size() >= 8)
			{
				C_tnms_exe_angou::make_angou_element(exe_angou_str, exe_angou_element);
				exe_angou_mode = 1;
				set_log(_T("- �Í������܂��B ( ") + exe_angou_str + _T(" )"));

#if 1			// ���ʏ��1
				// ���Í��o��
				BYTE angou[16];
				angou[0] = exe_angou_element[TNM_EXE_ANGOU_ELEMENT00A];
				angou[1] = exe_angou_element[TNM_EXE_ANGOU_ELEMENT01A];
				angou[2] = exe_angou_element[TNM_EXE_ANGOU_ELEMENT02A];
				angou[3] = exe_angou_element[TNM_EXE_ANGOU_ELEMENT03A];
				angou[4] = exe_angou_element[TNM_EXE_ANGOU_ELEMENT04A];
				angou[5] = exe_angou_element[TNM_EXE_ANGOU_ELEMENT05A];
				angou[6] = exe_angou_element[TNM_EXE_ANGOU_ELEMENT06A];
				angou[7] = exe_angou_element[TNM_EXE_ANGOU_ELEMENT07A];
				angou[8] = exe_angou_element[TNM_EXE_ANGOU_ELEMENT00B];
				angou[9] = exe_angou_element[TNM_EXE_ANGOU_ELEMENT01B];
				angou[10] = exe_angou_element[TNM_EXE_ANGOU_ELEMENT02B];
				angou[11] = exe_angou_element[TNM_EXE_ANGOU_ELEMENT03B];
				angou[12] = exe_angou_element[TNM_EXE_ANGOU_ELEMENT04B];
				angou[13] = exe_angou_element[TNM_EXE_ANGOU_ELEMENT05B];
				angou[14] = exe_angou_element[TNM_EXE_ANGOU_ELEMENT06B];
				angou[15] = exe_angou_element[TNM_EXE_ANGOU_ELEMENT07B];
				TSTR angou_test_str;
				for (int i = 0; i < 8; i++)	{
					angou_test_str += str_format(_T("#define\tKN_EXE_ANGOU_DATA%02dA\t0x%02X"), i, (int)angou[i]) + _T("\n");
				}
				angou_test_str += _T("\n");
				for (int i = 0; i < 8; i++)	{
					angou_test_str += str_format(_T("#define\tKN_EXE_ANGOU_DATA%02dB\t0x%02X"), i, (int)angou[i + 8]) + _T("\n");
				}
				if (!C_file::write_full_text_in_MB(tmp_dir_path + _T("\\EXE_ANGOU.h"), angou_test_str))	{
					set_log(_T("angou.h ���������߂܂���ł����B"));
					return false;
				}
#endif
			}
		}
	}
	// ���k�ƈÍ���
	BUFFER ged_data_noangou;
	BUFFER ged_data_angou;

	// �w�b�_�������m��
	ged_data_noangou.resize(sizeof(S_tnm_gameexe_dat_header));
	if (exe_angou_mode)	{
		ged_data_angou.resize(sizeof(S_tnm_gameexe_dat_header));
	}

	// �e�L�X�g����
	if (!gei_text.empty())
	{
		// ���k
		BUFFER lzss_ged;
		lzss_pack_easy((BYTE *)ged_text.c_str(), (DWORD)ged_text.size() * sizeof(TCHAR), lzss_ged); 

		// �ȈՈÍ���
		BYTE *wp = lzss_ged.get();
		int eac = 0;
		for (int i = 0; i < (int)lzss_ged.size(); i++)	{
			*wp++ ^= tns_gameexe_dat_angou_code[eac++];
			eac %= TNM_GAMEEXE_DAT_ANGOU_CODE_SIZE;
		}

		// �����i�ȈՈÍ��̂݁j
		ged_data_noangou.insert(ged_data_noangou.end(), lzss_ged.begin(), lzss_ged.end());

		// �d�w�d�Z�b�g�Í���
		if (exe_angou_mode)	{
			BYTE *wp = lzss_ged.get();
			int eac = 0;
			for (int i = 0; i < (int)lzss_ged.size(); i++)	{
				*wp++ ^= exe_angou_element[eac++];
				eac %= TNM_EXE_ANGOU_ELEMENT_CNT;
			}

			// �����i�ȈՈÍ����d�w�d�Z�b�g�Í��j
			ged_data_angou.insert(ged_data_angou.end(), lzss_ged.begin(), lzss_ged.end());
		}
	}

	// �o��
	if (exe_angou_mode)	{

		// �Í��Ȃ�
		{
			// �w�b�_�ݒ�
			S_tnm_gameexe_dat_header *header = (S_tnm_gameexe_dat_header *)ged_data_noangou.get();
			header->version = 0;
			header->exe_angou_mode = 0;

			// �o��
			if (!out_dir_path_noangou.empty())	{
				TSTR out_path = out_dir_path_noangou + _T("\\") + gameexe_dat;
				create_dir_from_file_path(out_path);
				if (!C_file::write_full_data(out_path, ged_data_noangou))	{
					set_log(_T("Gameexe.dat ���������߂܂���ł����B"));
					return false;
				}
			}
		}
		// �Í�����
		{
			// �w�b�_�ݒ�
			S_tnm_gameexe_dat_header *header = (S_tnm_gameexe_dat_header *)ged_data_angou.get();
			header->version = 0;
			header->exe_angou_mode = 1;

			// �o��
			TSTR out_path = out_dir_path + _T("\\") + gameexe_dat;
			create_dir_from_file_path(out_path);
			if (!C_file::write_full_data(out_path, ged_data_angou))	{
				set_log(_T("Gameexe.dat ���������߂܂���ł����B"));
				return false;
			}
		}
	}
	else	{

		// �w�b�_�ݒ�
		S_tnm_gameexe_dat_header *header = (S_tnm_gameexe_dat_header *)ged_data_noangou.get();
		header->version = 0;
		header->exe_angou_mode = 0;

		// �����ɈÍ��Ȃ����o��
		TSTR out_path = out_dir_path + _T("\\") + gameexe_dat;
		create_dir_from_file_path(out_path);
		if (!C_file::write_full_data(out_path, ged_data_noangou))	{
			set_log(_T("Gameexe.dat ���������߂܂���ł����B"));
			return false;
		}
		if (!out_dir_path_noangou.empty())	{
			TSTR out_path = out_dir_path_noangou + _T("\\") + gameexe_dat;
			create_dir_from_file_path(out_path);
			if (!C_file::write_full_data(out_path, ged_data_noangou))	{
				set_log(_T("Gameexe.dat ���������߂܂���ł����B"));
				return false;
			}
		}
	}

	return true;
}

