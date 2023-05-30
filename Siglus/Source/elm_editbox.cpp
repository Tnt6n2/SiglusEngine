#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_editbox.h"
#include	"engine/eng_display.h"
#include	"engine/eng_editbox.h"

// ****************************************************************
// エディットボックス：プロシージャ
// ================================================================
LRESULT C_tnm_editbox::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_SYSKEYDOWN:
			if (wp == VK_RETURN)	{
				if ((lp >> 29) & 1)	{
					tnm_switch_screen_size_mode(false);		// Alt+Enter 時はスクリーンモード切替
				}

				// Enter キーでは、デフォルトの動作を実行しません。
				// 実行すると、Windows のエラー効果音が鳴ってしまうからです。
				return TRUE;
			}
			break;

		case WM_KEYDOWN:
			if (wp == VK_TAB)	{
				if (GetAsyncKeyState(VK_SHIFT) & 0x8000)	{
					tnm_editbox_move_focus_backward();		// Shift+Tab: 前へ
				}
				else	{
					tnm_editbox_move_focus_forward();		// Tab: 次へ
				}
			}
			if (wp == VK_RETURN)	{
				m_action_flag = TNM_EDITBOX_DECIDED;		// Enter キーの処理（Alt+Enter 時は WM_SYSKEYDOWN を通るため、ここは通りません）

				// Enter キーでは、デフォルトの動作を実行しません。
				// 実行すると、Windows のエラー効果音が鳴ってしまうからです。
				return TRUE;
			}
			if (wp == VK_ESCAPE)	{
				m_action_flag = TNM_EDITBOX_CANCELED;		// Escape キーの処理
			}
			break;

		case WM_CHAR:
			// この処理がないと、Tab、Enter、ESC キーで Windows の警告音が鳴ってしまう
			if (wp == VK_TAB || wp == VK_RETURN || wp == VK_ESCAPE)	{
				return FALSE;
			}
			break;
	}

	return Cc_edit::window_proc(msg, wp, lp);
}

// ****************************************************************
// エレメント：エディットボックス
// ================================================================
C_elm_editbox::C_elm_editbox()
{
	m_h_font = NULL;
}
C_elm_editbox::~C_elm_editbox()
{
	destroy();
}

// ****************************************************************
// エレメント：エディットボックス：初期化
// ================================================================
void C_elm_editbox::init()
{
	// 再初期化
	reinit();
}

// ****************************************************************
// エレメント：エディットボックス：再初期化
// ================================================================
void C_elm_editbox::reinit()
{
	destroy();
	m_editbox.init();

	// パラメータ
	m_created = false;
	m_rect = C_rect(0, 0, 0, 0);
}

// ****************************************************************
// エレメント：エディットボックス：セーブ
// ================================================================
void C_elm_editbox::save(C_tnm_save_stream& stream)
{
	stream.save(m_created);
	stream.save(m_rect);
	stream.save(m_moji_size);
}

// ****************************************************************
// エレメント：エディットボックス：ロード
// ================================================================
void C_elm_editbox::load(C_tnm_save_stream& stream)
{
	stream.load(m_created);
	stream.load(m_rect);
	stream.load(m_moji_size);

	// 再構築
	if (m_created)
		create(m_rect, m_moji_size);
}

// ****************************************************************
// エレメント：エディットボックス：操作
// ================================================================

// 作成
void C_elm_editbox::create(C_rect rect, int moji_size)
{
	destroy();

	m_editbox.init();		// 決定したフラグを初期化する
	m_editbox.create(_T("siglus_editbox"), _T(""), rect.left, rect.top, rect.width(), rect.height(), false, WS_CHILD | ES_AUTOHSCROLL, 0, false, false, G_app.h_wnd);
	m_created = true;
	m_rect = rect;
	m_moji_size = moji_size;

	// 表示する前に矩形をウィンドウサイズに合わせて更新する
	update_rect();

	m_editbox.set_show(true);
	m_editbox.set_focus();
}

// 破壊
void C_elm_editbox::destroy()
{
	m_editbox.set_show(false);
	m_editbox.destroy();
	release_font();
	m_created = false;
}

// フォントを解放
void C_elm_editbox::release_font()
{
	if (m_h_font)	{
		::DeleteObject(m_h_font);
		m_h_font = NULL;
	}
}

// テキストを設定
void C_elm_editbox::set_text(CTSTR& text)
{
	m_editbox.set_text(text);

	// テキストを変更した場合、カーソルを末尾に移動する
	int moji_cnt_max = (int)m_editbox.send_message(EM_GETLIMITTEXT, 0, 0);
	m_editbox.set_sel(moji_cnt_max, moji_cnt_max);
}

// テキストを取得
TSTR C_elm_editbox::get_text()
{
	return m_editbox.get_text();
}

// フォーカスを設定する
void C_elm_editbox::set_focus()
{
	m_editbox.set_focus();
}

// 入力をクリアする
void C_elm_editbox::clear_input()
{
	m_editbox.clear_input();
}

