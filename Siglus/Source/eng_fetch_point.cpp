#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_tool.h"
#include	"data/tnm_input.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_timer.h"
#include	"engine/eng_input.h"
#include	"engine/eng_fetch_point.h"
#include	"engine/eng_etc.h"

class	C_tnm_fetch_point;
extern	C_tnm_fetch_point*	Gp_fetch_point;

// フェッチをクリアする
void tnm_init_fetch_point()
{
	Gp_fetch_point->m_fetch_point.clear();
	Gp_fetch_point->m_fetch_rect.clear();
	Gp_fetch_point->m_fetch_start_pos = C_point(0, 0);
	Gp_fetch_point->m_fetch_end_pos = C_point(0, 0);
	Gp_fetch_point->m_fetch_last_pos = C_point(0, 0);
	Gp_fetch_point->m_fetch_last_no = 0;
	Gp_fetch_point->m_fetch_start_time = 0;
	Gp_fetch_point->m_fetch_time_len = 0;
	Gp_fetch_point->m_key_repeat_key = 0;
	Gp_fetch_point->m_key_repeat_start_time = 0;
	Gp_fetch_point->m_key_repeat_cnt = 0;
}

// フェッチポイントを追加する
void tnm_add_fetch_point(C_point fetch_pos, C_rect fetch_rect)
{
	Gp_fetch_point->m_fetch_point.push_back(fetch_pos);
	Gp_fetch_point->m_fetch_rect.push_back(fetch_rect);
}

// フェッチ初回移動
bool tnm_fetch_proc_initial_move()
{
#if 1

	// -1: まだどこにもフェッチしていない
	Gp_fetch_point->m_fetch_last_no = -1;

#else

	// ０番にスーッって移動するタイプ


	// ０番にフェッチする
	Gp_fetch_point->m_fetch_last_no = 0;
	Gp_fetch_point->m_fetch_time_len = 250;

	// フェッチポイントをスクリーン座標に直す
	C_point fetch_point = Gp_fetch_point->m_fetch_point[Gp_fetch_point->m_fetch_last_no];
	fetch_point.x = fetch_point.x * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx + Gp_global->total_game_screen_pos.x;
	fetch_point.y = fetch_point.y * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
	fetch_point = Gp_wnd->client_to_screen(fetch_point);

	// （特殊）スクリーン系でのマウス座標を取得
	C_point mp = get_mouse_pos();

	// フェッチ開始
	Gp_fetch_point->m_fetch_start_pos = mp;
	Gp_fetch_point->m_fetch_last_pos = mp;
	Gp_fetch_point->m_fetch_end_pos = fetch_point;
	Gp_fetch_point->m_fetch_start_time = Gp_timer->real_time;
#endif
	return true;
}

// フェッチ処理：フェッチポイントをスクリーン座標に直す
C_point tnm_fetch_proc_fetch_point_client_to_screen(C_point fetch_point)
{
	fetch_point.x = fetch_point.x * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx + Gp_global->total_game_screen_pos.x;
	fetch_point.y = fetch_point.y * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
	fetch_point = Gp_wnd->client_to_screen(fetch_point);
	return fetch_point;
}

// フェッチ処理：フェッチ矩形をスクリーン座標に直す
C_rect tnm_fetch_proc_fetch_rect_client_to_screen(C_rect fetch_rect)
{
	fetch_rect.left = fetch_rect.left * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx + Gp_global->total_game_screen_pos.x;
	fetch_rect.top = fetch_rect.top * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
	fetch_rect.right = fetch_rect.right * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx + Gp_global->total_game_screen_pos.x;
	fetch_rect.bottom = fetch_rect.bottom * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
	fetch_rect = Gp_wnd->client_to_screen(fetch_rect);
	return fetch_rect;
}

// フェッチ処理：当たっているフェッチ矩形を求める
int tnm_fetch_proc_to_search_cur_fetch_point()
{
	int fetch_no = -1;
	for (int i = 0; i < (int)Gp_fetch_point->m_fetch_rect.size(); i++)	{
		// フェッチ矩形をスクリーン座標に直す
		C_rect fetch_rect = tnm_fetch_proc_fetch_rect_client_to_screen(Gp_fetch_point->m_fetch_rect[i]);
		// マウスが当たっていれば確定
		if (fetch_rect.left <= Gp_cur_input->mouse.pos.x && Gp_cur_input->mouse.pos.x < fetch_rect.right)	{
			if (fetch_rect.top <= Gp_cur_input->mouse.pos.y && Gp_cur_input->mouse.pos.y < fetch_rect.bottom)	{
				fetch_no = i;
				break;
			}
		}
	}

	return fetch_no;
}

