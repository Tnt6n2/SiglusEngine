#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_flag_ini.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_dlg_manager.h"
#include	"element/elm_stage.h"
#include	"engine/eng_scene.h"
#include	"../resource.h"

// ****************************************************************
// ダイアログウィンドウ
// ================================================================

// 登録
void C_tnm_dlg_manager::regist()
{
	// 環境設定ベース
	dlg_list.push_back(&cfg_wnd_config_base);					// ベース
	dlg_list.push_back(&cfg_wnd_config_base_full_mode);			// ベースフルモード

	// 環境設定ソロ
	dlg_list.push_back(&cfg_wnd_solo_screen);					// ソロ：画面モード
	dlg_list.push_back(&cfg_wnd_solo_font_msgspd);				// ソロ：メッセージ（フォント、文字速度）
	dlg_list.push_back(&cfg_wnd_solo_font);						// ソロ：フォント
	dlg_list.push_back(&cfg_wnd_solo_msgspd);					// ソロ：文字速度
	dlg_list.push_back(&cfg_wnd_solo_volume_bgmfade);			// ソロ：音量（音量、ＢＧＭフェード）
	dlg_list.push_back(&cfg_wnd_solo_volume);					// ソロ：音量
	dlg_list.push_back(&cfg_wnd_solo_bgmfade);					// ソロ：ＢＧＭフェード
	dlg_list.push_back(&cfg_wnd_solo_koemode_chrkoe);			// ソロ：音声（音声モード、キャラクター音声）
	dlg_list.push_back(&cfg_wnd_solo_koemode);					// ソロ：音声モード
	dlg_list.push_back(&cfg_wnd_solo_chrkoe);					// ソロ：キャラクター音声
	dlg_list.push_back(&cfg_wnd_solo_mwndbk);					// ソロ：ウィンドウ背景色
	dlg_list.push_back(&cfg_wnd_solo_automode);					// ソロ：オートモード
	dlg_list.push_back(&cfg_wnd_solo_jitan);					// ソロ：時短再生
	dlg_list.push_back(&cfg_wnd_solo_else);						// ソロ：その他
	dlg_list.push_back(&cfg_wnd_solo_system);					// ソロ：システム
	dlg_list.push_back(&cfg_wnd_solo_movtype);					// ソロ：ムービーの再生方法

	// セーブダイアログ
	dlg_list.push_back(&sys_wnd_solo_save);						// セーブダイアログ
	// ロードダイアログ
	dlg_list.push_back(&sys_wnd_solo_load);						// ロードダイアログ
	// ツイートダイアログ
	dlg_list.push_back(&sys_wnd_solo_tweet);					// ツイートダイアログ

	// デバッグウィンドウ（デバッグ時のみ）
	if (Gp_global->debug_flag)	{
		dlg_list.push_back(&db_wnd_info_debug);						// デバッグ設定
		dlg_list.push_back(&db_wnd_info_scn_control);				// シーンコントロール
		dlg_list.push_back(&db_wnd_info_trace);						// トレース
		dlg_list.push_back(&db_wnd_info_error);						// エラー情報
		dlg_list.push_back(&db_wnd_info_d3d_device);				// Direct3D デバイス情報
		dlg_list.push_back(&db_wnd_info_d3d_resource);				// Direct3D リソース情報
		dlg_list.push_back(&db_wnd_info_input);						// 入力情報
		dlg_list.push_back(&db_wnd_info_system);					// システム情報
		dlg_list.push_back(&db_wnd_info_dir);						// ディレクトリ情報
		dlg_list.push_back(&db_wnd_info_script);					// スクリプト情報
		dlg_list.push_back(&db_wnd_info_stack);						// スタック情報
		dlg_list.push_back(&db_wnd_info_call_history);				// 呼び出し履歴
		dlg_list.push_back(&db_wnd_info_proc_stack);				// プロセススタック情報
		dlg_list.push_back(&db_wnd_info_namae);						// 【名前】情報
		dlg_list.push_back(&db_wnd_info_read_flag);					// 既読フラグ情報
		dlg_list.push_back(&db_wnd_info_cg_table);					// ＣＧテーブル
		dlg_list.push_back(&db_wnd_info_bgm_table);					// ＢＧＭテーブル
		dlg_list.push_back(&db_wnd_info_element);					// エレメント情報
		for (int i = 0; i < TNM_FLAG_CNT; i++)	{
			dlg_list.push_back(&db_wnd_info_flag[i]);					// フラグ情報
		}
		for (int i = 0; i < 20; i++)	{
			dlg_list.push_back(&db_wnd_info_flag_ini[i]);				// Flag.ini 情報
		}
		dlg_list.push_back(&db_wnd_info_user_flag);					// ユーザフラグ情報
		dlg_list.push_back(&db_wnd_info_user_call_flag);			// ユーザコールフラグ情報
		dlg_list.push_back(&db_wnd_info_namae_flag);				// 名前フラグ情報
		dlg_list.push_back(&db_wnd_info_counter);					// カウンタ情報
		dlg_list.push_back(&db_wnd_info_btn);						// ボタン情報
		dlg_list.push_back(&db_wnd_info_group);						// グループ情報
		dlg_list.push_back(&db_wnd_info_world);						// ワールド情報
		for (int i = 0; i < 6; i++)	{
			dlg_list.push_back(&db_wnd_info_object[i]);					// オブジェクト情報
		}
		dlg_list.push_back(&db_wnd_info_mwnd);						// メッセージウィンドウ情報
		dlg_list.push_back(&db_wnd_info_sound);						// サウンド情報
		dlg_list.push_back(&db_wnd_info_system_sound);				// ボリューム情報
		dlg_list.push_back(&db_wnd_info_koe);						// 声ウォッチ情報
		dlg_list.push_back(&db_wnd_info_save_history);				// セーブポイント履歴
	}
}

