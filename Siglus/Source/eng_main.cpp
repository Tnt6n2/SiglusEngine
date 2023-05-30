#include	"pch.h"
#include	"tnm_eng.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_net.h"
#include	"data/tnm_dlg_manager.h"
#include	"data/tnm_gan_manager.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"

#include	"engine/eng_dialog.h"
#include	"engine/eng_config.h"
#include	"engine/ifc_sound.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// プロトタイプ宣言
// ================================================================

// エンジン：メッセージループ
bool tnm_eng_message_loop();

// エンジン：異常終了
void tnm_eng_fatal_error();
void tnm_eng_fatal_error(CTSTR& str);

// ****************************************************************
// グローバルなポインタ
// ================================================================
C_play_manager						G_pm;				// プレイマネージャ
C_tnm_wnd*							Gp_wnd;				// ウィンドウ
C_tnm_movie_wnd*					Gp_movie_wnd;		// ムービーウィンドウ
C_tnm_net*							Gp_net;				// ネットワーク
C_tnm_dlg_manager*					Gp_dlg_mng;			// ダイアログマネージャ
C_tnm_input*						Gp_input;			// 入力
C_input_state*						Gp_cur_input;		// 今回の入力
C_input_state*						Gp_last_input;		// 前回の入力
C_tnm_system_info*					Gp_system_info;		// システム情報
C_tnm_system_config*				Gp_system_config;	// システムコンフィグ
C_tnm_global_data*					Gp_global;			// グローバルデータ
C_tnm_dir*							Gp_dir;				// ディレクトリ
C_tnm_config*						Gp_config;			// ユーザコンフィグ
C_tnm_syscom*						Gp_syscom;			// システムコマンド
C_tnm_local_data*					Gp_local;			// シーンデータ
S_tnm_local_save*					Gp_local_save;		// ローカルセーブ
S_tnm_save_history*					Gp_save_history;	// セーブ履歴
S_tnm_backlog_save*					Gp_backlog_save;	// バックログセーブ
C_tnm_scene_lexer*					Gp_lexer;			// レクサー
C_tnm_stack*						Gp_stack;			// スタック
C_tnm_timer*						Gp_timer;			// タイマー
C_tnm_icon*							Gp_icon;			// アイコン
C_tnm_cursor_list*					Gp_cursor_list;		// カーソル
C_tnm_font*							Gp_font;			// フォント
C_font_name_list*					Gp_font_name_list;	// フォント名リスト
C_tnm_cg_table*						Gp_cg_table;		// ＣＧテーブル
C_tnm_bgm_table*					Gp_bgm_table;		// ＢＧＭテーブル
C_tnm_thumb_table*					Gp_thumb_table;		// サムネイルテーブル
ARRAY< ARRAY<BYTE> >*				Gp_read_flag;		// 既読フラグ
C_tnm_namae*						Gp_namae;			// 【名前】
ARRAY<C_tnm_chrkoe>*				Gp_chrkoe;			// キャラクター音声
C_tnm_color_table*					Gp_color_table;		// カラーテーブル
C_tnm_tonecurve*					Gp_tonecurve;		// トーンカーブ
C_tnm_fog*							Gp_fog;				// フォグ
C_tnm_wipe*							Gp_wipe;			// ワイプ
C_tnm_btn_mng*						Gp_btn_mng;			// ボタンマネージャ
C_tnm_btn_action_template_list*		Gp_btn_action_list;	// ボタンアクション
C_tnm_btn_se_template_list*			Gp_btn_se_list;		// ボタンシステム音
C_tnm_script_input*					Gp_script_input;	// スクリプト入力
C_tnm_fetch_point*					Gp_fetch_point;		// フェッチポイント
C_tnm_click_disable*				Gp_click_disable;	// クリック禁止
C_tnm_error*						Gp_error;			// エラー
C_tnm_debug_dll*					Gp_debug_dll;		// デバッグ dll
C_tnm_flag_ini*						Gp_flag_ini;		// Flag.ini
C_tnm_steam*						Gp_steam;			// Steam

