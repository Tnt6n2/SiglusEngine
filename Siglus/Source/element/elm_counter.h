#pragma		once

#include	"data/tnm_element.h"
#include	"element/elm_list.h"

class	C_tnm_save_stream;

// ****************************************************************
// カウンタ：パラメータ
// ================================================================
struct S_tnm_counter_param
{
	// 通常パラメータ
	bool	is_running;			// 実行中フラグ
	bool	real_flag;			// 実時間モード
	bool	frame_mode;			// フレームモード
	bool	frame_loop_flag;	// ループフラグ
	int		frame_start_value;	// 開始値
	int		frame_end_value;	// 終了値
	int		frame_time;			// フレーム時間

	// 一時パラメータ
	int		cur_time;			// 合計時刻
};

// ****************************************************************
// カウンタ
// ================================================================
class C_elm_counter : public C_tnm_element
{
public:

	// エレメントの基本動作
	void	init(S_element element, CTSTR& name);					// 初期化
	void	reinit();												// 再初期化
	void	save(C_tnm_save_stream& stream);						// セーブ
	void	load(C_tnm_save_stream& stream);						// ロード
	void	copy(const C_elm_counter* src);							// コピー
	void	update_time(int past_game_time, int past_real_time);	// 時間を進める

	// カウンタの操作
	void	reset();			// リセット
	void	start();			// 計測開始
	void	start_real();		// 計測開始（実時間）
	void	start_frame(int start_value, int end_value, int frame_time);
	void	start_frame_real(int start_value, int end_value, int frame_time);
	void	start_frame_loop(int start_value, int end_value, int frame_time);
	void	start_frame_loop_real(int start_value, int end_value, int frame_time);
	void	stop();				// 一時停止
	void	resume();			// 現在の値から再開

	// 状態取得
	void	set_count(int count);
	int		get_count() const;
	bool	is_active() const;

	// パラメータ取得
	const S_tnm_counter_param&	param() const	{	return m_param;	}

private:

	S_tnm_counter_param		m_param;
};


// ****************************************************************
// カウンタリスト
// ================================================================
class C_elm_counter_list : public C_elm_list<C_elm_counter>
{
protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end);							// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード
};

