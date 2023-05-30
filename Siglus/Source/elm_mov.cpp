#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_def.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_input.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_mov.h"
#include	"engine/eng_dir.h"
#include	"engine/eng_input.h"
#include	"engine/eng_config.h"
#include	"engine/ifc_error.h"
#include	"dialog/cfg_wnd_modal_movtype.h"


// ****************************************************************
// ムービー：wmp での再生に ATL を使います
// ================================================================
#define _ATL_APARTMENT_THREADED

#include	<atlbase.h>
#include	<atlhost.h>
#include	<lzss.h>

#include	"wmp.h"

// メンバ変数にしたいところ…でもヘッダが膨れ上がるのでとりあえずここに。
// C_elm_mov のインスタンスを複数作ったらやばい！
// impl パターンを使って隠蔽するべきかもね。
static	CAxWindow					Gwmp_wndView = 0;
static	CComPtr<IAxWinHostWindow>	Gwmp_spHost = 0;
static	CComPtr<IWMPPlayer>			Gwmp_spWMPPlayer = 0;
static	CComPtr<IWMPPlayer2>		Gwmp_spWMPPlayerE = 0;
static	CComPtr<IWMPControls>		Gwmp_spWMPControls = 0;
static	CComPtr<IWMPSettings>		Gwmp_spWMPSettings = 0;

const int TNM_MCI_VOLUME_MAX = 1000;
const int TNM_WMP_VOLUME_MAX = 100;

// ****************************************************************
// ムービー：初期化
// ================================================================
void C_elm_mov::init(S_element element, int form, CTSTR& name)
{
	// 基底の初期化
	C_tnm_element::init(element, form, name, NULL);

	// パラメータの初期化
	m_method = -1;

	// wmp パラメータ
	m_volume = 255;
	m_wmp_ready = false;
}

// ****************************************************************
// ムービー：再初期化
// ================================================================
void C_elm_mov::reinit()
{
	close();

	m_method = -1;
	m_wmp_ready = false;
}

// ****************************************************************
// ムービー：解放
// ================================================================
void C_elm_mov::free()
{
	// 閉じる
	close();
}

// ****************************************************************
// ムービー：セーブ
// ================================================================
void C_elm_mov::save(C_tnm_save_stream& stream)
{
	stream.save(m_param.file_name);
}

// ****************************************************************
// ムービー：ロード
// ================================================================
void C_elm_mov::load(C_tnm_save_stream& stream)
{
	stream.load(m_param.file_name);

	// 再構築
	restruct();
}