// 初期化
void C_tnm_dlg_manager::init()
{
	for (int i = 0; i < (int)dlg_list.size(); i++)	{
		dlg_list[i]->init();
	}

	// フラグ情報ダイアログはページ番号を指定
	TSTR flag_name[] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E"), _T("F"), _T("X"), _T("S"), _T("G"), _T("Z"), _T("M"), _T("L"), _T("K")};
	for (int i = 0; i < TNM_FLAG_CNT; i++)	{
		db_wnd_info_flag[i].regist(str_format(_T("db_wnd_info_flag_%s"), flag_name[i].c_str()), false, true, -1);
		db_wnd_info_flag[i].regist_caption_name(str_format(_T("フラグ%s"), flag_name[i].c_str()));
		db_wnd_info_flag[i].set_page_no(i);
	}
	// Flag.ini 情報ダイアログはページ番号を指定
	for (int i = 0; i < 20; i++)	{
		db_wnd_info_flag_ini[i].regist(str_format(_T("db_wnd_info_flag_ini_%02d"), i), false, true, -1);
		if (Gp_flag_ini->page[i].title.empty())	{
			db_wnd_info_flag_ini[i].regist_caption_name(str_format(_T("フラグ%02d"), i));
		}
		else	{
			db_wnd_info_flag_ini[i].regist_caption_name(Gp_flag_ini->page[i].title);
		}
		db_wnd_info_flag_ini[i].set_page_no(i);
	}
	// オブジェクト情報ダイアログはページ番号を指定
	TSTR regist_name[] = {_T("back_object"), _T("front_object"), _T("next_object"), _T("excall_back_object"), _T("excall_front_object"), _T("excall_next_object")};
	TSTR caption_name[] = {_T("back.object"), _T("front.object"), _T("next.object"), _T("excall.back.object"), _T("excall.front.object"), _T("excall.next.object")};
	for (int i = 0; i < 6; i++)	{
		db_wnd_info_object[i].regist(str_format(_T("db_wnd_info_object_%s"), regist_name[i].c_str()), false, true, -1);
		db_wnd_info_object[i].regist_caption_name(_T("オブジェクト情報（") + caption_name[i] + _T("）"));
		db_wnd_info_object[i].set_page_no(i);
	}
}

// 解放（free で WM_CLOSE が処理され、on_close 内でステータスが保存される）
void C_tnm_dlg_manager::free()
{
	for (int i = 0; i < (int)dlg_list.size(); i++)	{
		dlg_list[i]->free();
	}
}

// 初期ステータス読み込み
void C_tnm_dlg_manager::load_initial_state()
{
	for (int i = 0; i < (int)dlg_list.size(); i++)	{
		dlg_list[i]->load_state();
	}
}

// 初期ステータス保存
void C_tnm_dlg_manager::save_initial_state()
{
	for (int i = 0; i < (int)dlg_list.size(); i++)	{
		dlg_list[i]->save_state();
	}
}

// 初期オープン
void C_tnm_dlg_manager::initial_open()
{
	for (int i = 0; i < (int)dlg_list.size(); i++)	{
		dlg_list[i]->initial_open();
	}
}

// 禁止／許可を設定
void C_tnm_dlg_manager::set_enable(bool flag)
{
	for (int i = 0; i < (int)dlg_list.size(); i++)	{
		if (dlg_list[i]->get_handle())	{
			dlg_list[i]->set_enable(flag);
		}
	}
}

// マウスカーソルが当たっている／当たっていないを取得
bool C_tnm_dlg_manager::check_mouse_over()
{
	C_point mp = get_mouse_pos();
	for (int i = 0; i < (int)dlg_list.size(); i++)	{
		if (dlg_list[i]->get_handle() && dlg_list[i]->get_enable())	{
			C_rect wr = dlg_list[i]->get_window_rect();
			if (wr.left <= mp.x && mp.x < wr.right && wr.top <= mp.y && mp.y < wr.bottom)	{
				return true;	// 上に乗っている！
			}
		}
	}

	// エレメント情報ウィンドウを個別にチェック
	if (db_wnd_info_element.check_mouse_over(mp))
		return true;	// 上に乗っている！

	// 上に乗っていない
	return false;
}

