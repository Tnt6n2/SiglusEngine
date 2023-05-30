#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_cmd_line.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_dir.h"
#include	"engine/eng_debug.h"
#include	"engine/ifc_error.h"

#include	<Wincrypt.h>

// ****************************************************************
// デバッグ dll：dll 名
// ================================================================

#if 0	// デバッグ dll を切り替えない場合

const TCHAR*	DEBUG_DLL = _T("SiglusDebugger3.dll");

#else	// デバッグ dll を切り替える場合

#ifdef _DEBUG	// デバッグ
	const TCHAR*	DEBUG_DLL = _T("SiglusDebugger3_d.dll");
#else
#ifdef _TEST	// テスト
	const TCHAR*	DEBUG_DLL = _T("SiglusDebugger3_t.dll");
#else			// リリース
	const TCHAR*	DEBUG_DLL = _T("SiglusDebugger3.dll");
#endif
#endif

#endif

// ****************************************************************
// デバッグ dll：Crypt
// ================================================================
bool my_crypt(const char* p_password, const char* p_key, int src_size, BYTE* p_dst, int dst_size)
{
	// dst に key をコピーする
	memcpy(p_dst, p_key, src_size);

	TCHAR* container = _T("visualarts_siglus_debug_dll_container");

	bool ret = false;
	HCRYPTPROV h_prov = NULL;
	if (!CryptAcquireContext(&h_prov, container, NULL, PROV_RSA_FULL, 0))	{
		if (GetLastError() != NTE_BAD_KEYSET)	{
			set_last_error_win(_T("CryptAcquireContext"));
		}
		else	{
			if (!CryptAcquireContext(&h_prov, container, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))	{
				set_last_error_win(_T("CryptAcquireContext"));
			}
		}
	}

	if (h_prov)	{
		HCRYPTHASH h_hash = NULL;
		if (!CryptCreateHash(h_prov, CALG_SHA, 0, 0, &h_hash))	{
			set_last_error_win(_T("CryptCreateHash"));
		}
		else	{
			if (!CryptHashData(h_hash, (BYTE *)p_password, strlen(p_password), 0))	{
				set_last_error_win(_T("CryptHashData"));
			}
			else	{
				HCRYPTKEY h_key = NULL;
				if (!CryptDeriveKey(h_prov, CALG_RC2, h_hash, 0, &h_key))	{
					set_last_error_win(_T("CryptDeriveKey"));
				}
				else	{
					DWORD read_cnt = 0;
					if (!CryptEncrypt(h_key, 0, TRUE, 0, p_dst, (DWORD *)&src_size, dst_size))	{
						set_last_error_win(_T("CryptEncrypt"));
					}
					else	{

						ret = true;
					}
					CryptDestroyKey(h_key);
				}
			}
			CryptDestroyHash(h_hash);
		}
		CryptReleaseContext(h_prov, 0);
	}

	return ret;
}

// ****************************************************************
// デバッグ dll：コンストラクタ
// ================================================================
C_tnm_debug_dll::C_tnm_debug_dll()
{
	h_dll = NULL;
	get_major_version = NULL;
	get_minor_version = NULL;

	lb_add_item_001 = NULL;
	lb_insert_item_001 = NULL;

	lv_add_item_001 = NULL;
	lv_add_item_002 = NULL;
	lv_add_item_visible_001 = NULL;
	lv_insert_item_001 = NULL;
	lv_insert_item_002 = NULL;
	lv_insert_item_visible_001 = NULL;
	lv_reduce_item_001 = NULL;
	lv_set_item_cnt_001 = NULL;
	lv_set_cell_text_001 = NULL;
	lv_set_cell_text_visible_001 = NULL;
}

