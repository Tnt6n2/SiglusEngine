#include	"pch.h"

#include	"tnm_eng.h"
#include	"data/tnm_ini.h"

#include	<lzss.h>

bool	tnm_save_global_on_file_sub_global_data();		// グローバルデータをファイルにセーブ
bool	tnm_load_global_on_file_sub_global_data();		// グローバルデータをファイルからロード
bool	tnm_save_global_on_file_sub_system_data();		// システム設定データをファイルにセーブ
bool	tnm_load_global_on_file_sub_system_data();		// システム設定データをファイルからロード
bool	tnm_save_global_on_file_sub_movie_data();		// ムービー設定データをファイルにセーブ
bool	tnm_load_global_on_file_sub_movie_data();		// ムービー設定データをファイルからロード
bool	tnm_save_global_on_file_sub_read_data();		// 既読データをファイルにセーブ
bool	tnm_load_global_on_file_sub_read_data();		// 既読データをファイルからロード
bool	tnm_save_global_on_file_sub_config_data();		// 環境設定データをファイルにセーブ
bool	tnm_load_global_on_file_sub_config_data();		// 環境設定データをファイルからロード

// ****************************************************************
// ファイルセーブ（共通）
// ================================================================

// バイナリデータ
bool tnm_save_to_file(CTSTR& file_path, BUFFER& write_data)
{
	// ファイルの読み取り専用をはずす
	file_set_read_only(file_path, false);

	// ファイルに書き込む
	if (!C_file::write_full_data(file_path, write_data))
		return false;

	Sleep(0);

	// データチェックのために今書き込んだデータを読み込む
	BUFFER read_data;
	if (!C_file::read_full_data(file_path, read_data))
		return false;

	// 書き込んだデータと読み込んだデータを比較する
	if (read_data.size() != write_data.size() || memcmp(read_data.get(), write_data.get(), read_data.size()) != 0)	{

		// ★破損データなのでファイルを消します。
		filesys_delete_file(file_path);

		// 再度セーブファイルに書き込む
		if (!C_file::write_full_data(file_path, write_data))
			return false;

		// 再度データチェック
		BUFFER read_data;
		if (!C_file::read_full_data(file_path, read_data))
			return false;

		if (read_data.size() != write_data.size() || memcmp(read_data.get(), write_data.get(), read_data.size()) != 0)	{

			// ★破損データなのでファイルを消します。
			filesys_delete_file(file_path);

			return false;
		}
	}

	return true;
}

// テキストデータ
bool tnm_save_to_file(CTSTR& file_path, CTSTR& write_data)
{
	// ファイルの読み取り専用をはずす
	file_set_read_only(file_path, false);

	// ファイルに書き込む
	if (!C_file::write_full_text_in_MB(file_path, write_data))
		return false;

	// データチェックのために今書き込んだデータを読み込む
	TSTR read_data;
	if (!C_file::read_full_text(file_path, read_data))
		return false;

	// 書き込んだデータと読み込んだデータを比較する
	if (read_data != write_data)	{

		// ★破損データなのでファイルを消します。
		filesys_delete_file(file_path);

		// 再度セーブファイルに書き込む
		if (!C_file::write_full_text_in_MB(file_path, write_data))
			return false;

		// 再度データチェック
		TSTR read_data;
		if (!C_file::read_full_text(file_path, read_data))
			return false;

		if (read_data != write_data)	{

			// ★破損データなのでファイルを消します。
			filesys_delete_file(file_path);

			return false;
		}
	}

	return true;
}

// ****************************************************************
// 圧縮＆暗号
// ================================================================
bool tnm_pack_buffer(ARRAY<BYTE>& src)
{
	int rate = 5;	// 標準は５倍確保
	if (src.size() > 1024 * 1024)	{		// 1MBを超えるおおきなデータの場合
		rate = 2;	// ２倍まで下げる
	}

	ARRAY<BYTE> dst(src.size() * rate);		// ５倍のサイズを確保する

	Clzss_pack lzss;
	lzss.ready(src.get(), (DWORD)src.size(), 0);
	while (1)	{
		// 圧縮開始（出力先バッファのポインタとサイズを指定）
		if (lzss.proc(dst.get(), (DWORD)dst.size()))	{
			dst.resize(lzss.pack_size);		// 余分な領域をカット
			break;	// 圧縮完了
		}
		// 出力先バッファの再確保
		if (lzss.dst_buf_need_size > 0)	{
			dst.resize((DWORD)dst.size() + (int)src.size() * rate);		// さらに５倍のサイズを追加確保する
		}
	}

	// 暗号
	tpc_angou(dst.get(), (int)dst.size());

	src = dst;
	return true;
}

// ****************************************************************
// 復号＆解凍
// ================================================================
bool tnm_unpack_buffer(ARRAY<BYTE>& src)
{
	// 復号
	tpc_unangou(src.get(), (int)src.size());

	// 解凍
	int size = lzss_unpack(src.get(), NULL);
	ARRAY<BYTE> dst(size);				// バッファ確保
	lzss_unpack(src.get(), dst.get());

	src = dst;
	return true;
}

// ****************************************************************
// ファイルロード（システムデータ）
// ================================================================
bool tnm_load_system_on_file()
{
	// システム設定データを読み込む
	tnm_load_global_on_file_sub_system_data();

	return true;
}

