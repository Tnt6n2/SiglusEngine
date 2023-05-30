#include	"pch.h"
#include	"data.h"
#include	"dialog.h"
#include	"main.h"
#include	"element.h"
#include	"tnm_common_header.h"
#include	"IA.h"
#include	"CA.h"
#include	"LA.h"
#include	"SA.h"
#include	"MA.h"
#include	"BS.h"
#include	"lzss.h"
#include	"tnm_exe_angou.h"

BYTE tns_easy_angou_code[TNM_EASY_ANGOU_CODE_SIZE] = TNM_EASY_ANGOU_CODE;

// ****************************************************************
// コンパイル：スレッドデータ
// ================================================================
struct S_thread_data
{
	BUFFER	scn_data;
	BUFFER	lzss_data;
};

// ****************************************************************
// 処理：コンパイル本体
// ================================================================
/*
struct S_thread_compile_info
{
	COMPILE_FUNK_STRUCT			cfst;
	C_tnms_scn_compile_data*	p_scd;

};
*/


/*
DWORD WINAPI tnm_compile_thread_func(LPVOID pv_ref)
{
	S_thread_compile_info* p_info = (S_thread_compile_info *)pv_ref;

	// キャンセル
	//if (proc_cancel_flag)
	//	break;

	// ターゲットのシーンデータを設定する
	C_tnms_scn_compile_data* scn_data = p_info->p_scd;
	p_info->cfst.scn_file_name = scn_data->scn_name;
	p_info->cfst.scn_file_path = cfst.scn_dir_path + _T("\\") + p_info->cfst.scn_file_name + _T(".ss");
	p_info->cfst.ca_file_path = cfst.tmp_dir_path + _T("\\ca\\") + p_info->cfst.scn_file_name + _T(".txt");

	// リストビューの更新（ターゲットのファイルを表示する）
//	G_dlg->ci.lv_scn.set_item_visible(scn_data->lv_item_no);

	// ファイルデータの取得
	TSTR scn_text;
	if (!C_file::read_full_text(p_info->cfst.scn_file_path, scn_text))	{
		set_log(cfst.scn_file_path + _T(" を開けませんでした。"));
		error_cnt++;
		continue;
	}

	// コンパイル開始
	//set_log_jump(cfst.scn_file_name + _T(".ss"), cfst.scn_file_path, cfst.scn_file_path, 1);

	// 中間ファイルを削除
	filesys_delete_file(p_info->cfst.tmp_dir_path + _T("\\bs\\") + p_info->cfst.scn_file_name + _T(".dat"));
	filesys_delete_file(p_info->cfst.tmp_dir_path + _T("\\bs\\") + p_info->cfst.scn_file_name + _T(".txt"));
	filesys_delete_file(p_info->cfst.tmp_dir_path + _T("\\bs\\") + p_info->cfst.scn_file_name + _T(".lzss"));

	// os フォルダ内は消してはいけません。コンパイルに失敗したとき、前のデータを使う必要があります。
	//filesys_delete_file(cfst.tmp_dir_path + _T("\\os\\") + cfst.scn_file_name + _T(".ss"));

	// inc データをコピー
	//scn_data->ia_data = *p_ia_data;	▲先にやっといて！

	// ★時間計測
	//cur_time = ::GetTickCount();
	//before_time += cur_time - last_time;
	//last_time = cur_time;

	// 文字解析
	if (!proc_compile_funk_ca_analize(&cfst, scn_data, scn_text))	{	error_cnt++;	continue;	}

	// ★時間計測
	//cur_time = ::GetTickCount();
	//ca_time += cur_time - last_time;
	//last_time = cur_time;

	// 字句解析
	if (!proc_compile_funk_la_analize(&cfst, scn_data))	{	error_cnt++;	continue;	}

	// ★時間計測
	//cur_time = ::GetTickCount();
	//la_time += cur_time - last_time;
	//last_time = cur_time;

	// 構文解析
	if (!proc_compile_funk_sa_analize(&cfst, scn_data))	{	error_cnt++;	continue;	}

	// ★時間計測
	//cur_time = ::GetTickCount();
	//sa_time += cur_time - last_time;
	//last_time = cur_time;

	// 意味解析
	if (!proc_compile_funk_ma_analize(&cfst, scn_data))	{	error_cnt++;	continue;	}

	// ★時間計測
	//cur_time = ::GetTickCount();
	//ma_time += cur_time - last_time;
	//last_time = cur_time;

	// バイナリセーブ
	C_tnms_binary_saver bs;
	S_tnms_bs_data bs_data;
	if (!bs.compile(&scn_data->ia_data, &scn_data->la_data, &scn_data->sa_data, &bs_data, cfst.test_check))	{
		C_tnms_lexcial_analizer la;
		TSTR err_str;
		err_str += cfst.scn_file_name + _T(" line") + tostr(bs.get_error_line()) + _T(": ");
		err_str += _T("\"") + la.get_atom_text(bs.get_error_atom(), &scn_data->la_data) + _T("\"   ");
		err_str += _T("■") + bs.get_error_str();
		set_log_jump(err_str, cfst.ca_file_path, cfst.scn_file_path, bs.get_error_line());
		error_cnt++;
		continue;
	}

	// コンパイル結果出力
	TSTR bs_file_path = cfst.tmp_dir_path + _T("\\bs\\") + cfst.scn_file_name + _T(".dat");
	create_dir_from_file_path(bs_file_path);
	C_file::write_full_data(bs_file_path, bs_data.out_scn);

	// ★時間計測
	//cur_time = ::GetTickCount();
	//bs_time += cur_time - last_time;
	//last_time = cur_time;

	// 圧縮・簡易暗号（簡易リンクの場合は実行しない）
	BUFFER lzss_data;
	int lzss_size = (int)bs_data.out_scn.size();
	if (!G_dlg->ci.chk_easy_link.get_check())	{
		if ((int)lzss_data.size() < lzss_size)	{
			lzss_data.resize(lzss_size);
		}
		Clzss_pack lzss;
		lzss.ready(bs_data.out_scn.get(), bs_data.out_scn.size(), 0);
		while (1)	{
			// 圧縮開始（出力先バッファのポインタとサイズを指定）
			if (lzss.proc(lzss_data.get(), (DWORD)lzss_data.size()))	{
				lzss_size = lzss.pack_size;
				break;	// 圧縮完了
			}
			// 出力先バッファの再確保
			if (lzss.dst_buf_need_size > 0)	{
				lzss_size += bs_data.out_scn.size();
				if ((int)lzss_data.size() < lzss_size)	{
					lzss_data.resize(lzss_size);
				}
			}
		}
		BYTE *wp = lzss_data.get();
		int eac = 0;
		for (int i = 0; i < lzss_size; i++)	{
			*wp++ ^= tns_easy_angou_code[eac++];
			eac %= TNM_EASY_ANGOU_CODE_SIZE;
		}
	}

	// ★時間計測
	//cur_time = ::GetTickCount();
	//pack_time += cur_time - last_time;
	//last_time = cur_time;

	// コンパイル結果出力（バイナリ）
	//if (!G_dlg->ci.chk_easy_link.get_check())	{
		TSTR lzss_file_path = cfst.tmp_dir_path + _T("\\bs\\") + cfst.scn_file_name + _T(".lzss");
		create_dir_from_file_path(lzss_file_path);
		C_file::write_full_data(lzss_file_path, lzss_data.get(), lzss_size);
	//}

	// コンパイル結果出力（テキスト）
	//if (cfst.test_check)	{
	//	TSTR bs_txt_file_path = cfst.tmp_dir_path + _T("\\bs\\") + cfst.scn_file_name + _T(".txt");
	//	create_dir_from_file_path(bs_txt_file_path);
	//	C_file::write_full_text_in_MB(bs_txt_file_path, bs_data.out_txt);
	//}

	// リストビューの更新（コンパイルが成功したので選択状態を解除する）
	//G_dlg->ci.lv_scn.set_item_select(scn_data->lv_item_no, false);

	// ★時間計測
	//cur_time = ::GetTickCount();
	//save_time += cur_time - last_time;
	//last_time = cur_time;

	// ちょっと解放してみよう…
	scn_data->ia_data.replace_tree.clear();
	scn_data->la_data.atom_list.clear();
	scn_data->la_data.label_list.clear();
	scn_data->la_data.str_list.clear();
	scn_data->la_data.unknown_list.clear();
	scn_data->sa_data.root.reset();

	// 成功
	return TRUE;
}
*/

