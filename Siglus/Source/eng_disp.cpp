#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_tree.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_thumb_table.h"
#include	"data/tnm_fog.h"

#include	"element/elm_flag.h"
#include	"element/elm_world.h"
#include	"element/elm_screen.h"
#include	"element/elm_stage.h"
#include	"element/elm_msg_back.h"
#include	"element/elm_excall.h"

#include	"engine/eng_graphics.h"
#include	"engine/eng_fog.h"
#include	"engine/eng_scene.h"
#include	"engine/ifc_msg_back.h"
#include	"engine/ifc_sound.h"
#include	"engine/eng_etc.h"
#include	"engine/eng_save.h"

#if __USE_EMOTE
#include	"engine/eng_emote.h"
#endif

// �X�e�[�W���擾
inline C_elm_stage* STAGE(int excall_no, int stage_no)
{
	if (excall_no == 0)	{
		return &(*Gp_stage_list)[stage_no];
	}
	else	{
		return &Gp_excall->m_stage_list[stage_no];
	}
}

// ****************************************************************
// �E�B���h�E�F�Q�[���o�b�t�@���g�����ǂ����𔻒肷��
// ================================================================
bool is_use_game_buffer()
{
	// �Q�[���X�N���[���T�C�Y�̑傫�����W���łȂ���Ύg��
	if (Gp_global->game_screen_size != Gp_global->total_game_screen_size)
		return true;

	// �Q�[���X�N���[���T�C�Y�ƃX�N���[���T�C�Y���Ⴆ�Ύg��
	if (Gp_global->total_game_screen_size != Gp_global->total_screen_size)
		return true;

	// �Q�[���X�N���[���ʒu���W���łȂ���Ύg��
	if (Gp_global->total_game_screen_pos != C_point(0, 0))
		return true;

	// �t���V�F�C�N���Ȃ�g��
	if (Gp_screen->shake().m_no >= 0)
		return true;

	// �L���v�`���[���Ȃ�g��
	if (Gp_global->capture_type != TNM_CAPTURE_TYPE_NONE)
		return true;

	// �g��Ȃ�
	return false;
}

// ****************************************************************
// �E�B���h�E�F�t�H�[�}�b�g
// ================================================================
void C_tnm_wnd::init()
{
	// �w�i�F
	m_back_color = C_argb(255, 0, 0, 0);				// �����������{��
	m_game_back_color = C_argb(255, 0, 0, 0);
	m_wipe_back_color = C_argb(255, 0, 0, 0);
	//m_back_color = C_argb(255, 0, 0, 255);			// ���f�o�b�O���͂��������g���Ƃ�������
	//m_game_back_color = C_argb(255, 0, 255, 0);
	//m_wipe_back_color = C_argb(255, 255, 0, 0);

	m_ready_wipe_type = -1;

	// �p�����[�^
	m_first = true;
}

