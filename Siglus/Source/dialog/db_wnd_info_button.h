#pragma		once

#include	"data/tnm_sorter.h"
#include	"dialog/db_wnd_info_listview.h"

class		C_tnm_btn_mng;

// ****************************************************************
// デバッグ情報ウィンドウ：ボタンマネージャ
// ================================================================
class C_db_wnd_info_btn : public C_db_wnd_info_listview
{
public:
	void		init();		// 初期化
	void		frame();	// フレーム

private:

	// ステータス
	struct STATE
	{
		int				btn_cnt;			// ボタンの個数
		int				active_btn_guid;	// アクティブなボタンのGUID
		S_tnm_sorter	active_btn_sorter;	// アクティブなボタンのソーター
		int				active_btn_state;	// アクティブなボタンのステート
		bool			hit_now;			// 今回当たった
		bool			pushed_now;			// 今回押された
		bool			decided_now;		// 今回決定された

	}	m_ld;

	void		on_init_func();		// 初期化ファンクション
	void		on_close_func();	// 閉じるファンクション
};