bool C_tnm_compiler_main::proc_compile(S_tnms_ia_data* p_ia_data, int* p_error_cnt)
{
	ARRAY<C_tnms_scn_compile_data>	scn_data_list;

	// プロジェクトが選択されていなければ失敗
	if (!check_current_project(true))
		return false;

	// ありえないバグですが、ＳＳリストとＳＳリストビューのアイテム数が一致するかチェックする
	if ((int)G_data->scn_list.size() != G_dlg->ci.lv_scn.get_item_cnt())	{
		error_box(_T("システムバグ：ＳＳリストとＳＳリストビューのアイテム数が一致しません。"));
		return false;
	}

	// データのクリア
	scn_data_list.clear();

	// コンパイル処理用構造体
	COMPILE_FUNK_STRUCT cfst;
	cfst.scn_dir_path = G_data->project[G_data->cur_project_no].scn_path;		// ダイアログのデータを取得
	cfst.tmp_dir_path = G_data->project[G_data->cur_project_no].tmp_path;		// ダイアログのデータを取得
	cfst.utf8 = G_data->project[G_data->cur_project_no].utf8;
	cfst.test_check = G_dlg->ci.chk_test.get_check();

	// コンパイル開始
	set_log(_T("- コンパイルを開始します。"));

	// 選択されているシーンのコンパイル用データを作成
	for (int i = 0; i < (int)G_data->scn_list.size(); ++i)	{
		if (G_dlg->ci.lv_scn.get_item_select(i))	{
			C_tnms_scn_compile_data scn_data;
			scn_data.lv_item_no = i;
			scn_data.scn_name = erase_extension_ret(G_data->scn_list[i].scn_name);
			scn_data_list.push_back(scn_data);
		}
	}

	int ca_time = 0;
	int la_time = 0;
	int sa_time = 0;
	int ma_time = 0;
	int bs_time = 0;
	int pack_time = 0;
	int save_time = 0;
	int before_time = 0;

	// コンパイル成功フラグ
	int error_cnt = 0;

	int last_time = ::GetTickCount();
	int cur_time = 0;

	// 全シーンファイルについて構文解析までを行う
	for (int scn_no = 0; scn_no < (int)scn_data_list.size(); scn_no++)	{

		// キャンセル
		if (proc_cancel_flag)
			break;

		// ターゲットのシーンデータを設定する
		C_tnms_scn_compile_data* scn_data = &scn_data_list[scn_no];
		cfst.scn_file_name = scn_data->scn_name;
		cfst.scn_file_path = cfst.scn_dir_path + _T("\\") + cfst.scn_file_name + _T(".ss");
		cfst.ca_file_path = cfst.tmp_dir_path + _T("\\ca\\") + cfst.scn_file_name + _T(".txt");

		// リストビューの更新（ターゲットのファイルを表示する）
		G_dlg->ci.lv_scn.set_item_visible(scn_data->lv_item_no);

		// ファイルデータの取得
		TSTR scn_text;
		if (!(cfst.utf8 ? C_file::read_full_text_UTF8(cfst.scn_file_path, scn_text) : C_file::read_full_text(cfst.scn_file_path, scn_text)))	{
			set_log(cfst.scn_file_path + _T(" を開けませんでした。"));
			error_cnt++;
			continue;
		}

		// コンパイル開始
		set_log_jump(cfst.scn_file_name + _T(".ss"), _T(""), cfst.scn_file_path, 1);

		// 中間ファイルを削除
		filesys_delete_file(cfst.tmp_dir_path + _T("\\bs\\") + cfst.scn_file_name + _T(".dat"));
		filesys_delete_file(cfst.tmp_dir_path + _T("\\bs\\") + cfst.scn_file_name + _T(".txt"));
		filesys_delete_file(cfst.tmp_dir_path + _T("\\bs\\") + cfst.scn_file_name + _T(".lzss"));

		// os フォルダ内は消してはいけません。コンパイルに失敗したとき、前のデータを使う必要があります。
		//filesys_delete_file(cfst.tmp_dir_path + _T("\\os\\") + cfst.scn_file_name + _T(".ss"));

		// inc データをコピー
		scn_data->ia_data = *p_ia_data;

		// ★時間計測
		cur_time = ::GetTickCount();
		before_time += cur_time - last_time;
		last_time = cur_time;

		// 文字解析
		if (!proc_compile_funk_ca_analize(&cfst, scn_data, scn_text))	{	error_cnt++;	continue;	}

		// ★時間計測
		cur_time = ::GetTickCount();
		ca_time += cur_time - last_time;
		last_time = cur_time;

		// 字句解析
		if (!proc_compile_funk_la_analize(&cfst, scn_data))	{	error_cnt++;	continue;	}

		// ★時間計測
		cur_time = ::GetTickCount();
		la_time += cur_time - last_time;
		last_time = cur_time;

		// 構文解析
		if (!proc_compile_funk_sa_analize(&cfst, scn_data))	{	error_cnt++;	continue;	}

		// ★時間計測
		cur_time = ::GetTickCount();
		sa_time += cur_time - last_time;
		last_time = cur_time;

		// 意味解析
		if (!proc_compile_funk_ma_analize(&cfst, scn_data))	{	error_cnt++;	continue;	}

		// ★時間計測
		cur_time = ::GetTickCount();
		ma_time += cur_time - last_time;
		last_time = cur_time;

		// バイナリセーブ
		C_tnms_binary_saver bs;
		S_tnms_bs_data bs_data;
		if (!bs.compile(&scn_data->ia_data, &scn_data->la_data, &scn_data->sa_data, &bs_data, cfst.test_check))	{
			C_tnms_lexcial_analizer la;
			TSTR err_str;
			err_str += cfst.scn_file_name + _T(" line") + tostr(bs.get_error_line()) + _T(": ");
			err_str += _T("\"") + la.get_atom_text(bs.get_error_atom(), &scn_data->la_data) + _T("\"   ");
			err_str += _T("■") + bs.get_error_str();
			set_log_jump(err_str, cfst.ca_file_path, cfst.scn_file_path, bs.get_error_line());
			error_cnt++;
			continue;
		}

		// コンパイル結果出力
		TSTR bs_file_path = cfst.tmp_dir_path + _T("\\bs\\") + cfst.scn_file_name + _T(".dat");
		create_dir_from_file_path(bs_file_path);
		C_file::write_full_data(bs_file_path, bs_data.out_scn);

		// ★時間計測
		cur_time = ::GetTickCount();
		bs_time += cur_time - last_time;
		last_time = cur_time;

		// 圧縮・簡易暗号（簡易リンクの場合は実行しない）
		static BUFFER lzss_data;
		int lzss_size = (int)bs_data.out_scn.size();
		if (!G_dlg->ci.chk_easy_link.get_check())	{
			if ((int)lzss_data.size() < lzss_size)	{
				lzss_data.resize(lzss_size);
			}
			Clzss_pack lzss;
			lzss.ready(bs_data.out_scn.get(), bs_data.out_scn.size(), 0);
			while (1)	{
				// 圧縮開始（出力先バッファのポインタとサイズを指定）
				if (lzss.proc(lzss_data.get(), (DWORD)lzss_data.size()))	{
					lzss_size = lzss.pack_size;
					break;	// 圧縮完了
				}
				// 出力先バッファの再確保
				if (lzss.dst_buf_need_size > 0)	{
					lzss_size += bs_data.out_scn.size();
					if ((int)lzss_data.size() < lzss_size)	{
						lzss_data.resize(lzss_size);
					}
				}
			}
			BYTE *wp = lzss_data.get();
			int eac = 0;
			for (int i = 0; i < lzss_size; i++)	{
				*wp++ ^= tns_easy_angou_code[eac++];
				eac %= TNM_EASY_ANGOU_CODE_SIZE;
			}
		}

		// ★時間計測
		cur_time = ::GetTickCount();
		pack_time += cur_time - last_time;
		last_time = cur_time;

		// コンパイル結果出力（バイナリ）
		if (!G_dlg->ci.chk_easy_link.get_check())	{
			TSTR lzss_file_path = cfst.tmp_dir_path + _T("\\bs\\") + cfst.scn_file_name + _T(".lzss");
			create_dir_from_file_path(lzss_file_path);
			C_file::write_full_data(lzss_file_path, lzss_data.get(), lzss_size);
		}

		// コンパイル結果出力（テキスト）
		if (cfst.test_check)	{
			TSTR bs_txt_file_path = cfst.tmp_dir_path + _T("\\bs\\") + cfst.scn_file_name + _T(".txt");
			create_dir_from_file_path(bs_txt_file_path);
			cfst.utf8 ? C_file::write_full_text_in_UTF8(bs_txt_file_path, bs_data.out_txt) : C_file::write_full_text_in_MB(bs_txt_file_path, bs_data.out_txt);
		}

		// リストビューの更新（コンパイルが成功したので選択状態を解除する）
		G_dlg->ci.lv_scn.set_item_select(scn_data->lv_item_no, false);

		// ★時間計測
		cur_time = ::GetTickCount();
		save_time += cur_time - last_time;
		last_time = cur_time;

		// ちょっと解放してみよう…
#if 0
		scn_data->ia_data.init();
		scn_data->ca_data.property_list.clear();
		scn_data->la_data.atom_list.clear();
		scn_data->la_data.label_list.clear();
		scn_data->la_data.str_list.clear();
		scn_data->la_data.unknown_list.clear();
		scn_data->sa_data.root.reset();
		scn_data->sa_data.z_label_list.clear();
		scn_data->sa_data.call_prop_name_list.clear();
#else
		*scn_data = C_tnms_scn_compile_data();	// ディープな開放

//		scn_data->la_data.atom_list.clear();
//		scn_data->la_data.label_list.clear();
//		scn_data->la_data.str_list.clear();
//		scn_data->la_data.unknown_list.clear();
//		scn_data->sa_data.root.reset();
#endif
	}

	set_log(_T("準備時間: ") + tostr(before_time));
	set_log(_T("文字解析時間: ") + tostr(ca_time));
	set_log(_T("字句解析時間: ") + tostr(la_time));
	set_log(_T("構文解析時間: ") + tostr(sa_time));
	set_log(_T("意味解析時間: ") + tostr(ma_time));
	set_log(_T("ﾊﾞｲﾅﾘ作成時間: ") + tostr(bs_time));
	set_log(_T("圧縮時間: ") + tostr(pack_time));
	set_log(_T("保存時間: ") + tostr(save_time));

	if (p_error_cnt)
		*p_error_cnt = error_cnt;

	return error_cnt == 0;
}

