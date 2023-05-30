#pragma		once

#include	"db_wnd_info.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�V�[���R���g���[��
// ================================================================
class C_db_wnd_info_scn_control : public C_db_wnd_info
{

public:

	void		init();					// ������
	void		load_state();			// �X�e�[�^�X�ǂݍ���
	void		save_state();			// �X�e�[�^�X�ۑ�

	void		start_up();

	TSTR		get_scn_name();			// �V�[�����擾
	int			get_scn_no();			// �V�[���ԍ��擾
	int			get_z_no();				// �y�ԍ��擾

private:

	// �R���g���[���A�C�e��
	struct CONTROL_ITEM
	{
		Cc_button			btn_restart;
		Cc_button			btn_select_start;
		C_tnm_dbg_listbox	lst_scn_name;
		C_tnm_dbg_listbox	lst_z_no;
		C_dialog_separater	spa_v00;
	}	ci;

	// �X�e�[�^�X
	struct STATE
	{
		TSTR	scn_name;
		int		z_no;
	}	m_state;

	ARRAY<int>	m_z_no_list;		// �y�ԍ����X�g



	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// �E�B���h�E�v���V�[�W��
	bool		on_init();										// ������
	bool		on_close();										// ����
	bool		on_command(WPARAM wp, LPARAM lp);				// �R�}���h

	void		make_scn_name_list();							// �V�[�������X�g�쐬
	void		make_z_no_list();								// �y�ԍ����X�g�쐬
	int			get_item_no_from_z_no(int z_no);				// ���ۂ̂y�ԍ�����y�ԍ����X�g�̃A�C�e���ԍ����擾

};

