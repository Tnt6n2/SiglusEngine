#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_syscom.h"
#include	"data/tnm_config_data.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_config.h"
#include	"dialog/db_wnd_info_script.h"

// ****************************************************************
// デバッグ情報ウィンドウ：スクリプト
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_script")

// 初期化
void C_db_wnd_info_script::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// キャプション名登録
	regist_caption_name(_T("ゲーム情報"));

	// リストビュー項目
	listview_add_column(_T("state"), _T("ステート"), 100);
	listview_add_column(_T("value"), _T("値"), 100);
}

// 初期化ファンクション
void C_db_wnd_info_script::on_init_func()
{
	// 項目を追加
	lv.add_item(0, _T("シーン番号"));
	lv.add_item(0, _T("行番号"));
	lv.add_item(0, _T("現在のプロセス"));

	lv.add_item(0, _T("一時的にシステムコマンドメニュー禁止"));
	lv.add_item(0, _T("一時的にショートカット禁止"));
	lv.add_item(0, _T("一時的にメッセージウィンドウを隠す禁止"));

	lv.add_item(0, _T("一時的にスキップ禁止"));
	lv.add_item(0, _T("一時的に Ctrl スキップ禁止"));
	lv.add_item(0, _T("一時的にクリックしてもスキップを止めなくする"));
	lv.add_item(0, _T("一時的にクリックしてもメッセージを一括表示しなくする"));
	lv.add_item(0, _T("一時的に未読文章も早送りできるようにする"));
	lv.add_item(0, _T("一時的にオートモードにする"));
	lv.add_item(0, _T("一時的にメッセージスピードを設定"));
	lv.add_item(0, _T("一時的にメッセージをノーウェイトにする"));
	lv.add_item(0, _T("一時的にメッセージを非同期にする"));
	lv.add_item(0, _T("一時的にマルチメッセージを使う"));
	lv.add_item(0, _T("一時的にスキップトリガー"));
	lv.add_item(0, _T("一時的に文章が進んでも声を止めなくする"));
	lv.add_item(0, _T("一時的に文章が進んだら声を止めるようにする"));

	lv.add_item(0, _T("一時的にメッセージウィンドウボタン禁止"));
	lv.add_item(0, _T("一時的にメッセージウィンドウボタンに触れなくする"));
	lv.add_item(0, _T("一時的にメッセージウィンドウのアニメを行うようにする"));
	lv.add_item(0, _T("一時的にメッセージウィンドウのアニメを行わなくする"));
	lv.add_item(0, _T("一時的にメッセージウィンドウを消す"));

	lv.add_item(0, _T("一時的にメッセージバック禁止"));
	lv.add_item(0, _T("一時的にメッセージバックを更新しない"));
	lv.add_item(0, _T("一時的にメッセージバックを非表示にする"));

	lv.add_item(0, _T("一時的にマウスカーソルを非表示にする"));
	lv.add_item(0, _T("一時的にキーボードでカーソルを動かさなくする"));

	lv.add_item(0, _T("一時的にクエイクを止める"));
	lv.add_item(0, _T("一時的にＣＧテーブルに登録しなくする"));
	lv.add_item(0, _T("一時的にＢＧＭフェードをかっけぱなしにする"));
	lv.add_item(0, _T("一時的にセーブポイントを設置しなくする"));
	lv.add_item(0, _T("一時的にディスプレイの垂直同期を待たなくする"));

	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞ) 既読早送り"));
	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞ) 自動早送り"));
	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞ) オートモード"));
	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞ) メッセージウィンドウを隠す"));
	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞ) メッセージバック"));

	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭ-) 既読早送り禁止"));
	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭ-) 自動早送り禁止"));
	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭ-) オートモード禁止"));
	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭ-) メッセージウィンドウを消す禁止"));
	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭ-) メッセージバック禁止"));
	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭ-) セーブ禁止"));
	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭ-) ロード禁止"));
	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭ-) 前の選択肢に戻る禁止"));

	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭ-) 既読早送り非表示"));
	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭ-) 自動早送り非表示"));
	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭ-) オートモード非表示"));
	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭ-) メッセージウィンドウを消す非表示"));
	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭ-) メッセージバック非表示"));
	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭ-) セーブ非表示"));
	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭ-) ロード非表示"));
	lv.add_item(0, _T("(ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭ-) 前の選択肢に戻る非表示"));
}

