#pragma		once

#include	"element/elm_list.h"

// ****************************************************************
// エディットボックス：定数
// ================================================================
const int	TNM_EDITBOX_NOT_DECIDED = 0;
const int	TNM_EDITBOX_DECIDED = 1;
const int	TNM_EDITBOX_CANCELED = -1;

// ****************************************************************
// エディットボックス
// ================================================================
class C_tnm_editbox : public Cc_edit
{
public:
	void		init()				{	m_action_flag = TNM_EDITBOX_NOT_DECIDED;	}
	void		clear_input()		{	m_action_flag = TNM_EDITBOX_NOT_DECIDED;	}
	int			get_action_flag()	{	return m_action_flag;						}

protected:
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);

private:
	int			m_action_flag;
};

// ****************************************************************
// エレメント：エディットボックス
// ================================================================
class C_elm_editbox : public C_tnm_element
{
public:
	C_elm_editbox();
	~C_elm_editbox();

	void	init();								// 初期化
	void	reinit();							// 再初期化
	void	save(C_tnm_save_stream& stream);	// セーブ
	void	load(C_tnm_save_stream& stream);	// ロード
	void	frame();							// フレーム処理

	void	create(C_rect rect, int moji_size);	// 作成
	void	destroy();							// 破壊
	void	update_rect();						// 矩形を更新
	void	set_text(CTSTR& text);				// テキストを設定
	TSTR	get_text();							// テキストを取得
	void	set_focus();						// フォーカスを設定
	void	clear_input();						// 入力をクリア

	// 作成されているかを判定
	bool is_created()	{	return m_created;	}

	// 決定されているかを判定
	bool is_decided()	{	return m_editbox.get_action_flag() == TNM_EDITBOX_DECIDED;		}

	// キャンセルされているかを判定
	bool is_canceled()	{	return m_editbox.get_action_flag() == TNM_EDITBOX_CANCELED;		}

	// エディットボックス本体を取得
	C_tnm_editbox& editbox()	{	return m_editbox;	}

private:
	void	release_font();

	bool			m_created;
	C_rect			m_rect;
	int				m_moji_size;
	HFONT			m_h_font;
	C_tnm_editbox	m_editbox;
};

// ****************************************************************
// エレメント：エディットボックスリスト
// ================================================================
class C_elm_editbox_list : public C_elm_list<C_elm_editbox>
{
protected:
	void	_init(int begin, int end);								// サブの初期化
	void	_reinit(int begin, int end);							// サブの再初期化
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// サブのセーブ
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// サブのロード

};

