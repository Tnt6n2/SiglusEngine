#pragma		once

// ****************************************************************
// 文字マネージャ
// ================================================================
class	C_tnm_moji_manager;
extern	C_tnm_moji_manager	G_moji_manager;

// ****************************************************************
// 文字マネージャ
// ================================================================
//		よく使う文字（ひらがななど）のテクスチャを
//		記憶しておくことで、高速化を図ります。
// ================================================================
class C_tnm_moji_manager
{
public:

	// コンストラクタ
	C_tnm_moji_manager()
	{
		moji_map_max_cnt = 0;
		moji_memory_now_size = 0;
		moji_memory_max_size = 0;
		moji_load_counter = 0;
	}

	// 文字テクスチャをロードする
	BSP<C_d3d_texture>	load_moji(int moji_type, int moji, int moji_size, C_argb moji_color, MOJI_FACE_TYPE moji_face_type, bool tategaki, int buf_no);

	void		clear();								// クリアする
	int			get_moji_map_use_cnt();					// 使用中の文字マップ数を取得する
	int			get_moji_map_now_cnt();					// 現在の文字マップ数を取得する
	int			get_moji_map_max_cnt();					// 最大の文字マップ数を取得する

	__int64		get_moji_memory_use_size();				// 使用中の文字メモリーサイズを取得する
	__int64		get_moji_memory_now_size();				// 現在の文字メモリーサイズを取得する
	__int64		get_moji_memory_max_size();				// 最大の文字メモリーサイズを取得する

	int		get_moji_kind_useful_cnt();		// 有益な文字数を取得する
	int		get_moji_kind_unuseful_cnt();	// 非有益な文字数を取得する

private:

	// 文字テクスチャを作成する
	BSP<C_d3d_texture>	create_moji(int moji_type, int moji, int moji_size, C_argb moji_color, MOJI_FACE_TYPE moji_face_type, bool tategaki);

	struct S_moji
	{
		int				moji_type;
		int				moji;
		int				size;
		C_argb			color;
		MOJI_FACE_TYPE	face_type;

		bool operator < (const S_moji& rhs) const
		{
			if (moji_type < rhs.moji_type)	return true;
			if (moji_type > rhs.moji_type)	return false;
			if (moji < rhs.moji)			return true;
			if (moji > rhs.moji)			return false;
			if (size < rhs.size)			return true;
			if (size > rhs.size)			return false;
			if (color < rhs.color)			return true;
			if (color > rhs.color)			return false;
			if (face_type < rhs.face_type)	return true;
			return false;
			//return moji < rhs.moji || (moji == rhs.moji && (size < rhs.size || (size == rhs.size && (color < rhs.color || (color == rhs.color && type < rhs.type)))));
		}
	};
	struct S_texture
	{
		BSP<C_d3d_texture>	texture;
		int					moji_size;
		bool				useful;
		int					load_counter;
		int					texture_memory;
	};
	typedef		std::pair<S_moji, S_texture >		MOJI_PAIR;
	typedef		std::map<S_moji, S_texture >		MOJI_MAP;
	MOJI_MAP	moji_map;

	int		moji_map_max_cnt;			// 文字マップの最大数
	int		moji_load_counter;			// 文字のロードカウンター

	__int64		moji_memory_now_size;	// 現在の文字メモリーサイズ
	__int64		moji_memory_max_size;	// 最大の文字メモリーサイズ

	int		moji_kind_useful_cnt;		// 有益な文字数
	int		moji_kind_unuseful_cnt;		// 非有益な文字数
};