// ****************************************************************
// ファイルセーブ（グローバル）
// ================================================================
bool tnm_save_global_on_file()
{
	// ★初期化が終わってない場合は何もしない（起動に失敗したときなど）
	if (Gp_eng->m_init_success_flag)	{
		
		// グローバルデータ
		tnm_save_global_on_file_sub_global_data();

		// システム設定データ
		tnm_save_global_on_file_sub_system_data();

		// 既読データ
		tnm_save_global_on_file_sub_read_data();

		// 環境設定データ
		tnm_save_global_on_file_sub_config_data();
	}

	return true;
}

// ****************************************************************
// ファイルロード（グローバル）
// ================================================================
bool tnm_load_global_on_file()
{
	// グローバルデータ
	if (!tnm_load_global_on_file_sub_global_data())
		return false;

	// 既読データ
	if (!tnm_load_global_on_file_sub_read_data())
		return false;

	// 環境設定データ
	if (!tnm_load_global_on_file_sub_config_data())
		return false;

	return true;
}

// ****************************************************************
// ファイルセーブ（グローバルデータ）
// ================================================================
bool tnm_save_global_on_file_sub_global_data()
{
	// グローバルデータ
	C_tnm_save_stream global_data;
	Gp_eng->save_global(global_data);

	// 圧縮する
	tnm_pack_buffer(global_data.get_buffer());

	// ヘッダを作成する
	S_tnm_global_save_header global_header;
	global_header.major_version = 2;
	global_header.minor_version = 0;
	global_header.global_data_size = (int)global_data.size();

	// 全書き込みデータを統合する
	BUFFER global_total_data;
	global_total_data.insert(global_total_data.end(), (BYTE *)&global_header, (BYTE *)&global_header + sizeof(global_header));
	global_total_data.insert(global_total_data.end(), global_data.begin(), global_data.end());

	// パスを求めてフォルダを作成する
	TSTR global_save_file_path = Gp_dir->get_global_save_file_path();
	create_dir_from_file_path(global_save_file_path);
	if (!tnm_save_to_file(global_save_file_path, global_total_data))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("グローバルデータの保存に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	// 成功
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("グローバルデータを保存しました。"));

	return true;
}

// ****************************************************************
// ファイルロード（グローバルデータ）
// ================================================================
bool tnm_load_global_on_file_sub_global_data()
{
	// グローバルセーブのパスを求める
	TSTR global_save_file_path = Gp_dir->get_global_save_file_path();

	// ファイルの存在判定
	if (file_get_type(global_save_file_path) == 1)	{

		C_tnm_save_stream global_data;

		// ファイルを開く
		C_file file;
		if (!file.open(global_save_file_path, _T("rb")))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("グローバルデータが開けませんでした。\n") + get_last_error_msg());
			return false;			// 読めなかったのでここで終了
		}
		
		// ヘッダを読み込む
		S_tnm_global_save_header global_header;
		file.read(&global_header, sizeof(global_header));
		if (global_header.major_version == 2 && global_header.minor_version == 0)	{

			// データを読み込む
			global_data.resize(global_header.global_data_size);
			file.read(global_data.get(), (int)global_data.size());
			file.close();

			// 解凍する
			tnm_unpack_buffer(global_data.get_buffer());

			// シークをリセット
			global_data.set_seek(0);

			// 成功
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("グローバルデータを読み込みました。"));

			// エンジン本体を呼び出す
			Gp_eng->load_global(global_data);

			// 成功
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("グローバルデータの再構築に成功しました。"));
		}
		else	{

			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("セーブデータのバージョンが古いです。\nセーブデータを削除してから再度ゲームを起動して下さい。"));
			return false;			// 読めなかったのでここで終了
		}
	}

	return true;
}

// ****************************************************************
// ファイルセーブ（システム設定データ）
// ================================================================
#if 0
const TSTR system_ini_data =
	_T("// ****************************************************************\n")
	_T("// SiglusEngine システム設定\n")
	_T("// ****************************************************************\n")
	_T("\n")
	_T("// Direct3D: 使用するアダプタ番号\n")
	_T("#D3D.ADAPTER_NO = 0\n")
	_T("\n")
	_T("// Direct3D: 垂直同期を待つ\n")
	_T("#D3D.PRESENT_INTERVAL = 1\n")
	_T("\n")
	_T("// Direct3D: ミップマップの自動生成を行う\n")
	_T("#D3D.AUTOGENMIPMAP = 1\n")
	_T("\n")
	_T("// Direct3D: 頂点処理をハードウェアで行う\n")
	_T("#D3D.HARDWARE_VERTEX = 1\n")
	_T("\n")
	_T("// Direct3D: ワイプバッファの枚数\n")
	_T("#D3D.WIPE_BUFFER_COUNT = 2\n")
	_T("\n")
	_T("// DirectSound: バックグラウンドで無音を再生し続ける\n")
	_T("#DS.PLAY_SILENT_SOUND = 0\n")
;
bool tnm_save_global_on_file_sub_system_data()
{
	// システム設定データのパスを求める
	TSTR system_ini_file_path = Gp_dir->get_system_ini_file_path();

	// ファイルがなかった場合のみ新規作成（ファイルがあった場合は何もしない）
	if (file_get_type(system_ini_file_path) == -1)	{

		// システム設定データを書き込む
		create_dir_from_file_path(system_ini_file_path);
		if (!tnm_save_file(system_ini_file_path, system_ini_data))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("システム設定データの保存に失敗しました。\n") + get_last_error_msg());
			return false;
		}
	}

	return true;
}
#endif

