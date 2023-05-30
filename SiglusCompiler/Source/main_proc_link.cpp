#include	"pch.h"
#include	"data.h"
#include	"dialog.h"
#include	"main.h"
#include	"BS.h"
#include	"exe_angou.h"

#define		ORIGINAL_SOURCE_LINK		1

#if ORIGINAL_SOURCE_LINK
#include	"source_angou.h"
#endif

// ****************************************************************
// ファイルリストのソート関数
// ================================================================

#if ORIGINAL_SOURCE_LINK

struct ORIGINAL_FN_STRUCT
{
	TSTR original;
	TSTR sort_str;
};

bool fn_list_sort_func(ORIGINAL_FN_STRUCT& lhs, ORIGINAL_FN_STRUCT& rhs)
{
	if (lhs.sort_str < rhs.sort_str)			{	return true;	}
	else if (lhs.sort_str > rhs.sort_str)		{	return false;	}
	return false;
}

void make_fn_list(CTSTR& path, CTSTR& first_name, CTSTR& extension, ARRAY<ORIGINAL_FN_STRUCT>* fn_list)
{
	fn_list->clear();

	// ファイルリストを作成する
	C_file_find ff;
	ff.find(path, first_name, extension, false, true, false);
	int file_cnt = ff.get_file_cnt();
	if (file_cnt <= 0)	{	return;	}

	fn_list->resize(file_cnt);
	ARRAY<ORIGINAL_FN_STRUCT>::iterator itr = fn_list->begin();
	for (int i = 0; i < file_cnt; i++)	{
		itr->original = ff[i].rltv_path;
		itr->sort_str = str_to_lower_ret(itr->original);
		itr++;
	}

	// ファイル名をソートする
	std::sort(fn_list->begin(), fn_list->end(), fn_list_sort_func);
}

#endif