// ****************************************************************
// �`�揈��
// ================================================================
//		�ʏ펞�̓t�����g�𒼐ڕ`�悵�܂��B
//		���C�v���̓t�����g��`�悵����A�l�N�X�g�����C�v�`�悵�܂��B
//		�o�b�N��`�悷�邱�Ƃ͂���܂���B
// ================================================================
//		�X�N���[���o�b�t�@�F�ŏI�`��o�b�t�@�i������o�b�N�o�b�t�@�j
//		�Q�[���o�b�t�@�F�W���E�B���h�E���̍ŏI�o�b�t�@�B�t���X�N���[�����Ȃǂ͂�����g�k���ăX�N���[���o�b�t�@�ɕ`�悷��B
// ================================================================
bool C_tnm_wnd::disp_proc()
{
	// �X�e�[�g���e���v���[�g�ŏ���������
	G_d3d.state.set_template();

	// �X�N���[���o�b�t�@���N���A�i��ʓ]�������Ȃ��Ƃ��͏������Ȃ��j
	if (Gp_global->present_flag)
	{
		// �X�N���[���̃o�b�N�o�b�t�@�Ɛ[�x�X�e���V���o�b�t�@���擾���Ă���
		m_screen_back_buffer = m_view.get_back_buffer();
		m_screen_dpst_buffer = m_view.get_dpst_buffer();

		// �����_�[�^�[�Q�b�g���X�N���[���o�b�t�@�ɐݒ肷��
		m_view.set_render_target(m_screen_back_buffer.body());
		m_view.set_dpst_buffer(m_screen_dpst_buffer.body());

		// �r���[�|�[�g���e�N�X�`���S�̂ɂ���
		if (!G_d3d.device.set_view_port(C_point(0, 0), Gp_global->total_screen_size))
		{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�r���[�|�[�g�̐ݒ�Ɏ��s���܂����B\n") + get_last_error_msg());
			return false;
		}

		// �����_�[�^�[�Q�b�g�����ŃN���A
		m_view.clear_render_target(m_back_color);
	}

	// ���[�r�[�Đ����t���O
	bool movie_playing_flag = tnm_is_movie_playing() && !tnm_is_movie_readying();

	// ���[�r�[�Đ����͈ȉ��̏������s��Ȃ��i�������L���v�`���[���͍s���B�G���h�Z�[�u�̂��߁j
	if (!movie_playing_flag || Gp_global->capture_type != TNM_CAPTURE_TYPE_NONE)
	{
		// �Q�[���o�b�t�@��ݒ肷��
		{
			// �o�b�t�@�L���v�`���[�̏ꍇ�̓L���v�`���[�o�b�t�@���^�[�Q�b�g
			if (Gp_global->capture_type == TNM_CAPTURE_TYPE_BUFFER)
			{
				// �^�[�Q�b�g�o�b�t�@�̓L���v�`���[�o�b�t�@
				m_target_back_buffer = m_capture_back_buffer_texture->get_surface(0);
				m_target_dpst_buffer = *m_capture_dpst_buffer_surface;

				// �����_�[�^�[�Q�b�g���^�[�Q�b�g�o�b�t�@�ɐݒ肷��
				m_view.set_render_target(m_target_back_buffer.body());
				m_view.set_dpst_buffer(m_target_dpst_buffer.body());

				// �r���[�|�[�g���e�N�X�`���S�̂ɂ���
				G_d3d.device.set_view_port(C_point(0, 0), m_capture_back_buffer_texture->get_size_ex());

				// �����_�[�^�[�Q�b�g���N���A
				m_view.clear_render_target(C_argb(0, 0, 0, 0));

				// �r���[�|�[�g���o�b�t�@�T�C�Y�ɂ��킹��
				m_view.set_default_view_port(C_rect(0, 0, m_capture_buffer_width, m_capture_buffer_height));
			}
			// �Q�[���o�b�t�@���g��Ȃ��ꍇ�̓X�N���[���o�b�t�@���^�[�Q�b�g�o�b�t�@
			else if (!is_use_game_buffer())
			{
				// �Q�[���o�b�t�@�e�N�X�`�������
				if (m_game_back_buffer_texture)
					m_game_back_buffer_texture.reset();
				if (m_game_dpst_buffer_surface)
					m_game_dpst_buffer_surface.reset();

				// �^�[�Q�b�g�o�b�t�@�̓X�N���[���o�b�t�@
				m_target_back_buffer = m_screen_back_buffer;
				m_target_dpst_buffer = m_screen_dpst_buffer;

				// �r���[�|�[�g���Q�[����ʃT�C�Y�ɂ��킹��
				m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			}
			// �Q�[���o�b�t�@���g���ꍇ�̓Q�[���o�b�t�@���쐬
			else
			{
				// �Q�[���o�b�t�@�e�N�X�`�����Ȃ���ΐV�K�쐬
				if (!m_game_back_buffer_texture || !m_game_dpst_buffer_surface)	{
					tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�Q�[���o�b�t�@���쐬���܂��B"));

					// �O�̂��߁A��������Q�[���o�b�t�@���N���A����
					m_game_back_buffer_texture.reset();
					m_game_dpst_buffer_surface.reset();

					// �Q�[���o�b�t�@���쐬����
					C_size size = Gp_global->game_screen_size;
					m_game_back_buffer_texture = G_rsm.create_texture(_T("�ް��ޯ̧"), size.cx, size.cy, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, true);
					if (!m_game_back_buffer_texture)	{
						tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�Q�[���o�b�t�@�p�̃e�N�X�`���̍쐬�Ɏ��s���܂����B(%d x %d)\n"), size.cx, size.cy) + get_last_error_msg());
						return false;
					}
					m_game_dpst_buffer_surface = G_rsm.create_dpst_buffer(_T("�ް��ޯ̧"), size.cx, size.cy, true);
					if (!m_game_dpst_buffer_surface)	{
						tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("�Q�[���o�b�t�@�p�̐[�x�X�e���V���o�b�t�@�̍쐬�Ɏ��s���܂����B(%d x %d)\n"), size.cx, size.cy) + get_last_error_msg());
						return false;
					}

					tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("�Q�[���o�b�t�@���쐬���܂����B"));
				}

				// �Q�[���o�b�t�@���^�[�Q�b�g�o�b�t�@�ɂ���
				m_target_back_buffer = m_game_back_buffer_texture->get_surface(0);
				m_target_dpst_buffer = *m_game_dpst_buffer_surface;

				// �����_�[�^�[�Q�b�g���^�[�Q�b�g�o�b�t�@�ɐݒ肷��
				m_view.set_render_target(m_target_back_buffer.body());
				m_view.set_dpst_buffer(m_target_dpst_buffer.body());

				// �r���[�|�[�g���e�N�X�`���S�̂ɂ���
				G_d3d.device.set_view_port(C_point(0, 0), m_game_back_buffer_texture->get_size_ex());

				// �����_�[�^�[�Q�b�g���N���A
				m_view.clear_render_target(m_game_back_color);

				// �r���[�|�[�g���Q�[����ʃT�C�Y�ɂ��킹��
				m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));
			}
		}

		// �`�揈�����C��
		if (!disp_proc_main())
			return false;

		// ��ʓ]�����s���ꍇ�͑����ď���
		if (Gp_global->present_flag)	{

			// �Q�[���o�b�t�@���g���Ă���ꍇ�̓Q�[���o�b�t�@���X�N���[���ɕ`�悷��i���[�r�[�Đ����͍s��Ȃ��j
			if (is_use_game_buffer() && !movie_playing_flag)	{

				// �����_�[�^�[�Q�b�g���X�N���[���ɐݒ肷��
				m_view.set_render_target(m_screen_back_buffer.body());
				m_view.set_dpst_buffer(m_screen_dpst_buffer.body());			// === ��������͓����蔻��Ɋ֌W���Ȃ��␳�l�����߂�

				// �e�T�C�Y�̃G�C���A�X
				C_size gssize = Gp_global->game_screen_size;			// �Q�[���X�N���[���T�C�Y
				C_point t_gspos = Gp_global->total_game_screen_pos;		// ���v�Q�[���X�N���[���ʒu
				C_size t_gssize = Gp_global->total_game_screen_size;	// ���v�Q�[���X�N���[���T�C�Y

				// �t���V�F�C�N�̕␳�l�����߂�
				C_point shake_rep_pos;
				shake_rep_pos.x = Gp_screen->shake().m_cur_shake_x * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx;
				shake_rep_pos.y = Gp_screen->shake().m_cur_shake_y * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy;

				// �Q�[���X�v���C�g�̏����ݒ�
				C_d3d_sprite game_sprite;
				game_sprite.create_polygon_no_init();
				game_sprite.rp.disp = true;
				game_sprite.rp.d2_rect = true;
				game_sprite.rp.size = gssize;
				game_sprite.rp.pos.x = (float)(t_gspos.x + shake_rep_pos.x);
				game_sprite.rp.pos.y = (float)(t_gspos.y + shake_rep_pos.y);
				game_sprite.rp.scale.x = (float)t_gssize.cx / gssize.cx;
				game_sprite.rp.scale.y = (float)t_gssize.cy / gssize.cy;
				game_sprite.set_vertex_buffer(G_rect_vertex_buffer_d2_screen);
				game_sprite.set_index_buffer(G_rect_index_buffer);
				game_sprite.set_texture(m_game_back_buffer_texture, 0);

				// �r���[�|�[�g���v�Z�i�t���V�F�C�N���l���j
				int l = std::max<int>(0, std::max<int>(t_gspos.x, t_gspos.x + shake_rep_pos.x));
				int t = std::max<int>(0, std::max<int>(t_gspos.y, t_gspos.y + shake_rep_pos.y));
				int r = std::min<int>(Gp_global->total_screen_size.cx, std::min<int>(t_gspos.x + t_gssize.cx, t_gspos.x + t_gssize.cx + shake_rep_pos.x));
				int b = std::min<int>(Gp_global->total_screen_size.cy, std::min<int>(t_gspos.y + t_gssize.cy, t_gspos.y + t_gssize.cy + shake_rep_pos.y));

				// �r���[�|�[�g����ʃT�C�Y�ɂ��킹��i�t���V�F�C�N���l���j
				m_view.set_default_view_port(C_rect(l, t, r, b));

				// �Q�[���X�v���C�g���X�V����
				void* p_locked_vertex_buffer;
				G_rect_vertex_buffer_d2_screen->lock(0, 0, &p_locked_vertex_buffer, D3DLOCK_DISCARD);
				if (game_sprite.set_d2_vertex_param(0, 4, 0, 6, 2, (BYTE *)p_locked_vertex_buffer, NULL, NULL))	{
					G_rect_vertex_buffer_d2_screen->unlock();

					// �Q�[���X�v���C�g��`��
					if (!disp_proc_sprite(&game_sprite))
						return false;
				}
				else	{
					G_rect_vertex_buffer_d2_screen->unlock();
				}
			}
		}
	}

	// �X�N���[���o�b�t�@���������
	m_screen_back_buffer.release();
	m_screen_dpst_buffer.release();

	// �^�[�Q�b�g�o�b�t�@���������
	m_target_back_buffer.release();
	m_target_dpst_buffer.release();

	return true;
}

// ****************************************************************
// �`�揈�����C��
// ================================================================
bool C_tnm_wnd::disp_proc_main()
{
	// ���C�v���łȂ��ꍇ
	if (!Gp_wipe->is_wipe_doing())	{

		// �V�X�e�����C�v��
		if (Gp_global->system_wipe_flag)	{

			// �����`�悵�Ȃ�
		}
		else	{

			// �S�ẴI�[�_�[��`�揀������
			if (!disp_proc_all_order_ready(TNM_STAGE_FRONT))
				return false;
			// �S�ẴI�[�_�[��`�悷��
			if (!disp_proc_all_order(TNM_STAGE_FRONT))
				return false;
		}
	}
	// ���C�v���̏ꍇ
	else	{

		// ���C�v�`�揈��
		if (!disp_proc_wipe())
			return false;
	}

	return true;
}

// ****************************************************************
// ���C�v�o�b�t�@�O����������
// ================================================================
bool C_tnm_wnd::disp_proc_ready_wipe_buffer_0()
{
	// ���C�v�o�b�t�@�p�̃e�N�X�`���̃T�[�t�F�X���擾
	C_d3d_surface wipe_buffer_surface = m_wipe_back_buffer_texture_0->get_surface(0);
	if (!wipe_buffer_surface.body())
		return false;

	// �����_�[�^�[�Q�b�g�����C�v�o�b�t�@�ɐݒ肷��
	m_view.set_render_target(wipe_buffer_surface.body());
	m_view.set_dpst_buffer(m_wipe_dpst_buffer_surface_0->body());

	// �r���[�|�[�g���e�N�X�`���S�̂ɂ���
	G_d3d.device.set_view_port(C_point(0, 0), m_wipe_back_buffer_texture_0->get_size_ex());

	// �����_�[�^�[�Q�b�g�𓧖��ŃN���A�I
	if (!m_view.clear_render_target(C_argb(0, 0, 0, 0)))
		return false;

	// �r���[�|�[�g���Q�[���T�C�Y�ɂ��킹��
	m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));

	return true;
}

// ****************************************************************
// ���C�v�o�b�t�@�P����������
// ================================================================
bool C_tnm_wnd::disp_proc_ready_wipe_buffer_1()
{
	// ���C�v�o�b�t�@�p�̃e�N�X�`���̃T�[�t�F�X���擾
	C_d3d_surface wipe_buffer_surface = m_wipe_back_buffer_texture_1->get_surface(0);
	if (!wipe_buffer_surface.body())
		return false;

	// �����_�[�^�[�Q�b�g�����C�v�o�b�t�@�ɐݒ肷��
	m_view.set_render_target(wipe_buffer_surface.body());
	m_view.set_dpst_buffer(m_wipe_dpst_buffer_surface_0->body());

	// �r���[�|�[�g���e�N�X�`���S�̂ɂ���
	G_d3d.device.set_view_port(C_point(0, 0), m_wipe_back_buffer_texture_1->get_size_ex());

	// �����_�[�^�[�Q�b�g�𓧖��ŃN���A�I
	if (!m_view.clear_render_target(C_argb(0, 0, 0, 0)))
		return false;

	// �r���[�|�[�g���Q�[���T�C�Y�ɂ��킹��
	m_view.set_default_view_port(C_rect(C_point(0, 0), Gp_global->game_screen_size));

	return true;
}

// ****************************************************************
// ���C�v�`��F���C�v�X�v���C�g��������
// ================================================================
bool C_tnm_wnd::disp_proc_init_wipe_sprite(C_d3d_sprite* p_sprite)
{
	p_sprite->rp.disp = true;
	p_sprite->rp.d2_rect = true;
	p_sprite->rp.alpha_test = true;
	p_sprite->rp.alpha_blend = true;
	p_sprite->rp.blend_type = E_d3d_blend_type_alpha;
	p_sprite->rp.size = m_wipe_back_buffer_texture_0->get_size();
	p_sprite->set_texture(m_wipe_back_buffer_texture_0, 0);

	return true;
}

// ****************************************************************
// ���C�v�`��F�S�ẴI�[�_�[��`�揀������
// ================================================================
bool C_tnm_wnd::disp_proc_all_order_ready(int stage_no)
{
	if (stage_no == -1)
		return true;

	// �S�ẴI�[�_�[��`�揀������
	if (!disp_proc_stage_ready(stage_no, S_tnm_sorter(INT_MIN, INT_MIN), S_tnm_sorter(INT_MAX, INT_MAX)))
		return false;

	return true;
}

// ****************************************************************
// ���C�v�`��F�S�ẴI�[�_�[��`�悷��
// ================================================================
bool C_tnm_wnd::disp_proc_all_order(int stage_no)
{
	if (stage_no == -1)
		return true;

	// �S�ẴI�[�_�[��`�悷��
	if (!disp_proc_stage(stage_no, S_tnm_sorter(INT_MIN, INT_MIN), S_tnm_sorter(INT_MAX, INT_MAX)))
		return false;

	return true;
}

// ****************************************************************
// ���C�v�`��F���C�v��艺�̃I�[�_�[��`�揀������
// ================================================================
bool C_tnm_wnd::disp_proc_under_order_ready(int stage_no)
{
	if (stage_no == -1)
		return true;

	// �V�X�e�����C�v�̏ꍇ
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_IN || Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_OUT)	{

		// ���C�v��艺�͂��肦�Ȃ��̂ŉ������Ȃ�
	}
	// �d�w�R�[�����C�v�̏ꍇ
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_EX_CALL)	{

		// ���C�v��艺��`�揀������
		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		if (begin_sorter != S_tnm_sorter(INT_MIN, INT_MIN))	{
			begin_sorter += S_tnm_sorter(Gp_ini->excall_order, 0);
			if (!disp_proc_stage_ready(stage_no, S_tnm_sorter(INT_MIN, INT_MIN), begin_sorter.minus_1()))
				return false;
		}
	}
	// �ʏ�̃��C�v�̏ꍇ
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_NORMAL)	{

		// ���C�v��艺��`�揀������
		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		if (begin_sorter != S_tnm_sorter(INT_MIN, INT_MIN))	{
			if (!disp_proc_stage_ready(stage_no, S_tnm_sorter(INT_MIN, INT_MIN), begin_sorter.minus_1()))
				return false;
		}
	}

	return true;
}