// ****************************************************************
// ファイルセーブ（システム設定データ）
// ================================================================
bool tnm_save_global_on_file_sub_system_data()
{
	ARRAY<TSTR> system_ini_data;
	system_ini_data.push_back(_T("#D3D.ADAPTER_NO = ") + tostr(Gp_system_config->d3d_adapter_no));
	system_ini_data.push_back(_T("#D3D.HARDWARE_VERTEX = ") + tostr(Gp_system_config->d3d_hardware_vertex_flag ? 1 : 0));
	system_ini_data.push_back(_T("#D3D.PRESENT_INTERVAL = ") + tostr(Gp_system_config->d3d_present_interval));
	system_ini_data.push_back(_T("#D3D.USE_MIPMAP = ") + tostr(Gp_system_config->d3d_use_mipmap_flag ? 1 : 0));
	system_ini_data.push_back(_T("#D3D.WIPE_BUFFER_COUNT = ") + tostr(Gp_system_config->d3d_wipe_buffer_cnt));
	system_ini_data.push_back(_T("#DS.PLAY_SILENT_SOUND = ") + tostr(Gp_system_config->ds_play_silent_sound_flag ? 1 : 0));
	system_ini_data.push_back(_T("#MOVIE_TYPE = ") + tostr(Gp_system_config->movie_type));

	TSTR system_ini_file_path = Gp_dir->get_system_ini_file_path();
	if (!::create_dir_from_file_path(system_ini_file_path))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("セーブフォルダの作成に失敗しました。\n") + get_last_error_msg());
	}
	else if (!C_file::write_full_text_in_MB(system_ini_file_path, system_ini_data))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("system.ini の書き込みに失敗しました。\n") + get_last_error_msg());
	}

	return true;
}

// ****************************************************************
// ファイルロード（システム設定データ）
// ================================================================
bool tnm_load_global_on_file_sub_system_data()
{
	// システム設定のパスを求める
	TSTR system_ini_file_path = Gp_dir->get_system_ini_file_path();

	// ファイルの存在判定
	if (file_get_type(system_ini_file_path) == 1)	{

		// ファイルを読み込む
		ARRAY<TSTR> system_ini_data;
		if (!C_file::read_full_text(system_ini_file_path, system_ini_data))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("システム設定データが読み込めませんでした。\n") + get_last_error_msg());
			return false;			// 読めなかったのでここで終了
		}

		// ファイルを解析する
		for (int i = 0; i < (int)system_ini_data.size(); i++)	{
			TSTR line_data = system_ini_data[i];
			C_str_analizer anl(line_data.begin(), line_data.end());
			int num[4];

			if (false);
			else if (anl.get_format(_T("//")))	{
				// NOP
			}
			else if (anl.get_format(_T("#D3D.ADAPTER_NO = %d"), &num[0]))	{
				Gp_system_config->d3d_adapter_no = num[0];
			}
			else if (anl.get_format(_T("#D3D.HARDWARE_VERTEX = %d"), &num[0]))	{
				Gp_system_config->d3d_hardware_vertex_flag = num[0] ? true : false;
			}
			else if (anl.get_format(_T("#D3D.PRESENT_INTERVAL = %d"), &num[0]))	{
				Gp_system_config->d3d_present_interval = limit(0, num[0], 1);	// 0or1
			}
			else if (anl.get_format(_T("#D3D.USE_MIPMAP = %d"), &num[0]))	{
				Gp_system_config->d3d_use_mipmap_flag = num[0] ? true : false;
			}
			else if (anl.get_format(_T("#D3D.WIPE_BUFFER_COUNT = %d"), &num[0]))	{
				Gp_system_config->d3d_wipe_buffer_cnt = limit(1, num[0], 2);	// 1or2枚
			}
			else if (anl.get_format(_T("#DS.PLAY_SILENT_SOUND = %d"), &num[0]))	{
				Gp_system_config->ds_play_silent_sound_flag = num[0] ? true : false;
			}
			else if (anl.get_format(_T("#MOVIE_TYPE = %d"), &num[0]))	{
				Gp_system_config->movie_type = limit(0, num[0], 1);
			}
		}

		// 成功
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("システム設定データを読み込みました。"));

		// 履歴
		if (Gp_system_config->d3d_adapter_no != 0)
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ディスプレイアダプタ") + tostr(Gp_system_config->d3d_adapter_no) + _T("番を使います。"));
		if (Gp_system_config->d3d_hardware_vertex_flag == false)
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- 頂点処理をソフトウェアで行います。"));
		if (Gp_system_config->d3d_present_interval == 0)
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- 描画の際に、垂直同期を取りません。"));
		if (Gp_system_config->d3d_use_mipmap_flag == false)
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ミップマップを使いません。"));
		if (Gp_system_config->d3d_wipe_buffer_cnt == 1)
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- ワイプバッファを１枚に減らします。"));
		if (Gp_system_config->ds_play_silent_sound_flag)
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- バックグラウンドで無音を再生し続けます。"));
		if (Gp_system_config->movie_type == TNM_MOVIE_PLAY_TYPE_MCI)
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("- MCI でムービーを再生します。"));
	}

	return true;
}

