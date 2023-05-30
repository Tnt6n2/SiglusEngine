#include	"pch.h"

#include	"tnm_eng.h"
#include	"data/tnm_ini.h"
#include	"engine/eng_twitter.h"

// ****************************************************************
// セーブ（コンフィグ）
// ================================================================
bool C_tnm_eng::save_config(C_tnm_save_stream& stream)
{
	// セーブデータをクリア
	stream.clear();

	// セーブストリームにデータをセーブ

	// ユーザコンフィグ：スクリーンサイズ
	stream.save(m_config.screen_size_mode);
	stream.save(m_config.screen_size_mode_window);
	stream.save(m_config.screen_size_scale);
	stream.save(m_config.screen_size_free);

	// ユーザコンフィグ：フルスクリーン詳細
	stream.save(m_config.fullscreen_change_resolution);
	stream.save(m_config.fullscreen_display_cnt);
	stream.save(m_config.fullscreen_display_no);
	stream.save(m_config.fullscreen_resolution_cnt);
	stream.save(m_config.fullscreen_resolution_no);
	stream.save(m_config.fullscreen_resolution);
	stream.save(m_config.fullscreen_mode);
	stream.save(m_config.fullscreen_scale);
	stream.save(m_config.fullscreen_scale_sync_switch);
	stream.save(m_config.fullscreen_move);

	// ユーザコンフィグ：音量
	stream.save(m_config.all_sound_user_volume);
	for (int i = 0; i < TNM_VOLUME_TYPE_MAX; i++)	{
		stream.save(m_config.sound_user_volume[i]);
	}
	stream.save(m_config.play_all_sound_check);
	for (int i = 0; i < TNM_VOLUME_TYPE_MAX; i++)	{
		stream.save(m_config.play_sound_check[i]);
	}

	// ユーザコンフィグ：ＢＧＭフェード
	stream.save(m_config.bgmfade_volume);
	stream.save(m_config.bgmfade_use_check);

	// ユーザコンフィグ：フィルター色
	stream.save(m_config.filter_color);

	// ユーザコンフィグ：フォント
	stream.save(m_config.font_proportional);
	stream.save(m_config.font_name);
	stream.save(m_config.font_shadow);
	stream.save(m_config.font_futoku);

	// ユーザコンフィグ：文字速度
	stream.save(m_config.message_speed);
	stream.save(m_config.message_speed_nowait);

	// ユーザコンフィグ：オートモード
	stream.save(m_config.auto_mode_onoff);
	stream.save(m_config.auto_mode_moji_wait);
	stream.save(m_config.auto_mode_min_wait);

	// ユーザコンフィグ：マウスカーソルを消す
	stream.save(m_config.mouse_cursor_hide_onoff);
	stream.save(m_config.mouse_cursor_hide_time);

	// ユーザコンフィグ：時短再生
	stream.save(m_config.jitan_normal_onoff);
	stream.save(m_config.jitan_auto_mode_onoff);
	stream.save(m_config.jitan_msgbk_onoff);
	stream.save(m_config.jitan_speed);

	// ユーザコンフィグ：音声モード
	stream.save(m_config.koe_mode);

	// ユーザコンフィグ：キャラクター音声
	stream.save(m_config.chrkoe.size());
	stream.save(m_config.chrkoe.get(), (int)m_config.chrkoe.size() * sizeof(S_tnm_config_chrkoe));

	// ユーザコンフィグ：文章の色分け
	stream.save(m_config.message_chrcolor_flag);

	// ユーザコンフィグ：オブジェクト表示
	stream.save(m_config.object_disp_flag.size());
	for (int i = 0; i < (int)m_config.object_disp_flag.size(); i++)	{
		bool tmp = m_config.object_disp_flag[i].value;
		stream.save(tmp);
	}

	// ユーザコンフィグ：グローバル汎用スイッチ
	stream.save(m_config.global_extra_switch_flag.size());
	for (int i = 0; i < (int)m_config.global_extra_switch_flag.size(); i++)	{
		bool tmp = m_config.global_extra_switch_flag[i].value;
		stream.save(tmp);
	}

	// ユーザコンフィグ：グローバル汎用モード
	stream.save(m_config.global_extra_mode_flag.size());
	for (int i = 0; i < (int)m_config.global_extra_mode_flag.size(); i++)	{
		int tmp = m_config.global_extra_mode_flag[i];
		stream.save(tmp);
	}

	// ユーザコンフィグ：システム設定
	stream.save(m_config.system.sleep_flag);
	stream.save(m_config.system.no_wipe_anime_flag);
	stream.save(m_config.system.skip_wipe_anime_flag);
	stream.save(m_config.system.no_mwnd_anime_flag);
	stream.save(m_config.system.wheel_next_message_flag);
	stream.save(m_config.system.koe_dont_stop_flag);
	stream.save(m_config.system.skip_unread_message_flag);

	// ユーザコンフィグ：システム設定（セーブロードの警告）
	stream.save(m_config.system.saveload_alert_flag);

	// ユーザコンフィグ：システム設定（セーブロードのダブルクリック）
	stream.save(m_config.system.saveload_dblclick_flag);

	// ユーザコンフィグ：その他
	stream.save(m_config.ss_path);
	stream.save(m_config.editor_path);
	stream.save(m_config.koe_path);
	stream.save(m_config.koe_tool_path);

	return true;
}

