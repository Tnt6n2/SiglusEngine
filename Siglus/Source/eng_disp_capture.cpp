#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_thumb_table.h"

#include	"engine/eng_graphics.h"
#include	"engine/ifc_error.h"

// ****************************************************************
// �L���v�`���[�i�Z�[�u�T���l�C���j
// ================================================================
bool C_tnm_wnd::disp_proc_capture_for_save_thumb()
{
	// �T���l�C���e�[�u������T���l�C��������
	TSTR thumb_name = Gp_thumb_table->calc_thumb_file_name();
	
	// ��p�T���l�C�����g���ꍇ
	if (!thumb_name.empty())	{

		// �T���l�C���摜��ǂݍ���
		BSP<C_d3d_album> album = tnm_load_pct_d3d(thumb_name, false, false);
		if (!album)
			return false;

		// �J�b�g�O�Ԃ��g��
		m_capture_texture_for_save_thumb = album->get_texture(0);

		// ����
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("����p�T���l�C�����L���v�`���[�ɐݒ肵�܂����B"));
	}

	// ��ʂ��L���v�`���[���Ďg���ꍇ
	else	{

		// �Z�[�u�T���l�C���̃T�C�Y�ɏk������
		BSP<C_d3d_texture> capture_back_buffer_texture_src;
		BSP<C_d3d_texture> capture_back_buffer_texture_dst;
		BSP<C_d3d_surface> capture_dpst_buffer_surface_dst;
		int thumb_w = (int)Gp_ini->save_thumb_size.cx;
		int thumb_h = (int)Gp_ini->save_thumb_size.cy;

		// �L���v�`���[���̓Q�[���o�b�t�@
		capture_back_buffer_texture_src = m_game_back_buffer_texture;

		// �L���v�`���[��̃o�b�t�@���쐬����
		// �e�N�X�`���̓����_�[�^�[�Q�b�g�ɂ��邽�߁AD3DUSAGE_RENDERTARGET ���w��
		// D3DUSAGE_RENDERTARGET �ɂ���ɂ́AD3DPOOL_DEFAULT ���w�肷��K�v������
		// D3DPOOL_DEFAULT ���w�肵���ꍇ�́A�f�o�C�X�����������玩���ō쐬���Ȃ����K�v�����邽�߁A���\�[�X�}�l�[�W���𗘗p����
		// �J���[�t�H�[�}�b�g�́A�f�o�C�X�̍쐬���ɒ��ׂ� D3DFMT_X8R8G8B8 �ɂ��邱�ƁB�ڂ����̓f�o�C�X�̍쐬�Q�ƁB
		capture_back_buffer_texture_dst = G_rsm.create_texture(_T("�������"), thumb_w, thumb_h, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, true);
		if (!capture_back_buffer_texture_dst)	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�L���v�`���[�p�̃e�N�X�`���̍쐬�Ɏ��s���܂����B(%d x %d)\n"), thumb_w, thumb_h) + get_last_error_msg());
			return false;
		}
		capture_dpst_buffer_surface_dst = G_rsm.create_dpst_buffer(_T("�������"), thumb_w, thumb_h, true);
		if (!capture_dpst_buffer_surface_dst)	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�L���v�`���[�p�̐[�x�X�e���V���o�b�t�@�̍쐬�Ɏ��s���܂����B(%d x %d)\n"), thumb_w, thumb_h) + get_last_error_msg());
			return false;
		}

		// �Q�[���X�v���C�g�̏����ݒ�
		C_d3d_sprite game_sprite;
		game_sprite.create_polygon_no_init();
		game_sprite.rp.disp = true;
		game_sprite.rp.d2_rect = true;			// �Q�c��`
		game_sprite.rp.alpha_test = false;		// �A���t�@�e�X�g���Ȃ�
		game_sprite.rp.alpha_blend = false;		// �A���t�@�u�����h���Ȃ�
		game_sprite.rp.size = C_size(thumb_w, thumb_h);
		game_sprite.rp.dst_clip_use = true;
		game_sprite.rp.dst_clip = C_rect(C_point(0, 0), C_size(thumb_w, thumb_h));
		game_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
		game_sprite.set_index_buffer(G_rect_index_buffer);
		game_sprite.set_texture(capture_back_buffer_texture_src, 0);

		// �Q�[���X�v���C�g���X�V����
		void* p_locked_vertex_buffer;
		G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
		if (game_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
			G_rect_vertex_buffer_d2_screen->unlock();

			// �L���v�`���[�o�b�t�@�����o��
			C_d3d_surface capture_back_buffer_dst = capture_back_buffer_texture_dst->get_surface(0);
			C_d3d_surface capture_dpst_buffer_dst = *capture_dpst_buffer_surface_dst;

			// �����_�[�^�[�Q�b�g���L���v�`���[�o�b�t�@�ɐݒ肷��
			m_view.set_render_target(capture_back_buffer_dst.body());
			m_view.set_dpst_buffer(capture_dpst_buffer_dst.body());

			// �Q�[���X�v���C�g��`��
			if (!disp_proc_sprite(&game_sprite))
				return false;
		}
		else	{
			G_rect_vertex_buffer_d2_screen->unlock();
		}

		// �L���v�`���[�摜���V�X�e���������Ɏ��o��
		// �������Ȃ��ƃf�o�C�X�����������Ƃ��Ɏ����Ă��܂�

		// �f�[�^�擾�p�̃e�N�X�`�������
		// �L���v�`���[�摜�ɍ��킹�� D3DFMT_X8R8G8B8 �ɂ���K�v������
		// D3DPOOL_SYSTEMMEM �łȂ���΂Ȃ�Ȃ��i�d�v�I D3DPOOL_MANAGED ���ƃG���[���o��j
		BSP<C_d3d_texture> p_thumb_texture = G_rsm.create_texture(_T("���޻�Ȳ�"), thumb_w, thumb_h, 0, 0, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, false);
		if (!p_thumb_texture)
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�T���l�C���p�̃e�N�X�`���̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
			return false;
		}
		// �e�N�X�`���ɃL���v�`���[�摜���擾����
		if (!capture_back_buffer_texture_dst->get_render_target_data(0, p_thumb_texture->get_surface(0).body()))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�L���v�`���[�摜�̃f�[�^���擾�ł��܂���ł����B\n") + get_last_error_msg());
			return false;
		}

		// �����o�ɕۑ�
		m_capture_texture_for_save_thumb = p_thumb_texture;

		// ����
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���Z�[�u�T���l�C�����쐬���܂����B"));
	}

	return true;
}