// ****************************************************************
// ファイルセーブ（既読データ）
// ================================================================
bool tnm_save_global_on_file_sub_read_data()
{
	C_tnm_save_stream read_data;

	if (!Gp_lexer->get_cut_ptr())
		return true;	// エラーではない

	// シーン名を含めてセーブしてみよう。
	int scn_cnt = Gp_lexer->get_scn_cnt();
	for (int s = 0; s < scn_cnt; s++)	{
		TSTR scn_name = Gp_lexer->get_scn_name(s);
		read_data.save(scn_name);						// シーン名
		read_data.save((*Gp_read_flag)[s].size());		// 既読フラグの個数
		read_data.save((*Gp_read_flag)[s].get(), (int)(*Gp_read_flag)[s].size());
	}

	// 既読データの暗号化
	tnm_pack_buffer(read_data.get_buffer());

	// ヘッダの構築
	S_tnm_read_save_header header;
	header.major_version = 1;
	header.minor_version = 0;
	header.read_data_size = (int)read_data.size();
	header.scn_cnt = scn_cnt;

	// 全書き込みデータを結合する
	BUFFER total_data;
	total_data.insert(total_data.end(), (BYTE *)&header, (BYTE *)&header + sizeof(header));
	total_data.insert(total_data.end(), read_data.begin(), read_data.end());

	// パスを求めてフォルダを作成する
	TSTR read_save_file_path = Gp_dir->get_read_save_file_path();
	create_dir_from_file_path(read_save_file_path);
	if (!tnm_save_to_file(read_save_file_path, total_data))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("既読データの保存に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	// 成功
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("既読データを保存しました。"));

	return true;
}

// ****************************************************************
// ファイルロード（既読データ）
// ================================================================
bool tnm_load_global_on_file_sub_read_data()
{
	C_tnm_save_stream read_data;

	TSTR file_path = Gp_dir->get_read_save_file_path();

	// ファイルの存在判定
	if (file_get_type(file_path) != 1)
		return true;			// ファイルがない場合は何もしない

	// ファイルを開く
	C_file file;
	if (!file.open(file_path, _T("rb")))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("既読ファイルが開けませんでした。\n") + get_last_error_msg());
		return false;			// 読めなかったのでここで終了
	}

	// ファイルを読み込む
	S_tnm_read_save_header header;
	file.read(&header, sizeof(header));
	read_data.resize(header.read_data_size);
	file.read(read_data.get(), header.read_data_size);

	// 既読データを解凍
	tnm_unpack_buffer(read_data.get_buffer());

	// シークをリセット
	read_data.set_seek(0);

	// 読み込み開始
	for (int i = 0; i < header.scn_cnt; i++)	{
		TSTR scn_name;
		read_data.load(scn_name);							// シーン名
		int save_flag_cnt = read_data.load_ret<int>();		// 既読フラグの個数
		int scn_no = Gp_lexer->get_scn_no(scn_name);
		if (scn_no >= 0)	{
			int real_flag_cnt = Gp_lexer->get_read_flag_cnt(scn_no);

			// セーブした既読フラグと実際の既読フラグの個数が等しければ適用
			if (save_flag_cnt == real_flag_cnt)	{
				read_data.load((*Gp_read_flag)[scn_no].get(), save_flag_cnt);
			}
			// サイズが違えば読み飛ばす
			else	{
				read_data.add_seek(save_flag_cnt);
			}
		}
		// シーンがなければ読み飛ばす
		else	{
			read_data.add_seek(save_flag_cnt);
		}
	}

	// 成功
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("既読データを読み込みました。"));

	return true;
}

// ****************************************************************
// ファイルセーブ（環境設定データ）
// ================================================================
bool tnm_save_global_on_file_sub_config_data()
{
	// コンフィグデータ
	C_tnm_save_stream config_data;
	Gp_eng->save_config(config_data);

	// 圧縮する
	tnm_pack_buffer(config_data.get_buffer());

	// ヘッダを作成する
	S_tnm_config_save_header config_header;
	config_header.major_version = 1;
	config_header.minor_version = 3;
	config_header.config_data_size = (int)config_data.size();

	// 全書き込みデータを統合する
	BUFFER config_total_data;
	config_total_data.insert(config_total_data.end(), (BYTE *)&config_header, (BYTE *)&config_header + sizeof(config_header));
	config_total_data.insert(config_total_data.end(), config_data.begin(), config_data.end());

	// パスを求めてフォルダを作成する
	TSTR config_save_file_path = Gp_dir->get_config_save_file_path();
	create_dir_from_file_path(config_save_file_path);
	if (!tnm_save_to_file(config_save_file_path, config_total_data))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("環境設定データの保存に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	// 成功
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("環境設定データを保存しました。"));

	return true;
}

// ****************************************************************
// ファイルロード（環境設定データ）
// ================================================================
bool tnm_load_global_on_file_sub_config_data()
{
	// 環境設定セーブのパスを求める
	TSTR config_save_file_path = Gp_dir->get_config_save_file_path();

	// ファイルの存在判定
	if (file_get_type(config_save_file_path) == 1)	{

		C_tnm_save_stream config_data;

		// ファイルを開く
		C_file file;
		if (!file.open(config_save_file_path, _T("rb")))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("環境設定データが開けませんでした。\n") + get_last_error_msg());
			return false;			// 読めなかったのでここで終了
		}
		
		// データを読み込む
		S_tnm_config_save_header config_header;
		file.read(&config_header, sizeof(config_header));
		if (config_header.major_version == 1 && config_header.minor_version >= 2)
		{
			config_data.resize(config_header.config_data_size);
			file.read(config_data.get(), (int)config_data.size());
			file.close();

			// 解凍する
			tnm_unpack_buffer(config_data.get_buffer());

			// シークをリセット
			config_data.set_seek(0);

			// 成功
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("環境設定データを読み込みました。"));

			// エンジン本体を呼び出す
			Gp_eng->load_config(config_data, config_header.major_version, config_header.minor_version);
		}
		else
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("環境設定データのバージョンが古いです。\nセーブデータを削除してから再度ゲームを起動して下さい。"));
			return false;			// 読めなかったのでここで終了
		}
	}

	return true;
}

// ****************************************************************
// ファイルセーブ（ローカル）
// ================================================================

