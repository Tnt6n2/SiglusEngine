#pragma		once

// ****************************************************************
// ＧＡＮデータ
// ================================================================

// パターンデータ
struct S_gan_pat_data
{
	int		pat_no;
	int		x;
	int		y;
	int		z;
	BYTE	tr;
	int		wait;
	int		keika_time;		// パターンの先頭からの経過時間

	// コンストラクタ
	S_gan_pat_data()
	{
		pat_no = 0;
		x = 0;
		y = 0;
		z = 0;
		tr = 255;
		wait = 0;
		keika_time = 0;
	}
};

// セットデータ
struct S_gan_set_data
{
	ARRAY<S_gan_pat_data>	pat_list;
	int						total_time;

	// コンストラクタ
	S_gan_set_data()
	{
		pat_list.clear();
		total_time = 0;
	}
};

// 本体
class C_gan_data
{

public:

	TSTR					g00_file_name;			// エディター用のファイル名
	ARRAY<S_gan_set_data>	set_list;				// セットリスト

	bool			load(CTSTR& file_path);			// 読み込み

private:

	bool			analize(ARRAY<BYTE>& buf);		// 解析
	bool			analize_func(BYTE* sp);			// 解析ファンクション
	bool			analize_func_set(BYTE* sp, BYTE** ret_sp, S_gan_set_data* set, int* keika_time);	// セット解析
	bool			analize_func_pat(BYTE* sp, BYTE** ret_sp, S_gan_pat_data* pat, int* keika_time);	// パターン解析
};