C_elm_flag*							Gp_flag;				// フラグ
C_elm_user_inc_prop*				Gp_user_inc_prop;		// ユーザインクプロパティ
C_elm_user_scn_prop_list*			Gp_user_scn_prop_list;	// ユーザシーンプロパティ
C_elm_database_list*				Gp_database_list;		// データベース
C_elm_counter_list*					Gp_counter_list;		// カウンタ
C_elm_frame_action*					Gp_frame_action;		// フレームアクション
C_elm_frame_action_list*			Gp_frame_action_ch;		// フレームアクションチャンネル
C_elm_g00_buf_list*					Gp_g00_buf_list;		// Ｇ００バッファ
C_elm_mask_list*					Gp_mask_list;			// マスク
C_elm_stage_list*					Gp_stage_list;			// ステージ
C_elm_msg_back*						Gp_msg_back;			// メッセージバック
C_elm_screen*						Gp_screen;				// スクリーン
C_elm_sound*						Gp_sound;				// サウンド
C_elm_pcm_event_list*				Gp_pcm_event_list;		// 効果音イベント
C_elm_editbox_list*					Gp_editbox_list;		// エディットボックス
C_elm_call_list*					Gp_call_list;			// コール
C_elm_excall*						Gp_excall;				// ＥＸコール

// ****************************************************************
// エンジン：コンストラクタ
// ================================================================
C_tnm_eng::C_tnm_eng()
{
	m_init_success_flag = false;

	// グローバルなポインタの設定
	Gp_wnd = &m_wnd;
	Gp_movie_wnd = &m_movie_wnd;
	Gp_net = &m_net;
	Gp_dlg_mng = &m_dlg_mng;
	Gp_input = &m_input;
	Gp_cur_input = &m_cur_input;
	Gp_last_input = &m_last_input;
	Gp_system_info = &m_system_info;
	Gp_system_config = &m_system_config;
	Gp_global = &m_global;
	Gp_dir = &m_dir;
	Gp_config = &m_config;
	Gp_syscom = &m_syscom;
	Gp_local = &m_local;
	Gp_local_save = &m_local_save;
	Gp_save_history = &m_save_history;
	Gp_backlog_save = &m_backlog_save;
	Gp_lexer = &m_lexer;
	Gp_stack = &m_stack;
	Gp_timer = &m_timer;
	Gp_icon = &m_icon;
	Gp_cursor_list = &m_cursor_list;
	Gp_font = &m_font;
	Gp_font_name_list = &m_font_name_list;
	Gp_cg_table = &m_cg_table;
	Gp_bgm_table = &m_bgm_table;
	Gp_thumb_table = &m_thumb_table;
	Gp_read_flag = &m_read_flag_list;
	Gp_namae = &m_namae;
	Gp_chrkoe = &m_chrkoe;
	Gp_color_table = &m_color_table;
	Gp_tonecurve = &m_tonecurve;
	Gp_fog = &m_fog;
	Gp_wipe = &m_wipe;
	Gp_btn_mng = &m_btn_mng;
	Gp_btn_action_list = &m_btn_action_template_list;
	Gp_btn_se_list = &m_btn_se_template_list;
	Gp_script_input = &m_script_input;
	Gp_fetch_point = &m_fetch_point;
	Gp_click_disable = &m_click_disable;
	Gp_error = &m_error;
	Gp_debug_dll = &m_debug_dll;
	Gp_flag_ini = &m_flag_ini;

	Gp_flag = &m_flag;
	Gp_user_inc_prop = &m_user_inc_prop;
	Gp_user_scn_prop_list = &m_user_scn_prop_list;
	Gp_database_list = &m_database_list;
	Gp_counter_list = &m_counter_list;
	Gp_frame_action = &m_frame_action;
	Gp_frame_action_ch = &m_frame_action_ch_list;
	Gp_g00_buf_list = &m_g00_buf_list;
	Gp_mask_list = &m_mask_list;
	Gp_stage_list = &m_stage_list;
	Gp_msg_back = &m_msg_back;
	Gp_screen = &m_screen;
	Gp_sound = &m_sound;
	Gp_pcm_event_list = &m_pcm_event_list;
	Gp_editbox_list = &m_editbox_list;
	Gp_call_list = &m_call_list;
	Gp_excall = &m_excall;

#if ___USE_STEAM
	Gp_steam = &m_steam;
#endif
}

