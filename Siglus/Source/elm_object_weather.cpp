#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_global_data.h"
#include	"element/elm_object.h"
#include	"engine/eng_graphics.h"

static int	APPEAR_TIME_LEN = 1000;
static int	DISAPPEAR_TIME_LEN = 1000;
static int	CHANGE_TIME_LEN = 3000;		// 切り替わりにかける時間
static int	SCREEN_RATE = 1;			// 仮想画面サイズ（結局等倍になりました… 2以上にすると、cnt = 1 のときに複数表示されちゃうので…）


// ****************************************************************
// オブジェクト：環境パラメータの初期化
// ================================================================
void C_elm_object::init_weather_param()
{
	m_op.weather.type = TNM_OBJECT_WEATHER_TYPE_NONE;
	m_op.weather.cnt = 0;
	m_op.weather.pat_mode = 0;
	m_op.weather.pat_no_00 = 0;
	m_op.weather.pat_no_01 = 0;
	m_op.weather.pat_time = 0;
	m_op.weather.move_time_x = 0;
	m_op.weather.move_time_y = 0;
	m_op.weather.sin_time_x = 0;
	m_op.weather.sin_time_y = 0;
	m_op.weather.sin_power_x = 0;
	m_op.weather.sin_power_y = 0;
	m_op.weather.center_x = 0;
	m_op.weather.center_y = 0;
	m_op.weather.center_rotate = 0;
	m_op.weather.appear_range = 0;
	m_op.weather.zoom_min = 0;
	m_op.weather.zoom_max = 0;
	m_op.weather.scale_x = 0;
	m_op.weather.scale_y = 0;
	m_op.weather.active_time = 0;
	m_op.weather.real_time_flag = false;

	// 作業パラメータ
	m_weather.cnt_max = 0;
}

// ****************************************************************
// オブジェクト：環境オブジェクトの作成
// ================================================================
bool C_elm_object::create_weather(TSTR file_path)
{
	// タイプ別のパラメータを初期化する
	init_type(true);

	// パラメータを設定
	m_op.type = TNM_OBJECT_TYPE_WEATHER;
	m_op.file_path = file_path;

	// 環境オブジェクトを再構築する
	if (!restruct_weather())
		return false;

	return true;
}

// ****************************************************************
// オブジェクト：環境パラメータの設定：タイプＡ
// ================================================================
bool C_elm_object::set_weather_param_type_a(int cnt, int pat_mode, int pat_no_00, int pat_no_01, int pat_time, int move_time_x, int move_time_y, int sin_time_x, int sin_power_x, int sin_time_y, int sin_power_y, int scale_x, int scale_y, int active_time, bool real_time)
{
	// 環境オブジェクトでなかったら何もしない
	if (m_op.type != TNM_OBJECT_TYPE_WEATHER)
		return true;

	// 徐々に環境を変更させるため、タイプを解放してはいけない

	// パラメータを設定
	m_op.weather.type = TNM_OBJECT_WEATHER_TYPE_A;
	m_op.weather.cnt = cnt * SCREEN_RATE * SCREEN_RATE;
	m_op.weather.pat_mode = pat_mode;
	m_op.weather.pat_no_00 = pat_no_00;
	m_op.weather.pat_no_01 = pat_no_01;
	m_op.weather.pat_time = pat_time;
	m_op.weather.move_time_x = move_time_x;
	m_op.weather.move_time_y = move_time_y;
	m_op.weather.sin_time_x = sin_time_x;
	m_op.weather.sin_time_y = sin_time_y;
	m_op.weather.sin_power_x = sin_power_x;
	m_op.weather.sin_power_y = sin_power_y;
	m_op.weather.scale_x = scale_x;
	m_op.weather.scale_y = scale_y;
	m_op.weather.active_time = active_time;
	m_op.weather.real_time_flag = real_time;

	// 環境オブジェクトを再構築する
	if (!restruct_weather())
		return false;

	return true;
}

