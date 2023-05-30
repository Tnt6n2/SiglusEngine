#include	"pch.h"

#include	"data/tnm_reg_ini.h"

// ****************************************************************
// reg.ini�F����������
// ================================================================
C_tnm_reg_ini::C_tnm_reg_ini()
{
	// ================================================================
	// reg.ini �̃f�t�H���g�l�������Ŏw�肵�܂��B
	// ----------------------------------------------------------------

	// �V�X�e��
	reg_name = _T("");
}

// ****************************************************************
// reg.ini�F���
// ================================================================
bool C_tnm_reg_ini::analize()
{
	// �ǂݍ���
	ARRAY<TSTR> file_data;
	if (!C_file::read_full_text(_T("reg.ini"), file_data))
		return true;	// ini ��������Ȃ������i���s�ł͂Ȃ��j

	for (int line_no = 0; line_no < (int)file_data.size(); line_no ++)	{
		TSTR str[16];
		TSTR line_data = file_data[line_no];
		C_str_analizer anl(line_data.begin(), line_data.end());

		if (false);
		// �V�X�e��
		else if (anl.is_end())	{
			// NOP
		}
		else if (anl.get_format(_T("//")))	{
			// �R�����g�Ȃ̂� NOP
		}
		else if (anl.get_format(_T("#REGNAME = %s"), &str[0]))	{
			reg_name = str[0];
		}
		// ��̓G���[
		else	{
			return false;
		}
	}

	if (reg_name.empty())
		return false;

	return true;
}
