#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_input.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_local_save.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_namae.h"

#include	"element/elm_flag.h"
#include	"element/elm_stage.h"
#include	"element/elm_mwnd.h"
#include	"element/elm_btn_sel.h"

#include	"element/elm_msg_back.h"

#include	"engine/eng_input.h"
#include	"engine/eng_config.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_save.h"
#include	"engine/flow_command.h"
#include	"engine/eng_flag.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_message.h"
#include	"engine/ifc_msg_back.h"
#include	"engine/ifc_sound.h"
#include	"engine/eng_etc.h"

// ****************************************************************
// メッセージウィンドウ：現在のメッセージウィンドウを取得
// ================================================================
C_elm_mwnd* tnm_get_cur_mwnd()
{
	C_elm_mwnd* p_mwnd = (C_elm_mwnd *)tnm_get_element_ptr(Gp_local->cur_mwnd.begin(), Gp_local->cur_mwnd.end(), true);

	return p_mwnd;
}

C_elm_mwnd* tnm_get_cur_sel_mwnd()
{
	C_elm_mwnd* p_mwnd = (C_elm_mwnd *)tnm_get_element_ptr(Gp_local->cur_sel_mwnd.begin(), Gp_local->cur_sel_mwnd.end(), true);

	return p_mwnd;
}

// ****************************************************************
// メッセージ：メッセージブロックの先頭処理
// ================================================================
// メッセージブロックは、声、顔グラ、名前、メッセージ、の塊のことです。
// この順に記述された場合、先頭の「声」コマンドの直前に先頭処理を行います。
// 先頭処理の最後でセーブポイントを立てています。
// クリアなど、セーブ前に行って欲しい内容をここで記述します。
// 声の再生など、ロード後にも行って欲しい処理は、各処理内に記述します。
// ================================================================
void tnm_msg_proc_start_msg_block(C_elm_mwnd* p_mwnd)
{
	// 既にスタート処理を行っていれば何もしない
	if (p_mwnd->is_msg_block_started())
		return;

	// クリア準備フラグが立っているならクリアする
	if (p_mwnd->get_clear_ready_flag())
	{
		p_mwnd->clear();

		// フルメッセージをクリア
		Gp_local->cur_full_message.clear();
	}

	// メッセージが明らかに入りきらない場合はクリアする（オーバーフロー用）
	else if (p_mwnd->get_over_flow_check_size() > 0)
	{
		if (!p_mwnd->add_msg_check(false))
		{
			p_mwnd->clear();

			// フルメッセージをクリア
			Gp_local->cur_full_message.clear();
		}
	}

	// ノベルに対してのクリア処理
	p_mwnd->clear_for_novel_one_msg();

	// 最後に文字を表示した時間を設定
	p_mwnd->set_last_moji_disp_time();

	// メッセージバックを進める
	tnm_msg_back_next();

	// ================================================================
	// 決定キーをクリアする
	// ----------------------------------------------------------------
	// この処理は必須です。この処理がないと、古いクリックでメッセージがスキップされてしまいます。
	// 多くの処理は、～_WAIT プロセスの先頭でキークリアを行うことで回避していますが、
	// MSG_WAIT は特殊で、自動でキークリアを行わないません。
	// 自動でキークリアを行うと、メッセージnlメッセージnl…などの処理で、メッセージごとにキーがクリアされてしまい、
	// 「クリックでスキップする」機能が働かなくなるためです。
	// ----------------------------------------------------------------
	Gp_input->clear_ex_key(VK_EX_DECIDE);

	// メッセージブロックの先頭処理完了
	p_mwnd->set_msg_block_start_flag();

	// セーブポイントをクリアする
	tnm_init_local_save();

	// セーブポイントを立てる
	if (!Gp_local->pod.dont_set_save_point)
	{
		tnm_set_save_point();
	}
		
	// ★BGM がなってないチェック
	if (!tnm_check_bgm_playing())
	{ 
		Gp_global->__bgm_stop_cntr++;
		if (Gp_global->__bgm_stop_cntr >= 10)
		{
			//write_debug_log(_T("★10行以上 BGM が鳴っていません！"));
			//tnm_set_error(TNM_ERROR_TYPE_WARNING, _T("★10行以上 BGM が鳴っていません！"));
			Gp_global->__bgm_stop_cntr = 0;
		}
	}
}

// ****************************************************************
// メッセージ：メッセージＰＰブロックの先頭処理
// ================================================================
// メッセージＰＰブロックは、PP コマンドで区切られるメッセージ単位です。
// オートモードや、メッセージ速度の計算単位です。
// ================================================================
void tnm_msg_proc_start_msg_pp_block(C_elm_mwnd* p_mwnd)
{
	// オートモードカウントを終了
	Gp_global->is_auto_mode_count_start = false;

	// 声の再生が終わってからの待ち時間カウント終了
	Gp_global->is_koe_only_count_start = false;

	// ここまでオートモードを進めた
	p_mwnd->set_auto_mode_end_moji_cnt();	// ▲▲▲マルチメッセージ

	// メッセージ速度計算をリセット
	p_mwnd->set_progress_start_flag(false);
}

