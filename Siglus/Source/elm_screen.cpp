#include	"pch.h"

#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_tool.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_screen.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// スクリーン：初期化
// ================================================================
void C_elm_screen::init(S_element element, CTSTR& name)
{
	// 基底クラスの初期化
	C_tnm_element::init(element, FM_SCREEN, name, NULL);

	// エフェクトを初期化
	m_effect_list.init(element + S_element(ELM_SCREEN_EFFECT), FM_EFFECTLIST, name + _T(".effect"), Gp_ini->effect_cnt, false, NULL);

	// クエイクを初期化
	m_quake_list.init(element + S_element(ELM_SCREEN_QUAKE), FM_QUAKELIST, name + _T(".quake"), Gp_ini->quake_cnt, false, NULL);

	// 再初期化
	reinit();
}

// ****************************************************************
// スクリーン：再初期化
// ================================================================
void C_elm_screen::reinit()
{
	// エフェクト
	m_effect_list.reinit();

	// シェイク
	m_shake.init();

	// クエイク
	m_quake_list.reinit();
}

// ****************************************************************
// スクリーン：セーブ
// ================================================================
void C_elm_screen::save(C_tnm_save_stream& stream)
{
	// エフェクト
	m_effect_list.save(stream);

	// シェイク
	stream.save(m_shake);

	// クエイク
	m_quake_list.save(stream);
}

// ****************************************************************
// スクリーン：ロード
// ================================================================
void C_elm_screen::load(C_tnm_save_stream& stream)
{
	// エフェクト
	m_effect_list.load(stream);

	// シェイク
	stream.load(m_shake);

	// クエイク
	m_quake_list.load(stream);
}

// ****************************************************************
// スクリーン：時間を進める
// ================================================================
void C_elm_screen::update_time(int past_game_time, int past_real_time)
{
	// エフェクト
	for (ARRAY<C_elm_effect>::iterator itr_effect = m_effect_list.get_sub().begin(); itr_effect != m_effect_list.get_sub().end(); ++itr_effect)
		itr_effect->update_time(past_game_time, past_real_time);

	// シェイク
	m_shake.update_time(past_game_time);

	// クエイク
	for (ARRAY<C_elm_quake>::iterator itr_quake = m_quake_list.get_sub().begin(); itr_quake != m_quake_list.get_sub().end(); ++itr_quake)
		itr_quake->update_time(past_game_time);
}

// ****************************************************************
// スクリーン：フレーム処理
// ================================================================
void C_elm_screen::frame()
{
	// エフェクト
	for (ARRAY<C_elm_effect>::iterator itr_effect = m_effect_list.get_sub().begin(); itr_effect != m_effect_list.get_sub().end(); ++itr_effect)
		itr_effect->frame();

	// シェイク
	m_shake.frame();

	// クエイク
	for (ARRAY<C_elm_quake>::iterator itr_quake = m_quake_list.get_sub().begin(); itr_quake != m_quake_list.get_sub().end(); ++itr_quake)
		itr_quake->frame();
}

// ****************************************************************
// エフェクト：初期化
// ================================================================
void C_elm_effect::init(S_element element, CTSTR& name)
{
	// 基底クラスの初期化
	C_tnm_element::init(element, FM_EFFECT, name, NULL);

	// 再初期化
	reinit();
}

// ****************************************************************
// エフェクト：再初期化
// ================================================================
void C_elm_effect::reinit()
{
	// パラメータ
	m_param.x.init(0);
	m_param.y.init(0);
	m_param.z.init(0);
	m_param.mono.init(0);
	m_param.reverse.init(0);
	m_param.bright.init(0);
	m_param.dark.init(0);
	m_param.color_r.init(0);
	m_param.color_g.init(0);
	m_param.color_b.init(0);
	m_param.color_rate.init(0);
	m_param.color_add_r.init(0);
	m_param.color_add_g.init(0);
	m_param.color_add_b.init(0);

	m_param.begin_sorter.order = 0;
	m_param.end_sorter.order = 0;
	m_param.begin_sorter.layer = INT_MIN;
	m_param.end_sorter.layer = INT_MAX;
	m_param.wipe_copy = 0;
	m_param.wipe_erase = 0;
}