// ****************************************************************
// ���C�v�`��F���C�v��艺�̃I�[�_�[��`�悷��
// ================================================================
bool C_tnm_wnd::disp_proc_under_order(int stage_no)
{
	if (stage_no == -1)
		return true;

	// �V�X�e�����C�v�̏ꍇ
	if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_IN || Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_OUT)	{

		// ���C�v��艺�͂��肦�Ȃ��̂ŉ������Ȃ�
	}
	// �d�w�R�[�����C�v�̏ꍇ
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_EX_CALL)	{

		// ���C�v��艺��`�悷��
		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		if (begin_sorter != S_tnm_sorter(INT_MIN, INT_MIN))	{
			begin_sorter += S_tnm_sorter(Gp_ini->excall_order, 0);
			if (!disp_proc_stage(stage_no, S_tnm_sorter(INT_MIN, INT_MIN), begin_sorter.minus_1()))
				return false;
		}
	}
	// �ʏ�̃��C�v�̏ꍇ
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_NORMAL)	{

		// ���C�v��艺��`�悷��
		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		if (begin_sorter != S_tnm_sorter(INT_MIN, INT_MIN))	{
			if (!disp_proc_stage(stage_no, S_tnm_sorter(INT_MIN, INT_MIN), begin_sorter.minus_1()))
				return false;
		}
	}

	return true;
}

// ****************************************************************
// ���C�v�`��F���C�v����I�[�_�[��`�揀������
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_order_ready(int stage_no)
{
	if (stage_no == -1)
		return true;

	// �V�X�e�����C�v�̏ꍇ
	if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_IN || Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_OUT)	{

		// �V�X�e�����C�v�A�E�g���́A�u�t�����g�����v�Ƀ��C�v���܂��B�t�����g�����A�l�N�X�g���t�����g�@�Ȃ̂ł��B
		if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_OUT)	{
			if (stage_no == TNM_STAGE_NEXT)
				stage_no = TNM_STAGE_FRONT;
			else
				return true;
		}

		// ���C�v����I�[�_�[��`�悷��
		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (!disp_proc_stage_ready(stage_no, begin_sorter, end_sorter))
			return false;
	}
	// �d�w�R�[�����C�v�̏ꍇ
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_EX_CALL)	{

		// ���C�v����I�[�_�[��`�悷��
		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (begin_sorter != S_tnm_sorter(INT_MAX, INT_MAX))	{
			begin_sorter += S_tnm_sorter(Gp_ini->excall_order, 0);
		}
		if (end_sorter != S_tnm_sorter(INT_MAX, INT_MAX))	{
			end_sorter += S_tnm_sorter(Gp_ini->excall_order, 0);
		}
		if (!disp_proc_stage_ready(stage_no, begin_sorter, end_sorter))
			return false;
	}
	// �ʏ�̃��C�v�̏ꍇ
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_NORMAL)	{

		// ���C�v����I�[�_�[��`�悷��
		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (!disp_proc_stage_ready(stage_no, begin_sorter, end_sorter))
			return false;
	}

	return true;
}

// ****************************************************************
// ���C�v�`��F���C�v����I�[�_�[��`�悷��
// ================================================================
bool C_tnm_wnd::disp_proc_wipe_order(int stage_no)
{
	if (stage_no == -1)
		return true;

	// �V�X�e�����C�v�̏ꍇ
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_IN || Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_OUT)	{

		// �V�X�e�����C�v�A�E�g���́A�u�t�����g�����v�Ƀ��C�v���܂��B�t�����g�����A�l�N�X�g���t�����g�@�Ȃ̂ł��B
		if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_OUT)	{
			if (stage_no == TNM_STAGE_NEXT)
				stage_no = TNM_STAGE_FRONT;
			else
				return true;
		}

		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (!disp_proc_stage(stage_no, begin_sorter, end_sorter))
			return false;
	}
	// �d�w�R�[�����C�v�̏ꍇ
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_EX_CALL)	{

		// ���C�v����I�[�_�[��`�悷��
		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (begin_sorter != S_tnm_sorter(INT_MAX, INT_MAX))	{
			begin_sorter += S_tnm_sorter(Gp_ini->excall_order, 0);
		}
		if (end_sorter != S_tnm_sorter(INT_MAX, INT_MAX))	{
			end_sorter += S_tnm_sorter(Gp_ini->excall_order, 0);
		}
		if (!disp_proc_stage(stage_no, begin_sorter, end_sorter))
			return false;
	}
	// �ʏ�̃��C�v�̏ꍇ
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_NORMAL)	{

		// ���C�v����I�[�_�[��`�悷��
		S_tnm_sorter begin_sorter = Gp_wipe->m_begin_sorter;
		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (!disp_proc_stage(stage_no, begin_sorter, end_sorter))
			return false;
	}

	return true;
}

