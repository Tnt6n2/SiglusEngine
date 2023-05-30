#pragma		once

// ****************************************************************
// サムネイルテーブル
// ================================================================
class C_tnm_thumb_table
{
public:
	bool	create_table();			// サムネイルテーブルを初期化する
	TSTR	calc_thumb_file_name();	// 最終的に表示するサムネイルのファイル名を取得する

	std::map<TSTR, TSTR>&	get_table()	{	return m_thumb_table;	}

private:
	std::map<TSTR, TSTR>	m_thumb_table;
};


