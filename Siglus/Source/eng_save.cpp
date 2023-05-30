#include	"pch.h"

#include	"tnm_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_syscom.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_save_01.h"
#include	"data/tnm_read_flag.h"

#include	"element/elm_flag.h"

#include	"engine/eng_display.h"
#include	"engine/eng_config.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_etc.h"
#include	"engine/eng_save.h"
#include	"engine/ifc_stack.h"
#include	"engine/eng_flag.h"
#include	"engine/ifc_proc_stack.h"

// ****************************************************************
// セーブ（ローカル）
// ================================================================
bool tnm_save_local()
{
	// エンジン本体を呼び出す
	return Gp_eng->save_local();
}

// ****************************************************************
// ロード（ローカル）
// ================================================================
bool tnm_load_local()
{
	// エンジン本体を呼び出す
	return Gp_eng->load_local();
}

// ****************************************************************
// セーブ（ローカルの現在の文章）
// ================================================================
bool tnm_save_local_msg(CTSTR& msg)
{
	// エンジン本体を呼び出す
	return Gp_eng->save_local_msg(msg);
}

// ****************************************************************
// セーブ（コール）
// ================================================================
bool tnm_save_call()
{
	// エンジン本体を呼び出す
	return Gp_eng->save_call();
}

// ****************************************************************
// ロード（コール）
// ================================================================
bool tnm_load_call()
{
	// エンジン本体を呼び出す
	return Gp_eng->load_call();
}

// ****************************************************************
// セーブ履歴を初期化
// ================================================================
void tnm_init_save_history()
{
	if (Gp_global->debug_flag)
	{
		Gp_save_history->update_flag = false;
		Gp_save_history->update_time = ::timeGetTime();
		Gp_save_history->top = 0;
		Gp_save_history->cnt = 0;
		Gp_save_history->item.resize(Gp_ini->save_history_cnt);
	}
}

// ****************************************************************
// メッセージバックセーブを初期化
// ================================================================
void tnm_init_backlog_save()
{
	Gp_backlog_save->map.clear();
}

// ****************************************************************
// メッセージバックセーブに ID が存在するかを判定
// ================================================================
bool tnm_check_backlog_save(S_tid id)
{
	return Gp_backlog_save->map.find(id) != Gp_backlog_save->map.end();
}

// ****************************************************************
// メッセージバックセーブを整理する
// ================================================================
void tnm_remove_backlog_save()
{
	Gp_msg_back->remove();
}
	
// ****************************************************************
// ローカルセーブデータを初期化
// ================================================================
void tnm_init_local_save()
{
	Gp_eng->m_local_save_history_index = -1;
	Gp_eng->m_local_save.save_id = S_tid();
	Gp_eng->m_local_save.append_dir.clear();
	Gp_eng->m_local_save.append_name.clear();
	Gp_eng->m_local_save.save_scene_title.clear();
	Gp_eng->m_local_save.save_msg.clear();
	Gp_eng->m_local_save.save_full_msg.clear();
	Gp_eng->m_local_save.save_stream.clear();
	Gp_eng->m_local_save.save_stream_ex.clear();
}

// ****************************************************************
// ローカルセーブデータを設定
// ================================================================
void tnm_set_local_save(S_tnm_local_save* p_local_save)
{
	Gp_eng->m_local_save = *p_local_save;
}

// ****************************************************************
// ローカルセーブデータを判定
// ================================================================
bool tnm_is_exist_local_save()
{
	return !Gp_eng->m_local_save.save_stream.empty();
}

// ****************************************************************
// 選択肢セーブデータを初期化
// ================================================================
void tnm_init_sel_save()
{
	Gp_eng->m_sel_save.list.clear();
	Gp_eng->m_sel_save.map.clear();
}

void tnm_init_sel_save_stock()
{
	Gp_eng->m_sel_save_stock.save_id = S_tid();
	Gp_eng->m_sel_save_stock.append_dir.clear();
	Gp_eng->m_sel_save_stock.append_name.clear();
	Gp_eng->m_sel_save_stock.save_scene_title.clear();
	Gp_eng->m_sel_save_stock.save_msg.clear();
	Gp_eng->m_sel_save_stock.save_full_msg.clear();
	Gp_eng->m_sel_save_stock.save_stream.clear();
	Gp_eng->m_sel_save_stock.save_stream_ex.clear();
}

