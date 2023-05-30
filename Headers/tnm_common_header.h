#pragma		once

// ****************************************************************
// Siglus のバージョン
// ================================================================
CTSTR	SIGLUS_VERSION = _T("1.1.134.0");

// ****************************************************************
// Z ラベルの個数
// ================================================================
const int	TNM_Z_LABEL_CNT = 1000;

// ****************************************************************
// Gameexe.dat：ヘッダ構造体
// ================================================================
struct S_tnm_gameexe_dat_header
{
	int		version;
	int		exe_angou_mode;
};

// ****************************************************************
// シーンデータ：コマンドラベル構造体
// ================================================================
struct S_tnm_scn_cmd_label
{
	int		cmd_id;
	int		offset;
};

// ****************************************************************
// シーンデータ：シーンプロパティ構造体
// ================================================================
struct S_tnm_scn_scn_prop
{
	int		form;
	int		size;
};

// ****************************************************************
// シーンデータ：シーンコマンドラベル構造体
// ================================================================
struct S_tnm_scn_scn_cmd
{
	int		offset;
};

// ****************************************************************
// シーンデータ：既読フラグ構造体
// ================================================================
struct S_tnm_scn_read_flag
{
	int		line_no;	// 行番号
};

// ****************************************************************
// シーンデータ：ヘッダ構造体
// ================================================================
struct S_tnm_scn_header
{
	int		header_size;					// ヘッダサイズ
	int		scn_ofs;						// シーン
	int		scn_size;						// シーンのサイズ
	int		str_index_list_ofs;				// 文字列インデックスリスト
	int		str_index_cnt;					// 文字列インデックスの個数
	int		str_list_ofs;					// 文字列リスト
	int		str_cnt;						// 文字列の個数
	int		label_list_ofs;					// ラベルリスト
	int		label_cnt;						// ラベルの個数
	int		z_label_list_ofs;				// Ｚラベルリスト
	int		z_label_cnt;					// Ｚラベルの個数
	int		cmd_label_list_ofs;				// コマンドラベルリスト
	int		cmd_label_cnt;					// コマンドラベルの個数
	int		scn_prop_list_ofs;				// シーンプロパティリスト
	int		scn_prop_cnt;					// シーンプロパティの個数
	int		scn_prop_name_index_list_ofs;	// シーンプロパティ名インデックスリスト
	int		scn_prop_name_index_cnt;		// シーンプロパティ名の個数
	int		scn_prop_name_list_ofs;			// シーンプロパティ名リスト
	int		scn_prop_name_cnt;				// シーンプロパティ名の個数
	int		scn_cmd_list_ofs;				// シーンコマンドリスト
	int		scn_cmd_cnt;					// シーンコマンドの個数
	int		scn_cmd_name_index_list_ofs;	// シーンコマンド名インデックスリスト
	int		scn_cmd_name_index_cnt;			// シーンコマンド名インデックスの個数
	int		scn_cmd_name_list_ofs;			// シーンコマンド名リスト
	int		scn_cmd_name_cnt;				// シーンコマンド名の個数
	int		call_prop_name_index_list_ofs;	// コールプロパティ名インデックスリスト
	int		call_prop_name_index_cnt;		// コールプロパティ名の個数
	int		call_prop_name_list_ofs;		// コールプロパティ名リスト
	int		call_prop_name_cnt;				// コールプロパティ名の個数
	int		namae_list_ofs;					// 【名前】リスト
	int		namae_cnt;						// 【名前】の個数
	int		read_flag_list_ofs;				// 既読フラグリスト
	int		read_flag_cnt;					// 既読フラグの個数
};

// ****************************************************************
// シーンデータ構造体
// ================================================================
struct S_tnm_scn_data
{
	S_tnm_scn_header			header;						// ヘッダ
	C_stream					scn;						// シーン
	ARRAY<C_index>				str_index_list;				// 文字列インデックスリスト
	ARRAY<TSTR>					str_list;					// 文字列リスト
	ARRAY<int>					label_list;					// ラベルリスト
	ARRAY<int>					z_label_list;				// Ｚラベルリスト
	ARRAY<S_tnm_scn_cmd_label>	cmd_label_list;				// コマンドラベルリスト（リンカ専用）
	ARRAY<S_tnm_scn_scn_prop>	scn_prop_list;				// シーンプロパティリスト
	ARRAY<C_index>				scn_prop_name_index_list;	// シーンプロパティ名インデックスリスト
	ARRAY<TSTR>					scn_prop_name_list;			// シーンプロパティ名リスト
	ARRAY<S_tnm_scn_scn_cmd>	scn_cmd_list;				// シーンコマンドリスト
	ARRAY<C_index>				scn_cmd_name_index_list;	// シーンコマンド名インデックスリスト
	ARRAY<TSTR>					scn_cmd_name_list;			// シーンコマンド名リスト
	ARRAY<C_index>				call_prop_name_index_list;	// コールプロパティ名インデックスリスト
	ARRAY<TSTR>					call_prop_name_list;		// コールプロパティ名リスト
	ARRAY<int>					namae_list;					// 【名前】リスト
	ARRAY<S_tnm_scn_read_flag>	read_flag_list;				// 既読フラグリスト
};