// ****************************************************************
// 処理：コンパイル：文字解析
// ================================================================
bool C_tnm_compiler_main::proc_compile_funk_ca_analize(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data, CTSTR scn_text)
{
	// 文字解析
	C_tnms_character_analizer ca;
	if (!ca.analize_file(scn_text, &scn_data->ia_data, &scn_data->ca_data))	{
		TSTR err_str = cfst->scn_file_name + _T(" line") + tostr(ca.get_error_line()) + _T(": ") + ca.get_error_str();
		set_log_jump(err_str, cfst->scn_file_path, cfst->scn_file_path, ca.get_error_line());
		return false;
	}

	// 文字解析の結果出力
	// ステータスバーの表示に使うので、必ず出力します。

	//if (cfst->test_check)	{
		C_file ca_file;
		create_dir_from_file_path(cfst->ca_file_path);
		if (ca_file.open(cfst->ca_file_path, _T("wt")))
			cfst->utf8 ? ca_file.write_in_UTF8(scn_data->ca_data.scn_text) : ca_file.write_in_MB(scn_data->ca_data.scn_text);
	//}

	return true;
}

// ****************************************************************
// 処理：コンパイル：字句解析
// ================================================================
bool C_tnm_compiler_main::proc_compile_funk_la_analize(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data)
{
	C_tnms_lexcial_analizer la;

	// 字句解析
	if (!la.analize(&scn_data->ca_data, &scn_data->la_data))	{
		TSTR err_str = cfst->scn_file_name + _T(" line") + tostr(la.get_error_line()) + _T(": ") + la.get_error_str();
		set_log_jump(err_str, cfst->ca_file_path, cfst->scn_file_path, la.get_error_line());
		return false;
	}

	// 字句解析の結果出力
	if (cfst->test_check)	{
		C_file la_file;
		TSTR la_file_path = cfst->tmp_dir_path + _T("\\la\\") + cfst->scn_file_name + _T(".txt");
		create_dir_from_file_path(la_file_path);
		if (la_file.open(la_file_path, _T("wt")))	{

			// アトムリストの出力
			la_file.write("\n-- atom list --\n");
			int atom_cnt = (int)scn_data->la_data.atom_list.size();
			for (int a = 0; a < atom_cnt; a++)	{
				TSTR str;
				str += str_format(_T("%04d, Line %04d: "), a, scn_data->la_data.atom_list[a].line);
				str += TNMS_ATOM_TYPE_STR[scn_data->la_data.atom_list[a].type] + _T(",");
				str += tostr(scn_data->la_data.atom_list[a].opt) + _T(",");
				str += tostr(scn_data->la_data.atom_list[a].subopt);
				str += _T("\n");
				cfst->utf8 ? la_file.write_in_UTF8(str) : la_file.write_in_MB(str);
			}

			// 文字列リストの出力
			la_file.write("\n-- string list --\n");
			int str_cnt = (int)scn_data->la_data.str_list.size();
			for (int s = 0; s < str_cnt; s++)	{
				TSTR str;
				str += str_format(_T("%04d: "), s);
				str += scn_data->la_data.str_list[s];
				str += _T("\n");
				cfst->utf8 ? la_file.write_in_UTF8(str) : la_file.write_in_MB(str);
			}

			// ラベルリストの出力
			la_file.write("\n-- LabelList --\n");
			int label_cnt = (int)scn_data->la_data.label_list.size();
			for (int l = 0; l < label_cnt; l++)	{
				TSTR str;
				str += str_format(_T("%04d: "), l);
				str += scn_data->la_data.label_list[l].name;
				str += _T("\n");
				cfst->utf8 ? la_file.write_in_UTF8(str) : la_file.write_in_MB(str);
			}

			// 未定義語リストの出力
			la_file.write("\n-- UnknownList --\n");
			int unknown_cnt = (int)scn_data->la_data.unknown_list.size();
			for (int u = 0; u < unknown_cnt; u++)	{
				TSTR str;
				str += str_format(_T("%04d: "), u);
				str += scn_data->la_data.unknown_list[u];
				str += _T("\n");
				cfst->utf8 ? la_file.write_in_UTF8(str) : la_file.write_in_MB(str);
			}
		}
		la_file.close();
	}

	return true;
}