// ****************************************************************
// �L���v�`���[�i�c�C�[�g�j
// ================================================================
bool C_tnm_wnd::disp_proc_capture_for_tweet()
{
	// �Z�[�u�T���l�C���̃T�C�Y�ɏk������
	BSP<C_d3d_texture> capture_back_buffer_texture_src;
	BSP<C_d3d_texture> capture_back_buffer_texture_dst;
	BSP<C_d3d_surface> capture_dpst_buffer_surface_dst;
	int thumb_w = Gp_ini->screen_size.cx;
	int thumb_h = Gp_ini->screen_size.cy;

	// �L���v�`���[���̓Q�[���o�b�t�@
	capture_back_buffer_texture_src = m_game_back_buffer_texture;

	// �L���v�`���[��̃o�b�t�@���쐬����
	// �e�N�X�`���̓����_�[�^�[�Q�b�g�ɂ��邽�߁AD3DUSAGE_RENDERTARGET ���w��
	// D3DUSAGE_RENDERTARGET �ɂ���ɂ́AD3DPOOL_DEFAULT ���w�肷��K�v������
	// �g���̂ĂȂ̂Ń��\�[�X�}�l�[�W���̊Ǘ��͕K�v�Ȃ�
	// �J���[�t�H�[�}�b�g�́A�f�o�C�X�̍쐬���ɒ��ׂ� D3DFMT_X8R8G8B8 �ɂ��邱�ƁB�ڂ����̓f�o�C�X�̍쐬�Q�ƁB
	capture_back_buffer_texture_dst = G_rsm.create_texture(_T("�������"), thumb_w, thumb_h, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, false);
	if (!capture_back_buffer_texture_dst)
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�L���v�`���[�p�̃e�N�X�`���̍쐬�Ɏ��s���܂����B(%d x %d)\n"), thumb_w, thumb_h) + get_last_error_msg());
		return false;
	}
	capture_dpst_buffer_surface_dst = G_rsm.create_dpst_buffer(_T("�������"), thumb_w, thumb_h, true);
	if (!capture_dpst_buffer_surface_dst)
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�L���v�`���[�p�̐[�x�X�e���V���o�b�t�@�̍쐬�Ɏ��s���܂����B(%d x %d)\n"), thumb_w, thumb_h) + get_last_error_msg());
		return false;
	}

	// �Q�[���X�v���C�g�̏����ݒ�
	C_d3d_sprite game_sprite;
	game_sprite.create_polygon_no_init();
	game_sprite.rp.disp = true;
	game_sprite.rp.d2_rect = true;			// �Q�c��`
	game_sprite.rp.alpha_test = false;		// �A���t�@�e�X�g���Ȃ�
	game_sprite.rp.alpha_blend = false;		// �A���t�@�u�����h���Ȃ�
	game_sprite.rp.size = C_size(thumb_w, thumb_h);
	game_sprite.rp.dst_clip_use = true;
	game_sprite.rp.dst_clip = C_rect(C_point(0, 0), C_size(thumb_w, thumb_h));
	game_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
	game_sprite.set_index_buffer(G_rect_index_buffer);
	game_sprite.set_texture(capture_back_buffer_texture_src, 0);

	// �Q�[���X�v���C�g���X�V����
	void* p_locked_vertex_buffer;
	G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
	if (!game_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))
	{
		G_rect_vertex_buffer_d2_screen->unlock();
		return false;
	}
	
	G_rect_vertex_buffer_d2_screen->unlock();

	// �L���v�`���[�o�b�t�@�����o��
	C_d3d_surface capture_back_buffer_dst = capture_back_buffer_texture_dst->get_surface(0);
	C_d3d_surface capture_dpst_buffer_dst = *capture_dpst_buffer_surface_dst;

	// �����_�[�^�[�Q�b�g���L���v�`���[�o�b�t�@�ɐݒ肷��
	m_view.set_render_target(capture_back_buffer_dst.body());
	m_view.set_dpst_buffer(capture_dpst_buffer_dst.body());

	// �Q�[���X�v���C�g��`��
	if (!disp_proc_sprite(&game_sprite))
		return false;

	// ���S��ǂݍ���
	TSTR logo_file_name = Gp_ini->twitter_overlap_image;
	if (!logo_file_name.empty() && tnm_check_pct(logo_file_name, false))
	{
		BSP<C_d3d_album> album = tnm_load_pct_d3d(logo_file_name, false, false);
		if (album)
		{
			// ���S�X�v���C�g�̏����ݒ�
			C_d3d_sprite logo_sprite;
			logo_sprite.create_polygon_no_init();
			logo_sprite.rp.disp = true;
			logo_sprite.rp.d2_rect = true;			// �Q�c��`
			logo_sprite.rp.alpha_test = true;		// �A���t�@�e�X�g����
			logo_sprite.rp.alpha_blend = true;		// �A���t�@�u�����h���Ȃ�
			logo_sprite.rp.size = C_size(thumb_w, thumb_h);
			logo_sprite.rp.dst_clip_use = true;
			logo_sprite.rp.dst_clip = C_rect(C_point(0, 0), C_size(thumb_w, thumb_h));
			logo_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
			logo_sprite.set_index_buffer(G_rect_index_buffer);
			logo_sprite.set_texture(album->get_texture(0), 0);

			// ���S�X�v���C�g���X�V����
			G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
			if (!logo_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))
			{
				G_rect_vertex_buffer_d2_screen->unlock();
				return false;
			}

			G_rect_vertex_buffer_d2_screen->unlock();

			// ���S�X�v���C�g��`��
			if (!disp_proc_sprite(&logo_sprite))
				return false;
		}
	}

	// �L���v�`���[�摜���V�X�e���������Ɏ��o��

	// �f�[�^�擾�p�̃e�N�X�`�������
	// �L���v�`���[�摜�ɍ��킹�� D3DFMT_X8R8G8B8 �ɂ���K�v������
	// D3DPOOL_SYSTEMMEM �łȂ���΂Ȃ�Ȃ��i�d�v�I D3DPOOL_MANAGED ���ƃG���[���o��j
	BSP<C_d3d_texture> p_thumb_texture = G_rsm.create_texture(_T("²��"), thumb_w, thumb_h, 0, 0, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, false);
	if (!p_thumb_texture)
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�T���l�C���p�̃e�N�X�`���̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}
	// �e�N�X�`���ɃL���v�`���[�摜���擾����
	if (!capture_back_buffer_texture_dst->get_render_target_data(0, p_thumb_texture->get_surface(0).body()))
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�L���v�`���[�摜�̃f�[�^���擾�ł��܂���ł����B\n") + get_last_error_msg());
		return false;
	}

	// �����o�ɕۑ�
	m_capture_texture_for_tweet = p_thumb_texture;

	// ����
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���c�C�[�g�T���l�C�����쐬���܂����B"));

	return true;
}

// ****************************************************************
// �L���v�`���[�i�T���l�C���p�j
// ================================================================
bool C_tnm_wnd::disp_proc_capture_for_thumb()
{
	// �Z�[�u�T���l�C���̃T�C�Y�ɏk������
	BSP<C_d3d_texture> capture_back_buffer_texture_src;
	BSP<C_d3d_texture> capture_back_buffer_texture_dst;
	BSP<C_d3d_surface> capture_dpst_buffer_surface_dst;
	int thumb_w = Gp_global->capture_w;
	int thumb_h = Gp_global->capture_h;

	// �L���v�`���[���̓Q�[���o�b�t�@
	capture_back_buffer_texture_src = m_game_back_buffer_texture;

	// �L���v�`���[��̃o�b�t�@���쐬����
	// �e�N�X�`���̓����_�[�^�[�Q�b�g�ɂ��邽�߁AD3DUSAGE_RENDERTARGET ���w��
	// D3DUSAGE_RENDERTARGET �ɂ���ɂ́AD3DPOOL_DEFAULT ���w�肷��K�v������
	// D3DPOOL_DEFAULT ���w�肵���ꍇ�́A�f�o�C�X�����������玩���ō쐬���Ȃ����K�v�����邽�߁A���\�[�X�}�l�[�W���𗘗p����
	// �J���[�t�H�[�}�b�g�́A�f�o�C�X�̍쐬���ɒ��ׂ� D3DFMT_X8R8G8B8 �ɂ��邱�ƁB�ڂ����̓f�o�C�X�̍쐬�Q�ƁB
	capture_back_buffer_texture_dst = G_rsm.create_texture(_T("�������"), thumb_w, thumb_h, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, true);
	if (!capture_back_buffer_texture_dst)
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�L���v�`���[�p�̃e�N�X�`���̍쐬�Ɏ��s���܂����B(%d x %d)\n"), thumb_w, thumb_h) + get_last_error_msg());
		return false;
	}
	capture_dpst_buffer_surface_dst = G_rsm.create_dpst_buffer(_T("�������"), thumb_w, thumb_h, true);
	if (!capture_dpst_buffer_surface_dst)
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�L���v�`���[�p�̐[�x�X�e���V���o�b�t�@�̍쐬�Ɏ��s���܂����B(%d x %d)\n"), thumb_w, thumb_h) + get_last_error_msg());
		return false;
	}

	// �Q�[���X�v���C�g�̏����ݒ�
	C_d3d_sprite game_sprite;
	game_sprite.create_polygon_no_init();
	game_sprite.rp.disp = true;
	game_sprite.rp.d2_rect = true;			// �Q�c��`
	game_sprite.rp.alpha_test = false;		// �A���t�@�e�X�g���Ȃ�
	game_sprite.rp.alpha_blend = false;		// �A���t�@�u�����h���Ȃ�
	game_sprite.rp.size = C_size(thumb_w, thumb_h);
	game_sprite.rp.dst_clip_use = true;
	game_sprite.rp.dst_clip = C_rect(C_point(0, 0), C_size(thumb_w, thumb_h));
	game_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
	game_sprite.set_index_buffer(G_rect_index_buffer);
	game_sprite.set_texture(capture_back_buffer_texture_src, 0);

	// �Q�[���X�v���C�g���X�V����
	void* p_locked_vertex_buffer;
	G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
	if (game_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))
	{
		G_rect_vertex_buffer_d2_screen->unlock();

		// �L���v�`���[�o�b�t�@�����o��
		C_d3d_surface capture_back_buffer_dst = capture_back_buffer_texture_dst->get_surface(0);
		C_d3d_surface capture_dpst_buffer_dst = *capture_dpst_buffer_surface_dst;

		// �����_�[�^�[�Q�b�g���L���v�`���[�o�b�t�@�ɐݒ肷��
		m_view.set_render_target(capture_back_buffer_dst.body());
		m_view.set_dpst_buffer(capture_dpst_buffer_dst.body());

		// �Q�[���X�v���C�g��`��
		if (!disp_proc_sprite(&game_sprite))
			return false;
	}
	else
	{
		G_rect_vertex_buffer_d2_screen->unlock();
	}

	// �L���v�`���[�摜���V�X�e���������Ɏ��o��
	// �������Ȃ��ƃf�o�C�X�����������Ƃ��Ɏ����Ă��܂�

	// �f�[�^�擾�p�̃e�N�X�`�������
	// �L���v�`���[�摜�ɍ��킹�� D3DFMT_X8R8G8B8 �ɂ���K�v������
	// D3DPOOL_SYSTEMMEM �łȂ���΂Ȃ�Ȃ��i�d�v�I D3DPOOL_MANAGED ���ƃG���[���o��j
	BSP<C_d3d_texture> p_thumb_texture = G_rsm.create_texture(_T("��Ȳ�"), thumb_w, thumb_h, 0, 0, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, false);
	if (!p_thumb_texture)
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�T���l�C���p�̃e�N�X�`���̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	// �e�N�X�`���ɃL���v�`���[�摜���擾����
	if (!capture_back_buffer_texture_dst->get_render_target_data(0, p_thumb_texture->get_surface(0).body()))
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�L���v�`���[�摜�̃f�[�^���擾�ł��܂���ł����B\n") + get_last_error_msg());
		return false;
	}

	// �����o�ɕۑ�
	m_capture_texture_for_object = p_thumb_texture;

	// ����
	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���T���l�C�����쐬���܂����B"));

	return true;
}

