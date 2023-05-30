#pragma		once
#include	"cfg_wnd_solo.h"
#include	"cfg_wnd_func_mod.h"

// ****************************************************************
// コンフィグウィンドウ：ベース
// ================================================================
class C_cfg_wnd_base : public C_cfg_wnd_solo
{
public:
	void		frame();				// フレーム

private:

	bool		on_init();				// 初期化
	bool		on_close();				// 閉じる
};

