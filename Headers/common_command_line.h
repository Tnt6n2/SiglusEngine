#pragma		once

// ****************************************************************
// �R�}���h���C����͍\����
// ================================================================
//		/key = value value ...
// ================================================================
struct S_command_line_analizer_struct
{
	TSTR		key;
	ARRAY<TSTR>	value_list;
};

// ****************************************************************
// �R�}���h���C����̓N���X
// ================================================================
//		�R�}���h���C���i������j���g�[�N���ɕ������܂��B
//		value �ŃX�y�[�X���܂ޕ�����������ꍇ�́A"" �ł�����K�v������܂��B
//		value �� "" �ł�����ꂽ������̏ꍇ�A���i�~�L���j���G�X�P�[�v�����ɂȂ�܂��B
// ================================================================
class C_command_line_analizer
{
public:

	bool	analize_command_line(TSTR str, ARRAY<S_command_line_analizer_struct>& cmd_list);
	TSTR	get_error_str()	{	return erorr_str;	}

private:
	TSTR	erorr_str;
};
