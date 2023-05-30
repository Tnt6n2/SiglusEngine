#pragma		once

#include	"data/tnm_sorter.h"
#include	"data/tnm_timer.h"

struct S_element;

// ****************************************************************
// システムコマンド処理
// ================================================================
//		右クリックメニュー、ウィンドウボタンなどから行う処理
// ================================================================
void	tnm_syscom_init_syscom_flag();							// システムコマンドフラグを初期化

bool	tnm_syscom_open();										// システムコマンドを開く

bool	tnm_syscom_restart_from_start();						// 最初からはじめる
bool	tnm_syscom_restart_from_scene(int scn_no, int z_no);	// シーンからはじめる
bool	tnm_syscom_change_break();								// ブレーク開始／解除
bool	tnm_syscom_do_one_step();								// ステップ実行

void	tnm_syscom_read_skip_set_onoff_flag(bool onoff);		// 既読早送りの ON/OFF を設定
bool	tnm_syscom_read_skip_get_onoff_flag();					// 既読早送りの ON/OFF を取得
void	tnm_syscom_read_skip_set_enable_flag(bool enable);		// 既読早送りの許可／不許可を設定
bool	tnm_syscom_read_skip_get_enable_flag();					// 既読早送りの許可／不許可を取得
void	tnm_syscom_read_skip_set_exist_flag(bool exist);		// 既読早送りの表示／非表示を設定
bool	tnm_syscom_read_skip_get_exist_flag();					// 既読早送りの表示／非表示を取得
bool	tnm_syscom_read_skip_is_enable();						// 既読早送りが使用可能かを判定

void	tnm_syscom_auto_skip_set_onoff_flag(bool onoff);		// 自動早送りの ON/OFF を設定
bool	tnm_syscom_auto_skip_get_onoff_flag();					// 自動早送りの ON/OFF を取得
void	tnm_syscom_auto_skip_set_enable_flag(bool enable);		// 自動早送りの許可／不許可を設定
bool	tnm_syscom_auto_skip_get_enable_flag();					// 自動早送りの許可／不許可を取得
void	tnm_syscom_auto_skip_set_exist_flag(bool exist);		// 自動早送りの表示／非表示を設定
bool	tnm_syscom_auto_skip_get_exist_flag();					// 自動早送りの表示／非表示を取得
bool	tnm_syscom_auto_skip_is_enable();						// 自動早送りが使用可能かを判定

void	tnm_syscom_auto_mode_set_onoff_flag(bool onoff);		// オートモードの ON/OFF を設定
bool	tnm_syscom_auto_mode_get_onoff_flag();					// オートモードの ON/OFF を取得
void	tnm_syscom_auto_mode_set_enable_flag(bool enable);		// オートモードの許可／不許可を設定
bool	tnm_syscom_auto_mode_get_enable_flag();					// オートモードの許可／不許可を取得
void	tnm_syscom_auto_mode_set_exist_flag(bool exist);		// オートモードの表示／非表示を設定
bool	tnm_syscom_auto_mode_get_exist_flag();					// オートモードの表示／非表示を取得
bool	tnm_syscom_auto_mode_is_enable();						// オートモードが使用可能かを判定

void	tnm_syscom_hide_mwnd_set_onoff_flag(bool onoff);		// メッセージウィンドウを閉じるの ON/OFF を設定
bool	tnm_syscom_hide_mwnd_get_onoff_flag();					// メッセージウィンドウを閉じるの ON/OFF を取得
void	tnm_syscom_hide_mwnd_set_enable_flag(bool enable);		// メッセージウィンドウを閉じるの許可／不許可を設定
bool	tnm_syscom_hide_mwnd_get_enable_flag();					// メッセージウィンドウを閉じるの許可／不許可を取得
void	tnm_syscom_hide_mwnd_set_exist_flag(bool exist);		// メッセージウィンドウを閉じるの表示／非表示を設定
bool	tnm_syscom_hide_mwnd_get_exist_flag();					// メッセージウィンドウを閉じるの表示／非表示を取得
bool	tnm_syscom_hide_mwnd_is_enable();						// メッセージウィンドウを閉じるが使用可能かを判定