// ****************************************************************
// 処理：リンク
// ================================================================
bool C_tnm_compiler_main::proc_link(S_tnms_ia_data* p_ia_data)
{
	// プロジェクトが選択されていなければ失敗
	if (!check_current_project(true))
		return false;

	int offset = 0;
	int offset_2 = 0;

	C_file_find ff;

	TSTR scn_dir_path = G_data->project[G_data->cur_project_no].scn_path;
	TSTR tmp_dir_path = G_data->project[G_data->cur_project_no].tmp_path;
	TSTR out_dir_path = G_data->project[G_data->cur_project_no].out_path;
	TSTR out_dir_path_noangou = G_data->project[G_data->cur_project_no].out_path_noangou;
	TSTR scene_pck = G_data->project[G_data->cur_project_no].scene_pck;

	// リンク開始
	set_log(_T("- シーンデータを結合します。"));

	// 必要のないオブジェクトファイルを削除
	{
		C_file_find ff;

		// 
		std::set<TSTR> name_set;
		for (int i = 0; i < (int)G_data->scn_list.size(); i++)	{
			name_set.insert(str_to_lower_ret(erase_extension_ret(G_data->scn_list[i].scn_name)));
		}

		// bs
		ff.find(tmp_dir_path + _T("\\bs"), _T("*"), _T("*"), false, true, false);
		for (int i = 0; i < ff.get_file_cnt(); i++)	{
			TSTR file_name = get_file_name(ff[i].rltv_path);
			if (name_set.find(str_to_lower_ret(erase_extension_ret(file_name))) == name_set.end())	{
				filesys_delete_file(tmp_dir_path + _T("\\\\") + file_name);
			}
		}
	}

	// 作るもの
	S_tnm_pack_scn_data pack_scn_data;			// パックシーンデータ

	// 圧縮／ＥＸＥセット暗号
	int lzss_mode = 1;
	int exe_angou_mode = 0;

	// 圧縮モード
	if (G_dlg->ci.chk_easy_link.get_check())	{	// 圧縮しないフラグ
		lzss_mode = 0;
	}

	// ＥＸＥセット暗号を作成
	TSTR exe_angou_str;
	BYTE exe_angou_element[TNM_EXE_ANGOU_ELEMENT_CNT];
	if (G_main->load_exe_angou_dat(exe_angou_str))	{
		if(!exe_angou_str.empty())	{
			MBSTR mb_exe_angou_str = TSTR_to_MBSTR(exe_angou_str);
			if((int)mb_exe_angou_str.size() >= 8)	{
				C_tnms_exe_angou::make_angou_element(exe_angou_str, exe_angou_element);
				exe_angou_mode = 1;
				set_log(_T("- 暗号化します。 ( ") + exe_angou_str + _T(" )"));
			}
		}
	}

	// ================================================================
	// オリジナルソースを作成
	// ----------------------------------------------------------------

#if ORIGINAL_SOURCE_LINK

	ARRAY<ORIGINAL_FN_STRUCT> fn_list;
	ARRAY<TSTR> original_source_name_list;
	TSTR original_source_file_path;
	BUFFER original_source_size_list_data;
	ARRAY<DWORD>	original_source_size_list;	// オリジナルソースリスト
	ARRAY<BUFFER>	original_source_list;		// オリジナルソースリスト

	// 圧縮する場合のみオリジナルソースをつける
	if (lzss_mode)	{

		// Gameexe.ini
		make_fn_list(scn_dir_path, _T("Gameexe"), _T("ini"), &fn_list);
		for (int i = 0; i < (int)fn_list.size(); i++)	{
			original_source_name_list.push_back(fn_list[i].original);
		}

		// 暗号.dat
		make_fn_list(scn_dir_path, _T("暗号"), _T("dat"), &fn_list);
		for (int i = 0; i < (int)fn_list.size(); i++)	{
			original_source_name_list.push_back(fn_list[i].original);
		}

		// inc ファイル
		make_fn_list(scn_dir_path, _T("*"), _T("inc"), &fn_list);
		for (int i = 0; i < (int)fn_list.size(); i++)	{
			original_source_name_list.push_back(fn_list[i].original);
		}

		// ss ファイル
		make_fn_list(scn_dir_path, _T("*"), _T("ss"), &fn_list);
		for (int i = 0; i < (int)fn_list.size(); i++)	{
			original_source_name_list.push_back(fn_list[i].original);
		}

		// オリジナルソースを暗号化する（読み込みもここでやります）
		original_source_size_list.resize(original_source_name_list.size());
		original_source_list.resize(original_source_name_list.size());
		TSTR original_source_angou_file_path;
		FILETIME original_source_update, original_source_angou_update, tmp;
		bool angou_flag;
		BUFFER original_source_buf;
		for (int i = 0; i < (int)original_source_name_list.size(); i++)	{
			angou_flag = true;
			original_source_file_path = scn_dir_path + _T("\\") + original_source_name_list[i];
			original_source_angou_file_path = tmp_dir_path + _T("\\os\\") + original_source_name_list[i];
			if (file_get_type(original_source_angou_file_path) == 1)	{
				file_get_time(original_source_file_path, &tmp, &tmp, &original_source_update);
				file_get_time(original_source_angou_file_path, &tmp, &tmp, &original_source_angou_update);
				if (original_source_update.dwHighDateTime > original_source_angou_update.dwHighDateTime || (original_source_update.dwHighDateTime == original_source_angou_update.dwHighDateTime && original_source_update.dwLowDateTime > original_source_angou_update.dwLowDateTime))	{
					angou_flag = true;
				}
			}
			else	{
				angou_flag = true;
			}
			if (angou_flag)	{
				C_file::read_full_data(original_source_file_path, original_source_buf);
				C_tnms_source_angou::encryption(original_source_buf.get(), (int)original_source_buf.size(), original_source_name_list[i], original_source_list[i]);
				create_dir_from_file_path(original_source_angou_file_path);
				C_file::write_full_data(original_source_angou_file_path, original_source_list[i]);
			}
			else	{
				C_file::read_full_data(original_source_angou_file_path, original_source_list[i]);
			}
			original_source_size_list[i] = (int)original_source_list[i].size();
		}
		C_tnms_source_angou::encryption((BYTE *)original_source_size_list.get(), (int)original_source_size_list.size() * sizeof(DWORD), _T("__DummyName__"), original_source_size_list_data);
	}
#endif

	// ================================================================
	// パックシーンデータを作成
	// ----------------------------------------------------------------

	ARRAY<TSTR> scn_name_list;
	TSTR scn_name;
	TSTR scn_file_path;

	// オブジェクトファイルがあるものをリストに追加する
	for (int s = 0; s < (int)G_data->scn_list.size(); s++)	{
		scn_name = erase_extension_ret(G_data->scn_list[s].scn_name);
		scn_file_path = tmp_dir_path + _T("\\bs\\") + scn_name + _T(".dat");
		if (file_get_type(scn_file_path) == 1)	{
			scn_name_list.push_back(scn_name);
		}
	}

	// シーン名リストを作成
	offset = 0;
	pack_scn_data.scn_name_index_list.resize(scn_name_list.size());
	pack_scn_data.scn_name_list.resize(scn_name_list.size());
	for (int i = 0; i < (int)scn_name_list.size(); i++)	{
		pack_scn_data.scn_name_list[i] = str_to_lower_ret(scn_name_list[i]);
		pack_scn_data.scn_name_index_list[i].offset = offset;
		pack_scn_data.scn_name_index_list[i].size = (int)pack_scn_data.scn_name_list[i].size();
		offset += (int)pack_scn_data.scn_name_list[i].size();
	}

	// 全シーンデータのファイルを読みこむ
	BUFFER scn_data;
	offset = 0;
	pack_scn_data.nolzss_scn_data_index_list.resize(pack_scn_data.scn_name_list.size());
	pack_scn_data.nolzss_scn_data_list.resize(pack_scn_data.scn_name_list.size());
	if (lzss_mode)	{
		offset_2 = 0;
		pack_scn_data.lzss_scn_data_index_list.resize(pack_scn_data.scn_name_list.size());
		pack_scn_data.lzss_scn_data_list.resize(pack_scn_data.scn_name_list.size());
	}

	for (int s = 0; s < (int)pack_scn_data.scn_name_list.size(); s++)	{

		// 圧縮無しのシーンデータを読み込む（リンクに使うため必須です）
		scn_file_path = tmp_dir_path + _T("\\bs\\") + pack_scn_data.scn_name_list[s] + _T(".dat");
		if (!C_file::read_full_data(scn_file_path, scn_data))	{
			set_log(scn_file_path + _T(" が見つかりません。"));
			pack_scn_data.nolzss_scn_data_index_list[s].offset = offset;
			pack_scn_data.nolzss_scn_data_index_list[s].size = 0;
		}
		else	{
			pack_scn_data.nolzss_scn_data_list[s] = scn_data;
			pack_scn_data.nolzss_scn_data_index_list[s].offset = offset;
			pack_scn_data.nolzss_scn_data_index_list[s].size = (int)scn_data.size();
			offset += (int)scn_data.size();
		}

		if (lzss_mode)	{
			scn_file_path = tmp_dir_path + _T("\\bs\\") + pack_scn_data.scn_name_list[s] + _T(".lzss");
			if (!C_file::read_full_data(scn_file_path, scn_data))	{
				set_log(scn_file_path + _T(" が見つかりません。"));
				pack_scn_data.lzss_scn_data_index_list[s].offset = offset_2;
				pack_scn_data.lzss_scn_data_index_list[s].size = 0;
			}
			else	{
				pack_scn_data.lzss_scn_data_list[s] = scn_data;
				pack_scn_data.lzss_scn_data_index_list[s].offset = offset_2;
				pack_scn_data.lzss_scn_data_index_list[s].size = (int)scn_data.size();
				offset_2 += (int)scn_data.size();
			}
		}
	}

	// インクプロパティリストを作成
	offset = 0;
	pack_scn_data.inc_prop_list.resize((int)p_ia_data->property_list.size());
	for (int p = 0; p < (int)p_ia_data->property_list.size(); p++)	{
		pack_scn_data.inc_prop_list[p].form = p_ia_data->property_list[p]->form;
		pack_scn_data.inc_prop_list[p].size = p_ia_data->property_list[p]->size;
	}

	// インクプロパティ名リストを作成
	pack_scn_data.inc_prop_name_index_list.resize((int)p_ia_data->property_list.size());
	pack_scn_data.inc_prop_name_list.resize((int)p_ia_data->property_list.size());
	for (int p = 0; p < (int)p_ia_data->property_list.size(); p++)	{
		pack_scn_data.inc_prop_name_index_list[p].offset = offset;
		pack_scn_data.inc_prop_name_index_list[p].size = (int)p_ia_data->property_list[p]->name.size();
		pack_scn_data.inc_prop_name_list[p] = p_ia_data->property_list[p]->name;
		offset += (int)p_ia_data->property_list[p]->name.size();
	}

	// インクコマンドリストを作成
	pack_scn_data.inc_cmd_list.resize(p_ia_data->command_list.size());
	pack_scn_data.inc_cmd_name_list.resize(p_ia_data->command_list.size());

	// インクには宣言のリストしか入っていません。各シーンのヘッダを見て定義と結びつけます。定義がなければエラーです。
	int user_cmd_name_ofs = 0;
	for (int scn_no = 0; scn_no < (int)pack_scn_data.nolzss_scn_data_list.size(); scn_no++)	{
		// ヘッダの参照
		BYTE* data_top = pack_scn_data.nolzss_scn_data_list[scn_no].get();
		if (data_top)	{
			S_tnm_scn_header* p_scn_header = (S_tnm_scn_header *)data_top;
			S_tnm_scn_cmd_label* p_cmd_label = (S_tnm_scn_cmd_label *)(data_top + p_scn_header->cmd_label_list_ofs);
			int cmd_label_cnt = p_scn_header->cmd_label_cnt;

			// シーンにある全てのコマンドについて調査
			for (int c = 0; c < cmd_label_cnt; c++)	{
				int command_id = p_cmd_label[c].cmd_id;

				// インクで宣言されているコマンドの場合（シーンで宣言されているコマンドについてはここでは何もしないのです）
				if (command_id < p_ia_data->inc_command_cnt)	{

					if (p_ia_data->command_list[command_id]->is_defined)	{
						// 既に定義されている！エラー
						set_log(_T("コマンド ") + p_ia_data->command_list[command_id]->name + _T(" が二回以上定義されています。"));
						return false;
					}

					pack_scn_data.inc_cmd_list[command_id].scn_no = scn_no;
					pack_scn_data.inc_cmd_list[command_id].offset = p_cmd_label[c].offset;
					pack_scn_data.inc_cmd_name_list[command_id] = p_ia_data->command_list[command_id]->name;
					p_ia_data->command_list[command_id]->is_defined = true;	// 定義されたフラグ
				}
			}
		}
	}

	// 定義されていない関数があればエラー
	for (int c = 0; c < (int)p_ia_data->command_list.size(); c++)	{
		if (!p_ia_data->command_list[c]->is_defined)	{
			// 定義されていない！エラー
			set_log(_T("コマンド ") + p_ia_data->command_list[c]->name + _T(" が定義されていません。"));
			return false;
		}
	}

	// インクコマンド名インデックスリストを作成
	offset = 0;
	pack_scn_data.inc_cmd_name_index_list.resize(pack_scn_data.inc_cmd_name_list.size());
	for (int i = 0; i < (int)pack_scn_data.inc_cmd_name_list.size(); i++)	{
		pack_scn_data.inc_cmd_name_index_list[i].offset = offset;
		pack_scn_data.inc_cmd_name_index_list[i].size = (int)pack_scn_data.inc_cmd_name_list[i].size();
		offset += (int)pack_scn_data.inc_cmd_name_list[i].size();
	}

	// ================================================================
	// 出力するパックシーンデータを作成
	// ----------------------------------------------------------------

	// 暗号化なしのシーンデータを作成
	if (lzss_mode)	{
		pack_scn_data.noangou_scn_data_index_list = pack_scn_data.lzss_scn_data_index_list;
		pack_scn_data.noangou_scn_data_list = pack_scn_data.lzss_scn_data_list;
	}
	else	{
		pack_scn_data.noangou_scn_data_index_list = pack_scn_data.nolzss_scn_data_index_list;
		pack_scn_data.noangou_scn_data_list = pack_scn_data.nolzss_scn_data_list;
	}

	// 暗号化されたシーンデータを作成
	if (exe_angou_mode)	{

		if (lzss_mode)	{
			pack_scn_data.angou_scn_data_index_list = pack_scn_data.lzss_scn_data_index_list;
			pack_scn_data.angou_scn_data_list = pack_scn_data.lzss_scn_data_list;
		}
		else	{
			pack_scn_data.angou_scn_data_index_list = pack_scn_data.nolzss_scn_data_index_list;
			pack_scn_data.angou_scn_data_list = pack_scn_data.nolzss_scn_data_list;
		}

		// ＥＸＥセット暗号化
		for (int s = 0; s < (int)pack_scn_data.scn_name_list.size(); s++)	{
			BYTE *wp = pack_scn_data.angou_scn_data_list[s].get();
			int eac = 0;
			for (int i = 0; i < pack_scn_data.angou_scn_data_index_list[s].size; i++)	{
				*wp++ ^= exe_angou_element[eac++];
				eac %= TNM_EXE_ANGOU_ELEMENT_CNT;
			}
		}
	}

	// ================================================================
	// パックシーンデータを結合
	// ----------------------------------------------------------------
	C_stream pack_scn_stream_noangou;
	C_stream pack_scn_stream_angou;

	// ヘッダ
	pack_scn_data.header.header_size = sizeof(S_tnm_pack_scn_header);
	pack_scn_stream_noangou.push(pack_scn_data.header);
	// inc プロパティリスト
	pack_scn_data.header.inc_prop_list_ofs = (int)pack_scn_stream_noangou.size();
	pack_scn_data.header.inc_prop_cnt = (int)pack_scn_data.inc_prop_list.size();
	pack_scn_stream_noangou.push_array(pack_scn_data.inc_prop_list.get(), (int)pack_scn_data.inc_prop_list.size());
	// inc プロパティ名インデックスリスト
	pack_scn_data.header.inc_prop_name_index_list_ofs = (int)pack_scn_stream_noangou.size();
	pack_scn_data.header.inc_prop_name_index_cnt = (int)pack_scn_data.inc_prop_name_index_list.size();
	pack_scn_stream_noangou.push_array(pack_scn_data.inc_prop_name_index_list.get(), (int)pack_scn_data.inc_prop_name_index_list.size());
	// inc プロパティ名リスト
	pack_scn_data.header.inc_prop_name_list_ofs = (int)pack_scn_stream_noangou.size();
	pack_scn_data.header.inc_prop_name_cnt = (int)pack_scn_data.inc_prop_name_list.size();
	for (int i = 0; i < (int)pack_scn_data.inc_prop_name_list.size(); i++)
		pack_scn_stream_noangou.push_str(pack_scn_data.inc_prop_name_list[i]);
	// inc コマンドリスト
	pack_scn_data.header.inc_cmd_list_ofs = (int)pack_scn_stream_noangou.size();
	pack_scn_data.header.inc_cmd_cnt = (int)pack_scn_data.inc_cmd_list.size();
	pack_scn_stream_noangou.push_array(pack_scn_data.inc_cmd_list.get(), (int)pack_scn_data.inc_cmd_list.size());
	// inc コマンド名インデックスリスト
	pack_scn_data.header.inc_cmd_name_index_list_ofs = (int)pack_scn_stream_noangou.size();
	pack_scn_data.header.inc_cmd_name_index_cnt = (int)pack_scn_data.inc_cmd_name_index_list.size();
	pack_scn_stream_noangou.push_array(pack_scn_data.inc_cmd_name_index_list.get(), (int)pack_scn_data.inc_cmd_name_index_list.size());
	// inc コマンド名リスト
	pack_scn_data.header.inc_cmd_name_list_ofs = (int)pack_scn_stream_noangou.size();
	pack_scn_data.header.inc_cmd_name_cnt = (int)pack_scn_data.inc_cmd_name_list.size();
	for (int i = 0; i < (int)pack_scn_data.inc_cmd_name_list.size(); i++)
		pack_scn_stream_noangou.push_str(pack_scn_data.inc_cmd_name_list[i]);
	// シーン名インデックスリスト
	pack_scn_data.header.scn_name_index_list_ofs = (int)pack_scn_stream_noangou.size();
	pack_scn_data.header.scn_name_index_cnt = (int)pack_scn_data.scn_name_index_list.size();
	pack_scn_stream_noangou.push_array(pack_scn_data.scn_name_index_list.get(), (int)pack_scn_data.scn_name_index_list.size());
	// シーン名リスト
	pack_scn_data.header.scn_name_list_ofs = (int)pack_scn_stream_noangou.size();
	pack_scn_data.header.scn_name_cnt = (int)pack_scn_data.scn_name_list.size();
	for (int i = 0; i < (int)pack_scn_data.scn_name_list.size(); i++)
		pack_scn_stream_noangou.push_str(pack_scn_data.scn_name_list[i]);

	// ここでパックをコピー
	pack_scn_stream_angou = pack_scn_stream_noangou;

	// シーンデータ

	// 暗号なしから作業
	{
		// シーンデータインデックスリスト
		pack_scn_data.header.scn_data_index_list_ofs = (int)pack_scn_stream_noangou.size();
		pack_scn_data.header.scn_data_index_cnt = (int)pack_scn_data.noangou_scn_data_index_list.size();
		pack_scn_stream_noangou.push_array(pack_scn_data.noangou_scn_data_index_list.get(), (int)pack_scn_data.noangou_scn_data_index_list.size());
		// シーンデータリスト
		pack_scn_data.header.scn_data_list_ofs = (int)pack_scn_stream_noangou.size();
		pack_scn_data.header.scn_data_cnt = (int)pack_scn_data.noangou_scn_data_list.size();
		pack_scn_data.header.scn_data_exe_angou_mod = 0;
		for (int i = 0; i < (int)pack_scn_data.noangou_scn_data_list.size(); i++)
			pack_scn_stream_noangou.push(pack_scn_data.noangou_scn_data_list[i].get(), (int)pack_scn_data.noangou_scn_data_list[i].size());

		// ★ここにデータを追加すると、オリジナルソースが展開できなくなります。

		// オリジナルソースリスト
#if ORIGINAL_SOURCE_LINK

		if (lzss_mode)	{
			pack_scn_data.header.original_source_header_size = (int)original_source_size_list_data.size();
			pack_scn_stream_noangou.push(original_source_size_list_data.get(), (int)original_source_size_list_data.size());
			for (int i = 0; i < (int)original_source_list.size(); i++)
				pack_scn_stream_noangou.push(original_source_list[i].get(), (int)original_source_list[i].size());
		}
		else	{
			pack_scn_data.header.original_source_header_size = 0;
		}
#else
		pack_scn_data.header.original_source_header_size = 0;
#endif

		// ヘッダを修正
		*(S_tnm_pack_scn_header *)pack_scn_stream_noangou.get() = pack_scn_data.header;

		// パックシーンデータを出力
		if (!out_dir_path_noangou.empty())	{

			TSTR pack_scn_file_path = out_dir_path_noangou + _T("\\") + scene_pck;
			NT3::create_dir_from_file_path(pack_scn_file_path);
			if (!C_file::write_full_data(pack_scn_file_path, pack_scn_stream_noangou))	{
				set_log(pack_scn_file_path + _T(" に書き込めませんでした。"));
				return false;
			}
		}

		// 暗号化しない場合は本番にも同じものを書き込む
		if (exe_angou_mode == 0)	{

			// パックシーンデータを出力
			TSTR pack_scn_file_path = out_dir_path + _T("\\") + scene_pck;
			NT3::create_dir_from_file_path(pack_scn_file_path);
			if (!C_file::write_full_data(pack_scn_file_path, pack_scn_stream_noangou))	{
				set_log(pack_scn_file_path + _T(" に書き込めませんでした。"));
				return false;
			}
		}
	}

	// 暗号ありを作業
	if (exe_angou_mode)	{

		// シーンデータインデックスリスト
		pack_scn_data.header.scn_data_index_list_ofs = (int)pack_scn_stream_angou.size();
		pack_scn_data.header.scn_data_index_cnt = (int)pack_scn_data.angou_scn_data_index_list.size();
		pack_scn_stream_angou.push_array(pack_scn_data.angou_scn_data_index_list.get(), (int)pack_scn_data.angou_scn_data_index_list.size());
		// シーンデータリスト
		pack_scn_data.header.scn_data_list_ofs = (int)pack_scn_stream_angou.size();
		pack_scn_data.header.scn_data_cnt = (int)pack_scn_data.angou_scn_data_list.size();
		pack_scn_data.header.scn_data_exe_angou_mod = 1;
		for (int i = 0; i < (int)pack_scn_data.angou_scn_data_list.size(); i++)
			pack_scn_stream_angou.push(pack_scn_data.angou_scn_data_list[i].get(), (int)pack_scn_data.angou_scn_data_list[i].size());

		// ★ここにデータを追加すると、オリジナルソースが展開できなくなります。

		// オリジナルソースリスト
#if ORIGINAL_SOURCE_LINK

		if (lzss_mode)	{
			pack_scn_data.header.original_source_header_size = (int)original_source_size_list_data.size();
			pack_scn_stream_angou.push(original_source_size_list_data.get(), (int)original_source_size_list_data.size());
			for (int i = 0; i < (int)original_source_list.size(); i++)
				pack_scn_stream_angou.push(original_source_list[i].get(), (int)original_source_list[i].size());
		}
		else	{
			pack_scn_data.header.original_source_header_size = 0;
		}
#else
		pack_scn_data.header.original_source_header_size = 0;
#endif

		// ヘッダを修正
		*(S_tnm_pack_scn_header *)pack_scn_stream_angou.get() = pack_scn_data.header;

		// パックシーンデータを出力
		TSTR pack_scn_file_path = out_dir_path + _T("\\") + scene_pck;
		NT3::create_dir_from_file_path(pack_scn_file_path);
		if (!C_file::write_full_data(pack_scn_file_path, pack_scn_stream_angou))	{
			set_log(pack_scn_file_path + _T(" に書き込めませんでした。"));
			return false;
		}
	}

	return true;
}
