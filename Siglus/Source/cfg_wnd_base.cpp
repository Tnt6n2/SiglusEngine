#include	"pch.h"
#include	"dialog/cfg_wnd_base.h"

// ****************************************************************
// コンフィグウィンドウ：ベース
// ================================================================

// 初期化
bool C_cfg_wnd_base::on_init()
{
	// 初期化
	on_init_func();

	// ウィンドウ表示
	update();
	set_show(true);

	return true;
}

// 閉じる
bool C_cfg_wnd_base::on_close()
{
	on_close_func();
	return true;
}

// フレーム
void C_cfg_wnd_base::frame()
{
	frame_func();
}
