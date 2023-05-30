#pragma		once

#include	"data/tnm_wipe.h"
#include	"data/tnm_wnd_menu.h"

// ****************************************************************
// �O���錾
// ================================================================
template <typename TYPE>	class C_tree2;
struct	S_tnm_sorter;
class	C_tnm_sprite;
class	C_elm_stage;
class	C_elm_screen;
class   C_elm_world;

// ****************************************************************
// �`��p�̒��_�o�b�t�@
// ================================================================

// ****************************************************************
// �E�B���h�E
// ================================================================
class C_tnm_wnd : public C_window
{
public:

	// �O���[�o��������
	void	init();

	// �`�揈��
	bool	disp_proc();													// �`�揈��
	bool	disp_proc_main();												// �`�揈�����C��
	bool	disp_proc_sprite_tree_to_sprite_list(C_tree2<C_tnm_sprite *>& node, ARRAY<C_tnm_sprite *>& sprite_list);
	bool	disp_proc_sprite_list(ARRAY<C_tnm_sprite *>& sprite_list);		// �X�v���C�g���X�g��`�悷��
	bool	disp_proc_capture_for_save_thumb();								// �L���v�`���[����
	bool	disp_proc_capture_for_object();									// �L���v�`���[����i�I�u�W�F�N�g�p�j
	bool	disp_proc_capture_for_thumb();									// �L���v�`���[����i�T���l�C���p�j
	bool	disp_proc_capture_for_tweet();									// �L���v�`���[����i�c�C�[�g�p�j
	bool	disp_proc_init_wipe_sprite(C_d3d_sprite* p_sprite);				// ���C�v�X�v���C�g������������
	bool	disp_proc_sprite(C_d3d_sprite* p_sprite);						// �X�v���C�g��`�悷��

	// �X�e�[�W��`�揀������
	bool	disp_proc_stage_ready(int stage_no, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter);
	// �X�e�[�W��`�悷��
	bool	disp_proc_stage(int stage_no, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter);
	bool	disp_proc_stage_sub(int stage_no, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter);

	// �S�ẴI�[�_�[��`�揀������
	bool	disp_proc_all_order_ready(int stage_no);
	// �S�ẴI�[�_�[��`�悷��
	bool	disp_proc_all_order(int stage_no);
	// ���C�v����̃I�[�_�[��`�揀������
	bool	disp_proc_over_order_ready(int stage_no);
	// ���C�v����̃I�[�_�[��`�悷��
	bool	disp_proc_over_order(int stage_no);
	// ���C�v����I�[�_�[��`�揀������
	bool	disp_proc_wipe_order_ready(int stage_no);
	// ���C�v����I�[�_�[��`�悷��
	bool	disp_proc_wipe_order(int stage_no);
	// ���C�v��艺�̃I�[�_�[��`�揀������
	bool	disp_proc_under_order_ready(int stage_no);
	// ���C�v��艺�̃I�[�_�[��`�悷��
	bool	disp_proc_under_order(int stage_no);
	// �őO�ʂ�`�悷��
	bool	disp_proc_top_most();

	// ���C�v�o�b�t�@�O����������
	bool	disp_proc_ready_wipe_buffer_0();
	// ���C�v�o�b�t�@�P����������
	bool	disp_proc_ready_wipe_buffer_1();

	// ���C�v�p�����[�^�`�F�b�N���R�s�[
	bool	check_and_copy_for_wipe_param();
	// �}�X�N�쐬
	void	make_mask();

	// ���C�v�`��
	bool	disp_proc_wipe();												// ���C�v�`��
	bool	disp_proc_wipe_for_not_wipe_after_wait(int wipe_prg);			// ���C�v�`��F���C�v�Ȃ��i�\�����Ă���҂j
	bool	disp_proc_wipe_for_not_wipe_befor_wait(int wipe_prg);			// ���C�v�`��F���C�v�Ȃ��i�҂��Ă���\���j
	bool	disp_proc_wipe_for_cross_fade(int wipe_prg);					// ���C�v�`��F�N���X�t�F�[�h

