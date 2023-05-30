#pragma		once

// ****************************************************************
// 【名前】
// ================================================================

typedef		std::pair<TSTR, int>	NAMAE_FIND_PAIR;
typedef		std::map<TSTR, int>		NAMAE_FIND_MAP;

class C_tnm_namae
{

public:

	void		init();						// 【名前】初期化
	TSTR		change(CTSTR& name, int *color_mod, int *moji_color_no, int *shadow_color_no, int *fuchi_color_no);		// 【名前】変換

	// データベース用名前（デバッグ情報用）
	struct Snamae_data_base
	{
		bool		regist_flag;			// 登録の有無（Gameexe.ini に登録されているかどうか）
		bool		exist_flag;				// 存在の有無（シーン中で使用されているかどうか）
		TSTR		regist_name_str;		// 登録名
		TSTR		change_name_str;		// 変換名
		int			color_mod;				// 色モード
		int			moji_color_no;			// 文字色
		int			shadow_color_no;		// 影色
	};
	ARRAY<Snamae_data_base>		namae_data_base_list;

private:

	// 検索用名前マップ
	NAMAE_FIND_MAP		namae_find_map;

};

// ****************************************************************
// キャラクタ音声
// ================================================================
class C_tnm_chrkoe
{
public:
	TSTR	name_str;
	bool	look_flag;
};

