#include	"pch.h"
#include	"tnm_common_header.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_thumb_table.h"
#include	"engine/eng_dir.h"
#include	"engine/eng_graphics.h"
#include	"engine/ifc_error.h"

#include	<lzss_unpack.h>

// '\0' つき文字列を読み込む
TCHAR* get_str_null(TCHAR* p, TSTR& str)
{
	TCHAR* p_begin = p;
	for (; *p != _T('\0'); p++);

	str = TSTR(p_begin, p);
	return p + 1;	// '\0' の次へ
}

// ****************************************************************
// サムネイルテーブル：テーブルを構築
// ================================================================
bool C_tnm_thumb_table::create_table()
{
	// ファイル名を取得、空なら何もしない
	TSTR file_name = Gp_ini->thumb_table_file;
	if (file_name.empty())
		return true;

	// ファイルを読み込む
	C_file file;
	ARRAY<BYTE> file_data;
	change_extension(file_name, _T("dat"));
	TSTR file_path = tnm_find_dat(file_name);
	if (!file.read_full_data(file_path, file_data))	{
		tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, _T("サムネイルテーブル ") + file_name + _T(" が見つかりませんでした。"));
		return false;
	}

	// バッファへのポインタ
	BYTE* p = file_data.get();

	// ヘッダ
	S_tnm_thumbnail_database_header* p_header = (S_tnm_thumbnail_database_header *)p;
	p += sizeof(S_tnm_thumbnail_database_header);

	// データ部分を解凍する
	int unpack_size = lzss_unpack(p, NULL);
	ARRAY<BYTE> unpack_data(unpack_size);
	lzss_unpack(p, unpack_data.get());
	p = unpack_data.get();

	// データを読み込む
	TSTR pct, thumb;
	for (int i = 0; i < p_header->data_cnt; i++)	{
		p = (BYTE *)get_str_null((TCHAR *)p, pct);
		p = (BYTE *)get_str_null((TCHAR *)p, thumb);
		str_to_lower(pct);		// ★小文字に変換
		str_to_lower(thumb);	// ★小文字に変換
		m_thumb_table.insert(std::pair<TSTR, TSTR>(pct, thumb));
	}

	return true;
}

// ****************************************************************
// サムネイルテーブル：表示するべきサムネイルを取得する
// ================================================================
TSTR C_tnm_thumb_table::calc_thumb_file_name()
{
	if (m_thumb_table.empty())
		return _T("");

	// リソース状態を取得する
	const S_tnm_d3d_resource_data& rd = G_rsm.pd();

	// 登録されているＣＧをリソースから検索する
	int rsc_cnt = (int)rd.resource_list.size();
	for (int i = 0; i < rsc_cnt; i++)	{
		// g00 から作られたアルバムである場合
		if (rd.resource_list[i].type == E_tnm_d3d_resource_type_album_from_g00)	{
			// リソースが生きている必要があります！
			if (rd.resource_list[i].album.lock())	{

				// ファイル名を切り出す
				TSTR file_name = get_file_name(rd.resource_list[i].name);
				erase_extension(file_name);
				str_to_lower(file_name);		// ★小文字に変換

				// マップから検索する
				std::map<TSTR, TSTR>::iterator find_itr = m_thumb_table.find(file_name);
				if (find_itr != m_thumb_table.end())	{

					// 見つかった！！
					return find_itr->second;
				}
			}
		}
	}

	return _T("");
}