// ****************************************************************
// エンジン：メイン処理
// ================================================================
bool C_tnm_eng::main()
{
	// エンジンを初期化する
	try	{
		if (!init())	{
			tnm_eng_fatal_error();
		}
	}
	// 例外を受け取って表示する
	catch (TCHAR* str)	{
		tnm_eng_fatal_error(_T("ゲームの初期化処理で例外が発生しました。ゲームを終了します。\n") + TSTR(str));
	}
	catch (TSTR str)	{
		tnm_eng_fatal_error(_T("ゲームの初期化処理で例外が発生しました。ゲームを終了します。\n") + str);
	}
	catch (const std::exception& e)	{
		tnm_eng_fatal_error(_T("ゲームの初期化処理で例外が発生しました。ゲームを終了します。\n") + MBSTR_to_TSTR(e.what()));
	}

	// エラーが起きていなければエンジンを開始
	if (!Gp_global->game_end_flag)	{

		// エンジンの開始処理
		try	{
			if (!start())	{
				tnm_eng_fatal_error(_T("ゲームの開始処理でエラーが発生しました。ゲームを終了します。\n") + get_last_error_msg());
			}
		}
		// 例外を受け取って表示する
		catch (TCHAR* str)	{
			tnm_eng_fatal_error(_T("ゲームの開始処理で例外が発生しました。ゲームを終了します。\n") + TSTR(str));
		}
		catch (TSTR str)	{
			tnm_eng_fatal_error(_T("ゲームの開始処理で例外が発生しました。ゲームを終了します。\n") + str);
		}
		catch (const std::exception& e)	{
			tnm_eng_fatal_error(_T("ゲームの開始処理で例外が発生しました。ゲームを終了します。\n") + MBSTR_to_TSTR(e.what()));
		}
	}

	// ウィンドウがある限りゲームループを実行し続ける
	while (Gp_wnd->get_handle())	{

		// ゲーム終了判定（念のためここでも）
		if (Gp_wnd->get_handle() && Gp_global->game_end_flag)	{
			Gp_wnd->close();
		}

		// メッセージループを行う。WM_QUIT で抜ける
		if (!tnm_eng_message_loop())
			break;

		// ゲーム終了判定（この位置は必須です。WM_CLOSE などでフラグが立つことがあります。）
		if (Gp_wnd->get_handle() && Gp_global->game_end_flag)	{
			Gp_wnd->close();
		}

		// キャプチャー用の空ループ
		while (1)	{

			// ゲーム終了判定（フレーム処理でフラグが立つ場合の処理）
			if (Gp_global->game_end_flag)
				break;

			// エンジンのフレーム処理
			try	{
				if (!frame())	{
					tnm_eng_fatal_error(_T("ゲームのフレーム処理でエラーが発生しました。ゲームを終了します。\n") + get_last_error_msg());
				}
			}
			// 例外を受け取って表示する
			catch (TCHAR* str)	{
				tnm_eng_fatal_error(_T("ゲームのフレーム処理で例外が発生しました。ゲームを終了します。\n") + TSTR(str));
			}
			catch (TSTR str)	{
				tnm_eng_fatal_error(_T("ゲームのフレーム処理で例外が発生しました。ゲームを終了します。\n") + str);
			}
			catch (const std::exception& e)	{
				tnm_eng_fatal_error(_T("ゲームのフレーム処理で例外が発生しました。ゲームを終了します。\n") + MBSTR_to_TSTR(e.what()));
			}

			// ゲーム終了判定（フレーム処理でフラグが立つ場合の処理）
			if (Gp_global->game_end_flag)
				break;

			// 描画処理
			if (Gp_global->disp_flag)	{

				// エンジンの描画処理
				try	{
					if (!Gp_wnd->disp_proc())	{
						tnm_eng_fatal_error(_T("ゲームの描画処理でエラーが発生しました。ゲームを終了します。\n") + get_last_error_msg());
					}
				}
				// 例外を受け取って表示する
				catch (TCHAR* str)	{
					tnm_eng_fatal_error(_T("ゲームの描画処理で例外が発生しました。ゲームを終了します。\n") + TSTR(str));
				}
				catch (TSTR str)	{
					tnm_eng_fatal_error(_T("ゲームの描画処理で例外が発生しました。ゲームを終了します。\n") + str);
				}
				catch (const std::exception& e)	{
					tnm_eng_fatal_error(_T("ゲームの描画処理で例外が発生しました。ゲームを終了します。\n") + MBSTR_to_TSTR(e.what()));
				}
			}

			// キャプチャーの後処理
			if (Gp_global->capture_type == TNM_CAPTURE_TYPE_THUMB)	{
				tnm_stack_push_int(Gp_global->capture_time);	// キャプチャーコマンドの戻り値を積む
			}
			// キャプチャー終了
			Gp_global->capture_type = TNM_CAPTURE_TYPE_NONE;

			// ゲーム終了判定（描画処理でフラグが立つ場合の処理）
			if (Gp_global->game_end_flag)
				break;

			// 画面更新を行うなら抜ける
			if (Gp_global->present_flag)
				break;
		}

		// ゲーム終了判定（上の空ループ内でフラグが立つ場合の処理）
		if (Gp_wnd->get_handle() && Gp_global->game_end_flag)	{
			Gp_wnd->close();
		}

		if (!Gp_global->game_end_flag)	{

			// 画面を更新する
			Gp_wnd->invalidate_rect(NULL, false);

			// キャプションを更新する
			Gp_wnd->update_caption();

			// ダイアログマネージャの処理
			Gp_dlg_mng->frame();

			// リソースマネージャを整理する
			static DWORD last_time = ::timeGetTime();
			DWORD now_time = ::timeGetTime();
			if ((int)(now_time - last_time) >= 1000)	{
				G_rsm.organize();
				G_gan_manager.organize();		// ＧＡＮデータのリソースマネージャ
				last_time = now_time;
			}

			// ★ムービー再生中はスリープをはさむ
			if (tnm_is_movie_playing())	{
				Sleep(100);
			}
			// ★最小化時はスリープをはさむ
			else if (IsIconic(G_app.h_wnd))	{
				Sleep(100);
			}
			// 環境設定によってはスリープをはさむ
			else if (Gp_config->system.sleep_flag)	{
				Sleep(10);
			}
		}
	}

	// ウィンドウが開いていればウィンドウを閉じる
	//Gp_wnd->close();

	// エンジンを終了させる
	try	{
		if (!end())	{
			tnm_eng_fatal_error(_T("ゲームの終了処理でエラーが発生しました。\n") + get_last_error_msg());
		}
	}
	// 例外を受け取って表示する
	catch (TCHAR* str)	{
		tnm_eng_fatal_error(_T("ゲームの終了処理で例外が発生しました。\n") + TSTR(str));
	}
	catch (TSTR str)	{
		tnm_eng_fatal_error(_T("ゲームの終了処理で例外が発生しました。\n") + str);
	}
	catch (const std::exception& e)	{
		tnm_eng_fatal_error(_T("ゲームの終了処理で例外が発生しました。\n") + MBSTR_to_TSTR(e.what()));
	}

	return true;
}

// ****************************************************************
// エンジン：メッセージループ
// ================================================================
bool tnm_eng_message_loop()
{
	MSG msg;

	// メッセージを取り出す
	while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))	{

		// WM_QUIT が来たら false
		if (!GetMessage(&msg, NULL, 0, 0))
			return false;
		// メッセージ処理
		if (!G_app.default_message_proc(&msg))
			return false;
	}

	// メッセージが来なかったら戻る
	return true;
}

// ****************************************************************
// エンジン：異常終了する
// ================================================================
void tnm_eng_fatal_error()
{
	// 異常終了するフラグ
	Gp_global->game_end_flag = true;
	Gp_global->game_end_no_warning = true;
	Gp_global->game_end_save_done = true;
}

void tnm_eng_fatal_error(CTSTR& str)
{
	// 異常終了の場合はメッセージボックスを出す
	// MB_ICONERROR だと音が心臓に悪いので、MB_ICONWARNING にしておく
	message_box(str, Gp_ini->game_name, MB_ICONWARNING);

	// 異常終了するフラグ
	Gp_global->game_end_flag = true;
	Gp_global->game_end_no_warning = true;
	Gp_global->game_end_save_done = true;
}

