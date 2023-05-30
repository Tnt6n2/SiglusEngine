#pragma		once

#include	"element/elm_list.h"

// ****************************************************************
// 整数イベント：スピードタイプ
// ================================================================
enum E_tnm_int_event_speed_type
{
	E_tnm_int_event_speed_type_none = -1,
	E_tnm_int_event_speed_type_linear = 0,
	E_tnm_int_event_speed_type_speed_up = 1,
	E_tnm_int_event_speed_type_speed_down = 2,
};

// ****************************************************************
// 整数イベント：ループタイプ
// ================================================================
enum E_tnm_int_event_loop_type
{
	E_tnm_int_event_loop_type_none = -1,
	E_tnm_int_event_loop_type_oneshot = 0,
	E_tnm_int_event_loop_type_loop = 1,
	E_tnm_int_event_loop_type_turn = 2,

	// ３番は Flix で「揺れ」を作成中
	// １００番以降は Flix で吉田さんが実装
};

// ****************************************************************
// 整数イベント
// ================================================================
class C_elm_int_event	// 高速化のためエレメントを継承しない！
{
public:

	// 初期化
	void	init(int def_value);

	// 再初期化
	void	reinit();

	// 時間を更新
	void	update_time_sub(int past_game_time, int past_real_time);
	void	update_time(int past_game_time, int past_real_time)
	{
		// 判定部分だけインライン化することで高速処理
		if (m_loop_type != E_tnm_int_event_loop_type_none)
			update_time_sub(past_game_time, past_real_time);
	}

	// フレーム処理
	void	frame_sub();
	void	frame()
	{
		// 判定部分だけインライン化することで高速処理
		m_cur_value = m_value;
		if (m_loop_type != E_tnm_int_event_loop_type_none)
			frame_sub();
	}
	void	frame_no_event()
	{
		// イベントがないことが分かっている場合はこちらを呼び出す方が速いです。
		m_cur_value = m_value;
	}

	// 値を設定／取得
	void	set_value(int value)	{	m_value = value;	}
	int		get_value() const		{	return m_value;		}

	// 合計値を取得
	int		get_total_value() const	{	return m_cur_value;	}

	// イベント
	void	set_event(int value, int total_time, int delay_time, int speed_type, int real_flag);
	void	loop_event(int start_value, int end_value, int loop_time, int delay_time, int speed_type, int real_flag);
	void	turn_event(int start_value, int end_value, int loop_time, int delay_time, int speed_type, int real_flag);
	void	end_event();
	bool	check_event()	{	return m_loop_type != E_tnm_int_event_loop_type_none;	}	// 高速化

	int		m_def_value;
	int		m_value;
	int		m_cur_time;
	int		m_end_time;
	int		m_delay_time;
	int		m_start_value;
	int		m_cur_value;
	int		m_end_value;
	int		m_loop_type;
	int		m_speed_type;
	int		m_real_flag;
};

// ****************************************************************
// 整数イベントリスト
// ================================================================
class C_elm_int_event_list : public C_elm_list<C_elm_int_event>
{
public:
	void	init(S_element element, CTSTR& name, int size, bool extend_enable, int def_value);	// 初期化

protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end);							// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード

private:
	int		m_def_value;
};