// ****************************************************************
// ムービー：再構築
// ================================================================
void C_elm_mov::restruct()
{
	int err = 0;
	int movie_type = 0;

	// ファイル名が空なら終了
	if (m_param.file_name.empty())
		return;

	// ファイルを探す
	TSTR file_path = tnm_find_mov(Gp_dir->base_dir, _T("mov"), m_param.file_name, &movie_type);
	if (file_path.empty())
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("ムービーファイル ") + m_param.file_name + _T(" が見つかりませんでした。"));
		return;
	}

	// ムービー再生中に落ちた場合の補助処理
	if (check_file_playing())	{

		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("ムービーの再生中に落ちた時の補助処理を行います。"));

		// サポートダイアログで再生方法を変更する
		Ccfg_wnd_modal_movtype config_mov_wnd;
		bool res = config_mov_wnd.open_modal_result_bool();

		// 「再生しない」を選んだ場合は再生しない
		if (!res)	{
			close();
			return;
		}
	}

	// 現在のメソッドを指定
	m_method = Gp_system_config->movie_type;

	// 再生中を示すファイルを作成する
	create_file_playing();

	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("ムービーの再生を開始します。"));

	if (false);

	// mci による再生
	else if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{

		TSTR file_type_str;
		switch (movie_type)	{
		case TNM_MOVIE_FILE_TYPE_AVI:	file_type_str = _T("avivideo");		break;
		case TNM_MOVIE_FILE_TYPE_MPG:	file_type_str = _T("MPEGVideo");	break;
		case TNM_MOVIE_FILE_TYPE_WMV:	file_type_str = _T("MPEGVideo");	break;
		default:						file_type_str = _T("MPEGVideo");	break;
		}

		// ムービーを開く
		err = mciSendString(str_format(_T("open \"%s\" type %s alias SiglusMCIMovie wait"), file_path.c_str(), file_type_str.c_str()).c_str(), NULL, 0, NULL);
		if (err)	{

			TCHAR err_str[256];
			mciGetErrorString(err, err_str, 256);
			tnm_set_error(TNM_ERROR_TYPE_FATAL, file_path + _T("のオープンに失敗しました。\n\n≪詳細≫\n") + err_str);
		}

		else	{

			// ムービーウィンドウを作成する（ここではまだ表示されません。）
			if (!Gp_movie_wnd->create())	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("ムービーウィンドウの作成に失敗しました。"));
				return;
			}

			// ムービーウィンドウを無効にする（マウスカーソルを乗っ取らなくするため）
			Gp_movie_wnd->set_enable(false);

			// ムービーを再生するウィンドウを指定する
			err = mciSendString((_T("window SiglusMCIMovie handle ") + tostr((UINT)Gp_movie_wnd->get_handle()) + _T(" wait")).c_str(), NULL, 0, NULL);
			if (err)	{

				TCHAR err_str[256];
				mciGetErrorString(err, err_str, 256);
				tnm_set_error(TNM_ERROR_TYPE_FATAL, TSTR() + _T("ムービーをウィンドウに割り当てるのに失敗しました。") + err_str);
			}
			else	{
				C_point pos = m_emp.get_calc_pos();
				C_size size = m_emp.get_calc_size();

				// ウィンドウの位置とサイズを変更する
				Gp_movie_wnd->set_window_rect(C_rect::by_size(pos, size));

				// ムービーのサイズを指定する
				err = mciSendString(str_format(_T("put SiglusMCIMovie destination at 0 0 %d %d wait"), size.cx, size.cy).c_str(), NULL, 0, NULL);
				if (err)	{

					TCHAR err_str[256];
					mciGetErrorString(err, err_str, 256);
					tnm_set_error(TNM_ERROR_TYPE_FATAL, TSTR() + _T("ムービーの位置の設定に失敗しました。") + err_str);
				}
				else	{

					// ムービーを再生する
					err = mciSendString(_T("play SiglusMCIMovie"), NULL, 0, NULL);
					if (err)	{

						TCHAR err_str[256];
						mciGetErrorString(err, err_str, 256);
						tnm_set_error(TNM_ERROR_TYPE_FATAL, TSTR() + _T("ムービーの再生に失敗しました。") + err_str);
					}
					else	{

						// ボリュームを調整する
						int mci_volume = m_volume * TNM_MCI_VOLUME_MAX / 255;
						err = mciSendString(str_format(_T("setaudio SiglusMCIMovie left volume to %d"), mci_volume).c_str(), NULL, 0, NULL);
						err = mciSendString(str_format(_T("setaudio SiglusMCIMovie right volume to %d"), mci_volume).c_str(), NULL, 0, NULL);
						if (err)	{

							TCHAR err_str[256];
							mciGetErrorString(err, err_str, 256);
							tnm_set_error(TNM_ERROR_TYPE_FATAL, TSTR() + _T("ムービーの再生に失敗しました。") + err_str);
						}
						else	{

							// ウィンドウを表示する
							Gp_movie_wnd->set_show(true);
							tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("ムービーの再生を開始しました。"));
						}
					}
				}
			}
		}
	}

	// wmp による再生
	else if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{

		C_point pos = m_emp.get_calc_pos();
		C_size size = m_emp.get_calc_size();
		C_rect rect = C_rect::by_size(pos, size);

		//Gwmp_wndView.Create(G_app.h_wnd, rect, NULL, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0);				// wmp ウィンドウ作成
		//Gwmp_wndView.Create(G_app.h_wnd, C_rect(0, 0, rect.width() - 10, rect.height() - 10), NULL, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0);	// wmp ウィンドウ作成
		Gwmp_wndView.Create(G_app.h_wnd, C_rect(0, 0, rect.width() - 10, rect.height() - 10), NULL, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0);	// wmp ウィンドウ作成

		// ★ -10 って？
		// 準備終了後、確実にリサイズを起こすため、rect とは違う値を入れる必要がある
		// (0, 0, 0, 0) だと、解放時になんか白枠が表示される。謎。
		// とりあえず -10 入れてみたら上手くいったからこれでいいや。

		if (Gwmp_wndView.m_hWnd != NULL){

			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("ムービーウィンドウを作成しました。"));
			int wmp_volume = m_volume * TNM_WMP_VOLUME_MAX / 255;

			int res = 0;
			EnableWindow(Gwmp_wndView.m_hWnd, FALSE);										// wmp ウィンドウ上でのクリックを禁止
			res = Gwmp_wndView.QueryHost(&Gwmp_spHost);										// ホストクエリ
			if(res >= 0){res = Gwmp_spHost->CreateControl(CComBSTR(_T("{6BF52A52-394A-11d3-B153-00C04F79FAA6}")), Gwmp_wndView, 0);}		// コントロール作成
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("ムービーのコントロールを作成しました。"));
			if(res >= 0){res = Gwmp_wndView.QueryControl(&Gwmp_spWMPPlayer);}				// コントロールクエリ
			if(res >= 0){res = Gwmp_spWMPPlayer.QueryInterface(&Gwmp_spWMPPlayerE);}		// 上位プレイヤークエリ
			if(res >= 0){res = Gwmp_spWMPPlayerE->put_uiMode(A2WBSTR("none"));}				// ユーザーインターフェイス禁止
			if(res >= 0){res = Gwmp_spWMPPlayerE->put_enableContextMenu(FALSE);}			// コンテキストメニュー禁止
			if(res >= 0){res = Gwmp_spWMPPlayerE->put_enabled(FALSE);}						// コントロールを禁止
			if(res >= 0){res = Gwmp_spWMPPlayerE->put_stretchToFit(TRUE);}					// ウィンドウサイズにフィットさせる
			if(res >= 0){res = Gwmp_spWMPPlayerE->get_controls(&Gwmp_spWMPControls);}		// コントロール取得
			if(res >= 0){res = Gwmp_spWMPPlayerE->get_settings(&Gwmp_spWMPSettings);}		// セッティング取得
			if(res >= 0){res = Gwmp_spWMPSettings->put_volume(wmp_volume);}					// 音量を設定
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("ムービーの各種設定を行いました。"));
			if(res >= 0){res = Gwmp_spWMPPlayerE->put_URL(OLE2BSTR(file_path.c_str()));}	// 再生するファイルを関連付ける
			if(res >= 0){

				// 準備フラグを立てる
				m_wmp_ready = true;
				tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("ムービーファイルを開きました。"));
			}
		}
	}

	// ムービーが再生できていなければ停止
	if (get_state() != TNM_MOVIE_PLAY_STATE_PLAY)	{
		close();

		tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("ムービーが再生できませんでした。"));
	}
}

