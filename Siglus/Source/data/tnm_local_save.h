#pragma		once

#include	"data/tnm_save_00.h"
#include	"data/tnm_timer.h"

// ****************************************************************
// ローカルセーブ
// ================================================================
struct S_tnm_local_save
{
	S_tid				save_id;
	TSTR				append_dir;
	TSTR				append_name;
	TSTR				save_scene_title;			// シーンタイトル
	TSTR				save_msg;					// 現在の文章
	TSTR				save_full_msg;				// 現在の文章
	C_tnm_save_stream	save_stream;				// ローカルデータ
	C_tnm_save_stream	save_stream_ex;				// ローカル設定
};

// ****************************************************************
// 選択肢セーブシステム
// ================================================================
struct S_tnm_sel_save
{
	ARRAY<S_tid>							list;	// 前の選択肢に戻る用のローカルセーブリスト
	std::map<S_tid, BSP<S_tnm_local_save>>	map;	// 前の選択肢に戻る用のローカルセーブ実データ
};


// ****************************************************************
// セーブ履歴
// ================================================================
struct S_tnm_save_history_item
{
	BYTE				flag;						// フラグ
	SYSTEMTIME			systemtime;					// 時間
	S_tnm_local_save	local_save;					// ローカルセーブ
};

struct S_tnm_save_history
{
	bool							update_flag;	// 履歴の更新フラグ（ダイアログに反映した時点で降ろす）
	DWORD							update_time;	// 履歴を更新した時刻（一定期間で更新するのに使う）
	int								top;			// 履歴の先頭
	int								cnt;			// 履歴の数
	ARRAY<S_tnm_save_history_item>	item;			// 履歴
};

// ****************************************************************
// バックログセーブ
// ================================================================
struct S_tnm_backlog_save
{
	std::map<S_tid, BSP<S_tnm_local_save>>	map;
};