// ****************************************************************
// オブジェクト：環境パラメータの設定：タイプＢ
// ================================================================
bool C_elm_object::set_weather_param_type_b(int cnt, int pat_mode, int pat_no_00, int pat_no_01, int pat_time, int center_x, int center_y, int appear_range, int move_time, int sin_time_x, int sin_power_x, int sin_time_y, int sin_power_y, int center_rotate, int zoom_min, int zoom_max, int scale_x, int scale_y, bool real_time)
{
	// 環境オブジェクトでなかったら何もしない
	if (m_op.type != TNM_OBJECT_TYPE_WEATHER)
		return true;

	// 徐々に環境を変更させるため、タイプを解放してはいけない

	// パラメータを設定
	m_op.weather.type = TNM_OBJECT_WEATHER_TYPE_B;
	m_op.weather.cnt = cnt;
	m_op.weather.pat_mode = pat_mode;
	m_op.weather.pat_no_00 = pat_no_00;
	m_op.weather.pat_no_01 = pat_no_01;
	m_op.weather.pat_time = pat_time;
	m_op.weather.center_x = center_x;
	m_op.weather.center_y = center_y;
	m_op.weather.appear_range = appear_range;
	m_op.weather.move_time_x = move_time;
	m_op.weather.move_time_y = move_time;
	m_op.weather.sin_time_x = sin_time_x;
	m_op.weather.sin_time_y = sin_time_y;
	m_op.weather.sin_power_x = sin_power_x;
	m_op.weather.sin_power_y = sin_power_y;
	m_op.weather.center_rotate = center_rotate;
	m_op.weather.zoom_min = zoom_min;
	m_op.weather.zoom_max = zoom_max;
	m_op.weather.scale_x = scale_x;
	m_op.weather.scale_y = scale_y;
	m_op.weather.real_time_flag = real_time;

	// 環境オブジェクトを再構築する
	if (!restruct_weather())
		return false;

	return true;
}

// ****************************************************************
// オブジェクト：環境オブジェクトを再構築
// ================================================================
bool C_elm_object::restruct_weather()
{
	int i;

	// 環境オブジェクトでなければ何もしない
	if (m_op.type != TNM_OBJECT_TYPE_WEATHER)
		return true;

	// テクスチャを読み込む
	m_album = tnm_load_pct_d3d(m_op.file_path, false, m_op_def.set_cg_table);
	if (!m_album)	{
		m_op.file_path.clear();		// 失敗したのでファイル名をクリア
		return false;
	}

	// 現在の粒に関して再構築フラグを立てる
	for (int i = 0; i < (int)m_weather.sub.size(); i++)	{
		m_weather.sub[i].restruct_flag = true;
	}

	// 作業パラメータを準備する
	int old_cnt = (int)m_weather.sub.size();
	if (m_op.weather.cnt > old_cnt)	{
		m_weather.sub.resize(m_op.weather.cnt);
		for (i = old_cnt; i < m_op.weather.cnt; i++)	{
			setup_weather_param(i, 0);		// init から
		}
	}

	// 作業パラメータの最大値を更新
	if (m_op.weather.cnt > m_weather.cnt_max)	{
		m_weather.cnt_max = m_op.weather.cnt;
	}

	// スプライトの個数
	int sprite_cnt;
	switch (m_op.weather.type)	{
		case TNM_OBJECT_WEATHER_TYPE_A:		sprite_cnt = m_weather.cnt_max * 4;		break;		// 上下左右分
		case TNM_OBJECT_WEATHER_TYPE_B:		sprite_cnt = m_weather.cnt_max;			break;
		default:							sprite_cnt = 0;							break;
	}

	// スプライトを準備する
	// 個数は減らさない！！急に減ってしまうのを防ぐため。
	int old_sprite_cnt = (int)m_sprite_list.size();
	if (sprite_cnt > old_sprite_cnt)	{
		m_sprite_list.resize(sprite_cnt);
		for (i = old_sprite_cnt; i < sprite_cnt; i++)	{
			if (!m_sprite_list[i].create_polygon_no_init())
				return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("環境オブジェクトの作成に失敗しました。"));

			m_sprite_list[i].set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
			m_sprite_list[i].set_index_buffer(G_rect_index_buffer);
		}
	}

	return true;
}

