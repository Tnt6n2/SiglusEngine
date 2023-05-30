#include	"pch.h"
#include	"data.h"
#include	"dialog.h"
#include	"main.h"
#include	"element.h"
#include	"tnm_common_header.h"
#include	"IA.h"
#include	"CA.h"
#include	"LA.h"
#include	"SA.h"
#include	"MA.h"
#include	"BS.h"
#include	"lzss.h"
#include	"tnm_exe_angou.h"

BYTE tns_easy_angou_code[TNM_EASY_ANGOU_CODE_SIZE] = TNM_EASY_ANGOU_CODE;

// ****************************************************************
// �R���p�C���F�X���b�h�f�[�^
// ================================================================
struct S_thread_data
{
	BUFFER	scn_data;
	BUFFER	lzss_data;
};

// ****************************************************************
// �����F�R���p�C���{��
// ================================================================
/*
struct S_thread_compile_info
{
	COMPILE_FUNK_STRUCT			cfst;
	C_tnms_scn_compile_data*	p_scd;

};
*/


/*
DWORD WINAPI tnm_compile_thread_func(LPVOID pv_ref)
{
	S_thread_compile_info* p_info = (S_thread_compile_info *)pv_ref;

	// �L�����Z��
	//if (proc_cancel_flag)
	//	break;

	// �^�[�Q�b�g�̃V�[���f�[�^��ݒ肷��
	C_tnms_scn_compile_data* scn_data = p_info->p_scd;
	p_info->cfst.scn_file_name = scn_data->scn_name;
	p_info->cfst.scn_file_path = cfst.scn_dir_path + _T("\\") + p_info->cfst.scn_file_name + _T(".ss");
	p_info->cfst.ca_file_path = cfst.tmp_dir_path + _T("\\ca\\") + p_info->cfst.scn_file_name + _T(".txt");

	// ���X�g�r���[�̍X�V�i�^�[�Q�b�g�̃t�@�C����\������j
//	G_dlg->ci.lv_scn.set_item_visible(scn_data->lv_item_no);

	// �t�@�C���f�[�^�̎擾
	TSTR scn_text;
	if (!C_file::read_full_text(p_info->cfst.scn_file_path, scn_text))	{
		set_log(cfst.scn_file_path + _T(" ���J���܂���ł����B"));
		error_cnt++;
		continue;
	}

	// �R���p�C���J�n
	//set_log_jump(cfst.scn_file_name + _T(".ss"), cfst.scn_file_path, cfst.scn_file_path, 1);

	// ���ԃt�@�C�����폜
	filesys_delete_file(p_info->cfst.tmp_dir_path + _T("\\bs\\") + p_info->cfst.scn_file_name + _T(".dat"));
	filesys_delete_file(p_info->cfst.tmp_dir_path + _T("\\bs\\") + p_info->cfst.scn_file_name + _T(".txt"));
	filesys_delete_file(p_info->cfst.tmp_dir_path + _T("\\bs\\") + p_info->cfst.scn_file_name + _T(".lzss"));

	// os �t�H���_���͏����Ă͂����܂���B�R���p�C���Ɏ��s�����Ƃ��A�O�̃f�[�^���g���K�v������܂��B
	//filesys_delete_file(cfst.tmp_dir_path + _T("\\os\\") + cfst.scn_file_name + _T(".ss"));

	// inc �f�[�^���R�s�[
	//scn_data->ia_data = *p_ia_data;	����ɂ���Ƃ��āI

	// �����Ԍv��
	//cur_time = ::GetTickCount();
	//before_time += cur_time - last_time;
	//last_time = cur_time;

	// �������
	if (!proc_compile_funk_ca_analize(&cfst, scn_data, scn_text))	{	error_cnt++;	continue;	}

	// �����Ԍv��
	//cur_time = ::GetTickCount();
	//ca_time += cur_time - last_time;
	//last_time = cur_time;

	// ������
	if (!proc_compile_funk_la_analize(&cfst, scn_data))	{	error_cnt++;	continue;	}

	// �����Ԍv��
	//cur_time = ::GetTickCount();
	//la_time += cur_time - last_time;
	//last_time = cur_time;

	// �\�����
	if (!proc_compile_funk_sa_analize(&cfst, scn_data))	{	error_cnt++;	continue;	}

	// �����Ԍv��
	//cur_time = ::GetTickCount();
	//sa_time += cur_time - last_time;
	//last_time = cur_time;

	// �Ӗ����
	if (!proc_compile_funk_ma_analize(&cfst, scn_data))	{	error_cnt++;	continue;	}

	// �����Ԍv��
	//cur_time = ::GetTickCount();
	//ma_time += cur_time - last_time;
	//last_time = cur_time;

	// �o�C�i���Z�[�u
	C_tnms_binary_saver bs;
	S_tnms_bs_data bs_data;
	if (!bs.compile(&scn_data->ia_data, &scn_data->la_data, &scn_data->sa_data, &bs_data, cfst.test_check))	{
		C_tnms_lexcial_analizer la;
		TSTR err_str;
		err_str += cfst.scn_file_name + _T(" line") + tostr(bs.get_error_line()) + _T(": ");
		err_str += _T("\"") + la.get_atom_text(bs.get_error_atom(), &scn_data->la_data) + _T("\"   ");
		err_str += _T("��") + bs.get_error_str();
		set_log_jump(err_str, cfst.ca_file_path, cfst.scn_file_path, bs.get_error_line());
		error_cnt++;
		continue;
	}

	// �R���p�C�����ʏo��
	TSTR bs_file_path = cfst.tmp_dir_path + _T("\\bs\\") + cfst.scn_file_name + _T(".dat");
	create_dir_from_file_path(bs_file_path);
	C_file::write_full_data(bs_file_path, bs_data.out_scn);

	// �����Ԍv��
	//cur_time = ::GetTickCount();
	//bs_time += cur_time - last_time;
	//last_time = cur_time;

	// ���k�E�ȈՈÍ��i�ȈՃ����N�̏ꍇ�͎��s���Ȃ��j
	BUFFER lzss_data;
	int lzss_size = (int)bs_data.out_scn.size();
	if (!G_dlg->ci.chk_easy_link.get_check())	{
		if ((int)lzss_data.size() < lzss_size)	{
			lzss_data.resize(lzss_size);
		}
		Clzss_pack lzss;
		lzss.ready(bs_data.out_scn.get(), bs_data.out_scn.size(), 0);
		while (1)	{
			// ���k�J�n�i�o�͐�o�b�t�@�̃|�C���^�ƃT�C�Y���w��j
			if (lzss.proc(lzss_data.get(), (DWORD)lzss_data.size()))	{
				lzss_size = lzss.pack_size;
				break;	// ���k����
			}
			// �o�͐�o�b�t�@�̍Ċm��
			if (lzss.dst_buf_need_size > 0)	{
				lzss_size += bs_data.out_scn.size();
				if ((int)lzss_data.size() < lzss_size)	{
					lzss_data.resize(lzss_size);
				}
			}
		}
		BYTE *wp = lzss_data.get();
		int eac = 0;
		for (int i = 0; i < lzss_size; i++)	{
			*wp++ ^= tns_easy_angou_code[eac++];
			eac %= TNM_EASY_ANGOU_CODE_SIZE;
		}
	}

	// �����Ԍv��
	//cur_time = ::GetTickCount();
	//pack_time += cur_time - last_time;
	//last_time = cur_time;

	// �R���p�C�����ʏo�́i�o�C�i���j
	//if (!G_dlg->ci.chk_easy_link.get_check())	{
		TSTR lzss_file_path = cfst.tmp_dir_path + _T("\\bs\\") + cfst.scn_file_name + _T(".lzss");
		create_dir_from_file_path(lzss_file_path);
		C_file::write_full_data(lzss_file_path, lzss_data.get(), lzss_size);
	//}

	// �R���p�C�����ʏo�́i�e�L�X�g�j
	//if (cfst.test_check)	{
	//	TSTR bs_txt_file_path = cfst.tmp_dir_path + _T("\\bs\\") + cfst.scn_file_name + _T(".txt");
	//	create_dir_from_file_path(bs_txt_file_path);
	//	C_file::write_full_text_in_MB(bs_txt_file_path, bs_data.out_txt);
	//}

	// ���X�g�r���[�̍X�V�i�R���p�C�������������̂őI����Ԃ���������j
	//G_dlg->ci.lv_scn.set_item_select(scn_data->lv_item_no, false);

	// �����Ԍv��
	//cur_time = ::GetTickCount();
	//save_time += cur_time - last_time;
	//last_time = cur_time;

	// ������Ɖ�����Ă݂悤�c
	scn_data->ia_data.replace_tree.clear();
	scn_data->la_data.atom_list.clear();
	scn_data->la_data.label_list.clear();
	scn_data->la_data.str_list.clear();
	scn_data->la_data.unknown_list.clear();
	scn_data->sa_data.root.reset();

	// ����
	return TRUE;
}
*/

