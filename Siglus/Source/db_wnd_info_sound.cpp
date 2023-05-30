#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"element/elm_sound.h"
#include	"engine/ifc_sound.h"
#include	"dialog/db_wnd_info_sound.h"


// ****************************************************************
// デバッグ情報ウィンドウ：サウンド
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_sound")

// 初期化
void C_db_wnd_info_sound::init()
{
	regist(REGIST_WINDOW_NAME, false, true, -1);

	// キャプション名登録
	regist_caption_name(_T("サウンド情報"));

	// リストビュー項目
	listview_add_column(_T("type"), _T("ﾀｲﾌﾟ"), 100);
	listview_add_column(_T("id"), _T("ID"), 50);
	listview_add_column(_T("file"), _T("ﾌｧｲﾙ"), 100);
	listview_add_column(_T("volume"), _T("ﾎﾞﾘｭｰﾑ"), 50);
	listview_add_column(_T("ready"), _T("準備"), 50);
	listview_add_column(_T("delay"), _T("遅延"), 50);
	listview_add_column(_T("state"), _T("状態"), 50);
	listview_add_column(_T("state2"), _T("状態2"), 50);
	listview_add_column(_T("option"), _T("ｵﾌﾟｼｮﾝ"), 50);
	listview_add_column(_T("bgmfade"), _T("bgmfade"), 50);
	listview_add_column(_T("bgmfade2"), _T("bgmfade2"), 50);
}

// 初期化ファンクション
void C_db_wnd_info_sound::on_init_func()
{
}

// 閉じるファンクション
void C_db_wnd_info_sound::on_close_func()
{
}

