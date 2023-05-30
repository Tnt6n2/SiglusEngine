#pragma		once

#include	"elm_prop.h"
#include	"elm_list.h"
#include	"elm_counter.h"

class	C_tnm_save_stream;

// ****************************************************************
// フレームアクション
// ================================================================
class C_elm_frame_action : public C_tnm_element
{
public:

	void	init(S_element element, CTSTR& name, S_element target);	// 初期化
	void	reinit(bool finish_flag);								// 再初期化
	void	finish();												// 終了
	void	save(C_tnm_save_stream& stream);						// セーブ
	void	load(C_tnm_save_stream& stream);						// ロード
	void	copy(const C_elm_frame_action* src, bool end_flag);		// コピー
	void	update_time(int past_game_time, int past_real_time);	// 時間を更新
	void	frame();												// フレーム処理

	// パラメータを設定
	void	set_param(int end_time, int real_flag, CTSTR& scn_name, CTSTR& user_cmd_name, C_tnm_prop_list& arg_list);
	void	do_action();											// アクションを実行
	void	do_action(int scn_no, int user_cmd_no);					// アクションを実行
	bool	is_end_action()	{	return m_end_action_flag;	}		// エンドアクション中かを判定

	S_element			m_target;			// ターゲット
	int					m_scn_no;			// シーン番号
	int					m_cmd_no;			// コマンド番号
	int					m_end_time;			// 終了時刻
	TSTR				m_scn_name;			// シーン名
	TSTR				m_cmd_name;			// コマンド名
	C_tnm_prop_list		m_arg_list;			// 引数リスト
	C_elm_counter		m_counter;			// カウンタ
	bool				m_end_action_flag;	// エンドアクション中フラグ

private:
	void	restruct();						// 再構築
};

// ****************************************************************
// フレームアクションリスト
// ================================================================
class C_elm_frame_action_list : public C_elm_list_ex<C_elm_frame_action>
{
public:
	void	init(S_element element, CTSTR& name, int size, bool extend_enable, S_element target);	// 初期化
	void	reinit(bool finish_flag, bool restruct_flag);											// 再初期化
	void	finish();																				// 終了

protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end, bool restruct_flag);		// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード

private:
	S_element	m_target;
};