// ****************************************************************
// メッセージ：声を設定
// ================================================================
void tnm_msg_proc_koe(C_elm_mwnd* p_mwnd, int koe_no, int chara_no, int rf_scn_no, int rf_flag_no, bool wait_flag, bool key_skip_flag, bool no_auto_mode_flag, bool return_value_flag)
{
	if (!p_mwnd)
		return;

	// 既読早送り処理（メッセージブロックに持っていきたいけど、全てに既読フラグ持たせるのもなぁ…）
	{
		// 「現在の既読フラグ番号」を設定（既読早送りが可能かの判定に使います）
		Gp_local->pod.cur_read_flag_scn_no = rf_scn_no;
		Gp_local->pod.cur_read_flag_flag_no = rf_flag_no;

		// 早送りの更新
		tnm_update_read_skip();

		// 自動早送りを開始する
		tnm_update_start_skip();
	}

	// 声を再生するかどうかを判定する
	bool play_flag = tnm_is_koe_play(chara_no);

	// 時短再生のレートを求める
	int jitan_rate = tnm_is_auto_mode() ?
		(Gp_config->jitan_auto_mode_onoff ? Gp_config->jitan_speed : TNM_JITAN_RATE_NORMAL): 
		(Gp_config->jitan_normal_onoff ? Gp_config->jitan_speed : TNM_JITAN_RATE_NORMAL);

	// 声を再生する（再生しない場合はファイルチェックのみ）
	tnm_play_koe(koe_no, chara_no, jitan_rate, 0, wait_flag, key_skip_flag, return_value_flag, false, play_flag ? false : true);

	// メッセージに声を設定
	p_mwnd->set_koe(koe_no, play_flag, no_auto_mode_flag);

	// ★ＢＧＭフェード２が必要フラグを立てる
	Gp_global->bgmfade2_need_flag = true;

	// メッセージに既読フラグを追加（クリア時にこの既読フラグが立ちます）
	p_mwnd->add_read_flag(rf_scn_no, rf_flag_no);

	// メッセージバックに声を設定する
	if (!Gp_local->pod.msg_back_off)
	{
		tnm_msg_back_add_koe(koe_no, chara_no, Gp_lexer->get_cur_scn_no(), Gp_lexer->get_cur_line_no());
	}

	// 「現在の声番号」を設定（声再生ボタンで再生する声です）
	Gp_local->pod.cur_koe_no = koe_no;
	Gp_local->pod.cur_chr_no = chara_no;
}

// ****************************************************************
// メッセージ：ＥＸ声を設定
// ================================================================
void tnm_msg_proc_ex_koe(C_elm_mwnd* p_mwnd, int koe_no, int chara_no, bool wait_flag, bool key_skip_flag, bool return_value_flag)
{
	// 声を再生する（再生しない場合はファイルチェックのみ）
	tnm_play_koe(koe_no, chara_no, TNM_JITAN_RATE_NORMAL, 0, wait_flag, key_skip_flag, return_value_flag, true, false);
}

// ****************************************************************
// メッセージ：顔グラを設定
// ================================================================
void tnm_msg_proc_face(C_elm_mwnd* p_mwnd, int face_no, CTSTR& file_path)
{
	if (!p_mwnd)
		return;

	// 顔グラを設定
	p_mwnd->set_face(face_no, file_path);
}

// ****************************************************************
// メッセージ：名前フラグを展開
// ================================================================
//		＊Ａや％Ａなどを変換します。
// ================================================================
void tnm_msg_proc_expand_namae_flag(TSTR& msg)
{
	TSTR new_msg;

	for (TSTR::const_iterator itr = msg.begin(); itr != msg.end(); )	{

		if (*itr == _T('＊') || *itr == _T('％'))	{

			// １文字目がアルファベット？
			if (itr + 1 != msg.end() && is_zenkaku_large_alpha(*(itr + 1)))	{
				TSTR namae;

				// ２文字目がアルファベット？
				if (itr + 2 != msg.end() && is_zenkaku_large_alpha(*(itr + 2)))	{
					namae = *Gp_flag->get_namae_flag_by_name(TSTR(itr, itr + 3));		// 「＊ＡＡ」
					itr += 3;
				}
				else	{
					namae = *Gp_flag->get_namae_flag_by_name(TSTR(itr, itr + 2));		// 「＊Ａ」
					itr += 2;
				}

				// 名前の何文字目を表示する？
				if (itr != msg.end() && is_zenkaku_num(*itr))	{

					int moji_no = 0;
					while (itr != msg.end())	{
						if (is_zenkaku_num(*itr))	{
							moji_no = moji_no * 10 + *itr - _T('０');
							++ itr;
						}
						else	{
							break;
						}
					}

					if (moji_no < (int)namae.length())
						namae = namae.substr(moji_no, 1);
					else
						namae.clear();
				}

				// 名前を結合
				new_msg += namae;
			}
			else	{
				new_msg += *itr;	// 「＊」のみ
				++itr;
			}
		}
		else	{
			new_msg += *itr;	// 通常の文字
			++itr;
		}
	}

	msg = new_msg;
}