// ****************************************************************
// エフェクト：セーブ
// ================================================================
void C_elm_effect::save(C_tnm_save_stream& stream)
{
	// パラメータ
	stream.save(m_param);
}

// ****************************************************************
// エフェクト：ロード
// ================================================================
void C_elm_effect::load(C_tnm_save_stream& stream)
{
	// パラメータ
	stream.load(m_param);
}

// ****************************************************************
// エフェクト：コピー
// ================================================================
void C_elm_effect::copy(const C_elm_effect* p_src)
{
	// パラメータ
	m_param = p_src->m_param;
}

// ****************************************************************
// エフェクト：時間を進める
// ================================================================
void C_elm_effect::update_time(int past_game_time, int past_real_time)
{
	m_param.x.update_time(past_game_time, past_real_time);
	m_param.y.update_time(past_game_time, past_real_time);
	m_param.z.update_time(past_game_time, past_real_time);
	m_param.mono.update_time(past_game_time, past_real_time);
	m_param.reverse.update_time(past_game_time, past_real_time);
	m_param.bright.update_time(past_game_time, past_real_time);
	m_param.dark.update_time(past_game_time, past_real_time);
	m_param.color_r.update_time(past_game_time, past_real_time);
	m_param.color_g.update_time(past_game_time, past_real_time);
	m_param.color_b.update_time(past_game_time, past_real_time);
	m_param.color_rate.update_time(past_game_time, past_real_time);
	m_param.color_add_r.update_time(past_game_time, past_real_time);
	m_param.color_add_g.update_time(past_game_time, past_real_time);
	m_param.color_add_b.update_time(past_game_time, past_real_time);
}

// ****************************************************************
// エフェクト：フレーム処理
// ================================================================
void C_elm_effect::frame()
{
	m_param.x.frame();
	m_param.y.frame();
	m_param.z.frame();
	m_param.mono.frame();
	m_param.reverse.frame();
	m_param.bright.frame();
	m_param.dark.frame();
	m_param.color_r.frame();
	m_param.color_g.frame();
	m_param.color_b.frame();
	m_param.color_rate.frame();
	m_param.color_add_r.frame();
	m_param.color_add_g.frame();
	m_param.color_add_b.frame();
}

// ****************************************************************
// エフェクト：レンダーデータを取得
// ================================================================
bool C_elm_effect::is_use()
{
	if (m_param.x.get_total_value() != 0)			return true;
	if (m_param.y.get_total_value() != 0)			return true;
	if (m_param.z.get_total_value() != 0)			return true;
	if (m_param.mono.get_total_value() != 0)		return true;
	if (m_param.reverse.get_total_value() != 0)		return true;
	if (m_param.bright.get_total_value() != 0)		return true;
	if (m_param.dark.get_total_value() != 0)		return true;
	if (m_param.color_r.get_total_value() != 0)		return true;
	if (m_param.color_g.get_total_value() != 0)		return true;
	if (m_param.color_b.get_total_value() != 0)		return true;
	if (m_param.color_rate.get_total_value() != 0)	return true;
	if (m_param.color_add_r.get_total_value() != 0)	return true;
	if (m_param.color_add_g.get_total_value() != 0)	return true;
	if (m_param.color_add_b.get_total_value() != 0)	return true;

	return false;
}

