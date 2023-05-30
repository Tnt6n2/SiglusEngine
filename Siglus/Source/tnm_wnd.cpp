#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_input.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_icon.h"
#include	"engine/eng_init.h"
#include	"engine/eng_input.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_msgbox.h"
#include	"engine/eng_display.h"
#include	"engine/eng_etc.h"
#include	"../resource.h"

// ****************************************************************
// ウィンドウプロシージャ
// ================================================================
LRESULT C_tnm_wnd::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	// 入力関係のメッセージ処理
	Gp_input->set_state_by_wm(msg, wp, lp);

	switch (msg)
	{
		case WM_CREATE:			if (!on_create())				return FALSE;	break;
		case WM_CLOSE:			if (!on_close())				return FALSE;	break;
		case WM_PAINT:			if (!on_paint())				return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))		return FALSE;	break;
		case WM_SETCURSOR:		if (!on_set_cursor(wp, lp))		return FALSE;	break;

//		case WM_MENUSELECT:		m_wnd_menu.update();	break;		// メニューを更新（★メニューの選択中に選択している項目が変わる度に送られてくるので適切ではない）
		case WM_INITMENU:
			if ((HMENU)wp == m_wnd_menu.get_handle())	{
				m_wnd_menu.update();		// メニューを更新
			}
			break;

		case WM_SYSKEYDOWN:

			// F10 はデフォルトの処理に回さない
			if (wp == VK_F10)	{
				return FALSE;
			}
			//if (wp == VK_RETURN)	{
			//	return 0;	// 警告音を鳴らさない
			//}
			break;

		case WM_SYSCOMMAND:

			// システムコマンドに入った場合は時間を止める
			if (!Gp_global->alt_menu_wait_flag)	{
				Gp_global->alt_menu_wait_flag = true;
				// 処理が戻ると、フレーム処理で自動的に false に戻る
			}

			// 早送り中やオートモード中はスクリーンセーバを起動させない
			if (wp == SC_SCREENSAVE && (tnm_is_skipping() || tnm_is_auto_mode()))	{
				return 1;	// スクリーンセーバを起動させない場合は 1 を返す
			}
			break;

#if 0	

			// 以下、実験コード
			// 低解像度のモニタに Aero スナップすると、ウィンドウサイズが補正され、アスペクト比とかが変わってえらいことになる
			// どうにかしたいが、WM_SIZE しか飛んでこない上に、メニューを外したときの WM_SIZE などと区別できず…
			// もうあきらめる。

		case WM_SIZE:
			info_box(tostr(LOWORD(lp)) + _T(",") + tostr(HIWORD(lp)) + _T(",") + 
				(wp == SIZE_MAXHIDE ? _T("SIZE_MAXHIDE")
				: wp == SIZE_MAXIMIZED ? _T("SIZE_MAXIMIZED")
				: wp == SIZE_MAXSHOW ? _T("SIZE_MAXSHOW")
				: wp == SIZE_MINIMIZED ? _T("SIZE_MINIMIZED")
				: wp == SIZE_RESTORED ? _T("SIZE_RESTORED") : _T("???")));
			break;