// ****************************************************************
// オブジェクト：環境パラメータをセットアップ
// ================================================================
void C_elm_object::setup_weather_param(int sub_no, int init_state)
{
	C_elm_object_work_param_weather_sub* p_sub = &m_weather.sub[sub_no];
	C_size scr_size = Gp_global->game_screen_size;

	if (m_op.weather.type == TNM_OBJECT_WEATHER_TYPE_A)	{

		p_sub->state = init_state;				// init sleep
		p_sub->move_start_pos_x = rand() % scr_size.cx * SCREEN_RATE;
		p_sub->move_start_pos_y = rand() % scr_size.cy * SCREEN_RATE;//init_state == 0 ? rand() % scr_size.cy : 0;
		p_sub->move_time_x = m_op.weather.move_time_x * (rand() % 20 + 90) / 100;		// ９０％～１１０％
		p_sub->move_time_y = m_op.weather.move_time_y * (rand() % 20 + 90) / 100;		// ９０％～１１０％
		p_sub->move_cur_time = rand();
		p_sub->sin_time_x = m_op.weather.sin_time_x * (rand() % 20 + 90) / 100;			// ９０％～１１０％
		p_sub->sin_time_y = m_op.weather.sin_time_y * (rand() % 20 + 90) / 100;			// ９０％～１１０％
		p_sub->sin_power_x = m_op.weather.sin_power_x * (rand() % 20 + 90) / 100;		// ９０％～１１０％
		p_sub->sin_power_y = m_op.weather.sin_power_y * (rand() % 20 + 90) / 100;		// ９０％～１１０％
		p_sub->sin_cur_time = rand();
		p_sub->scale_x = m_op.weather.scale_x;
		p_sub->scale_y = m_op.weather.scale_y;

		p_sub->active_time_len = m_op.weather.active_time * (rand() % 40 + 80) / 100;		// ８０％～１２０％
		p_sub->state_time_len = init_state == 0 ? rand() % CHANGE_TIME_LEN : p_sub->active_time_len;
		p_sub->state_cur_time = 0;
		p_sub->real_time_flag = m_op.weather.real_time_flag;
		p_sub->restruct_flag = false;
	}
	else if (m_op.weather.type == TNM_OBJECT_WEATHER_TYPE_B)	{

		int max_distance_x,	max_distance_y, max_distance;
		max_distance_x = m_op.weather.center_x > scr_size.cx / 2 ? m_op.weather.center_x : scr_size.cx - m_op.weather.center_x;
		max_distance_y = m_op.weather.center_y > scr_size.cy / 2 ? m_op.weather.center_y : scr_size.cy - m_op.weather.center_y;
		max_distance = (int)sqrt((double)max_distance_x * max_distance_x + (double)max_distance_y * max_distance_y);

		int appear_distance = rand() % max_distance * m_op.weather.appear_range / 100;
		double appear_radian = (double)rand() / RAND_MAX * D3DX_PI * 2;

		p_sub->state = init_state;				// init sleep
		p_sub->move_start_distance = appear_distance;
		p_sub->move_start_degree = (int)(appear_radian / D3DX_PI * 1800);
		p_sub->move_time_x = abs(m_op.weather.move_time_x) * (rand() % 40 + 80) / 100;	// ８０％～１２０％
		p_sub->move_time_y = abs(m_op.weather.move_time_y) * (rand() % 40 + 80) / 100;	// ８０％～１２０％
		p_sub->sin_time_x = m_op.weather.sin_time_x * (rand() % 20 + 90) / 100;			// ９０％～１１０％
		p_sub->sin_time_y = m_op.weather.sin_time_y * (rand() % 20 + 90) / 100;			// ９０％～１１０％
		p_sub->sin_power_x = m_op.weather.sin_power_x * (rand() % 20 + 90) / 100;		// ９０％～１１０％
		p_sub->sin_power_y = m_op.weather.sin_power_y * (rand() % 20 + 90) / 100;		// ９０％～１１０％
		p_sub->sin_cur_time = rand();
		p_sub->center_rotate = m_op.weather.center_rotate * (rand() % 20 + 90) / 100;	// ９０％～１１０％
		p_sub->zoom_min = m_op.weather.zoom_min;
		p_sub->zoom_max = m_op.weather.zoom_max;
		p_sub->scale_x = m_op.weather.scale_x;
		p_sub->scale_y = m_op.weather.scale_y;

		p_sub->active_time_len = abs(m_op.weather.move_time_x) * max_distance / 1000 - APPEAR_TIME_LEN;
		p_sub->state_time_len = init_state == 0 ? rand() % (APPEAR_TIME_LEN + p_sub->active_time_len + DISAPPEAR_TIME_LEN) : p_sub->active_time_len;
		p_sub->state_cur_time = 0;
		p_sub->move_cur_time = p_sub->state_time_len == 0 ? 0 : rand() % p_sub->state_time_len;
		p_sub->real_time_flag = m_op.weather.real_time_flag;
		p_sub->restruct_flag = false;
	}
}