// ****************************************************************
// ムービー：再生
// ================================================================
void C_elm_mov::play(CTSTR& file_name, int px, int py, int sx, int sy)
{
	// パラメータを設定
	m_param.file_name = file_name;
	m_emp.set_pos(px, py);
	m_emp.set_size(sx, sy);

	// 再構築
	restruct();
}

// ****************************************************************
// ムービー：閉じる
// ================================================================
void C_elm_mov::close()
{
	// パラメータをクリア
	m_param.file_name.clear();

	// 閉じる
	if (false);

	// mci による停止
	else if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{

		// ウィンドウを非表示にする
		Gp_movie_wnd->set_show(false);

		// 停止する
		mciSendString(_T("close SiglusMCIMovie wait"), NULL, 0, NULL);

		// ムービーウィンドウを終了する
		Gp_movie_wnd->close();
			
		// 再生中を示すファイルを削除する
		delete_file_playing();

		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("ムービーを停止しました。"));
	}

	// wmp による停止
	else if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{

		// まずはウィンドウを隠す
		//::ShowWindow(Gwmp_wndView.m_hWnd, SW_HIDE);
		//Gwmp_wndView.MoveWindow(0, 0, 0, 0, FALSE);
		//Gwmp_spWMPPlayerE->

		// 順に停止
		if (Gwmp_spWMPControls)	{
			Gwmp_spWMPControls->stop();
		}
		if (Gwmp_spWMPPlayerE)	{
			Gwmp_spWMPPlayerE->close();
		}
		if (Gwmp_spWMPPlayer)	{
			Gwmp_spWMPPlayer->close();
		}
		if (Gwmp_spWMPSettings)	{
			Gwmp_spWMPSettings.Release();
			Gwmp_spWMPSettings = 0;
		}
		if (Gwmp_spWMPControls)	{
			Gwmp_spWMPControls.Release();
			Gwmp_spWMPControls = 0;
		}
		if (Gwmp_spWMPPlayerE)	{
			Gwmp_spWMPPlayerE.Release();
			Gwmp_spWMPPlayerE = 0;
		}
		if (Gwmp_spWMPPlayer)	{
			Gwmp_spWMPPlayer.Release();
			Gwmp_spWMPPlayer = 0;
		}
		if (Gwmp_spHost)	{
			Gwmp_spHost.Release();
			Gwmp_spHost = 0;
		}
		if (Gwmp_wndView)	{
			Gwmp_wndView.DestroyWindow();
			Gwmp_wndView = 0;
		}

		// 再生中を示すファイルを削除する
		delete_file_playing();

		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("ムービーを停止しました。"));
	}

	// パラメータを初期化
	m_method = -1;
	m_wmp_ready = false;
}

