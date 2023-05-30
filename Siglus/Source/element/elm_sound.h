#pragma		once

class		C_tnm_save_stream;

#include	"data/tnm_element.h"

#include	"element/elm_list.h"
#include	"element/elm_sound_player.h"
#include	"element/elm_mov.h"

// ****************************************************************
// BGM
// ================================================================
//		２つのプレイヤーを使ってクロスフェードを行うことが出来ます。
// ================================================================
class C_elm_bgm : public C_tnm_element
{
public:
	void	init(S_element element, int form, CTSTR& name);		// 初期化
	void	reinit();											// 再初期化
	void	free();												// 解放
	void	save(C_tnm_save_stream& stream);					// セーブ
	void	load(C_tnm_save_stream& stream);					// ロード
	void	update_time(int past_real_time);					// 時間を進める
	
	// コマンド
	void	play(TSTR regist_name, int fade_in_time, int fade_out_time, int start_pos, bool pause_flag, int delay_time);				// 再生
	void	play_oneshot(TSTR regist_name, int fade_in_time, int fade_out_time, int start_pos, bool pause_flag, int delay_time);		// 再生
	void	play_wait(TSTR regist_name, int fade_in_time, int fade_out_time, int start_pos, bool pause_flag, int delay_time);			// 再生
	void	stop(int fade_out_time = 100);								// 停止
	void	pause(int fade_out_time = 100);								// 一時停止
	void	resume(bool wait_flag, int fade_in_time, int delay_time);	// 再開
	void	wait(bool key_skip_flag, bool return_value_flag);			// ウェイト
	void	wait_fade(bool key_skip_flag, bool return_value_flag);		// フェードウェイト
	int		check();
	void	set_volume(int volume, int fade_time = 0);					// ボリューム設定
	void	set_volume_max(int fade_time = 0);							// ボリューム設定（最大）
	void	set_volume_min(int fade_time = 0);							// ボリューム設定（最小）
	void	set_system_volume(int volume);								// システムボリュームを設定
	int		get_play_pos();												// プレイ位置を取得

	// 情報取得
	int		get_player_cnt()	{ return (int)m_player_list.size(); }
	TSTR	get_regist_name()	{ return m_regist_name; }
	int		get_volume()		{ return m_volume; }
	bool	is_loop()			{ return m_loop_flag; }
	bool	is_playing();
	bool	is_fade_out_doing();
	bool	is_pause()			{ return m_pause_flag; }
	int		get_delay_time()	{ return m_delay_time; }

	C_tnm_player*		get_player(int player_no);

private:
	void	play(TSTR regist_name, bool loop_flag, bool wait_flag, int fade_in_time, int fade_out_time, int start_pos, bool pause_flag, int delay_time);		// 再生

	// パラメータ
	TSTR				m_regist_name;		// BGM の登録名
	int					m_volume;			// ボリューム
	int					m_delay_time;		// 遅延時間
	int					m_fade_in_time;		// フェードイン時間
	bool				m_loop_flag;		// ループフラグ
	bool				m_pause_flag;		// ポーズフラグ

	// 作業変数
	ARRAY<C_tnm_player>	m_player_list;		// プレイヤー（クロスフェードのために複数必要です）
	int					m_cur_player_id;	// 現在使用中のプレイヤーＩＤ
};

// ****************************************************************
// KOE
// ================================================================
class C_elm_koe : public C_tnm_element
{
public:
	void	init(S_element element, int form, CTSTR& name);		// 初期化
	void	reinit();											// 再初期化
	void	free();												// 解放
	void	save(C_tnm_save_stream& stream);					// セーブ
	void	load(C_tnm_save_stream& stream);					// ロード
	
	// コマンド
	void	play(int koe_no, int chara_no, int jitan_rate, int play_pos_mills, bool wait_flag, bool key_skip_flag, bool return_value_flag, bool ex_koe_flag, bool check_only);			// 再生
	void	stop(int fade_time = 100);							// 停止
	void	wait(bool key_wait_flag, bool return_value_flag);	// ウェイト
	int		check();											// チェック
	void	set_volume(int volume, int fade_time = 0);			// ボリューム設定
	void	set_volume_max(int fade_time = 0);					// ボリューム設定（最大）
	void	set_volume_min(int fade_time = 0);					// ボリューム設定（最小）
	void	set_system_volume(int volume);						// システムボリュームを設定

