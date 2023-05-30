#pragma		once

// ****************************************************************
// コメントカット
// ================================================================
class C_ini_comment_cut
{
public:
	bool	comment_cut(ARRAY<TSTR>& txt_data);

	int		get_error_line_no()	{	return error_line_no;	}
	TSTR	get_error_str()		{	return error_str;		}

private:
	int		error_line_no;
	TSTR	error_str;

	bool	error(int line_no, CTSTR& str);
};
