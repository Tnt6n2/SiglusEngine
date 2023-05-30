#pragma		once

// 圧縮単位
#define		NWA_UNIT_SAMPLE_CNT			512

// ＮＷＡヘッダ
struct NWA_HEADER_STRUCT	{
	WORD	channels;				// チャンネル（1=ﾓﾉﾗﾙ / 2=ｽﾃﾚｵ）
	WORD	bits_per_sample;		// ビット（8bit / 16bit）
	DWORD	samples_per_sec;		// 周波数（11025 / 22050 / 44100）

	int		pack_mod;				// 圧縮モード（-1=無圧縮  0=低品質  1=中品質  2=高品質  3=より高品質  4=よりより高品質  5=よりよりより高品質）
	int		zero_mod;				// 無音圧縮モード（0=何もしない  1=無音部分を個数圧縮する）

	DWORD	unit_cnt;				// ユニット数

	DWORD	original_size;			// オリジナルサイズ
	DWORD	pack_size;				// 圧縮後のサイズ
	DWORD	sample_cnt;				// サンプル数（チャンネルごとに数えています）

	DWORD	unit_sample_cnt;		// 圧縮単位のサンプル数
	DWORD	last_sample_cnt;		// 最後の圧縮のサンプル数
	DWORD	last_sample_pack_size;	// 最後の圧縮サイズ
};

// ＮＷＡ用の展開用のテーブルデータ
struct NWA_TABLE_STRUCT		{
	int offset;
};

// ****************************************************************
// ＮＷＡ圧縮
// ================================================================
class Cnwa_pack
{

public:
				Cnwa_pack();						// コンストラクタ

	bool		ready(BYTE* src_data_, DWORD src_data_size, WORD channels, WORD bits_per_sample, DWORD samples_per_sec, int pack_mod, int zero_mod, DWORD break_out_size_);		// 準備
	bool		proc(BYTE* dst_buf_, DWORD dst_buf_size_);		// プロセス

	DWORD		src_complate_size;					// 処理完了サイズ
	DWORD		dst_buf_need_size;					// 足りないバッファ数
	DWORD		pack_size;							// 圧縮サイズ

private:

	// プロセス関数
	bool		_proc_start();						// プロセス スタート
	bool		_proc_copy();						// プロセス コピー
	bool		_proc_end();						// プロセス 終了処理
	bool		_proc_check_dst_buf_size();			// ＮＷＡ圧縮 プロセス 出力先サイズを判定

	// 圧縮関数
	DWORD		_pack_func_pack(BYTE* src_p, DWORD src_size);	// 圧縮関数 本体

	void		_pack_func_pack16m();				// 圧縮関数 １６ビット・モノラル
	void		_pack_func_pack16s();				// 圧縮関数 １６ビット・ステレオ
	void		_pack_func_pack16(int *nowsmp);		// 圧縮関数 １６ビット

	void		_pack_func_set_bit(DWORD add_bit_cnt, DWORD set_bit_dat);	// 圧縮関数 ビットフラグ設定

	bool		_pack_func_zero_proc();				// 圧縮関数 ＺＥＲＯプロセス
	bool		_pack_func_zero_check();			// 圧縮関数 ＺＥＲＯ判定
	void		_pack_func_zero_set();				// 圧縮関数 ＺＥＲＯ設定
	void		_pack_func_set_bit_zero_one();		// 圧縮関数 ＺＥＲＯフラグ１つを設定
	void		_pack_func_set_bit_zero();			// 圧縮関数 ＺＥＲＯフラグを設定

	void		_pack_func_dat_set0(DWORD sign);	// 圧縮関数 データセット０
	void		_pack_func_dat_set1(DWORD sign);	// 圧縮関数 データセット１
	void		_pack_func_dat_set2(DWORD sign);	// 圧縮関数 データセット２
	void		_pack_func_dat_set3(DWORD sign);	// 圧縮関数 データセット３
	void		_pack_func_dat_set4(DWORD sign);	// 圧縮関数 データセット４
	void		_pack_func_dat_set5(DWORD sign);	// 圧縮関数 データセット５

	// 関数テーブル
	typedef			void (Cnwa_pack::*_PACK_FUNC)(DWORD);
	_PACK_FUNC		_pack_func_dat_set[6];

	// プロセスステップ
	enum PROC_STEP{
		__START,
		__PACK,
		__COPY,
		__LAST_COPY,
		__END,
		__COMPLATE,
	};
	PROC_STEP	_proc_step;					// プロセスステップ

	// ヘッダ
	NWA_HEADER_STRUCT	_header;

	DWORD		_byte_per_sample;			// １サンプルのバイトサイズ

	// ユニット情報
	DWORD		_unit_index;				// ユニットのインデックス
	DWORD		_unit_size;					// ユニットのサイズ
	DWORD		_last_unit_size;			// 最後のユニットのサイズ

	BYTE*		_src_data;					// ソースデータポインタ

	// 出力先バッファ
	BYTE*		_dst_buf;					// 出力先バッファ
	DWORD		_dst_buf_size;				// 出力先バッファサイズ
	DWORD		_dst_buf_additional;		// 出力先バッファの追加サイズ

	DWORD		_pack_buf_size;				// 圧縮バッファサイズ

	// 処理サイズに応じてループ抜け
	DWORD		_break_out_size;			// ループ抜け処理サイズ
	DWORD		_break_out_last;			// 前回のループ抜け値

	// 圧縮処理変数
	struct PACK{
		BYTE*		src_p;					// ソースデータポインタ
		UINT_PTR	src_end;				// ソースデータ終了位置
		std::vector<BYTE>	dst;			// 出力先バッファ
		BYTE*		dst_p;					// 出力先バッファポインタ
		DWORD		dst_bit_size;			// 出力先バッファのビットサイズ
		DWORD		zero_cnt;				// ＺＥＲＯの数
		DWORD		transition;				// サンプル値の変移
	} _pack;
};