	// 情報取得
	bool	is_playing()		{	return check() == TNM_PLAYER_STATE_PLAY;											}
	bool	is_using()			{	return check() == TNM_PLAYER_STATE_PLAY || check() == TNM_PLAYER_STATE_FADE_OUT;	}
	int		get_player_cnt()	{	return (int)m_player_list.size();	}
	int		get_koe_no()		{	return m_koe_no;					}
	int		get_chara_no()		{	return m_chara_no;					}
	int		get_volume()		{	return m_volume;					}
	bool	get_ex_koe_flag()	{	return m_ex_koe_flag;				}

	int		get_length();
	int		get_length_by_millsecond();
	int		get_play_pos();
	int		get_play_pos_by_millsecond();
	float	get_current_mouth_volume();

	C_tnm_player*				get_player(int player_no);
	C_sound_stream_base*		get_stream(int player_no);


private:

	ARRAY<C_tnm_player>	m_player_list;		// プレイヤー
	int					m_koe_no;			// 声番号
	int					m_chara_no;			// キャラ番号
	int					m_volume;			// ボリューム
	bool				m_ex_koe_flag;		// exkoe フラグ
};

// ****************************************************************
// PCM
// ================================================================
//		PCM を管理するクラスです。
//		汎用プレイヤーを使って PCM を再生します。
// ================================================================
class C_elm_pcm : public C_tnm_element
{
public:
	// システム
	void	init(S_element element, int form, CTSTR& name, int player_type);	// 初期化
	void	reinit();							// 再初期化
	void	free();								// 解放
	void	save(C_tnm_save_stream& stream);	// セーブ
	void	load(C_tnm_save_stream& stream);	// ロード

	// コマンド
	void	play_koe(int koe_no, int* p_used_player_no);		// 声を再生
	void	play_pcm(TSTR file_name, int* p_used_player_no);	// 効果音を再生
	void	play_se(int se_no, int* p_used_player_no);			// システム音を再生
	void	stop(int fade_out_time = 100);		// 停止
	void	wait(bool is_key_wait);				// 再生終了待ち
	int		check();
	void	set_volume(int volume, int fade_time = 0);
	void	set_volume_max(int fade_time = 0);
	void	set_volume_min(int fade_time = 0);
	void	set_system_volume(int volume);

	// 情報取得
	bool	is_playing()		{	return check() == TNM_PLAYER_STATE_PLAY;											}
	bool	is_pausing()		{	return check() == TNM_PLAYER_STATE_PAUSE;											}
	bool	is_using()			{	return check() == TNM_PLAYER_STATE_PLAY || check() == TNM_PLAYER_STATE_FADE_OUT;	}
	int		get_player_cnt()	{	return (int)m_player_list.size();		}
	int		get_volume()		{	return m_volume;						}

	C_tnm_player*		get_player(int player_no);

private:

	// 固定パラメータ
	int					m_player_type;

	// 通常パラメータ
	int					m_volume;

	// 一時パラメータ
	ARRAY<C_tnm_player>	m_player_list;
	int					m_next_player_id;
};

// ****************************************************************
// PCMCH
// ================================================================
//		PCMCH を利用して PCM 再生を行います。
// ================================================================
class C_elm_pcmch : public C_tnm_element
{
public:
	// システム
	void	init(S_element element, int form, CTSTR& name);	// 初期化
	void	reinit();										// 再初期化
	void	free();											// 解放
	void	save(C_tnm_save_stream& stream);				// セーブ
	void	load(C_tnm_save_stream& stream);				// ロード
	void	update_time(int past_real_time);				// 時間を進める

