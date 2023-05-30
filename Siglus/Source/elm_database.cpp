#include	"pch.h"

#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"element/elm_database.h"
#include	"engine/eng_dir.h"
#include	"engine/ifc_error.h"

#include	<lzss.h>

// ****************************************************************
// 非公開定数
// ================================================================
const	int		MAP_WIDTH   = 16;		// １行のピクセル数
const	int		TILE_WIDTH  = 5;		// タイルの幅
const	int		TILE_HEIGHT = 5;		// タイルの高さ
static	BYTE	TILE[TILE_WIDTH * TILE_HEIGHT] = {255,0,0,255,255, 0,0,255,255,0, 255,255,255,0,255, 0,0,255,0,0, 0,0,0,255,255};
static	DWORD	XORCODE[3] = {0x7190C70E, 0x499BF135, 0x89F4622D};

// ****************************************************************
// 非公開構造体
// ================================================================
struct Stnm_database_header
{
	int data_size;				// データサイズ
	int row_cnt;				// 行数
	int column_cnt;				// 列数
	int row_header_offset;		// 行ヘッダへのオフセット
	int column_header_offset;	// 列ヘッダへのオフセット
	int data_offset;			// データへのオフセット
	int str_offset;				// 文字列データへのオフセット
};

// ****************************************************************
// XOR をかける（32bit）
// ================================================================
void tnm_database_set_xor_32(DWORD* src, int data_size, DWORD xor_code){

	DWORD* p = src;
	int ui_size = data_size / sizeof(DWORD);

	// 32bit 単位で xor をかける
	for (int i = 0; i < ui_size; i++, p++)
		*p ^= xor_code;
}

// ****************************************************************
// マスクコピー
// ================================================================
void tnm_database_mask_copy(BYTE *dst, BYTE *src, int xl, int yl, BYTE *mask, int m_xl, int m_yl, int reverse)
{
#if 1
	tile_copy(dst, src, xl, yl, mask, m_xl, m_yl, 0, 0, reverse, 128);
#else
	Cnamalib_grp grp;
	grp.true_color_grp_draw(
		dst, xl, yl, 0, 0, false, 0, 0, 0, 0, 
		src, 0, xl, yl, 0, 0, false, 0, 0, 0, 0, 
		0, 0, TRUE_COLOR_GRP_DRAW_MOD_STD, 255, 0, 0, 0, 
		mask, m_xl, m_yl, 0, 0, reverse ? true : false, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 1000, 0, true, NULL, NULL);
#endif
}

// ****************************************************************
// データベースを解凍
// ================================================================
bool tnm_database_expand(BYTE* src_data, int src_size, ARRAY<BYTE>& dst_data)
{
	// XOR をかける
	tnm_database_set_xor_32((DWORD *)src_data, src_size, XORCODE[2]);

	// Lzss で解凍する
	int unpack_size = lzss_unpack(src_data, NULL);
	if (unpack_size == 0)
		return false;

	ARRAY<BYTE> unpack_data(unpack_size);
	lzss_unpack(src_data, unpack_data.get());

	// 一時マップに分割
	ARRAY<BYTE> temp_map_a(unpack_size), temp_map_b(unpack_size);
	tnm_database_mask_copy(temp_map_a.get(), unpack_data.get(), MAP_WIDTH, unpack_size / MAP_WIDTH / 4, TILE, TILE_WIDTH, TILE_HEIGHT, 0);
	tnm_database_mask_copy(temp_map_b.get(), unpack_data.get(), MAP_WIDTH, unpack_size / MAP_WIDTH / 4, TILE, TILE_WIDTH, TILE_HEIGHT, 1);
	tnm_database_set_xor_32((DWORD *)temp_map_a.get(), unpack_size, XORCODE[0]);
	tnm_database_set_xor_32((DWORD *)temp_map_b.get(), unpack_size, XORCODE[1]);

	// 出力データに結合
	dst_data.resize(unpack_size);
	tnm_database_mask_copy(dst_data.get(), temp_map_a.get(), MAP_WIDTH, unpack_size / MAP_WIDTH / 4, TILE, TILE_WIDTH, TILE_HEIGHT, 0);
	tnm_database_mask_copy(dst_data.get(), temp_map_b.get(), MAP_WIDTH, unpack_size / MAP_WIDTH / 4, TILE, TILE_WIDTH, TILE_HEIGHT, 1);

	return true;
}

