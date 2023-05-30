#include	"pch.h"
#include	"lzss.h"

#include	"ifc_eng.h"

#include	"data/tnm_dir.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_lexer.h"

// ****************************************************************
// 簡易暗号コード
// ================================================================
#include	"tnm_exe_angou.h"
BYTE tnm_easy_angou_code[TNM_EASY_ANGOU_CODE_SIZE] = TNM_EASY_ANGOU_CODE;

// ****************************************************************
// ＥＸＥセット暗号コード
// ================================================================
volatile extern BYTE tnm_exe_angou_find_code00[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ★ＥＸＥセット暗号コード
volatile extern BYTE tnm_exe_angou_find_code01[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ★ＥＸＥセット暗号コード
volatile extern BYTE tnm_exe_angou_find_code02[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ★ＥＸＥセット暗号コード
volatile extern BYTE tnm_exe_angou_find_code03[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ★ＥＸＥセット暗号コード
volatile extern BYTE tnm_exe_angou_find_code04[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ★ＥＸＥセット暗号コード
volatile extern BYTE tnm_exe_angou_find_code05[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ★ＥＸＥセット暗号コード
volatile extern BYTE tnm_exe_angou_find_code06[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ★ＥＸＥセット暗号コード
volatile extern BYTE tnm_exe_angou_find_code07[TNM_EXE_ANGOU_FIND_CODE_SIZE];		// ★ＥＸＥセット暗号コード

// ****************************************************************
// スクリプトレクサー
// ================================================================
C_tnm_scene_lexer::C_tnm_scene_lexer()
{
	m_cur_ptr = NULL;
	m_cur_scn_no = 0;
	m_cur_line_no = 0;
}

// ****************************************************************
// レクサー：初期化
// ================================================================
void C_tnm_scene_lexer::init()
{
	m_scn_data.clear();
	
	m_pack_scn_info.p_data_top = NULL;
	m_pack_scn_info.p_header = NULL;
	m_pack_scn_info.p_inc_prop_list = NULL;
	m_pack_scn_info.p_inc_prop_name_index_list = NULL;
	m_pack_scn_info.p_inc_prop_name_list = NULL;
	m_pack_scn_info.p_inc_cmd_name_index_list = NULL;
	m_pack_scn_info.p_inc_cmd_name_list = NULL;
	m_pack_scn_info.p_inc_cmd_list = NULL;
	m_pack_scn_info.p_scn_name_index_list = NULL;
	m_pack_scn_info.p_scn_name_list = NULL;
	m_pack_scn_info.p_scn_data_index_list = NULL;
	m_pack_scn_info.p_scn_data_list = NULL;
	m_pack_scn_info.scn_no_map.clear();
	m_pack_scn_info.inc_prop_no_map.clear();
	m_pack_scn_info.inc_cmd_no_map.clear();

	m_scn_info_list.clear();

	m_cur_ptr = NULL;
	m_cur_scn_no = 0;
	m_cur_line_no = 0;
}

// ****************************************************************
// レクサー：シーンを読み込み
// ================================================================
bool C_tnm_scene_lexer::load_scn(CTSTR& scn_pack_name)
{
	m_scn_data.clear();

	// シーンパス
	TSTR scene_path = Gp_dir->exe_dir + _T("\\") + Gp_dir->append_dir + _T("\\") + scn_pack_name;

	// ファイルの読み込み
	BUFFER tmp;
	if (!C_file::read_full_data(scene_path, tmp))
		return false;

	S_tnm_pack_scn_header *tmp_p_header = (S_tnm_pack_scn_header *)tmp.get();

	// ＥＸＥセット暗号のコードを取得
	BYTE exe_angou_element[TNM_EXE_ANGOU_ELEMENT_CNT];
	if (tmp_p_header->scn_data_exe_angou_mod)
	{
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT00A] = tnm_exe_angou_find_code00[TNM_EXE_ANGOU_OFFSET00A];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT01A] = tnm_exe_angou_find_code01[TNM_EXE_ANGOU_OFFSET01A];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT02A] = tnm_exe_angou_find_code02[TNM_EXE_ANGOU_OFFSET02A];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT03A] = tnm_exe_angou_find_code03[TNM_EXE_ANGOU_OFFSET03A];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT04A] = tnm_exe_angou_find_code04[TNM_EXE_ANGOU_OFFSET04A];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT05A] = tnm_exe_angou_find_code05[TNM_EXE_ANGOU_OFFSET05A];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT06A] = tnm_exe_angou_find_code06[TNM_EXE_ANGOU_OFFSET06A];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT07A] = tnm_exe_angou_find_code07[TNM_EXE_ANGOU_OFFSET07A];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT00B] = tnm_exe_angou_find_code00[TNM_EXE_ANGOU_OFFSET00B];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT01B] = tnm_exe_angou_find_code01[TNM_EXE_ANGOU_OFFSET01B];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT02B] = tnm_exe_angou_find_code02[TNM_EXE_ANGOU_OFFSET02B];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT03B] = tnm_exe_angou_find_code03[TNM_EXE_ANGOU_OFFSET03B];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT04B] = tnm_exe_angou_find_code04[TNM_EXE_ANGOU_OFFSET04B];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT05B] = tnm_exe_angou_find_code05[TNM_EXE_ANGOU_OFFSET05B];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT06B] = tnm_exe_angou_find_code06[TNM_EXE_ANGOU_OFFSET06B];
		exe_angou_element[TNM_EXE_ANGOU_ELEMENT07B] = tnm_exe_angou_find_code07[TNM_EXE_ANGOU_OFFSET07B];
	}

	// シーンデータの再構築（圧縮暗号化されたシーンデータを元の状態に戻す）
	m_scn_data.resize(tmp_p_header->scn_data_list_ofs);
	CopyMemory(m_scn_data.get(), tmp.get(), tmp_p_header->scn_data_list_ofs);
	C_index *tmp_p_scn_data_index_list = (C_index *)(tmp.get() + tmp_p_header->scn_data_index_list_ofs);
	C_index *scn_data_p_scn_data_index_list;
	int offset = tmp_p_scn_data_index_list[0].offset;
	int size, eac;
	BYTE *sp, *dp;
	for (int scn_no = 0; scn_no < tmp_p_header->scn_data_cnt; scn_no++)	{

		if (tmp_p_scn_data_index_list[scn_no].size > 0)	{

			// 圧縮データへのポインタ
			sp = tmp.get() + tmp_p_header->scn_data_list_ofs + tmp_p_scn_data_index_list[scn_no].offset;
			size = tmp_p_scn_data_index_list[scn_no].size;

			// 簡易リンクでない場合はシーンデータを解凍
			if (tmp_p_header->original_source_header_size > 0)	{

				// ＥＸＥセット暗号解除
				if (tmp_p_header->scn_data_exe_angou_mod)	{
					dp = sp;
					eac = 0;
					for (int i = 0; i < size; i++)	{
						*dp++ ^= exe_angou_element[eac++];
						eac %= TNM_EXE_ANGOU_ELEMENT_CNT;
					}
				}

				// 簡易暗号解除
				dp = sp;
				eac = 0;
				for (int i = 0; i < size; i++)	{
					*dp++ ^= tnm_easy_angou_code[eac++];
					eac %= TNM_EASY_ANGOU_CODE_SIZE;
				}

				// 解凍
				size = lzss_unpack(sp, NULL);
				m_scn_data.resize((int)m_scn_data.size() + size);				// バッファ確保
				dp = m_scn_data.get() + tmp_p_header->scn_data_list_ofs + offset;
				lzss_unpack(sp, dp);
			}
			else	{

				// 簡易リンクの場合はそのままコピー
				m_scn_data.resize((int)m_scn_data.size() + tmp_p_scn_data_index_list[scn_no].size);
				sp = tmp.get() + tmp_p_header->scn_data_list_ofs + tmp_p_scn_data_index_list[scn_no].offset;
				dp = m_scn_data.get() + tmp_p_header->scn_data_list_ofs + offset;
				memcpy(dp, sp, tmp_p_scn_data_index_list[scn_no].size);
			}
		}
		else	{
			size = 0;
		}

		scn_data_p_scn_data_index_list = (C_index *)(m_scn_data.get() + tmp_p_header->scn_data_index_list_ofs);
		scn_data_p_scn_data_index_list[scn_no].offset = offset;		// 新しいシーンデータのオフセット
		scn_data_p_scn_data_index_list[scn_no].size = size;			// 新しいシーンデータのサイズ
		offset += size;
	}

	// パックシーンデータの解析
	BYTE* p_pack_data_top = m_scn_data.get();
	m_pack_scn_info.p_data_top = m_scn_data.get();
	m_pack_scn_info.p_header = (S_tnm_pack_scn_header *)p_pack_data_top;																// ヘッダ
	m_pack_scn_info.p_inc_prop_list = (S_tnm_pack_scn_inc_prop *)(p_pack_data_top + m_pack_scn_info.p_header->inc_prop_list_ofs);		// インクプロパティ
	m_pack_scn_info.p_inc_prop_name_index_list = (C_index *)(p_pack_data_top + m_pack_scn_info.p_header->inc_prop_name_index_list_ofs);	// インクプロパティ名
	m_pack_scn_info.p_inc_prop_name_list = (TCHAR *)(p_pack_data_top + m_pack_scn_info.p_header->inc_prop_name_list_ofs);				// インクプロパティ名
	m_pack_scn_info.p_inc_cmd_list = (S_tnm_pack_scn_inc_cmd *)(p_pack_data_top + m_pack_scn_info.p_header->inc_cmd_list_ofs);			// インクコマンド
	m_pack_scn_info.p_inc_cmd_name_index_list = (C_index *)(p_pack_data_top + m_pack_scn_info.p_header->inc_cmd_name_index_list_ofs);	// インクコマンド名トップ
	m_pack_scn_info.p_inc_cmd_name_list = (TCHAR *)(p_pack_data_top + m_pack_scn_info.p_header->inc_cmd_name_list_ofs);					// インクコマンド名
	m_pack_scn_info.p_scn_name_index_list = (C_index *)(p_pack_data_top + m_pack_scn_info.p_header->scn_name_index_list_ofs);			// シーン名トップ
	m_pack_scn_info.p_scn_name_list = (TCHAR *)(p_pack_data_top + m_pack_scn_info.p_header->scn_name_list_ofs);							// シーン名
	m_pack_scn_info.p_scn_data_index_list = (C_index *)(p_pack_data_top + m_pack_scn_info.p_header->scn_data_index_list_ofs);			// シーンデータトップ
	m_pack_scn_info.p_scn_data_list = (BYTE *)(p_pack_data_top + m_pack_scn_info.p_header->scn_data_list_ofs);							// シーンデータ


	// シーン名 ⇒ シーン番号のマップを作成
	for (int i = 0; i < m_pack_scn_info.p_header->scn_name_cnt; i++)	{
		C_index* index = &m_pack_scn_info.p_scn_name_index_list[i];
		TCHAR* begin = &m_pack_scn_info.p_scn_name_list[index->offset];
		TCHAR* end = &m_pack_scn_info.p_scn_name_list[index->offset + index->size];
		m_pack_scn_info.scn_no_map.insert(std::pair<TSTR, int>(TSTR(begin, end), i));
	}

	// インクプロパティ名 ⇒ インクプロパティ番号のマップを作成
	for (int i = 0; i < m_pack_scn_info.p_header->inc_prop_name_cnt; i++)	{
		C_index* index = &m_pack_scn_info.p_inc_prop_name_index_list[i];
		TCHAR* begin = &m_pack_scn_info.p_inc_prop_name_list[index->offset];
		TCHAR* end = &m_pack_scn_info.p_inc_prop_name_list[index->offset + index->size];
		m_pack_scn_info.inc_prop_no_map.insert(std::pair<TSTR, int>(TSTR(begin, end), i));
	}

	// インクコマンド名 ⇒ インクコマンド番号のマップを作成
	for (int i = 0; i < m_pack_scn_info.p_header->inc_cmd_name_cnt; i++)	{
		C_index* index = &m_pack_scn_info.p_inc_cmd_name_index_list[i];
		TCHAR* begin = &m_pack_scn_info.p_inc_cmd_name_list[index->offset];
		TCHAR* end = &m_pack_scn_info.p_inc_cmd_name_list[index->offset + index->size];
		m_pack_scn_info.inc_cmd_no_map.insert(std::pair<TSTR, int>(TSTR(begin, end), i));
	}

	// シーンデータインデックスから、各シーンの先頭のポインタを求める
	m_scn_info_list.resize(m_pack_scn_info.p_header->scn_data_cnt);
	for (int scn_no = 0; scn_no < m_pack_scn_info.p_header->scn_data_cnt; scn_no++)	{
		BYTE* p_scn_data_top = m_pack_scn_info.p_scn_data_list + m_pack_scn_info.p_scn_data_index_list[scn_no].offset;

		// 各シーンを解析
		S_tnm_scn_header* p_scn_header						= (S_tnm_scn_header *)p_scn_data_top;
		m_scn_info_list[scn_no].p_header					= (S_tnm_scn_header *)p_scn_data_top;
		m_scn_info_list[scn_no].p_data_top					= p_scn_data_top;																// トップ
		m_scn_info_list[scn_no].p_scn						= p_scn_data_top + p_scn_header->scn_ofs;										// シーン
		m_scn_info_list[scn_no].p_str_index_list			= (C_index *)(p_scn_data_top + p_scn_header->str_index_list_ofs);				// 文字列リストトップ
		m_scn_info_list[scn_no].p_str_list					= (TCHAR *)(p_scn_data_top + p_scn_header->str_list_ofs);						// 文字列リスト
		m_scn_info_list[scn_no].p_label_list				= (int *)(p_scn_data_top + p_scn_header->label_list_ofs);						// ラベルリスト
		m_scn_info_list[scn_no].p_z_label_list				= (int *)(p_scn_data_top + p_scn_header->z_label_list_ofs);						// Ｚラベルリスト
		m_scn_info_list[scn_no].p_scn_prop_list				= (S_tnm_scn_scn_prop *)(p_scn_data_top + p_scn_header->scn_prop_list_ofs);		// シーンプロパティリスト
		m_scn_info_list[scn_no].p_scn_prop_name_index_list	= (C_index *)(p_scn_data_top + p_scn_header->scn_prop_name_index_list_ofs);		// シーンプロパティ名リストトップ
		m_scn_info_list[scn_no].p_scn_prop_name_list		= (TCHAR *)(p_scn_data_top + p_scn_header->scn_prop_name_list_ofs);				// シーンプロパティ名リスト
		m_scn_info_list[scn_no].p_scn_cmd_list				= (S_tnm_scn_scn_cmd *)(p_scn_data_top + p_scn_header->scn_cmd_list_ofs);		// シーンコマンドリスト
		m_scn_info_list[scn_no].p_scn_cmd_name_index_list	= (C_index *)(p_scn_data_top + p_scn_header->scn_cmd_name_index_list_ofs);		// シーンコマンド名インデックスリスト
		m_scn_info_list[scn_no].p_scn_cmd_name_list			= (TCHAR *)(p_scn_data_top + p_scn_header->scn_cmd_name_list_ofs);				// シーンコマンド名リスト
		m_scn_info_list[scn_no].p_call_prop_name_index_list	= (C_index *)(p_scn_data_top + p_scn_header->call_prop_name_index_list_ofs);	// コールプロパティ名リストトップ
		m_scn_info_list[scn_no].p_call_prop_name_list		= (TCHAR *)(p_scn_data_top + p_scn_header->call_prop_name_list_ofs);			// コールプロパティ名リスト
		m_scn_info_list[scn_no].p_namae_list				= (int *)(p_scn_data_top + p_scn_header->namae_list_ofs);						// 【名前】リスト
		m_scn_info_list[scn_no].p_read_flag_list			= (S_tnm_scn_read_flag *)(p_scn_data_top + p_scn_header->read_flag_list_ofs);	// 既読フラグリスト

		// シーンプロパティ名 ⇒ シーンプロパティ番号のマップを作成
		for (int i = 0; i < m_scn_info_list[scn_no].p_header->scn_prop_name_cnt; i++)	{
			C_index* index = &m_scn_info_list[scn_no].p_scn_prop_name_index_list[i];
			TCHAR* begin = &m_scn_info_list[scn_no].p_scn_prop_name_list[index->offset];
			TCHAR* end = &m_scn_info_list[scn_no].p_scn_prop_name_list[index->offset + index->size];
			m_scn_info_list[scn_no].scn_prop_no_map.insert(std::pair<TSTR, int>(TSTR(begin, end), i));
		}

		// シーンコマンド名 ⇒ シーンコマンド番号のマップを作成
		for (int i = 0; i < m_scn_info_list[scn_no].p_header->scn_cmd_name_cnt; i++)	{
			C_index* index = &m_scn_info_list[scn_no].p_scn_cmd_name_index_list[i];
			TCHAR*  begin = &m_scn_info_list[scn_no].p_scn_cmd_name_list[index->offset];
			TCHAR*  end = &m_scn_info_list[scn_no].p_scn_cmd_name_list[index->offset + index->size];
			m_scn_info_list[scn_no].scn_cmd_no_map.insert(std::pair<TSTR, int>(TSTR(begin, end), i));
		}
	}

	return true;
}

