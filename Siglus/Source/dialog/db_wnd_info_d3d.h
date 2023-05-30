#pragma		once

#include	"db_wnd_info_listview.h"
#include	"data/tnm_rsc_manager.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�FDirect3D - �f�o�C�X
// ================================================================
class C_db_wnd_info_d3d_device : public C_db_wnd_info_listview
{
public:
	void		init();					// ������
	void		frame();				// �t���[��

private:

	// �X�e�[�^�X
	struct STATE
	{
		int		video_memory_default_size;
		int		video_memory_rest_size;
		int		video_memory_use_size;
		int		video_memory_use_max_size;
	}	m_ld;

	void		on_init_func();		// �������t�@���N�V����
	void		on_close_func();	// ����t�@���N�V����
};

// ****************************************************************
// �f�o�b�O���E�B���h�E�FDirect3D - ���\�[�X
// ================================================================
class C_db_wnd_info_d3d_resource : public C_db_wnd_info_listview
{

public:

	void		init();					// ������
	void		frame();				// �t���[��

private:

	// �X�e�[�^�X
	struct STATE
	{
		ARRAY<S_tnm_d3d_resource>	resource_list;
	}	last_state;

	// ��ƕϐ�
	struct WORK
	{
	}	work;

	void		on_init_func();		// �������t�@���N�V����
	void		on_close_func();	// ����t�@���N�V����
};