bool C_tnm_compiler_main::proc_compile(S_tnms_ia_data* p_ia_data, int* p_error_cnt)
{
	ARRAY<C_tnms_scn_compile_data>	scn_data_list;

	// �v���W�F�N�g���I������Ă��Ȃ���Ύ��s
	if (!check_current_project(true))
		return false;

	// ���肦�Ȃ��o�O�ł����A�r�r���X�g�Ƃr�r���X�g�r���[�̃A�C�e��������v���邩�`�F�b�N����
	if ((int)G_data->scn_list.size() != G_dlg->ci.lv_scn.get_item_cnt())	{
		error_box(_T("�V�X�e���o�O�F�r�r���X�g�Ƃr�r���X�g�r���[�̃A�C�e��������v���܂���B"));
		return false;
	}

	// �f�[�^�̃N���A
	scn_data_list.clear();

	// �R���p�C�������p�\����
	COMPILE_FUNK_STRUCT cfst;
	cfst.scn_dir_path = G_data->project[G_data->cur_project_no].scn_path;		// �_�C�A���O�̃f�[�^���擾
	cfst.tmp_dir_path = G_data->project[G_data->cur_project_no].tmp_path;		// �_�C�A���O�̃f�[�^���擾
	cfst.utf8 = G_data->project[G_data->cur_project_no].utf8;
	cfst.test_check = G_dlg->ci.chk_test.get_check();

	// �R���p�C���J�n
	set_log(_T("- �R���p�C�����J�n���܂��B"));

	// �I������Ă���V�[���̃R���p�C���p�f�[�^���쐬
	for (int i = 0; i < (int)G_data->scn_list.size(); ++i)	{
		if (G_dlg->ci.lv_scn.get_item_select(i))	{
			C_tnms_scn_compile_data scn_data;
			scn_data.lv_item_no = i;
			scn_data.scn_name = erase_extension_ret(G_data->scn_list[i].scn_name);
			scn_data_list.push_back(scn_data);
		}
	}

	int ca_time = 0;
	int la_time = 0;
	int sa_time = 0;
	int ma_time = 0;
	int bs_time = 0;
	int pack_time = 0;
	int save_time = 0;
	int before_time = 0;

	// �R���p�C�������t���O
	int error_cnt = 0;

	int last_time = ::GetTickCount();
	int cur_time = 0;

	// �S�V�[���t�@�C���ɂ��č\����͂܂ł��s��
	for (int scn_no = 0; scn_no < (int)scn_data_list.size(); scn_no++)	{

		// �L�����Z��
		if (proc_cancel_flag)
			break;

		// �^�[�Q�b�g�̃V�[���f�[�^��ݒ肷��
		C_tnms_scn_compile_data* scn_data = &scn_data_list[scn_no];
		cfst.scn_file_name = scn_data->scn_name;
		cfst.scn_file_path = cfst.scn_dir_path + _T("\\") + cfst.scn_file_name + _T(".ss");
		cfst.ca_file_path = cfst.tmp_dir_path + _T("\\ca\\") + cfst.scn_file_name + _T(".txt");

		// ���X�g�r���[�̍X�V�i�^�[�Q�b�g�̃t�@�C����\������j
		G_dlg->ci.lv_scn.set_item_visible(scn_data->lv_item_no);

		// �t�@�C���f�[�^�̎擾
		TSTR scn_text;
		if (!(cfst.utf8 ? C_file::read_full_text_UTF8(cfst.scn_file_path, scn_text) : C_file::read_full_text(cfst.scn_file_path, scn_text)))	{
			set_log(cfst.scn_file_path + _T(" ���J���܂���ł����B"));
			error_cnt++;
			continue;
		}

		// �R���p�C���J�n
		set_log_jump(cfst.scn_file_name + _T(".ss"), _T(""), cfst.scn_file_path, 1);

		// ���ԃt�@�C�����폜
		filesys_delete_file(cfst.tmp_dir_path + _T("\\bs\\") + cfst.scn_file_name + _T(".dat"));
		filesys_delete_file(cfst.tmp_dir_path + _T("\\bs\\") + cfst.scn_file_name + _T(".txt"));
		filesys_delete_file(cfst.tmp_dir_path + _T("\\bs\\") + cfst.scn_file_name + _T(".lzss"));

		// os �t�H���_���͏����Ă͂����܂���B�R���p�C���Ɏ��s�����Ƃ��A�O�̃f�[�^���g���K�v������܂��B
		//filesys_delete_file(cfst.tmp_dir_path + _T("\\os\\") + cfst.scn_file_name + _T(".ss"));

		// inc �f�[�^���R�s�[
		scn_data->ia_data = *p_ia_data;

		// �����Ԍv��
		cur_time = ::GetTickCount();
		before_time += cur_time - last_time;
		last_time = cur_time;

		// �������
		if (!proc_compile_funk_ca_analize(&cfst, scn_data, scn_text))	{	error_cnt++;	continue;	}

		// �����Ԍv��
		cur_time = ::GetTickCount();
		ca_time += cur_time - last_time;
		last_time = cur_time;

		// ������
		if (!proc_compile_funk_la_analize(&cfst, scn_data))	{	error_cnt++;	continue;	}

		// �����Ԍv��
		cur_time = ::GetTickCount();
		la_time += cur_time - last_time;
		last_time = cur_time;

		// �\�����
		if (!proc_compile_funk_sa_analize(&cfst, scn_data))	{	error_cnt++;	continue;	}

		// �����Ԍv��
		cur_time = ::GetTickCount();
		sa_time += cur_time - last_time;
		last_time = cur_time;

		// �Ӗ����
		if (!proc_compile_funk_ma_analize(&cfst, scn_data))	{	error_cnt++;	continue;	}

		// �����Ԍv��
		cur_time = ::GetTickCount();
		ma_time += cur_time - last_time;
		last_time = cur_time;

		// �o�C�i���Z�[�u
		C_tnms_binary_saver bs;
		S_tnms_bs_data bs_data;
		if (!bs.compile(&scn_data->ia_data, &scn_data->la_data, &scn_data->sa_data, &bs_data, cfst.test_check))	{
			C_tnms_lexcial_analizer la;
			TSTR err_str;
			err_str += cfst.scn_file_name + _T(" line") + tostr(bs.get_error_line()) + _T(": ");
			err_str += _T("\"") + la.get_atom_text(bs.get_error_atom(), &scn_data->la_data) + _T("\"   ");
			err_str += _T("��") + bs.get_error_str();
			set_log_jump(err_str, cfst.ca_file_path, cfst.scn_file_path, bs.get_error_line());
			error_cnt++;
			continue;
		}

		// �R���p�C�����ʏo��
		TSTR bs_file_path = cfst.tmp_dir_path + _T("\\bs\\") + cfst.scn_file_name + _T(".dat");
		create_dir_from_file_path(bs_file_path);
		C_file::write_full_data(bs_file_path, bs_data.out_scn);

		// �����Ԍv��
		cur_time = ::GetTickCount();
		bs_time += cur_time - last_time;
		last_time = cur_time;

		// ���k�E�ȈՈÍ��i�ȈՃ����N�̏ꍇ�͎��s���Ȃ��j
		static BUFFER lzss_data;
		int lzss_size = (int)bs_data.out_scn.size();
		if (!G_dlg->ci.chk_easy_link.get_check())	{
			if ((int)lzss_data.size() < lzss_size)	{
				lzss_data.resize(lzss_size);
			}
			Clzss_pack lzss;
			lzss.ready(bs_data.out_scn.get(), bs_data.out_scn.size(), 0);
			while (1)	{
				// ���k�J�n�i�o�͐�o�b�t�@�̃|�C���^�ƃT�C�Y���w��j
				if (lzss.proc(lzss_data.get(), (DWORD)lzss_data.size()))	{
					lzss_size = lzss.pack_size;
					break;	// ���k����
				}
				// �o�͐�o�b�t�@�̍Ċm��
				if (lzss.dst_buf_need_size > 0)	{
					lzss_size += bs_data.out_scn.size();
					if ((int)lzss_data.size() < lzss_size)	{
						lzss_data.resize(lzss_size);
					}
				}
			}
			BYTE *wp = lzss_data.get();
			int eac = 0;
			for (int i = 0; i < lzss_size; i++)	{
				*wp++ ^= tns_easy_angou_code[eac++];
				eac %= TNM_EASY_ANGOU_CODE_SIZE;
			}
		}

		// �����Ԍv��
		cur_time = ::GetTickCount();
		pack_time += cur_time - last_time;
		last_time = cur_time;

		// �R���p�C�����ʏo�́i�o�C�i���j
		if (!G_dlg->ci.chk_easy_link.get_check())	{
			TSTR lzss_file_path = cfst.tmp_dir_path + _T("\\bs\\") + cfst.scn_file_name + _T(".lzss");
			create_dir_from_file_path(lzss_file_path);
			C_file::write_full_data(lzss_file_path, lzss_data.get(), lzss_size);
		}

		// �R���p�C�����ʏo�́i�e�L�X�g�j
		if (cfst.test_check)	{
			TSTR bs_txt_file_path = cfst.tmp_dir_path + _T("\\bs\\") + cfst.scn_file_name + _T(".txt");
			create_dir_from_file_path(bs_txt_file_path);
			cfst.utf8 ? C_file::write_full_text_in_UTF8(bs_txt_file_path, bs_data.out_txt) : C_file::write_full_text_in_MB(bs_txt_file_path, bs_data.out_txt);
		}

		// ���X�g�r���[�̍X�V�i�R���p�C�������������̂őI����Ԃ���������j
		G_dlg->ci.lv_scn.set_item_select(scn_data->lv_item_no, false);

		// �����Ԍv��
		cur_time = ::GetTickCount();
		save_time += cur_time - last_time;
		last_time = cur_time;

		// ������Ɖ�����Ă݂悤�c
#if 0
		scn_data->ia_data.init();
		scn_data->ca_data.property_list.clear();
		scn_data->la_data.atom_list.clear();
		scn_data->la_data.label_list.clear();
		scn_data->la_data.str_list.clear();
		scn_data->la_data.unknown_list.clear();
		scn_data->sa_data.root.reset();
		scn_data->sa_data.z_label_list.clear();
		scn_data->sa_data.call_prop_name_list.clear();
#else
		*scn_data = C_tnms_scn_compile_data();	// �f�B�[�v�ȊJ��

//		scn_data->la_data.atom_list.clear();
//		scn_data->la_data.label_list.clear();
//		scn_data->la_data.str_list.clear();
//		scn_data->la_data.unknown_list.clear();
//		scn_data->sa_data.root.reset();
#endif
	}

	set_log(_T("��������: ") + tostr(before_time));
	set_log(_T("������͎���: ") + tostr(ca_time));
	set_log(_T("�����͎���: ") + tostr(la_time));
	set_log(_T("�\����͎���: ") + tostr(sa_time));
	set_log(_T("�Ӗ���͎���: ") + tostr(ma_time));
	set_log(_T("�޲�؍쐬����: ") + tostr(bs_time));
	set_log(_T("���k����: ") + tostr(pack_time));
	set_log(_T("�ۑ�����: ") + tostr(save_time));

	if (p_error_cnt)
		*p_error_cnt = error_cnt;

	return error_cnt == 0;
}

