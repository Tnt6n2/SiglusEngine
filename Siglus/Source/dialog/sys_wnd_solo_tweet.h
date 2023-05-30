#pragma		once

#include	"cfg_wnd_solo.h"

// ****************************************************************
// �V�X�e���E�B���h�E�F�\���F�c�C�[�g
// ================================================================

class C_sys_wnd_solo_tweet_bitmap : public Cc_picture
{
public:
	C_sys_wnd_solo_tweet_bitmap();

	C_dib		m_dib;
	HDC			m_h_src_dc;
protected:
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
private:
	bool		on_paint();					// �`��
};


class C_sys_wnd_solo_tweet : public C_cfg_wnd_solo
{
public:

	C_sys_wnd_solo_tweet_bitmap	m_stc_bitmap;

	Cc_static	m_stc_user_name;
	Cc_edit		m_edt_tweet_text;
	Cc_button	m_btn_authorize;
	Cc_button	m_btn_tweet;

	void		init();						// ������
	void		update_state();				// �X�e�[�g��������
	void		on_success_tweet();			// �c�C�[�g�ɐ���

private:

	bool		on_init_func();									// ������
	bool		on_close_func();								// ����
	bool		on_command(WPARAM wp, LPARAM lp);				// �R�}���h
};