	// コマンド
	void	play_pcm(TSTR file_name, bool loop_flag, bool wait_flag, int fade_time, int volume_type, int chara_no, bool bgm_fade_target_flag, bool bgm_fade2_target_flag, bool bgm_fade_source_flag, bool ready_only);
	void	play_bgm(TSTR regist_name, bool loop_flag, bool wait_flag, int fade_time, int volume_type, int chara_no, bool bgm_fade_target_flag, bool bgm_fade2_target_flag, bool bgm_fade_source_flag, bool ready_only);
	void	play_koe(int koe_no, bool loop_flag, bool wait_flag, int fade_time, int volume_type, int chara_no, bool bgm_fade_target_flag, bool bgm_fade2_target_flag, bool bgm_fade_source_flag, bool ready_only);
	void	play_se(int se_no, bool loop_flag, bool wait_flag, int fade_time, int volume_type, int chara_no, bool bgm_fade_target_flag, bool bgm_fade2_target_flag, bool bgm_fade_source_flag, bool ready_only);
	void	stop(int fade_time = 100);
	void	pause(int fade_time = 100);
	void	resume(bool wait_flag, int fade_time, int delay_time);
	void	wait(bool key_skip_flag, bool return_value_flag);
	void	wait_fade(bool key_skip_flag, bool return_value_flag);
	int		check()				{	return m_player.get_play_state();	}
	void	set_volume(int volume, int fade_time = 0);
	void	set_volume_max(int fade_time = 0);
	void	set_volume_min(int fade_time = 0);
	void	set_system_volume(int volume);			// システムボリュームを設定
	int		get_system_volume();					// システムボリュームを取得

	// 情報取得
	TSTR	get_pcm_name()			{	return m_pcm_name;					}
	TSTR	get_bgm_name()			{	return m_bgm_name;					}
	int		get_koe_no()			{	return m_koe_no;					}
	int		get_se_no()				{	return m_se_no;						}
	int		get_volume_type()		{	return m_volume_type;				}
	int		get_volume()			{	return m_volume;					}
	int		get_chara_no()			{	return m_chara_no;					}
	bool	is_loop()				{	return m_loop_flag;					}
	bool	is_bgm_fade_target()	{	return m_bgm_fade_target_flag;		}
	bool	is_bgm_fade2_target()	{	return m_bgm_fade2_target_flag;		}
	bool	is_bgm_fade_source()	{	return m_bgm_fade_source_flag;		}
	bool	is_playing();
	bool	is_fade_out_doing();

	// プレイヤー情報を取得
	int		get_length_by_millsecond()	{ return m_player.get_length_by_millsecond(); }
	int		get_play_state_2()			{ return m_player.get_play_state_2(); }

private:

	// 通常パラメータ
	TSTR			m_pcm_name;					// 効果音名
	TSTR			m_bgm_name;					// ＢＧＭ名
	int				m_koe_no;					// 声番号
	int				m_se_no;					// システム音番号
	int				m_volume_type;				// ボリュームタイプ
	int				m_chara_no;					// キャラクタ音声
	int				m_volume;					// ボリューム
	int				m_delay_time;				// 遅延時間
	int				m_fade_in_time;				// フェードイン時間
	bool			m_loop_flag;				// ループフラグ
	bool			m_bgm_fade_target_flag;		// ＢＧＭフェードのターゲットとなる
	bool			m_bgm_fade2_target_flag;	// ＢＧＭフェードのターゲットとなる
	bool			m_bgm_fade_source_flag;		// ＢＧＭフェードのソースとなる
	bool			m_ready_flag;				// 準備フラグ

	// 一時パラメータ
	C_tnm_player		m_player;			// プレイヤー
};

// ****************************************************************
// PCMCH リスト
// ================================================================
class C_elm_pcmch_list : public C_elm_list<C_elm_pcmch>
{

protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end);							// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード
};

// ****************************************************************
// SE
// ================================================================
//		SE の再生を管理します。
//		汎用の SE プレイヤーを使って SE を再生します。
// ================================================================
class C_elm_se : public C_elm_pcm
{
};

// ****************************************************************
// サウンド
// ================================================================
class C_elm_sound
{
public:
	void	init();								// 初期化
	void	reinit();							// 再初期化
	void	free();								// 解放
	void	save(C_tnm_save_stream& stream);	// セーブ
	void	load(C_tnm_save_stream& stream);	// ロード
	void	frame();							// フレーム処理

public:

	// 子エレメント
	C_elm_bgm			m_bgm;				// BGM
	C_elm_koe			m_koe;				// KOE
	C_elm_pcm			m_pcm;				// PCM
	C_elm_pcmch_list	m_pcmch_list;		// PCMCH
	C_elm_se			m_se;				// SE
	C_elm_mov			m_mov;				// MOV

	// サポート用の無音サウンド
	BSP<C_sound>					m_muon_sound;
	BSP<C_sound_stream>				m_muon_stream;
	BSP<C_player>					m_muon_player;
};


