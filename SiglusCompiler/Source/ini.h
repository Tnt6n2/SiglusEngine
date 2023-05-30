

// ****************************************************************
// ini analizer
// ================================================================
class C_ini_file_analizer
{
public:
	bool	analize(TSTR src, TSTR& dst);	// ini を解析する
	TSTR	get_error_str();				// エラー文字列を取得
	int		get_error_line();				// エラー行番号を取得

private:
	bool	error(int line, CTSTR& str);
	int		error_line;
	TSTR	error_str;

	bool	inia_comment_cut(TSTR& str);
	bool	inia_empty_cut(TSTR& str);
};


