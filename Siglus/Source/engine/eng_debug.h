#pragma		once

#include	<tnm_debugger.h>

// ****************************************************************
// crypt
// ================================================================
bool my_crypt(const char* p_password, const char* p_key, int src_size, BYTE* p_dst, int dst_size);

// ****************************************************************
// ƒfƒoƒbƒO‚c‚k‚k
// ================================================================
class C_tnm_debug_dll
{
public:
	C_tnm_debug_dll();

	HINSTANCE						h_dll;
	T_get_major_version				get_major_version;
	T_get_minor_version				get_minor_version;

	T_lb_add_item_001				lb_add_item_001;
	T_lb_insert_item_001			lb_insert_item_001;

	T_lv_add_item_001				lv_add_item_001;
	T_lv_add_item_002				lv_add_item_002;
	T_lv_add_item_visible_001		lv_add_item_visible_001;
	T_lv_insert_item_001			lv_insert_item_001;
	T_lv_insert_item_002			lv_insert_item_002;
	T_lv_insert_item_visible_001	lv_insert_item_visible_001;
	T_lv_reduce_item_001			lv_reduce_item_001;
	T_lv_set_item_cnt_001			lv_set_item_cnt_001;
	T_lv_set_cell_text_001			lv_set_cell_text_001;
	T_lv_set_cell_text_visible_001	lv_set_cell_text_visible_001;

	void	load();
	void	free();
};