// ****************************************************************
// エレメント：エディットボックス：フレーム処理
// ================================================================
void C_elm_editbox::frame()
{
	// エディットボックスを表示する条件：
	bool show_flag = m_created								// 作られている
		&& Gp_global->change_display_mode_proc_cnt == 0		// 解像度切り替え中でない
//		&& !Gp_excall->is_ready()							// ＥＸコールが準備中でない
		;

	m_editbox.set_show(show_flag);
}

// ****************************************************************
// エレメント：エディットボックス：矩形を更新
// ================================================================
void C_elm_editbox::update_rect()
{
	// ゲームスクリーンサイズに合わせて矩形を更新します。
	C_rect new_rect;
	new_rect.left = m_rect.left * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx  + Gp_global->total_game_screen_pos.x;
	new_rect.top = m_rect.top * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
	new_rect.right = m_rect.right * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx + Gp_global->total_game_screen_pos.x;
	new_rect.bottom = m_rect.bottom * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
	m_editbox.set_window_rect(new_rect);

	// さらに文字サイズも変更します。
	release_font();
	int new_moji_size = m_moji_size * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy;	// 文字サイズはゲームスクリーンの縦幅にあわせます。
	m_h_font = ::CreateFont(new_moji_size, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("ＭＳ ゴシック"));
	m_editbox.send_message(WM_SETFONT, (WPARAM)m_h_font, (LPARAM)MAKELPARAM(TRUE, 0));
}

// ****************************************************************
// エレメント：エディットボックスリスト：サブの初期化
// ================================================================
void C_elm_editbox_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init();
}

// ****************************************************************
// エレメント：エディットボックスリスト：サブの再初期化
// ================================================================
void C_elm_editbox_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// エレメント：エディットボックスリスト：サブのセーブ
// ================================================================
void C_elm_editbox_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// エレメント：エディットボックスリスト：サブのロード
// ================================================================
void C_elm_editbox_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}

// ****************************************************************
// エディットボックスのフォーカスを次に移動（Tab キーによる処理）
// ================================================================
void tnm_editbox_move_focus_forward()
{
	int editbox_cnt = Gp_editbox_list->get_size();

	// エディットボックスがなければ終了
	if (editbox_cnt == 0)
		return;

	// フォーカスのあるエディットボックスを取得する
	int focused_editbox_no = -1;
	HWND h_focused_wnd = ::GetFocus();
	if (h_focused_wnd)	{
		for (int i = 0; i < editbox_cnt; i++)	{
			if ((*Gp_editbox_list)[i].editbox().get_handle() == h_focused_wnd)	{
				focused_editbox_no = i;
				break;
			}
		}
	}
	if (focused_editbox_no == -1)
		return;

	// フォーカスを移動する
	for (int i = focused_editbox_no; ; )	{
		i = (i + editbox_cnt + 1) % editbox_cnt;
		if (i == focused_editbox_no)	{
			return;		// 見つからなかったので終了
		}
		if ((*Gp_editbox_list)[i].is_created())	{
			(*Gp_editbox_list)[i].set_focus();
			return;
		}
	}
}


// ****************************************************************
// エディットボックスのフォーカスを前に移動（Shift+Tab キーによる処理）
// ================================================================
void tnm_editbox_move_focus_backward()
{
	int editbox_cnt = Gp_editbox_list->get_size();

	// エディットボックスがなければ終了
	if (editbox_cnt == 0)
		return;

	// フォーカスのあるエディットボックスを取得する
	int focused_editbox_no = -1;
	HWND h_focused_wnd = ::GetFocus();
	if (h_focused_wnd)	{
		for (int i = 0; i < editbox_cnt; i++)	{
			if ((*Gp_editbox_list)[i].editbox().get_handle() == h_focused_wnd)	{
				focused_editbox_no = i;
				break;
			}
		}
	}
	if (focused_editbox_no == -1)
		return;

	// フォーカスを移動する
	for (int i = focused_editbox_no; ; )	{
		i = (i + editbox_cnt - 1) % editbox_cnt;
		if (i == focused_editbox_no)	{
			return;		// 見つからなかったので終了
		}
		if ((*Gp_editbox_list)[i].is_created())	{
			(*Gp_editbox_list)[i].set_focus();
			return;
		}
	}
}


// ****************************************************************
// エディットボックスにマウスが乗っているかを判定
// ================================================================
bool tnm_editbox_check_mouse_over()
{
	int editbox_cnt = Gp_editbox_list->get_size();
	C_point mp = get_mouse_pos();

	// カーソルが載っているかを判定
	for (int i = 0; i < editbox_cnt; i++)	{
		if ((*Gp_editbox_list)[i].editbox().get_handle())	{
			C_rect wr = (*Gp_editbox_list)[i].editbox().get_window_rect();
			if (wr.left <= mp.x && mp.x < wr.right && wr.top <= mp.y && mp.y < wr.bottom)	{
				return true;	// 上に乗っている！
			}
		}
	}

	return false;
}

// ****************************************************************
// エディットボックスを全て非表示にする
// ================================================================
void tnm_editbox_set_hide_all()
{
	// エディットボックスを全て隠します。（復帰処理はフレーム処理でやってくれます。）
	int editbox_cnt = (int)Gp_editbox_list->get_size();
	for (int i = 0; i < editbox_cnt; i++)	{
		(*Gp_editbox_list)[i].editbox().set_show(false);
	}
}