	bool	disp_proc_wipe_for_mask(int wipe_prg);									// ���C�v�`��F�}�X�N
	void	disp_proc_wipe_for_mask_func(C_d3d_sprite* p_sprite, int wipe_prg);		// ���C�v�`��F�}�X�N�e�t�m�b
	
	bool	disp_proc_wipe_for_shimi(int wipe_prg);									// ���C�v�`��F�V�~
	void	disp_proc_wipe_for_shimi_func(C_d3d_sprite* p_sprite, int wipe_prg);	// ���C�v�`��F�V�~�e�t�m�b

	bool	disp_proc_wipe_for_cross_raster(int wipe_prg);							// ���C�v�`��F�N���X���X�^
	bool	disp_proc_wipe_for_raster(int wipe_prg);								// ���C�v�`��F���X�^
	void	disp_proc_wipe_for_raster_func(C_d3d_sprite* p_sprite, int wipe_prg);	// ���C�v�`��F���X�^�e�t�m�b

	bool	disp_proc_wipe_for_move(int wipe_prg);							// ���C�v�`��F�ړ��n
	bool	disp_proc_wipe_for_move_get_stage(int wipe_prg);				// ���C�v�`��F�ړ��n�X�e�[�W�擾
	bool	disp_proc_wipe_for_move_switch(int wipe_prg, bool is_front);	// ���C�v�`��F�ړ��n�X�C�b�`
	bool	disp_proc_wipe_for_page(int wipe_prg, bool is_front);			// ���C�v�`��F�y�[�W�߂���	
	
	void	disp_proc_wipe_for_move_scroll_and_expansion_and_contraction(C_d3d_sprite* p_sprite, int wipe_prg, bool is_front);	// ���C�v�`��F�ړ��n�F�X�N���[���ƐL�k�ό`
	void	disp_proc_wipe_for_move_func_scroll(int wipe_prg, C_d3d_sprite *sprite, int dir, bool is_in);						// ���C�v�`��F�ړ��n�e�t�m�b�F�X�N���[��
	void	disp_proc_wipe_for_move_func_expansion_and_contraction(int wipe_prg, C_d3d_sprite *sprite, int dir, bool is_in);	// ���C�v�`��F�ړ��n�e�t�m�b�F�L�k�ό`

	bool	disp_proc_wipe_for_scale(int wipe_prg);							// ���C�v�`��F�X�P�[���n
	bool	disp_proc_wipe_for_scale_get_stage(int wipe_prg);				// ���C�v�`��F�X�P�[���n�X�e�[�W�擾
	bool	disp_proc_wipe_for_scale_switch(int wipe_prg, bool is_front);	// ���C�v�`��F�X�P�[���n�X�C�b�`
	void	disp_proc_wipe_for_scale_func(C_d3d_sprite* sprite, int wipe_prg);		// ���C�v�`��F�X�P�[���n�e�t�m�b
	void	disp_proc_wipe_for_scale_func(D3DXVECTOR4 pos[], D3DXVECTOR2 uv[], C_d3d_sprite* sprite, int wipe_prg);		// ���C�v�`��F�X�P�[���n�e�t�m�b

	bool	disp_proc_wipe_for_explosion_blur(int wipe_prg);								// ���C�v�`��F�����u���[
	bool	disp_proc_wipe_for_explosion_blur_get_stage(int wipe_prg);						// ���C�v�`��F�����u���[�X�e�[�W�擾
	void	disp_proc_wipe_for_explosion_blur_func(C_d3d_sprite* sprite, int wipe_prg);		// ���C�v�`��F�����u���[�e�t�m�b

	bool	disp_proc_wipe_for_mosaic(int wipe_prg);								// ���C�v�`��F���U�C�N
	bool	disp_proc_wipe_for_mosaic_get_stage(int wipe_prg);						// ���C�v�`��F���U�C�N�X�e�[�W�擾
	void	disp_proc_wipe_for_mosaic_func(C_d3d_sprite* sprite, int wipe_prg);		// ���C�v�`��F���U�C�N�e�t�m�b

	// �L���v�V�������X�V
	void	update_caption();

	// �L���v�`���[�o�b�t�@
	bool	create_capture_buffer(int width, int height);
	bool	destroy_capture_buffer();
	bool	disp_proc_capture_to_buffer();