// ファイルにセーブ
bool tnm_save_local_on_file(int save_no)
{
	// セーブデータが空なら何もしない
	if (Gp_eng->m_local_save.save_stream.empty())
		return false;

	// 範囲チェック
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// キャッシュをクリアする
	Gp_eng->m_save_cache.clear_cache(save_no);

	// ローカル設定データを保存
	Gp_eng->save_local_ex();

	// ストリームにデータを構築していく
	C_stream data_stream;
	{
		// ローカルセーブデータ
		data_stream.push(Gp_eng->m_local_save.save_id);
		data_stream.push_str_len(Gp_eng->m_local_save.append_dir);
		data_stream.push_str_len(Gp_eng->m_local_save.append_name);
		data_stream.push_str_len(Gp_eng->m_local_save.save_scene_title);
		data_stream.push_str_len(Gp_eng->m_local_save.save_msg);
		data_stream.push_str_len(Gp_eng->m_local_save.save_full_msg);
		data_stream.push(Gp_eng->m_local_save.save_stream.size());
		data_stream.push(Gp_eng->m_local_save.save_stream.get(), Gp_eng->m_local_save.save_stream.size());
		data_stream.push(Gp_eng->m_local_save.save_stream_ex.size());
		data_stream.push(Gp_eng->m_local_save.save_stream_ex.get(), Gp_eng->m_local_save.save_stream_ex.size());

		// 選択肢セーブデータ
		data_stream.push(Gp_eng->m_sel_save.map.size());
		for (std::map<S_tid, BSP<S_tnm_local_save>>::iterator itr = Gp_eng->m_sel_save.map.begin(); itr != Gp_eng->m_sel_save.map.end(); ++itr)	{

			data_stream.push(itr->second->save_id);
			data_stream.push_str_len(itr->second->append_dir);
			data_stream.push_str_len(itr->second->append_name);
			data_stream.push_str_len(itr->second->save_scene_title);
			data_stream.push_str_len(itr->second->save_msg);
			data_stream.push_str_len(itr->second->save_full_msg);
			data_stream.push(itr->second->save_stream.size());
			data_stream.push(itr->second->save_stream.get(), itr->second->save_stream.size());
			data_stream.push(itr->second->save_stream_ex.size());
			data_stream.push(itr->second->save_stream_ex.get(), itr->second->save_stream_ex.size());
		}
	}

	// データを圧縮
	BUFFER data_buffer_angou = data_stream.get_buffer();

	tnm_pack_buffer(data_buffer_angou);

	// 日時を取得
	SYSTEMTIME system_time;
	GetLocalTime(&system_time);
	
	// ヘッダを構築（ver 1.0）
	S_tnm_save_header header = {0};
	header.major_version = 1;
	header.minor_version = 0;
	header.year = system_time.wYear;
	header.month = system_time.wMonth;
	header.day = system_time.wDay;
	header.weekday = system_time.wDayOfWeek;
	header.hour = system_time.wHour;
	header.minute = system_time.wMinute;
	header.second = system_time.wSecond;
	header.millisecond = system_time.wMilliseconds;
	ZeroMemory(header.append_dir, sizeof(header.append_dir));		// アペンドディレクトリ
	_tcsncpy_s(header.append_dir, TNM_SAVE_APPEND_DIR_MAX_LEN, Gp_eng->m_local_save.append_dir.c_str(), TNM_SAVE_APPEND_DIR_MAX_LEN - 1);
	ZeroMemory(header.append_name, sizeof(header.append_name));		// アペンド名
	_tcsncpy_s(header.append_name, TNM_SAVE_APPEND_NAME_MAX_LEN, Gp_eng->m_local_save.append_name.c_str(), TNM_SAVE_APPEND_NAME_MAX_LEN - 1);
	ZeroMemory(header.title, sizeof(header.title));					// タイトル
	_tcsncpy_s(header.title, TNM_SAVE_TITLE_MAX_LEN, Gp_eng->m_local_save.save_scene_title.c_str(), TNM_SAVE_TITLE_MAX_LEN - 1);
	ZeroMemory(header.message, sizeof(header.message));				// メッセージ
	_tcsncpy_s(header.message, TNM_SAVE_MESSAGE_MAX_LEN, Gp_eng->m_local_save.save_msg.c_str(), TNM_SAVE_MESSAGE_MAX_LEN - 1);
	ZeroMemory(header.full_message, sizeof(header.full_message));				// フルメッセージ
	_tcsncpy_s(header.full_message, TNM_SAVE_FULL_MESSAGE_MAX_LEN, Gp_eng->m_local_save.save_full_msg.c_str(), TNM_SAVE_FULL_MESSAGE_MAX_LEN - 1);
	ZeroMemory(header.comment, sizeof(header.comment));				// コメント１（予約）
	ZeroMemory(header.comment2, sizeof(header.comment2));			// コメント２（予約）
	ZeroMemory(header.flag, sizeof(int) * TNM_SAVE_FLAG_MAX_CNT);	// フラグ（予約）
	header.data_size = (int)data_buffer_angou.size();				// データサイズ（暗号化済み）

	// 全書き込みデータを結合する
	BUFFER total_data;
	total_data.insert(total_data.end(), (BYTE *)&header, (BYTE *)&header + sizeof(header));
	total_data.insert(total_data.end(), data_buffer_angou.begin(), data_buffer_angou.end());

	// パスを求めてフォルダを作成する
	TSTR save_path = Gp_dir->get_local_save_file_path(save_no);
	if (!create_dir_from_file_path(save_path))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("セーブフォルダの作成に失敗しました。\n") + get_last_error_msg());
		return false;
	}
	if (!tnm_save_to_file(save_path, total_data))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("セーブデータの保存に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	// 成功
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("セーブデータを保存しました。"));

	// セーブサムネイルを使わない場合はここで終了
	if (!Gp_ini->use_save_thumb)	{
		return true;
	}

	// 古いセーブサムネイルを削除する
	TSTR thumb_path = Gp_dir->get_save_thumb_file_path(save_no);
	if (file_get_type(thumb_path) == 1)	{
		file_set_read_only(thumb_path, false);
		if (!filesys_delete_file(thumb_path))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("古いセーブサムネイルが削除できませんでした。\n（データのセーブは正常に行われました。）\n") + get_last_error_msg());
			return false;
		}
	}

	// セーブサムネイルが作られていない場合はエラー
	if (!Gp_wnd->m_capture_texture_for_save_thumb)	{
		tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("セーブサムネイルが作られていません。"));
		return false;
	}

	// ================================================================
	// キャプチャーバッファからピクセルデータを取り出します。
	// ================================================================
	BSP<C_d3d_texture> p_texture = Gp_wnd->m_capture_texture_for_save_thumb;
	int capture_prior = Gp_global->capture_prior;
	int width = p_texture->get_width();
	int height = p_texture->get_height();
	D3DLOCKED_RECT rect;
	ARRAY<BYTE> bmp_buffer;

	// png の場合
	if (Gp_ini->save_thumb_type == TNM_SAVE_THUMB_TYPE_PNG)	{

		// テクスチャをロックする
		if (!p_texture->lock_rect(0, &rect, NULL, 0))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("サムネイル用テクスチャのロックに失敗しました。\n") + get_last_error_msg());
			return false;
		}

		// png に書きこむ
		change_extension(thumb_path, _T("png"));
		if (!tnm_create_png_from_texture_and_save_to_file(thumb_path, width, height, &rect, false))	{		// アルファ値を使わない
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("セーブサムネイルをファイルに保存できませんでした。\n") + get_last_error_msg());
			return false;
		}

		// テクスチャをアンロックする
		if (!p_texture->unlock_rect(0))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("サムネイル用テクスチャのアンロックに失敗しました。\n") + get_last_error_msg());
			return false;
		}
	}
	// bmp の場合
	else	{

		// テクスチャをロックする
		if (!p_texture->lock_rect(0, &rect, NULL, 0))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("サムネイル用テクスチャのロックに失敗しました。\n") + get_last_error_msg());
			return false;
		}

		// bmp に落とし込む
		if (!tnm_create_bmp_from_texture(bmp_buffer, width, height, &rect))
			return false;

		// テクスチャをアンロックする
		if (!p_texture->unlock_rect(0))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("サムネイル用テクスチャのアンロックに失敗しました。\n") + get_last_error_msg());
			return false;
		}

		// ファイルに保存
		if (!tnm_save_to_file(thumb_path, bmp_buffer))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("セーブサムネイルをファイルに保存できませんでした。\n") + get_last_error_msg());
			return false;
		}
	}

	// 成功
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("セーブサムネイルを保存しました。"));

	return true;
}

