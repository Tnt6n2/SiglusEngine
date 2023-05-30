#pragma		once

// ****************************************************************
// プレイヤー数
// ================================================================
const int	TNM_BGM_PLAYER_CNT = 2;		// クロスフェード用
const int	TNM_KOE_PLAYER_CNT = 1;		// 単一プレイヤー
const int	TNM_PCM_PLAYER_CNT = 16;	// 汎用プレイヤー
const int	TNM_SE_PLAYER_CNT = 16;		// 汎用プレイヤー

// ****************************************************************
// プレイヤータイプ
// ================================================================
const int	TNM_PLAYER_TYPE_NONE = -1;
const int	TNM_PLAYER_TYPE_BGM = 0;
const int	TNM_PLAYER_TYPE_KOE = 1;
const int	TNM_PLAYER_TYPE_PCM = 2;
const int	TNM_PLAYER_TYPE_SE = 3;

// ****************************************************************
// プレイヤーの状態
// ================================================================
const int	TNM_PLAYER_STATE_FREE = 0;
const int	TNM_PLAYER_STATE_PLAY = 1;
const int	TNM_PLAYER_STATE_FADE_OUT = 2;
const int	TNM_PLAYER_STATE_PAUSE = 3;

// ****************************************************************
// サウンドタイプ
// ================================================================
const int	TNM_SOUND_TYPE_NONE = -1;
const int	TNM_SOUND_TYPE_BGM = 0;
const int	TNM_SOUND_TYPE_KOE = 1;
const int	TNM_SOUND_TYPE_PCM = 2;
const int	TNM_SOUND_TYPE_SE = 3;

// ****************************************************************
// ボリュームチャンネル
// ================================================================
const int	TNM_PLAYER_VOLUME_CHANNEL_SYSTEM = 0;	// システムで設定するボリューム
const int	TNM_PLAYER_VOLUME_CHANNEL_GAME = 1;		// スクリプトで設定するボリューム

// ****************************************************************
// ボリューム
// ================================================================
const int	TNM_VOLUME_MAX = 255;
const int	TNM_VOLUME_MIN = 0;

// ****************************************************************
// プレイヤー
// ================================================================
//		となま専用のプレイヤー。
//		BGM, KOE, PCM, SE などはすべて、このプレイヤーで音を再生します。
// ================================================================
class C_tnm_player
{
public:
	void	init();					// 初期化
	void	reinit();				// 再初期化
	void	free();					// 解放

	bool	play_bgm(int bgm_no, bool loop_flag, int fade_in_time, int start_pos, bool ready_only);
	bool	play_koe(int koe_no, bool loop_flag, int jitan_rate, int play_pos_mills, bool ready_only, bool check_only);
	bool	play_pcm(TSTR file_name, bool loop_flag, int fade_in_time, bool ready_only);
	bool	play_se(int se_no, bool ready_only);
	void	stop(int fade_time = 100);
	void	pause(int fade_time = 100);
	void	resume(int fade_time = 100);
	void	release_sound();

	void	set_system_volume(int volume);
	int		get_system_volume();
	void	set_game_volume(int volume, int fade_time);
	int		get_game_volume();

	bool	is_playing()		{	return m_player.is_playing() && !m_player.is_fade_out() && !m_player.is_pausing();	}	// 再生中
	bool	is_fade_out()		{	return m_player.is_fade_out();			}	// フェードアウト中
	bool	is_pausing()		{	return m_player.is_pausing();			}	// 一時停止中
	bool	is_using()			{	return m_player.is_playing();			}	// 再生中またはフェードアウト中

	int		get_length();					// サンプル数で取得
	int		get_length_by_millsecond();		// ﾐﾘ秒で取得

	int		get_play_pos();								// プレイ位置を取得（サンプル位置で指定）
	void	set_play_pos_by_sample(int sample_no);		// プレイ位置を設定（サンプル位置で指定）
	int		get_play_pos_by_millsecond();				// プレイ位置を取得（ﾐﾘ秒で指定）
	void	set_play_pos_by_millsecond(int mills);		// プレイ位置を設定（ﾐﾘ秒で指定）

	int		get_play_state()
	{
		if (!m_player.is_playing())		return TNM_PLAYER_STATE_FREE;
		if (m_player.is_fade_out())		return TNM_PLAYER_STATE_FADE_OUT;
		return TNM_PLAYER_STATE_PLAY;
	}

	int		get_play_state_2()
	{
		if (!m_player.is_playing())		return TNM_PLAYER_STATE_FREE;
		if (m_player.is_fade_out())		return TNM_PLAYER_STATE_FADE_OUT;
		if (m_player.is_pausing())		return TNM_PLAYER_STATE_PAUSE;
		return TNM_PLAYER_STATE_PLAY;
	}

	int		get_sound_type()	{	return m_sound_type;	}
	int		get_bgm_no()		{	return m_bgm_no;		}
	int		get_koe_no()		{	return m_koe_no;		}
	TSTR	get_file_name()		{	return m_file_name;		}
	int		get_se_no()			{	return m_se_no;			}

	C_sound_stream_base* get_stream()
	{
		return m_player.get_stream();
	}
	float	get_current_mouth_volume();

private:
	bool	__play_wav(TSTR base_dir, TSTR sub_dir, TSTR file_name, bool loop, int fade_time, bool ready_only, bool check_only);
	bool	__play_wav(TSTR base_dir, TSTR sub_dir, TSTR file_name, bool loop, int start_pos, int end_pos, int repeat_pos, int fade_time, bool ready_only, bool check_only);
	void	__set_volume(int volume_channel_no, int volume, int fade_time);
	int		__get_volume(int volume_channel_no);

	C_player	m_player;		// プレイヤー

	int		m_sound_type;	// サウンドタイプ
	int		m_bgm_no;		// 再生中の BGM の登録番号
	int		m_koe_no;		// 再生中の KOE の声番号
	TSTR	m_file_name;	// 再生中の PCM のファイル名
	int		m_se_no;		// 再生中の SE の登録番号

	ARRAY<float>	m_mouth_volume_table;
};

