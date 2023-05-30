#pragma		once

#include	"data/tnm_sorter.h"

#include	"element/elm_group_def.h"
#include	"element/elm_list.h"

class	C_tnm_save_stream;
class	C_elm_object_list;

// ****************************************************************
// グループ
// ================================================================
//		ボタンのグループです。
// ================================================================

// グループの固定パラメータ
struct C_elm_group_def_param
{
	int				group_no;			// グループ番号
	S_element		target_object;		// ターゲットとなるオブジェクト
};

// グループの通常パラメータ
struct C_elm_group_param
{
	S_tnm_sorter	sorter;				// ソーター
	int				cancel_priority;	// キャンセルの優先度
	int				cancel_se_no;		// キャンセル時の効果音番号
	int				decided_button_no;	// 決定したボタン番号

	int				result;				// 結果。-1 = キャンセル、0 = none、1 = 決定
	int				result_button_no;	// 決定したボタン番号（v1 と少し仕様が違います）

	bool			doing;				// 稼働中
	bool			pause_flag;			// ポーズ中
	bool			wait_flag;			// ウェイトフラグ
	bool			cancel_flag;		// キャンセル可能フラグ
	S_element		target_object;		// ターゲットとなるオブジェクト
};

// グループの作業パラメータ
struct C_elm_group_work_param
{
	int			last_hit_button_no;
	int			last_pushed_button_no;
	int			hit_button_no;
	int			pushed_button_no;
};

// グループ
class C_elm_group : public C_tnm_element
{
public:

	// 初期化と作成処理
	void	init(S_element element, CTSTR& name, int group_no, S_element target_object);
	void	reinit();
	void	save(C_tnm_save_stream& stream);
	void	load(C_tnm_save_stream& stream);
	void	copy(C_elm_group* src);
	void	button_frame_init();

	// ソーターの取得／設定
	void			set_sorter(S_tnm_sorter sorter)	{	m_param.sorter = sorter;		}
	S_tnm_sorter	get_sorter()					{	return m_param.sorter;			}
	void			set_order(int value)			{	m_param.sorter.order = value;	}
	int				get_order()						{	return m_param.sorter.order;	}
	void			set_layer(int value)			{	m_param.sorter.layer = value;	}
	int				get_layer()						{	return m_param.sorter.layer;	}

	// 選択肢処理
	void	init_sel();
	void	start()							{	m_param.doing = true;	m_param.decided_button_no = TNM_GROUP_NOT_DECIDED;	}
	void	end()							{	m_param.doing = false;	m_param.decided_button_no = TNM_GROUP_NOT_DECIDED;	}
	void	set_wait_flag(bool flag)		{	m_param.wait_flag = flag;			}
	void	set_cancel_flag(bool flag)		{	m_param.cancel_flag = flag;			}
	void	set_cancel_se_no(int se_no)		{	m_param.cancel_se_no = se_no;		}
	void	set_cancel_priority(int prior)	{	m_param.cancel_priority = prior;	}
	void	decide(int button_no);
	void	cancel();
	bool	is_doing()					{	return m_param.doing && !m_param.pause_flag;				}

	// キャンセル処理
	void	cancel_check();

	// 選択肢関連のパラメータを取得
	int		get_hit_button_no()			{	return m_work.hit_button_no;								}
	int		get_last_hit_button_no()	{	return m_work.last_hit_button_no;							}
	int		get_pushed_button_no()		{	return m_work.pushed_button_no;								}
	int		get_last_pushed_button_no()	{	return m_work.last_pushed_button_no;						}
	bool	get_decided()				{	return m_param.decided_button_no >= 0;						}
	int		get_decided_button_no()		{	return m_param.decided_button_no;							}

	int		get_result()				{	return m_param.result;										}
	int		get_result_button_no()		{	return m_param.result_button_no;							}

	bool	get_canceled()				{	return m_param.decided_button_no == TNM_GROUP_CANCELED;		}
	bool	get_doing_flag()			{	return m_param.doing;										}
	bool	get_pause_flag()			{	return m_param.pause_flag;									}
	bool	get_wait_flag()				{	return m_param.wait_flag;									}
	bool	get_cancel_flag()			{	return m_param.cancel_flag;									}
	int		get_cancel_se_no()			{	return m_param.cancel_se_no;								}
	int		get_cancel_priority()		{	return m_param.cancel_priority;								}

	// 選択肢関連のパラメータを設定
	void	set_hit_button_no(int value)	{	m_work.hit_button_no = value;		}
	void	set_pushed_button_no(int value)	{	m_work.pushed_button_no = value;	}

private:
	C_elm_group_def_param	m_def;		// 固定パラメータ
	C_elm_group_param		m_param;	// 通常パラメータ
	C_elm_group_work_param	m_work;		// 作業パラメータ
};

// ****************************************************************
// グループリスト
// ================================================================
class C_elm_group_list : public C_elm_list<C_elm_group>
{
public:
	void	init(S_element element, CTSTR& name, int size, bool extend_enable, S_element target_object);	// 初期化

protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end);							// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード

private:
	S_element	m_target_object;
};
