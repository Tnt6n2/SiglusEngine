#pragma		once

#include	"tnm_form_code.h"
#include	"element/elm_list.h"

class	C_tnm_save_stream;
class	C_tnm_prop;

// ****************************************************************
// プロパティリスト
// ================================================================
class C_tnm_prop_list : public C_elm_list<C_tnm_prop>
{
protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end);							// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード
};

// ****************************************************************
// プロパティ
// ================================================================
class C_tnm_prop
{
public:
	C_tnm_prop() : id(0), form(FM_INT), Int(0), exp_cnt(0)	{}

	void	init();								// 初期化
	void	reinit();							// 再初期化
	void	save(C_tnm_save_stream& stream);	// セーブ
	void	load(C_tnm_save_stream& stream);	// ロード

	int					id;				// ID（名前つき引数に使う）
	int					form;			// 型
	int					Int;			// 整数型の場合のデータ
	TSTR				str;			// 文字列型の場合データ
	S_element			element;		// 参照型の場合データ
	C_tnm_prop_list		exp_list;		// 式リストの場合のデータ
	int					exp_cnt;		// 式リストの場合のデータの個数
	boost::any			any;			// その他のデータ
};

