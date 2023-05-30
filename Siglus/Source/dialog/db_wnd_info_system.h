#pragma		once

#include	"data/tnm_proc_def.h"
#include	"dialog/db_wnd_info_listview.h"

// ****************************************************************
// デバッグ情報ウィンドウ：システム情報
// ================================================================
class C_db_wnd_info_system : public C_db_wnd_info_listview
{
public:
	void		init();		// 初期化
	void		frame();	// フレーム

private:

	// ステータス
	struct STATE
	{
		int				frame_rate;					// フレームレート
		TSTR			save_point;					// セーブポイント
		bool			sel_point_exist;			// 選択肢ポイント
		bool			capture_exist_flag;			// キャプチャーが存在する
		bool			ex_call_flag;				// ＥＸコール中
		bool			system_wipe_flag;			// システムワイプ中
		bool			game_timer_move_flag;		// ゲーム時刻が動いてる
		int				stack_size;					// スタックのサイズ
		int				element_stack_size;			// エレメントスタックのサイズ
		int				msg_back_save_size;			// メッセージバックセーブのサイズ

		int				real_time;					// 現在の実時刻
		int				local_real_time;			// ローカルの実時刻
		int				local_game_time;			// ローカルのゲーム時刻
		int				local_wipe_time;			// ローカルのワイプ時刻
		bool			ctrl_skip_flag;				// Ctrl スキップ中
		int				skip_speed;					// スキップ速度
		bool			cant_auto_skip_before_click;	// スキップを止めた

		bool			active_flag;				// ウィンドウがアクティブである
		bool			focused_flag;				// ウィンドウにフォーカスがある
		bool			focused_flag_on_edit_box;	// エディットボックスにフォーカスがある

		int				total_bgm_fade;				// 最終ＢＧＭフェード値
		int				total_bgm_fade2;			// 最終ＢＧＭフェード値
		int				total_bgm_volume;			// 最終ＢＧＭボリューム
		int				total_koe_volume;			// 最終声ボリューム
		int				total_pcm_volume;			// 最終効果音ボリューム
		int				total_se_volume;			// 最終システム音ボリューム
		int				total_mov_volume;			// 最終ムービーボリューム
		int				mov_volume;
		int				mov_volume_real;
		bool			play_silent_sound;			// バックグラウンドで無音を再生する

		int				video_memory_use_size;		// ビデオメモリ使用サイズ
		int				video_memory_use_max_size;	// ビデオメモリ使用サイズ（最大値）
		int				max_vertex_buffer_size;		// 頂点バッファサイズ
		int				moji_map_use_cnt;			// 使用中の文字マップ数
		int				moji_map_now_cnt;			// 現在の文字マップ数
		int				moji_map_max_cnt;			// 最大の文字マップ数
		int				moji_memory_use_size;		// 使用中の文字メモリーサイズ
		int				moji_memory_now_size;		// 現在の文字使用メモリーサイズ
		int				moji_memory_max_size;		// 最大の文字使用メモリーサイズ
		int				moji_kind_useful_cnt;		// 有益な文字数
		int				moji_kind_unuseful_cnt;		// 非有益な文字数
		int				gan_data_map_now_cnt;		// 現在のＧＡＮデータマップ数
		int				gan_data_map_max_cnt;		// 最大のＧＡＮデータマップ数

		int				test1;
		int				test2;
		int				test3;
		int				test4;
		int				test5;
		int				test6;
		int				test7;
		int				test8;
		int				test9;

	}	m_ld;

	void		on_init_func();		// 初期化ファンクション
	void		on_close_func();	// 閉じるファンクション
};