// ****************************************************************
// オリジナルソース判定
// ================================================================
bool C_tnm_scene_lexer::is_exist_org_source()
{
	if (m_pack_scn_info.p_header)	{
		if (m_pack_scn_info.p_header->original_source_header_size > 0)	{
			return true;
		}
	}

	return false;
}

// ****************************************************************
// ユーザプロパティ名からユーザプロパティ番号を取得
// ================================================================
int C_tnm_scene_lexer::get_user_prop_no(CTSTR& user_prop_name)
{
	return get_user_prop_no(m_cur_scn_no, user_prop_name);
}

int C_tnm_scene_lexer::get_user_prop_no(int scn_no, CTSTR& user_prop_name)
{
	if (scn_no < 0)
		return -1;

	// ファイル名は小文字で格納されていることに注意！

	// インクコマンドを探す
	std::map<TSTR, int>::iterator itr_inc_prop = m_pack_scn_info.inc_prop_no_map.find(str_to_lower_ret(user_prop_name));
	if (itr_inc_prop != m_pack_scn_info.inc_prop_no_map.end())
		return itr_inc_prop->second;

	// シーンコマンドを探す
	std::map<TSTR, int>::iterator itr_scn_prop = m_scn_info_list[scn_no].scn_prop_no_map.find(str_to_lower_ret(user_prop_name));
	if (itr_scn_prop != m_scn_info_list[scn_no].scn_prop_no_map.end())
		return itr_scn_prop->second + m_pack_scn_info.p_header->inc_prop_cnt;	// シーンプロパティID → ユーザプロパティID

	return -1;
}