#endif

		case WM_EXITSIZEMOVE:
		{
			// サイズ変更をした場合は強制でフリーに移行します
			tnm_change_screen_size_mode_free(get_window_rect().left_top(), get_client_rect().size(), false);

			break;
		}

		case WM_ENTERSIZEMOVE:
		{
			// サイズ変更前のウィンドウサイズとクライアントサイズの差分を取得しておく
			m_diff.cx = get_window_rect().width() - get_client_rect().width();
			m_diff.cy = get_window_rect().height() - get_client_rect().height();
			break;
		}

		case WM_SIZING:
		{
			C_size old_client_size;
			C_size new_client_size;

			C_rect* screen_rect = (C_rect *)lp;
			old_client_size.cx = screen_rect->width() - m_diff.cx;
			old_client_size.cy = screen_rect->height() - m_diff.cy;

			switch (wp)
			{
				case WMSZ_TOPLEFT:
				{
					float x_rate = (float)old_client_size.cx / Gp_ini->screen_size.cx;
					float y_rate = (float)old_client_size.cy / Gp_ini->screen_size.cy;
					float rate = std::max(x_rate, y_rate);
					new_client_size.cx = std::max((int)ceil(Gp_ini->screen_size.cx * rate), 16);
					new_client_size.cy = std::max((int)ceil(Gp_ini->screen_size.cy * rate), 16);

					screen_rect->left = screen_rect->left - (new_client_size.cx - old_client_size.cx);
					screen_rect->top = screen_rect->top - (new_client_size.cy - old_client_size.cy);
					break;
				}
				case WMSZ_TOP:
				{
					float rate = (float)old_client_size.cy / Gp_ini->screen_size.cy;
					new_client_size.cx = std::max((int)ceil(Gp_ini->screen_size.cx * rate), 16);
					new_client_size.cy = std::max((int)ceil(Gp_ini->screen_size.cy * rate), 16);

					screen_rect->left = screen_rect->left - (new_client_size.cx - old_client_size.cx) / 2;
					screen_rect->right = screen_rect->right + (new_client_size.cx - old_client_size.cx) / 2;
					screen_rect->top = screen_rect->top - (new_client_size.cy - old_client_size.cy);
					break;
				}
				case WMSZ_TOPRIGHT:
				{
					float x_rate = (float)old_client_size.cx / Gp_ini->screen_size.cx;
					float y_rate = (float)old_client_size.cy / Gp_ini->screen_size.cy;
					float rate = std::max(x_rate, y_rate);
					new_client_size.cx = std::max((int)ceil(Gp_ini->screen_size.cx * rate), 16);
					new_client_size.cy = std::max((int)ceil(Gp_ini->screen_size.cy * rate), 16);

					screen_rect->right = screen_rect->right + (new_client_size.cx - old_client_size.cx);
					screen_rect->top = screen_rect->top - (new_client_size.cy - old_client_size.cy);
					break;
				}
				case WMSZ_LEFT:
				{
					float rate = (float)old_client_size.cx / Gp_ini->screen_size.cx;
					new_client_size.cx = std::max((int)ceil(Gp_ini->screen_size.cx * rate), 16);
					new_client_size.cy = std::max((int)ceil(Gp_ini->screen_size.cy * rate), 16);

					screen_rect->left = screen_rect->left - (new_client_size.cx - old_client_size.cx);
					screen_rect->bottom = screen_rect->bottom + (new_client_size.cy - old_client_size.cy);
					break;
				}
				case WMSZ_RIGHT:
				{
					float rate = (float)old_client_size.cx / Gp_ini->screen_size.cx;
					new_client_size.cx = std::max((int)ceil(Gp_ini->screen_size.cx * rate), 16);
					new_client_size.cy = std::max((int)ceil(Gp_ini->screen_size.cy * rate), 16);

					screen_rect->right = screen_rect->right + (new_client_size.cx - old_client_size.cx);
					screen_rect->bottom = screen_rect->bottom + (new_client_size.cy - old_client_size.cy);
					break;
				}
				case WMSZ_BOTTOMLEFT:
				{
					float x_rate = (float)old_client_size.cx / Gp_ini->screen_size.cx;
					float y_rate = (float)old_client_size.cy / Gp_ini->screen_size.cy;
					float rate = std::max(x_rate, y_rate);
					new_client_size.cx = std::max((int)ceil(Gp_ini->screen_size.cx * rate), 16);
					new_client_size.cy = std::max((int)ceil(Gp_ini->screen_size.cy * rate), 16);

					screen_rect->left = screen_rect->left - (new_client_size.cx - old_client_size.cx);
					screen_rect->bottom = screen_rect->bottom + (new_client_size.cy - old_client_size.cy);
					break;
				}
				case WMSZ_BOTTOM:
				{
					float rate = (float)old_client_size.cy / Gp_ini->screen_size.cy;
					new_client_size.cx = std::max((int)ceil(Gp_ini->screen_size.cx * rate), 16);
					new_client_size.cy = std::max((int)ceil(Gp_ini->screen_size.cy * rate), 16);

					screen_rect->left = screen_rect->left - (new_client_size.cx - old_client_size.cx) / 2;
					screen_rect->right = screen_rect->right + (new_client_size.cx - old_client_size.cx) / 2;
					screen_rect->bottom = screen_rect->bottom + (new_client_size.cy - old_client_size.cy);
					break;
				}
				case WMSZ_BOTTOMRIGHT:
				{
					float x_rate = (float)old_client_size.cx / Gp_ini->screen_size.cx;
					float y_rate = (float)old_client_size.cy / Gp_ini->screen_size.cy;
					float rate = std::max(x_rate, y_rate);
					new_client_size.cx = std::max((int)ceil(Gp_ini->screen_size.cx * rate), 16);
					new_client_size.cy = std::max((int)ceil(Gp_ini->screen_size.cy * rate), 16);

					screen_rect->right = screen_rect->right + (new_client_size.cx - old_client_size.cx);
					screen_rect->bottom = screen_rect->bottom + (new_client_size.cy - old_client_size.cy);
					break;
				}
			}

			// サイズ変更前のウィンドウサイズとクライアントサイズの差分を更新
			m_diff.cx = get_window_rect().width() - get_client_rect().width();
			m_diff.cy = get_window_rect().height() - get_client_rect().height();

			return TRUE;
		}

		case WM_ENTERMENULOOP:

			// メニューを開いた場合は時間を止める
			if (!Gp_global->alt_menu_wait_flag)	{
				Gp_global->alt_menu_wait_flag = true;
				// 処理が戻ると、フレーム処理で自動的に false に戻る
			}
			break;
	}

	return C_window::window_proc(msg, wp, lp);
}