void C_elm_effect::get_rp(S_d3d_render_param* p_rp)
{
	p_rp->disp = true;
	p_rp->pos.x = (float)m_param.x.get_total_value();
	p_rp->pos.y = (float)m_param.y.get_total_value();
	p_rp->pos.z = (float)m_param.z.get_total_value();
	p_rp->mono = m_param.mono.get_total_value();
	p_rp->reverse = m_param.reverse.get_total_value();
	p_rp->bright = m_param.bright.get_total_value();
	p_rp->dark = m_param.dark.get_total_value();
	p_rp->color_r = m_param.color_r.get_total_value();
	p_rp->color_g = m_param.color_g.get_total_value();
	p_rp->color_b = m_param.color_b.get_total_value();
	p_rp->color_rate = m_param.color_rate.get_total_value();
	p_rp->color_add_r = m_param.color_add_r.get_total_value();
	p_rp->color_add_g = m_param.color_add_g.get_total_value();
	p_rp->color_add_b = m_param.color_add_b.get_total_value();
}

// ****************************************************************
// エフェクトリスト：サブの初期化
// ================================================================
void C_elm_effect_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"));
}

// ****************************************************************
// エフェクトリスト：サブの再初期化
// ================================================================
void C_elm_effect_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// エフェクトリスト：サブのセーブ
// ================================================================
void C_elm_effect_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// エフェクトリスト：サブのロード
// ================================================================
void C_elm_effect_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}

// ****************************************************************
// シェイク：初期化
// ================================================================
void S_tnm_shake::init()
{
	m_no = -1;
	m_cur_time = 0;

	// 作業値
	m_cur_shake_x = 0;
	m_cur_shake_y = 0;
}

// ****************************************************************
// シェイク：コピーを作成
// ================================================================
void S_tnm_shake::copy(const S_tnm_shake* src)
{
	m_no = src->m_no;
	m_cur_time = src->m_cur_time;
}

// ****************************************************************
// シェイク：開始
// ================================================================
void S_tnm_shake::start(int shake_no, bool wait_flag)
{
	if (shake_no < 0 || Gp_ini->shake_cnt <= shake_no)
		return;

	// パラメータを設定
	m_no = shake_no;
	m_cur_time = 0;

	// 待ち状態に入る
	if (wait_flag)	{
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_SHAKE_WAIT;
		tnm_push_proc(proc);
	}
}

// ****************************************************************
// シェイク：終了
// ================================================================
void S_tnm_shake::end()
{
	m_no = -1;
}

// ****************************************************************
// シェイク：時間を進める
// ================================================================
void S_tnm_shake::update_time(int past_time)
{
	// 時間を進める
	m_cur_time += past_time;
}

// ****************************************************************
// シェイク：フレーム処理
// ================================================================
void S_tnm_shake::frame()
{
	// 範囲外補正
	if (m_no < 0 || Gp_ini->shake_cnt <= m_no)
		m_no = -1;

	// シェイク中の処理
	if (m_no >= 0)	{

		// ini の情報を取得しておく
		C_tnm_ini::Sshake* p_ini = &Gp_ini->shake[m_no];

		// 現在いるピリオドを求める
		int total_time = 0, cur_period = -1, period_time = 0;
		for (int p = 0; p < (int)p_ini->sub.size(); p++)	{
			period_time = m_cur_time - total_time;	// ピリオド内での時間
			if (period_time < p_ini->sub[p].time)	{
				cur_period = p;
				break;
			}
			total_time += p_ini->sub[p].time;
		}

		// 合計時間を過ぎていたらシェイク終了
		if (cur_period == -1)	{
			end();
		}
		else	{

			// シェイク座標を求める
			m_cur_shake_x = p_ini->sub[cur_period].x;
			m_cur_shake_y = p_ini->sub[cur_period].y;
		}
	}

	// シェイク終了ならパラメータを初期化
	if (m_no == -1)
		init();
}

// ****************************************************************
// クエイク：初期化
// ================================================================
void C_elm_quake::init(S_element element, CTSTR& name)
{
	// 基底クラスの初期化
	C_tnm_element::init(element, FM_QUAKE, name, NULL);

	// 再初期化
	reinit();
}