// ****************************************************************
// メッセージ：名前を設定
// ================================================================
void tnm_msg_proc_name(C_elm_mwnd* p_mwnd, TSTR name)
{
	if (!p_mwnd)
		return;

	// 既に名前がある場合は名前コマンドを無視する
	if (!p_mwnd->get_name().empty())
		return;

	// キャラクター音声の名前の伏字を公開するかどうか判定（公開フラグをＯＮにします）
	tnm_is_chrkoe_name_check(name);

	// 名前を変換（ini に登録された名前）
	int color_mode, moji_color_no, shadow_color_no, fuchi_color_no;
	name = Gp_namae->change(name, &color_mode, &moji_color_no, &shadow_color_no, &fuchi_color_no);

	// キャラクタ文字色を設定
	p_mwnd->set_chara_moji_color(moji_color_no);
	p_mwnd->set_chara_shadow_color(shadow_color_no);
	p_mwnd->set_chara_fuchi_color(fuchi_color_no);

	// 名前フラグを展開
	tnm_msg_proc_expand_namae_flag(name);

	// 名前の表示モードを取得
	int name_disp_mode = p_mwnd->get_name_disp_mode();

	// ノベルの場合、名前表示モードが名前ウィンドウの場合は「メッセージの先頭に表示」に変更
	if (p_mwnd->get_novel_flag() == 1 && name_disp_mode == TNM_MWND_NAME_DISP_TYPE_NAME)
	{
		name_disp_mode = TNM_MWND_NAME_DISP_TYPE_MSG;
	}

	// 括弧つき名前を求める
	TSTR name_with_bracket;
	int name_bracket_type = p_mwnd->get_name_bracket();
	if (name_bracket_type == TNM_MWND_NAME_BRACKET_TYPE_SUMI)
	{
		name_with_bracket = _T("【") + name + _T("】");
	}
	else
	{
		name_with_bracket = name;
	}
	
	// セーブメッセージに名前を追加（名前が「表示しない」の場合は追加しない）
	if (name_disp_mode != TNM_MWND_NAME_DISP_TYPE_NONE)
	{
		tnm_save_local_msg(name_with_bracket);
	}

	// 名前ウィンドウに名前を表示
	if (name_disp_mode == TNM_MWND_NAME_DISP_TYPE_NAME)
	{
		p_mwnd->set_name(name_with_bracket);
	}
	// メッセージの先頭に名前を表示
	else if (name_disp_mode == TNM_MWND_NAME_DISP_TYPE_MSG)
	{
		p_mwnd->add_name_to_msg(name_with_bracket, NULL);
		p_mwnd->set_line_head();		// 行の先頭を設定する
	}

	// メッセージバックに名前を設定する
	if (!Gp_local->pod.msg_back_off)
	{
		// メッセージウィンドウに準拠する場合
		if (Gp_ini->msg_back.name_disp_mode == TNM_MSGBK_NAME_DISP_TYPE_DEFAULT)
		{
			if (name_disp_mode != TNM_MWND_NAME_DISP_TYPE_NONE)
			{
				if (name_bracket_type == TNM_MWND_NAME_BRACKET_TYPE_NONE)
				{
					tnm_msg_back_add_name(name, name, Gp_lexer->get_cur_scn_no(), Gp_lexer->get_cur_line_no());
				}
				else if (name_bracket_type == TNM_MWND_NAME_BRACKET_TYPE_SUMI)
				{
					tnm_msg_back_add_name(name, _T("【") + name + _T("】"), Gp_lexer->get_cur_scn_no(), Gp_lexer->get_cur_line_no());
				}
			}
			else
			{
				tnm_msg_back_add_name(name, _T(""), Gp_lexer->get_cur_scn_no(), Gp_lexer->get_cur_line_no());
			}
		}
		// オンの場合
		else if (Gp_ini->msg_back.name_disp_mode == TNM_MSGBK_NAME_DISP_TYPE_ON)
		{
			if (Gp_ini->msg_back.name_bracket_type == TNM_MWND_NAME_BRACKET_TYPE_NONE)
			{
				tnm_msg_back_add_name(name, name, Gp_lexer->get_cur_scn_no(), Gp_lexer->get_cur_line_no());
			}
			else if (Gp_ini->msg_back.name_bracket_type == TNM_MWND_NAME_BRACKET_TYPE_SUMI)
			{
				tnm_msg_back_add_name(name, _T("【") + name + _T("】"), Gp_lexer->get_cur_scn_no(), Gp_lexer->get_cur_line_no());
			}
		}
	}
}

// ****************************************************************
// メッセージ：メッセージを追加
// ================================================================
//		メッセージコマンドが現れた場合の処理です。
//		１．クリア準備フラグが立っていればメッセージをクリアします。
//		２．新たにメッセージを追加します。
//		３．最初のメッセージだった場合はセーブポイントを立てます。
// ================================================================
void tnm_msg_proc_print(C_elm_mwnd* p_mwnd, TSTR msg, int rf_scn_no, int rf_flag_no, bool msg_back_flag)
{
	if (!p_mwnd)
		return;

	TSTR over_flow_msg;

	// 既読早送り処理（メッセージブロックに持っていきたいけど、全てに既読フラグ持たせるのもなぁ…）
	{
		// 「現在の既読フラグ番号」を設定（既読早送りが可能かの判定に使います）
		Gp_local->pod.cur_read_flag_scn_no = rf_scn_no;
		Gp_local->pod.cur_read_flag_flag_no = rf_flag_no;

		// 早送りの更新
		tnm_update_read_skip();

		// 自動早送りを開始する
		tnm_update_start_skip();
	}

	// 名前フラグを展開
	tnm_msg_proc_expand_namae_flag(msg);

	// セーブデータにメッセージを追加
	tnm_save_local_msg(msg);

	// フルメッセージを設定
	Gp_local->cur_full_message += msg;

	// メッセージを追加
	p_mwnd->add_msg(msg, &over_flow_msg);

	// 最後に文字を表示した時間を設定
	p_mwnd->set_last_moji_disp_time();

	// オーバーフローした場合はキー待ちを自動的に挿入
	if (!over_flow_msg.empty())
	{
		// オーバフロー処理
		tnm_msg_proc_over_flow_msg(p_mwnd, msg, over_flow_msg, rf_scn_no, rf_flag_no);
	}

	// メッセージに既読フラグを追加（クリア時にこの既読フラグが立ちます）
	p_mwnd->add_read_flag(rf_scn_no, rf_flag_no);

	// メッセージバックにメッセージを設定する
	if (msg_back_flag && !Gp_local->pod.msg_back_off)
	{
		tnm_msg_proc_msg_back_add_msg(msg, p_mwnd->get_debug_msg(), Gp_lexer->get_cur_scn_no(), Gp_lexer->get_cur_line_no());
	}

	// メッセージウィンドウの自動処理開始
	p_mwnd->set_auto_proc_ready();

	// メッセージを進める
	p_mwnd->msg_frame();

	// 自動的にメッセージウェイト
	// オーバーフロー時は上でメッセージウェイトを入れてるので入れない
	// 非同期モードの場合はメッセージウェイトを入れない
	if (!Gp_local->pod.async_msg_mode && !Gp_local->pod.multi_msg_mode && over_flow_msg.empty())
	{
		tnm_msg_proc_command_msg_wait(p_mwnd);
	}
}

