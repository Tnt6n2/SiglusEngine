#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_sorter.h"
#include	"data/tnm_btn.h"
#include	"data/tnm_input.h"
#include	"engine/eng_input.h"

// ****************************************************************
// ボタン
// ================================================================
C_tnm_btn::C_tnm_btn()
{
	m_rotate = 0;
	m_rect_2_use = false;
	m_push_keep = false;
	m_is_decided = false;
}

C_tnm_btn::~C_tnm_btn()
{
}

// 初期化
void C_tnm_btn::init()
{
	m_guid.init();
	m_rect = C_rect(0, 0, 0, 0);
	m_rect_2 = C_rect(0, 0, 0, 0);
	m_center = C_point(0, 0);
	m_rotate = 0;
	m_sorter = S_tnm_sorter(0, 0);
	m_rect_2_use = false;
	m_push_keep = false;
	m_is_decided = false;
	m_texture.reset();
}

// フレーム初期化
void C_tnm_btn::frame_init()
{
	m_rect = C_rect(0, 0, 0, 0);
	m_rect_2 = C_rect(0, 0, 0, 0);
	m_center = C_point(0, 0);
	m_rotate = 0;
	m_sorter = S_tnm_sorter(0, 0);
	m_rect_2_use = false;
	m_push_keep = false;
	m_texture.reset();
}

// パラメータを設定
void C_tnm_btn::set_rect(C_rect rect)
{
	m_rect = rect;
}

void C_tnm_btn::set_rect_2(bool use, C_rect rect)
{
	m_rect_2_use = use;
	m_rect_2 = rect;
}

void C_tnm_btn::set_center(C_point center)
{
	m_center = center;
}

void C_tnm_btn::set_rotate(int rotate)
{
	m_rotate = rotate;
}

void C_tnm_btn::set_sorter(S_tnm_sorter sorter)
{
	m_sorter = sorter;
}

void C_tnm_btn::set_push_keep(bool push_keep)
{
	m_push_keep = push_keep;
}

void C_tnm_btn::set_param(C_rect rect, S_tnm_sorter sorter)
{
	m_rect = rect;
	m_sorter = sorter;
}

void C_tnm_btn::set_texture(BSP<C_d3d_texture> texture)
{
	m_texture = texture;
}

// ヒットテスト
bool C_tnm_btn::hit_test(C_point pos)
{
	bool hit = true;

	// 第２矩形を先に判定
	if (m_rect_2_use)
	{
		hit &= m_rect_2.is_in(pos);
	}

	// 第１矩形は回転することがある
	if (hit && m_rotate)
	{
		// マウス位置を center に合わせて逆回転する
		double rotate_rad = -(double)m_rotate * D3DX_PI / 1800;

		C_point new_pos;
		new_pos.x = (LONG)((double)(pos.x - m_center.x) * cos(rotate_rad) - (double)(pos.y - m_center.y) * sin(rotate_rad) + m_center.x);
		new_pos.y = (LONG)((double)(pos.x - m_center.x) * sin(rotate_rad) + (double)(pos.y - m_center.y) * cos(rotate_rad) + m_center.y);

		pos = new_pos;
	}

	// テクスチャが指定されている場合はテクスチャを参照
	if (hit && m_texture)
	{
		bool texture_hit = false;

		C_rect texture_rect = m_texture->get_rect();
		C_point pos_on_texture = pos - m_center;
		pos_on_texture.x = pos_on_texture.x * m_texture->get_size().cx / m_rect.width();
		pos_on_texture.y = pos_on_texture.y * m_texture->get_size().cy / m_rect.height();

		if (texture_rect.is_in(pos_on_texture))
		{
			C_argb color = m_texture->get_color(pos_on_texture.x, pos_on_texture.y);
			if (color.a > 0)
			{
				texture_hit = true;
			}
		}

		hit &= texture_hit;
	}
	// テクスチャが設定されていない場合は矩形で判定
	else
	{
		hit &= m_rect.is_in(pos);
	}

	return hit;
}