// ****************************************************************
// クエイク：再初期化
// ================================================================
void C_elm_quake::reinit()
{
	// 通常パラメータ
	m_param.type = TNM_QUAKE_TYPE_NONE;
	m_param.vec = TNM_QUAKE_VEC_U;
	m_param.power = 0;
	m_param.cur_time = 0;
	m_param.total_time = 0;
	m_param.end_flag = 0;
	m_param.end_cur_time = 0;
	m_param.end_total_time = 0;
	m_param.cnt = 0;
	m_param.end_cnt = 0;
	m_param.center_x = 0;
	m_param.center_y = 0;
	m_param.begin_order = 0;
	m_param.end_order = 0;

	// 一時パラメータ
	m_work.cur_pos_x = 0;
	m_work.cur_pos_y = 0;
	m_work.cur_scale = TNM_SCALE_UNIT;
	m_work.cur_rotate = 0;
}

// ****************************************************************
// クエイク：セーブ
// ================================================================
void C_elm_quake::save(C_tnm_save_stream& stream)
{
	stream.save(m_param);
}

// ****************************************************************
// クエイク：ロード
// ================================================================
void C_elm_quake::load(C_tnm_save_stream& stream)
{
	stream.load(m_param);
}

// ****************************************************************
// クエイク：コピー
// ================================================================
void C_elm_quake::copy(const C_elm_quake* src)
{
	// 通常パラメータ
	m_param = src->m_param;
}

// ****************************************************************
// クエイク：「vec」開始
// ================================================================
void C_elm_quake::start_vec(int vec, int power, int time, int cnt, int end_cnt, int begin_order, int end_order, bool wait_flag, bool key_flag)
{
	end(0);

	m_param.type = TNM_QUAKE_TYPE_VEC;
	m_param.vec = vec;
	m_param.power = power;
	m_param.cur_time = 0;
	m_param.total_time = time;
	m_param.cnt = cnt;
	m_param.end_cnt = end_cnt;
	m_param.begin_order = begin_order;
	m_param.end_order = end_order;

	// 待ち状態に入る
	if (wait_flag)	{
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_QUAKE_WAIT;
		proc.element = get_element();
		proc.key_skip_enable_flag = key_flag;
		tnm_push_proc(proc);
	}
}

// ****************************************************************
// クエイク：「dir」開始
// ================================================================
void C_elm_quake::start_dir(int vec, int power, int time, int cnt, int end_cnt, int begin_order, int end_order, bool wait_flag, bool key_flag)
{
	end(0);

	m_param.type = TNM_QUAKE_TYPE_DIR;
	m_param.vec = vec;
	m_param.power = power;
	m_param.cur_time = 0;
	m_param.total_time = time;
	m_param.cnt = cnt;
	m_param.end_cnt = end_cnt;
	m_param.begin_order = begin_order;
	m_param.end_order = end_order;

	// 待ち状態に入る
	if (wait_flag)	{
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_QUAKE_WAIT;
		proc.element = get_element();
		proc.key_skip_enable_flag = key_flag;
		tnm_push_proc(proc);
	}
}

// ****************************************************************
// クエイク：「zoom」開始
// ================================================================
void C_elm_quake::start_zoom(int power, int time, int cnt, int end_cnt, int center_x, int center_y, int begin_order, int end_order, bool wait_flag, bool key_flag)
{
	end(0);

	m_param.type = TNM_QUAKE_TYPE_ZOOM;
	m_param.power = power;
	m_param.cur_time = 0;
	m_param.total_time = time;
	m_param.cnt = cnt;
	m_param.end_cnt = end_cnt;
	m_param.center_x = center_x;
	m_param.center_y = center_y;
	m_param.begin_order = begin_order;
	m_param.end_order = end_order;

	// 待ち状態に入る
	if (wait_flag)	{
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_QUAKE_WAIT;
		proc.element = get_element();
		proc.key_skip_enable_flag = key_flag;
		tnm_push_proc(proc);
	}
}

// ****************************************************************
// クエイク：終了（time 秒かけて終了）
// ================================================================
void C_elm_quake::end(int time)
{
	m_param.end_flag = 1;
	m_param.end_cur_time = 0;
	m_param.end_total_time = time;

	// 指定が０秒なら再初期化
	if (time == 0)
		reinit();
}