// ****************************************************************
// メッセージ：メッセージバックを追加
// ================================================================
//		メッセージバックからのロードに関わります
//		メッセージ処理、または msgbk.add_msg などから 呼ばれます
// ================================================================

void tnm_msg_proc_msg_back_add_msg(CTSTR& msg, CTSTR& debug_msg, int scn_no, int line_no)
{
	tnm_msg_back_add_msg(msg, debug_msg, scn_no, line_no);

	// メッセージバックセーブを使う場合
	if (Gp_ini->backlog_save_cnt > 0)
	{
		if (!tnm_msg_back_get_save_id_check_flag())
		{
			int interval = std::max(Gp_ini->backlog_save_interval, 1);

			// 数個置きに保存するための仕掛け
			if (Gp_local->pod.msg_back_save_cntr++ % interval == 0)
			{
				// ローカルセーブがある場合
				if (!Gp_local_save->save_stream.empty())
				{
					// メッセージバックセーブ用のアイテムを作る
					BSP<S_tnm_local_save> save_item = BSP<S_tnm_local_save>(new S_tnm_local_save);
					*save_item = *Gp_local_save;

					// メッセージバックセーブにアイテムを追加する
					Gp_backlog_save->map.insert(std::pair<S_tid, BSP<S_tnm_local_save>>(Gp_local_save->save_id, save_item));
					// メッセージバックとアイテムを関連付ける
					tnm_msg_back_set_save_id(Gp_local_save->save_id);
					// バックログセーブを整理する
					tnm_remove_backlog_save();
				}
			}

			tnm_msg_back_set_save_id_check_flag(true);
		}
	}
}

