#pragma		once

class		C_tnm_dbg_listview;
class		C_tnm_element;
class		C_elm_int_list;
class		C_elm_int_event;
class		C_elm_int_event_list;
class		C_elm_str_list;
class		C_elm_counter;
class		C_elm_counter_list;
class		C_elm_mask;
class		C_elm_mask_list;
class		C_elm_world;
class		C_elm_world_list;
class		C_elm_stage;
class		C_elm_object;
class		C_elm_object_list;
class		C_elm_group;
class		C_elm_mwnd;
class		C_elm_mwnd_list;
class		C_elm_screen;
class		C_elm_effect;
class		C_elm_effect_list;
class		C_elm_quake;
class		C_elm_quake_list;
class		C_elm_bgm;
class		C_elm_koe;
class		C_elm_pcm;
class		C_elm_pcmch;
class		C_elm_se;
class		C_elm_mov;
class		C_elm_pcm_event;
class		C_elm_frame_action;
class		C_elm_call;
class		C_elm_excall;

template <typename TYPE>	class C_elm_list_base;
template <typename TYPE>	class C_elm_list;

// ****************************************************************
// 汎用関数
// ================================================================
bool		tnm_form_is_list(int form);

// ****************************************************************
// デバッグ情報ウィンドウ：リストビューを初期化：汎用関数
// ================================================================
void		tnm_init_lv_element(int element_form, bool table_flag, C_tnm_dbg_listview* p_lv);

// ****************************************************************
// デバッグ情報ウィンドウ：リストビューを更新：汎用関数
// ================================================================
void		tnm_update_lv_int(const int* p_int, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_int_event(const C_elm_int_event* p_int_event, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_str(const TSTR* p_str, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_counter(const C_elm_counter* p_counter, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_mask(const C_elm_mask* p_mask, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_world(const C_elm_world* p_world, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_stage(C_elm_stage* p_stage, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_object(C_elm_object* p_object, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_group(C_elm_group* p_group, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_mwnd(C_elm_mwnd* p_mwnd, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_effect(const C_elm_effect* p_effect, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_quake(const C_elm_quake* p_quake, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_bgm(C_elm_bgm* p_bgm, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_koe(C_elm_koe* p_koe, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_pcm(C_elm_pcm* p_pcm, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_pcmch(C_elm_pcmch* p_pcmch, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_se(C_elm_se* p_se, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_mov(C_elm_mov* p_mov, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_pcm_event(C_elm_pcm_event* p_pcm_event, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_call(C_elm_call* p_call, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_excall(C_elm_excall* p_excall, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_element(C_tnm_element* p_element, int element_form, int bit, int top_index, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);
void		tnm_update_lv_element_table(C_tnm_element* p_element, int element_form, int bit, int top_index, C_tnm_dbg_listview* p_lv, int* p_item_no);
void		tnm_update_lv_simple_element(C_tnm_element* p_element, int element_form, C_tnm_dbg_listview* p_lv, int* p_item_no, int* p_column_no, int* p_inc_param);

// 汎用リスト
template <typename TYPE> void tnm_update_lv_list_table(C_elm_list_base<TYPE>* p_list, C_tnm_dbg_listview* p_lv, int* p_item_no, bool reduce);
// 整数リストを特殊化
void tnm_update_lv_list_table(C_elm_list_base<int>* p_int_list, int bit, int top_index, C_tnm_dbg_listview* p_lv, int* p_item_no, bool reduce);
// 整数イベントリストを特殊化
void tnm_update_lv_list_table(C_elm_list_base<C_elm_int_event>* p_int_list, C_tnm_dbg_listview* p_lv, int* p_item_no, bool reduce);
// 文字列リストを特殊化
void tnm_update_lv_list_table(C_elm_list_base<TSTR>* p_str_list, C_tnm_dbg_listview* p_lv, int* p_item_no, bool reduce);
// オブジェクトリストを特殊化
void tnm_update_lv_list_table(C_elm_object_list* p_object_list, C_tnm_dbg_listview* p_lv, int* p_item_no, bool reduce);

// ****************************************************************
// デバッグ情報ウィンドウ：リストビューをダブルクリックした：汎用関数
// ================================================================
void		tnm_on_dblclk_lv_element(C_tnm_dbg_listview* p_lv, int item_no, int column_no, int element_form, int bit, bool table_flag);
void		tnm_on_dblclk_lv_int_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no, int bit);
void		tnm_on_dblclk_lv_int_event_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no);
void		tnm_on_dblclk_lv_str_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no);
void		tnm_on_dblclk_lv_counter(C_tnm_dbg_listview* p_lv, int item_no, int column_no);
void		tnm_on_dblclk_lv_counter_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no);
void		tnm_on_dblclk_lv_mask(C_tnm_dbg_listview* p_lv, int item_no, int column_no);
void		tnm_on_dblclk_lv_mask_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no);
void		tnm_on_dblclk_lv_object(C_tnm_dbg_listview* p_lv, int item_no, int column_no);
void		tnm_on_dblclk_lv_object_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no);
void		tnm_on_dblclk_lv_effect(C_tnm_dbg_listview* p_lv, int item_no, int column_no);
void		tnm_on_dblclk_lv_effect_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no);
void		tnm_on_dblclk_lv_world(C_tnm_dbg_listview* p_lv, int item_no, int column_no);
void		tnm_on_dblclk_lv_world_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no);

// ****************************************************************
// デバッグ情報ウィンドウ：リストビューの値を変更した：汎用関数
// ================================================================
void		tnm_on_change_lv_value_element(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_tnm_element* p_element, int element_form, int bit, int top_index, bool table_flag);
void		tnm_on_change_lv_value_int_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_int_list* p_int_list, int bit, int top_index);
void		tnm_on_change_lv_value_int_event_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_int_event_list* p_int_list);
void		tnm_on_change_lv_value_str_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_str_list* p_str_list);
void		tnm_on_change_lv_value_counter(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_counter* p_counter);
void		tnm_on_change_lv_value_counter_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_counter_list* p_counter_list);
void		tnm_on_change_lv_value_mask(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_mask* p_mask);
void		tnm_on_change_lv_value_mask_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_mask_list* p_mask_list);
void		tnm_on_change_lv_value_object(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_object* p_object);
void		tnm_on_change_lv_value_object_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_object_list* p_object_list);
void		tnm_on_change_lv_value_effect(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_effect* p_effect);
void		tnm_on_change_lv_value_effect_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_effect_list* p_effect_list);
void		tnm_on_change_lv_value_world(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_world* p_world);
void		tnm_on_change_lv_value_world_list_table(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, C_elm_world_list* p_world_list);

// リストの要素数を取得
int			tnm_get_list_size(C_tnm_element* p_element, int form, int bit);
// アイテム数を削減
void		tnm_reduce_item_cnt(C_tnm_dbg_listview* p_lv, int item_no);

