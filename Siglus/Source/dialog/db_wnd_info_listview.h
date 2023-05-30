#pragma		once

#include	"db_wnd_info.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�ėp���X�g�r���[
// ================================================================
class C_db_wnd_info_listview : public C_db_wnd_info
{
public:
	void		regist_caption_name(CTSTR& name);								// �L���v�V�������o�^
	void		listview_add_column(CTSTR& name, CTSTR& caption, int width);	// ���X�g�r���[�̍��ڒǉ�
	void		open(HWND parent_h_wnd = G_app.h_wnd);							// �J���i�쐬�j

	C_tnm_dbg_listview*	get_lv()	{	return &lv;	}

protected:

	// �t���[������t���O
	bool	frame_first_flag;

	// �L���v�V������
	TSTR	caption_name_str;


	// ���X�g�r���[�R�������X�g
	struct LISTVIEW_COLUMN
	{
		TSTR	name;
		TSTR	caption;
		int		width;
	};
	ARRAY<LISTVIEW_COLUMN>	lvc_list;

	// ���X�g�r���[
	C_tnm_dbg_listview		lv;

	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);	// �E�B���h�E�v���V�[�W��
	bool		on_init();										// ������
	bool		on_close();										// ����

	virtual		void		on_init_func() = 0;					// ������
	virtual		void		on_close_func() = 0;				// ����
};