// ****************************************************************
// メッセージ：オーバーフロー
// ================================================================
//		オーバーフローしたときに自動で追加されるコマンドです。
// ================================================================
void tnm_msg_proc_over_flow_msg(C_elm_mwnd* p_mwnd, CTSTR& msg, CTSTR& over_flow_msg, int rf_scn_no, int rf_flag_no)
{
	// オーバーフローをスライドメッセージで送るタイプ
	if (p_mwnd->is_slide_msg())
	{
		C_tnm_proc print_proc;
		print_proc.type = TNM_PROC_TYPE_COMMAND;
		print_proc.element = p_mwnd->get_element() + S_element(ELM_MWND____OVER_FLOW_PRINT);
		print_proc.arg_list_id = 1;
		print_proc.arg_list.resize(3);
		print_proc.arg_list[0].form = FM_STR;
		print_proc.arg_list[0].str = over_flow_msg;
		print_proc.arg_list[1].form = FM_INT;
		print_proc.arg_list[1].Int = rf_scn_no;
		print_proc.arg_list[2].form = FM_INT;
		print_proc.arg_list[2].Int = rf_flag_no;
		tnm_push_proc(print_proc);				// ４．メッセージ

		C_tnm_proc msg_block_proc;
		msg_block_proc.type = TNM_PROC_TYPE_COMMAND;
		msg_block_proc.element = p_mwnd->get_element() + S_element(ELM_MWND_MSG_PP_BLOCK);
		tnm_push_proc(msg_block_proc);			// ３．メッセージブロックの先頭処理

		C_tnm_proc slide_msg_proc;
		slide_msg_proc.type = TNM_PROC_TYPE_COMMAND;
		slide_msg_proc.element = p_mwnd->get_element() + S_element(ELM_MWND____SLIDE_MSG);
		tnm_push_proc(slide_msg_proc);			// ２．スライドメッセージ

		// １文字でも表示していたらメッセージを待つ
		if (msg != over_flow_msg)
		{
			C_tnm_proc msg_wait_proc;
			msg_wait_proc.type = TNM_PROC_TYPE_MESSAGE_WAIT;
			msg_wait_proc.element = p_mwnd->get_element();
			tnm_push_proc(msg_wait_proc);			// １．メッセージ表示終了待ち
		}
	}

	// オーバーフローをキー待ちで待つタイプ
	else
	{
		// スライドメッセージでなければエラーを出す
		tnm_set_error(TNM_ERROR_TYPE_OVERFLOW, _T("メッセージがオーバーフローしました。"));

		C_tnm_proc print_proc;
		print_proc.type = TNM_PROC_TYPE_COMMAND;
		print_proc.element = p_mwnd->get_element() + S_element(ELM_MWND____OVER_FLOW_PRINT);
		print_proc.arg_list_id = 1;
		print_proc.arg_list.resize(3);
		print_proc.arg_list[0].form = FM_STR;
		print_proc.arg_list[0].str = over_flow_msg;
		print_proc.arg_list[1].form = FM_INT;
		print_proc.arg_list[1].Int = rf_scn_no;
		print_proc.arg_list[2].form = FM_INT;
		print_proc.arg_list[2].Int = rf_flag_no;
		tnm_push_proc(print_proc);				// ５．メッセージ

		// １文字でも表示していたらキー待ちを入れる
		if (msg != over_flow_msg)	{

			C_tnm_proc msg_block_proc;
			msg_block_proc.type = TNM_PROC_TYPE_COMMAND;
			msg_block_proc.element = p_mwnd->get_element() + S_element(ELM_MWND_MSG_BLOCK);
			tnm_push_proc(msg_block_proc);			// ４．メッセージブロックの先頭処理

			C_tnm_proc clear_ready_proc;
			clear_ready_proc.type = TNM_PROC_TYPE_COMMAND;
			clear_ready_proc.element = p_mwnd->get_element() + S_element(ELM_MWND_CLEAR);
			tnm_push_proc(clear_ready_proc);		// ３．クリア準備フラグを立てる

			// ノベルモード
			if (p_mwnd->get_novel_flag() == 1)
			{
				p_mwnd->set_key_icon_mode(1);	// ページ待ちアイコン
			}
			// 通常モード
			else
			{
				p_mwnd->set_key_icon_mode(0);	// キー待ちアイコン
			}

			C_tnm_proc msg_key_wait_proc;
			msg_key_wait_proc.type = TNM_PROC_TYPE_MESSAGE_KEY_WAIT;
			msg_key_wait_proc.element = p_mwnd->get_element();
			tnm_push_proc(msg_key_wait_proc);		// ２．メッセージキー待ち

			C_tnm_proc msg_wait_proc;
			msg_wait_proc.type = TNM_PROC_TYPE_MESSAGE_WAIT;
			msg_wait_proc.element = p_mwnd->get_element();
			tnm_push_proc(msg_wait_proc);			// １．メッセージ表示終了待ち
		}

		// それ以外の場合はクリアのみ（ただし声は止めない）
		else
		{
			C_tnm_proc msg_block_proc;
			msg_block_proc.type = TNM_PROC_TYPE_COMMAND;
			msg_block_proc.element = p_mwnd->get_element() + S_element(ELM_MWND_MSG_BLOCK);
			tnm_push_proc(msg_block_proc);			// ４．メッセージブロックの先頭処理

			C_tnm_proc clear_ready_proc;
			clear_ready_proc.type = TNM_PROC_TYPE_COMMAND;
			clear_ready_proc.element = p_mwnd->get_element() + S_element(ELM_MWND_CLEAR);
			tnm_push_proc(clear_ready_proc);		// ３．クリア準備フラグを立てる
		}
	}
}

// ****************************************************************
// メッセージ：改行
// ================================================================
void tnm_msg_proc_command_nl(C_elm_mwnd* p_mwnd)
{
	p_mwnd->new_line_no_indent();

	if (Gp_ini->mwnd.sub[p_mwnd->get_mwnd_no()].msg_back_insert_nl)	{
		Gp_msg_back->add_new_line_no_indent(Gp_lexer->get_cur_scn_no(), Gp_lexer->get_cur_line_no());
	}
}

void tnm_msg_proc_command_nli(C_elm_mwnd* p_mwnd)
{
	p_mwnd->new_line_indent();

	if (Gp_ini->mwnd.sub[p_mwnd->get_mwnd_no()].msg_back_insert_nl)	{
		Gp_msg_back->add_new_line_indent(Gp_lexer->get_cur_scn_no(), Gp_lexer->get_cur_line_no());
	}
}

// ****************************************************************
// メッセージ：ルビ
// ================================================================
//		ルビコマンドの処理です。
// ================================================================
void tnm_msg_proc_ruby(C_elm_mwnd* p_mwnd, TSTR ruby)
{
	if (!p_mwnd)
		return;

	// 名前フラグを展開
	tnm_msg_proc_expand_namae_flag(ruby);

	// ルビを表示する
	p_mwnd->start_ruby(ruby);

	// メッセージウィンドウの自動処理開始
	p_mwnd->set_auto_proc_ready();
}

void tnm_msg_proc_end_ruby(C_elm_mwnd* p_mwnd)
{
	if (!p_mwnd)
		return;

	// ルビを終了する
	p_mwnd->end_ruby();

	// メッセージウィンドウの自動処理開始
	p_mwnd->set_auto_proc_ready();
}

// ****************************************************************
// メッセージ：名前フラグを展開
// ================================================================
int tnm_zenkaku_num_moji_to_int(TCHAR moji)
{
	switch (moji)	{
		case _T('０'):	return 0;
		case _T('１'):	return 1;
		case _T('２'):	return 2;
		case _T('３'):	return 3;
		case _T('４'):	return 4;
		case _T('５'):	return 5;
		case _T('６'):	return 6;
		case _T('７'):	return 7;
		case _T('８'):	return 8;
		case _T('９'):	return 9;
		default:		return 0;
	}
}