// 閉じるファンクション
void C_db_wnd_info_script::on_close_func()
{
}

// ****************************************************************
// フレーム
// ================================================================
void C_db_wnd_info_script::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

	// 現在の値を取得する
	cd.cur_scn_no = Gp_lexer->get_cur_scn_no();
	cd.cur_line_no = Gp_lexer->get_cur_line_no();
	cd.proc_type = Gp_local->cur_proc.type;

	cd.syscom_menu_disable = Gp_local->pod.syscom_menu_disable;
	cd.hide_mwnd_disable = Gp_local->pod.hide_mwnd_disable;
	cd.shortcut_disable = Gp_local->pod.shortcut_disable;

	cd.skip_disable = Gp_local->pod.skip_disable;
	cd.ctrl_disable = Gp_local->pod.ctrl_disable;
	cd.not_stop_skip_by_click = Gp_local->pod.not_stop_skip_by_click;
	cd.not_skip_msg_by_click = Gp_local->pod.not_skip_msg_by_click;
	cd.skip_unread_message = Gp_local->pod.skip_unread_message;
	cd.auto_mode_flag = Gp_local->pod.auto_mode_flag;
	cd.msg_speed = Gp_local->pod.msg_speed;
	cd.msg_nowait = Gp_local->pod.msg_nowait;
	cd.async_msg_mode = Gp_local->pod.async_msg_mode;
	cd.multi_msg_mode = Gp_local->pod.multi_msg_mode;
	cd.skip_trigger = Gp_local->pod.skip_trigger;
	cd.koe_dont_stop_on_flag = Gp_local->pod.koe_dont_stop_on_flag;
	cd.koe_dont_stop_off_flag = Gp_local->pod.koe_dont_stop_off_flag;

	cd.mwnd_btn_disable_all = Gp_local->pod.mwnd_btn_disable_all;
	cd.mwnd_btn_touch_disable = Gp_local->pod.mwnd_btn_touch_disable;
	cd.mwnd_anime_on_flag = Gp_local->pod.mwnd_anime_on_flag;
	cd.mwnd_anime_off_flag = Gp_local->pod.mwnd_anime_off_flag;
	cd.mwnd_disp_off_flag = Gp_local->pod.mwnd_disp_off_flag;

	cd.msg_back_disable = Gp_local->pod.msg_back_disable;
	cd.msg_back_off = Gp_local->pod.msg_back_off;
	cd.msg_back_disp_off = Gp_local->pod.msg_back_disp_off;

	cd.cursor_disp_off = Gp_local->pod.cursor_disp_off;
	cd.cursor_move_by_key_disable = Gp_local->pod.cursor_move_by_key_disable;

	cd.quake_stop_flag = Gp_local->pod.quake_stop_flag;
	cd.cg_table_off_flag = Gp_local->pod.cg_table_off_flag;
	cd.bgmfade_flag = Gp_local->pod.bgmfade_flag;
	cd.dont_set_save_point = Gp_local->pod.dont_set_save_point;
	cd.wait_display_vsync_off_flag = Gp_local->pod.wait_display_vsync_off_flag;

	cd.syscom_read_skip = Gp_syscom->read_skip_flag;
	cd.syscom_auto_skip = Gp_syscom->auto_skip_flag;
	cd.syscom_auto_mode = Gp_config->auto_mode_onoff;
	cd.syscom_hide_mwnd = Gp_syscom->hide_mwnd_flag;
	cd.syscom_msg_back = Gp_syscom->msg_back_flag;

	cd.syscom_read_skip_disable = !Gp_syscom->syscommenu.read_skip.enable;
	cd.syscom_auto_skip_disable = !Gp_syscom->syscommenu.auto_skip.enable;
	cd.syscom_auto_mode_disable = !Gp_syscom->syscommenu.auto_mode.enable;
	cd.syscom_save_disable = !Gp_syscom->syscommenu.save.enable;
	cd.syscom_load_disable = !Gp_syscom->syscommenu.load.enable;
	cd.syscom_return_to_sel_disable = !Gp_syscom->syscommenu.return_to_sel.enable;
	cd.syscom_hide_mwnd_disable = !Gp_syscom->syscommenu.hide_mwnd.enable;
	cd.syscom_msg_back_disable = !Gp_syscom->syscommenu.msg_back.enable;

	cd.syscom_read_skip_off = !Gp_syscom->syscommenu.read_skip.exist;
	cd.syscom_auto_skip_off = !Gp_syscom->syscommenu.auto_skip.exist;
	cd.syscom_auto_mode_off = !Gp_syscom->syscommenu.auto_mode.exist;
	cd.syscom_save_off = !Gp_syscom->syscommenu.save.exist;
	cd.syscom_load_off = !Gp_syscom->syscommenu.load.exist;
	cd.syscom_return_to_sel_off = !Gp_syscom->syscommenu.return_to_sel.exist;
	cd.syscom_hide_mwnd_off = !Gp_syscom->syscommenu.hide_mwnd.exist;
	cd.syscom_msg_back_off = !Gp_syscom->syscommenu.msg_back.exist;

	// 項目を順に設定する
	int item_no = -1;

	// シーン番号
	item_no ++;
	if (frame_first_flag || cd.cur_scn_no != m_ld.cur_scn_no)
		lv.set_cell_text(item_no, 1, tostr(cd.cur_scn_no));

	// 行番号
	item_no ++;
	if (frame_first_flag || cd.cur_line_no != m_ld.cur_line_no)
		lv.set_cell_text(item_no, 1, tostr(cd.cur_line_no));

	// 現在のプロセス
	item_no ++;
	if (frame_first_flag || cd.proc_type != m_ld.proc_type)	{
		if (0 <= cd.proc_type && cd.proc_type < TNM_PROC_TYPE_MAX)	{
			lv.set_cell_text(item_no, 1, TNM_PROC_STR[cd.proc_type]);
		}
		else	{
			lv.set_cell_text(item_no, 1, _T("???"));
		}
	}

	// システムコマンドメニューを禁止
	item_no ++;
	if (frame_first_flag || cd.syscom_menu_disable != m_ld.syscom_menu_disable)
		lv.set_cell_text(item_no, 1, cd.syscom_menu_disable ? _T("オン") : _T("--"));

	// ショートカットを禁止
	item_no ++;
	if (frame_first_flag || cd.shortcut_disable != m_ld.shortcut_disable)
		lv.set_cell_text(item_no, 1, cd.shortcut_disable ? _T("オン") : _T("--"));

	// メッセージウィンドウを隠すを禁止
	item_no ++;
	if (frame_first_flag || cd.hide_mwnd_disable != m_ld.hide_mwnd_disable)
		lv.set_cell_text(item_no, 1, cd.hide_mwnd_disable ? _T("オン") : _T("--"));


	// スキップを禁止
	item_no ++;
	if (frame_first_flag || cd.skip_disable != m_ld.skip_disable)
		lv.set_cell_text(item_no, 1, cd.skip_disable ? _T("オン") : _T("--"));

	// スキップ禁止（Ctrl 含む）
	item_no ++;
	if (frame_first_flag || cd.ctrl_disable != m_ld.ctrl_disable)
		lv.set_cell_text(item_no, 1, cd.ctrl_disable ? _T("オン") : _T("--"));

	// クリックしてもスキップを止めない
	item_no ++;
	if (frame_first_flag || cd.not_stop_skip_by_click != m_ld.not_stop_skip_by_click)
		lv.set_cell_text(item_no, 1, cd.not_stop_skip_by_click ? _T("オン") : _T("--"));

	// クリックしてもメッセージを一括表示しない
	item_no ++;
	if (frame_first_flag || cd.not_skip_msg_by_click != m_ld.not_skip_msg_by_click)
		lv.set_cell_text(item_no, 1, cd.not_skip_msg_by_click ? _T("オン") : _T("--"));

	// 一時的に未読文章も早送りできるようにする
	item_no ++;
	if (frame_first_flag || cd.skip_unread_message != m_ld.skip_unread_message)
		lv.set_cell_text(item_no, 1, cd.skip_unread_message ? _T("オン") : _T("--"));

	// オートモードフラグ
	item_no ++;
	if (frame_first_flag || cd.auto_mode_flag != m_ld.auto_mode_flag)
		lv.set_cell_text(item_no, 1, cd.auto_mode_flag ? _T("オン") : _T("--"));

	// メッセージスピード
	item_no ++;
	if (frame_first_flag || cd.msg_speed != m_ld.msg_speed)
		lv.set_cell_text(item_no, 1, cd.msg_speed >= 0 ? tostr(cd.msg_speed) : _T("--"));

	// ノーウェイト
	item_no ++;
	if (frame_first_flag || cd.msg_nowait != m_ld.msg_nowait)
		lv.set_cell_text(item_no, 1, cd.msg_nowait ? _T("オン") : _T("--"));

	// 非同期メッセージ
	item_no ++;
	if (frame_first_flag || cd.async_msg_mode != m_ld.async_msg_mode)
		lv.set_cell_text(item_no, 1, cd.async_msg_mode ? _T("オン") : _T("--"));

	// マルチメッセージ
	item_no ++;
	if (frame_first_flag || cd.multi_msg_mode != m_ld.multi_msg_mode)
		lv.set_cell_text(item_no, 1, cd.multi_msg_mode ? _T("オン") : _T("--"));

	// スキップトリガー
	item_no ++;
	if (frame_first_flag || cd.skip_trigger != m_ld.skip_trigger)
		lv.set_cell_text(item_no, 1, cd.skip_trigger ? _T("オン") : _T("--"));

	// 声を止めない
	item_no ++;
	if (frame_first_flag || cd.koe_dont_stop_on_flag != m_ld.koe_dont_stop_on_flag)
		lv.set_cell_text(item_no, 1, cd.koe_dont_stop_on_flag ? _T("オン") : _T("--"));

	// 声を止める
	item_no ++;
	if (frame_first_flag || cd.koe_dont_stop_off_flag != m_ld.koe_dont_stop_off_flag)
		lv.set_cell_text(item_no, 1, cd.koe_dont_stop_off_flag ? _T("オン") : _T("--"));



	// メッセージウィンドウボタンを禁止
	item_no ++;
	if (frame_first_flag || cd.mwnd_btn_disable_all != m_ld.mwnd_btn_disable_all)
		lv.set_cell_text(item_no, 1, cd.mwnd_btn_disable_all ? _T("オン") : _T("--"));

	// メッセージウィンドウボタンに触れない
	item_no ++;
	if (frame_first_flag || cd.mwnd_btn_touch_disable != m_ld.mwnd_btn_touch_disable)
		lv.set_cell_text(item_no, 1, cd.mwnd_btn_touch_disable ? _T("オン") : _T("--"));

	// メッセージウィンドウのアニメを行う
	item_no ++;
	if (frame_first_flag || cd.mwnd_anime_on_flag != m_ld.mwnd_anime_on_flag)
		lv.set_cell_text(item_no, 1, cd.mwnd_anime_on_flag ? _T("オン") : _T("--"));

	// メッセージウィンドウのアニメを行わない
	item_no ++;
	if (frame_first_flag || cd.mwnd_anime_off_flag != m_ld.mwnd_anime_off_flag)
		lv.set_cell_text(item_no, 1, cd.mwnd_anime_off_flag ? _T("オン") : _T("--"));

	// メッセージウィンドウを消す
	item_no ++;
	if (frame_first_flag || cd.mwnd_disp_off_flag != m_ld.mwnd_disp_off_flag)
		lv.set_cell_text(item_no, 1, cd.mwnd_disp_off_flag ? _T("オン") : _T("--"));

	
	
	// メッセージバックを禁止
	item_no ++;
	if (frame_first_flag || cd.msg_back_disable != m_ld.msg_back_disable)
		lv.set_cell_text(item_no, 1, cd.msg_back_disable ? _T("オン") : _T("--"));

	// メッセージバックを使わない
	item_no ++;
	if (frame_first_flag || cd.msg_back_off != m_ld.msg_back_off)
		lv.set_cell_text(item_no, 1, cd.msg_back_off ? _T("オン") : _T("--"));

	// メッセージバックを非表示にする
	item_no ++;
	if (frame_first_flag || cd.msg_back_disp_off != m_ld.msg_back_disp_off)
		lv.set_cell_text(item_no, 1, cd.msg_back_disp_off ? _T("オン") : _T("--"));


	// カーソルを非表示
	item_no ++;
	if (frame_first_flag || cd.cursor_disp_off != m_ld.cursor_disp_off)
		lv.set_cell_text(item_no, 1, cd.cursor_disp_off ? _T("オン") : _T("--"));

	// キーボードでカーソルを動かさない
	item_no ++;
	if (frame_first_flag || cd.cursor_move_by_key_disable != m_ld.cursor_move_by_key_disable)
		lv.set_cell_text(item_no, 1, cd.cursor_move_by_key_disable ? _T("オン") : _T("--"));



	// クエイクを止める
	item_no ++;
	if (frame_first_flag || cd.quake_stop_flag != m_ld.quake_stop_flag)
		lv.set_cell_text(item_no, 1, cd.quake_stop_flag ? _T("オン") : _T("--"));

	// ＣＧテーブルに登録しない
	item_no ++;
	if (frame_first_flag || cd.cg_table_off_flag != m_ld.cg_table_off_flag)
		lv.set_cell_text(item_no, 1, cd.cg_table_off_flag ? _T("オン") : _T("--"));

	// ＢＧＭフェードをかけっぱなし
	item_no ++;
	if (frame_first_flag || cd.bgmfade_flag != m_ld.bgmfade_flag)
		lv.set_cell_text(item_no, 1, cd.bgmfade_flag ? _T("オン") : _T("--"));

	// セーブポイントを設置しない
	item_no ++;
	if (frame_first_flag || cd.dont_set_save_point != m_ld.dont_set_save_point)
		lv.set_cell_text(item_no, 1, cd.dont_set_save_point ? _T("オン") : _T("--"));

	// ディスプレイの垂直同期を待たない
	item_no ++;
	if (frame_first_flag || cd.wait_display_vsync_off_flag != m_ld.wait_display_vsync_off_flag)
		lv.set_cell_text(item_no, 1, cd.wait_display_vsync_off_flag ? _T("オン") : _T("--"));



	// ｼｽﾃﾑｺﾏﾝﾄﾞ：既読早送り
	item_no ++;
	if (frame_first_flag || cd.syscom_read_skip != m_ld.syscom_read_skip)
		lv.set_cell_text(item_no, 1, cd.syscom_read_skip ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞ：自動早送り
	item_no ++;
	if (frame_first_flag || cd.syscom_auto_skip != m_ld.syscom_auto_skip)
		lv.set_cell_text(item_no, 1, cd.syscom_auto_skip ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞ：オートモード
	item_no ++;
	if (frame_first_flag || cd.syscom_auto_mode != m_ld.syscom_auto_mode)
		lv.set_cell_text(item_no, 1, cd.syscom_auto_mode ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞ：メッセージウィンドウを隠す
	item_no ++;
	if (frame_first_flag || cd.syscom_hide_mwnd != m_ld.syscom_hide_mwnd)
		lv.set_cell_text(item_no, 1, cd.syscom_hide_mwnd ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞ：メッセージバック
	item_no ++;
	if (frame_first_flag || cd.syscom_msg_back != m_ld.syscom_msg_back)
		lv.set_cell_text(item_no, 1, cd.syscom_msg_back ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ：既読早送り：禁止
	item_no ++;
	if (frame_first_flag || cd.syscom_read_skip_disable != m_ld.syscom_read_skip_disable)
		lv.set_cell_text(item_no, 1, cd.syscom_read_skip_disable ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ：自動早送り：禁止
	item_no ++;
	if (frame_first_flag || cd.syscom_auto_skip_disable != m_ld.syscom_auto_skip_disable)
		lv.set_cell_text(item_no, 1, cd.syscom_auto_skip_disable ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ：オートモード：禁止
	item_no ++;
	if (frame_first_flag || cd.syscom_auto_mode_disable != m_ld.syscom_auto_mode_disable)
		lv.set_cell_text(item_no, 1, cd.syscom_auto_mode_disable ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ：メッセージウィンドウを隠す：禁止
	item_no ++;
	if (frame_first_flag || cd.syscom_hide_mwnd_disable != m_ld.syscom_hide_mwnd_disable)
		lv.set_cell_text(item_no, 1, cd.syscom_hide_mwnd_disable ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ：メッセージバック：禁止
	item_no ++;
	if (frame_first_flag || cd.syscom_msg_back_disable != m_ld.syscom_msg_back_disable)
		lv.set_cell_text(item_no, 1, cd.syscom_msg_back_disable ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ：セーブ：禁止
	item_no ++;
	if (frame_first_flag || cd.syscom_save_disable != m_ld.syscom_save_disable)
		lv.set_cell_text(item_no, 1, cd.syscom_save_disable ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ：ロード：禁止
	item_no ++;
	if (frame_first_flag || cd.syscom_load_disable != m_ld.syscom_load_disable)
		lv.set_cell_text(item_no, 1, cd.syscom_load_disable ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ：前の選択肢に戻る：禁止
	item_no ++;
	if (frame_first_flag || cd.syscom_return_to_sel_disable != m_ld.syscom_return_to_sel_disable)
		lv.set_cell_text(item_no, 1, cd.syscom_return_to_sel_disable ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ：既読早送り：非表示
	item_no ++;
	if (frame_first_flag || cd.syscom_read_skip_off != m_ld.syscom_read_skip_off)
		lv.set_cell_text(item_no, 1, cd.syscom_read_skip_off ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ：自動早送り：非表示
	item_no ++;
	if (frame_first_flag || cd.syscom_auto_skip_off != m_ld.syscom_auto_skip_off)
		lv.set_cell_text(item_no, 1, cd.syscom_auto_skip_off ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ：オートモード：非表示
	item_no ++;
	if (frame_first_flag || cd.syscom_auto_mode_off != m_ld.syscom_auto_mode_off)
		lv.set_cell_text(item_no, 1, cd.syscom_auto_mode_off ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ：メッセージウィンドウを隠す：非表示
	item_no ++;
	if (frame_first_flag || cd.syscom_hide_mwnd_off != m_ld.syscom_hide_mwnd_off)
		lv.set_cell_text(item_no, 1, cd.syscom_hide_mwnd_off ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ：メッセージバック：非表示
	item_no ++;
	if (frame_first_flag || cd.syscom_msg_back_off != m_ld.syscom_msg_back_off)
		lv.set_cell_text(item_no, 1, cd.syscom_msg_back_off ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ：セーブ：非表示
	item_no ++;
	if (frame_first_flag || cd.syscom_save_off != m_ld.syscom_save_off)
		lv.set_cell_text(item_no, 1, cd.syscom_save_off ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ：ロード：非表示
	item_no ++;
	if (frame_first_flag || cd.syscom_load_off != m_ld.syscom_load_off)
		lv.set_cell_text(item_no, 1, cd.syscom_load_off ? _T("オン") : _T("--"));

	// ｼｽﾃﾑｺﾏﾝﾄﾞﾒﾆｭｰ：前の選択肢に戻る：非表示
	item_no ++;
	if (frame_first_flag || cd.syscom_return_to_sel_off != m_ld.syscom_return_to_sel_off)
		lv.set_cell_text(item_no, 1, cd.syscom_return_to_sel_off ? _T("オン") : _T("--"));

	// データを記憶
	m_ld = cd;

	// 初回フラグをおろす
	frame_first_flag = false;
}