// ****************************************************************
// ファイルからロード（ローカル）
// ================================================================
bool tnm_load_local_on_file(int save_no)
{
	// 範囲チェック
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)	{
		return false;
	}

	// セーブデータをクリアする
	Gp_eng->m_local_save.save_id = S_tid();
	Gp_eng->m_local_save.append_dir.clear();
	Gp_eng->m_local_save.append_name.clear();
	Gp_eng->m_local_save.save_scene_title.clear();
	Gp_eng->m_local_save.save_msg.clear();
	Gp_eng->m_local_save.save_full_msg.clear();
	Gp_eng->m_local_save.save_stream.clear();
	Gp_eng->m_local_save.save_stream_ex.clear();
	Gp_eng->m_sel_save.list.clear();
	Gp_eng->m_sel_save.map.clear();

	// パスを求める
	TSTR save_path = Gp_dir->get_local_save_file_path(save_no);

	// ファイルを開く
	C_file file;
	if (!file.open(save_path, _T("rb")))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("セーブデータが読み込めませんでした。\n") + get_last_error_msg());
		return false;
	}

	// ファイルから読み込む
	S_tnm_save_header header;
	file.read(&header, sizeof(header));
	Gp_eng->m_local_save.append_dir = header.append_dir;
	Gp_eng->m_local_save.append_name = header.append_name;
	Gp_eng->m_local_save.save_scene_title = header.title;
	Gp_eng->m_local_save.save_msg = header.message;
	Gp_eng->m_local_save.save_full_msg = header.full_message;

	C_stream data_stream;
	data_stream.resize(header.data_size);
	file.read(data_stream.get(), header.data_size);
	tnm_unpack_buffer(data_stream.get_buffer());

	// ストリームからデータを構築していく
	{
		data_stream.set_seek();

		// ローカルセーブデータ
		Gp_eng->m_local_save.save_id = data_stream.pop_ret<S_tid>();
		Gp_eng->m_local_save.append_dir = data_stream.pop_str_len_ret<TSTR>();
		Gp_eng->m_local_save.append_name = data_stream.pop_str_len_ret<TSTR>();
		Gp_eng->m_local_save.save_scene_title = data_stream.pop_str_len_ret<TSTR>();
		Gp_eng->m_local_save.save_msg = data_stream.pop_str_len_ret<TSTR>();
		Gp_eng->m_local_save.save_full_msg = data_stream.pop_str_len_ret<TSTR>();
		int local_save_size = data_stream.pop_ret<int>();
		Gp_eng->m_local_save.save_stream.resize(local_save_size);
		data_stream.pop(Gp_eng->m_local_save.save_stream.get(), local_save_size);
		int local_save_ex_size = data_stream.pop_ret<int>();
		Gp_eng->m_local_save.save_stream_ex.resize(local_save_ex_size);
		data_stream.pop(Gp_eng->m_local_save.save_stream_ex.get(), local_save_ex_size);

		// 選択肢データ
		int sel_save_cnt = data_stream.pop_ret<int>();
		for (int i = 0; i < sel_save_cnt; i++)	{

			BSP<S_tnm_local_save> sel_save(new S_tnm_local_save);
			sel_save->save_id = data_stream.pop_ret<S_tid>();
			sel_save->append_dir = data_stream.pop_str_len_ret<TSTR>();
			sel_save->append_name = data_stream.pop_str_len_ret<TSTR>();
			sel_save->save_scene_title = data_stream.pop_str_len_ret<TSTR>();
			sel_save->save_msg = data_stream.pop_str_len_ret<TSTR>();
			sel_save->save_full_msg = data_stream.pop_str_len_ret<TSTR>();
			int sel_save_size = data_stream.pop_ret<int>();
			sel_save->save_stream.resize(sel_save_size);
			data_stream.pop(sel_save->save_stream.get(), sel_save_size);
			int sel_save_ex_size = data_stream.pop_ret<int>();
			sel_save->save_stream_ex.resize(sel_save_ex_size);
			data_stream.pop(sel_save->save_stream_ex.get(), sel_save_ex_size);

			Gp_eng->m_sel_save.map[sel_save->save_id] = sel_save;
		}
	}

	// ファイルを閉じる
	file.close();

	// 成功
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("セーブデータを読み込みました。"));

	return true;
}