// ****************************************************************
// ボタンマネージャ：初期化
// ================================================================
void C_tnm_btn_mng::init()
{
	m_last_mouse_push_flag = false;
	m_last_active_btn_guid = -1;
}

// ****************************************************************
// ボタンマネージャ：フレーム初期化
// ================================================================
bool C_tnm_btn_mng::frame_init()
{
	m_btn_list.clear();

	return true;
}

// ****************************************************************
// ボタンマネージャ：ボタンを登録
// ================================================================
void C_tnm_btn_mng::regist_button(C_tnm_btn* btn)
{
	m_btn_list.push_back(btn);
}

// ****************************************************************
// ボタンマネージャ：ボタンがアクティブかどうかを判定
// ================================================================
bool C_tnm_btn_mng::is_active(C_tnm_btn* p_btn)
{
	return m_active_btn_guid != -1 && m_active_btn_guid == p_btn->get_guid();
}

// ****************************************************************
// ボタンマネージャ：当たり状態を取得
// ================================================================
int C_tnm_btn_mng::get_hit_state(C_tnm_btn* p_btn)
{
	// アクションを選択
	if (false);
	else if (is_active(p_btn) && get_active_btn_state() == TNM_BTN_STATE_HIT)	{
		return TNM_BTN_STATE_HIT;
	}
	else if (is_active(p_btn) && get_active_btn_state() == TNM_BTN_STATE_PUSH)	{
		return TNM_BTN_STATE_PUSH;
	}
	else	{
		return TNM_BTN_STATE_NORMAL;
	}
}