// ****************************************************************
// オブジェクト：環境オブジェクトの時間を進める
// ================================================================
void C_elm_object::update_weather_time(int past_game_time, int past_real_time)
{
	int i;

	if (m_op.type == TNM_OBJECT_TYPE_WEATHER)	{

		for (i = 0; i < m_weather.cnt_max; i++)	{
			C_elm_object_work_param_weather_sub* p_sub = &m_weather.sub[i];

			// タイプＡ・タイプＢ
			if (m_op.weather.type == TNM_OBJECT_WEATHER_TYPE_A || m_op.weather.type == TNM_OBJECT_WEATHER_TYPE_B)	{

				// 完全消滅した粒に対しては何もしない
				if (!(i >= m_op.weather.cnt && p_sub->state == 0))	{
					int past_time = (p_sub->real_time_flag ? past_real_time : past_game_time);

					// 時間を進める
					p_sub->state_cur_time += past_time;
					p_sub->move_cur_time += past_time;
					p_sub->sin_cur_time += past_time;

					// 再構築するべきなのに state_time_len が３秒以上残ってる粒は３秒未満に設定
					if (i >= m_op.weather.cnt || p_sub->restruct_flag)	{
						if (p_sub->state == 2 && p_sub->state_time_len - p_sub->state_cur_time >= CHANGE_TIME_LEN)	{
							p_sub->state_cur_time = p_sub->state_time_len - rand() % CHANGE_TIME_LEN;
						}
					}
					// ステート時間が過ぎた
					while (p_sub->state_cur_time - p_sub->state_time_len > 0)	{
						int amari_time = p_sub->state_cur_time - p_sub->state_time_len;

						// 次のステートへ
						p_sub->state = (p_sub->state + 1) % 4;
						// sleep になれば再セットアップ
						if (p_sub->state == 0)	{
							if (i >= m_op.weather.cnt)	{
								break;	// 完全消滅
							}
							else	{
								setup_weather_param(i, 1);			// appear から
							}
						}
						// appear になった場合時間を初期化する
						if (p_sub->state == 1)	{
							p_sub->move_cur_time = amari_time;	// あまり時間を進める
						}
						// ステートの時間を設定
						switch (p_sub->state)	{
							case 1:		p_sub->state_time_len = APPEAR_TIME_LEN;		break;	// appear
							case 2:		p_sub->state_time_len = p_sub->active_time_len;	break;	// active
							case 3:		p_sub->state_time_len = DISAPPEAR_TIME_LEN;		break;	// disappear
						}
						// ステートの現在時間を補正する
						p_sub->state_cur_time = amari_time;
					}
				}
			}
		}
	}
}

// ****************************************************************
// オブジェクト：環境オブジェクトのフレーム処理
// ================================================================
#define		__MASHIRO_SUMMER	0

