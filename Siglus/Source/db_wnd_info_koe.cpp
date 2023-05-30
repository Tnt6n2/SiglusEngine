#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_local_data.h"
#include	"element/elm_sound.h"
#include	"dialog/db_wnd_info_koe.h"
#include	"engine/ifc_sound.h"
#include	"../resource.h"

// ****************************************************************
// デバッグ情報ウィンドウ：既読フラグ
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_koe")

// 初期化
void C_db_wnd_info_koe::init()
{
	regist(REGIST_WINDOW_NAME, false, true, IDD_DB_WIN_INFO_KOE);
}

// ウィンドウプロシージャ
LRESULT C_db_wnd_info_koe::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
	case WM_INITDIALOG:		if (!on_init())					return FALSE;	break;
	case WM_CLOSE:			if (!on_close())				return FALSE;	break;
	case WM_COMMAND:		if (!on_command(wp, lp))		return FALSE;	break;
	case WM_NOTIFY:			if (!on_notify(wp, lp))			return FALSE;	break;
	case WM_LBUTTONDOWN:	if (!on_lbutton_down(wp, lp))	return FALSE;	break;
	case WM_SIZE:			if (!on_size(wp, lp))			return FALSE;	break;
	}

	return C_db_wnd_info::window_proc(msg, wp, lp);
}

// 初期化
bool C_db_wnd_info_koe::on_init()
{
	// 初期化
	m_first = true;
	m_click_pos = 0;

	// バインド
	bind_control(&m_stc_file, IDC_STC_KOE, _T("stc_koe"), 8);
	bind_control(&m_progress, IDC_PRG_KOE, _T("prg_koe"), 8);
	bind_control(&m_picture, IDC_PCT_KOE, _T("pct_koe"), 5);
	bind_control(&m_picture_rect, IDC_STC_PCT_KOE_RECT, _T("stc_pct_koe_rect"), 5);
	bind_control(&m_stc_koe_length, IDC_STC_KOE_LENGTH, _T("stc_koe_length"), 3);
	bind_control(&m_stc_koe_now, IDC_STC_KOE_NOW, _T("stc_koe_now"), 2);
	bind_control(&m_button_copy, IDC_BTN_COPY, _T("btn_copy"), 1);
	bind_control(&m_chk_copy, IDC_CHK_COPY, _T("chk_copy"), 9);
	
	// ピクチャコントロールのデフォルトのサイズを設定
	m_picture.set_def_rltv_rect(m_picture_rect.get_def_rltv_rect());

	// ウィンドウ矩形の読み込み
	load_ini_window_rect();
	m_chk_copy.load_ini(false);

	// ウィンドウ表示
	update();
	set_show(true);

	// WM_SIZE を送る
	send_wm_size();

	return true;
}

// 閉じる
bool C_db_wnd_info_koe::on_close()
{
	// ウィンドウ矩形の保存
	m_chk_copy.save_ini();
	save_ini_window_rect();

	return true;
}


// コマンド
bool C_db_wnd_info_koe::on_command(WPARAM wp, LPARAM lp)
{
	int ctrl_id = LOWORD(wp);
	if (ctrl_id == IDC_BTN_COPY)
	{
		::str_copy_to_clip_board_in_MB(tostr(m_click_pos));
	}

	return true;
}

// 通知
bool C_db_wnd_info_koe::on_notify(WPARAM wp, LPARAM lp)
{
	return true;
}

// サイズが変更された
bool C_db_wnd_info_koe::on_size(WPARAM wp, LPARAM lp)
{
	update_view();

	return true;
}

// 左クリック
bool C_db_wnd_info_koe::on_lbutton_down(WPARAM wp, LPARAM lp)
{
	C_rect rect = m_picture.get_rltv_rect();

	m_click_pos = (int)((double)(m_ld.mills_max) * (LOWORD(lp) - rect.left) / rect.width());
	m_click_pos = limit(0, m_click_pos, m_ld.mills_max);

	m_stc_koe_now.set_text(_T("ｸﾘｯｸ位置 = ") + tostr(m_click_pos) + _T(" ﾐﾘ秒"));

	tnm_play_koe(m_ld.koe_no, m_ld.chara_no, TNM_JITAN_RATE_NORMAL, m_click_pos, false, false, false, true, false);

	if (m_chk_copy.get_check())
	{
		::str_copy_to_clip_board_in_MB(tostr(m_click_pos));
	}

	return true;
}

// ****************************************************************
// ステータス読み込み
// ================================================================
void C_db_wnd_info_koe::load_state()
{
}

// ****************************************************************
// ステータス保存
// ================================================================
void C_db_wnd_info_koe::save_state()
{
	C_ini_file::body().save_bool(_T("copy_by_click"), m_chk_copy.get_check(), REGIST_WINDOW_NAME);
}

