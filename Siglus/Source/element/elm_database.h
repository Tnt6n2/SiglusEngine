#pragma		once

#include	"element/elm_list.h"

struct S_tnm_database_row_header
{
	int call_no;		// 呼出し番号
};

struct S_tnm_database_column_header
{
	int call_no;		// 呼出し番号
	int data_type;		// データタイプ
};

struct S_tnm_database_info
{
	int row_cnt;			// 行数
	int column_cnt;			// 列数
	S_tnm_database_row_header*		p_row_header;		// 行ヘッダテーブルへのポインタ
	S_tnm_database_column_header*	p_column_header;	// 列ヘッダテーブルへのポインタ
	DWORD* p_data;			// データテーブルへのポインタ
	char* p_str;			// 文字列データテーブルへのポインタ
};

// ****************************************************************
// エレメント：データベース
// ================================================================
class C_elm_database : public C_tnm_element
{
public:
	C_elm_database();

	void	init(S_element element, CTSTR& name, int ini_no);	// 初期化

	bool	load_dbs(CTSTR& file_path);
	bool	get_data(int item_call_no, int column_call_no, int* ret_value);
	bool	get_data(int item_call_no, int column_call_no, TSTR& ret_value);
	int		check_item_no(int item_call_no);
	int		check_column_no(int column_call_no);
	int		find_num(int column_call_no, int num);
	int		find_str(int column_call_no, CTSTR& str);
	int		find_str_real(int column_call_no, CTSTR& str);

private:
	TSTR	get_str(int str_offset);
	int		get_item_no(int item_call_no);
	int		get_column_no(int column_call_no);

	int					m_type;
	ARRAY<BYTE>			m_data;
	S_tnm_database_info	m_info;
};

// ****************************************************************
// エレメント：データベースリスト
// ================================================================
class C_elm_database_list : public C_elm_list<C_elm_database>
{
protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end);							// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード
};