// ****************************************************************
// ロード（コンフィグ）
// ================================================================
bool C_tnm_eng::load_config(C_tnm_save_stream& stream, int major_version, int minor_version)
{
	if (stream.empty())
		return false;

	// シークをリセット
	stream.set_seek();

	// セーブストリームから順にデータをロード

	// ユーザコンフィグ：スクリーンサイズ
	if (major_version >= 2 || (major_version == 1 && minor_version >= 3))
	{
		stream.load(m_config.screen_size_mode);
		stream.load(m_config.screen_size_mode_window);
		stream.load(m_config.screen_size_scale);
		stream.load(m_config.screen_size_free);
	}
	else if (major_version == 1 && minor_version == 2 && Gp_ini->game_id == _T("planetarian [HD Edition]"))
	{
		// planetarian [HD Edition] で唯一、ヘッダが v1.2 で中身が v1.3 のことがあります。

		stream.load(m_config.screen_size_mode);
		stream.load(m_config.screen_size_mode_window);
		stream.load(m_config.screen_size_scale);
		stream.load(m_config.screen_size_free);
	}
	else
	{
		stream.load(m_config.screen_size_mode);
		stream.load(m_config.screen_size_scale);
	}

	// ユーザコンフィグ：フルスクリーン詳細
	stream.load(m_config.fullscreen_change_resolution);
	stream.load(m_config.fullscreen_display_cnt);
	stream.load(m_config.fullscreen_display_no);
	stream.load(m_config.fullscreen_resolution_cnt);
	stream.load(m_config.fullscreen_resolution_no);
	stream.load(m_config.fullscreen_resolution);
	stream.load(m_config.fullscreen_mode);
	stream.load(m_config.fullscreen_scale);
	stream.load(m_config.fullscreen_scale_sync_switch);
	stream.load(m_config.fullscreen_move);

	// ユーザコンフィグ：音量
	stream.load(m_config.all_sound_user_volume);
	for (int i = 0; i < TNM_VOLUME_TYPE_MAX; i++)	{
		stream.load(m_config.sound_user_volume[i]);
	}
	stream.load(m_config.play_all_sound_check);
	for (int i = 0; i < TNM_VOLUME_TYPE_MAX; i++)	{
		stream.load(m_config.play_sound_check[i]);
	}

	// ユーザコンフィグ：ＢＧＭフェード
	stream.load(m_config.bgmfade_volume);
	stream.load(m_config.bgmfade_use_check);

	// ユーザコンフィグ：フィルター色
	stream.load(m_config.filter_color);

	// ユーザコンフィグ：フォント
	stream.load(m_config.font_proportional);
	stream.load(m_config.font_name);
	stream.load(m_config.font_shadow);
	stream.load(m_config.font_futoku);

	// ユーザコンフィグ：文字速度
	stream.load(m_config.message_speed);
	stream.load(m_config.message_speed_nowait);

	// ユーザコンフィグ：オートモード
	stream.load(m_config.auto_mode_onoff);
	stream.load(m_config.auto_mode_moji_wait);
	stream.load(m_config.auto_mode_min_wait);

	// ユーザコンフィグ：マウスカーソルを消す
	stream.load(m_config.mouse_cursor_hide_onoff);
	stream.load(m_config.mouse_cursor_hide_time);

	// ユーザコンフィグ：時短再生
	stream.load(m_config.jitan_normal_onoff);
	stream.load(m_config.jitan_auto_mode_onoff);
	stream.load(m_config.jitan_msgbk_onoff);
	stream.load(m_config.jitan_speed);

	// ユーザコンフィグ：音声モード
	stream.load(m_config.koe_mode);

	// ユーザコンフィグ：キャラクター音声
	m_config.chrkoe.resize(stream.load_ret<size_t>());
	stream.load(m_config.chrkoe.get(), (int)m_config.chrkoe.size() * sizeof(S_tnm_config_chrkoe));

	// ユーザコンフィグ：文章の色分け
	stream.load(m_config.message_chrcolor_flag);

	// ユーザコンフィグ：オブジェクト表示
	m_config.object_disp_flag.resize(stream.load_ret<size_t>());
	for (int i = 0; i < (int)m_config.object_disp_flag.size(); i++)	{
		bool tmp;
		stream.load(tmp);
		m_config.object_disp_flag[i].value = tmp;
	}

	// ユーザコンフィグ：グローバル汎用スイッチ
	m_config.global_extra_switch_flag.resize(stream.load_ret<size_t>());
	for (int i = 0; i < (int)m_config.global_extra_switch_flag.size(); i++)	{
		bool tmp;
		stream.load(tmp);
		m_config.global_extra_switch_flag[i].value = tmp;
	}

	// ユーザコンフィグ：グローバル汎用モード
	m_config.global_extra_mode_flag.resize(stream.load_ret<size_t>());
	for (int i = 0; i < (int)m_config.global_extra_mode_flag.size(); i++)	{
		int tmp;
		stream.load(tmp);
		m_config.global_extra_mode_flag[i] = tmp;
	}

	// ユーザコンフィグ：システム設定
	stream.load(m_config.system.sleep_flag);
	stream.load(m_config.system.no_wipe_anime_flag);
	stream.load(m_config.system.skip_wipe_anime_flag);
	stream.load(m_config.system.no_mwnd_anime_flag);
	stream.load(m_config.system.wheel_next_message_flag);
	stream.load(m_config.system.koe_dont_stop_flag);
	stream.load(m_config.system.skip_unread_message_flag);

	// ユーザコンフィグ：システム設定（セーブロードの警告）
	stream.load(m_config.system.saveload_alert_flag);

	// ユーザコンフィグ：システム設定（セーブロードのダブルクリック）
	stream.load(m_config.system.saveload_dblclick_flag);

	// ユーザコンフィグ：その他
	stream.load(m_config.ss_path);
	stream.load(m_config.editor_path);
	stream.load(m_config.koe_path);
	stream.load(m_config.koe_tool_path);

	// セーブデータをクリアします。
	stream.clear();

	return true;
}

