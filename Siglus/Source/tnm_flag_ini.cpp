#include	"pch.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_element.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_flag_ini.h"
#include	"data/tnm_error.h"
#include	"engine/ifc_error.h"

// ****************************************************************
// Flag.ini �A�i���C�U
// ================================================================
bool tnm_flag_ini_str_analize_get_word(TSTR::const_iterator& itr_, TSTR::const_iterator end, TSTR& word)
{
	TSTR::const_iterator itr = itr_;

	for (; itr != end; ++itr)	{
		if (!is_alpha(*itr) && !is_num(*itr) && *itr != _T('_') && *itr != _T('$') && *itr != _T('@'))
			break;
	}

	word = TSTR(itr_, itr);
	itr_ = itr;
	return true;
}

class C_tnm_flag_ini_str_analizer : public C_str_analizer
{
public:
	C_tnm_flag_ini_str_analizer()		{}
	C_tnm_flag_ini_str_analizer(TSTR::const_iterator itr_begin, TSTR::const_iterator itr_end, CTSTR& separator_list = _T(" \t\n"))
	{
		set_string(itr_begin, itr_end);
		set_separator_list(separator_list);
	}

	bool	get_word(TSTR& word);
};

bool C_tnm_flag_ini_str_analizer::get_word(TSTR& word)
{
	C_state state = save_state();

	skip_separator();
	if (!tnm_flag_ini_str_analize_get_word(cur_itr, end_itr, word))
		return FAIL(state);

	// �����I
	return true;
}

// ****************************************************************
// Flag.ini
// ================================================================
C_tnm_flag_ini::C_tnm_flag_ini()
{
}

bool C_tnm_flag_ini::analize_flag_ini()
{
	// 20�y�[�W����Ă݂�
	page.resize(20);

	// �t�@�C����ǂݍ���
	TSTR file_path = Gp_dir->work_dir + _T("\\Flag.ini");
	if (file_get_type(file_path) != 1)
		return true;	// �t�@�C����������Ȃ��i����I���j

	// �t�@�C�����擾
	ARRAY<TSTR> file_data;
	if (!C_file::read_full_text(file_path, file_data))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("Flag.ini ���ǂݍ��߂܂���ł����B\n") + get_last_error_msg());
		return false;
	}

	for (int line_no = 0; line_no < (int)file_data.size(); line_no++)	{
		TSTR& line = file_data[line_no];
		TSTR::const_iterator p = line.begin();

		str_to_lower(line);

		// A[0] : 0 : ���߂�ƁI�I
		// $... : 0 : ���߂�ƁI�I

		C_tnm_flag_ini_str_analizer anl(line.begin(), line.end());
		while (1)	{

			if (anl.check_moji(_T('#')))	{
				int page_no;
				TSTR title;

				if (!anl.check_str(_T("title_flag")))
					break;
				if (!anl.get_num(&page_no))
					break;
				if (!anl.check_moji(_T('=')))
					break;

				title = TSTR(anl.get_cur(), anl.get_end());

				if (0 <= page_no && page_no < 20)	{
					page[page_no].title = title;
				}
			}
			else	{
				int flag = 0, index = 0, page_no = 0;
				bool int_flag = false;
				bool call_flag = false;
				bool user_flag = false;
				int bit_cnt = 32;
				TSTR name, comment;

				if (!anl.get_word(name))
					break;

				if (!name.empty() && name[0] == _T('$'))	{

					// ���[�U�t���O
					user_flag = true;

					// �r�b�g�t���O
					if (anl.check_moji(_T('.')))	{

						if (false);
						else if (anl.check_str(_T("bit16")))	bit_cnt = 16;
						else if (anl.check_str(_T("bit8")))		bit_cnt = 8;
						else if (anl.check_str(_T("bit4")))		bit_cnt = 4;
						else if (anl.check_str(_T("bit2")))		bit_cnt = 2;
						else if (anl.check_str(_T("bit")))		bit_cnt = 1;
						if (bit_cnt == 32)
							break;
					}

					// �C���f�b�N�X
					if (anl.check_moji(_T('[')))	{
						if (!anl.get_integer(&index))		break;
						if (!anl.check_moji(_T(']')))		break;
					}
					if (!anl.check_moji(_T(':')))		break;
					if (!anl.get_integer(&page_no))		break;
					if (!anl.check_moji(_T(':')))		break;
				}
				else	{

					// �ʏ�̃t���O
					if (false);
					else if (name == _T("a"))	{	flag = ELM_GLOBAL_A;	int_flag = true;						}
					else if (name == _T("b"))	{	flag = ELM_GLOBAL_B;	int_flag = true;						}
					else if (name == _T("c"))	{	flag = ELM_GLOBAL_C;	int_flag = true;						}
					else if (name == _T("d"))	{	flag = ELM_GLOBAL_D;	int_flag = true;						}
					else if (name == _T("e"))	{	flag = ELM_GLOBAL_E;	int_flag = true;						}
					else if (name == _T("f"))	{	flag = ELM_GLOBAL_F;	int_flag = true;						}
					else if (name == _T("g"))	{	flag = ELM_GLOBAL_G;	int_flag = true;						}
					else if (name == _T("z"))	{	flag = ELM_GLOBAL_Z;	int_flag = true;						}
					else if (name == _T("l"))	{	flag = ELM_CALL_L;		int_flag = true;	call_flag = true;	}
					else if (name == _T("s"))	{	flag = ELM_GLOBAL_S;											}
					else if (name == _T("m"))	{	flag = ELM_GLOBAL_M;											}
					else if (name == _T("k"))	{	flag = ELM_CALL_K;							call_flag = true;	}
					else	{
						break;
					}

					// �r�b�g�t���O
					if (int_flag && anl.check_moji(_T('.')))	{

						if (false);
						else if (anl.check_str(_T("bit16")))	bit_cnt = 16;
						else if (anl.check_str(_T("bit8")))		bit_cnt = 8;
						else if (anl.check_str(_T("bit4")))		bit_cnt = 4;
						else if (anl.check_str(_T("bit2")))		bit_cnt = 2;
						else if (anl.check_str(_T("bit")))		bit_cnt = 1;
						if (bit_cnt == 32)
							break;
					}

					if (!anl.check_moji(_T('[')))		break;
					if (!anl.get_integer(&index))		break;
					if (!anl.check_moji(_T(']')))		break;
					if (!anl.check_moji(_T(':')))		break;
					if (!anl.get_integer(&page_no))		break;
					if (!anl.check_moji(_T(':')))		break;
				}
				
				comment = TSTR(anl.get_cur(), anl.get_end());

				if (0 <= page_no && page_no < 20)	{
					S_flag_ini_flag_data flag_data;

					if (false);
					else if (user_flag)	{
						flag_data.name = name;
						flag_data.index = index;
						flag_data.bit_cnt = bit_cnt;
					}
					else if (call_flag)	{
						flag_data.element = S_element(ELM_GLOBAL_CUR_CALL, flag, ELM_ARRAY, index);
						flag_data.bit_cnt = bit_cnt;
					}
					else	{
						flag_data.element = S_element(flag, ELM_ARRAY, index);
						flag_data.bit_cnt = bit_cnt;
					}
					flag_data.comment = comment;
					page[page_no].flag.push_back(flag_data);
				}
			}
		}
	}

	return true;
}









