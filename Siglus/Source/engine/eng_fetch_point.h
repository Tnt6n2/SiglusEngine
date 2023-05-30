#pragma		once

class C_tnm_fetch_point
{
public:
	ARRAY<C_point>	m_fetch_point;
	ARRAY<C_rect>	m_fetch_rect;
	C_point			m_fetch_start_pos;
	C_point			m_fetch_end_pos;
	C_point			m_fetch_last_pos;
	int				m_fetch_last_no;
	int				m_fetch_start_time;
	int				m_fetch_time_len;
	int				m_key_repeat_key;
	int				m_key_repeat_start_time;
	int				m_key_repeat_cnt;
};

// フェッチを初期化する
void tnm_init_fetch_point();

// フェッチポイントを追加する
void tnm_add_fetch_point(C_point fetch_pos, C_rect fetch_rect);

// カーソル移動でフェッチ処理を行うかどうか
bool tnm_is_fetch_proc();

// フェッチ処理を行う
bool tnm_fetch_proc();

// フェッチ初回移動
bool tnm_fetch_proc_initial_move();
