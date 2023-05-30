#pragma		once

#include	"main.h"

// ****************************************************************
// ���C���_�C�A���O
// ================================================================
class C_tnm_project_setting_dialog : public C_modal_dialog
{
public:
	bool		open(HWND parent_h_wnd, PROJECT_SETTING_MODE proc_mode);	// �J���i�쐬�j

private:

	class Cc_stc_dragdrop : public Cc_static
	{
	public:
		C_tnm_project_setting_dialog* p_parent_class;
	private:
		LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// �E�B���h�E�v���V�[�W��
	};

	// �R���g���[���A�C�e��
	struct CONTROL_ITEM
	{
		Cc_edit		edt_project_name;
		Cc_edit		edt_scn_path;
		Cc_edit		edt_extend_scn_name;
		Cc_edit		edt_tmp_path;
		Cc_edit		edt_out_path;
		Cc_edit		edt_out_path_noangou;
		Cc_edit		edt_exe_path;
		Cc_edit		edt_work_path;
		Cc_edit		edt_editor_path;
		Cc_edit		edt_manual_path;

		Cc_edit		edt_gameexe_ini;
		Cc_edit		edt_extend_inc;
		Cc_edit		edt_gameexe_dat;
		Cc_edit		edt_scene_pck;

		Cc_stc_dragdrop	stc_dragdrop;
		Cc_check	chk_utf8;
		Cc_button	btn_ok;
		Cc_button	btn_cancel;
	}	ci;

	struct WORK
	{
		PROJECT_SETTING_MODE	proc_mode;		// �������[�h
		bool					return_value;	// �߂�l
	}	work;

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// �E�B���h�E�v���V�[�W��
	bool		on_init(WPARAM wp, LPARAM lp);					// ����������
	bool		on_close(WPARAM wp, LPARAM lp);					// �N���[�Y����
	bool		on_command(WPARAM wp, LPARAM lp);				// �R�}���h����

	void		set_path_easy(TSTR path);						// �h���b�O�A���h�h���b�v�ňꊇ�p�X�ݒ�
	void		set_path_easy_for_exe_path(TSTR path);			// �h���b�O�A���h�h���b�v�ňꊇ�p�X�ݒ�i�d�w�d�p�X�p�j
};