void C_elm_object::weather_frame()
{
	int i;

	C_size scr_size = Gp_global->game_screen_size;

	// 粒の設定
	for (i = 0; i < m_weather.cnt_max; i++)	{
		C_elm_object_work_param_weather_sub* p_sub = &m_weather.sub[i];

		// sleep 中は何もしない
		if (p_sub->state > 0)	{

			// パターン番号を決める
			int pat_no = 0;
			if (m_op.weather.pat_mode == 0)	{
				pat_no = m_op.weather.pat_no_00;
			}
			else if (m_op.weather.pat_mode == 1)	{	// ループ
				if (m_op.weather.pat_time > 0)	{
					int pat_time = (DWORD)p_sub->move_cur_time % m_op.weather.pat_time;
					int min = std::min(m_op.weather.pat_no_00, m_op.weather.pat_no_01);
					int max = std::max(m_op.weather.pat_no_00, m_op.weather.pat_no_01);
					pat_no = pat_time * (max - min + 1) / m_op.weather.pat_time + min;
				}
			}
			else if (m_op.weather.pat_mode == 2)	{
				int min = std::min(m_op.weather.pat_no_00, m_op.weather.pat_no_01);
				int max = std::max(m_op.weather.pat_no_00, m_op.weather.pat_no_01);
				pat_no = rand() % (max - min + 1) + min;
			}
			// テクスチャを設定する
			if (0 <= pat_no && pat_no < m_album->get_texture_cnt())	{
				int x = 0, y = 0, tr = 255, scale_x = TNM_SCALE_UNIT, scale_y = TNM_SCALE_UNIT;

				// 出現
				switch (p_sub->state)	{
					case 1:		tr = tr * linear_limit(p_sub->state_cur_time, 0, 0, p_sub->state_time_len, 255) / 255;	break;	// appear
					case 3:		tr = tr * linear_limit(p_sub->state_cur_time, 0, 255, p_sub->state_time_len, 0) / 255;	break;	// disappear
				}

				// タイプＡ
				if (m_op.weather.type == TNM_OBJECT_WEATHER_TYPE_A)	{

#if __MASHIRO_SUMMER
					// （★ましろサマー特殊）先にスプライト計算をしておく
					int sprite_no = i * 4;
					trp_to_rp(&m_sprite_list[sprite_no].rpex, &m_sprite_list[sprite_no].rp);
					x += (int)m_sprite_list[sprite_no].rp.pos.x;
					y += (int)m_sprite_list[sprite_no].rp.pos.y;
#endif
					// 初期位置
					x += p_sub->move_start_pos_x;
					y += p_sub->move_start_pos_y;

					// 移動
					if (p_sub->move_time_x != 0)	{
						x += (int)(1000.0f / p_sub->move_time_x * (DWORD)p_sub->move_cur_time);
					}
					if (p_sub->move_time_y != 0)	{
						y += (int)(1000.0f / p_sub->move_time_y * (DWORD)p_sub->move_cur_time);
					}

					// 揺れ
					if (p_sub->sin_time_x > 0 && p_sub->sin_power_x > 0)	{
						double sin_time = (double)((DWORD)p_sub->sin_cur_time % p_sub->sin_time_x);
						double radian = linear(sin_time, (double)0.0f, (double)0.0f, (double)p_sub->sin_time_x, (double)(2 * D3DX_PI));
						x += (int)(sin(radian) * p_sub->sin_power_x);
					}
					if (p_sub->sin_time_y > 0 && p_sub->sin_power_y > 0)	{
						double sin_time = (double)((DWORD)p_sub->sin_cur_time % p_sub->sin_time_y);
						double radian = linear(sin_time, (double)0.0f, (double)0.0f, (double)p_sub->sin_time_y, (double)(2 * D3DX_PI));
						y += (int)(sin(radian) * p_sub->sin_power_y);
					}

					// 周回補正
					x = x > 0 ? x % (scr_size.cx * SCREEN_RATE) : (scr_size.cx * SCREEN_RATE) - ((-x) % (scr_size.cx * SCREEN_RATE));
					y = y > 0 ? y % (scr_size.cy * SCREEN_RATE) : (scr_size.cy * SCREEN_RATE) - ((-y) % (scr_size.cy * SCREEN_RATE));

					// スケール
					scale_x = p_sub->scale_x;
					scale_y = p_sub->scale_y;

					// 端にかかった場合はスプライトを増やす
					bool over_l = false, over_r = false, over_u = false, over_d = false;
					int pct_w = m_album->get_texture(pat_no)->get_width();
					int pct_h = m_album->get_texture(pat_no)->get_height();
					int center_x = m_album->get_texture(pat_no)->get_center().x;
					int center_y = m_album->get_texture(pat_no)->get_center().y;
					if (x - center_x * scale_x / TNM_SCALE_UNIT < 0)
						over_l = true;
					if (x + (pct_w - center_x) * scale_x / TNM_SCALE_UNIT >= scr_size.cx * SCREEN_RATE)
						over_r = true;
					if (y - center_y * scale_y / TNM_SCALE_UNIT < 0)
						over_u = true;
					if (y + (pct_h - center_y) * scale_y / TNM_SCALE_UNIT >= scr_size.cy * SCREEN_RATE)
						over_d = true;

#if __MASHIRO_SUMMER
					// スプライトに設定
					m_sprite_list[sprite_no].set_texture(m_album->get_texture(pat_no));
					m_sprite_list[sprite_no].rp.size_fit_to_texture = 0;
					m_sprite_list[sprite_no].rp.pos.x = (float)x;		// ★代入！
					m_sprite_list[sprite_no].rp.pos.y = (float)y;		// ★代入！
					m_sprite_list[sprite_no].rp.scale.x *= (float)scale_x / TNM_SCALE_UNIT;
					m_sprite_list[sprite_no].rp.scale.y *= (float)scale_y / TNM_SCALE_UNIT;
					m_sprite_list[sprite_no].rp.tr = m_sprite_list[sprite_no].rp.tr * tr / 255;
					if (over_l || over_r)	{	// 左右にはみ出した
						trp_to_rp(&m_sprite_list[sprite_no + 1].rpex, &m_sprite_list[sprite_no + 1].rp);
						m_sprite_list[sprite_no + 1].set_texture(m_album->get_texture(pat_no));
						m_sprite_list[sprite_no + 1].rp.size_fit_to_texture = 0;
						m_sprite_list[sprite_no + 1].rp.pos.x = (float)x + (over_l ? scr_size.cx : - scr_size.cx);		// ★代入！
						m_sprite_list[sprite_no + 1].rp.pos.y = (float)y;												// ★代入！
						m_sprite_list[sprite_no + 1].rp.scale.x *= (float)scale_x / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 1].rp.scale.y *= (float)scale_y / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 1].rp.tr = m_sprite_list[sprite_no + 1].rp.tr * tr / 255;
					}
					if (over_u || over_d)	{	// 上下にはみ出した
						trp_to_rp(&m_sprite_list[sprite_no + 2].rpex, &m_sprite_list[sprite_no + 2].rp);
						m_sprite_list[sprite_no + 2].set_texture(m_album->get_texture(pat_no));
						m_sprite_list[sprite_no + 2].rp.size_fit_to_texture = 0;
						m_sprite_list[sprite_no + 2].rp.pos.x = (float)x;												// ★代入！
						m_sprite_list[sprite_no + 2].rp.pos.y = (float)y + (over_u ? scr_size.cy : - scr_size.cy);		// ★代入！
						m_sprite_list[sprite_no + 2].rp.scale.x *= (float)scale_x / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 2].rp.scale.y *= (float)scale_y / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 2].rp.tr = m_sprite_list[sprite_no + 2].rp.tr * tr / 255;
					}
					if ((over_l || over_r) && (over_u || over_d))	{	// 上下にも左右にもはみ出した
						trp_to_rp(&m_sprite_list[sprite_no + 3].rpex, &m_sprite_list[sprite_no + 3].rp);
						m_sprite_list[sprite_no + 3].set_texture(m_album->get_texture(pat_no));
						m_sprite_list[sprite_no + 3].rp.size_fit_to_texture = 0;
						m_sprite_list[sprite_no + 3].rp.pos.x = (float)x + (over_l ? scr_size.cx : - scr_size.cx);		// ★代入！
						m_sprite_list[sprite_no + 3].rp.pos.y = (float)y + (over_u ? scr_size.cy : - scr_size.cy);		// ★代入！
						m_sprite_list[sprite_no + 3].rp.scale.x *= (float)scale_x / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 3].rp.scale.y *= (float)scale_y / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 3].rp.tr = m_sprite_list[sprite_no + 3].rp.tr * tr / 255;
					}
