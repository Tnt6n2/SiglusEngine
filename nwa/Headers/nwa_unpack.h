#pragma		once

#include	"nwa_pack.h"

// ユニット解凍
int nwa_unpack_unit(BYTE* src_p, DWORD src_smp_cnt, DWORD unpack_smp_top, DWORD unpack_smp_cnt, BYTE *dst_p, NWA_HEADER_STRUCT* header);

// ****************************************************************
// ＮＷＡ解凍
// ================================================================
class Cnwa_unpack
{

public:
				Cnwa_unpack();									// コンストラクタ
				~Cnwa_unpack();									// デストラクタ

	// 主関数
	bool		open(const TCHAR* file_path, DWORD offset);		// ファイルオープン
	void		close();										// ファイルクローズ
	void		set_read_sample_pos(DWORD sample_pos);			// 読み込み位置を設定（サンプル値：ステレオは左右で１サンプルです）
	DWORD		get_read_sample_pos();							// 読み込み位置を取得（サンプル値：ステレオは左右で１サンプルです）
	DWORD		read_sample(BYTE* buf, DWORD sample_cnt);		// 読み込み（サンプル数：ステレオは左右で１サンプルです）

	// 主情報取得関数
	WORD		get_info_channels();							// 情報取得 チャンネル数取得
	WORD		get_info_bits_per_sample();						// 情報取得 ビット値取得
	DWORD		get_info_samples_per_sec();						// 情報取得 周波数取得

	DWORD		get_info_sample_cnt();							// 情報取得 総サンプル数取得（サンプル数：ステレオは左右で１サンプルです）

	// 圧縮情報取得関数
	DWORD		get_info_original_size();						// 情報取得 オリジナルサイズ取得
	DWORD		get_info_pack_size();							// 情報取得 圧縮サイズ取得
	int			get_info_pack_mod();							// 情報取得 圧縮モード取得（-1=無圧縮  0=低品質  1=中品質  2=高品質  3=より高品質  4=よりより高品質  5=よりよりより高品質）
	int			get_info_zero_mod();							// 情報取得 ＺＥＲＯ圧縮モード取得（0=何もしない  1=無音部分を個数圧縮する）

private:

	void		_init();										// 初期化
	bool		_read_unit(DWORD unit_no);						// ユニットを読み込んで解凍する
	bool		_read_no_pack_data(DWORD need_byte_size, BYTE* dp, DWORD* copy_byte_size);		// 無圧縮データの読み込み

	struct	_PM{
		FILE*				fp;							// ファイルポインタ
		DWORD				offset;						// ファイルオフセット
		DWORD				read_sample_pos;			// 読み込みサンプル位置
	} _pm;

	struct	_INFO{
		NWA_HEADER_STRUCT	header;						// ＮＷＡヘッダ
		std::vector<DWORD>	unit_ofs_tbl;				// ユニットオフセットテーブル
		DWORD				one_sample_byte_size;		// １サンプルのバイト数
	} _info;

	struct	_UNPACK{
		std::vector<BYTE>	buf;						// 解凍バッファ
		DWORD				unit_no;					// 解凍ユニット番号
		DWORD				unit_sample_cnt;			// 解凍ユニットのサンプル数
	} _unpack;
};