// ****************************************************************
// �����F�R���p�C���F�������
// ================================================================
bool C_tnm_compiler_main::proc_compile_funk_ca_analize(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data, CTSTR scn_text)
{
	// �������
	C_tnms_character_analizer ca;
	if (!ca.analize_file(scn_text, &scn_data->ia_data, &scn_data->ca_data))	{
		TSTR err_str = cfst->scn_file_name + _T(" line") + tostr(ca.get_error_line()) + _T(": ") + ca.get_error_str();
		set_log_jump(err_str, cfst->scn_file_path, cfst->scn_file_path, ca.get_error_line());
		return false;
	}

	// ������͂̌��ʏo��
	// �X�e�[�^�X�o�[�̕\���Ɏg���̂ŁA�K���o�͂��܂��B

	//if (cfst->test_check)	{
		C_file ca_file;
		create_dir_from_file_path(cfst->ca_file_path);
		if (ca_file.open(cfst->ca_file_path, _T("wt")))
			cfst->utf8 ? ca_file.write_in_UTF8(scn_data->ca_data.scn_text) : ca_file.write_in_MB(scn_data->ca_data.scn_text);
	//}

	return true;
}

// ****************************************************************
// �����F�R���p�C���F������
// ================================================================
bool C_tnm_compiler_main::proc_compile_funk_la_analize(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data)
{
	C_tnms_lexcial_analizer la;

	// ������
	if (!la.analize(&scn_data->ca_data, &scn_data->la_data))	{
		TSTR err_str = cfst->scn_file_name + _T(" line") + tostr(la.get_error_line()) + _T(": ") + la.get_error_str();
		set_log_jump(err_str, cfst->ca_file_path, cfst->scn_file_path, la.get_error_line());
		return false;
	}

	// �����͂̌��ʏo��
	if (cfst->test_check)	{
		C_file la_file;
		TSTR la_file_path = cfst->tmp_dir_path + _T("\\la\\") + cfst->scn_file_name + _T(".txt");
		create_dir_from_file_path(la_file_path);
		if (la_file.open(la_file_path, _T("wt")))	{

			// �A�g�����X�g�̏o��
			la_file.write("\n-- atom list --\n");
			int atom_cnt = (int)scn_data->la_data.atom_list.size();
			for (int a = 0; a < atom_cnt; a++)	{
				TSTR str;
				str += str_format(_T("%04d, Line %04d: "), a, scn_data->la_data.atom_list[a].line);
				str += TNMS_ATOM_TYPE_STR[scn_data->la_data.atom_list[a].type] + _T(",");
				str += tostr(scn_data->la_data.atom_list[a].opt) + _T(",");
				str += tostr(scn_data->la_data.atom_list[a].subopt);
				str += _T("\n");
				cfst->utf8 ? la_file.write_in_UTF8(str) : la_file.write_in_MB(str);
			}

			// �����񃊃X�g�̏o��
			la_file.write("\n-- string list --\n");
			int str_cnt = (int)scn_data->la_data.str_list.size();
			for (int s = 0; s < str_cnt; s++)	{
				TSTR str;
				str += str_format(_T("%04d: "), s);
				str += scn_data->la_data.str_list[s];
				str += _T("\n");
				cfst->utf8 ? la_file.write_in_UTF8(str) : la_file.write_in_MB(str);
			}

			// ���x�����X�g�̏o��
			la_file.write("\n-- LabelList --\n");
			int label_cnt = (int)scn_data->la_data.label_list.size();
			for (int l = 0; l < label_cnt; l++)	{
				TSTR str;
				str += str_format(_T("%04d: "), l);
				str += scn_data->la_data.label_list[l].name;
				str += _T("\n");
				cfst->utf8 ? la_file.write_in_UTF8(str) : la_file.write_in_MB(str);
			}

			// ����`�ꃊ�X�g�̏o��
			la_file.write("\n-- UnknownList --\n");
			int unknown_cnt = (int)scn_data->la_data.unknown_list.size();
			for (int u = 0; u < unknown_cnt; u++)	{
				TSTR str;
				str += str_format(_T("%04d: "), u);
				str += scn_data->la_data.unknown_list[u];
				str += _T("\n");
				cfst->utf8 ? la_file.write_in_UTF8(str) : la_file.write_in_MB(str);
			}
		}
		la_file.close();
	}

	return true;
}