// ****************************************************************
// ムービー：フレーム処理
// ================================================================
void C_elm_mov::frame()
{
	if (m_method == -1)	{
		return;
	}

	// 再生中になれば表示する（準備中の間は表示しないので常に監視する必要があるのです。）
	if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{
		if (m_wmp_ready)	{
			if (get_wmp_state_detail() == wmppsPlaying)	{

				// 準備完了！！ウィンドウを表示する
				Sleep(100);		// Vista で一瞬ゴミが現れる対策。wmp のバージョンのせいかなぁ？

				Gwmp_wndView.ShowWindow(SW_SHOW);	// ここでウィンドウを表示する
				tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("ムービーの再生を開始しました。"));
				m_wmp_ready = false;

				// ここでボリュームを再設定してみる（効果なかったのでやめる）
				//long volume = 0;
				//Gwmp_spWMPSettings->get_volume(&volume);
				//Gwmp_spWMPSettings->put_volume(volume - 10);
				//Gwmp_spWMPSettings->put_volume(volume);

				// ここでリサイズ
				update_rect_org();
			}
		}

		//C_point pos = m_emp.get_calc_pos();
		//C_size size = m_emp.get_calc_size();
		//C_rect rect = C_rect::by_size(pos, size);
		//Gwmp_wndView.ResizeClient(rect.width(), rect.height(), FALSE);

		// ▲▲▲
		//if (Gp_input->now.keyboard.key[_T('Q')].is_down())	{
			//Gwmp_wndView.ShowWindow(SW_HIDE);
		//}

	}

	// ムービーが終了していれば閉じる
	if (get_state() == TNM_MOVIE_PLAY_STATE_STOP)	{
		close();
	}
}