#else
					// スプライトに設定
					int sprite_no = i * 4;
					trp_to_rp(&m_sprite_list[sprite_no].rpex, &m_sprite_list[sprite_no].rp);
					m_sprite_list[sprite_no].set_texture(m_album->get_texture(pat_no));
					m_sprite_list[sprite_no].rp.size_fit_to_texture = 0;
					m_sprite_list[sprite_no].rp.pos.x += (float)x;
					m_sprite_list[sprite_no].rp.pos.y += (float)y;
					m_sprite_list[sprite_no].rp.scale.x *= (float)scale_x / TNM_SCALE_UNIT;
					m_sprite_list[sprite_no].rp.scale.y *= (float)scale_y / TNM_SCALE_UNIT;
					m_sprite_list[sprite_no].rp.tr = m_sprite_list[sprite_no].rp.tr * tr / 255;
					if (over_l || over_r)	{	// 左右にはみ出した
						trp_to_rp(&m_sprite_list[sprite_no + 1].rpex, &m_sprite_list[sprite_no + 1].rp);
						m_sprite_list[sprite_no + 1].set_texture(m_album->get_texture(pat_no));
						m_sprite_list[sprite_no + 1].rp.size_fit_to_texture = 0;
						m_sprite_list[sprite_no + 1].rp.pos.x += (float)x + (over_l ? scr_size.cx * SCREEN_RATE : -scr_size.cx * SCREEN_RATE);
						m_sprite_list[sprite_no + 1].rp.pos.y += (float)y;
						m_sprite_list[sprite_no + 1].rp.scale.x *= (float)scale_x / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 1].rp.scale.y *= (float)scale_y / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 1].rp.tr = m_sprite_list[sprite_no + 1].rp.tr * tr / 255;
					}
					if (over_u || over_d)	{	// 上下にはみ出した
						trp_to_rp(&m_sprite_list[sprite_no + 2].rpex, &m_sprite_list[sprite_no + 2].rp);
						m_sprite_list[sprite_no + 2].set_texture(m_album->get_texture(pat_no));
						m_sprite_list[sprite_no + 2].rp.size_fit_to_texture = 0;
						m_sprite_list[sprite_no + 2].rp.pos.x += (float)x;
						m_sprite_list[sprite_no + 2].rp.pos.y += (float)y + (over_u ? scr_size.cy * SCREEN_RATE : -scr_size.cy * SCREEN_RATE);
						m_sprite_list[sprite_no + 2].rp.scale.x *= (float)scale_x / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 2].rp.scale.y *= (float)scale_y / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 2].rp.tr = m_sprite_list[sprite_no + 2].rp.tr * tr / 255;
					}
					if ((over_l || over_r) && (over_u || over_d))	{	// 上下にも左右にもはみ出した
						trp_to_rp(&m_sprite_list[sprite_no + 3].rpex, &m_sprite_list[sprite_no + 3].rp);
						m_sprite_list[sprite_no + 3].set_texture(m_album->get_texture(pat_no));
						m_sprite_list[sprite_no + 3].rp.size_fit_to_texture = 0;
						m_sprite_list[sprite_no + 3].rp.pos.x += (float)x + (over_l ? scr_size.cx * SCREEN_RATE : -scr_size.cx * SCREEN_RATE);
						m_sprite_list[sprite_no + 3].rp.pos.y += (float)y + (over_u ? scr_size.cy * SCREEN_RATE : -scr_size.cy * SCREEN_RATE);
						m_sprite_list[sprite_no + 3].rp.scale.x *= (float)scale_x / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 3].rp.scale.y *= (float)scale_y / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 3].rp.tr = m_sprite_list[sprite_no + 3].rp.tr * tr / 255;
					}
