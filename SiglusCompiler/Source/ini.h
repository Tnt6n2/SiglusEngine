

// ****************************************************************
// ini analizer
// ================================================================
class C_ini_file_analizer
{
public:
	bool	analize(TSTR src, TSTR& dst);	// ini ����͂���
	TSTR	get_error_str();				// �G���[��������擾
	int		get_error_line();				// �G���[�s�ԍ����擾

private:
	bool	error(int line, CTSTR& str);
	int		error_line;
	TSTR	error_str;

	bool	inia_comment_cut(TSTR& str);
	bool	inia_empty_cut(TSTR& str);
};


