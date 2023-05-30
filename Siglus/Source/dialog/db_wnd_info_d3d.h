#pragma		once

#include	"db_wnd_info_listview.h"
#include	"data/tnm_rsc_manager.h"

// ****************************************************************
// デバッグ情報ウィンドウ：Direct3D - デバイス
// ================================================================
class C_db_wnd_info_d3d_device : public C_db_wnd_info_listview
{
public:
	void		init();					// 初期化
	void		frame();				// フレーム

private:

	// ステータス
	struct STATE
	{
		int		video_memory_default_size;
		int		video_memory_rest_size;
		int		video_memory_use_size;
		int		video_memory_use_max_size;
	}	m_ld;

	void		on_init_func();		// 初期化ファンクション
	void		on_close_func();	// 閉じるファンクション
};

// ****************************************************************
// デバッグ情報ウィンドウ：Direct3D - リソース
// ================================================================
class C_db_wnd_info_d3d_resource : public C_db_wnd_info_listview
{

public:

	void		init();					// 初期化
	void		frame();				// フレーム

private:

	// ステータス
	struct STATE
	{
		ARRAY<S_tnm_d3d_resource>	resource_list;
	}	last_state;

	// 作業変数
	struct WORK
	{
	}	work;

	void		on_init_func();		// 初期化ファンクション
	void		on_close_func();	// 閉じるファンクション
};

