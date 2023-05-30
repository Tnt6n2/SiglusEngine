#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_global_data.h"
#include	"element/elm_object.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_dir.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// �I�u�W�F�N�g�F���[�r�[�I�u�W�F�N�g�̍쐬
// ================================================================
bool C_elm_object::create_movie(TSTR file_path, bool loop, bool auto_free, bool real_time, bool ready_only)
{
	// �^�C�v�ʂ̃p�����[�^������������
	init_type(true);

	// �p�����[�^��ݒ�
	m_op.type = TNM_OBJECT_TYPE_MOVIE;
	m_op.file_path = file_path;
	m_op.movie.loop_flag = loop;
	m_op.movie.auto_free_flag = auto_free;
	m_op.movie.real_time_flag = real_time;
	m_op.movie.pause_flag = ready_only ? true : false;

	// ���[�r�[�I�u�W�F�N�g���č\�z����
	if (!restruct_movie())
		return false;

	return true;
}

bool C_elm_object::restruct_movie()
{
	// �t�@�C��������
	TSTR file_path = tnm_find_omv(m_op.file_path);
	if (file_path.empty())	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���[�r�[�t�@�C�� ") + m_op.file_path + _T(".omv ��������܂���ł����B"));
		init_type(true);		// �^�C�v��������
		return false;
	}

	// omv ����������
	m_omv_player = BSP<C_omv_player>(new C_omv_player);
	if (!m_omv_player->ready_omv(file_path, m_op.movie.loop_flag ? true : false, false))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���[�r�[�I�u�W�F�N�g ") + m_op.file_path + _T(" �̍쐬�Ɏ��s���܂����B\n") + m_omv_player->get_last_error());
		init_type(true);		// �^�C�v��������
		return false;
	}

	// �e�N�X�`�����쐬����
	// ���t���[���X�V����̂� D3DUSAGE_DYNAMIC ���w�肷��
	// ���̏ꍇ D3DPOOL_MANAGED �͎g���Ȃ��̂ŁAD3DPOOL_DEFAULT ���g��
	// �f�o�C�X����������ƍ�蒼���K�v������̂ŁA�Ō�̈����� true ��^���ă}�l�[�W���ɊǗ�������
	S_omv_theora_info omv_theora_info;
	m_omv_player->get_theora_info(&omv_theora_info);

	BSP<C_d3d_texture> texture = G_rsm.create_texture(_T("Ѱ�ް��޼ު��"), omv_theora_info.size.cx, omv_theora_info.size.cy, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, true);							// �k���~
	//BSP<C_d3d_texture> texture = G_rsm.create_texture(_T("Ѱ�ް��޼ު��"), omv_theora_info.size.cx, omv_theora_info.size.cy, 0, D3DUSAGE_AUTOGENMIPMAP | D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, true);	// �k���~
	//BSP<C_d3d_texture> texture = G_rsm.create_texture(_T("Ѱ�ް��޼ު��"), omv_theora_info.size.cx, omv_theora_info.size.cy, 0, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, true);							// �k���~
	//BSP<C_d3d_texture> texture = G_rsm.create_texture(_T("Ѱ�ް��޼ު��"), omv_theora_info.size.cx, omv_theora_info.size.cy, 0, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, true);						// �k����
	if (!texture)	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���[�r�[�I�u�W�F�N�g ") + m_op.file_path + _T(" �̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
		init_type(true);		// �^�C�v��������
		return false;
	}

	// �e�N�X�`�����N���A���Ă����i�K�{�B�����`���Ńf�[�^���������܂�邽�߁A���ꂪ�Ȃ��ƃS�~���c��j
	texture->clear_color_ex(C_argb(0, 0, 0, 0));

	// �A���o�����쐬����
	m_album = BSP<C_d3d_album>(new C_d3d_album);
	m_album->add_texture(texture);

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F���[�r�[�I�u�W�F�N�g�̃t���[������
// ================================================================
void C_elm_object::movie_frame()
{
	// �r�f�I���擾����
	if (m_omv_player && m_album && m_album->get_texture(0))	{
		BSP<C_d3d_texture> texture = m_album->get_texture(0);

		// ���ԂɒB���Ă��Ȃ��ꍇ�A���[�r�[���X�V����Ȃ����Ƃ�����܂��B
		// ���̏ꍇ�A�O�t���[���̃��[�r�[��j�����Ă͂����܂���B
		// ����� D3DLOCK_DISCARD ���w�肷�邱�Ƃ͂ł��܂���B

		// ���[�v�̏ꍇ�͐�Ɏ��������[�v�����Ă���
		// �u�r���Ń��[�v����߂�v�̏����̂��߂ɕK�{�ł��B
		if (m_op.movie.loop_flag)	{
			int movie_total_time = m_omv_player->get_theora_total_frame_time();
			if (movie_total_time > 0)	{
				if (m_omv_timer > movie_total_time)	{
					m_omv_timer %= movie_total_time;
				}
			}
		}

		int new_frame_no;

		// dirty �ȏꍇ�͋����I�ɍX�V���邱�ƂɂȂ�܂����i2014/9/16�j
		bool update_by_force = texture->is_dirty();
		if (m_omv_player->check_need_update(m_omv_timer, &new_frame_no, update_by_force))	{

#if 0
			// �ĂP�FD3DPOOL_SYSTEMMEM �ȃT�[�t�F�X�ɕ`�悵�Ă��� UpdateSurface �œ]���B
			// �x�������肵�Ȃ��B�����ƁB
			D3DLOCKED_RECT locked_rect;
			if (m_omv_surface->lock_rect(&locked_rect, NULL, D3DLOCK_DISCARD))	{

				m_omv_player->get_video_for_frame(new_frame_no, (BYTE *)locked_rect.pBits, locked_rect.Pitch, NULL);
				m_omv_surface->unlock_rect();

				C_d3d_surface dst_surface = texture->get_surface(0);
				C_rect src_rect(C_point(0, 0), m_omv_surface->get_size());
				C_point dst_point(0, 0);
				G_d3d.device->UpdateSurface(m_omv_surface->body(), &src_rect, dst_surface.body(), &dst_point);
			}
#else
			// �ĂQ�F�e�N�X�`�������b�N���ď������݁B
			// ���[�r�[��S�ăL�[�t���[���ɂ����̂ŁAD3DLOCK_DISCARD ���w��ł���悤�ɂȂ����B
			// �������Ń��b�N�ɂ����鎞�Ԃ��قڂO�ɂȂ�A���ǂ��ꂪ�œK�ɂȂ����B
			D3DLOCKED_RECT locked_rect;
			if (texture->lock_rect(0, &locked_rect, NULL, D3DLOCK_DISCARD))	{
				m_omv_player->get_video_for_frame(new_frame_no, (BYTE *)locked_rect.pBits, locked_rect.Pitch, false, update_by_force, NULL);
				texture->add_edge_for_filterring(&locked_rect);
				texture->unlock_rect(0);
			}

			// dirty �t���O���~�낷
			texture->set_dirty_flag(false);
#endif
		}

		// ��{�p�����[�^��ݒ�
		trp_to_rp(&m_sprite->rpex, &m_sprite->rp);

		// �e�N�X�`����ݒ�
		m_sprite->set_texture(texture);
		m_sprite->rp.size_fit_to_texture = 0;
	}
}

// ****************************************************************
// �I�u�W�F�N�g�F���[�r�[���V�[�N
// ================================================================
void C_elm_object::seek_movie(int time)
{
	m_omv_timer = time;

	if (m_omv_player)	{
		m_omv_player->update_time_only(time);
	}
}

// ****************************************************************
// �I�u�W�F�N�g�F���[�r�[�̃V�[�N�ʒu��ݒ�
// ================================================================
int C_elm_object::get_movie_seek_time()
{
	if (m_omv_player)	{
		int movie_total_time = m_omv_player->get_theora_total_frame_time();
		if (movie_total_time <= 0)	{
			return 0;
		}
		else	{
			return m_omv_timer % movie_total_time;
		}
	}
	else	{
		return 0;
	}
}

// ****************************************************************
// �I�u�W�F�N�g�F���[�r�[�𔻒�
// ================================================================
bool C_elm_object::check_movie()
{
	if (m_omv_player)	{

		// �Đ����𔻒�
		if (m_omv_player->is_playing())
			return true;
	}

	return false;
}

// ****************************************************************
// �I�u�W�F�N�g�F���[�r�[�I����҂�
// ================================================================
void C_elm_object::wait_movie(bool key_skip_flag, bool return_value_flag)
{
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_OBJ_MOV_WAIT;
	proc.element = get_element();
	proc.key_skip_enable_flag = key_skip_flag;
	proc.return_value_flag = return_value_flag;
	tnm_push_proc(proc);
}

// ****************************************************************
// �I�u�W�F�N�g�F���[�r�[�̃��[�v���I��������
// ================================================================
void C_elm_object::end_movie_loop()
{
	if (m_omv_player)	{
		m_omv_player->end_loop();
	}
}

// ****************************************************************
// �I�u�W�F�N�g�F���[�r�[�̎�������t���O��ݒ肷��
// ================================================================
void C_elm_object::set_movie_auto_free(bool value)
{
	m_op.movie.auto_free_flag = value;
}