// ****************************************************************
// 処理：コンパイル：構文解析
// ================================================================
bool C_tnm_compiler_main::proc_compile_funk_sa_analize(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data)
{
	C_tnms_lexcial_analizer la;
	C_tnms_sentence_analizer sa;

	// 構文解析
	if (!sa.analize(&scn_data->ia_data, &scn_data->la_data, &scn_data->sa_data))	{
		TSTR err_str;
		err_str += cfst->scn_file_name + _T(" line") + tostr(sa.get_error_line()) + _T(": ");
		err_str += _T("\"") + la.get_atom_text(sa.get_error_atom(), &scn_data->la_data) + _T("\"   ");
		err_str += _T("■") + sa.get_error_str();
		set_log_jump(err_str, cfst->ca_file_path, cfst->scn_file_path, sa.get_error_line());
		return false;
	}

	return true;
}

// ****************************************************************
// 処理：コンパイル：意味解析
// ================================================================
bool C_tnm_compiler_main::proc_compile_funk_ma_analize(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data)
{
	C_tnms_lexcial_analizer la;
	C_tnms_semantic_analizer ma;

	if (!ma.analize(&scn_data->ia_data, &scn_data->la_data, &scn_data->sa_data))	{
		TSTR err_str;
		err_str += cfst->scn_file_name + _T(" line") + tostr(ma.get_error_line()) + _T(": ");
		err_str += _T("\"") + la.get_atom_text(ma.get_error_atom(), &scn_data->la_data) + _T("\"   ");
		err_str += _T("■") + ma.get_error_str();
		set_log_jump(err_str, cfst->ca_file_path, cfst->scn_file_path, ma.get_error_line());
		return false;
	}
	return true;
}