// ****************************************************************
// �����F�R���p�C���F�\�����
// ================================================================
bool C_tnm_compiler_main::proc_compile_funk_sa_analize(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data)
{
	C_tnms_lexcial_analizer la;
	C_tnms_sentence_analizer sa;

	// �\�����
	if (!sa.analize(&scn_data->ia_data, &scn_data->la_data, &scn_data->sa_data))	{
		TSTR err_str;
		err_str += cfst->scn_file_name + _T(" line") + tostr(sa.get_error_line()) + _T(": ");
		err_str += _T("\"") + la.get_atom_text(sa.get_error_atom(), &scn_data->la_data) + _T("\"   ");
		err_str += _T("��") + sa.get_error_str();
		set_log_jump(err_str, cfst->ca_file_path, cfst->scn_file_path, sa.get_error_line());
		return false;
	}

	return true;
}

// ****************************************************************
// �����F�R���p�C���F�Ӗ����
// ================================================================
bool C_tnm_compiler_main::proc_compile_funk_ma_analize(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data)
{
	C_tnms_lexcial_analizer la;
	C_tnms_semantic_analizer ma;

	if (!ma.analize(&scn_data->ia_data, &scn_data->la_data, &scn_data->sa_data))	{
		TSTR err_str;
		err_str += cfst->scn_file_name + _T(" line") + tostr(ma.get_error_line()) + _T(": ");
		err_str += _T("\"") + la.get_atom_text(ma.get_error_atom(), &scn_data->la_data) + _T("\"   ");
		err_str += _T("��") + ma.get_error_str();
		set_log_jump(err_str, cfst->ca_file_path, cfst->scn_file_path, ma.get_error_line());
		return false;
	}
	return true;
}