// ****************************************************************
// ムービー：表示矩形を変更する
// ================================================================
/*
void C_elm_mov::update_rect()
{
	if (false);

	// mci
	else if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{
		C_point pos = Gp_global->total_game_screen_pos;
		C_size size = Gp_global->total_game_screen_size;

		// ウィンドウの位置とサイズを変更する
		Gp_movie_wnd->set_window_rect(C_rect::by_size(pos, size));

		// ムービーのサイズを指定する
		MCI_ANIM_RECT_PARMS marp;
		ZeroMemory(&marp, sizeof(marp));
		marp.dwCallback = (PTR)Gapp.h_wnd;
		marp.rc = C_rect(C_point(0, 0), size);	// right,bottom に右下座標でなくサイズを入れないとうまくいかない！（位置が 0, 0 になったので関係ないか…）
		mciSendCommand(m_mci_device_id, MCI_PUT, MCI_WAIT | MCI_ANIM_RECT | MCI_ANIM_PUT_DESTINATION, (PTR)&marp);
	}

	// wmp
	else if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{
		if (Gwmp_wndView && Gwmp_wndView.m_hWnd)	{
			C_rect rect = C_rect::by_size(Gp_global->total_game_screen_pos, Gp_global->total_game_screen_size);
			::SetWindowPos(Gwmp_wndView.m_hWnd, 0, rect.left, rect.top, rect.width(), rect.height(), SWP_NOZORDER | SWP_NOACTIVATE);	// 位置と大きさ
		}
	}

	update_rect_org();
}
*/

void C_elm_mov::update_rect_org()
{
	if (false);

	// mci
	else if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{

		//C_point pos = Gp_global->total_game_screen_pos;
		//C_size size = Gp_global->total_game_screen_size;
		C_point pos = m_emp.get_calc_pos();
		C_size size = m_emp.get_calc_size();

		// ウィンドウの位置とサイズを変更する
		Gp_movie_wnd->set_window_rect(C_rect::by_size(pos, size));

		// ムービーのサイズを指定する
		//MCI_ANIM_RECT_PARMS marp;
		//ZeroMemory(&marp, sizeof(marp));
		//marp.dwCallback = (PTR)G_app.h_wnd;
		//marp.rc = C_rect(C_point(0, 0), size);	// right,bottom に右下座標でなくサイズを入れないとうまくいかない！（位置が 0, 0 になったので関係ないか…）
		//mciSendCommand(m_mci_device_id, MCI_PUT, MCI_WAIT | MCI_ANIM_RECT | MCI_ANIM_PUT_DESTINATION, (PTR)&marp);
		mciSendString(str_format(_T("put SiglusMCIMovie destination at 0 0 %d %d wait"), size.cx, size.cy).c_str(), NULL, 0, NULL);
	}

	// wmp
	else if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{
		if (Gwmp_wndView && Gwmp_wndView.m_hWnd)	{
			C_point pos = m_emp.get_calc_pos();
			C_size size = m_emp.get_calc_size();
			C_rect rect = C_rect::by_size(pos, size);
			//Gwmp_wndView.ResizeClient(rect.width(), rect.height(), TRUE);
			Gwmp_wndView.MoveWindow(&rect, FALSE);
		}
	}
}

void C_elm_mov::change_rect(const C_point& pos, const C_size& size)
{
	m_emp.set_pos_and_size(pos, size);
	update_rect_org();
}

void C_elm_mov::add_rect(const C_point& pos, const C_size& size)
{
	m_emp.add_pos(pos);
	m_emp.add_size(size);
	update_rect_org();
}

void C_elm_mov::set_size_rate(float x, float y)
{
	m_emp.set_size_rate(x, y);
}

void C_elm_mov::set_total_game_screen_pos(const C_point& pos)
{
	m_emp.set_total_game_screen_pos(pos);
}

// ****************************************************************
// ムービー：ボリュームを設定する
// ================================================================
void C_elm_mov::set_volume(int volume)
{
	m_volume = volume;

	// mci
	if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{

		int mci_volume = m_volume * TNM_MCI_VOLUME_MAX / 255;
		mciSendString(str_format(_T("setaudio SiglusMCIMovie left volume to %d"), mci_volume).c_str(), NULL, 0, NULL);
		mciSendString(str_format(_T("setaudio SiglusMCIMovie right volume to %d"), mci_volume).c_str(), NULL, 0, NULL);
	}
	// wmp
	else if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{
		if (Gwmp_spWMPSettings)	{

			int wmp_volume = m_volume * TNM_WMP_VOLUME_MAX / 255;
			Gwmp_spWMPSettings->put_volume(wmp_volume);
		}
	}
}

