#include	"pch.h"

#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_counter.h"

// ****************************************************************
// カウンタ：初期化
// ================================================================
void C_elm_counter::init(S_element element, CTSTR& name)
{
	// 基底クラスの初期化
	C_tnm_element::init(element, FM_COUNTER, name, NULL);

	// 再初期化
	reinit();
}

// ****************************************************************
// カウンタ：再初期化
// ================================================================
void C_elm_counter::reinit()
{
	// パラメータ
	m_param.is_running = false;
	m_param.real_flag = false;
	m_param.frame_mode = false;
	m_param.frame_loop_flag = false;
	m_param.frame_start_value = 0;
	m_param.frame_end_value = 0;
	m_param.frame_time = 0;

	// 一時パラメータ
	m_param.cur_time = 0;
}

// ****************************************************************
// カウンタ：セーブ
// ================================================================
void C_elm_counter::save(C_tnm_save_stream& stream)
{
	stream.save(m_param);
}

// ****************************************************************
// カウンタ：ロード
// ================================================================
void C_elm_counter::load(C_tnm_save_stream& stream)
{
	stream.load(m_param);
}

// ****************************************************************
// カウンタ：コピー
// ================================================================
void C_elm_counter::copy(const C_elm_counter* src)
{
	m_param = src->m_param;
}

// ****************************************************************
// カウンタ：リセット
// ================================================================
void C_elm_counter::reset()
{
	// パラメータ
	m_param.is_running = false;
	m_param.real_flag = false;
	m_param.frame_mode = false;

	// 一時パラメータ
	m_param.cur_time = 0;				// リセット

	// frame_mode も初期化するのは重要です。
	// これを初期化しない場合、get_count がフレームの最終値を返し続けます。
	// 0 を返すようにしなければいけません。
}

// ****************************************************************
// カウンタ：開始
// ================================================================
void C_elm_counter::start()
{
	// パラメータ
	m_param.is_running = true;			// 開始
	m_param.real_flag = false;
	m_param.frame_mode = false;

	// 一時パラメータ
	m_param.cur_time = 0;				// リセット
}

// ****************************************************************
// カウンタ：開始（実時間）
// ================================================================
void C_elm_counter::start_real()
{
	// パラメータ
	m_param.is_running = true;			// 開始
	m_param.real_flag = true;
	m_param.frame_mode = false;

	// 一時パラメータ
	m_param.cur_time = 0;				// リセット
}

// ****************************************************************
// カウンタ：フレーム開始
// ================================================================
void C_elm_counter::start_frame(int start_value, int end_value, int frame_time)
{
	// パラメータ
	m_param.is_running = true;			// 開始
	m_param.real_flag = false;
	m_param.frame_mode = true;
	m_param.frame_loop_flag = false;
	m_param.frame_start_value = start_value;
	m_param.frame_end_value = end_value;
	m_param.frame_time = frame_time;

	// 一時パラメータ
	m_param.cur_time = 0;				// リセット
}

// ****************************************************************
// カウンタ：フレーム開始（実時間）
// ================================================================
void C_elm_counter::start_frame_real(int start_value, int end_value, int frame_time)
{
	// パラメータ
	m_param.is_running = true;			// 開始
	m_param.real_flag = true;
	m_param.frame_mode = true;
	m_param.frame_loop_flag = false;
	m_param.frame_start_value = start_value;
	m_param.frame_end_value = end_value;
	m_param.frame_time = frame_time;

	// 一時パラメータ
	m_param.cur_time = 0;				// リセット
}

// ****************************************************************
// カウンタ：フレーム開始（ループ）
// ================================================================
void C_elm_counter::start_frame_loop(int start_value, int end_value, int frame_time)
{
	// パラメータ
	m_param.is_running = true;			// 開始
	m_param.real_flag = false;
	m_param.frame_mode = true;
	m_param.frame_loop_flag = true;
	m_param.frame_start_value = start_value;
	m_param.frame_end_value = end_value;
	m_param.frame_time = frame_time;

	// 一時パラメータ
	m_param.cur_time = 0;				// リセット
}