// ****************************************************************
// �����F�R���p�C���F���p������p�����[�^�̕ϊ��i���G���[�͂��肦�Ȃ��̂ŃG���[�����͂܂������l���Ă��܂���j
// ================================================================
bool C_tnm_compiler_main::proc_compile_funk_hankaku_param_format(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data)
{
	// ���̃R�s�[
	TSTR in_text = scn_data->ca_data.scn_text;

	// �ԕ��̑}��
	for (int i = 0; i < 256; i++)
		in_text.push_back(_T('\0'));

	enum STATE
	{
		STATE_NONE,
		STATE_SINGLE_QUOTE,			// �V���O���R�[�e�[�V����
		STATE_SINGLE_QUOTE_ESCAPE,	// �V���O���R�[�e�[�V�����G�X�P�[�v
		STATE_SINGLE_QUOTE_END,		// �V���O���R�[�e�[�V�����I��
		STATE_DOUBLE_QUOTE,			// �_�u���R�[�e�[�V����
		STATE_DOUBLE_QUOTE_ESCAPE,	// �_�u���R�[�e�[�V�����G�X�P�[�v
	};

	TSTR out_text;
	STATE state = STATE_NONE;			// ���݂̃X�e�[�g
	int kako_cnt = 0;
	bool push_back_ok;
	TSTR word;

	for (TSTR::iterator p = in_text.begin(); *p != _T('\0'); ++p)	{

		push_back_ok = true;	// �o�̓t���O���n�m
		TCHAR moji = *p;		// �o�͕���

		// �V���O���R�[�e�[�V������
		if (state == STATE_SINGLE_QUOTE)	{
			if (false);
			// �G�X�P�[�v
			else if (*p == _T('\\'))
				state = STATE_SINGLE_QUOTE_ESCAPE;
			else
				state = STATE_SINGLE_QUOTE_END;
		}
		// �V���O���R�[�e�[�V�����G�X�P�[�v��
		else if (state == STATE_SINGLE_QUOTE_ESCAPE)	{
			if (*p == _T('\\') || *p == _T('\''))
				state = STATE_SINGLE_QUOTE_END;
		}
		// �V���O���R�[�e�[�V�����I��
		else if (state == STATE_SINGLE_QUOTE_END)	{
			// �V���O���R�[�e�[�V�����I��
			if (*p == _T('\''))
				state = STATE_NONE;
		}
		// �_�u���R�[�e�[�V������
		else if (state == STATE_DOUBLE_QUOTE)	{
			// �G�X�P�[�v
			if (*p == _T('\\'))
				state = STATE_DOUBLE_QUOTE_ESCAPE;
			// �_�u���R�[�e�[�V�����I��
			if (*p == _T('"'))
				state = STATE_NONE;
		}
		// �_�u���R�[�e�[�V�����G�X�P�[�v��
		else if (state == STATE_DOUBLE_QUOTE_ESCAPE)	{
			if (*p == _T('\\') || *p == _T('"'))
				state = STATE_DOUBLE_QUOTE;
		}
		// �ʏ폈��
		else if (state == STATE_NONE)	{

			if (false);
			// �V���O���R�[�e�[�V�����J�n
			else if (*p == _T('\''))
				state = STATE_SINGLE_QUOTE;
			// �_�u���R�[�e�[�V�����J�n
			else if (*p == _T('"'))
				state = STATE_DOUBLE_QUOTE;
			// �J�����ʔ���
			else if (*p == _T('('))
				kako_cnt++;
			// �����ʔ���
			else if (*p == _T(')'))
				kako_cnt--;
			// �t�@�C�����̓������Ƃ��Ďg�p�\�ȕ���
			else if (kako_cnt > 0 && (*p == _T('_') || (_T('a') <= *p && *p <= _T('z'))))	{
				while (1)	{
					if (*p == _T('_') || (_T('a') <= *p && *p <= _T('z')) || (_T('0') <= *p && *p <= _T('9')))	{
						word.push_back(*p);
						p++;
					}
					else	{	break;	}
				}
				if (!scn_data->ia_data.form_table.check_element_info(word))	{
					out_text += _T('\"') + word + _T('\"');
				}
				else	{
					out_text += word;
				}
				word.clear();
				p--;					// �i�߂������|�C���^���P�߂�
				push_back_ok = false;	// �o�̓t���O���n�e�e
			}
		}

		// �o��
		if (push_back_ok)	{	out_text.push_back(moji);	}
	}

	// �e�L�X�g�̓���ւ�
	scn_data->ca_data.scn_text = out_text;

	// ���p������p�����[�^�̕ϊ��̌��ʏo��
	TSTR ca2_file_path = cfst->tmp_dir_path + _T("\\ca2\\") + cfst->scn_file_name + _T(".txt");
	C_file ca_file;
	create_dir_from_file_path(ca2_file_path);
	if (ca_file.open(ca2_file_path, _T("wt")))
		cfst->utf8 ? ca_file.write_in_UTF8(scn_data->ca_data.scn_text) : ca_file.write_in_MB(scn_data->ca_data.scn_text);

	return true;
}