// ****************************************************************
// ムービー：再生中なら一時停止
// ================================================================
void C_elm_mov::pause()
{
	if (false);	

	// mci
	else if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{

		//MCI_GENERIC_PARMS mgp;
		//ZeroMemory(&mgp, sizeof(mgp));
		//mciSendCommand(m_mci_device_id, MCI_PAUSE, 0, (PTR)&mgp);
		mciSendString(_T("pause SiglusMCIMovie wait"), NULL, 0, NULL);
	}
	// wmp
	else if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{
		if(Gwmp_spWMPControls) {
			Gwmp_spWMPControls->pause();
		}
	}
}

// ****************************************************************
// ムービー：一時停止中なら再生再開
// ================================================================
void C_elm_mov::resume()
{
	if (false);	

	// mci
	else if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{

		//MCI_GENERIC_PARMS mgp;
		//ZeroMemory(&mgp, sizeof(mgp));
		//mciSendCommand(m_mci_device_id, MCI_RESUME, 0, (PTR)&mgp);
		mciSendString(_T("resume SiglusMCIMovie wait"), NULL, 0, NULL);
	}
	// wmp
	else if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{
		if(Gwmp_spWMPControls) {
			Gwmp_spWMPControls->play();
		}
	}
}

// ****************************************************************
// ムービー：状態を取得
// ================================================================
int C_elm_mov::get_state()
{
	if (false);

	// mci
	else if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{

		//MCI_STATUS_PARMS msp;
		//ZeroMemory(&msp, sizeof(msp));
		//msp.dwItem = MCI_STATUS_MODE;
		//mciSendCommand(m_mci_device_id, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (PTR)&msp);

		TCHAR mci_buf[256] = {0};
		mciSendString(_T("status SiglusMCIMovie mode wait"), mci_buf, 256, NULL);

		return convert_play_state(mci_buf);
	}

	// wmp
	else if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{

		if (Gwmp_spWMPPlayerE)	{
			WMPPlayState state;
			int res = Gwmp_spWMPPlayerE->get_playState(&state);
			if (res >= 0){
				return convert_play_state(state);
			}
		}
		return TNM_MOVIE_PLAY_STATE_STOP;
	}

	// 再生していない
	else	{

		return TNM_MOVIE_PLAY_STATE_STOP;
	}
}

// ****************************************************************
// ムービー：MCI,WMP形式の再生状態を独自形式に変換する
// ================================================================
int C_elm_mov::convert_play_state(CTSTR& str)
{
	int ret = TNM_MOVIE_PLAY_STATE_STOP;

	if (false);

	// mci
	else if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{

		if (false);
		else if (str.substr(0, 9) == _T("not ready"))	ret = TNM_MOVIE_PLAY_STATE_STOP;
		else if (str.substr(0, 7) == _T("stopped"))		ret = TNM_MOVIE_PLAY_STATE_STOP;
		else if (str.substr(0, 7) == _T("playing"))		ret = TNM_MOVIE_PLAY_STATE_PLAY;
		else if (str.substr(0, 6) == _T("paused"))		ret = TNM_MOVIE_PLAY_STATE_PAUSE;
	}

	return ret;
}