// ****************************************************************
// ウィンドウが構築された
// ================================================================
//		メインウィンドウに設定します。
//		保存した矩形を取り出します。
// ================================================================
bool C_tnm_wnd::on_create()
{
	// ★アイコンを設定する
	// ★window.ini 読み込みより先に行わないと設定されない場合があります。原因不明。注意！！
	set_icon(Gp_icon->get_large_icon());
	set_small_icon(Gp_icon->get_small_icon());

	// IME の対象からはずす
	::ImmAssociateContext(m_h_wnd, NULL);	// ← これが正解。このウィンドウのみ IME をオフにする。
	//ImmDisableIME(0);						// ← これは間違い。アプリ自体 IME をオフにしてしまう。エディットボックスへの入力もできない！
	// メインウィンドウに設定
	set_main_window();
	// ini からウィンドウ矩形を取得
	load_ini_window_rect();

	return true;
}

// ****************************************************************
// ウィンドウが閉じられる
// ================================================================
//		ウィンドウ矩形を保存します。
//		ウィンドウが閉じられる直前の処理を行います。
// ================================================================
bool C_tnm_wnd::on_close()
{
	// 終了確認
	if (!Gp_global->game_end_no_warning)	{

		int res = tnm_game_warning_box(Gp_ini->config.warninginfo.gameend_warning_str);
		if (res == IDNO)
			return false;	// WM_CLOSE をキャンセルする
	}

	// エンドセーブを行っていないなら行う
	if (!Gp_global->game_end_save_done && Gp_ini->end_save_cnt > 0)	{
		Gp_global->game_end_save_done = true;	// 失敗しても無限ループにならないよう、先にフラグを立てる

		tnm_syscom_end_game(false, false, false);
		return false;		// WM_CLOSE をキャンセルする
	}

	// ini にウィンドウ矩形を保存
	save_ini_window_rect();
	// ウィンドウが閉じられる直前の解放処理を行います。
	tnm_free();

	return true;
}

// ****************************************************************
// 再描画を求められた
// ================================================================
//		Direct3D を利用して描画を行います。
// ================================================================
bool C_tnm_wnd::on_paint()
{
	// Direct3D のデバイスが準備されている場合
	// Direct3D のデバイスが消失していない場合
	if (G_d3d.device.body() && !G_d3d.device.is_lost())	{

		// 更新矩形をクリアします。
		::ValidateRect(m_h_wnd, NULL);

		// 更新矩形を無視して、画面全体を描画します。
		G_d3d.device.present(NULL);
	}

	// Direct3D のデバイスが準備されていない場合
	else	{

		// 更新矩形をクリアするために、以下の処理は必須です。
		PAINTSTRUCT ps;
		HDC h_dc = BeginPaint(m_h_wnd, &ps);

		// 黒に塗りつぶします。
		HBRUSH h_brush = (HBRUSH)GetStockObject(BLACK_BRUSH);
		SelectObject(h_dc, h_brush);
		FillRect(h_dc, &ps.rcPaint, h_brush);

		// 更新矩形をクリアします。
		EndPaint(m_h_wnd, &ps);
	}

	// デフォルトの処理を実行しない
	return false;
}

// ****************************************************************
// コマンド
// ================================================================
//		メニューなどの処理を行います。
// ================================================================
bool C_tnm_wnd::on_command(WPARAM wp, LPARAM lp)
{
	// ウィンドウメニュー
	if (HIWORD(wp) == 0)
		m_wnd_menu.on_command(LOWORD(wp));

	// ゲームを強制終了（コンパイラから再起動を求められたときに飛んでくる）
	else if (HIWORD(wp) == IDC_END)	{

		// 警告なしでゲームを終了するフラグ
		Gp_global->game_end_no_warning = true;
		// ウィンドウを閉じることでゲームを終了する
		send_message(WM_CLOSE, 0, 0);
	}

	return true;
}

// ****************************************************************
// カーソルを設定
// ================================================================
//		マウスカーソルが動いたときに呼び出されます。
// ================================================================
bool C_tnm_wnd::on_set_cursor(WPARAM wp, LPARAM lp)
{
	// カーソルを再構築する
	//Gp_cursor->restruct_cursor();

	// ↑いらないです。フレーム処理で全て賄います。
	// モーダルダイアログが出ている間にこれを通ると、
	// １瞬だけゲームのカーソルが出て、すぐにシステムカーソルに戻ってしまうからです。

	// どうしても入れる必要がある場合は、
	// フレーム処理内で「モーダルダイアログが出ている間はシステム任せにする」処理を入れてください。

	return true;
}