void	tnm_syscom_open_msg_back();								// メッセージバックを開く
void	tnm_syscom_close_msg_back();							// メッセージバックを閉じる
void	tnm_syscom_msg_back_set_enable_flag(bool enable);		// メッセージバックの許可／不許可を設定
bool	tnm_syscom_msg_back_get_enable_flag();					// メッセージバックの許可／不許可を取得
void	tnm_syscom_msg_back_set_exist_flag(bool exist);			// メッセージバックの表示／非表示を設定
bool	tnm_syscom_msg_back_get_exist_flag();					// メッセージバックの表示／非表示を取得
bool	tnm_syscom_msg_back_is_enable();						// メッセージバックが使用可能かを判定

bool	tnm_syscom_open_save(bool dialog_flag, bool capture_flag, bool enable_check);	// セーブを開く
void	tnm_syscom_open_save_set_enable_flag(bool enable);								// セーブの許可／不許可を設定
bool	tnm_syscom_open_save_get_enable_flag();											// セーブの許可／不許可を取得
void	tnm_syscom_open_save_set_exist_flag(bool exist);								// セーブの表示／非表示を設定
bool	tnm_syscom_open_save_get_exist_flag();											// セーブの表示／非表示を取得
bool	tnm_syscom_open_save_is_enable();												// セーブが使用可能かを判定

bool	tnm_syscom_open_load(bool dialog_flag, bool enable_check);						// ロードを開く
void	tnm_syscom_open_load_set_enable_flag(bool enable);								// ロードの許可／不許可を設定
bool	tnm_syscom_open_load_get_enable_flag();											// ロードの許可／不許可を取得
void	tnm_syscom_open_load_set_exist_flag(bool exist);								// ロードの表示／非表示を設定
bool	tnm_syscom_open_load_get_exist_flag();											// ロードの表示／非表示を取得
bool	tnm_syscom_open_load_is_enable();												// ロードが使用可能かを判定

bool	tnm_syscom_save(int save_no, bool warning, bool se_play);						// セーブ
bool	tnm_syscom_save_is_enable(int save_no);											// セーブが使用可能かを判定

bool	tnm_syscom_quick_save(int quick_save_no, bool warning, bool se_play);			// クイックセーブ
bool	tnm_syscom_quick_save_is_enable(int quick_save_no);								// クイックセーブが使用可能かを判定

bool	tnm_syscom_end_save(bool warning, bool se_play);								// エンドセーブ
bool	tnm_syscom_end_save_period_2(bool capture_flag);								// エンドセーブ（２段階目）
bool	tnm_syscom_end_save_is_enable();												// エンドセーブが使用可能かを判定

bool	tnm_syscom_load(int save_no, bool warning, bool se_play, bool fade_out);		// ロード
bool	tnm_syscom_load_easy(int save_no);												// ロード（デバッグ用：簡易バージョン）
bool	tnm_syscom_load_is_enable(int save_no);											// ロードが使用可能かを判定

bool	tnm_syscom_quick_load(int save_no, bool warning, bool se_play, bool fade_out);	// クイックロード
bool	tnm_syscom_quick_load_is_enable(int save_no);									// クイックロードが使用可能かを判定

bool	tnm_syscom_end_load(bool warning, bool se_play, bool fade_out);					// エンドロード
bool	tnm_syscom_end_load_is_enable();												// エンドロードが使用可能かを判定

bool	tnm_syscom_inner_load(int inner_save_no, bool warning, bool se_play, bool fade_out);	// インナーロード
bool	tnm_syscom_inner_load_is_enable();														// インナーロードが使用可能かを判定

bool	tnm_syscom_msgbk_load(S_tid save_id, bool warning, bool se_play, bool fade_out);		// メッセージバックロード