#endif
				}
				// タイプＢ
				else if (m_op.weather.type == TNM_OBJECT_WEATHER_TYPE_B)	{

					// 合計時間
					DWORD total_time = (DWORD)(APPEAR_TIME_LEN + p_sub->active_time_len + DISAPPEAR_TIME_LEN);
					DWORD move_cur_time = p_sub->move_time_x > 0 ? (DWORD)p_sub->move_cur_time : total_time - (DWORD)p_sub->move_cur_time;

					// 補正位置
					double rep_x = 0, rep_y = 0;

					// 初期位置
					rep_x = p_sub->move_start_distance;

					// 移動（Ｘに移動時間が入ってる）
					if (p_sub->move_time_x != 0)	{
						rep_x += 1000.0f / p_sub->move_time_x / p_sub->move_time_x * move_cur_time * move_cur_time;
					}

					// 揺れ
					if (p_sub->sin_time_x > 0 && p_sub->sin_power_x > 0)	{
						double sin_time = (double)(p_sub->sin_cur_time % p_sub->sin_time_x);
						double radian = linear(sin_time, (double)0.0f, (double)0.0f, (double)p_sub->sin_time_x, (double)(2 * D3DX_PI));
						rep_y += sin(radian) * p_sub->sin_power_x;		// x と y 逆になる点に注意（進行方向が y）
					}
					if (p_sub->sin_time_y > 0 && p_sub->sin_power_y > 0)	{
						double sin_time = (double)(p_sub->sin_cur_time % p_sub->sin_time_y);
						double radian = linear(sin_time, (double)0.0f, (double)0.0f, (double)p_sub->sin_time_y, (double)(2 * D3DX_PI));
						rep_x += sin(radian) * p_sub->sin_power_y;		// x と y 逆になる点に注意（進行方向が y）
					}

					// 角度を求めておく
					double radian = D3DXToRadian((double)p_sub->move_start_degree / TNM_ANGLE_UNIT);

					// 中心回転
					double theta = rep_x * ((double)p_sub->center_rotate / TNM_ANGLE_UNIT) / 1000;
					radian += D3DXToRadian(theta);

					// 回転
					double cos_value = cos(radian);
					double sin_value = sin(radian);
					x += (int)(rep_x * cos_value - rep_y * sin_value);
					y += (int)(rep_x * sin_value + rep_y * cos_value);

					// 位置決定
					x += m_op.weather.center_x;
					y += m_op.weather.center_y;

					// 進行具合を計算（0～1000）
					int process = (int)((double)move_cur_time * 1000 / total_time);

					// ズーム
					scale_x = linear(process, 0, p_sub->zoom_min, 1000, p_sub->zoom_max);
					scale_y = linear(process, 0, p_sub->zoom_min, 1000, p_sub->zoom_max);

					// スプライトに設定
					int sprite_no = i;
					trp_to_rp(&m_sprite_list[sprite_no].rpex, &m_sprite_list[sprite_no].rp);
					m_sprite_list[sprite_no].set_texture(m_album->get_texture(pat_no));
					m_sprite_list[sprite_no].rp.size_fit_to_texture = 0;
					m_sprite_list[sprite_no].rp.pos.x += (float)x;
					m_sprite_list[sprite_no].rp.pos.y += (float)y;
					m_sprite_list[sprite_no].rp.scale.x *= (float)scale_x / TNM_SCALE_UNIT;
					m_sprite_list[sprite_no].rp.scale.y *= (float)scale_y / TNM_SCALE_UNIT;
					m_sprite_list[sprite_no].rp.tr = m_sprite_list[sprite_no].rp.tr * tr / 255;
				}
			}
		}
	}
}