// ****************************************************************
// ユーザコマンド名からユーザコマンド番号を取得
// ================================================================
int C_tnm_scene_lexer::get_user_cmd_no(int scn_no, CTSTR& user_cmd_name)
{
	if (scn_no < 0)
		return -1;

	// ファイル名は小文字で格納されていることに注意！

	// インクコマンドを探す
	std::map<TSTR, int>::iterator itr_inc_cmd = m_pack_scn_info.inc_cmd_no_map.find(str_to_lower_ret(user_cmd_name));
	if (itr_inc_cmd != m_pack_scn_info.inc_cmd_no_map.end())
		return itr_inc_cmd->second;

	// シーンコマンドを探す
	std::map<TSTR, int>::iterator itr_scn_cmd = m_scn_info_list[scn_no].scn_cmd_no_map.find(str_to_lower_ret(user_cmd_name));
	if (itr_scn_cmd != m_scn_info_list[scn_no].scn_cmd_no_map.end())
		return itr_scn_cmd->second + m_pack_scn_info.p_header->inc_cmd_cnt;	// シーンコマンドID → ユーザコマンドID

	return -1;
}

// ****************************************************************
// ユーザコマンド番号からユーザコマンド名を取得
// ================================================================
TSTR C_tnm_scene_lexer::get_user_cmd_name(int scn_no, int user_cmd_no)
{
	// インクコマンドの場合
	if (user_cmd_no < m_pack_scn_info.p_header->inc_cmd_cnt)	{
		
		C_index* index = &m_pack_scn_info.p_inc_cmd_name_index_list[user_cmd_no];
		TCHAR*  start = &m_pack_scn_info.p_inc_cmd_name_list[index->offset];
		TCHAR*  end = &m_pack_scn_info.p_inc_cmd_name_list[index->offset + index->size];
		return TSTR(start, end);
	}

	// シーンコマンドの場合
	else	{

		// ユーザコマンド番号 → シーンコマンド番号
		int scn_cmd_no = user_cmd_no - m_pack_scn_info.p_header->inc_cmd_cnt;

		C_index* index = &m_scn_info_list[scn_no].p_scn_cmd_name_index_list[scn_cmd_no];
		TCHAR*  start = &m_scn_info_list[scn_no].p_scn_cmd_name_list[index->offset];
		TCHAR*  end = &m_scn_info_list[scn_no].p_scn_cmd_name_list[index->offset + index->size];
		return TSTR(start, end);
	}
}