bool	tnm_syscom_is_exist_capture();													// キャプチャーがあるかを判定する
void	tnm_syscom_create_capture(int prior, bool disp);								// キャプチャーを準備する
void	tnm_syscom_free_capture(int prior);												// キャプチャーを解放する
void	tnm_syscom_create_capture_for_object(S_tnm_sorter sorter, bool disp);			// オブジェクト用のキャプチャーを準備する
void	tnm_syscom_free_capture_for_object();											// オブジェクト用のキャプチャーを解放する
void	tnm_syscom_create_capture_for_local_save(S_tnm_sorter sorter, int width, int height, bool disp);		// ローカルセーブ用のキャプチャーを準備する
void	tnm_syscom_create_capture_for_tweet();											// ツイート用のキャプチャーを準備する
void	tnm_syscom_free_capture_for_tweet();											// ツイート用のキャプチャーを解放する

void	tnm_syscom_create_capture_buffer(int width, int height);											// キャプチャーバッファを作成
void	tnm_syscom_destroy_capture_buffer();																// キャプチャーバッファを破壊
void	tnm_syscom_buffer_capture(int x, int y, CTSTR& file_name);											// バッファキャプチャーを行う

bool	tnm_syscom_save_capture_buffer_to_file(CTSTR& file_name, TSTR extension, bool dialog, CTSTR& dialog_title, S_element* p_flag_element, int start_flag, int flag_cnt, S_element* p_str_flag_element, int start_str_flag, int str_flag_cnt);
bool	tnm_syscom_load_flag_from_capture_file(CTSTR& file_name, TSTR extension, bool dialog, CTSTR& dialog_title, S_element* p_flag_element, int start_flag, int flag_cnt, S_element* p_str_flag_element, int start_str_flag, int str_flag_cnt);

void	tnm_syscom_return_to_sel(bool warning, bool se_play, bool fade_out);			// 前の選択肢に戻る
void	tnm_syscom_return_to_sel_set_enable_flag(bool enable);							// 前の選択肢に戻るの許可／不許可を設定
bool	tnm_syscom_return_to_sel_get_enable_flag();										// 前の選択肢に戻るの許可／不許可を取得
void	tnm_syscom_return_to_sel_set_exist_flag(bool exist);							// 前の選択肢に戻るの表示／非表示を設定
bool	tnm_syscom_return_to_sel_get_exist_flag();										// 前の選択肢に戻るの表示／非表示を取得
bool	tnm_syscom_return_to_sel_is_enable();											// 前の選択肢に戻るが使用可能かを判定

void	tnm_syscom_return_to_menu(bool warning, bool se_play, bool fade_out, bool msgback_except);		// メニューに戻る
void	tnm_syscom_return_to_menu_set_enable_flag(bool enable);							// メニューに戻るの許可／不許可を設定
bool	tnm_syscom_return_to_menu_get_enable_flag();									// メニューに戻るの許可／不許可を取得
void	tnm_syscom_return_to_menu_set_exist_flag(bool exist);							// メニューに戻るの表示／非表示を設定
bool	tnm_syscom_return_to_menu_get_exist_flag();										// メニューに戻るの表示／非表示を取得
bool	tnm_syscom_return_to_menu_is_enable();											// メニューに戻るが使用可能かを判定

void	tnm_syscom_end_game(bool warning, bool se_play, bool fade_out);					// ゲームを終了する
void	tnm_syscom_end_game_set_enable_flag(bool enable);								// ゲームを終了するの許可／不許可を設定
bool	tnm_syscom_end_game_get_enable_flag();											// ゲームを終了するの許可／不許可を取得
void	tnm_syscom_end_game_set_exist_flag(bool exist);									// ゲームを終了するの表示／非表示を設定
bool	tnm_syscom_end_game_get_exist_flag();											// ゲームを終了するの表示／非表示を取得
bool	tnm_syscom_end_game_is_enable();												// ゲームを終了するが使用可能かを判定

void	tnm_syscom_koe_play();															// 声を再生する
bool	tnm_syscom_koe_play_is_enable();												// 声を再生するが使用可能かを判定

void	tnm_syscom_open_config();														// 環境設定を開く
void	tnm_syscom_open_config_set_enable_flag(bool enable);							// 環境設定を開くの許可／不許可を設定
bool	tnm_syscom_open_config_get_enable_flag();										// 環境設定を開くの許可／不許可を取得
void	tnm_syscom_open_config_set_exist_flag(bool exist);								// 環境設定を開くの表示／非表示を設定
bool	tnm_syscom_open_config_get_exist_flag();										// 環境設定を開くの表示／非表示を取得
bool	tnm_syscom_open_config_is_enable();												// 環境設定を開くが使用可能かを判定