// ****************************************************************
// ボタンマネージャ：当たり判定処理
// ================================================================
bool C_tnm_btn_mng::hit_test_proc()
{
	// フレーム初期化
	m_hit_this_frame = false;
	m_pushed_this_frame = false;
	m_decided_this_frame = false;

	// 今回のステートを取得
	m_active_btn_guid = -1;
	m_active_btn_sorter.order = 0;
	m_active_btn_sorter.layer = 0;
	m_active_btn_state = TNM_BTN_STATE_NORMAL;

	// 入力を取得
	C_point mouse_pos = tnm_input_get_mouse_pos();
	bool mouse_push_flag = m_last_mouse_push_flag;
	if (tnm_input_is_key_up(VK_EX_DECIDE))		mouse_push_flag = false;
	if (tnm_input_is_key_down(VK_EX_DECIDE))	mouse_push_flag = true;

	// １フレーム内に「押して放した」場合、「押した」状態にする
	// 自動的に「放した」は次フレームに処理される
	// これはタッチパネル対策です。木本さんのるっきゅんで１タッチした場合、
	// 放したときの１フレーム内に押した放したが来ます。
	//if (m_last_mouse_push_flag == false && mouse_push_flag == false)	{
	//	if (tnm_input_check_key_down_up(VK_EX_DECIDE))	{
	//		mouse_push_flag = true;							// 「押した」
	//	}
	//}

	// 処理が決定したフラグ（「当たった」「押した」「決定した」「キャンセルした」などの処理が複数発生しないようにするためのフラグ）
	bool end = false;

	// ボタンにGUIDをあらかじめ与えておく
	for (int b = 0; b < (int)m_btn_list.size(); b++)	{
		C_tnm_btn* btn = m_btn_list[b];

		// ボタンにGUIDを与える
		btn->create_guid();
	}

	// １．前回押していて今回放していない場合、アクティブを維持する
	if (m_last_mouse_push_flag && mouse_push_flag)	{

		// アクティブを維持する
		m_active_btn_guid = m_last_active_btn_guid;

		// 全ボタンに対して処理を行う
		for (int b = 0; b < (int)m_btn_list.size(); b++)	{
			C_tnm_btn* btn = m_btn_list[b];

			// アクティブなボタンの場合
			if (m_active_btn_guid == btn->get_guid())	{

				m_active_btn_sorter = btn->m_sorter;		// アクティブなボタンのソーター
				m_active_btn_state = (btn->m_push_keep || btn->hit_test(mouse_pos)) ? TNM_BTN_STATE_PUSH : TNM_BTN_STATE_NORMAL;

				// ※アクティブなのに TNM_HIT_STATE_NORMAL とは？
				//		アクティブなヒットを押すと、TNM_HIT_STATE_PUSH 状態になります。
				//		押したままマウスを遠ざけると、アクティブを保ったまま押していない状態、つまり TNM_HIT_STATE_NORMAL になります。
			}
		}
	}

	// ２．それ以外の場合は、アクティブを維持しないかもしれない
	else	{

		// ２－１．前回押していたけど今回離した場合、決定判定を行う
		if (m_last_mouse_push_flag && !mouse_push_flag)	{

			// 全ボタンに対して処理を行う
			for (int b = 0; b < (int)m_btn_list.size(); b++)	{
				C_tnm_btn* btn = m_btn_list[b];

				// 前回アクティブだったボタンの場合
				if (m_last_active_btn_guid == btn->get_guid())	{

					tnm_input_use_key_up(VK_EX_DECIDE);		// 入力を消費

					// 前回当たっていた場合
					if (m_last_active_btn_state == TNM_BTN_STATE_PUSH)	{

						m_active_btn_guid = btn->get_guid();		// アクティブなボタンのGUID
						m_active_btn_sorter = btn->m_sorter;		// アクティブなボタンのソーター
						m_active_btn_state = TNM_BTN_STATE_PUSH;	// 決定状態
						m_decided_this_frame = true;				// 今回決定された
						btn->m_is_decided = true;					// 決定された
						end = true;									// 処理を行った
					}

					break;							// ループアウト
				}
			}
		}

		// ２－２．まだ誰もアクティブになっていない場合、アクティブなボタンを判定する
		if (!end && m_active_btn_guid == -1)	{

			// マウスが画面内に入っている場合のみボタンに当たる
			if (0 <= mouse_pos.x && mouse_pos.x < Gp_global->game_screen_size.cx && 0 <= mouse_pos.y && mouse_pos.y < Gp_global->game_screen_size.cy)
			{
				// 全ボタンに対して処理を行う
				for (int b = 0; b < (int)m_btn_list.size(); b++)	{
					C_tnm_btn* btn = m_btn_list[b];

					// 当たっている
					if (btn->hit_test(mouse_pos) && Gp_global->active_flag)	{

						// 最初のヒットもしくは現在のヒットより上のソーターにある
						if (m_active_btn_guid == -1 || btn->m_sorter >= m_active_btn_sorter)	{		// 後優先！ '>' ではなく '>='

							m_active_btn_guid = btn->get_guid();	// アクティブなボタンのGUID
							m_active_btn_sorter = btn->m_sorter;	// アクティブなボタンのソーター
						}
					}
				}
			}

			// ここまででアクティブなボタンが決定したはず
			if (m_active_btn_guid >= 0)	{

				// ボタンを押した場合
				if (tnm_input_check_key_down(VK_EX_DECIDE))	{

					// アクティブなボタンのステートを変更
					m_active_btn_state = TNM_BTN_STATE_PUSH;

					// 今回押した判定
					if (m_last_active_btn_guid != m_active_btn_guid || m_last_active_btn_state != m_active_btn_state || tnm_input_check_key_down(VK_EX_DECIDE))	{
						m_pushed_this_frame = true;				// 今回押したフラグ
						tnm_input_use_key_down(VK_EX_DECIDE);	// 入力を消費
						end = true;								// 処理を行った
					}
				}

				// ボタンを押していない場合
				else	{

					// アクティブなボタンのステートを変更
					m_active_btn_state = TNM_BTN_STATE_HIT;

					// 今回ヒットした判定
					if (m_last_active_btn_guid != m_active_btn_guid || m_last_active_btn_state != m_active_btn_state)	{
						m_hit_this_frame = true;		// 今回当たったフラグ
						end = true;						// 処理を行った
					}
				}

			}
		}
	}

	// ステートを保持しておく
	m_last_active_btn_guid = m_active_btn_guid;
	m_last_active_btn_sorter = m_active_btn_sorter;
	m_last_active_btn_state = m_active_btn_state;
	m_last_mouse_push_flag = mouse_push_flag;

	return true;
}

