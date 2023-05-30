#pragma		once

#include	"db_wnd_info_listview.h"

// ****************************************************************
// デバッグ情報ウィンドウ：ディレクトリ情報
// ================================================================
class C_db_wnd_info_dir : public C_db_wnd_info_listview
{
public:
	void		init();		// 初期化
	void		frame();	// フレーム

private:

	// ステータス
	struct STATE
	{
		int		setup_type;
		TSTR	exe_dir;
		TSTR	base_dir;
		TSTR	work_dir;
		TSTR	save_dir;
	}	m_ld;

	void		on_init_func();		// 初期化ファンクション
	void		on_close_func();	// 閉じるファンクション
};