// フェッチ処理：次に移動
bool tnm_fetch_proc_to_down()
{
#if 1
	// 現在当たっているフェッチ矩形を探す
	C_rect fetch_rect;
	int fetch_no = tnm_fetch_proc_to_search_cur_fetch_point();

	// フェッチ番号を進める
	if (fetch_no == -1)	{
		fetch_no = 0;
	}
	else	{
		fetch_no ++;
		if (fetch_no >= (int)Gp_fetch_point->m_fetch_point.size())
			fetch_no = 0;
	}

	// フェッチ位置を求める
	C_point fetch_point = Gp_fetch_point->m_fetch_point[fetch_no];
	fetch_point = tnm_fetch_proc_fetch_point_client_to_screen(fetch_point);

	// マウス位置を設定
	Gp_cur_input->mouse.pos = fetch_point;
	::SetCursorPos(Gp_cur_input->mouse.pos.x, Gp_cur_input->mouse.pos.y);

	return true;

#else

	// フェッチ番号と移動時間を求める
	if (Gp_fetch_point->m_fetch_last_no == -1)	{
		Gp_fetch_point->m_fetch_last_no = 0;
	}
	else	{
		Gp_fetch_point->m_fetch_last_no ++;
		if (Gp_fetch_point->m_fetch_last_no >= (int)Gp_fetch_point->m_fetch_point.size())
			Gp_fetch_point->m_fetch_last_no = 0;
	}

	// フェッチポイントをスクリーン座標に直す
	C_point fetch_point = Gp_fetch_point->m_fetch_point[Gp_fetch_point->m_fetch_last_no];
	fetch_point.x = fetch_point.x * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx + Gp_global->total_game_screen_pos.x;
	fetch_point.y = fetch_point.y * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
	fetch_point = Gp_wnd->client_to_screen(fetch_point);

	// マウスが既に当たっている場合はもう１つ次に進む
	if (Gp_cur_input->mouse.pos == fetch_point)	{
		Gp_fetch_point->m_fetch_last_no ++;
		if (Gp_fetch_point->m_fetch_last_no >= (int)Gp_fetch_point->m_fetch_point.size())
			Gp_fetch_point->m_fetch_last_no = 0;

		// フェッチポイントをスクリーン座標に直す
		fetch_point = Gp_fetch_point->m_fetch_point[Gp_fetch_point->m_fetch_last_no];
		fetch_point.x = fetch_point.x * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx + Gp_global->total_game_screen_pos.x;
		fetch_point.y = fetch_point.y * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
		fetch_point = Gp_wnd->client_to_screen(fetch_point);
	}

	// マウス位置を設定
	Gp_cur_input->mouse.pos = fetch_point;
	::SetCursorPos(Gp_cur_input->mouse.pos.x, Gp_cur_input->mouse.pos.y);

	return true;
#endif
}

// フェッチ処理：前に移動
bool tnm_fetch_proc_to_up()
{

#if 1
	// 現在当たっているフェッチ矩形を探す
	C_rect fetch_rect;
	int fetch_no = tnm_fetch_proc_to_search_cur_fetch_point();

	// フェッチ番号を戻す
	if (fetch_no == -1)	{
		fetch_no = (int)Gp_fetch_point->m_fetch_point.size() - 1;
	}
	else	{
		fetch_no --;
		if (fetch_no < 0)
			fetch_no = (int)Gp_fetch_point->m_fetch_point.size() - 1;
	}

	// フェッチ位置を求める
	C_point fetch_point = Gp_fetch_point->m_fetch_point[fetch_no];
	fetch_point = tnm_fetch_proc_fetch_point_client_to_screen(fetch_point);

	// マウス位置を設定
	Gp_cur_input->mouse.pos = fetch_point;
	::SetCursorPos(Gp_cur_input->mouse.pos.x, Gp_cur_input->mouse.pos.y);

	return true;

#else
	// フェッチ番号と移動時間を求める
	if (Gp_fetch_point->m_fetch_last_no == -1)	{
		Gp_fetch_point->m_fetch_last_no = (int)Gp_fetch_point->m_fetch_point.size() - 1;
	}
	else	{
		Gp_fetch_point->m_fetch_last_no --;
		if (Gp_fetch_point->m_fetch_last_no < 0)
			Gp_fetch_point->m_fetch_last_no = (int)Gp_fetch_point->m_fetch_point.size() - 1;
	}

	// フェッチポイントをスクリーン座標に直す
	C_point fetch_point = Gp_fetch_point->m_fetch_point[Gp_fetch_point->m_fetch_last_no];
	fetch_point.x = fetch_point.x * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx + Gp_global->total_game_screen_pos.x;
	fetch_point.y = fetch_point.y * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
	fetch_point = Gp_wnd->client_to_screen(fetch_point);

	// マウスが既に当たっている場合はもう１つ次に進む
	if (Gp_cur_input->mouse.pos == fetch_point)	{
		Gp_fetch_point->m_fetch_last_no --;
		if (Gp_fetch_point->m_fetch_last_no < 0)
			Gp_fetch_point->m_fetch_last_no = (int)Gp_fetch_point->m_fetch_point.size() - 1;

		// フェッチポイントをスクリーン座標に直す
		fetch_point = Gp_fetch_point->m_fetch_point[Gp_fetch_point->m_fetch_last_no];
		fetch_point.x = fetch_point.x * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx + Gp_global->total_game_screen_pos.x;
		fetch_point.y = fetch_point.y * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
		fetch_point = Gp_wnd->client_to_screen(fetch_point);
	}

	// マウス位置を設定
	Gp_cur_input->mouse.pos = fetch_point;
	::SetCursorPos(Gp_cur_input->mouse.pos.x, Gp_cur_input->mouse.pos.y);

	return true;
#endif
}