// ****************************************************************
// パックシーン：インクプロパティ構造体
// ================================================================
struct S_tnm_pack_scn_inc_prop
{
	int		form;
	int		size;
};

// ****************************************************************
// パックシーン：インクコマンド構造体
// ================================================================
struct S_tnm_pack_scn_inc_cmd
{
	int		scn_no;
	int		offset;
};

// ****************************************************************
// パックシーン：ヘッダ構造体
// ================================================================
struct S_tnm_pack_scn_header
{
	int		header_size;					// ヘッダ
	int		inc_prop_list_ofs;				// インクプロパティリストへのオフセット
	int		inc_prop_cnt;					// インクプロパティの個数
	int		inc_prop_name_index_list_ofs;	// インクプロパティ名リスト
	int		inc_prop_name_index_cnt;		// インクプロパティ名リスト
	int		inc_prop_name_list_ofs;			// インクプロパティ名リスト
	int		inc_prop_name_cnt;				// インクプロパティ名リスト
	int		inc_cmd_list_ofs;				// インクコマンドリストへのオフセット
	int		inc_cmd_cnt;					// インクコマンドの個数
	int		inc_cmd_name_index_list_ofs;	// インクコマンド名リストへのオフセット
	int		inc_cmd_name_index_cnt;			// インクコマンド名の個数
	int		inc_cmd_name_list_ofs;			// インクコマンド名リストへのオフセット
	int		inc_cmd_name_cnt;				// インクコマンド名の個数
	int		scn_name_index_list_ofs;		// シーン名インデックスリストへのオフセット
	int		scn_name_index_cnt;				// シーン名リストの個数
	int		scn_name_list_ofs;				// シーン名リストへのオフセット
	int		scn_name_cnt;					// シーン名リストの個数
	int		scn_data_index_list_ofs;		// シーンデータインデックスリストへのオフセット
	int		scn_data_index_cnt;				// シーンデータリストの個数
	int		scn_data_list_ofs;				// シーンデータリストへのオフセット
	int		scn_data_cnt;					// シーンデータリストの個数
	int		scn_data_exe_angou_mod;			// シーンデータの暗号化モード
	int		original_source_header_size;	// オリジナルソースのヘッダサイズ
};

// ****************************************************************
// パックシーンデータ構造体
// ================================================================
struct S_tnm_pack_scn_data
{
	S_tnm_pack_scn_header			header;						// ヘッダ
	ARRAY<S_tnm_pack_scn_inc_prop>	inc_prop_list;				// インクプロパティリスト
	ARRAY<C_index>					inc_prop_name_index_list;	// インクプロパティ名リスト
	ARRAY<TSTR>						inc_prop_name_list;			// インクプロパティ名リスト
	ARRAY<S_tnm_pack_scn_inc_cmd>	inc_cmd_list;				// インクコマンドリスト
	ARRAY<C_index>					inc_cmd_name_index_list;	// インクコマンド名リスト
	ARRAY<TSTR>						inc_cmd_name_list;			// インクコマンド名リスト
	ARRAY<C_index>					scn_name_index_list;		// シーン名リスト
	ARRAY<TSTR>						scn_name_list;				// シーン名リスト

	ARRAY<C_index>					nolzss_scn_data_index_list;		// シーンデータリスト
	ARRAY<BUFFER>					nolzss_scn_data_list;			// シーンデータリスト
	ARRAY<C_index>					lzss_scn_data_index_list;		// シーンデータリスト
	ARRAY<BUFFER>					lzss_scn_data_list;				// シーンデータリスト

	ARRAY<C_index>					noangou_scn_data_index_list;	// 暗号化なしシーンデータリスト
	ARRAY<BUFFER>					noangou_scn_data_list;			// 暗号化なしシーンデータリスト
	ARRAY<C_index>					angou_scn_data_index_list;		// 暗号化ありシーンデータリスト
	ARRAY<BUFFER>					angou_scn_data_list;			// 暗号化ありシーンデータリスト
};

// ****************************************************************
// サムネイルデータベースヘッダー
// ================================================================
struct S_tnm_thumbnail_database_header
{
	int		header_size;
	int		version;
	int		data_cnt;
};