// ****************************************************************
// シーン名からシーン番号を取得
// ================================================================
int C_tnm_scene_lexer::get_scn_no(CTSTR& scn_name)
{
	// ファイル名は小文字で格納されていることに注意！

	// シーンを探す
	std::map<TSTR, int>::iterator itr = m_pack_scn_info.scn_no_map.find(str_to_lower_ret(scn_name));
	if (itr == m_pack_scn_info.scn_no_map.end())
		return -1;

	return itr->second;
}

// ****************************************************************
// シーン番号からシーン名を取得
// ================================================================
TSTR C_tnm_scene_lexer::get_scn_name(int scn_no)
{
	C_index* index = &m_pack_scn_info.p_scn_name_index_list[scn_no];
	TCHAR* begin = &m_pack_scn_info.p_scn_name_list[index->offset];
	TCHAR* end = &m_pack_scn_info.p_scn_name_list[index->offset + index->size];

	return TSTR(begin, end);
}

// ****************************************************************
// シーン存在判定
// ================================================================
bool C_tnm_scene_lexer::is_exist_scn(CTSTR& scn_name)
{
	// ファイル名は小文字で格納されている！
	return m_pack_scn_info.scn_no_map.find(str_to_lower_ret(scn_name)) != m_pack_scn_info.scn_no_map.end();
}