// ****************************************************************
// �L���v�`���[�i�I�u�W�F�N�g�p�j
// ================================================================
bool C_tnm_wnd::disp_proc_capture_for_object()
{
	bool ret = false;

	int sw = (int)Gp_ini->screen_size.cx;
	int sh = (int)Gp_ini->screen_size.cy;

	//
	// �P�D�L���v�`���[�摜�����o��
	//

	// �L���v�`���[�摜�擾�p�̃e�N�X�`�� sysmem_texture �����B
	// GetRenderTargetData ���g���ɂ� D3DPOOL_SYSTEMMEM �łȂ���΂Ȃ�Ȃ��B
	// D3DPOOL_SYSTEMMEM �ł� D3DUSAGE_AUTOGENMIPMAP �͎w��ł��Ȃ��B
	// �o�b�N�o�b�t�@�ɍ��킹�� D3DFMT_X8R8G8B8 �ɂ���K�v������B
	BSP<C_d3d_texture> sysmem_texture = G_rsm.create_texture(_T("�������(������)"), sw, sh, 0, 0, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, false);
	if (!sysmem_texture)	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�L���v�`���[�p�̈ꎞ�e�N�X�`���̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
	}
	else	{

		// sysmem_texture �ɃL���v�`���[�摜���擾����
		if (!m_game_back_buffer_texture->get_render_target_data(0, sysmem_texture->get_surface(0).body()))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�L���v�`���[���擾�ł��܂���ł����B\n") + get_last_error_msg());
		}
		else	{

			//
			// �Q�D�L���v�`���[�摜�� MANAGED �e�N�X�`���ɃR�s�[����
			//

			// MANAGED �e�N�X�`�����쐬����B
			// �I�u�W�F�N�g�ɓn�����߁AD3DFMT_A8R8G8B8 ���w�肷��K�v������B
			// D3DUSAGE_AUTOGENMIPMAP ��Y�ꂸ�ɁB
			BSP<C_d3d_texture> managed_texture = G_rsm.create_texture(_T("�������"), sw, sh, 0, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, false);
			if (!managed_texture)	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�L���v�`���[�p�̃e�N�X�`���̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
			}
			else	{

				// ���b�N���ăf�[�^���R�s�[����
				// �s�����x�� 255 �Ŗ��߂�
				D3DLOCKED_RECT lr_sysmem, lr_managed;
				if (!sysmem_texture->lock_rect(0, &lr_sysmem, NULL, D3DLOCK_DISCARD))	{
					tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�L���v�`���[�p�̈ꎞ�e�N�X�`���̃��b�N�Ɏ��s���܂����B\n") + get_last_error_msg());
				}
				else	{
					if (!managed_texture->lock_rect(0, &lr_managed, NULL, D3DLOCK_DISCARD))	{
						tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�L���v�`���[�p�̃e�N�X�`���̃��b�N�Ɏ��s���܂����B\n") + get_last_error_msg());
					}
					else	{
						for (int y = 0; y < sh; y++)	{
							BYTE* dst = ((BYTE *)lr_managed.pBits) + lr_managed.Pitch * y;
							BYTE* src = ((BYTE *)lr_sysmem.pBits) + lr_sysmem.Pitch * y;
							for (int x = 0; x < sw; x++)	{
								*dst++ = *src++;
								*dst++ = *src++;
								*dst++ = *src++;
								*dst++ = 255;	src++;
							}
						}

						ret = true;	// �����I

						managed_texture->unlock_rect(0);
					}
					sysmem_texture->unlock_rect(0);
				}

				// �����o�ɕۑ�
				m_capture_texture_for_object = managed_texture;
			}
		}
	}

	// ����
	if (ret)	{
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("����ʂ��L���v�`���[���܂����B"));
	}
	else	{
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("����ʂ̃L���v�`���[�Ɏ��s���܂����B"));
	}

	return ret;
}

