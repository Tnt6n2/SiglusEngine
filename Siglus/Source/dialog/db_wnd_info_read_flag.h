#pragma		once

#include	"db_wnd_info.h"

class	C_tnm_scene_lexer;

// ****************************************************************
// �f�o�b�O���E�B���h�E�F���ǃt���O
// ================================================================
class C_db_wnd_info_read_flag : public C_db_wnd_info
{
public:
	void		init();					// ������
	void		load_state();			// �X�e�[�^�X�ǂݍ���
	void		save_state();			// �X�e�[�^�X�ۑ�
	void		frame();				// �t���[������

private:

	// �R���g���[���A�C�e��
	struct CONTROL_ITEM
	{
		C_tnm_dbg_listview	lv_scn;
		C_tnm_dbg_listview	lv_read_flag;
		Cc_button			btn_find;
		Cc_static			stc_status;
		C_dialog_separater	separater;
	}	ci;

	// �X�e�[�^�X
	struct STATE
	{
		struct SCENE
		{
			ARRAY<BYTE>	read_flag;
			int			flag_cnt;
			int			read_cnt;
		};

		ARRAY<SCENE>	scn_data;
		ARRAY<BYTE>		read_flag;
		int				cur_scn_no;
		TSTR			status;
	}	m_ld, m_cd;

	// ��ƕϐ�
	bool		m_first;

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// �E�B���h�E�v���V�[�W��
	bool		on_init();										// ������
	bool		on_close();										// ����
	bool		on_command(WPARAM wp, LPARAM lp);				// �R�}���h
	bool		on_notify(WPARAM wp, LPARAM lp);				// �ʒm

	void		create_read_flag_view();
};

