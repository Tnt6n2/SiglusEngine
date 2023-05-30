#include	"pch.h"
#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"element/elm_int_event.h"

// ****************************************************************
// 整数イベント：初期化
// ================================================================
void C_elm_int_event::init(int def_value)
{
	// 固定パラメータを設定
	m_def_value = def_value;

	// 最初期化
	reinit();
}

// ****************************************************************
// 整数イベント：再初期化
// ================================================================
void C_elm_int_event::reinit()
{
	m_value = m_def_value;
	m_cur_time = 0;
	m_end_time = 0;
	m_delay_time = 0;
	m_start_value = m_def_value;
	m_cur_value = m_def_value;
	m_end_value = m_def_value;
	m_loop_type = E_tnm_int_event_loop_type_none;
	m_speed_type = E_tnm_int_event_speed_type_none;
	m_real_flag = 0;
}

// ****************************************************************
// 整数イベント：イベントを設定
// ================================================================
void C_elm_int_event::set_event(int value, int total_time, int delay_time, int speed_type, int real_flag)
{
	m_cur_time = 0;
	m_end_time = total_time;
	m_delay_time = delay_time;
	m_start_value = m_value;
	m_cur_value = m_value;
	m_end_value = value;
	m_loop_type = E_tnm_int_event_loop_type_oneshot;
	m_speed_type = speed_type;
	m_real_flag = real_flag;

	// 現在の値を更新
	m_value = value;
}

// ****************************************************************
// 整数イベント：ループイベントを設定
// ================================================================
void C_elm_int_event::loop_event(int start_value, int end_value, int loop_time, int delay_time, int speed_type, int real_flag)
{
	m_cur_time = 0;
	m_end_time = loop_time;
	m_delay_time = delay_time;
	m_start_value = start_value;
	m_cur_value = start_value;
	m_end_value = end_value;
	m_loop_type = E_tnm_int_event_loop_type_loop;
	m_speed_type = speed_type;
	m_real_flag = real_flag;
}

// ****************************************************************
// 整数イベント：ターンイベントを設定
// ================================================================
void C_elm_int_event::turn_event(int start_value, int end_value, int loop_time, int delay_time, int speed_type, int real_flag)
{
	m_cur_time = 0;
	m_end_time = loop_time;
	m_delay_time = delay_time;
	m_start_value = start_value;
	m_cur_value = start_value;
	m_end_value = end_value;
	m_loop_type = E_tnm_int_event_loop_type_turn;
	m_speed_type = speed_type;
	m_real_flag = real_flag;
}

// ****************************************************************
// 整数イベント：イベントを終了
// ================================================================
void C_elm_int_event::end_event()
{
	m_loop_type = E_tnm_int_event_loop_type_none;
}

// ****************************************************************
// 整数イベント：イベントを判定
// ================================================================
/*
bool C_elm_int_event::check_event()
{
	return m_loop_type != E_tnm_int_event_loop_type_none;
}
*/

// ****************************************************************
// 整数イベント：時間を進める
// ================================================================
void C_elm_int_event::update_time_sub(int game_past_time, int real_past_time)
{
	// 時刻を進める
	if (m_real_flag == 0)	{
		m_cur_time += game_past_time;
	}
	else	{
		m_cur_time += real_past_time;
	}
}

// ****************************************************************
// 整数イベント：フレーム処理
// ================================================================
//		メインである frame() はヘッダで定義されています。（インライン）
// ================================================================
void C_elm_int_event::frame_sub()
{
	// パラメータを取得
	int end_time = m_end_time;
	int start_value = m_start_value;
	int end_value = m_end_value;
	int cur_time = m_cur_time - m_delay_time;

	// ※ここで時刻を０以上に補正してはいけません。
	// イベント時間＝０、修正時間＝１０００の場合、すぐにイベントが終わってしまうからです。
	// （１０００ﾐﾘ秒待つべきです）

	// ワンショットの場合は終了時刻を過ぎていたらイベント終了
	if (m_loop_type == E_tnm_int_event_loop_type_oneshot)	{
		if (cur_time - end_time >= 0)	{
			m_loop_type = E_tnm_int_event_loop_type_none;
			return;
		}
	}

	// 開始時刻になっていなければ開始値
	if (cur_time <= 0)	{
		m_cur_value = start_value;
		return;
	}

	// ワンショットの場合は終了時刻を過ぎていたら終了値
	//if (m_loop_type == E_tnm_int_event_loop_type_oneshot)	{
	//	if (cur_time - end_time >= 0)	{
	//		m_cur_value = end_value;
	//		return;
	//	}
	//}

	// ループの場合の時間補正
	if (m_loop_type == E_tnm_int_event_loop_type_loop)	{
		cur_time %= end_time;
	}

	// ターンの場合の時間補正
	if (m_loop_type == E_tnm_int_event_loop_type_turn)	{
		cur_time %= end_time * 2;

		if (cur_time - end_time > 0)	{
			cur_time = end_time - (cur_time - end_time);
		}
	}

	if (false);
	else if (m_speed_type == 0)	{	// 線形
		m_cur_value = (int)((double)(end_value - start_value) * (cur_time) / (end_time) + start_value);
	}
	else if (m_speed_type == 1)	{	// 加速
		m_cur_value = (int)((double)(end_value - start_value) * (cur_time) * (cur_time) / (end_time) / (end_time) + start_value);
	}
	else if (m_speed_type == 2)	{	// 減速
		m_cur_value = (int)(- (double)(end_value - start_value) * (cur_time - end_time) * (cur_time - end_time) / (end_time) / (end_time) + end_value);
	}
}

// ****************************************************************
// 整数リストイベント：初期化
// ================================================================
void C_elm_int_event_list::init(S_element element, CTSTR& name, int size, bool extend_enable, int def_value)
{
	// 固定パラメータ
	m_def_value = def_value;

	// 基底の初期化
	C_elm_list<C_elm_int_event>::init(element, FM_INTEVENTLIST, name, size, extend_enable, NULL);
}

// ****************************************************************
// 整数イベントリスト：初期化
// ================================================================
void C_elm_int_event_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(m_def_value);
}

// ****************************************************************
// 整数イベントリスト：再初期化
// ================================================================
void C_elm_int_event_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// 整数イベントリスト：セーブ
// ================================================================
void C_elm_int_event_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	// 整数イベントはＰＯＤなので memcpy で一括セーブする
	stream.save(m_sub.get() + begin, (end - begin) * sizeof(C_elm_int_event));
}

// ****************************************************************
// 整数イベントリスト：ロード
// ================================================================
void C_elm_int_event_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	// 整数イベントはＰＯＤなので memcpy で一括ロードする
	stream.load(m_sub.get() + begin, (end - begin) * sizeof(C_elm_int_event));
}