// フェッチ処理
bool tnm_fetch_proc()
{
	// この処理では座標はグローバルな位置で行われる

	// 前回からマウスが動いていればフェッチを終了する
	if (Gp_fetch_point->m_fetch_time_len > 0 && Gp_cur_input->mouse.pos != Gp_fetch_point->m_fetch_last_pos)	{
		Gp_fetch_point->m_fetch_time_len = 0;
	}

	// フェッチする
	if (Gp_fetch_point->m_fetch_time_len > 0)	{

		int ct = Gp_timer->real_time;
		int st = Gp_fetch_point->m_fetch_start_time;
		int et = st + Gp_fetch_point->m_fetch_time_len;
		int svx = Gp_fetch_point->m_fetch_start_pos.x;
		int svy = Gp_fetch_point->m_fetch_start_pos.y;
		int evx = Gp_fetch_point->m_fetch_end_pos.x;
		int evy = Gp_fetch_point->m_fetch_end_pos.y;

		// 減速計算
		int cvx = speed_down_limit(ct, st, svx, et, evx);
		int cvy = speed_down_limit(ct, st, svy, et, evy);

		// マウス位置を設定
		Gp_cur_input->mouse.pos = C_point(cvx, cvy);
		::SetCursorPos(Gp_cur_input->mouse.pos.x, Gp_cur_input->mouse.pos.y);

		// 設定した値を記憶しておく
		Gp_fetch_point->m_fetch_last_pos = C_point(cvx, cvy);

		// 終了時刻を越えたらフェッチ終了
		if (ct - et >= 0)	{
			Gp_fetch_point->m_fetch_time_len = 0;
		}
	}

	// キーボードでカーソル移動（フェッチ移動中は移動しない）
	if (!Gp_fetch_point->m_fetch_point.empty() && Gp_fetch_point->m_fetch_time_len == 0)	{

		// ↓キー／→キー
		if (tnm_input_use_key_down(VK_DOWN) || tnm_input_use_key_down(VK_RIGHT))	{

			// 下に移動
			tnm_fetch_proc_to_down();

			// キーリピート開始
			Gp_fetch_point->m_key_repeat_key = 1;
			Gp_fetch_point->m_key_repeat_start_time = Gp_timer->real_time;
			Gp_fetch_point->m_key_repeat_cnt = 0;
		}
		// ↑キー／←キー
		else if (tnm_input_use_key_down(VK_UP) || tnm_input_use_key_down(VK_LEFT))	{

			// 上に移動
			tnm_fetch_proc_to_up();

			// キーリピート開始
			Gp_fetch_point->m_key_repeat_key = 2;
			Gp_fetch_point->m_key_repeat_start_time = Gp_timer->real_time;
			Gp_fetch_point->m_key_repeat_cnt = 0;
		}
	}

	// キーリピート
	if (Gp_fetch_point->m_key_repeat_key > 0)	{

		// キーリピート終了判定
		if (Gp_fetch_point->m_key_repeat_key == 1 && !tnm_input_is_key_down(VK_DOWN) &&  !tnm_input_is_key_down(VK_RIGHT))	{
			Gp_fetch_point->m_key_repeat_key = 0;
		}
		else if (Gp_fetch_point->m_key_repeat_key == 2 && !tnm_input_is_key_down(VK_UP) && !tnm_input_is_key_down(VK_LEFT))	{
			Gp_fetch_point->m_key_repeat_key = 0;
		}
		else	{

			// リピート回数から次の移動時刻を取得
			int next_time;
			if (Gp_fetch_point->m_key_repeat_cnt == 0)	{
				next_time = Gp_fetch_point->m_key_repeat_start_time + 500;
			}
			else	{
				next_time = Gp_fetch_point->m_key_repeat_start_time + 500 + Gp_fetch_point->m_key_repeat_cnt * 50;
			}

			// 移動時刻を超えてたら移動
			if (Gp_timer->real_time - next_time > 0)	{
				if (Gp_fetch_point->m_key_repeat_key == 1)	{
					tnm_fetch_proc_to_down();
				}
				else	{
					tnm_fetch_proc_to_up();
				}

				// キーリピート回数を増やす
				Gp_fetch_point->m_key_repeat_cnt++;
			}
		}
	}

	return true;
}

// ****************************************************************
// フェッチ処理を行うかを判定
// ================================================================
bool tnm_is_fetch_proc()
{
	// ＥＸコール：「ＥＸコール」中はフェッチ処理を行わない
	if (tnm_excall_is_excall())
		return false;

	// ＥＸコール：「メッセージバック」中はフェッチ処理を行わない
	if (tnm_excall_is_msg_back())
		return false;

	// ＥＸコール：「メッセージウィンドウを隠す」中はフェッチ処理を行わない
	if (tnm_excall_is_hide_mwnd())
		return false;

	// フェッチポイントがなければフェッチ処理を行わない
	if (Gp_fetch_point->m_fetch_point.empty())
		return false;

	return true;
}


