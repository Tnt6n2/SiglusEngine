#pragma		once

#include	"dialog/db_wnd_info_listview.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�V�X�e�����
// ================================================================
class C_db_wnd_info_save_history : public C_db_wnd_info_listview
{

public:

	void		init();		// ������
	void		frame();	// �t���[��

private:

	// �X�e�[�^�X
	struct STATE
	{
		int		m_save_history_top;				// �����̐擪
		int		m_save_history_cnt;				// �����̐�
	}	m_ld;

	// ��ƕϐ�
	struct WORK
	{
	}	work;

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// �E�B���h�E�v���V�[�W��
	void		on_init_func();		// �������t�@���N�V����
	void		on_close_func();	// ����t�@���N�V����
	bool		on_notify(WPARAM wp, LPARAM lp);	// �m�n�s�h�e�x

	void		load(int click_index);	// ���[�h

};