bool C_tnm_scene_lexer::is_exist_z(int scn_no, int z_no)
{
	// Ｚ番号の範囲を判定
	if (z_no < 0 || TNM_Z_LABEL_CNT <= z_no)
		return false;

	// Ｚラベルの検索
	return m_scn_info_list[scn_no].p_z_label_list[z_no] > 0;
}

// ****************************************************************
// 【名前】の個数を取得
// ================================================================
int C_tnm_scene_lexer::get_namae_cnt(int scn_no)
{
	return m_scn_info_list[scn_no].p_header->namae_cnt;
}

// ****************************************************************
// 【名前】の文字列を取得
// ================================================================
TSTR C_tnm_scene_lexer::get_namae_str(int scn_no, int namae_no)
{
	int str_index = m_scn_info_list[scn_no].p_namae_list[namae_no];
	return get_string_with_scn_no(scn_no, str_index);
}

// ****************************************************************
// 既読フラグ数を取得
// ================================================================
int C_tnm_scene_lexer::get_read_flag_cnt(int scn_no)
{
	return m_scn_info_list[scn_no].p_header->read_flag_cnt;
}

// ****************************************************************
// 既読フラグの行番号を取得
// ================================================================
int C_tnm_scene_lexer::get_read_flag_line_no(int scn_no, int rf_no)
{
	return m_scn_info_list[scn_no].p_read_flag_list[rf_no].line_no;
}