bool tnm_msg_proc_expand_name_flag(TSTR::const_iterator& itr_ret, TSTR::const_iterator end, int* moji_type_ret, int* moji_code_ret)
{
	int moji_type = TNM_MOJI_TYPE_MOJI;
	int moji_code = -1;

	// 現在のイテレータを保存しておく
	TSTR::const_iterator itr = itr_ret;

	// 絵文字判定
	if (itr != end && *itr == _T('＃'))	{
		++ itr;
		if (itr != end && *itr == _T('Ａ') || itr != end && *itr == _T('Ｂ'))	{
			moji_type = *itr == _T('Ａ') ? TNM_MOJI_TYPE_EMOJI_A : TNM_MOJI_TYPE_EMOJI_B;
			++ itr;
			if (itr != end && is_zenkaku_num(*itr))	{
				moji_code = tnm_zenkaku_num_moji_to_int(*itr);
				++ itr;
				for (int i = 0;;)	{
					if (itr != end && is_zenkaku_num(*itr))	{
						moji_code = moji_code * 10 + tnm_zenkaku_num_moji_to_int(*itr);
						++ itr;
					}
					else	{
						break;
					}
				}
			}
		}
		if (moji_code >= 0)	{
			*moji_type_ret = moji_type;
			*moji_code_ret = moji_code;
			itr_ret = itr;
			return true;
		}
	}

	// 絵文字じゃなかったのでイテレータを戻す
	itr = itr_ret;

	// 通常の文字判定
	if (itr != end)	{
		*moji_type_ret = TNM_MOJI_TYPE_MOJI;
		*moji_code_ret = *itr++;
		itr_ret = itr;
		return true;
	}

	return false;
}

// ****************************************************************
// メッセージ：選択肢
// ================================================================
//		選択肢コマンドの処理です。
// ================================================================
void tnm_msg_proc_sel(C_elm_mwnd* p_mwnd, ARRAY<S_tnm_sel_param>& sel_list, int rf_scn_no, int rf_flag_no, bool cancel_enable, bool close_mwnd)
{
	if (!p_mwnd)
		return;

	// クリア準備フラグが立っているならクリアする
	if (p_mwnd->get_clear_ready_flag())
		tnm_msg_proc_clear(p_mwnd);

	// 選択肢をクリア
	p_mwnd->clear_sel();

	// 選択肢を準備
	p_mwnd->ready_sel();

	// 選択肢を追加していく
	for (ARRAY<S_tnm_sel_param>::iterator itr = sel_list.begin(); itr != sel_list.end(); ++itr)	{
		TSTR msg = itr->str;

		// 名前フラグを展開
		tnm_msg_proc_expand_namae_flag(msg);

		// 選択肢を追加
		p_mwnd->add_sel(msg, itr->type, itr->color);
	}

	// 選択肢を整理する
	p_mwnd->restruct_sel();

	// キャンセル可能フラグを設定
	p_mwnd->set_sel_cancel_enable(cancel_enable);

	// 「現在の既読フラグ番号」を設定
	Gp_local->pod.cur_read_flag_scn_no = rf_scn_no;
	Gp_local->pod.cur_read_flag_flag_no = rf_flag_no;

	// メッセージに既読フラグを追加
	p_mwnd->add_read_flag(rf_scn_no, rf_flag_no);

	// 早送りの更新
	tnm_update_read_skip();

	// 自動早送りを開始する
	tnm_update_start_skip();

	// 自動処理開始
	p_mwnd->set_auto_proc_ready();

	// プロセスの設定（３．メッセージウィンドウを閉じる）
	if (close_mwnd)	{
		C_tnm_proc proc_close;
		proc_close.type = TNM_PROC_TYPE_COMMAND;
		proc_close.element = p_mwnd->get_element() + S_element(ELM_MWND_CLOSE);
		tnm_push_proc(proc_close);
	}

	// プロセスの設定（２．クリア準備フラグを立てる）

	// ノベルモード
	if (p_mwnd->get_novel_flag() == 1)	{
		C_tnm_proc clear_ready_proc;
		clear_ready_proc.type = TNM_PROC_TYPE_COMMAND;
		clear_ready_proc.element = p_mwnd->get_element() + S_element(ELM_MWND____NOVEL_CLEAR);
		tnm_push_proc(clear_ready_proc);
	}
	// 通常モード
	else	{
		C_tnm_proc clear_ready_proc;
		clear_ready_proc.type = TNM_PROC_TYPE_COMMAND;
		clear_ready_proc.element = p_mwnd->get_element() + S_element(ELM_MWND_CLEAR);
		tnm_push_proc(clear_ready_proc);
	}

	// プロセスの設定（１．選択肢）
	C_tnm_proc proc_sel;
	proc_sel.type = TNM_PROC_TYPE_SEL;
	proc_sel.element = p_mwnd->get_element();
	tnm_push_proc(proc_sel);
}

// ****************************************************************
// メッセージ：ボタン選択肢
// ================================================================
//		ボタン選択肢コマンドの処理です。
// ================================================================
void tnm_msg_proc_selbtn_ready(C_elm_btn_select* p_btn_sel, int template_no, ARRAY<S_tnm_sel_param>& sel_list, bool cancel_enable, bool capture_flag, CTSTR& sel_start_scn, int sel_start_z_no)
{
	if (!p_btn_sel)
		return;

	// ボタン選択肢を初期化
	p_btn_sel->reinit();
	p_btn_sel->set_template(template_no);

	// 選択肢を追加していく
	for (ARRAY<S_tnm_sel_param>::iterator itr = sel_list.begin(); itr != sel_list.end(); ++itr)	{
		TSTR msg = itr->str;

		// 名前フラグを展開
		tnm_msg_proc_expand_namae_flag(msg);

		// 選択肢を追加
		p_btn_sel->add_sel(msg, itr->type, itr->color);
	}

	// 選択肢を整理する
	p_btn_sel->restruct_item();

	// キャンセル可能フラグを設定する
	p_btn_sel->set_cancel_enable(cancel_enable);

	// キャプチャーフラグを設定する
	p_btn_sel->set_capture_flag(capture_flag);

	// 選択開始時に呼ばれるコマンドを設定する
	p_btn_sel->set_sel_start_call(sel_start_scn, sel_start_z_no);
}