// ****************************************************************
// 選択肢セーブデータを追加
// ================================================================
void tnm_set_sel_save(S_tnm_local_save* p_local_save)
{
	if (Gp_ini->sel_save_cnt > 0 && !p_local_save->save_stream.empty())
	{
		// 満杯なら先頭を削除
		if (Gp_eng->m_sel_save.list.size() == Gp_ini->sel_save_cnt)
		{
			S_tid erase_id = Gp_eng->m_sel_save.list[0];
			Gp_eng->m_sel_save.map.erase(erase_id);
			Gp_eng->m_sel_save.list.erase(Gp_eng->m_sel_save.list.begin());
		}

		// セーブデータをコピー
		BSP<S_tnm_local_save> save(new S_tnm_local_save);
		save->save_id = p_local_save->save_id;
		save->append_dir = p_local_save->append_dir;
		save->append_name = p_local_save->append_name;
		save->save_scene_title = p_local_save->save_scene_title;
		save->save_msg = p_local_save->save_msg;
		save->save_full_msg = p_local_save->save_full_msg;
		save->save_stream = p_local_save->save_stream;
		save->save_stream_ex = p_local_save->save_stream_ex;

		// セーブデータを追加
		Gp_eng->m_sel_save.list.push_back(p_local_save->save_id);
		Gp_eng->m_sel_save.map[p_local_save->save_id] = save;
	}
}

// ****************************************************************
// 選択肢セーブデータを取得
// ================================================================
void tnm_get_sel_save(S_tnm_local_save* p_local_save)
{
	if (!Gp_eng->m_sel_save.list.empty())
	{
		S_tid save_id = Gp_eng->m_sel_save.list.back();
		p_local_save->save_id = Gp_eng->m_sel_save.map[save_id]->save_id;
		p_local_save->append_dir = Gp_eng->m_sel_save.map[save_id]->append_dir;
		p_local_save->append_name = Gp_eng->m_sel_save.map[save_id]->append_name;
		p_local_save->save_scene_title = Gp_eng->m_sel_save.map[save_id]->save_scene_title;
		p_local_save->save_msg = Gp_eng->m_sel_save.map[save_id]->save_msg;
		p_local_save->save_full_msg = Gp_eng->m_sel_save.map[save_id]->save_full_msg;
		p_local_save->save_stream = Gp_eng->m_sel_save.map[save_id]->save_stream;
		p_local_save->save_stream_ex = Gp_eng->m_sel_save.map[save_id]->save_stream_ex;
	}
}

// ****************************************************************
// 選択肢セーブデータを判定
// ================================================================
bool tnm_is_exist_sel_save()
{
	return !Gp_eng->m_sel_save.list.empty();
}

// ****************************************************************
// ローカルセーブデータを選択肢セーブデータにコピー
// ================================================================
void tnm_copy_local_save_to_sel_save()
{
	tnm_set_sel_save(&Gp_eng->m_local_save);
}
void tnm_copy_local_save_to_sel_save_stock()
{
	Gp_eng->m_sel_save_stock = Gp_eng->m_local_save;
}

// ****************************************************************
// 選択肢セーブデータをコピー
// ================================================================
void tnm_copy_sel_save_stock_to_sel_save()
{
	tnm_set_sel_save(&Gp_eng->m_sel_save_stock);
}

// ****************************************************************
// インナーセーブ
// ================================================================
void tnm_savedata_system_clear_inner_save(int inner_save_no)
{
	if (inner_save_no < 0 || (int)Gp_eng->m_inner_save_list.size() <= inner_save_no)
		return;

	Gp_eng->m_inner_save_list[inner_save_no].save_id = S_tid();
	Gp_eng->m_inner_save_list[inner_save_no].append_dir.clear();
	Gp_eng->m_inner_save_list[inner_save_no].append_name.clear();
	Gp_eng->m_inner_save_list[inner_save_no].save_scene_title.clear();
	Gp_eng->m_inner_save_list[inner_save_no].save_msg.clear();
	Gp_eng->m_inner_save_list[inner_save_no].save_full_msg.clear();
	Gp_eng->m_inner_save_list[inner_save_no].save_stream.clear();
	Gp_eng->m_inner_save_list[inner_save_no].save_stream_ex.clear();
}

void tnm_savedata_system_copy_local_save_to_inner_save(int inner_save_no)
{
	if (inner_save_no < 0 || (int)Gp_eng->m_inner_save_list.size() <= inner_save_no)
		return;

	Gp_eng->m_inner_save_list[inner_save_no] = Gp_eng->m_local_save;
}

void tnm_savedata_system_copy_inner_save(int inner_save_no_from, int inner_save_no_to)
{
	if (inner_save_no_from < 0 || (int)Gp_eng->m_inner_save_list.size() <= inner_save_no_from)
		return;

	if (inner_save_no_to < 0 || (int)Gp_eng->m_inner_save_list.size() <= inner_save_no_to)
		return;

	Gp_eng->m_inner_save_list[inner_save_no_to] = Gp_eng->m_inner_save_list[inner_save_no_from];
}

