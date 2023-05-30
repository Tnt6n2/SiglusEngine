#pragma		once

// ****************************************************************
// �_�C�A���O
// ================================================================
class C_tnm_dlg : public C_dialog
{
public:

	struct REGIST_STATE
	{
		TSTR		window_name;			// �o�^�E�B���h�E��
		bool		limit_min;				// �E�B���h�E�̍ŏ���`���~�b�g
		bool		attach;					// �E�B���h�E�A�^�b�`
		int			dialog_id;				// �_�C�A���O�h�c
	}	regist_state;

	bool		initial_open_check();		// �����I�[�v������
	void		initial_open();				// �����I�[�v��
	void		save_ini_open_state();		// �I�[�v����Ԃ��h�m�h�ɕۑ�

	// ���@�[�`�����֐�
	virtual		void		init() = 0;		// ������
	virtual		void		free();			// ���
	virtual		void		open(HWND parent_h_wnd = G_app.h_wnd);		// �J���i�쐬�j
	virtual		void		load_state();	// �X�e�[�^�X�ǂݍ���
	virtual		void		save_state();	// �X�e�[�^�X�ۑ�
	virtual		void		frame();		// �t���[��

	bool		kill_focus_for_radio_item();			// ���W�I�{�^������t�H�[�J�X���O��
	void		regist(CTSTR window_name, bool limit_min, bool attach, int dialog_id);		// �o�^

protected:

	void		window_proc_pre(UINT msg, WPARAM wp, LPARAM lp);	// �E�B���h�E�v���V�[�W��
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);		// �E�B���h�E�v���V�[�W��
};