// フレーム処理
void C_tnm_dlg_manager::frame()
{
	C_elm_stage* p_back = &(*Gp_stage_list)[TNM_STAGE_BACK];
	C_elm_stage* p_front = &(*Gp_stage_list)[TNM_STAGE_FRONT];
	C_elm_stage* p_next = &(*Gp_stage_list)[TNM_STAGE_NEXT];

	C_elm_group_list* group_list[] = {&p_back->m_group_list, &p_front->m_group_list, &p_next->m_group_list};
	C_elm_mwnd_list* mwnd_list[] = {&p_back->m_mwnd_list, &p_front->m_mwnd_list, &p_next->m_mwnd_list};

	// 環境設定ベース
	cfg_wnd_config_base.frame();			// ベース
	cfg_wnd_config_base_full_mode.frame();	// ベースフルモード

	// 環境設定ソロ
	cfg_wnd_solo_screen.frame();			// ソロ：画面モード
	cfg_wnd_solo_font_msgspd.frame();		// ソロ：メッセージ（フォント、文字速度）
	cfg_wnd_solo_font.frame();				// ソロ：フォント
	cfg_wnd_solo_msgspd.frame();			// ソロ：文字速度
	cfg_wnd_solo_volume_bgmfade.frame();	// ソロ：音量（音量、ＢＧＭフェード）
	cfg_wnd_solo_volume.frame();			// ソロ：音量
	cfg_wnd_solo_bgmfade.frame();			// ソロ：ＢＧＭフェード
	cfg_wnd_solo_koemode_chrkoe.frame();	// ソロ：音声（音声モード、キャラクター音声）
	cfg_wnd_solo_koemode.frame();			// ソロ：音声モード
	cfg_wnd_solo_chrkoe.frame();			// ソロ：キャラクター音声
	cfg_wnd_solo_mwndbk.frame();			// ソロ：ウィンドウ背景色
	cfg_wnd_solo_automode.frame();			// ソロ：オートモード
	cfg_wnd_solo_jitan.frame();				// ソロ：時短再生
	cfg_wnd_solo_else.frame();				// ソロ：その他
	cfg_wnd_solo_system.frame();			// ソロ：システム
	cfg_wnd_solo_movtype.frame();			// ソロ：ムービーの再生方法

	// セーブダイアログ
	sys_wnd_solo_save.frame();				// セーブダイアログ
	// ロードダイアログ
	sys_wnd_solo_load.frame();				// ロードダイアログ
	// ツイートダイアログ
	sys_wnd_solo_tweet.frame();				// ツイートダイアログ

	// デバッグウィンドウ（デバッグ時のみ）
	if (Gp_global->debug_flag)	{
		db_wnd_info_debug.frame();
		db_wnd_info_scn_control.frame();
		db_wnd_info_trace.frame();
		db_wnd_info_error.frame();
		db_wnd_info_d3d_device.frame();
		db_wnd_info_d3d_resource.frame();
		db_wnd_info_input.frame();
		db_wnd_info_system.frame();
		db_wnd_info_dir.frame();
		db_wnd_info_script.frame();
		db_wnd_info_stack.frame();
		db_wnd_info_call_history.frame();
		db_wnd_info_proc_stack.frame();
		db_wnd_info_namae.frame();
		db_wnd_info_cg_table.frame();
		db_wnd_info_bgm_table.frame();
		db_wnd_info_read_flag.frame();
		db_wnd_info_element.frame();
		for (int i = 0; i < TNM_FLAG_CNT; i++)	{
			db_wnd_info_flag[i].frame();
		}
		for (int i = 0; i < 20; i++)	{
			db_wnd_info_flag_ini[i].frame();
		}
		db_wnd_info_user_flag.frame();
		db_wnd_info_user_call_flag.frame();
		db_wnd_info_namae_flag.frame();
		db_wnd_info_counter.frame() ;
		db_wnd_info_world.frame();
		db_wnd_info_btn.frame();
		db_wnd_info_group.frame(group_list, sizeof_array(group_list));
		for (int i = 0; i < 6; i++)	{
			db_wnd_info_object[i].frame();
		}
		db_wnd_info_mwnd.frame(mwnd_list, sizeof_array(mwnd_list));
		db_wnd_info_sound.frame();
		db_wnd_info_system_sound.frame();
		db_wnd_info_koe.frame();
		db_wnd_info_save_history.frame();
	}

	// ウィンドウ座標を補正する
	for (int i = 0; i < (int)dlg_list.size(); i++)	{
		if (dlg_list[i]->get_handle())	{
			dlg_list[i]->rep_window_pos_outside_desktop();
		}
	}
}