// ****************************************************************
// シーンジャンプ
// ================================================================
bool C_tnm_scene_lexer::jump_to_scn(int scn_no, int z_no)
{
	// Ｚ番号の範囲を判定
	if (z_no < 0 || TNM_Z_LABEL_CNT <= z_no)
		return false;

	// 現在のシーン番号を設定
	m_cur_scn_no = scn_no;

	// シーン先頭からのオフセットを取得
	int z_label_offset = m_scn_info_list[m_cur_scn_no].p_z_label_list[z_no];

	// Ｚラベルの位置に移動
	m_cur_ptr = m_scn_info_list[m_cur_scn_no].p_scn + z_label_offset;

	return true;
}

// ****************************************************************
// ラベルにジャンプ
// ================================================================
bool C_tnm_scene_lexer::jump_to_label(int label_no)
{
	// ラベルの範囲を判定
	if (label_no >= m_scn_info_list[m_cur_scn_no].p_header->label_cnt)
		return false;

	// シーン先頭からのオフセットを取得
	int label_offset = m_scn_info_list[m_cur_scn_no].p_label_list[label_no];

	// ラベルの位置に移動
	m_cur_ptr = m_scn_info_list[m_cur_scn_no].p_scn + label_offset;

	return true;
}

// ****************************************************************
// ユーザコマンドにジャンプ
// ================================================================
bool C_tnm_scene_lexer::jump_to_user_cmd(int scn_no, int user_cmd_no)
{
	// インクコマンドの場合
	if (user_cmd_no < m_pack_scn_info.p_header->inc_cmd_cnt)	{

		// コマンドの取得
		S_tnm_pack_scn_inc_cmd* p_user_cmd = &m_pack_scn_info.p_inc_cmd_list[user_cmd_no];

		// コマンドが存在するシーンに移動
		m_cur_scn_no = p_user_cmd->scn_no;

		// コマンドの位置に移動
		m_cur_ptr = m_scn_info_list[m_cur_scn_no].p_scn + p_user_cmd->offset;
	}

	// シーンコマンドの場合
	else	{

		// ユーザコマンド番号 → シーンコマンド番号
		int scn_cmd_no = user_cmd_no - m_pack_scn_info.p_header->inc_cmd_cnt;

		// コマンドの取得
		S_tnm_scn_scn_cmd* p_scn_cmd = &m_scn_info_list[scn_no].p_scn_cmd_list[scn_cmd_no];

		// コマンドが存在するシーンに移動
		m_cur_scn_no = scn_no;

		// コマンドの位置に移動
		m_cur_ptr = m_scn_info_list[scn_no].p_scn + p_scn_cmd->offset;
	}

	return true;
}