// ****************************************************************
// デバッグ dll：読み込み
// ================================================================
BYTE G_debug_dll_cr_data[] = {0x75,0x42,0x9e,0x48,0xee,0x23,0x29,0xd8,0xae,0xa5,0x76,0x9c,0x1b,0x97,0x85,0x11,0x31,0x21,0xb4,0x91,0x54,0x34,0x53,0x52,0xf0,0x43,0xa0,0x3e,0xe6,0x30,0x21,0x1a,0x7f,0x6b,0x6d,0x48,0x7b,0x9b,0x8a,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void C_tnm_debug_dll::load()
{
	char password[64] = {0};
	char key[64] = {0};
	BYTE cr_data[64] = {0};

	// デバッグオフが設定されている場合はチェックしない
	if (Gp_cmd_line->debug_off_set)
		return;

	// dll を探す
	h_dll = ::LoadLibrary(DEBUG_DLL);	// まずは system32 を検索
	if (!h_dll)	{
		TSTR dll_path = Gp_dir->work_dir + _T("\\") + DEBUG_DLL;
		h_dll = ::LoadLibrary(dll_path.c_str());		// 次に作業フォルダを検索
		if (!h_dll)	{
			// あえてエラーを出さない（ユーザに気づかれないようにするため）
			//info_box(dll_path + _T(" が読み込めませんでした。（ファイルはありました）\n\n詳細なエラー = ") + get_last_error_message());
		}
	}
	if (h_dll)	{
		get_major_version = (T_get_major_version)::GetProcAddress(h_dll, "get_major_version");
		get_minor_version = (T_get_minor_version)::GetProcAddress(h_dll, "get_minor_version");
		lb_add_item_001 = (T_lb_add_item_001)::GetProcAddress(h_dll, "lb_add_item_001");
		lb_insert_item_001 = (T_lb_insert_item_001)::GetProcAddress(h_dll, "lb_insert_item_001");
		lv_add_item_001 = (T_lv_add_item_001)::GetProcAddress(h_dll, "lv_add_item_001");
		lv_add_item_002 = (T_lv_add_item_002)::GetProcAddress(h_dll, "lv_add_item_002");
		lv_add_item_visible_001 = (T_lv_add_item_visible_001)::GetProcAddress(h_dll, "lv_add_item_visible_001");
		lv_insert_item_001 = (T_lv_insert_item_001)::GetProcAddress(h_dll, "lv_insert_item_001");
		lv_insert_item_002 = (T_lv_insert_item_002)::GetProcAddress(h_dll, "lv_insert_item_002");
		lv_insert_item_visible_001 = (T_lv_insert_item_visible_001)::GetProcAddress(h_dll, "lv_insert_item_visible_001");
		lv_reduce_item_001 = (T_lv_reduce_item_001)::GetProcAddress(h_dll, "lv_reduce_item_001");
		lv_set_item_cnt_001 = (T_lv_set_item_cnt_001)::GetProcAddress(h_dll, "lv_set_item_cnt_001");
		lv_set_cell_text_001 = (T_lv_set_cell_text_001)::GetProcAddress(h_dll, "lv_set_cell_text_001");
		lv_set_cell_text_visible_001 = (T_lv_set_cell_text_visible_001)::GetProcAddress(h_dll, "lv_set_cell_text_visible_001");

		if (!get_major_version || !get_minor_version
			|| !lb_add_item_001 || !lb_insert_item_001
			|| !lv_add_item_001 || !lv_add_item_002 || !lv_add_item_visible_001
			|| !lv_insert_item_001 || !lv_insert_item_002 || !lv_insert_item_visible_001
			|| !lv_reduce_item_001 || !lv_set_item_cnt_001
			|| !lv_set_cell_text_001 || !lv_set_cell_text_visible_001
			)	{
			tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("debug.dll 内に必要な関数が見つかりませんでした。"));
			free();		// dll を解放する
		}
		else	{

			int major_version = get_major_version(password);
			int minor_version = get_minor_version(key);
			if (!my_crypt(password, key, 32, cr_data, 64))	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("debug.dll の処理に失敗しました。\n") + get_last_error_msg());
				free();		// dll を解放する
			}
			else if (major_version == 3 && memcmp(G_debug_dll_cr_data, cr_data, 64) == 0)	{
				Gp_global->debug_flag = true;		// dll が見つかったので自動でデバッグモードに入る
				tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("debug.dll を読み込みました。"));
			}
			else	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("debug.dll のバージョンが不正です。"));
				free();		// dll を解放する
			}
		}
	}

	// デバッグオンが設定されている場合はパスワードを判定
	if (!Gp_global->debug_flag && Gp_cmd_line->debug_on_set)	{
		memcpy(password, TSTR_to_MBSTR(Gp_cmd_line->debug_password).c_str(), 32);
		memcpy(key, TSTR_to_MBSTR(Gp_cmd_line->debug_key).c_str(), 32);
		my_crypt(password, key, 32, cr_data, 64);
		if (memcmp(G_debug_dll_cr_data, cr_data, 64) == 0)	{
			Gp_global->debug_flag = true;		// パスが一致したのでデバッグモードに入る
		}
	}
}

// ****************************************************************
// デバッグ dll：解放
// ================================================================
void C_tnm_debug_dll::free()
{
	if (h_dll)	{
		::FreeLibrary(h_dll);
		h_dll = NULL;
		get_major_version = NULL;
		get_minor_version = NULL;

		lb_add_item_001 = NULL;
		lb_insert_item_001 = NULL;

		lv_add_item_001 = NULL;
		lv_add_item_002 = NULL;
		lv_add_item_visible_001 = NULL;
		lv_insert_item_001 = NULL;
		lv_insert_item_002 = NULL;
		lv_insert_item_visible_001 = NULL;
		lv_reduce_item_001 = NULL;
		lv_set_item_cnt_001 = NULL;
		lv_set_cell_text_001 = NULL;
		lv_set_cell_text_visible_001 = NULL;
	}
}