// ****************************************************************
// ���C�v�`��F���C�v����̃I�[�_�[��`�揀������
// ================================================================
bool C_tnm_wnd::disp_proc_over_order_ready(int stage_no)
{
	if (stage_no == -1)
		return true;

	// �V�X�e�����C�v�̏ꍇ
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_IN || Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_OUT)	{

		// ���C�v����͂��肦�Ȃ��̂ŉ������Ȃ�
	}
	// �d�w�R�[�����C�v�̏ꍇ
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_EX_CALL)	{

		// ���C�v�����`�揀������
		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (end_sorter != S_tnm_sorter(INT_MAX, INT_MAX))	{
			end_sorter += S_tnm_sorter(Gp_ini->excall_order, 0);
			if (!disp_proc_stage_ready(stage_no, end_sorter.plus_1(), S_tnm_sorter(INT_MAX, INT_MAX)))
				return false;
		}
	}
	// �ʏ�̃��C�v�̏ꍇ
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_NORMAL)	{

		// ���C�v�����`�揀������
		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (end_sorter != S_tnm_sorter(INT_MAX, INT_MAX))	{
			if (!disp_proc_stage_ready(stage_no, end_sorter.plus_1(), S_tnm_sorter(INT_MAX, INT_MAX)))
				return false;
		}
	}

	return true;
}

// ****************************************************************
// ���C�v�`��F���C�v����̃I�[�_�[��`�悷��
// ================================================================
bool C_tnm_wnd::disp_proc_over_order(int stage_no)
{
	if (stage_no == -1)
		return true;

	// �V�X�e�����C�v�̏ꍇ
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_IN || Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_SYSTEM_OUT)	{

		// ���C�v����͂��肦�Ȃ��̂ŉ������Ȃ�
	}
	// �d�w�R�[�����C�v�̏ꍇ
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_EX_CALL)	{

		// ���C�v�����`�悷��
		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (end_sorter != S_tnm_sorter(INT_MAX, INT_MAX))	{
			end_sorter += S_tnm_sorter(Gp_ini->excall_order, 0);
			if (!disp_proc_stage(stage_no, end_sorter.plus_1(), S_tnm_sorter(INT_MAX, INT_MAX)))
				return false;
		}
	}
	// �ʏ�̃��C�v�̏ꍇ
	else if (Gp_wipe->m_wipe_range == TNM_WIPE_RANGE_NORMAL)	{

		S_tnm_sorter end_sorter = Gp_wipe->m_end_sorter;
		if (end_sorter != S_tnm_sorter(INT_MAX, INT_MAX))	{
			if (!disp_proc_stage(stage_no, end_sorter.plus_1(), S_tnm_sorter(INT_MAX, INT_MAX)))
				return false;
		}
	}

	return true;
}

// ****************************************************************
// ���C�v�`��F�X�e�[�W��`�悷��i�����j
// ================================================================
void disp_proc_effect_to_sprite_list(ARRAY<C_tnm_sprite *>& sprite_list, C_elm_effect* p_effect)
{
	ARRAY<C_tnm_sprite *>::iterator sprite_itr;
	ARRAY<C_tnm_sprite *>::iterator sprite_end;

	if (p_effect->is_use())	{

		// �G�t�F�N�g�̃����_�[�f�[�^���擾
		S_d3d_render_param effect_rp;
		p_effect->get_rp(&effect_rp);

		// �G�t�F�N�g���g�p�����ꍇ�A�S�X�v���C�g�ɑ΂��ēK�p
		if (effect_rp.disp)	{
			sprite_end = sprite_list.end();
			for (sprite_itr = sprite_list.begin(); sprite_itr != sprite_end; ++sprite_itr)	{
				// �X�v���C�g���I�[�_�[���ɓ����Ă���ꍇ�̂ݓK�p
				if (p_effect->get_begin_sorter() <= (*sprite_itr)->rpex.sorter && (*sprite_itr)->rpex.sorter <= p_effect->get_end_sorter())	{
					tnm_affect_sp_and_rp(NULL, NULL, &(*sprite_itr)->rp, &effect_rp);
				}
			}
		}
	}
}
void disp_proc_quake_to_sprite_list(ARRAY<C_tnm_sprite *>& sprite_list, C_elm_quake* p_quake)
{
	ARRAY<C_tnm_sprite *>::iterator sprite_itr;
	ARRAY<C_tnm_sprite *>::iterator sprite_end;

	if (p_quake->is_quake_doing())	{

		// �N�G�C�N�̃����_�[�f�[�^���擾
		S_d3d_render_param quake_rp;
		p_quake->get_rp(&quake_rp);

		// �N�G�C�N���g�p�����ꍇ�A�S�X�v���C�g�ɑ΂��ēK�p
		if (quake_rp.disp)	{
			sprite_end = sprite_list.end();
			for (sprite_itr = sprite_list.begin(); sprite_itr != sprite_end; ++sprite_itr)	{
				// �X�v���C�g���I�[�_�[���ɓ����Ă���ꍇ�̂ݓK�p
				if (p_quake->get_begin_order() <= (*sprite_itr)->rpex.sorter.order && (*sprite_itr)->rpex.sorter.order <= p_quake->get_end_order())	{
					tnm_affect_sp_and_rp(NULL, NULL, &(*sprite_itr)->rp, &quake_rp);
				}
			}
		}
	}
}