// ****************************************************************
// カウンタ：フレーム開始（ループ）（実時間）
// ================================================================
void C_elm_counter::start_frame_loop_real(int start_value, int end_value, int frame_time)
{
	// パラメータ
	m_param.is_running = true;			// 開始
	m_param.real_flag = true;
	m_param.frame_mode = true;
	m_param.frame_loop_flag = true;
	m_param.frame_start_value = start_value;
	m_param.frame_end_value = end_value;
	m_param.frame_time = frame_time;

	// 一時パラメータ
	m_param.cur_time = 0;				// リセット
}

// ****************************************************************
// カウンタ：一時停止
// ================================================================
void C_elm_counter::stop()
{
	m_param.is_running = false;		// 停止
}

// ****************************************************************
// カウンタ：現在の値から再開
// ================================================================
void C_elm_counter::resume()
{
	m_param.is_running = true;		// 開始
}

// ****************************************************************
// カウンタ：時間を進める
// ================================================================
void C_elm_counter::update_time(int past_game_time, int past_real_time)
{
	// 実行中なら合計時刻を加算
	if (m_param.is_running)	{
		m_param.cur_time += m_param.real_flag ? past_real_time : past_game_time;
	}

	// フレームの場合、時間が来たら止める
	if (m_param.frame_mode && !m_param.frame_loop_flag)	{
		if (m_param.cur_time >= m_param.frame_time)	{
			m_param.is_running = false;
		}
	}
}

// ****************************************************************
// カウンタ：値を取得
// ================================================================
int C_elm_counter::get_count() const
{
	// フレームの場合
	if (m_param.frame_mode)	{

		int value = 0;

		if (m_param.frame_time <= 0)									// 例外処理：フレームにかける時間が０以下
			return m_param.frame_end_value;
		if (m_param.frame_start_value == m_param.frame_end_value)		// 例外処理：フレームの開始値と終了値が同じ
			return m_param.frame_end_value;

		// 値を計算
		value = (int)((double)(m_param.frame_end_value - m_param.frame_start_value) * m_param.cur_time / m_param.frame_time);

		// ループの場合
		if (m_param.frame_loop_flag)	{
			value %= m_param.frame_end_value - m_param.frame_start_value;
			value += m_param.frame_start_value;
		}
		// ループでない場合
		else	{
			value += m_param.frame_start_value;
			if (m_param.frame_start_value > m_param.frame_end_value)
				value = limit(m_param.frame_end_value, value, m_param.frame_start_value);
			else
				value = limit(m_param.frame_start_value, value, m_param.frame_end_value);
		}

		return value;
	}

	// カウンタの場合
	else	{

		return m_param.cur_time;
	}
}

// ****************************************************************
// カウンタ：値を設定
// ================================================================
void C_elm_counter::set_count(int value)
{
	// フレームの場合
	if (m_param.frame_mode)	{

		if (m_param.frame_end_value == m_param.frame_start_value)	{	// 例外処理：フレームにかける時間が０以下
			m_param.cur_time = 0;
			return;
		}

		// ループの場合
		if (m_param.frame_loop_flag)	{
			m_param.cur_time = (value - m_param.frame_start_value) * m_param.frame_time / (m_param.frame_end_value - m_param.frame_start_value);
			m_param.cur_time = limit(0, m_param.cur_time, m_param.frame_time - 1);	// ループの場合は -1 までしか行かない
		}
		// ループでない場合
		else	{
			m_param.cur_time = (value - m_param.frame_start_value) * m_param.frame_time / (m_param.frame_end_value - m_param.frame_start_value);
			m_param.cur_time = limit(0, m_param.cur_time, m_param.frame_time);		// ループでない場合は最後まで行く
		}

	}
	else	{
		m_param.cur_time = value;
	}
}

// ****************************************************************
// カウンタ：アクティブかどうか判定
// ================================================================
bool C_elm_counter::is_active() const
{
	return m_param.is_running;
}

// ****************************************************************
// カウンタリスト：サブの初期化
// ================================================================
void C_elm_counter_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"));
}

// ****************************************************************
// カウンタリスト：サブの再初期化
// ================================================================
void C_elm_counter_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// カウンタリスト：サブのセーブ
// ================================================================
void C_elm_counter_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// カウンタリスト：サブのロード
// ================================================================
void C_elm_counter_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}