// ****************************************************************
// �L���v�`���[�o�b�t�@�V�X�e��
// ================================================================

// �L���v�`���[�o�b�t�@���쐬
bool C_tnm_wnd::create_capture_buffer(int width, int height)
{
	// �Z�[�u�T���l�C���̃T�C�Y�ɏk������
	BSP<C_d3d_texture> capture_back_buffer_texture;
	BSP<C_d3d_surface> capture_dpst_buffer_surface;

	// �L���v�`���[��̃o�b�t�@���쐬����
	// �e�N�X�`���̓����_�[�^�[�Q�b�g�ɂ��邽�߁AD3DUSAGE_RENDERTARGET ���w��
	// D3DUSAGE_RENDERTARGET �ɂ���ɂ́AD3DPOOL_DEFAULT ���w�肷��K�v������
	// D3DPOOL_DEFAULT ���w�肵���ꍇ�́A�f�o�C�X�����������玩���ō쐬���Ȃ����K�v�����邽�߁A���\�[�X�}�l�[�W���𗘗p����
	// �J���[�t�H�[�}�b�g�́A�f�o�C�X�̍쐬���ɒ��ׂ� D3DFMT_X8R8G8B8 �ɂ��邱�ƁB�ڂ����̓f�o�C�X�̍쐬�Q�ƁB
	capture_back_buffer_texture = G_rsm.create_texture(_T("�������"), width, height, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, true);
	if (!capture_back_buffer_texture)	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�L���v�`���[�p�̃e�N�X�`���̍쐬�Ɏ��s���܂����B(%d x %d)\n"), width, height) + get_last_error_msg());
		return false;
	}
	capture_dpst_buffer_surface = G_rsm.create_dpst_buffer(_T("�������"), width, height, true);
	if (!capture_dpst_buffer_surface)	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�L���v�`���[�p�̐[�x�X�e���V���o�b�t�@�̍쐬�Ɏ��s���܂����B(%d x %d)\n"), width, height) + get_last_error_msg());
		return false;
	}
	
	// �f�[�^�擾�p�̃e�N�X�`�������
	// ������͓��ߓx�͕K�v�Ȃ̂� D3DFMT_A8R8G8B8 �ɂ���K�v������
	// D3DPOOL_SYSTEMMEM �łȂ���΂Ȃ�Ȃ��i�d�v�I D3DPOOL_MANAGED ���ƃG���[���o��j
	BSP<C_d3d_texture> temp_texture = G_rsm.create_texture(_T("�������"), width, height, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, false);
	if (!temp_texture)	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�L���v�`���[���o���p�̃e�N�X�`���̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	m_capture_back_buffer_texture = capture_back_buffer_texture;
	m_capture_dpst_buffer_surface = capture_dpst_buffer_surface;
	m_capture_temp_buffer_texture = temp_texture;
	m_capture_buffer_width = width;
	m_capture_buffer_height = height;

	return true;
}

// �L���v�`���[�o�b�t�@��j��
bool C_tnm_wnd::destroy_capture_buffer()
{
	m_capture_back_buffer_texture.reset();
	m_capture_dpst_buffer_surface.reset();
	m_capture_temp_buffer_texture.reset();
	m_capture_buffer_width = 0;
	m_capture_buffer_height = 0;

	return true;
}

// �L���v�`���[
bool C_tnm_wnd::disp_proc_capture_to_buffer()
{
	// �e�N�X�`���ɃL���v�`���[�摜���擾����
	if (!m_capture_back_buffer_texture->get_render_target_data(0, m_capture_temp_buffer_texture->get_surface(0).body()))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�L���v�`���[�摜�̃f�[�^���擾�ł��܂���ł����B\n") + get_last_error_msg());
		return false;
	}

	return true;
}