// ****************************************************************
// ファイルを削除
// ================================================================
bool tnm_delete_save_file(int save_no)
{
	// 範囲チェック
	if (save_no < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no)
		return false;

	// キャッシュからチェック
	if (!Gp_eng->m_save_cache.check_file(save_no))
		return false;

	// キャッシュをクリアする
	Gp_eng->m_save_cache.clear_cache(save_no);

	// パスを求める
	TSTR save_path = Gp_dir->get_local_save_file_path(save_no);
	TSTR thumb_path = Gp_dir->get_save_thumb_file_path(save_no);

	// ファイルを消去する
	file_set_read_only(save_path, false);
	if (!filesys_delete_file(save_path))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("セーブファイルの削除に失敗しました。\n") + get_last_error_msg());
		return false;
	}

	// サムネイルを消去する
	if (Gp_ini->use_save_thumb)	{
		file_set_read_only(thumb_path, false);
		if (!filesys_delete_file(thumb_path))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("セーブサムネイルの削除に失敗しました。\n") + get_last_error_msg());
			return false;
		}
	}

	// 成功
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("セーブファイルを削除しました。"));

	return true;
}

// ****************************************************************
// ファイルをコピー
// ================================================================
bool tnm_copy_save_file(int save_no_from, int save_no_to)
{
	// 範囲チェック
	if (save_no_from < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no_from)
		return false;
	if (save_no_to < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no_to)
		return false;

	// 同じファイルなら失敗
	if (save_no_from == save_no_to)
		return false;

	// キャッシュをクリアする
	Gp_eng->m_save_cache.clear_cache(save_no_to);

	// パスを求める
	TSTR save_path_from = Gp_dir->get_local_save_file_path(save_no_from);
	TSTR save_path_to = Gp_dir->get_local_save_file_path(save_no_to);
	TSTR thumb_path_from = Gp_dir->get_save_thumb_file_path(save_no_from);
	TSTR thumb_path_to = Gp_dir->get_save_thumb_file_path(save_no_to);

	// ファイル０をファイル１にコピー
	file_set_read_only(save_path_to, false);
	if (!filesys_copy_file(save_path_from, save_path_to))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("ファイルのコピーに失敗しました。%d → %d"), save_no_from, save_no_to) + _T("\n") + get_last_error_msg());
		return false;
	}

	// サムネイル０をサムネイル１にコピー
	if (Gp_ini->use_save_thumb)	{
		file_set_read_only(thumb_path_to, false);
		if (!filesys_copy_file(thumb_path_from, thumb_path_to))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("サムネイルのコピーに失敗しました。%d → %d"), save_no_from, save_no_to) + _T("\n") + get_last_error_msg());
			return false;
		}
	}

	return true;
}