// ****************************************************************
// フレーム
// ================================================================
void C_db_wnd_info_sound::frame()
{
	if (!m_h_wnd)
		return;

	STATE cd;

	// 現在のデータを作る
	cd.item_list.resize(
		Gp_sound->m_bgm.get_player_cnt()
		+ Gp_sound->m_koe.get_player_cnt()
		+ Gp_sound->m_pcm.get_player_cnt()
		+ (int)Gp_sound->m_pcmch_list.get_size()
		+ Gp_sound->m_se.get_player_cnt()
		+ 1);

	int item_no = 0;

	// BGM
	for (int i = 0; i < (int)Gp_sound->m_bgm.get_player_cnt(); i++, item_no++)	{
		cd.item_list[item_no].type = 0;
		cd.item_list[item_no].player_no = i;
		cd.item_list[item_no].file_name = tnm_get_regist_name_by_bgm_no(Gp_sound->m_bgm.get_player(i)->get_bgm_no());
		cd.item_list[item_no].volume = Gp_sound->m_bgm.get_volume();
		cd.item_list[item_no].pause_flag = Gp_sound->m_bgm.is_pause();
		cd.item_list[item_no].delay_time = Gp_sound->m_bgm.get_delay_time();
		cd.item_list[item_no].state = Gp_sound->m_bgm.get_player(i)->get_play_state();
		cd.item_list[item_no].state2 = 0;
		cd.item_list[item_no].option = 0;
		cd.item_list[item_no].bgmfade_target = false;
		cd.item_list[item_no].bgmfade2_target = false;

		if (cd.item_list[item_no].state == TNM_PLAYER_STATE_PLAY)	{
			if (false);
			else if (Gp_sound->m_bgm.is_loop())			cd.item_list[item_no].option = 1;
		}
	}

	// KOE
	for (int i = 0; i < (int)Gp_sound->m_koe.get_player_cnt(); i++, item_no++)	{
		cd.item_list[item_no].type = 1;
		cd.item_list[item_no].player_no = i;
		int koe_no = Gp_sound->m_koe.get_player(i)->get_koe_no();
		cd.item_list[item_no].file_name = (koe_no == -1) ? _T("") : str_format(_T("%04d %05d"), koe_no / 10000, koe_no % 10000);
		cd.item_list[item_no].volume = Gp_sound->m_koe.get_volume();
		cd.item_list[item_no].pause_flag = false;
		cd.item_list[item_no].delay_time = 0;
		cd.item_list[item_no].state = Gp_sound->m_koe.get_player(i)->get_play_state_2();
		cd.item_list[item_no].state2 = 0;
		cd.item_list[item_no].option = 0;
		cd.item_list[item_no].bgmfade_target = false;
		cd.item_list[item_no].bgmfade2_target = false;
	}

	// PCM
	for (int i = 0; i < (int)Gp_sound->m_pcm.get_player_cnt(); i++, item_no++)	{
		cd.item_list[item_no].type = 2;
		cd.item_list[item_no].player_no = i;
		cd.item_list[item_no].file_name = Gp_sound->m_pcm.get_player(i)->get_file_name();
		cd.item_list[item_no].volume = Gp_sound->m_pcm.get_volume();
		cd.item_list[item_no].pause_flag = false;
		cd.item_list[item_no].delay_time = 0;
		cd.item_list[item_no].state = Gp_sound->m_pcm.get_player(i)->get_play_state_2();
		cd.item_list[item_no].state2 = 0;
		cd.item_list[item_no].option = 0;
		cd.item_list[item_no].bgmfade_target = false;
		cd.item_list[item_no].bgmfade2_target = false;
	}

	// PCMCH
	int pcmch_cnt = Gp_sound->m_pcmch_list.get_size();
	for (int i = 0; i < pcmch_cnt; i++, item_no++)	{
		C_elm_pcmch* p_pcmch = &Gp_sound->m_pcmch_list[i];

		cd.item_list[item_no].type = 3;
		cd.item_list[item_no].player_no = i;
		if (!p_pcmch->get_pcm_name().empty())	{
			cd.item_list[item_no].file_name = p_pcmch->get_pcm_name();
		}
		else if (!p_pcmch->get_bgm_name().empty())	{
			cd.item_list[item_no].file_name = _T("bgm; ") + p_pcmch->get_bgm_name();
		}
		else if (p_pcmch->get_koe_no() >= 0)	{
			if (p_pcmch->get_chara_no() >= 0)	{
				cd.item_list[item_no].file_name = str_format(_T("KOE(%09d,%03d)"), p_pcmch->get_koe_no(), p_pcmch->get_chara_no());
			}
			else	{
				cd.item_list[item_no].file_name = str_format(_T("KOE(%09d)"), p_pcmch->get_koe_no());
			}
		}
		else if (p_pcmch->get_se_no() >= 0)	{
			cd.item_list[item_no].file_name = _T("se; ") + tostr(p_pcmch->get_se_no()) + _T("番");
		}
		cd.item_list[item_no].volume = p_pcmch->get_volume();
		cd.item_list[item_no].pause_flag = false;
		cd.item_list[item_no].delay_time = 0;
		cd.item_list[item_no].state = p_pcmch->get_play_state_2();
		cd.item_list[item_no].state2 = 0;
		cd.item_list[item_no].option = 0;
		cd.item_list[item_no].bgmfade_target = p_pcmch->is_bgm_fade_target();
		cd.item_list[item_no].bgmfade2_target = p_pcmch->is_bgm_fade2_target();

		if (cd.item_list[item_no].state == TNM_PLAYER_STATE_PLAY || cd.item_list[item_no].state == TNM_PLAYER_STATE_PAUSE)	{
			if (false);
			else if (p_pcmch->is_loop())	cd.item_list[item_no].option = 1;
		}
	}

	// SE
	for (int i = 0; i < (int)Gp_sound->m_se.get_player_cnt(); i++, item_no++)	{
		int sound_type = Gp_sound->m_se.get_player(i)->get_sound_type();
		int koe_no = Gp_sound->m_se.get_player(i)->get_koe_no();
		int se_no = Gp_sound->m_se.get_player(i)->get_se_no();
		cd.item_list[item_no].type = 4;
		cd.item_list[item_no].player_no = i;
		switch (sound_type)	{
			case TNM_SOUND_TYPE_BGM:	cd.item_list[item_no].file_name = tnm_get_regist_name_by_bgm_no(Gp_sound->m_se.get_player(i)->get_bgm_no());			break;
			case TNM_SOUND_TYPE_KOE:	cd.item_list[item_no].file_name = (koe_no == -1) ? _T("") : str_format(_T("%04d %05d"), koe_no / 10000, koe_no % 10000);	break;
			case TNM_SOUND_TYPE_PCM:	cd.item_list[item_no].file_name = Gp_sound->m_se.get_player(i)->get_file_name();										break;
			case TNM_SOUND_TYPE_SE:		cd.item_list[item_no].file_name = (0 <= se_no && se_no < Gp_ini->se_cnt) ? Gp_ini->se[se_no].file_name : _T("");			break;
		}
		cd.item_list[item_no].volume = Gp_sound->m_se.get_volume();
		cd.item_list[item_no].pause_flag = false;
		cd.item_list[item_no].delay_time = 0;
		cd.item_list[item_no].state = Gp_sound->m_se.get_player(i)->get_play_state();
		cd.item_list[item_no].state2 = 0;
		cd.item_list[item_no].option = 0;
		cd.item_list[item_no].bgmfade_target = false;
		cd.item_list[item_no].bgmfade2_target = false;
	}

	// MOV
	cd.item_list[item_no].type = 5;
	cd.item_list[item_no].player_no = 0;
	cd.item_list[item_no].file_name = Gp_sound->m_mov.get_file_name();
	cd.item_list[item_no].volume = 255;
	cd.item_list[item_no].pause_flag = false;
	cd.item_list[item_no].delay_time = 0;
	int mov_play_state = Gp_sound->m_mov.get_state();
	switch (mov_play_state)	{
		case TNM_MOVIE_PLAY_STATE_PLAY:		cd.item_list[item_no].state = TNM_PLAYER_STATE_PLAY;	break;
		default:							cd.item_list[item_no].state = TNM_PLAYER_STATE_FREE;	break;
	}
	cd.item_list[item_no].state2 = Gp_sound->m_mov.get_wmp_state_detail();
	cd.item_list[item_no].option = 0;
	cd.item_list[item_no].bgmfade_target = false;
	cd.item_list[item_no].bgmfade2_target = false;

	// リストビューのアイテム数を調整する
	if (frame_first_flag || cd.item_list.size() != m_ld.item_list.size())
		lv.set_item_cnt((int)cd.item_list.size());

	// 増えたもしくは変更された分に関して値を表示
	for (int i = 0; i < (int)cd.item_list.size(); i++)	{
		bool change = frame_first_flag || i >= (int)m_ld.item_list.size();

		// タイプ
		if (change || cd.item_list[i].type != m_ld.item_list[i].type)	{
			TSTR str;
			switch (cd.item_list[i].type)	{
				case 0:		str = _T("BGM");		break;
				case 1:		str = _T("KOE");		break;
				case 2:		str = _T("汎用PCM");	break;
				case 3:		str = _T("専用PCM");	break;
				case 4:		str = _T("SE");			break;
				case 5:		str = _T("MOV");		break;
			}
			lv.set_cell_text(i, 0, str);
		}

		// ID
		if (change || cd.item_list[i].player_no != m_ld.item_list[i].player_no)	{
			lv.set_cell_text(i, 1, tostr(cd.item_list[i].player_no));
		}

		// ファイル名
		if (change || cd.item_list[i].file_name != m_ld.item_list[i].file_name)	{
			lv.set_cell_text(i, 2, cd.item_list[i].file_name);
		}

		// ボリューム
		if (change || cd.item_list[i].volume != m_ld.item_list[i].volume)	{
			lv.set_cell_text(i, 3, tostr(cd.item_list[i].volume));
		}

		// ポーズフラグ
		if (change || cd.item_list[i].pause_flag != m_ld.item_list[i].pause_flag)	{
			lv.set_cell_text(i, 4, cd.item_list[i].pause_flag ? _T("ポーズ中") : _T(""));
		}

		// 待ち時間
		if (change || cd.item_list[i].delay_time != m_ld.item_list[i].delay_time)	{
			lv.set_cell_text(i, 5, tostr(cd.item_list[i].delay_time));
		}

		// 状態
		if (change || cd.item_list[i].state != m_ld.item_list[i].state)	{
			TSTR str;
			switch (cd.item_list[i].state)	{
				case TNM_PLAYER_STATE_FREE:				str = _T("");			break;
				case TNM_PLAYER_STATE_PLAY:				str = _T("再生中");		break;
				case TNM_PLAYER_STATE_FADE_OUT:			str = _T("ﾌｪｰﾄﾞｱｳﾄ中");	break;
				case TNM_PLAYER_STATE_PAUSE:			str = _T("一時停止中");		break;
			}
			lv.set_cell_text(i, 6, str);
		}

		// ｵﾌﾟｼｮﾝ
		if (change || cd.item_list[i].option != m_ld.item_list[i].option)	{
			TSTR str;
			switch (cd.item_list[i].option)	{
				case 0:	str = _T("");						break;
				case 1:	str = _T("ループ");					break;
				case 2:	str = _T("再生終了待ち");			break;
				case 3:	str = _T("フェードアウト終了待ち");	break;
			}
			lv.set_cell_text(i, 8, str);
		}

		// bgmfade
		if (change || cd.item_list[i].bgmfade_target != m_ld.item_list[i].bgmfade_target)	{
			lv.set_cell_text(i, 9, tostr(cd.item_list[i].bgmfade_target));
		}
		if (change || cd.item_list[i].bgmfade2_target != m_ld.item_list[i].bgmfade2_target)	{
			lv.set_cell_text(i, 10, tostr(cd.item_list[i].bgmfade2_target));
		}
	}

	// データを記憶
	m_ld = cd;

	// 初回フラグをおろす
	frame_first_flag = false;
}