// ****************************************************************
// プログラムカウンタを取得
// ================================================================
//		シーンの先頭からのオフセットを計算します。
// ================================================================
int C_tnm_scene_lexer::get_prg_cntr()
{
	return (int)((PTR)m_cur_ptr - (PTR)m_scn_info_list[m_cur_scn_no].p_scn);
}

// ****************************************************************
// プログラムカウンタを設定
// ================================================================
//		シーンの先頭からのオフセットで指定します。
// ================================================================
void C_tnm_scene_lexer::set_prg_cntr(int prg_cntr)
{
	m_cur_ptr = m_scn_info_list[m_cur_scn_no].p_scn + prg_cntr;
}

// ****************************************************************
// 文字列を取得
// ================================================================
//		文字列番号からインデックスを取得し、文字列を返す
// ================================================================
TSTR C_tnm_scene_lexer::get_string_with_scn_no(int scn_no, int str_index)
{
	C_index* index = &m_scn_info_list[scn_no].p_str_index_list[str_index];
	TCHAR* start = &m_scn_info_list[scn_no].p_str_list[index->offset];
	TCHAR* end = &m_scn_info_list[scn_no].p_str_list[index->offset + index->size];

	boost::scoped_array<TCHAR> buf(new TCHAR[index->size]);
	for (int i = 0; i < index->size; i++)	{
		buf[i] = start[i] ^ (28807 * str_index);
	}

	return TSTR(buf.get(), buf.get() + index->size);
}

