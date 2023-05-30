#pragma		once

#include	"data/tnm_dlg.h"

// ****************************************************************
// デバッグウィンドウ
// ================================================================
class C_db_wnd_info : public C_tnm_dlg
{

public:

protected:

};

// ****************************************************************
// デバッグ用のリストコントロール
// ================================================================
class C_tnm_dbg_listbox : public Cc_listbox
{
public:
	int		add_item(CTSTR& str);				// アイテムを適当な位置に追加
	int		insert_item(int pos, CTSTR& str);	// アイテムを位置を指定して追加（-1 で末尾）（LBS_SORT が指定されていないこと）
};

// ****************************************************************
// デバッグ用のリストビュー
// ================================================================
class C_tnm_dbg_listview : public Cc_listview
{
public:
	int		add_item();														// アイテムを追加
	int		add_item(int column_no, CTSTR& str);							// アイテムを追加
	int		add_item_visible(int column_no, CTSTR& str);					// アイテムを追加、可視範囲に来るようにスクロール
	int		insert_item(int item_no);										// アイテムを挿入
	int		insert_item(int item_no, int column_no, CTSTR& str);			// アイテムを挿入
	int		insert_item_visible(int item_no, int column_no, CTSTR& str);	// アイテムを挿入、可視範囲に来るようにスクロール
	void	reduce_item(int item_cnt);										// アイテムを削減
	void	set_item_cnt(int item_cnt);										// アイテム数を設定（増やしたり減らしたりする）

	void	set_cell_text(int item_no, int column_no, CTSTR& str);			// テキストを設定
	void	set_cell_text_visible(int item_no, int column_no, CTSTR& str);	// テキストを設定してアイテムを可視範囲に移動
};