// ****************************************************************
// クエイク：待つ
// ================================================================
void C_elm_quake::wait(bool key_skip_flag)
{
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_QUAKE_WAIT;
	proc.element = get_element();
	proc.key_skip_enable_flag = key_skip_flag;
	tnm_push_proc(proc);
}

// ****************************************************************
// クエイク：判定
// ================================================================
int C_elm_quake::check()
{
	return
		m_param.type == TNM_QUAKE_TYPE_NONE ? 0 :	// 揺れていないなら０
		m_param.end_flag == 1 ? 2 :					// 終了揺れ中なら２
		1;											// 揺れているなら１
}

// ****************************************************************
// クエイク：時間を進める
// ================================================================
void C_elm_quake::update_time(int past_time)
{
	// 時間を加算
	m_param.cur_time += past_time;
	m_param.end_cur_time += past_time;
}

// ****************************************************************
// クエイク：フレーム処理
// ================================================================
void C_elm_quake::frame()
{
	if (m_param.type >= 0)	{

		// 例外処理
		if (m_param.total_time <= 0)	{
			reinit();	// 再初期化
		}
		else	{

			// 無制限フラグ
			bool loop = (m_param.cnt == 0 && m_param.end_cnt == 0);

			// 時間が過ぎていれば終了
			if (!loop && m_param.cur_time - (m_param.total_time * (m_param.cnt + m_param.end_cnt)) >= 0)	{
				reinit();	// 再初期化
			}
			else	{

				// タイプ：vec
				if (m_param.type == TNM_QUAKE_TYPE_VEC)	{
					int power = m_param.power;

					int x_sign = 0, y_sign = 0;
					if (m_param.vec == TNM_QUAKE_VEC_U)		{	x_sign = 0;		y_sign = -1;	}
					if (m_param.vec == TNM_QUAKE_VEC_RU)	{	x_sign = 1;		y_sign = -1;	}
					if (m_param.vec == TNM_QUAKE_VEC_R)		{	x_sign = 1;		y_sign = 0;		}
					if (m_param.vec == TNM_QUAKE_VEC_RD)	{	x_sign = 1;		y_sign = 1;		}
					if (m_param.vec == TNM_QUAKE_VEC_D)		{	x_sign = 0;		y_sign = 1;		}
					if (m_param.vec == TNM_QUAKE_VEC_LD)	{	x_sign = -1;	y_sign = 1;		}
					if (m_param.vec == TNM_QUAKE_VEC_L)		{	x_sign = -1;	y_sign = 0;		}
					if (m_param.vec == TNM_QUAKE_VEC_LU)	{	x_sign = -1;	y_sign = -1;	}

					int jump_cur_time = m_param.cur_time % m_param.total_time;
					if (jump_cur_time < m_param.total_time / 4)	{				// 進む１
						int time = jump_cur_time;
						m_work.cur_pos_x = speed_up_limit(time, 0, 0, m_param.total_time / 4, power / 2) * x_sign;
						m_work.cur_pos_y = speed_up_limit(time, 0, 0, m_param.total_time / 4, power / 2) * y_sign;
					}
					else if (jump_cur_time < m_param.total_time / 2)	{		// 進む２
						int time = jump_cur_time - m_param.total_time / 4;
						m_work.cur_pos_x = speed_down_limit(time, 0, power / 2, m_param.total_time / 4, power) * x_sign;
						m_work.cur_pos_y = speed_down_limit(time, 0, power / 2, m_param.total_time / 4, power) * y_sign;
					}
					else if (jump_cur_time < m_param.total_time * 3 / 4)	{	// 戻る１
						int time = jump_cur_time - m_param.total_time / 2;
						m_work.cur_pos_x = speed_up_limit(time, 0, power, m_param.total_time / 4, power / 2) * x_sign;
						m_work.cur_pos_y = speed_up_limit(time, 0, power, m_param.total_time / 4, power / 2) * y_sign;
					}
					else if (jump_cur_time < m_param.total_time)	{			// 戻る２
						int time = jump_cur_time - m_param.total_time * 3 / 4;
						m_work.cur_pos_x = speed_down_limit(time, 0, power / 2, m_param.total_time / 4, 0) * x_sign;
						m_work.cur_pos_y = speed_down_limit(time, 0, power / 2, m_param.total_time / 4, 0) * y_sign;
					}
				}

				// タイプ：dir
				else if (m_param.type == TNM_QUAKE_TYPE_DIR)	{
					int power = m_param.power;

					int x_sign = 0, y_sign = 0;
					if (m_param.vec == TNM_QUAKE_VEC_U)		{	x_sign = 0;		y_sign = -1;	}
					if (m_param.vec == TNM_QUAKE_VEC_RU)	{	x_sign = 1;		y_sign = -1;	}
					if (m_param.vec == TNM_QUAKE_VEC_R)		{	x_sign = 1;		y_sign = 0;		}
					if (m_param.vec == TNM_QUAKE_VEC_RD)	{	x_sign = 1;		y_sign = 1;		}
					if (m_param.vec == TNM_QUAKE_VEC_D)		{	x_sign = 0;		y_sign = 1;		}
					if (m_param.vec == TNM_QUAKE_VEC_LD)	{	x_sign = -1;	y_sign = 1;		}
					if (m_param.vec == TNM_QUAKE_VEC_L)		{	x_sign = -1;	y_sign = 0;		}
					if (m_param.vec == TNM_QUAKE_VEC_LU)	{	x_sign = -1;	y_sign = -1;	}

					// 揺れ
					int jump_cur_time = m_param.cur_time % m_param.total_time;
					if (jump_cur_time < m_param.total_time / 4)	{				// 進む１
						int time = jump_cur_time;
						m_work.cur_pos_x = speed_down_limit(time, 0, 0, m_param.total_time / 4, power / 2) * x_sign;
						m_work.cur_pos_y = speed_down_limit(time, 0, 0, m_param.total_time / 4, power / 2) * y_sign;
					}
					else if (jump_cur_time < m_param.total_time / 2)	{		// 戻る１
						int time = jump_cur_time - m_param.total_time / 4;
						m_work.cur_pos_x = speed_up_limit(time, 0, power / 2, m_param.total_time / 4, 0) * x_sign;
						m_work.cur_pos_y = speed_up_limit(time, 0, power / 2, m_param.total_time / 4, 0) * y_sign;
					}
					else if (jump_cur_time < m_param.total_time * 3 / 4)	{	// 進む２
						int time = jump_cur_time - m_param.total_time / 2;
						m_work.cur_pos_x = speed_down_limit(time, 0, 0, m_param.total_time / 4, - power / 2) * x_sign;
						m_work.cur_pos_y = speed_down_limit(time, 0, 0, m_param.total_time / 4, - power / 2) * y_sign;
					}
					else if (jump_cur_time < m_param.total_time)	{			// 戻る２
						int time = jump_cur_time - m_param.total_time * 3 / 4;
						m_work.cur_pos_x = speed_up_limit(time, 0, - power / 2, m_param.total_time / 4, 0) * x_sign;
						m_work.cur_pos_y = speed_up_limit(time, 0, - power / 2, m_param.total_time / 4, 0) * y_sign;
					}
				}
	
				// タイプ：zoom
				else if (m_param.type == TNM_QUAKE_TYPE_ZOOM)	{
					int power = limit(0, m_param.power, 255);
					int scale = 256 * TNM_SCALE_UNIT / (256 - power);
					int half_scale = (scale - TNM_SCALE_UNIT) / 2 + TNM_SCALE_UNIT;

					int jump_cur_time = m_param.cur_time % m_param.total_time;
					if (jump_cur_time < m_param.total_time / 4)			{		// 大きくなる
						int time = jump_cur_time;
						m_work.cur_scale = speed_up_limit(time, 0, TNM_SCALE_UNIT, m_param.total_time / 4, half_scale);
					}
					else if (jump_cur_time < m_param.total_time / 2)		{	// 大きくなる
						int time = jump_cur_time - m_param.total_time / 4;
						m_work.cur_scale = speed_down_limit(time, 0, half_scale, m_param.total_time / 4, scale);
					}
					else if (jump_cur_time < m_param.total_time * 3 / 4)	{	// 大きくなる
						int time = jump_cur_time - m_param.total_time / 2;
						m_work.cur_scale = speed_up_limit(time, 0, scale, m_param.total_time / 4, half_scale);
					}
					else	{													// 小さくなる
						int time = jump_cur_time - m_param.total_time * 3 / 4;
						m_work.cur_scale = speed_down_limit(time, 0, half_scale, m_param.total_time / 4, TNM_SCALE_UNIT);
					}
				}

				// エンド揺れ補正
				if (!loop && (m_param.cur_time - m_param.total_time * m_param.cnt >= 0))	{
					m_work.cur_pos_x = (int)((double)m_work.cur_pos_x * linear_limit<double, double>(m_param.cur_time, m_param.total_time * m_param.cnt, 1, m_param.total_time * (m_param.cnt + m_param.end_cnt), 0));
					m_work.cur_pos_y = (int)((double)m_work.cur_pos_y * linear_limit<double, double>(m_param.cur_time, m_param.total_time * m_param.cnt, 1, m_param.total_time * (m_param.cnt + m_param.end_cnt), 0));
					m_work.cur_scale = (int)((double)(m_work.cur_scale - TNM_SCALE_UNIT) * linear_limit<double, double>(m_param.cur_time, m_param.total_time * m_param.cnt, 1, m_param.total_time * (m_param.cnt + m_param.end_cnt), 0) + TNM_SCALE_UNIT);
				}

				// 終了処理
				if (m_param.end_flag)	{
					if (m_param.end_cur_time - m_param.end_total_time < 0)	{
						m_work.cur_pos_x = (int)((double)m_work.cur_pos_x * linear_limit<double, double>(m_param.end_cur_time, 0, 1, m_param.end_total_time, 0));
						m_work.cur_pos_y = (int)((double)m_work.cur_pos_y * linear_limit<double, double>(m_param.end_cur_time, 0, 1, m_param.end_total_time, 0));
						m_work.cur_scale = (int)((double)(m_work.cur_scale - TNM_SCALE_UNIT) * linear_limit<double, double>(m_param.end_cur_time, 0, 1, m_param.end_total_time, 0) + TNM_SCALE_UNIT);
					}
					else	{
						reinit();
					}
				}
			}
		}
	}

	// クエイク終了ならパラメータを初期化
	if (m_param.type == TNM_QUAKE_TYPE_NONE)
		reinit();
}

// ****************************************************************
// クエイク：レンダーパラメータを取得
// ================================================================
void C_elm_quake::get_rp(S_d3d_render_param* p_rp)
{
	if (m_param.type >= 0)	{
		p_rp->disp = true;
		p_rp->pos.x += m_work.cur_pos_x;
		p_rp->pos.y += m_work.cur_pos_y;
		p_rp->center.x += m_param.center_x;
		p_rp->center.y += m_param.center_y;
		p_rp->scale.x *= (float)m_work.cur_scale / TNM_SCALE_UNIT;
		p_rp->scale.y *= (float)m_work.cur_scale / TNM_SCALE_UNIT;
		p_rp->rotate.z = (float)m_work.cur_rotate / TNM_ANGLE_UNIT / 360 * D3DX_PI;
	}
}

// ****************************************************************
// クエイクリスト：サブの初期化
// ================================================================
void C_elm_quake_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"));
}

// ****************************************************************
// クエイクリスト：サブの再初期化
// ================================================================
void C_elm_quake_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// クエイクリスト：サブのセーブ
// ================================================================
void C_elm_quake_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// クエイクリスト：サブのロード
// ================================================================
void C_elm_quake_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}