// ****************************************************************
// フレーム処理
// ================================================================
void C_db_wnd_info_koe::frame()
{
	S_param cd;

	if (!m_h_wnd)
		return;

	// パラメータを取得
	cd.koe_no = Gp_sound->m_koe.get_koe_no();
	cd.chara_no = Gp_sound->m_koe.get_chara_no();

	if (m_first)
	{
		cd.mills_max = Gp_sound->m_koe.get_length_by_millsecond();
		cd.mills_now = Gp_sound->m_koe.get_play_pos_by_millsecond();
		cd.prg_max = Gp_sound->m_koe.get_length() / 1024;
		cd.prg_now = Gp_sound->m_koe.get_play_pos() / 1024;
	}
	else
	{
		int mills_max = Gp_sound->m_koe.get_length_by_millsecond();
		int mills_now = Gp_sound->m_koe.get_play_pos_by_millsecond();
		int prg_max = Gp_sound->m_koe.get_length() / 1024;
		int prg_now = Gp_sound->m_koe.get_play_pos() / 1024;

		if (cd.koe_no == m_ld.koe_no && cd.chara_no == m_ld.chara_no && m_ld.prg_max > 0 && prg_max == 0)
		{
			cd.mills_max = m_ld.mills_max;		// 前回の値を使う
			cd.mills_now = m_ld.mills_max;		// 前回の最大を使う（最後まで再生されたとみなす）
			cd.prg_max = m_ld.prg_max;			// 前回の値を使う
			cd.prg_now = m_ld.prg_max;			// 前回の最大を使う（最後まで再生されたとみなす）
		}
		else
		{
			cd.mills_max = mills_max;
			cd.mills_now = mills_now;
			cd.prg_max = prg_max;
			cd.prg_now = prg_now;
		}
	}

	// 声ファイル
	if (m_first || cd.koe_no != m_ld.koe_no || cd.chara_no != m_ld.chara_no)
	{
		if (cd.koe_no >= 0)
		{
			if (cd.chara_no >= 0)
			{
				m_stc_file.set_text(str_format(_T("KOE(%08d,%03d)"), cd.koe_no, cd.chara_no));
			}
			else
			{
				m_stc_file.set_text(str_format(_T("KOE(%08d)"), cd.koe_no));
			}

			update_view();
		}
		else
		{
			m_stc_file.clear_text();
		}
	}

	// プログレス
	if (m_first || cd.prg_max != m_ld.prg_max || cd.prg_now != m_ld.prg_now)
	{
		m_progress.set_max_prg(cd.prg_max + 1);
		m_progress.set_prg(cd.prg_now + 1);
		m_progress.set_prg(cd.prg_now);
		m_progress.set_max_prg(cd.prg_max);

		m_stc_koe_length.set_text(tostr(cd.mills_now) + _T(" / ") + tostr(cd.mills_max) + _T(" ﾐﾘ秒"));
	}

	// 今回の情報を保存
	m_ld = cd;

	// 初回フラグを降ろす
	m_first = false;
}

// ****************************************************************
// ビューを更新する
// ================================================================
void C_db_wnd_info_koe::update_view()
{
	int cw = m_picture.get_client_rect().width();
	int ch = m_picture.get_client_rect().height();
	if (cw > 0 && ch > 0)
	{
		// ピクチャを作成
		m_bitmap.create(cw, ch, 32);
		m_bitmap.clear_color(C_argb(255, 0, 0, 255));

		// ストリームを取得
		C_sound_stream_base* p_stream = Gp_sound->m_koe.get_stream(0);
		if (p_stream)
		{
			int sample_cnt = p_stream->get_sample_cnt();
			int byte = p_stream->get_bit_cnt_per_sample() / 8;
			int channnel = p_stream->get_channel_cnt();

			// データへのポインタを取得
			BYTE* p = p_stream->get_ptr();
			if (p)
			{
				for (int x = 0; x < cw; x++)
				{
					int first_sample = sample_cnt * x / cw;
					int last_sample = sample_cnt * (x + 1) / cw;
					int cnt = last_sample - first_sample;

					if (byte == 1)
					{
						int max_value = 0;

						for (int i = 0; i < cnt; i++)
						{
							max_value = std::max(abs(*(signed char *)p), max_value);

							p += channnel;
						}

						m_bitmap.draw_rect(x, ch / 2 - max_value * ch / 256, 1, max_value * ch / 128, C_argb(255, 0, 255, 0), true, false);
					}
					else if (byte == 2)
					{
						int max_value = 0;

						for (int i = 0; i < cnt; i++)
						{
							max_value = std::max(abs(*(short int *)p), max_value);

							p += channnel * 2;
						}

						m_bitmap.draw_rect(x, ch / 2 - max_value * ch / 65536, 1, max_value * ch / 32768, C_argb(255, 0, 255, 0), true, false);
					}
				}
			}
		}

		m_picture.set_bitmap(m_bitmap.get_handle());
	}
}

