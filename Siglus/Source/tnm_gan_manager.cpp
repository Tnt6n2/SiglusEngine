#include	"pch.h"

#include	"data/tnm_gan_data.h"
#include	"data/tnm_gan_manager.h"

// ****************************************************************
// ＧＡＮマネージャの実体
// ================================================================
C_tnm_gan_manager	G_gan_manager;

// ****************************************************************
// コンストラクタ
// ================================================================
C_tnm_gan_manager::C_tnm_gan_manager()
{
	work.map_max_cnt = 0;
}

// ****************************************************************
// 読み込み
// ================================================================
BSP<C_gan_data> C_tnm_gan_manager::load(CTSTR& file_path)
{
	// すでに読み込まれていないか検索する
	GAN_DATA_MAP::iterator itr = work.data_map.find(file_path);
	if (itr != work.data_map.end())	{	return itr->second;	}

	// 読み込み
	GAN_DATA_PAIR data_pair;
	data_pair.second = BSP<C_gan_data>(new C_gan_data);
	if (!data_pair.second->load(file_path))	{
		return BSP<C_gan_data>();	// 読み込みエラー
	}

	// マップに追加
	data_pair.first = file_path;
	work.data_map.insert(data_pair);

	// 最大マップ数
	int now_cnt = (int )work.data_map.size();
	if (work.map_max_cnt < now_cnt)	{	work.map_max_cnt = now_cnt;	}

	return data_pair.second;
}

// ****************************************************************
// リソースデータを整理する
// ================================================================
void C_tnm_gan_manager::organize()
{
	if (work.data_map.empty())	{	return;	}
	for (GAN_DATA_MAP::iterator itr = work.data_map.begin(); itr != work.data_map.end();)	{
		if (itr->second.use_count() == 1)	{
			itr = work.data_map.erase(itr);
		}
		else	{
			++itr;
		}
	}
}

// ****************************************************************
// 現在の文字マップ数を取得する
// ================================================================
int C_tnm_gan_manager::get_map_now_cnt()
{
	return (int)work.data_map.size();
}

// ****************************************************************
// 最大の文字マップ数を取得する
// ================================================================
int C_tnm_gan_manager::get_map_max_cnt()
{
	return work.map_max_cnt;
}