void tnm_savedata_system_get_inner_save(int inner_save_no, S_tnm_local_save* p_local_save)
{
	if (inner_save_no < 0 || (int)Gp_eng->m_inner_save_list.size() <= inner_save_no)
		return;

	*p_local_save = Gp_eng->m_inner_save_list[inner_save_no];
}

bool tnm_savedata_system_check_inner_save(int inner_save_no)
{
	if (inner_save_no < 0 || (int)Gp_eng->m_inner_save_list.size() <= inner_save_no)
		return false;

	if (Gp_eng->m_inner_save_list[inner_save_no].save_stream.empty())
		return false;

	return true;
}

int tnm_savedata_system_get_inner_save_size(int inner_save_no)
{
	if (inner_save_no < 0 || (int)Gp_eng->m_inner_save_list.size() <= inner_save_no)
		return 0;

	return (int)Gp_eng->m_inner_save_list[inner_save_no].save_stream.size();
}

// ****************************************************************
// ファイルセーブ（ヘッダ）
// ================================================================
bool tnm_save_save_header(int save_no, S_tnm_save_header* p_header)
{
	if (p_header == NULL)
		return false;

	// 範囲チェック
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// キャッシュへセーブ
	if (!Gp_eng->m_save_cache.save_cache(save_no, p_header))
		return false;

	return true;
}

// ****************************************************************
// ファイルロード（ヘッダ）
// ================================================================
bool tnm_load_save_header(int save_no, S_tnm_save_header* p_header)
{
	if (p_header == NULL)
		return false;

	ZeroMemory(p_header, sizeof(*p_header));

	// 範囲チェック
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// キャッシュからロード
	if (!Gp_eng->m_save_cache.load_cache(save_no, p_header))
		return false;

	return true;
}

// ****************************************************************
// セーブ可能かどうか判定
// ================================================================
bool tnm_is_save_enable()
{
	// ストリームが空ならセーブできない
	if (Gp_eng->m_local_save.save_stream.empty())
		return false;

	return true;
}

// ****************************************************************
// セーブ可能かどうか判定
// ================================================================
bool tnm_is_save_enable(int save_no)
{
	// ストリームが空ならセーブできない
	if (Gp_eng->m_local_save.save_stream.empty())
		return false;

	return true;
}

// ****************************************************************
// ロード可能かどうか判定
// ================================================================
bool tnm_is_load_enable()
{
	return true;	// 必ず出来る
}

// ****************************************************************
// ロード可能かどうか判定
// ================================================================
bool tnm_is_load_enable(int save_no)
{
	// ファイルがないならロードできない
	if (!tnm_check_save_file(save_no))
		return false;

	return true;
}

// ****************************************************************
// ファイルチェック
// ================================================================
bool tnm_check_save_file(int save_no)
{
	// 範囲チェック
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// キャッシュからチェック
	if (!Gp_eng->m_save_cache.check_file(save_no))
		return false;

	return true;
}

// ****************************************************************
// 最新のセーブデータ番号を取得
// ================================================================
int tnm_get_new_save_no()
{
	return Gp_eng->m_save_cache.get_new_save_data_no();
}

int tnm_get_new_save_no(int start, int cnt)
{
	// 範囲指定の場合は結果がキャッシュできないので探します
	// 各データの日付自体はキャッシュを使います

	int new_no = -1;
	S_tnm_save_header new_header = { 0 };

	for (int i = 0; i < cnt; i++)
	{
		int save_no = start + i;

		S_tnm_save_header header = { 0 };

		if (0 <= save_no && save_no < Gp_ini->save_cnt)
		{
			if (Gp_eng->m_save_cache.load_cache(save_no, &header))
			{
				if (new_no == -1 || new_header < header)
				{
					new_no = save_no;
					new_header = header;
				}
			}
		}
	}

	return new_no;
}

// ****************************************************************
// 最新のクイックセーブデータ番号を取得
// ================================================================
int tnm_get_new_quick_save_no()
{
	return Gp_eng->m_save_cache.get_new_quick_save_data_no();
}

int tnm_get_new_quick_save_no(int start, int cnt)
{
	// 範囲指定の場合は結果がキャッシュできないので探します
	// 各データの日付自体はキャッシュを使います

	int new_no = -1;
	S_tnm_save_header new_header = { 0 };

	for (int i = 0; i < cnt; i++)
	{
		int quick_save_no = start + i;

		S_tnm_save_header header = { 0 };

		if (!Gp_eng->m_save_cache.load_cache(Gp_ini->save_cnt + quick_save_no, &header))
		{
			if (new_no == -1 || new_header < header)
			{
				new_no = quick_save_no;
				new_header = header;
			}
		}
	}

	return new_no;
}