int C_elm_mov::convert_play_state(int state)
{
	int retv = TNM_MOVIE_PLAY_STATE_STOP;

	if (false);

	// mci
	else if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{
		//MCI_MODE_NOT_READY
		//MCI_MODE_STOP     
		//MCI_MODE_PLAY     
		//MCI_MODE_RECORD   
		//MCI_MODE_SEEK     
		//MCI_MODE_PAUSE    
		//MCI_MODE_OPEN     
		switch (state) {
			case MCI_MODE_STOP:  retv = TNM_MOVIE_PLAY_STATE_STOP;	break;
			case MCI_MODE_PLAY:  retv = TNM_MOVIE_PLAY_STATE_PLAY;	break;
			case MCI_MODE_PAUSE: retv = TNM_MOVIE_PLAY_STATE_PAUSE;	break;
		}
	}
	// wmp
	else if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{
		//wmppsUndefined	= 0,
		//wmppsStopped	= wmppsUndefined + 
		//wmppsPaused	= wmppsStopped + 1,
		//wmppsPlaying	= wmppsPaused + 1,
		//wmppsScanForward	= wmppsPlaying 
		//wmppsScanReverse	= wmppsScanForw
		//wmppsBuffering	= wmppsScanReverse 
		//wmppsWaiting	= wmppsBuffering + 
		//wmppsMediaEnded	= wmppsWaiting + 1,
		//wmppsTransitioning	= wmppsMediaEnd
		//wmppsReady	= wmppsTransitioning + 
		//wmppsReconnecting	= wmppsReady + 
		//wmppsLast	= wmppsReconnecting + 1
		switch (state) {
			case wmppsStopped:			retv = TNM_MOVIE_PLAY_STATE_STOP;	break;
			case wmppsPlaying:			retv = TNM_MOVIE_PLAY_STATE_PLAY;	break;
			case wmppsPaused:			retv = TNM_MOVIE_PLAY_STATE_PAUSE;	break;
			case wmppsTransitioning:	retv = TNM_MOVIE_PLAY_STATE_PLAY;	break;	// STATE増やすべきかも
			case wmppsWaiting:			retv = TNM_MOVIE_PLAY_STATE_PLAY;	break;	// STATE増やすべきかも
		}
	}

	return retv;
}

// ****************************************************************
// ムービー：wmp のステート詳細を取得する
// ================================================================
int C_elm_mov::get_wmp_state_detail()
{
	// wmp の場合のみ処理
	if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{
		if (Gwmp_spWMPPlayerE)	{

			WMPPlayState state;
			Gwmp_spWMPPlayerE->get_playState(&state);
			return (int)state;
		}
	}

	return -1;
}

// ****************************************************************
// ムービー：再生中かを判定する
// ================================================================
//		準備中も「再生中」に含みます。
// ================================================================
bool C_elm_mov::is_playing()
{	
	int state = get_state();
	return state == TNM_MOVIE_PLAY_STATE_PLAY || state == TNM_MOVIE_PLAY_STATE_PAUSE;
}

// ****************************************************************
// ムービー：準備中かを判定する
// ================================================================
bool C_elm_mov::is_readying()
{
	if (get_wmp_state_detail() == (int)wmppsTransitioning)
		return true;

	return false;
}

// ****************************************************************
// ムービー：ボリュームを取得する
// ================================================================
int C_elm_mov::get_volume()
{
	return m_volume;
}
int C_elm_mov::get_volume_real()
{
	if (Gwmp_spWMPSettings)	{
		long volume = 0;
		Gwmp_spWMPSettings->get_volume(&volume);
		return volume;
	}
	return 0;
}

// ****************************************************************
// ムービー：再生中を示すファイルを作成する
// ================================================================
void C_elm_mov::create_file_playing()
{
	C_file::write_full_text_in_MB(Gp_dir->get_movie_playing_file_path(), _T("playing"));
}

// ****************************************************************
// ムービー：再生中を示すファイルを削除する
// ================================================================
void C_elm_mov::delete_file_playing()
{
	filesys_delete_file(Gp_dir->get_movie_playing_file_path());
}

// ****************************************************************
// ムービー：再生中を示すファイルをチェックする
// ================================================================
bool C_elm_mov::check_file_playing()
{
	int res = file_get_type(Gp_dir->get_movie_playing_file_path());
	if (res == 1)
		return true;	// ファイルが存在する

	return false;
}

// ****************************************************************
// ムービーパラメータ
// ================================================================
const C_size C_elm_mov_param::DEF_MOV_SIZE = C_size(640, 480);

