#pragma		once

#include	"db_wnd_info_listview.h"

// ****************************************************************
// デバッグ情報ウィンドウ：声
// ================================================================
class C_db_wnd_info_koe : public C_db_wnd_info
{
public:

	C_db_wnd_info_koe() : m_bitmap(true)
	{
	}

	void		init();					// 初期化
	void		load_state();			// ステータス読み込み
	void		save_state();			// ステータス保存
	void		frame();				// フレーム処理

	Cc_static	m_stc_file;
	Cc_static	m_stc_koe_now;
	Cc_static	m_stc_koe_length;
	Cc_progress	m_progress;
	Cc_picture	m_picture;
	Cc_static	m_picture_rect;
	Cc_button	m_button_copy;
	Cc_check	m_chk_copy;

	NT3::C_dib	m_bitmap;

private:

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// ウィンドウプロシージャ
	bool		on_init();										// 初期化
	bool		on_close();										// 閉じる
	bool		on_command(WPARAM wp, LPARAM lp);				// コマンド
	bool		on_notify(WPARAM wp, LPARAM lp);				// 通知
	bool		on_lbutton_down(WPARAM wp, LPARAM lp);			// 左クリック
	bool		on_size(WPARAM wp, LPARAM lp);					// サイズが変更された

	void		update_view();

	// パラメータ
	struct S_param
	{
		int		koe_no;
		int		chara_no;
		int		prg_max;
		int		prg_now;
		int		mills_max;
		int		mills_now;
	};

	int			m_click_pos;
	S_param		m_ld;

	// 作業変数
	bool		m_first;
};

