#pragma		once

#include	"data/tnm_element.h"


// ****************************************************************
// Flag.ini
// ================================================================
struct S_flag_ini_flag_data
{
	S_element	element;
	TSTR		comment;
	TSTR		name;		// ユーザフラグ
	int			index;		// ユーザフラグのインデックス
	int			bit_cnt;	// ビットフラグのビット数
};
struct S_flag_ini_page_data
{
	TSTR						title;
	ARRAY<S_flag_ini_flag_data>	flag;
};

class C_tnm_flag_ini
{
public:
	C_tnm_flag_ini();

	bool	analize_flag_ini();

	ARRAY<S_flag_ini_page_data>	page;
};