bool C_tnm_wnd::disp_proc_stage_ready(int stage_no, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter)
{
	//
	// �d�w�R�[���O
	//
	{
		// ���[�g�X�v���C�g���쐬
		C_tnm_sprite root_sprite;
		C_tree2<C_tnm_sprite *> root;
		root.data = &root_sprite;

		// �X�e�[�W�̃X�v���C�g�c���[���擾����
		STAGE(0, stage_no)->get_sprite_tree(root, begin_sorter, end_sorter);

		// �X�v���C�g�����X�g�ɂ���
		ARRAY<C_tnm_sprite *> sprite_list;
		disp_proc_sprite_tree_to_sprite_list(root, sprite_list);

		// �X�N���[���̃G�t�F�N�g���擾
		{
			int effect_cnt = Gp_screen->effect_list().get_size();
			for (int i = 0; i < effect_cnt; i++)	{
				C_elm_effect* p_effect = &Gp_screen->effect_list()[i];
				disp_proc_effect_to_sprite_list(sprite_list, p_effect);
			}
		}

		// �N�G�C�N��~�t���O�������Ă��Ȃ��ꍇ
		if (!Gp_local->pod.quake_stop_flag)	{

			// �X�N���[���̃N�G�C�N���擾
			int quake_cnt = Gp_screen->quake_list().get_size();
			for (int i = 0; i < quake_cnt; i++)	{
				C_elm_quake* p_quake = &Gp_screen->quake_list()[i];
				disp_proc_quake_to_sprite_list(sprite_list, p_quake);
			}
		}

		// �X�e�[�W�̃G�t�F�N�g���擾
		{
			int effect_cnt = STAGE(0, stage_no)->m_effect_list.get_size();
			for (int i = 0; i < effect_cnt; i++)	{
				C_elm_effect* p_effect = &STAGE(0, stage_no)->m_effect_list[i];
				disp_proc_effect_to_sprite_list(sprite_list, p_effect);
			}
		}

		// �N�G�C�N��~�t���O�������Ă��Ȃ��ꍇ
		if (!Gp_local->pod.quake_stop_flag)	{

			// �X�e�[�W�̃N�G�C�N���擾
			int quake_cnt = STAGE(0, stage_no)->m_quake_list.get_size();
			for (int i = 0; i < quake_cnt; i++)	{
				C_elm_quake* p_quake = &STAGE(0, stage_no)->m_quake_list[i];
				disp_proc_quake_to_sprite_list(sprite_list, p_quake);
			}
		}

		// �L���v�`���[�ʒu���w�肷��ꍇ
		if (Gp_global->capture_type == TNM_CAPTURE_TYPE_BUFFER)	{

			for (ARRAY<C_tnm_sprite *>::iterator itr = sprite_list.begin(); itr != sprite_list.end(); ++itr)	{
				(*itr)->rp.pos.x += Gp_global->capture_x;
				(*itr)->rp.pos.y += Gp_global->capture_y;
			}
		}
	}

	//
	// �d�w�R�[���P
	//
	if (Gp_excall->is_ready())	{

		// ���[�g�X�v���C�g���쐬
		C_tnm_sprite root_sprite;
		C_tree2<C_tnm_sprite *> root;
		root.data = &root_sprite;

		// �X�e�[�W�̃X�v���C�g�c���[���擾����
		STAGE(1, stage_no)->get_sprite_tree(root, begin_sorter, end_sorter);

		// �X�v���C�g�����X�g�ɂ���
		ARRAY<C_tnm_sprite *> sprite_list;
		disp_proc_sprite_tree_to_sprite_list(root, sprite_list);

		// �X�e�[�W�̃G�t�F�N�g���擾
		{
			int effect_cnt = STAGE(1, stage_no)->m_effect_list.get_size();
			for (int i = 0; i < effect_cnt; i++)	{
				C_elm_effect* p_effect = &STAGE(1, stage_no)->m_effect_list[i];
				disp_proc_effect_to_sprite_list(sprite_list, p_effect);
			}
		}

		// �N�G�C�N��~�t���O�������Ă��Ȃ��ꍇ
		if (!Gp_local->pod.quake_stop_flag)	{

			// �X�e�[�W�̃N�G�C�N���擾
			int quake_cnt = STAGE(1, stage_no)->m_quake_list.get_size();
			for (int i = 0; i < quake_cnt; i++)	{
				C_elm_quake* p_quake = &STAGE(1, stage_no)->m_quake_list[i];
				disp_proc_quake_to_sprite_list(sprite_list, p_quake);
			}
		}
	}


#if 0


	// �X�e�[�W�̃X�v���C�g�c���[���擾����
	STAGE(0, stage_no)->get_sprite_tree(root, begin_sorter, end_sorter);
	//if (Gp_excall->is_ready())	{
	//	STAGE(1, stage_no)->get_sprite_tree(root, begin_sorter, end_sorter);
	//}
	//p_stage->get_sprite_tree(root, begin_sorter, end_sorter);

	// �X�v���C�g�����X�g�ɂ���
	ARRAY<C_tnm_sprite *> sprite_list;
	disp_proc_sprite_tree_to_sprite_list(root, sprite_list);

	//
	// �X�N���[���̃����_�[�f�[�^��K�p����
	//
	if (p_screen)	{

		// �X�N���[���̃G�t�F�N�g���擾
		int effect_cnt = p_screen->effect_list().get_size();
		for (int i = 0; i < effect_cnt; i++)	{
			C_elm_effect* p_effect = &p_screen->effect_list()[i];
			if (p_effect->is_use())	{

				// �G�t�F�N�g�̃����_�[�f�[�^���擾
				S_d3d_render_param effect_rp;
				p_effect->get_rp(&effect_rp);

				// �G�t�F�N�g���g�p�����ꍇ�A�S�X�v���C�g�ɑ΂��ēK�p
				if (effect_rp.disp)	{
					sprite_end = sprite_list.end();
					for (sprite_itr = sprite_list.begin(); sprite_itr != sprite_end; ++sprite_itr)	{
						// �X�v���C�g���I�[�_�[���ɓ����Ă���ꍇ�̂ݓK�p
						if (p_effect->get_begin_sorter() <= (*sprite_itr)->rpex.sorter && (*sprite_itr)->rpex.sorter <= p_effect->get_end_sorter())	{
							tnm_affect_sp_and_rp(NULL, NULL, &(*sprite_itr)->rp, &effect_rp);
						}
					}
				}
			}
		}

		// �N�G�C�N��~�t���O�������Ă��Ȃ��ꍇ
		if (!Gp_local->quake_stop_flag)	{

			// �X�N���[���̃N�G�C�N���擾
			int quake_cnt = p_screen->quake_list().get_size();
			for (int i = 0; i < quake_cnt; i++)	{
				C_elm_quake* p_quake = &p_screen->quake_list()[i];
				if (p_quake->is_quake_doing())	{

					// �N�G�C�N�̃����_�[�f�[�^���擾
					S_d3d_render_param quake_rp;
					p_quake->get_rp(&quake_rp);

					// �N�G�C�N���g�p�����ꍇ�A�S�X�v���C�g�ɑ΂��ēK�p
					if (quake_rp.disp)	{
						sprite_end = sprite_list.end();
						for (sprite_itr = sprite_list.begin(); sprite_itr != sprite_end; ++sprite_itr)	{
							// �X�v���C�g���I�[�_�[���ɓ����Ă���ꍇ�̂ݓK�p
							if (p_quake->get_begin_order() <= (*sprite_itr)->rpex.sorter.order && (*sprite_itr)->rpex.sorter.order <= p_quake->get_end_order())	{
								tnm_affect_sp_and_rp(NULL, NULL, &(*sprite_itr)->rp, &quake_rp);
							}
						}
					}
				}
			}
		}
	}

	//
	// �X�e�[�W�̃����_�[�f�[�^��K�p����
	//

	{

		// �X�e�[�W�̃G�t�F�N�g���擾
		int effect_cnt = p_stage->m_effect_list.get_size();
		for (int i = 0; i < effect_cnt; i++)	{
			C_elm_effect* p_effect = &p_stage->m_effect_list[i];
			if (p_effect->is_use())	{

				// �G�t�F�N�g�̃����_�[�f�[�^���擾
				S_d3d_render_param effect_rp;
				p_effect->get_rp(&effect_rp);

				// �G�t�F�N�g���g�p�����ꍇ�A�S�X�v���C�g�ɑ΂��ēK�p
				if (effect_rp.disp)	{

					sprite_end = sprite_list.end();
					for (sprite_itr = sprite_list.begin(); sprite_itr != sprite_end; ++sprite_itr)	{
						// �X�v���C�g���I�[�_�[���ɓ����Ă���ꍇ�̂ݓK�p
						if (p_effect->get_begin_sorter() <= (*sprite_itr)->rpex.sorter && (*sprite_itr)->rpex.sorter <= p_effect->get_end_sorter())	{
							tnm_affect_sp_and_rp(NULL, NULL, &(*sprite_itr)->rp, &effect_rp);
						}
					}
				}
			}
		}

		// �N�G�C�N��~�t���O�������Ă��Ȃ��ꍇ
		if (!Gp_local->quake_stop_flag)	{

			// �X�e�[�W�̃N�G�C�N���擾
			int quake_cnt = p_stage->m_quake_list.get_size();
			for (int i = 0; i < quake_cnt; i++)	{
				C_elm_quake* p_quake = &p_stage->m_quake_list[i];
				if (p_quake->is_quake_doing())	{

					// �N�G�C�N�̃����_�[�f�[�^���擾
					S_d3d_render_param quake_rp;
					p_quake->get_rp(&quake_rp);

					// �N�G�C�N���g�p�����ꍇ�A�S�X�v���C�g�ɑ΂��ēK�p
					if (quake_rp.disp)	{
						sprite_end = sprite_list.end();
						for (sprite_itr = sprite_list.begin(); sprite_itr != sprite_end; ++sprite_itr)	{
							// �X�v���C�g���I�[�_�[���ɓ����Ă���ꍇ�̂ݓK�p
							if (p_quake->get_begin_order() <= (*sprite_itr)->rpex.sorter.order && (*sprite_itr)->rpex.sorter.order <= p_quake->get_end_order())	{
								tnm_affect_sp_and_rp(NULL, NULL, &(*sprite_itr)->rp, &quake_rp);
							}
						}
					}
				}
			}
		}
	}
#endif

	return true;
}