// ****************************************************************
// ファイルを交換
// ================================================================
bool tnm_change_save_file(int save_no_0, int save_no_1)
{
	// 範囲チェック
	if (save_no_0 < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no_0)
		return false;
	if (save_no_1 < 0 || Gp_ini->save_cnt + Gp_ini->quick_save_cnt + Gp_ini->end_save_cnt <= save_no_1)
		return false;

	// 同じファイルなら失敗
	if (save_no_0 == save_no_1)
		return false;

	// 存在を先に判定しておく
	bool save_exist_0 = Gp_eng->m_save_cache.check_file(save_no_0);
	bool save_exist_1 = Gp_eng->m_save_cache.check_file(save_no_1);

	// キャッシュをクリアする
	Gp_eng->m_save_cache.clear_cache(save_no_0);
	Gp_eng->m_save_cache.clear_cache(save_no_1);

	// パスを求める
	TSTR save_path_0 = Gp_dir->get_local_save_file_path(save_no_0);
	TSTR save_path_1 = Gp_dir->get_local_save_file_path(save_no_1);
	TSTR thumb_path_0 = Gp_dir->get_save_thumb_file_path(save_no_0);
	TSTR thumb_path_1 = Gp_dir->get_save_thumb_file_path(save_no_1);

	if (false);
	// ファイル０のみがある場合
	else if (save_exist_0 && !save_exist_1)	{

		// ファイル０をファイル１にリネーム
		if (!filesys_rename_file(save_path_0, save_path_1))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("ファイル名の変更に失敗しました。%d → %d"), save_no_0, save_no_1) + _T("\n") + get_last_error_msg());
			return false;
		}
		// サムネイル０をサムネイル１にリネーム
		if (Gp_ini->use_save_thumb)	{
			if (!filesys_rename_file(thumb_path_0, thumb_path_1))	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("サムネイル名の変更に失敗しました。%d → %d"), save_no_0, save_no_1) + _T("\n") + get_last_error_msg());
				return false;
			}
		}

		// 成功
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("ファイル名を変更しました。%d → %d"), save_no_0, save_no_1));
	}
	// ファイル１のみがある場合
	else if (!save_exist_0 && save_exist_1)	{

		// ファイル１をファイル０にリネーム
		if (!filesys_rename_file(save_path_1, save_path_0))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("ファイル名の変更に失敗しました。%d ← %d"), save_no_0, save_no_1) + _T("\n") + get_last_error_msg());
			return false;
		}
		// サムネイル１をサムネイル０にリネーム
		if (Gp_ini->use_save_thumb)	{
			if (!filesys_rename_file(thumb_path_1, thumb_path_0))	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("サムネイル名の変更に失敗しました。%d ← %d"), save_no_0, save_no_1) + _T("\n") + get_last_error_msg());
				return false;
			}
		}

		// 成功
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("ファイル名を変更しました。%d ← %d"), save_no_0, save_no_1));
	}
	// ファイル０もファイル１もない場合
	else if (!save_exist_0 && !save_exist_1)	{

		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("交換するファイルが共に存在しませんでした。")));
		return false;
	}
	// ファイル０もファイル１もある場合
	else if (save_exist_0 && save_exist_1)	{

		TSTR tmp_save_path_0 = save_path_0 + _T("__tmp");
		TSTR tmp_save_path_1 = save_path_1 + _T("__tmp");
		TSTR tmp_thumb_path_0 = thumb_path_0 + _T("__tmp");
		TSTR tmp_thumb_path_1 = thumb_path_1 + _T("__tmp");

		// テンポラリファイルを削除する
		filesys_delete_file(tmp_save_path_0);
		filesys_delete_file(tmp_save_path_1);
		filesys_delete_file(tmp_thumb_path_0);
		filesys_delete_file(tmp_thumb_path_1);

		// ファイル０をテンポラリファイルにリネーム
		if (!filesys_rename_file(save_path_0, tmp_save_path_0))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("ファイル名の変更に失敗しました。%d → tmp"), save_no_0) + _T("\n") + get_last_error_msg());
			return false;
		}

		// ファイル０を元に戻す
		filesys_rename_file(tmp_save_path_0, save_path_0);

		// ファイル１をテンポラリファイルにリネーム
		if (!filesys_rename_file(save_path_1, tmp_save_path_1))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("ファイル名の変更に失敗しました。%d → tmp"), save_no_1) + _T("\n") + get_last_error_msg());
			return false;
		}

		// ファイル１を元に戻す
		filesys_rename_file(tmp_save_path_1, save_path_1);

		// サムネイルを使う場合の処理
		if (Gp_ini->use_save_thumb)	{

			// サムネイル０をテンポラリファイルにリネーム
			if (!filesys_rename_file(thumb_path_0, tmp_thumb_path_0))	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("サムネイル名の変更に失敗しました。%d → tmp"), save_no_0) + _T("\n") + get_last_error_msg());
				return false;
			}

			// サムネイル０を元に戻す
			filesys_rename_file(tmp_thumb_path_0, thumb_path_0);

			// サムネイル１をテンポラリファイルにリネーム
			if (!filesys_rename_file(thumb_path_1, tmp_thumb_path_1))	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("サムネイル名の変更に失敗しました。%d → tmp"), save_no_1) + _T("\n") + get_last_error_msg());
				return false;
			}

			// サムネイル１を元に戻す
			filesys_rename_file(tmp_thumb_path_1, thumb_path_1);
		}

		// ★これで、両方のファイルがリネーム可能だと言うことが分かった！

		// ファイル０をテンポラリファイルにリネーム
		filesys_rename_file(save_path_0, tmp_save_path_0);
		// ファイル１をテンポラリファイルにリネーム
		filesys_rename_file(save_path_1, tmp_save_path_1);
		// ファイル０をファイル１にリネーム
		filesys_rename_file(tmp_save_path_0, save_path_1);
		// ファイル１をファイル０にリネーム
		filesys_rename_file(tmp_save_path_1, save_path_0);

		// サムネイルを使う場合の処理
		if (Gp_ini->use_save_thumb)	{

			// サムネイル０をテンポラリファイルにリネーム
			filesys_rename_file(thumb_path_0, tmp_thumb_path_0);
			// サムネイル１をテンポラリファイルにリネーム
			filesys_rename_file(thumb_path_1, tmp_thumb_path_1);
			// サムネイル０をファイル１にリネーム
			filesys_rename_file(tmp_thumb_path_0, thumb_path_1);
			// サムネイル１をファイル０にリネーム
			filesys_rename_file(tmp_thumb_path_1, thumb_path_0);
		}

		// 成功
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, str_format(_T("ファイル名を変更しました。%d ⇔ %d"), save_no_0, save_no_1));
	}

	return true;
}

