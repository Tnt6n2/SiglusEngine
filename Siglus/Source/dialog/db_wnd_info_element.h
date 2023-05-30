#pragma		once

#include	"data/tnm_element.h"
#include	"dialog/db_wnd_info.h"
#include	"dialog/db_wnd_info_listview.h"

class		C_elm_int_list;
class		C_elm_int_event;
class		C_elm_str_list;
class		C_elm_mask;
class		C_elm_counter;
class		C_elm_stage;
class		C_elm_object;
class		C_elm_group;
class		C_elm_mwnd;
class		C_elm_world;
class		C_elm_screen;
class		C_elm_effect;
class		C_elm_quake;
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
bool tnm_form_is_list(int form);

// ****************************************************************
// デバッグ情報ウィンドウ：エレメント：リストビュー
// ================================================================
class C_db_wnd_info_element_lv : public C_db_wnd_info_listview
{
public:
	S_element	m_element;
	int			m_element_form;
	TSTR		m_element_name;
	int			m_bit;
	int			m_top_index;
	bool		m_table_flag;		// テーブル表示

protected:
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// ウィンドウプロシージャ

private:
	void		init()			{}
	void		on_init_func()	{}
	void		on_close_func()	{}

	bool		on_notify(WPARAM wp, LPARAM lp);

	// リストビューをダブルクリックした
	void		on_dblclk_lv();

	// リストビューの値を変更した
	void		on_change_lv_value(int item_no, int column_no, CTSTR& value);
};

// ****************************************************************
// デバッグ情報ウィンドウ：エレメント：ツリーアイテム構造体
// ================================================================
struct S_treeview_item_element
{
	S_treeview_item_element()	{}
	S_treeview_item_element(S_element element_, int element_form_, int bit_, int top_index_, CTSTR& element_name_)
	{
		element = element_;
		element_form = element_form_;
		bit = bit_;
		top_index = top_index_;
		element_name = element_name_;
	}

	S_element	element;
	int			element_form;
	int			bit;
	int			top_index;
	TSTR		element_name;
};

// ****************************************************************
// デバッグ情報ウィンドウ：エレメント
// ================================================================
class C_db_wnd_info_element : public C_db_wnd_info
{
public:
	void		init();					// 初期化
	void		frame();				// フレーム処理

	Cc_treeview			tv_element;
	C_tnm_dbg_listview	lv_element;
	C_tnm_dbg_listview	lv_list;
	C_tnm_dbg_listview	lv_int_list;
	C_tnm_dbg_listview	lv_str_list;
	C_tnm_dbg_listview	lv_counter_list;
	C_tnm_dbg_listview	lv_mask_list;
	C_tnm_dbg_listview	lv_object_list;
	C_tnm_dbg_listview	lv_group_list;
	C_tnm_dbg_listview	lv_mwnd_list;
	C_tnm_dbg_listview	lv_effect_list;
	C_tnm_dbg_listview	lv_quake_list;
	C_tnm_dbg_listview	lv_world_list;
	C_tnm_dbg_listview	lv_pcmch_list;
	C_tnm_dbg_listview	lv_pcm_event_list;
	C_tnm_dbg_listview	lv_frame_action_list;
	C_tnm_dbg_listview	lv_call_list;
	Cc_button			btn_open_element;
	Cc_button			btn_open_list;
	C_dialog_separater	separater_1;
	C_dialog_separater	separater_2;

	bool	check_mouse_over(C_point mouse_pos);

private:

	// 作業変数
	std::map<HTREEITEM, S_treeview_item_element>	m_item_map;
	ARRAY<C_tnm_dbg_listview *>						m_lv_list;
	ARRAY< BSP<C_db_wnd_info_element_lv> >			m_opened_lv_list;

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// ウィンドウプロシージャ
	bool		on_init();										// 初期化
	bool		on_close();										// 閉じる
	bool		on_command(WPARAM wp, LPARAM lp);				// コマンド
	bool		on_notify(WPARAM wp, LPARAM lp);				// 通知
	bool		on_notify_lv(C_tnm_dbg_listview* p_lv, WPARAM wp, LPARAM lp, bool table_flag);	// 通知（リストビュー）

	void		save_ini();
	void		load_ini();

	void		update_tv();		// ツリービューを更新
	void		update_lv();		// リストビューを更新

	// 別ウィンドウで開く
	void		open_element_by_another_wnd();
	void		open_list_by_another_wnd();

	// リストビューを再構築する
	void		restruct_lv();
	void		restruct_lv_object();

	// ツリービューを更新する
	void		update_tv_global(HTREEITEM h_item, bool is_opened);
	void		update_tv_stage(HTREEITEM h_item, C_elm_stage* p_stage, bool is_opened);
	void		update_tv_object(HTREEITEM h_item, C_elm_object* p_object, bool is_opened);
	void		update_tv_mwnd(HTREEITEM h_item, C_elm_mwnd* p_mwnd, bool is_opened);
	void		update_tv_screen(HTREEITEM h_item, const C_elm_screen* p_screen, bool is_opened);
	void		update_tv_call(HTREEITEM h_item, const C_elm_call* p_call, bool is_opened);
	void		update_tv_excall(HTREEITEM h_item, C_elm_excall* p_excall, bool is_opened);
	HTREEITEM	update_tv_element(HTREEITEM h_item, HTREEITEM h_child, const C_tnm_element* p_element, bool is_opened);
	HTREEITEM	update_tv_element(HTREEITEM h_item, HTREEITEM h_child, const C_elm_list_base<int>* p_element, int bit, int top_index, CTSTR& name, bool is_opened);

	template <typename TYPE>	// 汎用リスト
	void		update_tv_list(HTREEITEM h_item, C_elm_list_base<TYPE>* p_list, bool is_opened);
	template <>					// オブジェクトリストを特殊化
	void		update_tv_list<C_elm_object>(HTREEITEM h_item, C_elm_list_base<C_elm_object>* p_list, bool is_opened);
	template <>					// 整数リストを特殊化
	void		update_tv_list<int>(HTREEITEM h_item, C_elm_list_base<int>* p_list, bool is_opened);
	template <>					// 整数イベントリストを特殊化
	void		update_tv_list<C_elm_int_event>(HTREEITEM h_item, C_elm_list_base<C_elm_int_event>* p_list, bool is_opened);
	template <>					// 文字列リストを特殊化
	void		update_tv_list<TSTR>(HTREEITEM h_item, C_elm_list_base<TSTR>* p_list, bool is_opened);

	// リストビューを更新する
	void		update_lv_element(C_tnm_element* p_element, int element_form, int bit, int top_index);

	// リストビューをダブルクリックした
	void		on_dblclk_lv(C_tnm_dbg_listview* p_lv, bool table_flag);

	// リストビューの値を変更した
	void		on_change_lv_value(C_tnm_dbg_listview* p_lv, int item_no, int column_no, CTSTR& value, bool table_flag);

	// 現在選択されているアイテム
	HTREEITEM	h_selected_item;
};

