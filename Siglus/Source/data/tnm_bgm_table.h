#pragma		once

#include	"element/elm_list.h"

// ****************************************************************
// ‚a‚f‚lƒe[ƒuƒ‹
// ================================================================
class C_tnm_bgm_table
{
public:
	void	init();
	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);

	void	set_all_flag(bool value);
	void	set_listened(int bgm_no, bool value);
	bool	get_listened(int bgm_no);

	int		get_bgm_cnt()	{	return flag_list.get_size();	}

private:
	C_elm_int_list		flag_list;
};