// ****************************************************************
// ���C�v�`��F�X�e�[�W��`�悷��F��P�i�K�F�L���v�`���[�̑O��ŕ�����
// ================================================================
bool C_tnm_wnd::disp_proc_stage(int stage_no, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter)
{
	// �L���v�`���[�͈̔͂ŕ����Ă݂悤�c
	S_tnm_sorter capture_sorter = Gp_global->capture_sorter;

	// �L���v�`���[�����ޏꍇ
	if (Gp_global->capture_type != TNM_CAPTURE_TYPE_NONE && stage_no == TNM_STAGE_FRONT && begin_sorter <= capture_sorter && capture_sorter <= end_sorter)	{

		// �L���v�`���[��艺
		disp_proc_stage_sub(stage_no, begin_sorter, capture_sorter);

		// �L���v�`���[
		if (Gp_global->capture_type == TNM_CAPTURE_TYPE_SAVE_THUMB)	{
			if (!disp_proc_capture_for_save_thumb())	// ���̒��Ń^�[�Q�b�g�o�b�t�@���ς���Ă܂��I
				return false;
		}
		else if (Gp_global->capture_type == TNM_CAPTURE_TYPE_OBJECT)	{
			if (!disp_proc_capture_for_object())		// ���̒��Ń^�[�Q�b�g�o�b�t�@���ς���Ă܂��I
				return false;
		}
		else if (Gp_global->capture_type == TNM_CAPTURE_TYPE_THUMB)	{
			if (!disp_proc_capture_for_thumb())			// ���̒��Ń^�[�Q�b�g�o�b�t�@���ς���Ă܂��I
				return false;
		}
		else if (Gp_global->capture_type == TNM_CAPTURE_TYPE_BUFFER)	{
			if (!disp_proc_capture_to_buffer())			// ���̒��Ń^�[�Q�b�g�o�b�t�@���ς���Ă܂��I
				return false;
		}
		else if (Gp_global->capture_type == TNM_CAPTURE_TYPE_TWEET)	{
			if (!disp_proc_capture_for_tweet())			// ���̒��Ń^�[�Q�b�g�o�b�t�@���ς���Ă܂��I
				return false;
		}

		// ���[�J���t�@�C���ɕۑ��i�Z�[�u�T���l�C���j
		if (Gp_global->capture_type == TNM_CAPTURE_TYPE_THUMB && m_capture_texture_for_object)	{
			int width = m_capture_texture_for_object->get_width();
			int height = m_capture_texture_for_object->get_height();

			D3DLOCKED_RECT locked_rect;
			if (!m_capture_texture_for_object->lock_rect(0, &locked_rect, NULL, D3DLOCK_DISCARD))	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���[�J���t�@�C���ւ̃L���v�`���[�Ɏ��s���܂����B\n") + get_last_error_msg());
			}
			else	{

				time_t now = time(NULL);
				TSTR file_path = Gp_dir->save_dir + _T("\\") + tostr(now) + _T(".png");
				tnm_create_png_from_texture_and_save_to_file(file_path, width, height, &locked_rect, false);	// �A���t�@�l���g��Ȃ�

				Gp_global->capture_time = (int)now;

				m_capture_texture_for_object->unlock_rect(0);
			}
			m_capture_texture_for_object.reset();
		}
		// ���[�J���t�@�C���ɕۑ��ipng �o�́j
		else if (Gp_global->capture_type == TNM_CAPTURE_TYPE_BUFFER && !Gp_global->capture_file_name.empty())	{

			int width = m_capture_temp_buffer_texture->get_width();
			int height = m_capture_temp_buffer_texture->get_height();

			// �������̓Z�[�u�t�H���_�Œ�
			TSTR file_path = Gp_dir->save_dir + _T("\\") + tostr(Gp_global->capture_file_name) + _T(".png");

			// �w��̃t�@�C���ɕۑ�
			D3DLOCKED_RECT locked_rect;
			if (!m_capture_temp_buffer_texture->lock_rect(0, &locked_rect, NULL, D3DLOCK_DISCARD))	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�L���v�`���[�o�b�t�@�̃��b�N�Ɏ��s���܂����B\n") + get_last_error_msg());
			}
			else	{

				tnm_create_png_from_texture_and_save_to_file(file_path, width, height, &locked_rect, true);	// �A���t�@�l���g��

				m_capture_temp_buffer_texture->unlock_rect(0);
			}
		}

		// ��ʓ]�����s���ꍇ�͑����ď���
		if (Gp_global->present_flag)	{

			// �����_�[�^�[�Q�b�g���^�[�Q�b�g�o�b�t�@�ɖ߂�
			m_view.set_render_target(m_target_back_buffer.body());
			m_view.set_dpst_buffer(m_target_dpst_buffer.body());

			// �L���v�`���[����
			if (capture_sorter < end_sorter)	{
				disp_proc_stage_sub(stage_no, capture_sorter.plus_1(), end_sorter);
			}
		}
	}
	else	{

		// �w�肳�ꂽ�S�ẴI�[�_�[
		disp_proc_stage_sub(stage_no, begin_sorter, end_sorter);
	}

	return true;
}

// ****************************************************************
// ���C�v�`��F�X�e�[�W��`�悷��F��Q�i�K�F�X�v���C�g���X�g���\�z���ĕ`�悷��
// ================================================================
bool C_tnm_wnd::disp_proc_stage_sub(int stage_no, S_tnm_sorter begin_sorter, S_tnm_sorter end_sorter)
{
	// ���[�g�X�v���C�g���쐬
	C_tnm_sprite root_sprite;
	C_tree2<C_tnm_sprite *> root;
	root.data = &root_sprite;

	// �X�e�[�W�̃X�v���C�g�c���[���擾����
	STAGE(0, stage_no)->get_sprite_tree(root, begin_sorter, end_sorter);

	// ���b�Z�[�W�o�b�N
	if (!Gp_local->pod.msg_back_disp_off)	{
		Gp_msg_back->get_sprite_tree(root);
	}

	// �d�w�R�[��
	if (Gp_excall->is_ready())	{
		STAGE(1, stage_no)->get_sprite_tree(root, begin_sorter, end_sorter);
	}

	// �X�v���C�g�����X�g�ɂ���
	ARRAY<C_tnm_sprite *> sprite_list;
	disp_proc_sprite_tree_to_sprite_list(root, sprite_list);

	// �X�v���C�g���X�g��`�悷��
	if (!disp_proc_sprite_list(sprite_list))
		return false;

	return true;
}

// ****************************************************************
// �X�v���C�g�c���[����`�悷��X�v���C�g���X�g���쐬����
// ================================================================

inline bool node_cmp_default(C_tree2<C_tnm_sprite *> lhs, C_tree2<C_tnm_sprite *> rhs)
{
	return lhs.data->rpex.sorter < rhs.data->rpex.sorter;
}
inline bool node_cmp_pat_no(C_tree2<C_tnm_sprite *> lhs, C_tree2<C_tnm_sprite *> rhs)
{
	if (lhs.data->rpex.sorter < rhs.data->rpex.sorter)	return true;
	if (lhs.data->rpex.sorter > rhs.data->rpex.sorter)	return false;
	if (lhs.data->get_texture(0).get() < rhs.data->get_texture(0).get())	return true;
	return false;
}
inline bool node_cmp_test(C_tree2<C_tnm_sprite *> lhs, C_tree2<C_tnm_sprite *> rhs)
{
	if (lhs.data->rpex.sorter < rhs.data->rpex.sorter)	return true;
	if (lhs.data->rpex.sorter > rhs.data->rpex.sorter)	return false;
	if (lhs.data->rp.pos.z < rhs.data->rp.pos.z)		return true;
	if (lhs.data->rp.pos.z > rhs.data->rp.pos.z)		return false;
	if (lhs.data->get_texture(0).get() < rhs.data->get_texture(0).get())	return true;
	return false;
}

bool C_tnm_wnd::disp_proc_sprite_tree_to_sprite_list(C_tree2<C_tnm_sprite *>& node, ARRAY<C_tnm_sprite *>& sprite_list)
{
	// �m�[�h�̃X�v���C�g�����X�g�ɒǉ�����
	if (node.data->type != E_d3d_sprite_type_none && node.data->rp.disp)	{

		// �X�v���C�g��o�^����
		sprite_list.push_back(node.data);
	}

	int child_cnt = (int)node.child_list.size();
	if (child_cnt > 0)	{

		// �m�[�h�̎q�����\�[�g����
		if (node.data->rpex.child_sort_type == TNM_SPRITE_SORT_TYPE_DEFAULT)	{
			std::stable_sort(node.child_list.begin(), node.child_list.end(), node_cmp_default);
		}

		// �S�Ă̎q���ɑ΂��ē��l�̏������s��
		for (int i = 0; i < child_cnt; i++)
			disp_proc_sprite_tree_to_sprite_list(node.child_list[i], sprite_list);
	}

	return true;
}

// ****************************************************************
// �X�v���C�g���X�g��`�悷��
// ================================================================
bool C_tnm_wnd::disp_proc_sprite_list_sub(C_elm_world* p_world, ARRAY<C_tnm_sprite *>& sprite_list)
{
	// �R�c�̏ꍇ
	/*
	if (world_no >= 0)	{

		// �u�����h������̂Ƃ��Ȃ����̂ɕ�����
		ARRAY<C_tnm_sprite *> blend_sprite_list, no_blend_sprite_list;
		for (ARRAY<C_tnm_sprite *>::iterator itr = sprite_list.begin(); itr != sprite_list.end(); ++itr)	{
			if ((*itr)->rp.alpha_blend)	{
				blend_sprite_list.push_back(*itr);
			}
			else	{
				no_blend_sprite_list.push_back(*itr);
			}
		}

		// �u�����h���Ȃ����̂�`��
		{
#if 0
			C_elm_world* p_world = &(*Gp_world_list)[world_no];
			int sprite_cnt = (int)no_blend_sprite_list.size();
			ARRAY<float> z_value(sprite_cnt);
			for (int i = 0; i < sprite_cnt; i++)	{
				D3DXVECTOR3 a = *(D3DXVECTOR3 *)&no_blend_sprite_list[i]->rp.pos - m_view.m_camera_eye;
				D3DXVECTOR3 b = m_view.m_camera_dir;
				D3DXVec3Normalize(&b, &b);
				float dot = D3DXVec3Dot(&a, &b);
				z_value[i] = dot;
			}

			for (int i = 0; i < sprite_cnt; i++)	{
				int min_i = i;
				for (int j = i + 1; j < sprite_cnt; j++)	{
					if (z_value[j] < z_value[min_i])	{
						min_i = j;
					}
				}
				std::swap(z_value[i], z_value[min_i]);
				std::swap(no_blend_sprite_list[i], no_blend_sprite_list[min_i]);
			}
			for (ARRAY<C_tnm_sprite *>::iterator itr = no_blend_sprite_list.begin(); itr != no_blend_sprite_list.end(); ++itr)	{
				m_view.regist_sprite(*itr);
			}
			if (!m_view.render())	{
				return false;
			}
			m_view.clear_sprite_list();
#else
			for (ARRAY<C_tnm_sprite *>::iterator itr = no_blend_sprite_list.begin(); itr != no_blend_sprite_list.end(); ++itr)	{
				m_view.regist_sprite(*itr);
			}
			if (!m_view.render())	{
				return false;
			}
			m_view.clear_sprite_list();
#endif
		}
		// �u�����h������̂�`��
		{
			// �R�c�ŃA���t�@�u�����h���s���̂ŁA�y�\�[�g���s��
			C_elm_world* p_world = &(*Gp_world_list)[world_no];
			int sprite_cnt = (int)blend_sprite_list.size();
			ARRAY<float> z_value(sprite_cnt);
			for (int i = 0; i < sprite_cnt; i++)	{
				D3DXVECTOR3 a = *(D3DXVECTOR3 *)&blend_sprite_list[i]->rp.pos - m_view.m_camera_eye;
				D3DXVECTOR3 b = m_view.m_camera_dir;
				D3DXVec3Normalize(&b, &b);
				float dot = D3DXVec3Dot(&a, &b);
				z_value[i] = dot;
			}

			for (int i = 0; i < sprite_cnt; i++)	{
				int max_i = i;
				for (int j = i + 1; j < sprite_cnt; j++)	{
					if (z_value[j] > z_value[max_i])	{
						max_i = j;
					}
				}
				std::swap(z_value[i], z_value[max_i]);
				std::swap(blend_sprite_list[i], blend_sprite_list[max_i]);
			}
			for (ARRAY<C_tnm_sprite *>::iterator itr = blend_sprite_list.begin(); itr != blend_sprite_list.end(); ++itr)	{
				m_view.regist_sprite(*itr);
			}
			if (!m_view.render())	{
				return false;
			}
			m_view.clear_sprite_list();
		}

		// �̃X�v���C�g���X�g���N���A
		sprite_list.clear();

		// �����_�[�^�[�Q�b�g�̐[�x�X�e���V���o�b�t�@���N���A
		m_view.clear_render_target_z_buffer();
		m_view.clear_render_target_stencil();
	}

	// �R�c�łȂ��ꍇ
	else	{
	*/

		// ���܂����X�v���C�g��o�^
		for (ARRAY<C_tnm_sprite *>::iterator itr = sprite_list.begin(); itr != sprite_list.end(); ++itr)	{
			m_view.regist_sprite(*itr);
		}

		// �o�^�����X�v���C�g�������_�[
		if (!m_view.render())	{
			return false;
		}
		// �r���[�̃X�v���C�g���X�g���N���A
		m_view.clear_sprite_list();

		// �X�v���C�g���X�g���N���A
		sprite_list.clear();

		// �����_�[�^�[�Q�b�g�̐[�x�X�e���V���o�b�t�@���N���A
		m_view.clear_render_target_z_buffer();
		m_view.clear_render_target_stencil();
//	}

	return true;
}

bool C_tnm_wnd::disp_proc_sprite_list(ARRAY<C_tnm_sprite *>& sprite_list)
{
	C_size gsize = Gp_global->game_screen_size;

	ARRAY<C_tnm_sprite *>::iterator sprite_itr;
	ARRAY<C_tnm_sprite *>::iterator sprite_end;

	// �S�ẴX�v���C�g�̒��_�o�b�t�@���X�V����
	ARRAY<C_tnm_sprite *> sprite_list_2;
	if (!tnm_update_vertex_list(sprite_list, sprite_list_2))
		return false;

	// �r���[�̃X�v���C�g���X�g���N���A
	m_view.clear_sprite_list();

	// �A���t�@�u�����h�p�̃X�v���C�g���X�g
	ARRAY<C_tnm_sprite *> sprite_list_3;

	// �S�ẴX�v���C�g��`�悷��
	C_elm_world* p_last_world = NULL;
	for (sprite_itr = sprite_list_2.begin(); sprite_itr != sprite_list_2.end(); ++ sprite_itr)	{
		S_d3d_render_param* rp = &(*sprite_itr)->rp;
		C_elm_world* p_world = (*sprite_itr)->rpex.p_world;

		// ���[���h���ς������`��
		if (p_world != p_last_world)	{

			// ���܂����X�v���C�g��`��
			if (!disp_proc_sprite_list_sub(p_last_world, sprite_list_3))
				return false;

			// ���[���h����������
			if (p_world != NULL)	{

				// �r���[�̃J������ݒ�

				// 2D�ʏ탂�[�h
				if (p_world->get_mode() == TNM_CAMERA_MODE_2D)	{
				}
				// 3D�ʏ탂�[�h
				else if (p_world->get_mode() == TNM_CAMERA_MODE_3D)	{

					D3DXVECTOR3 camera_eye, camera_pint, camera_up;
					float camera_aspect, camera_view_angle, zn, zf;
					camera_eye.x = (float)p_world->get_camera_eye_total_x() / TNM_WORLD_UNIT;
					camera_eye.y = (float)p_world->get_camera_eye_total_y() / TNM_WORLD_UNIT;
					camera_eye.z = (float)p_world->get_camera_eye_total_z() / TNM_WORLD_UNIT;
					camera_pint.x = (float)p_world->get_camera_pint_total_x() / TNM_WORLD_UNIT;
					camera_pint.y = (float)p_world->get_camera_pint_total_y() / TNM_WORLD_UNIT;
					camera_pint.z = (float)p_world->get_camera_pint_total_z() / TNM_WORLD_UNIT;
					camera_up.x = (float)p_world->get_camera_up_total_x() / TNM_WORLD_UNIT;
					camera_up.y = (float)p_world->get_camera_up_total_y() / TNM_WORLD_UNIT;
					camera_up.z = (float)p_world->get_camera_up_total_z() / TNM_WORLD_UNIT;
					camera_view_angle = D3DXToRadian((float)p_world->get_camera_view_angle() / TNM_ANGLE_UNIT);
					camera_aspect = (float)gsize.cx / gsize.cy;
					zn = 1.0f;
					zf = 10000.0f;

					// ���C�g��ݒ肵�Ă݂�
					C_d3d_light light;
					ZeroMemory(&light, sizeof(light));
					light.Type = D3DLIGHT_DIRECTIONAL;
					light.Position.x = camera_eye.x;
					light.Position.y = camera_eye.y;
					light.Position.z = camera_eye.z;
					light.Diffuse.a = 1.0f;
					light.Diffuse.r = 1.0f;
					light.Diffuse.g = 1.0f;
					light.Diffuse.b = 1.0f;
					light.Ambient.a = 1.0f;
					light.Ambient.r = 3.0f;
					light.Ambient.g = 3.0f;
					light.Ambient.b = 3.0f;
					D3DXVECTOR3 dir(0.0f, 1.0f, 0.0f);
					D3DXVec3Normalize(&dir, &dir);
					light.Direction = dir;

					// ���C�g�̌v�Z���@
					light.camera_light_calc_type = D3D_AMBIENT_TYPE_MULTIPLE;
					light.ambient_light_calc_type = D3D_AMBIENT_TYPE_MULTIPLE;
					G_d3d.state.set_light(0, &light);

					// �J�����̌�����ݒ�
					m_view.m_camera_dir = camera_pint - camera_eye;
					m_view.m_camera_eye = camera_eye;
					D3DXVec3Normalize(&m_view.m_camera_dir, &m_view.m_camera_dir);

					// �t�H�O
					m_view.m_fog_use = 0;
					m_view.m_fog_x = (float)Gp_fog->get_total_x();
					m_view.m_fog_near = (float)Gp_fog->get_fog_near();
					m_view.m_fog_far = (float)Gp_fog->get_fog_far();

					// �r���[�s���ݒ�
					D3DXMatrixLookAtLH(&m_view.m_mat_view, &camera_eye, &camera_pint, &camera_up);

					// �ˉe�s���ݒ�
					float camera_h_half = zn * tan(camera_view_angle / 2);
					float camera_w_half = camera_h_half * camera_aspect;
					D3DXMatrixPerspectiveOffCenterLH(&m_view.m_mat_proj, - camera_w_half, camera_w_half, - camera_h_half, camera_h_half, zn, zf);
				}

				// �ȈՃ��[�h
				else	{

					D3DXVECTOR3 camera_eye, camera_pint, camera_up;
					float camera_aspect, camera_view_angle, zn, zf;
					float tan_y;

					camera_view_angle = D3DXToRadian((float)p_world->get_camera_view_angle() / TNM_ANGLE_UNIT);
					tan_y = tan(camera_view_angle / 2);
					camera_aspect = - (float)gsize.cx / gsize.cy;
					camera_eye.x = (*sprite_itr)->rp.pos.x;
					camera_eye.y = (*sprite_itr)->rp.pos.y;
					camera_eye.z = - gsize.cy / 2 / tan_y;
					camera_pint.x = (*sprite_itr)->rp.pos.x;
					camera_pint.y = (*sprite_itr)->rp.pos.y;
					camera_pint.z = 0.0f;
					camera_up.x = 0.0f;
					camera_up.y = -1.0f;
					camera_up.z = 0.0f;
						// ���}�C�i�X�ɂ��邱�ƂŔ��]���Ă��܂��I

					zn = 1.0f;
					zf = 10000.0f;

					// �r���[�s��̐ݒ�
					D3DXMatrixLookAtLH(&m_view.m_mat_view, &camera_eye, &camera_pint, &camera_up);

					// �ˉe�s��̐ݒ�
					D3DXMatrixPerspectiveOffCenterLH(&m_view.m_mat_proj, 
						(0.0f - rp->pos.x / (gsize.cx / 2)) * camera_aspect * zn * zf / (zf - zn) * tan_y, 
						(2.0f - rp->pos.x / (gsize.cx / 2)) * camera_aspect * zn * zf / (zf - zn) * tan_y, 
						(-2.0f + rp->pos.y / (gsize.cy / 2)) * zn * zf / (zf - zn) * tan_y, 
						(0.0f + rp->pos.y / (gsize.cy / 2)) * zn * zf / (zf - zn) * tan_y, 
						zn, zf);
				}
			}
		}

		// ���̃��[���h��
		p_last_world = p_world;

		// �X�v���C�g��o�^
		sprite_list_3.push_back(*sprite_itr);
	}

	// ���܂����X�v���C�g��`��
	if (!disp_proc_sprite_list_sub(p_last_world, sprite_list_3))
		return false;

	return true;
}

// ****************************************************************
// �X�v���C�g��`�悷��
// ================================================================
bool C_tnm_wnd::disp_proc_sprite(C_d3d_sprite* p_sprite)
{
	// �X�v���C�g���X�g���N���A
	m_view.clear_sprite_list();

	// �X�v���C�g��ǉ�
	m_view.regist_sprite(p_sprite);

	// �X�e�[�W��`��
	if (!m_view.render())
		return false;

	return true;
}

// ****************************************************************
// �őO�ʂ�`�悷��
// ================================================================
bool C_tnm_wnd::disp_proc_top_most()
{
	// ���[�g�X�v���C�g���쐬
	C_tnm_sprite root_sprite;
	C_tree2<C_tnm_sprite *> root;
	root.data = &root_sprite;

	// ���b�Z�[�W�o�b�N
	if (!Gp_local->pod.msg_back_disp_off)	{
		Gp_msg_back->get_sprite_tree(root);
	}

	// �X�v���C�g�����X�g�ɂ���
	ARRAY<C_tnm_sprite *> sprite_list;
	disp_proc_sprite_tree_to_sprite_list(root, sprite_list);

	// �X�v���C�g���X�g�̒��_�o�b�t�@���X�V����
	ARRAY<C_tnm_sprite *> sprite_list_2;
	if (!tnm_update_vertex_list(sprite_list, sprite_list_2))
		return false;

	// �X�v���C�g���X�g���N���A
	m_view.clear_sprite_list();

	// �X�v���C�g��ǉ�
	for (ARRAY<C_tnm_sprite *>::iterator sprite = sprite_list_2.begin(); sprite != sprite_list_2.end(); ++ sprite)
		m_view.regist_sprite(*sprite);

	// �o�b�N�o�b�t�@�ɃX�e�[�W��`��
	if (!m_view.render())
		return false;

	return true;
}

// ****************************************************************
// �L���v�V�������X�V
// ================================================================
void C_tnm_wnd::update_caption()
{
	STATE cd;

	// ���݂̃p�����[�^���\�z����
	cd.title = tnm_get_full_title();

	// �E�B���h�E�^�C�g����ݒ�
	if (m_first || cd.title != m_ld.title)
		set_text(cd.title);

	// �p�����[�^��ۑ�
	m_ld = cd;

	// ����t���O�����낷
	m_first = false;
}