C_elm_mov_param::C_elm_mov_param()
:m_pos(C_point(0, 0))
,m_size(DEF_MOV_SIZE)
,m_size_rate(1.0f, 1.0f)
,m_total_game_screen_pos(0, 0)
{
}

C_elm_mov_param::C_elm_mov_param(const C_point& pos, const C_size& size)
:m_pos(pos)
,m_size(size)
,m_size_rate(1.0f, 1.0f)
,m_total_game_screen_pos(0, 0)
{
}

void C_elm_mov_param::add_pos(const C_point& add)
{
	m_pos += add;
}
void C_elm_mov_param::add_pos(int x, int y)
{
	m_pos.x += x;
	m_pos.y += y;
}
void C_elm_mov_param::add_size(const C_size& add)
{
	m_size.cx += add.cx;
	m_size.cy += add.cy;
}
void C_elm_mov_param::add_size(int x, int y)
{
	m_size.cx += x;
	m_size.cy += y;
}

// アクセサメソッド
void C_elm_mov_param::set_pos(const C_point& pos)
{
	m_pos = pos;
}
void C_elm_mov_param::set_pos(int x, int y)
{
	m_pos.x = x;
	m_pos.y = y;
}
void C_elm_mov_param::set_size(const C_size& size)
{
	m_size = size;
}
void C_elm_mov_param::set_size(int x, int y)
{
	m_size.cx = x;
	m_size.cy = y;
}
void C_elm_mov_param::set_pos_and_size(const C_point& pos, const C_size& size)
{
	m_pos = pos;
	m_size = size;
}
void C_elm_mov_param::set_pos_and_size(int px, int py, int sx, int sy)
{
	m_pos.x = px;
	m_pos.y = py;
	m_size.cx = sx;
	m_size.cy = sy;
}

void C_elm_mov_param::set_size_rate(float rate_x, float rate_y)
{
	m_size_rate.x = rate_x;
	m_size_rate.y = rate_y;
}

void C_elm_mov_param::set_total_game_screen_pos(const C_point& pos)
{
	m_total_game_screen_pos = pos;
}

C_point C_elm_mov_param::get_pos()
{
	return m_pos;
}
C_size C_elm_mov_param::get_size()
{
	return m_size;
}

D3DXVECTOR2 C_elm_mov_param::get_size_rate()
{
	return m_size_rate;
}

C_point C_elm_mov_param::get_calc_pos()
{
	C_point retv = m_pos;

	// 拡縮サイズ比率を掛ける
	retv.x = (LONG)(retv.x * m_size_rate.x) + m_total_game_screen_pos.x;
	retv.y = (LONG)(retv.y * m_size_rate.y) + m_total_game_screen_pos.y;

	return retv;
}

C_size C_elm_mov_param::get_calc_size()
{
	C_size retv = m_size;

	// 拡縮サイズ比率を掛ける
	retv.cx = (LONG)(retv.cx * m_size_rate.x);
	retv.cy = (LONG)(retv.cy * m_size_rate.y);

	return retv;
}



// ****************************************************************
// ムービーウィンドウ：
// ================================================================
LRESULT C_tnm_movie_wnd::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	// 入力関係のメッセージ処理
	Gp_input->set_state_by_wm(msg, wp, lp);

	switch (msg)	{
		case WM_PAINT:		if (!on_paint())	return FALSE;	break;
	}

	return C_window::window_proc(msg, wp, lp);
}

bool C_tnm_movie_wnd::create()
{
	C_point pos = Gp_global->total_game_screen_pos;
	C_size size = Gp_global->total_game_screen_size;
	if (!C_window::create(_T("siglus_engine_movie_wnd"), _T("siglus_engine_movie_wnd"), _T(""), pos.x, pos.y, size.cx, size.cy, -1, -1, 0, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, false, false, Gp_wnd->get_handle()))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("ムービーウィンドウの構築に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	return true;
}

bool C_tnm_movie_wnd::on_paint()
{
	PAINTSTRUCT ps;
	HDC dc = BeginPaint(m_h_wnd, &ps);
	EndPaint(m_h_wnd, &ps);

	return true;
}

