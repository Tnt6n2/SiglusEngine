#pragma		once

#include	"data/tnm_element.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_flag.h"

class	C_tnm_save_stream;

// ****************************************************************
// コールのタイプ
// ================================================================
enum E_tnm_call_type
{
	E_tnm_call_type_none = 0,
	E_tnm_call_type_gosub = 1,
	E_tnm_call_type_farcall = 2,
	E_tnm_call_type_user_cmd = 3,
};

// ****************************************************************
// コール：セーブ構造体
// ================================================================
struct S_elm_call_save_struct
{
	TSTR	lexer_scn_name;
	int		lexer_line_no;
	int		lexer_prg_cntr;
};

// ****************************************************************
// コール
// ================================================================
class C_elm_call : public C_tnm_element
{
public:
	void	init(S_element element, CTSTR& name);
	void	reinit();
	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);

	// コールプロパティ
	C_tnm_user_call_prop_list	user_prop_list;
	C_elm_int_list				L;
	C_elm_str_list				K;

	// その他の情報
	E_tnm_call_type		call_type;
	bool				excall_flag;
	bool				frame_action_flag;
	int					ret_form_code;
	int					arg_cnt;		// 引数の個数（フレームアクション時のみ設定されます）
	int					scn_no;			// ユーザコマンドのシーン番号（フレームアクション時のみ設定されます）
	int					cmd_no;			// ユーザコマンドのコマンド番号（フレームアクション時のみ設定されます）

	S_elm_call_save_struct	m_call_save;		// プログラムカウンタなど、外部が持っているデータを保存するための構造体

	// デバッグ情報
	int		src_scn_no;
	int		dst_scn_no;
	int		dst_label_no;
};

// ****************************************************************
// コールリスト
// ================================================================
class C_elm_call_list : public C_elm_list<C_elm_call>
{
public:
	void	init(S_element element, int form, CTSTR& name, int size, bool extend_enable, C_tnm_element* p_parent);	// 初期化
	void	reinit();
	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);

	int		get_call_cnt()	{	return m_call_cnt;	}
	void	set_call_cnt(int call_cnt);
	void	add_call();
	void	sub_call();

protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end);							// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード

private:
	int		m_call_cnt;		// 高速化のため、コールの数を自力で制御します。
};