TSTR C_tnm_scene_lexer::get_string(int str_index)
{
	C_index* index = &m_scn_info_list[m_cur_scn_no].p_str_index_list[str_index];
	TCHAR* start = &m_scn_info_list[m_cur_scn_no].p_str_list[index->offset];
	TCHAR* end = &m_scn_info_list[m_cur_scn_no].p_str_list[index->offset + index->size];

	boost::scoped_array<TCHAR> buf(new TCHAR[index->size]);
	for (int i = 0; i < index->size; i++)	{
		buf[i] = start[i] ^ (28807 * str_index);
	}

	return TSTR(buf.get(), buf.get() + index->size);
}

// ****************************************************************
// ユーザインクプロパティ名を取得
// ================================================================
//		ユーザインクプロパティの名前を取得する
// ================================================================
TSTR C_tnm_scene_lexer::get_user_inc_prop_name(int prop_id)
{
	C_index* index = &m_pack_scn_info.p_inc_prop_name_index_list[prop_id];
	TCHAR* start = &m_pack_scn_info.p_inc_prop_name_list[index->offset];
	TCHAR* end = &m_pack_scn_info.p_inc_prop_name_list[index->offset + index->size];

	return TSTR(start, end);
}

// ****************************************************************
// ユーザシーンプロパティ名を取得
// ================================================================
//		ユーザシーンプロパティの名前を取得する
// ================================================================
TSTR C_tnm_scene_lexer::get_user_scn_prop_name(int scn_no, int prop_id)
{
	C_index* index = &m_scn_info_list[scn_no].p_scn_prop_name_index_list[prop_id];
	TCHAR* start = &m_scn_info_list[scn_no].p_scn_prop_name_list[index->offset];
	TCHAR* end = &m_scn_info_list[scn_no].p_scn_prop_name_list[index->offset + index->size];

	return TSTR(start, end);
}

// ****************************************************************
// ユーザコールプロパティ名を取得
// ================================================================
//		ユーザコールプロパティの名前を取得する
// ================================================================
TSTR C_tnm_scene_lexer::get_user_call_prop_name(int scn_no, int prop_id)
{
	C_index* index = &m_scn_info_list[scn_no].p_call_prop_name_index_list[prop_id];
	TCHAR* start = &m_scn_info_list[scn_no].p_call_prop_name_list[index->offset];
	TCHAR* end = &m_scn_info_list[scn_no].p_call_prop_name_list[index->offset + index->size];

	return TSTR(start, end);
}

// ****************************************************************
// ロード（ローカル）
// ================================================================
bool C_tnm_scene_lexer::load_local(int scn_no, int line_no, int prg_cntr)
{
	m_cur_scn_no = scn_no;
	m_cur_line_no = line_no;

	// プログラムカウンタを設定する
	set_prg_cntr(prg_cntr);

	return true;
}