// ****************************************************************
// エレメント：データベース：コンストラクタ
// ================================================================
C_elm_database::C_elm_database()
{
	m_type = 0;
	m_info.row_cnt = 0;
	m_info.column_cnt = 0;
	m_info.p_row_header = NULL;
	m_info.p_column_header = NULL;
	m_info.p_data = NULL;
	m_info.p_str = NULL;
}

// ****************************************************************
// エレメント：データベース：初期化
// ================================================================
void C_elm_database::init(S_element element, CTSTR& name, int ini_no)
{
	C_tnm_element::init(S_element(), FM_DATABASE, name, NULL);

	// ini に従ってデータベース読み込み
	if (!Gp_ini->database_name[ini_no].empty())	{
		load_dbs(Gp_ini->database_name[ini_no]);
	}
}

// ****************************************************************
// エレメント：データベース：読み込み
// ================================================================
bool C_elm_database::load_dbs(CTSTR& file_name)
{
	// ファイルを検索
	TSTR file_path = tnm_find_dat(file_name + _T(".dbs"));
	if (file_path.empty())	{
		tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("データベース ") + file_name + _T(" が見つかりませんでした。"));
		return false;
	}

	// ファイルを読み込む
	ARRAY<BYTE> file_data;
	if (!C_file::read_full_data(file_path, file_data))	{
		tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("データベース ") + file_name + _T(" が読み込めませんでした。"));
		return false;
	}

	// バージョン情報を取り出す
	m_type = *(int *)file_data.get();

	// データベースを解凍
	if (!tnm_database_expand(file_data.get() + sizeof(int), file_data.size() - sizeof(int), m_data))	{
		tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("データベース ") + file_name + _T(" の展開に失敗しました。不正なデータです。"));
		return false;
	}

	Stnm_database_header* p_header = (Stnm_database_header *)m_data.get();
	m_info.row_cnt = p_header->row_cnt;
	m_info.column_cnt = p_header->column_cnt;
	m_info.p_row_header = (S_tnm_database_row_header *)(m_data.get() + p_header->row_header_offset);
	m_info.p_column_header = (S_tnm_database_column_header *)(m_data.get() + p_header->column_header_offset);
	m_info.p_data = (DWORD *)(m_data.get() + p_header->data_offset);
	m_info.p_str = (char *)(m_data.get() + p_header->str_offset);

	return true;
}

// ****************************************************************
// エレメント：データベース：アイテム番号を取得
// ================================================================
int C_elm_database::get_item_no(int item_call_no)
{
	for (int i = 0; i < m_info.row_cnt; i++)	{
		if (m_info.p_row_header[i].call_no == item_call_no)
			return i;
	}

	return -1;	// 見つからなかった
}

// ****************************************************************
// エレメント：データベース：コラム番号を取得
// ================================================================
int C_elm_database::get_column_no(int column_call_no)
{
	for (int i = 0; i < m_info.column_cnt; i++)	{
		if (m_info.p_column_header[i].call_no == column_call_no)
			return i;
	}

	return -1;	// 見つからなかった
}

// ****************************************************************
// 共通：文字列を取得
// ================================================================
TSTR C_elm_database::get_str(int str_offset)
{
	TSTR ret_value;

	if (m_type == 0)
	{
		ret_value = MBSTR_to_TSTR(m_info.p_str + str_offset);
	}
	else
	{
		ret_value = (TCHAR *)(m_info.p_str + str_offset);
	}

	return ret_value;
}

// ****************************************************************
// エレメント：データベース：整数データを取得
// ================================================================
bool C_elm_database::get_data(int item_call_no, int column_call_no, int* ret_value)
{
	if (ret_value == NULL)
		return false;

	if (m_info.p_data == NULL)
		return false;

	// 呼出し番号から実番号を取得する
	int item_no = get_item_no(item_call_no);
	int column_no = get_column_no(column_call_no);
	if (item_no == -1 || column_no == -1)
		return false;

	// データの型をチェック
	if (m_info.p_column_header[column_no].data_type != 'V')
		return false;

	// データ番号を取得
	int data_no = item_no * m_info.column_cnt + column_no;
	// データを取得
	*ret_value = m_info.p_data[data_no];

	return true;
}

// ****************************************************************
// エレメント：データベース：文字列データを取得
// ================================================================
bool C_elm_database::get_data(int item_call_no, int column_call_no, TSTR& ret_value)
{
	if (m_info.p_data == NULL)
		return false;

	// 呼出し番号から実番号を取得する
	int item_no = get_item_no(item_call_no);
	int column_no = get_column_no(column_call_no);
	if (item_no == -1 || column_no == -1)
		return false;

	// データの型をチェック
	if (m_info.p_column_header[column_no].data_type != 'S')
		return false;

	// データ番号を取得
	int data_no = item_no * m_info.column_cnt + column_no;
	// 文字列へのオフセットを取得
	int str_offset = m_info.p_data[data_no];

	// 文字列を取得
	ret_value = get_str(str_offset);

	return true;
}