void tnm_msg_proc_selbtn_start(C_elm_btn_select* p_btn_sel, int rf_scn_no, int rf_flag_no, int sync_type)
{
	// 現在の既読フラグ番号を設定
	Gp_local->pod.cur_read_flag_scn_no = rf_scn_no;
	Gp_local->pod.cur_read_flag_flag_no = rf_flag_no;

	// 既読フラグを登録する
	p_btn_sel->regist_read_flag(rf_scn_no, rf_flag_no);

	// 既読早送りを更新する
	tnm_update_read_skip();

	// 自動早送りを開始する
	tnm_update_start_skip();

	// 選択肢を出現させる
	p_btn_sel->start_sel(sync_type);

	// プロセスの設定（１．選択肢）
	C_tnm_proc proc_sel;
	proc_sel.type = TNM_PROC_TYPE_SEL_BTN;
	tnm_push_proc(proc_sel);
}


// ****************************************************************
// メッセージ：メッセージをクリア準備
// ================================================================
//		メッセージをクリア準備する処理です。
//		１．既読フラグを立てます。
//		２．声を停止します。
// ================================================================
void tnm_msg_proc_clear_ready(C_elm_mwnd* p_mwnd)
{
	// クリア準備フラグを立てる
	p_mwnd->set_clear_ready_flag();

	// 割り当てられた既読フラグを立てる
	p_mwnd->set_read_flag();

	// 既読フラグをクリアする
	p_mwnd->clear_read_flag();

	// マルチメッセージを終了する
	Gp_local->pod.multi_msg_mode = false;

	// 非同期メッセージを終了するかも
	if (Gp_local->pod.async_msg_mode_once)	{
		Gp_local->pod.async_msg_mode = false;
		Gp_local->pod.async_msg_mode_once = false;
	}

	// スキップトリガーによる早送りを停止する
	Gp_global->skip_because_skip_trigger = false;

	// 声を停止する（システムの設定による）
	if (tnm_is_stop_koe())	{
		tnm_stop_koe();

		// ★ＢＧＭフェード２が必要フラグを降ろす
		Gp_global->bgmfade2_need_flag = false;
	}

	// 現在の声（システムコマンドの「声を再生」で再生する声）をクリアする
	Gp_local->pod.cur_koe_no = -1;
	Gp_local->pod.cur_chr_no = -1;

	// オートモードの文字数をクリア
	Gp_local->pod.auto_mode_moji_cnt = 0;

	// メッセージブロックの先頭処理完了フラグをクリア
	p_mwnd->clear_msg_block_start_flag();
}

// ****************************************************************
// メッセージウィンドウ：メッセージをクリア準備（ノベル）
// ================================================================
//		メッセージをクリア準備する処理です。
//		１．既読フラグを立てます。
//		２．声を停止します。
// ================================================================
void tnm_msg_proc_clear_ready_novel(C_elm_mwnd* p_mwnd)
{
	// 既読フラグを立てる
	p_mwnd->set_read_flag();

	// 既読フラグをクリアする
	p_mwnd->clear_read_flag();

	// マルチメッセージを終了する
	Gp_local->pod.multi_msg_mode = false;

	// 声を停止する（システムの設定による）
	if (tnm_is_stop_koe())	{
		tnm_stop_koe();
	}

	// 現在の声をクリアする
	Gp_local->pod.cur_koe_no = -1;
	Gp_local->pod.cur_chr_no = -1;

	// メッセージブロックの先頭処理完了フラグをクリア
	p_mwnd->clear_msg_block_start_flag();

	// 改行をはさんでみる
	p_mwnd->new_line_no_indent();
}

// ****************************************************************
// メッセージ：メッセージをクリア
// ================================================================
//		メッセージをクリアする処理です。
// ================================================================
void tnm_msg_proc_clear(C_elm_mwnd* p_mwnd)
{
	if (!p_mwnd)
		return;

	// フルメッセージをクリア
	Gp_local->cur_full_message.clear();

	// メッセージをクリアする
	p_mwnd->clear();
}

// ****************************************************************
// メッセージ：メッセージウィンドウを閉じる
// ================================================================
void tnm_msg_proc_close(C_elm_mwnd* p_mwnd, bool wait_flag)
{
	if (!p_mwnd)
		return;

	// ウィンドウが開いていなければ何もしない
	if (!p_mwnd->get_window_appear_flag())
		return;

	// 既読フラグを立てる
	p_mwnd->set_read_flag();

	// メッセージウィンドウを閉じる
	p_mwnd->close();

	// 閉じアニメ待ち状態にする
	if (wait_flag)	{
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_MWND_CLOSE_WAIT;
		proc.element = p_mwnd->get_element();
		tnm_push_proc(proc);
	}
}

