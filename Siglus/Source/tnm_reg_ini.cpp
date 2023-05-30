#include	"pch.h"

#include	"data/tnm_reg_ini.h"

// ****************************************************************
// reg.ini：情報を初期化
// ================================================================
C_tnm_reg_ini::C_tnm_reg_ini()
{
	// ================================================================
	// reg.ini のデフォルト値をここで指定します。
	// ----------------------------------------------------------------

	// システム
	reg_name = _T("");
}

// ****************************************************************
// reg.ini：解析
// ================================================================
bool C_tnm_reg_ini::analize()
{
	// 読み込み
	ARRAY<TSTR> file_data;
	if (!C_file::read_full_text(_T("reg.ini"), file_data))
		return true;	// ini が見つからなかった（失敗ではない）

	for (int line_no = 0; line_no < (int)file_data.size(); line_no ++)	{
		TSTR str[16];
		TSTR line_data = file_data[line_no];
		C_str_analizer anl(line_data.begin(), line_data.end());

		if (false);
		// システム
		else if (anl.is_end())	{
			// NOP
		}
		else if (anl.get_format(_T("//")))	{
			// コメントなので NOP
		}
		else if (anl.get_format(_T("#REGNAME = %s"), &str[0]))	{
			reg_name = str[0];
		}
		// 解析エラー
		else	{
			return false;
		}
	}

	if (reg_name.empty())
		return false;

	return true;
}