void	tnm_syscom_local_extra_switch_set_onoff_flag(int switch_no, bool onoff);		// ローカル汎用スイッチの ON/OFF を設定
bool	tnm_syscom_local_extra_switch_get_onoff_flag(int switch_no);					// ローカル汎用スイッチの ON/OFF を取得
void	tnm_syscom_local_extra_switch_turn_onoff_flag(int switch_no);					// ローカル汎用スイッチの ON/OFF を反転
void	tnm_syscom_local_extra_switch_set_enable_flag(int switch_no, bool enable);		// ローカル汎用スイッチの許可／不許可を設定
bool	tnm_syscom_local_extra_switch_get_enable_flag(int switch_no);					// ローカル汎用スイッチの許可／不許可を取得
void	tnm_syscom_local_extra_switch_set_exist_flag(int switch_no, bool exist);		// ローカル汎用スイッチの表示／非表示を設定
bool	tnm_syscom_local_extra_switch_get_exist_flag(int switch_no);					// ローカル汎用スイッチの表示／非表示を取得
bool	tnm_syscom_local_extra_switch_is_enable(int switch_no);							// ローカル汎用スイッチが使用可能かを判定

void	tnm_syscom_local_extra_mode_set_value(int mode_no, int value);					// ローカル汎用モードの値を設定
int		tnm_syscom_local_extra_mode_get_value(int mode_no);								// ローカル汎用モードの値を取得
void	tnm_syscom_local_extra_mode_increment(int mode_no);								// ローカル汎用モードの値を進める
void	tnm_syscom_local_extra_mode_set_enable_flag(int mode_no, bool enable);			// ローカル汎用モードの許可／不許可を設定
bool	tnm_syscom_local_extra_mode_get_enable_flag(int mode_no);						// ローカル汎用モードの許可／不許可を取得
void	tnm_syscom_local_extra_mode_set_exist_flag(int mode_no, bool exist);			// ローカル汎用モードの表示／非表示を設定
bool	tnm_syscom_local_extra_mode_get_exist_flag(int mode_no);						// ローカル汎用モードの表示／非表示を取得
bool	tnm_syscom_local_extra_mode_is_enable(int mode_no);								// ローカル汎用モードが使用可能かを判定

void	tnm_syscom_manual_set_enable_flag(bool enable);									// マニュアルの許可／不許可を設定
bool	tnm_syscom_manual_get_enable_flag();											// マニュアルの許可／不許可を取得
void	tnm_syscom_manual_set_exist_flag(bool exist);									// マニュアルの表示／非表示を設定
bool	tnm_syscom_manual_get_exist_flag();												// マニュアルの表示／非表示を取得

void	tnm_syscom_version_set_enable_flag(bool enable);								// バージョン情報の許可／不許可を設定
bool	tnm_syscom_version_get_enable_flag();											// バージョン情報の許可／不許可を取得
void	tnm_syscom_version_set_exist_flag(bool exist);									// バージョン情報の表示／非表示を設定
bool	tnm_syscom_version_get_exist_flag();											// バージョン情報の表示／非表示を取得

void	tnm_syscom_cancel_set_enable_flag(bool enable);									// キャンセルの許可／不許可を設定
bool	tnm_syscom_cancel_get_enable_flag();											// キャンセルの許可／不許可を取得
void	tnm_syscom_cancel_set_exist_flag(bool exist);									// キャンセルの表示／非表示を設定
bool	tnm_syscom_cancel_get_exist_flag();												// キャンセルの表示／非表示を取得
bool	tnm_syscom_cancel_is_enable();													// キャンセルが使用可能かを判定


void	tnm_syscom_close_window();														// ウィンドウを閉じる

// ****************************************************************
// システムコマンド処理（補助）
// ================================================================
//		通常は直接は呼ばれないが、２重処理などのために直接呼ばれることがある
// ================================================================

bool	tnm_open_save_dialog(bool capture_flag);				// セーブダイアログを開く