// ****************************************************************
// メッセージ：全てのメッセージウィンドウを閉じる
// ================================================================
void tnm_msg_proc_close_all(bool wait_flag)
{
	// すべてのウィンドウに対して判定
	for (int stage_no = 0; stage_no < (int)Gp_stage_list->get_size(); stage_no ++)	{
		for (int mwnd_no = 0; mwnd_no < (int)(*Gp_stage_list)[stage_no].m_mwnd_list.get_size(); mwnd_no++)	{
			C_elm_mwnd* p_mwnd = &(*Gp_stage_list)[stage_no].m_mwnd_list[mwnd_no];

			// 既読フラグを立てる
			p_mwnd->set_read_flag();

			// メッセージウィンドウを閉じる
			p_mwnd->close();
		}
	}
	// 閉じアニメ待ち状態にする
	if (wait_flag)	{
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_MWND_CLOSE_WAIT_ALL;
		tnm_push_proc(proc);
	}
}

// ****************************************************************
// メッセージ：Ｒ命令
// ================================================================
void tnm_msg_proc_command_msg_wait(C_elm_mwnd* p_mwnd)
{
	C_tnm_proc msg_wait_proc;
	msg_wait_proc.type = TNM_PROC_TYPE_MESSAGE_WAIT;
	msg_wait_proc.element = p_mwnd->get_element();
	tnm_push_proc(msg_wait_proc);			// １．メッセージ表示終了待ち
}

void tnm_msg_proc_command_pp(C_elm_mwnd* p_mwnd)
{
	p_mwnd->set_key_icon_mode(0);	// キー待ちアイコン

	C_tnm_proc msg_key_wait_proc;
	msg_key_wait_proc.type = TNM_PROC_TYPE_MESSAGE_KEY_WAIT;
	msg_key_wait_proc.element = p_mwnd->get_element();
	tnm_push_proc(msg_key_wait_proc);		// ２．メッセージキー待ち

	C_tnm_proc msg_wait_proc;
	msg_wait_proc.type = TNM_PROC_TYPE_MESSAGE_WAIT;
	msg_wait_proc.element = p_mwnd->get_element();
	tnm_push_proc(msg_wait_proc);			// １．メッセージ表示終了待ち
}

void tnm_msg_proc_command_r(C_elm_mwnd* p_mwnd)
{
	if (Gp_local->pod.ignore_r_flag)
		return;

	// ノベルモード
	if (p_mwnd->get_novel_flag() == 1)	{

		// メッセージが明らかに入らない場合は PAGE コマンドの動作になる
		if (p_mwnd->get_over_flow_check_size() > 0 && !p_mwnd->add_msg_check(true))	{
			p_mwnd->set_key_icon_mode(1);	// ページ待ちアイコン

			C_tnm_proc clear_ready_proc;
			clear_ready_proc.type = TNM_PROC_TYPE_COMMAND;
			clear_ready_proc.element = p_mwnd->get_element() + S_element(ELM_MWND_CLEAR);
			tnm_push_proc(clear_ready_proc);	// ３．クリア準備フラグを立てる
		}
		else	{
			p_mwnd->set_key_icon_mode(0);	// キー待ちアイコン

			C_tnm_proc clear_ready_proc;
			clear_ready_proc.type = TNM_PROC_TYPE_COMMAND;
			clear_ready_proc.element = p_mwnd->get_element() + S_element(ELM_MWND____NOVEL_CLEAR);
			tnm_push_proc(clear_ready_proc);	// ３．クリア準備フラグを立てる
		}
	}
	// 通常モード
	else	{
		C_tnm_proc clear_ready_proc;
		clear_ready_proc.type = TNM_PROC_TYPE_COMMAND;
		clear_ready_proc.element = p_mwnd->get_element() + S_element(ELM_MWND_CLEAR);
		tnm_push_proc(clear_ready_proc);	// ３．クリア準備フラグを立てる
	}

	C_tnm_proc msg_key_wait_proc;
	msg_key_wait_proc.type = TNM_PROC_TYPE_MESSAGE_KEY_WAIT;
	msg_key_wait_proc.element = p_mwnd->get_element();
	tnm_push_proc(msg_key_wait_proc);		// ２．メッセージキー待ち

	C_tnm_proc msg_wait_proc;
	msg_wait_proc.type = TNM_PROC_TYPE_MESSAGE_WAIT;
	msg_wait_proc.element = p_mwnd->get_element();
	tnm_push_proc(msg_wait_proc);			// １．メッセージ表示終了待ち
}

void tnm_msg_proc_command_page(C_elm_mwnd* p_mwnd)
{
	// ノベルモード
	if (p_mwnd->get_novel_flag() == 1)	{
		p_mwnd->set_key_icon_mode(1);	// ページ待ちアイコン
	}
	// 通常モード
	else	{
		p_mwnd->set_key_icon_mode(0);	// キー待ちアイコン
	}

	C_tnm_proc clear_ready_proc;
	clear_ready_proc.type = TNM_PROC_TYPE_COMMAND;
	clear_ready_proc.element = p_mwnd->get_element() + S_element(ELM_MWND_CLEAR);
	tnm_push_proc(clear_ready_proc);		// ３．クリア準備フラグを立てる

	C_tnm_proc msg_key_wait_proc;
	msg_key_wait_proc.type = TNM_PROC_TYPE_MESSAGE_KEY_WAIT;
	msg_key_wait_proc.element = p_mwnd->get_element();
	tnm_push_proc(msg_key_wait_proc);		// ２．メッセージキー待ち

	C_tnm_proc msg_wait_proc;
	msg_wait_proc.type = TNM_PROC_TYPE_MESSAGE_WAIT;
	msg_wait_proc.element = p_mwnd->get_element();
	tnm_push_proc(msg_wait_proc);			// １．メッセージ表示終了待ち
}