// ****************************************************************
// 処理：コンパイル：半角文字列パラメータの変換（※エラーはありえないのでエラー処理はまったく考えていません）
// ================================================================
bool C_tnm_compiler_main::proc_compile_funk_hankaku_param_format(const COMPILE_FUNK_STRUCT* cfst, C_tnms_scn_compile_data* scn_data)
{
	// 情報のコピー
	TSTR in_text = scn_data->ca_data.scn_text;

	// 番兵の挿入
	for (int i = 0; i < 256; i++)
		in_text.push_back(_T('\0'));

	enum STATE
	{
		STATE_NONE,
		STATE_SINGLE_QUOTE,			// シングルコーテーション
		STATE_SINGLE_QUOTE_ESCAPE,	// シングルコーテーションエスケープ
		STATE_SINGLE_QUOTE_END,		// シングルコーテーション終了
		STATE_DOUBLE_QUOTE,			// ダブルコーテーション
		STATE_DOUBLE_QUOTE_ESCAPE,	// ダブルコーテーションエスケープ
	};

	TSTR out_text;
	STATE state = STATE_NONE;			// 現在のステート
	int kako_cnt = 0;
	bool push_back_ok;
	TSTR word;

	for (TSTR::iterator p = in_text.begin(); *p != _T('\0'); ++p)	{

		push_back_ok = true;	// 出力フラグをＯＮ
		TCHAR moji = *p;		// 出力文字

		// シングルコーテーション中
		if (state == STATE_SINGLE_QUOTE)	{
			if (false);
			// エスケープ
			else if (*p == _T('\\'))
				state = STATE_SINGLE_QUOTE_ESCAPE;
			else
				state = STATE_SINGLE_QUOTE_END;
		}
		// シングルコーテーションエスケープ中
		else if (state == STATE_SINGLE_QUOTE_ESCAPE)	{
			if (*p == _T('\\') || *p == _T('\''))
				state = STATE_SINGLE_QUOTE_END;
		}
		// シングルコーテーション終了
		else if (state == STATE_SINGLE_QUOTE_END)	{
			// シングルコーテーション終了
			if (*p == _T('\''))
				state = STATE_NONE;
		}
		// ダブルコーテーション中
		else if (state == STATE_DOUBLE_QUOTE)	{
			// エスケープ
			if (*p == _T('\\'))
				state = STATE_DOUBLE_QUOTE_ESCAPE;
			// ダブルコーテーション終了
			if (*p == _T('"'))
				state = STATE_NONE;
		}
		// ダブルコーテーションエスケープ中
		else if (state == STATE_DOUBLE_QUOTE_ESCAPE)	{
			if (*p == _T('\\') || *p == _T('"'))
				state = STATE_DOUBLE_QUOTE;
		}
		// 通常処理
		else if (state == STATE_NONE)	{

			if (false);
			// シングルコーテーション開始
			else if (*p == _T('\''))
				state = STATE_SINGLE_QUOTE;
			// ダブルコーテーション開始
			else if (*p == _T('"'))
				state = STATE_DOUBLE_QUOTE;
			// 開き括弧発見
			else if (*p == _T('('))
				kako_cnt++;
			// 閉じ括弧発見
			else if (*p == _T(')'))
				kako_cnt--;
			// ファイル名の頭文字として使用可能な文字
			else if (kako_cnt > 0 && (*p == _T('_') || (_T('a') <= *p && *p <= _T('z'))))	{
				while (1)	{
					if (*p == _T('_') || (_T('a') <= *p && *p <= _T('z')) || (_T('0') <= *p && *p <= _T('9')))	{
						word.push_back(*p);
						p++;
					}
					else	{	break;	}
				}
				if (!scn_data->ia_data.form_table.check_element_info(word))	{
					out_text += _T('\"') + word + _T('\"');
				}
				else	{
					out_text += word;
				}
				word.clear();
				p--;					// 進めすぎたポインタを１つ戻す
				push_back_ok = false;	// 出力フラグをＯＦＦ
			}
		}

		// 出力
		if (push_back_ok)	{	out_text.push_back(moji);	}
	}

	// テキストの入れ替え
	scn_data->ca_data.scn_text = out_text;

	// 半角文字列パラメータの変換の結果出力
	TSTR ca2_file_path = cfst->tmp_dir_path + _T("\\ca2\\") + cfst->scn_file_name + _T(".txt");
	C_file ca_file;
	create_dir_from_file_path(ca2_file_path);
	if (ca_file.open(ca2_file_path, _T("wt")))
		cfst->utf8 ? ca_file.write_in_UTF8(scn_data->ca_data.scn_text) : ca_file.write_in_MB(scn_data->ca_data.scn_text);

	return true;
}

