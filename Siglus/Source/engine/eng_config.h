#pragma		once

// ****************************************************************
// システムコンフィグ
// ================================================================
//		system.ini で定義する、システム系のコンフィグ
// ================================================================

// ****************************************************************
// システムコンフィグ構造体
// ================================================================
class C_tnm_system_config
{
public:
	int		d3d_adapter_no;
	int		d3d_present_interval;
	int		d3d_wipe_buffer_cnt;
	bool	d3d_hardware_vertex_flag;
	bool	d3d_use_mipmap_flag;
	bool	ds_play_silent_sound_flag;
	int		movie_type;
};

// ****************************************************************
// ユーザコンフィグ
// ================================================================
//		環境設定など、ユーザが操作できるコンフィグ
// ================================================================
void	tnm_format_config();							// 全てフォーマット
void	tnm_init_config_screen_mode();					// 画面モード
void	tnm_init_config_fullscreen();					// フルスクリーンの詳細設定
void	tnm_init_config_volume();						// 音量
void	tnm_init_config_bgmfade();						// ＢＧＭフェード
void	tnm_init_config_filter();						// フィルター色
void	tnm_init_config_font();							// フォント
void	tnm_init_config_message_speed();				// 文字速度
void	tnm_init_config_auto_mode();					// オートモード
void	tnm_init_config_mouse_cursor_hide();			// 自動でマウスカーソルを隠す
void	tnm_init_config_jitan();						// 時短再生
void	tnm_init_config_koe_mode();						// 音声モード
void	tnm_init_config_chrkoe();						// キャラクター音声
void	tnm_init_config_message_chrcolor();				// 文章の色分け
void	tnm_init_config_object_disp();					// オブジェクト表示
void	tnm_init_config_global_extra_switch();			// グローバル汎用スイッチ
void	tnm_init_config_global_extra_mode();			// グローバル汎用モード
void	tnm_init_config_system();						// システム設定
void	tnm_init_config_system_saveload_alert();		// システム設定（セーブロードの警告）
void	tnm_init_config_system_saveload_dblclick();		// システム設定（セーブロードのダブルクリック）

void	tnm_init_system_config_play_silent_sound();		// システム設定（無音を再生する）
void	tnm_init_system_config_movie_play_type();		// システム設定（ムービーの再生方法）