// ****************************************************************
// セーブ（グローバル）
// ================================================================
bool C_tnm_eng::save_global(C_tnm_save_stream& stream)
{
	// セーブデータをクリア
	stream.clear();

	// セーブストリームにデータをセーブ

	// 総プレイ時間
	stream.save(m_timer.global_real_time);

	// フラグ
	m_flag.save_global(stream);

	// ＣＧテーブル
	m_cg_table.save(stream);

	// ＢＧＭテーブル
	m_bgm_table.save(stream);

	// Twitter
	twitter_save_state(stream);

	// キャラクター音声
	stream.save(m_chrkoe.size());
	for (int i = 0; i < (int)m_chrkoe.size(); i++)	{
		stream.save(m_chrkoe[i].name_str);
		stream.save(m_chrkoe[i].look_flag);
	}

	return true;
}

// ****************************************************************
// ロード（グローバル）
// ================================================================
bool C_tnm_eng::load_global(C_tnm_save_stream& stream)
{
	if (stream.empty())
		return false;

	// シークをリセット
	stream.set_seek();

	// セーブストリームから順にデータをロード

	// 総プレイ時間
	stream.load(m_timer.global_real_time);

	// フラグ
	m_flag.load_global(stream);

	// ＣＧテーブル
	m_cg_table.load(stream);

	// ＢＧＭテーブル
	m_bgm_table.load(stream);

	// Twitter
	twitter_load_state(stream);

	// グローバルシステム：キャラクター音声
	m_chrkoe.resize(stream.load_ret<size_t>());
	for (int i = 0; i < (int)m_chrkoe.size(); i++)	{
		stream.load(m_chrkoe[i].name_str);
		stream.load(m_chrkoe[i].look_flag);
	}

	// セーブデータをクリアします。
	stream.clear();

	return true;
}

