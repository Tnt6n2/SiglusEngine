#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_save_cache.h"

// ****************************************************************
// セーブデータキャッシュ：初期化
// ================================================================
void C_tnm_save_cache::init()
{
	cache_list.clear();
	cache_list.resize(Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt);

	// キャッシュの最新のセーブデータ番号を初期化
	clear_new_cache();
}

// ****************************************************************
// セーブデータキャッシュ：記憶
// ================================================================
bool C_tnm_save_cache::memory_cache(int save_no, S_tnm_save_header* p_header)
{
	// 範囲チェック
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// キャッシュに記憶
	cache_list[save_no].save_header = BSP<S_tnm_save_header>(new S_tnm_save_header);
	*cache_list[save_no].save_header = *p_header;

	// キャッシュの最新のセーブデータ番号を記憶する（★最新のセーブデータ番号が記憶されている場合のみ処理する）

	// １．通常のセーブデータ
	if (0 <= save_no && save_no < Gp_ini->save_cnt)	{
		if (new_save_data_no != -1 && new_save_data_no != save_no)	{
			if (*cache_list[new_save_data_no].save_header < *cache_list[save_no].save_header)	{
				new_save_data_no = save_no;
			}
		}
	}
	// ２．クイックセーブデータ
	if (Gp_ini->save_cnt <= save_no && save_no < Gp_ini->save_cnt + Gp_ini->quick_save_cnt)	{
		int quick_save_no = save_no - Gp_ini->save_cnt;
		if (new_quick_save_data_no != -1 && new_quick_save_data_no != quick_save_no)	{
			if (*cache_list[new_quick_save_data_no + Gp_ini->save_cnt].save_header < *cache_list[quick_save_no + Gp_ini->save_cnt].save_header)	{
				new_quick_save_data_no = quick_save_no;
			}
		}
	}

	return true;
}

// ****************************************************************
// セーブデータキャッシュ：クリア
// ================================================================
bool C_tnm_save_cache::clear_cache(int save_no)
{
	// 範囲チェック
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// キャッシュクリア
	cache_list[save_no].save_header.reset();
	cache_list[save_no].data_none_flag = false;			// データが存在しないと言う事のキャッシュも忘れずクリアする

	// キャッシュの最新のセーブデータ番号をクリア
	clear_new_cache();

	return true;
}

// ****************************************************************
// セーブデータキャッシュ：最新のセーブデータ番号クリア
// ================================================================
bool C_tnm_save_cache::clear_new_cache()
{
	new_save_data_no = -1;
	new_quick_save_data_no = -1;
	return true;
}

// ****************************************************************
// セーブデータキャッシュ：セーブ
// ================================================================
bool C_tnm_save_cache::save_cache(int save_no, S_tnm_save_header* p_header)
{
	if (p_header == NULL)
		return false;

	// 範囲チェック
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// キャッシュをクリア
	clear_cache(save_no);

	// パスを求める
	TSTR path = Gp_dir->get_local_save_file_path(save_no);

	// ファイルを開く
	C_file file;
	if (!file.open(path, _T("rb+")))	{
		cache_list[save_no].data_none_flag = true;		// データが存在しないと言う事をキャッシュに記憶する
		return false;
	}

	// ファイルに書き込む
	file.write(p_header, sizeof(S_tnm_save_header));
	file.close();

	return true;
}

// ****************************************************************
// セーブデータキャッシュ：ロード
// ================================================================
bool C_tnm_save_cache::load_cache(int save_no, S_tnm_save_header* p_header)
{
	if (p_header == NULL)
		return false;

	// 範囲チェック
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// キャッシュからロード
	if (cache_list[save_no].save_header)	{
		*p_header = *cache_list[save_no].save_header;
		return true;
	}
	else if (cache_list[save_no].data_none_flag)	{
		return false;		// データが存在しない
	}

	// パスを求める
	TSTR path = Gp_dir->get_local_save_file_path(save_no);

	// ファイルを開く
	C_file file;
	if (!file.open(path, _T("rb")))	{
		cache_list[save_no].data_none_flag = true;		// データが存在しないと言う事をキャッシュに記憶する
		return false;
	}

	// ファイルから読み込む
	file.read(p_header, sizeof(S_tnm_save_header));
	file.close();

	// キャッシュに記憶
	memory_cache(save_no, p_header);
	return true;
}

// ****************************************************************
// セーブデータキャッシュ：ファイルチェック
// ================================================================
bool C_tnm_save_cache::check_file(int save_no)
{
	// 範囲チェック
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// キャッシュがあれば存在する
	if (cache_list[save_no].save_header)	{
		return true;
	}
	else if (cache_list[save_no].data_none_flag)	{
		return false;		// データが存在しない
	}

	// パスを求める
	TSTR path = Gp_dir->get_local_save_file_path(save_no);

	// ファイルを開く
	C_file file;
	if (!file.open(path, _T("rb")))	{
		cache_list[save_no].data_none_flag = true;		// データが存在しないと言う事をキャッシュに記憶する
		return false;
	}

	// ファイルから読み込む
	S_tnm_save_header header;
	file.read(&header, sizeof(S_tnm_save_header));
	file.close();

	// キャッシュに記憶
	memory_cache(save_no, &header);
	return true;
}

// ****************************************************************
// 最新のセーブデータ番号を取得
// ================================================================
int C_tnm_save_cache::get_new_save_data_no()
{
	// キャッシュに記憶されている最新データ番号を戻す
	if (new_save_data_no != -1)
	{
		return new_save_data_no;
	}

	// キャッシュを読みながら最新のセーブデータを探す
	int new_no = -1;
	S_tnm_save_header header;
	for (int i = 0; i < Gp_ini->save_cnt; i++)
	{
		if (load_cache(i, &header))
		{
			// 最新（日付が最も大きい）のセーブデータを取得する
			if (new_no == -1 || *cache_list[new_no].save_header < header)
			{
				new_no = i;
			}
		}
	}

	// キャッシュに最新のセーブデータ番号を記憶する
	new_save_data_no = new_no;

	return new_save_data_no;
}

// ****************************************************************
// 最新のクイックセーブデータ番号を取得
// ================================================================
int C_tnm_save_cache::get_new_quick_save_data_no()
{
	// キャッシュに記憶されている最新データ番号を戻す
	if (new_quick_save_data_no != -1)
	{
		return new_quick_save_data_no;
	}

	// キャッシュを読みながら最新のセーブデータを探す
	int new_no = -1;
	S_tnm_save_header header;
	for (int i = 0; i < Gp_ini->quick_save_cnt; i++)
	{
		if (load_cache(i + Gp_ini->save_cnt, &header))
		{
			// 最新（日付が最も大きい）のセーブデータを取得する
			if (new_no == -1 || *cache_list[new_no + Gp_ini->save_cnt].save_header < header)
			{
				new_no = i;
			}
		}
	}

	// キャッシュに最新のセーブデータ番号を記憶する
	new_quick_save_data_no = new_no;

	return new_quick_save_data_no;
}