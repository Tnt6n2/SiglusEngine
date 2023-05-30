#pragma		once

class	C_gan_data;

// ****************************************************************
// 文字マネージャ
// ================================================================
class	C_tnm_gan_manager;
extern	C_tnm_gan_manager	G_gan_manager;

// ****************************************************************
// ＧＡＮマネージャ
// ================================================================
class C_tnm_gan_manager
{
public:

	C_tnm_gan_manager();							// コンストラクタ
	BSP<C_gan_data>	load(CTSTR& file_name);			// 読み込み
	void			organize();						// リソースデータを整理する
	int				get_map_now_cnt();				// 現在の文字マップ数を取得する
	int				get_map_max_cnt();				// 最大の文字マップ数を取得する

private:

	typedef		std::pair<TSTR, BSP<C_gan_data> >	GAN_DATA_PAIR;		// ＧＡＮデータペア
	typedef		std::map<TSTR, BSP<C_gan_data> >	GAN_DATA_MAP;		// ＧＡＮデータマップ

	struct WORK
	{
		GAN_DATA_MAP	data_map;					// ＧＡＮデータマップ
		int				map_max_cnt;				// ＧＡＮデータマップマップの最大数
	}	work;

};



