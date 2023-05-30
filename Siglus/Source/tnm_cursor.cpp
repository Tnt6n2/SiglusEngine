#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_cursor.h"
#include	"data/tnm_local_data.h"

#include	"engine/eng_dir.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_dialog.h"
#include	"engine/eng_editbox.h"
#include	"engine/ifc_error.h"
#include	"engine/eng_etc.h"

// ****************************************************************
// カーソル構造体
// ================================================================
S_cursor::S_cursor()
{
	h_icon = NULL;
	h_mask = NULL;
}
S_cursor::~S_cursor()
{
	if (h_mask)	{
		::DeleteObject(h_mask);
		h_mask = NULL;
	}
	if (h_icon)	{
		::DestroyIcon(h_icon);
		h_icon = NULL;
	}
}

// ****************************************************************
// カーソル
// ================================================================
C_tnm_cursor::C_tnm_cursor()
{
	m_cursor_pat_cnt = 0;
}

// ****************************************************************
// カーソル：カーソルを作成する
// ================================================================
void C_tnm_cursor::create_cursor(int cursor_no)
{
	if (cursor_no < 0 || Gp_ini->mouse_cursor_cnt <= cursor_no)
		return;

	// ▲まだ g00 しか対応していません。
	TSTR file_name = Gp_ini->mouse_cursor[cursor_no].file_name;
	if (file_name.empty())
		return;
	
	int pct_type = 0;
	TSTR file_path = tnm_find_g00(file_name, &pct_type);

	if (file_path.empty())
	{
		tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("マウスカーソル ") + file_name + _T(" が見つかりませんでした。\n") + file_name);
		return;
	}
	if (pct_type != 1)
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("マウスカーソルは g00 しか対応していません。\n") + file_name);
		return;
	}

	// アルバムに g00 を読み込む
	C_album album;
	if (!::load_g00_to_album(&album, file_path))
	{
		tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("マウスカーソル ") + file_name + _T(" が読み込めませんでした。\n") + get_last_error_msg());
		return;
	}

	// サイズを調べる（32×32 固定）
	for (int i = 0; i < album.get_pct_cnt(); i++)
	{
		BSP<C_pct> pct = album.get_pct(i);
		if (pct)
		{
			if (pct->get_width() != 32 || pct->get_height() != 32)
			{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("マウスカーソルのサイズは 32×32 でなければいけません。\n") + file_name + _T(" (patno = ") + tostr(i) + _T(")"));
			}
		}
	}

	// アイコンをカット数だけ準備する
	m_cursor_pat_cnt = (int)album.get_pct_cnt();
	m_cursor_pat_list = BSA<S_cursor>(new S_cursor[m_cursor_pat_cnt]);

	// 各カットデータを展開する
	for (int i = 0; i < album.get_pct_cnt(); i++)
	{
		BSP<C_pct> pct = album.get_pct(i);
		C_point center = pct->get_center();
		BSP<C_dib_chip> src_chip = pct->expand_chips();
		if (src_chip)
		{
			// アイコン用のビットマップを作成
			C_dib dib_icon(true);
			dib_icon.create(32, 32, 32);	// 32×32 固定なのです。
			{
				int w = std::min(src_chip->get_width(), 32);	// 32×32に制限。
				int h = std::min(src_chip->get_height(), 32);	// 32×32に制限。
				for (int y = 0; y < h; y++)	{
					C_argb* dst = (C_argb *)dib_icon.get_ptr(0, y);
					C_argb* src = (C_argb *)src_chip->get_ptr(0, y);
					memcpy(dst, src, w * sizeof(C_argb));
				}
			}

			// マスク用の空ビットマップを作成
			m_cursor_pat_list[i].h_mask = ::CreateBitmap(32, 32, 1, 1, NULL);

			// アイコンの情報を設定
			ICONINFO info;
			info.fIcon = FALSE;			// FALSE: カーソル
			info.xHotspot = center.x;	// 中心座標をホットスポットとして扱う
			info.yHotspot = center.y;	// 中心座標をホットスポットとして扱う
			info.hbmColor = dib_icon.get_handle();
			info.hbmMask = m_cursor_pat_list[i].h_mask;

			// アイコンを作成
			m_cursor_pat_list[i].h_icon = ::CreateIconIndirect(&info);
			if (m_cursor_pat_list[i].h_icon == NULL)
			{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("%d 番目のマウスカーソルの作成に失敗しました。\n"), i) + get_last_error_msg());
			}
		}
	}
}

// ****************************************************************
// カーソル：フレーム処理
// ================================================================
void tnm_cursor_frame(int cur_time)
{
	HCURSOR h_cursor = NULL;

	// クライアント外にいるときはシステムにまかせるので何もしない
	C_point mp = Gp_wnd->screen_to_client(get_mouse_pos());
	C_rect cr = Gp_wnd->get_client_rect();
	if (!(cr.left <= mp.x && mp.x < cr.right && cr.top <= mp.y && mp.y < cr.bottom))
		return;

	// 他のダイアログ内にいるときはシステムにまかせるので何もしない
	if (tnm_dlg_check_mouse_over())
		return;

	// 他のエディットボックス内にいるときはシステムにまかせるので何もしない
	if (tnm_editbox_check_mouse_over())
		return;

	// 表示フラグ
	bool disp_flag = true;

	// カーソルを表示しない場合
	if (!tnm_is_cursor_disp())
		disp_flag = false;

	// 最終値：カーソルを表示する場合のみ処理
	if (disp_flag)	{

		int cursor_no = Gp_local->pod.cursor_no;

		// Windows デフォルトのカーソルの場合
		if (cursor_no < 0 || Gp_ini->mouse_cursor_cnt <= cursor_no || !Gp_cursor_list->sub[cursor_no].m_cursor_pat_list)
		{
			h_cursor = LoadCursor(NULL, IDC_ARROW);
		}

		// 画像から読み込む場合
		else
		{
			int pat_no = 0;

			// アニメーションする
			int anime_speed = Gp_ini->mouse_cursor[cursor_no].anime_speed;
			pat_no = (anime_speed == 0) ? 0 : (DWORD)cur_time / anime_speed % Gp_cursor_list->sub[cursor_no].m_cursor_pat_cnt;
				// ↑(DWORD) は重要です。int のままだと、時刻がマイナスのとき、% 演算子はマイナス値を返してしまいます。

			// 該当するカーソルのハンドルを取得
			h_cursor = Gp_cursor_list->sub[cursor_no].m_cursor_pat_list[pat_no].h_icon;
		}
	}

	// カーソルが変わったら変更する
	if (::GetCursor() != h_cursor)
		::SetCursor(h_cursor);
}