	// �r���[
	C_d3d_renderer		m_view;
	C_argb				m_back_color;
	C_argb				m_game_back_color;
	C_argb				m_wipe_back_color;

	// �E�B���h�E���j���[
	C_tnm_wnd_menu		m_wnd_menu;

	// ���C�v
	C_d3d_surface		m_screen_back_buffer;				// �X�N���[���̃o�b�N�o�b�t�@
	C_d3d_surface		m_screen_dpst_buffer;				// �X�N���[���̐[�x�X�e���V���o�b�t�@
	C_d3d_surface		m_target_back_buffer;				// �^�[�Q�b�g�ƂȂ�o�b�N�o�b�t�@
	C_d3d_surface		m_target_dpst_buffer;				// �^�[�Q�b�g�ƂȂ�[�x�X�e���V���o�b�t�@
	C_size				m_target_buffer_size;				// �^�[�Q�b�g�ƂȂ�o�b�t�@�T�C�Y
	C_size				m_target_buffer_size_ex;			// �^�[�Q�b�g�ƂȂ�o�b�t�@�T�C�Y�d�w
	BSP<C_d3d_texture>	m_game_back_buffer_texture;			// �Q�[����ʗp�̃o�b�N�o�b�t�@
	BSP<C_d3d_surface>	m_game_dpst_buffer_surface;			// �Q�[����ʗp�̐[�x�X�e���V���o�b�t�@
	BSP<C_d3d_texture>	m_wipe_back_buffer_texture_0;		// ���C�v�p�̃o�b�N�o�b�t�@�O
	BSP<C_d3d_surface>	m_wipe_dpst_buffer_surface_0;		// ���C�v�p�̐[�x�X�e���V���o�b�t�@�O
	BSP<C_d3d_texture>	m_wipe_back_buffer_texture_1;		// ���C�v�p�̃o�b�N�o�b�t�@�P
	BSP<C_d3d_texture>	m_mask_buffer_texture;				// �}�X�N�o�b�t�@
	BSP<C_dib_chip>		m_mask_buffer_dib;					// �}�X�N�o�b�t�@�c�h�a
	BSP<C_d3d_texture>	m_capture_texture_for_save_thumb;	// �L���v�`���[�p�̃e�N�X�`��
	BSP<C_d3d_texture>	m_capture_texture_for_object;		// �L���v�`���[�p�̃e�N�X�`��
	BSP<C_d3d_texture>	m_capture_texture_for_tweet;		// �L���v�`���[�p�̃e�N�X�`��

	BSP<C_d3d_texture>	m_capture_back_buffer_texture;		// �L���v�`���[�o�b�t�@�p�̃e�N�X�`��
	BSP<C_d3d_surface>	m_capture_dpst_buffer_surface;		// �L���v�`���[�o�b�t�@�p�̃e�N�X�`��
	BSP<C_d3d_texture>	m_capture_temp_buffer_texture;		// �L���v�`���[�o�b�t�@�p�̃e�N�X�`��
	int					m_capture_buffer_width;
	int					m_capture_buffer_height;

private:
	int					m_ready_wipe_type;								// �����ł������C�v�ԍ�
	TSTR				m_ready_wipe_file;								// �����ł����}�X�N���C�v�t�@�C��
	int					m_ready_wipe_option[TNM_WIPE_OPTION_MAX];		// �����ł������C�v�I�v�V����

	bool	disp_proc_sprite_list_sub(C_elm_world* p_world, ARRAY<C_tnm_sprite *>& sprite_list);	// �X�v���C�g���X�g��`�悷��

protected:
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);

private:
	bool	on_create();
	bool	on_close();
	bool	on_paint();
	bool	on_command(WPARAM wp, LPARAM lp);
	bool	on_set_cursor(WPARAM wp, LPARAM lp);

	// �p�����[�^
	struct	STATE
	{
		TSTR	title;
	}	m_ld;

	bool	m_first;

	// �E�B���h�E�T�C�Y�ƃN���C�A���g�T�C�Y�̍���
	C_size m_diff;
};



