#pragma		once

#include	"db_wnd_info_listview.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F��
// ================================================================
class C_db_wnd_info_koe : public C_db_wnd_info
{
public:

	C_db_wnd_info_koe() : m_bitmap(true)
	{
	}

	void		init();					// ������
	void		load_state();			// �X�e�[�^�X�ǂݍ���
	void		save_state();			// �X�e�[�^�X�ۑ�
	void		frame();				// �t���[������

	Cc_static	m_stc_file;
	Cc_static	m_stc_koe_now;
	Cc_static	m_stc_koe_length;
	Cc_progress	m_progress;
	Cc_picture	m_picture;
	Cc_static	m_picture_rect;
	Cc_button	m_button_copy;
	Cc_check	m_chk_copy;

	NT3::C_dib	m_bitmap;

private:

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// �E�B���h�E�v���V�[�W��
	bool		on_init();										// ������
	bool		on_close();										// ����
	bool		on_command(WPARAM wp, LPARAM lp);				// �R�}���h
	bool		on_notify(WPARAM wp, LPARAM lp);				// �ʒm
	bool		on_lbutton_down(WPARAM wp, LPARAM lp);			// ���N���b�N
	bool		on_size(WPARAM wp, LPARAM lp);					// �T�C�Y���ύX���ꂽ

	void		update_view();

	// �p�����[�^
	struct S_param
	{
		int		koe_no;
		int		chara_no;
		int		prg_max;
		int		prg_now;
		int		mills_max;
		int		mills_now;
	};

	int			m_click_pos;
	S_param		m_ld;

	// ��ƕϐ�
	bool		m_first;
};

