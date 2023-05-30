#pragma		once

#include	"data/tnm_sorter.h"
#include	"dialog/db_wnd_info_listview.h"

class		C_tnm_btn_mng;

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�{�^���}�l�[�W��
// ================================================================
class C_db_wnd_info_btn : public C_db_wnd_info_listview
{
public:
	void		init();		// ������
	void		frame();	// �t���[��

private:

	// �X�e�[�^�X
	struct STATE
	{
		int				btn_cnt;			// �{�^���̌�
		int				active_btn_guid;	// �A�N�e�B�u�ȃ{�^����GUID
		S_tnm_sorter	active_btn_sorter;	// �A�N�e�B�u�ȃ{�^���̃\�[�^�[
		int				active_btn_state;	// �A�N�e�B�u�ȃ{�^���̃X�e�[�g
		bool			hit_now;			// ���񓖂�����
		bool			pushed_now;			// ���񉟂��ꂽ
		bool			decided_now;		// ���񌈒肳�ꂽ

	}	m_ld;

	void		on_init_func();		// �������t�@���N�V����
	void		on_close_func();	// ����t�@���N�V����
};