// ****************************************************************
// エレメント：データベース：アイテム番号を探す
// ================================================================
int C_elm_database::check_item_no(int item_call_no)
{
	if (m_info.p_data == NULL)
		return 0;

	// 呼出し番号から実番号を取得する
	int item_no = get_item_no(item_call_no);
	if (item_no == -1)
		return 0;

	return 1;
}

// ****************************************************************
// エレメント：データベース：コラム番号を探す
// ================================================================
int C_elm_database::check_column_no(int column_call_no)
{
	if (m_info.p_data == NULL)
		return 0;

	// 呼出し番号から実番号を取得する
	int column_no = get_column_no(column_call_no);
	if (column_no == -1)
		return 0;

	// データの型をチェック
	if (m_info.p_column_header[column_no].data_type == 'V')
		return 1;
	if (m_info.p_column_header[column_no].data_type == 'S')
		return 2;

	return 0;
}

// ****************************************************************
// エレメント：データベース：整数を探す
// ================================================================
int C_elm_database::find_num(int column_call_no, int num)
{
	if (m_info.p_data == NULL)
		return -1;

	// 呼出し番号から実番号を取得する
	int column_no = get_column_no(column_call_no);
	if (column_no == -1)
		return -1;

	// データの型をチェック
	if (m_info.p_column_header[column_no].data_type != 'V')
		return -1;

	// データを探す
	for (int item_no = 0; item_no < m_info.row_cnt; item_no ++)	{
		int data_no = item_no * m_info.column_cnt + column_no;
		int data = m_info.p_data[data_no];
		if (data == num)	{
			return m_info.p_row_header[item_no].call_no;	// 見つけた
		}
	}

	return -1;		// 見つからなかった
}

// ****************************************************************
// エレメント：データベース：文字列を探す
// ================================================================
int C_elm_database::find_str(int column_call_no, CTSTR& str)
{
	if (m_info.p_data == NULL)
		return -1;

	// 呼出し番号から実番号を取得する
	int column_no = get_column_no(column_call_no);
	if (column_no == -1)
		return -1;

	// データの型をチェック
	if (m_info.p_column_header[column_no].data_type != 'S')
		return -1;

	// 小文字に変換しておく
	TSTR str_for_find = str_to_lower_ret(str);

	// データを探す
	for (int item_no = 0; item_no < m_info.row_cnt; item_no ++)	{
		int data_no = item_no * m_info.column_cnt + column_no;
		int str_offset = m_info.p_data[data_no];

		// 文字列を取得
		TSTR data = get_str(str_offset);
		if (str_to_lower_ret(data) == str_for_find)	{
			return m_info.p_row_header[item_no].call_no;	// 見つけた
		}
	}

	return -1;		// 見つからなかった
}

// ****************************************************************
// エレメント：データベース：文字列を探す（大文字小文字を区別する）
// ================================================================
int C_elm_database::find_str_real(int column_call_no, CTSTR& str)
{
	if (m_info.p_data == NULL)
		return -1;

	// 呼出し番号から実番号を取得する
	int column_no = get_column_no(column_call_no);
	if (column_no == -1)
		return -1;

	// データの型をチェック
	if (m_info.p_column_header[column_no].data_type != 'S')
		return -1;

	// データを探す
	for (int item_no = 0; item_no < m_info.row_cnt; item_no ++)	{
		int data_no = item_no * m_info.column_cnt + column_no;
		int str_offset = m_info.p_data[data_no];
		TSTR data = get_str(str_offset);
		if (data == str)	{
			return m_info.p_row_header[item_no].call_no;	// 見つけた
		}
	}

	return -1;		// 見つからなかった
}

// ****************************************************************
// エレメント：データベースリスト：サブの初期化
// ================================================================
void C_elm_database_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"), i);
}

// ****************************************************************
// エレメント：データベースリスト：サブの再初期化
// ================================================================
void C_elm_database_list::_reinit(int begin, int end)
{
	// NOP
}

// ****************************************************************
// エレメント：データベースリスト：サブのセーブ
// ================================================================
void C_elm_database_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	// NOP
}

// ****************************************************************
// エレメント：データベースリスト：サブのロード
// ================================================================
void C_elm_database_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	// NOP
}
