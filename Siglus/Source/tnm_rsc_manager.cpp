#include	"pch.h"
#include	"data/tnm_rsc_manager.h"
#include	"engine/ifc_error.h"

#include	<boost/algorithm/string.hpp>

// ****************************************************************
// Direct3D ���\�[�X�F���\�[�X�f�[�^�F�R���X�g���N�^
// ================================================================
S_tnm_d3d_resource::S_tnm_d3d_resource()
{
	type = E_tnm_d3d_resource_type_none;
	id = -1;
	size = C_size(0, 0);
	mip_level = 0;
	usage = 0;
	format = (D3DFORMAT)(-1);
	pool = (D3DPOOL)(-1);
	texture_cnt = 0;
	moji = 0;
	extend_type = 0;
	color = C_argb(0, 0, 0, 0);
	FVF = 0;
	resume = false;
}

// ****************************************************************
// Direct3D ���\�[�X�}�l�[�W���F�R���X�g���N�^
// ================================================================
C_tnm_d3d_resource_manager::C_tnm_d3d_resource_manager()
{
	m_next_id = 0;
}

// ****************************************************************
// Direct3D ���\�[�X�}�l�[�W���F���\�[�X�f�[�^���N���A����
// ================================================================
void C_tnm_d3d_resource_manager::clear()
{
	m_pd.resource_list.clear();
}

// ****************************************************************
// Direct3D ���\�[�X�}�l�[�W���F���\�[�X�f�[�^�𐮗�����
// ================================================================
void C_tnm_d3d_resource_manager::organize()
{
	for (ARRAY<S_tnm_d3d_resource>::iterator itr = m_pd.resource_list.begin(); itr != m_pd.resource_list.end();)	{
		bool erase_flag = false;

		if (false);
		// �e�N�X�`���̏ꍇ
		else if (itr->type == E_tnm_d3d_resource_type_texture || itr->type == E_tnm_d3d_resource_type_texture_from_file || itr->type == E_tnm_d3d_resource_type_texture_from_g00 || itr->type == E_tnm_d3d_resource_type_texture_from_moji || itr->type == E_tnm_d3d_resource_type_texture_from_mwnd)	{
			BSP<C_d3d_texture> texture = itr->texture.lock();
			if (!texture)	{
				itr = m_pd.resource_list.erase(itr);
				erase_flag = true;
			}
		}
		// �A���o���̏ꍇ
		else if (itr->type == E_tnm_d3d_resource_type_album_from_g00)	{
			BSP<C_d3d_album> album = itr->album.lock();
			if (!album)	{
				itr = m_pd.resource_list.erase(itr);
				erase_flag = true;
			}
		}
		// �[�x�X�e���V���o�b�t�@�̏ꍇ
		else if (itr->type == E_tnm_d3d_resource_type_depth_stencil_buffer)	{
			BSP<C_d3d_surface> surface = itr->surface.lock();
			if (!surface)	{
				itr = m_pd.resource_list.erase(itr);
				erase_flag = true;
			}
		}
		// ���b�V���̏ꍇ
		else if (itr->type == E_tnm_d3d_resource_type_mesh_from_x)	{
			BSP<C_d3d_mesh> mesh = itr->mesh.lock();
			if (!mesh)	{
				itr = m_pd.resource_list.erase(itr);
				erase_flag = true;
			}
		}
		// �t���[���̏ꍇ
		else if (itr->type == E_tnm_d3d_resource_type_frame_from_x)	{
			BSP<C_d3d_frame_set> frame_set = itr->frame_set.lock();
			if (!frame_set)	{
				itr = m_pd.resource_list.erase(itr);
				erase_flag = true;
			}
		}

		if (!erase_flag)
			++ itr;
	}
}

// ****************************************************************
// Direct3D ���\�[�X�}�l�[�W���F���\�[�X�ꎞ���
// ================================================================
//		���\�[�X�̃��X�g�͈ێ����܂��B
//		device->Reset() �̑O�ɕK���Ă�ł��������B
//		resume() ���Ăяo�����ƂŁA���\�[�X�𕜌����邱�Ƃ��ł��܂��B
// ================================================================
void C_tnm_d3d_resource_manager::release()
{
	// ���\�[�X���ꎞ���
	for (ARRAY<S_tnm_d3d_resource>::iterator itr = m_pd.resource_list.begin(); itr != m_pd.resource_list.end(); ++itr)	{
		if (itr->resume)	{

			if (false);
			// �e�N�X�`���̏ꍇ
			else if (itr->type == E_tnm_d3d_resource_type_texture || itr->type == E_tnm_d3d_resource_type_texture_from_file || itr->type == E_tnm_d3d_resource_type_texture_from_g00 || itr->type == E_tnm_d3d_resource_type_texture_from_moji)	{
				BSP<C_d3d_texture> texture = itr->texture.lock();
				if (texture)
					texture->release();
			}
			// �A���o���̏ꍇ
			else if (itr->type == E_tnm_d3d_resource_type_album_from_g00)	{
				for (int i = 0; i < (int)itr->album.lock()->get_texture_cnt(); i++)	{
					BSP<C_d3d_texture> texture = itr->album.lock()->get_texture(i);
					if (texture)
						texture->release();
				}
			}
			// �[�x�X�e���V���o�b�t�@�̏ꍇ
			else if (itr->type == E_tnm_d3d_resource_type_depth_stencil_buffer)	{
				BSP<C_d3d_surface> surface = itr->surface.lock();
				if (surface)
					surface->release();
			}
		}
	}
}

// ****************************************************************
// Direct3D ���\�[�X�}�l�[�W���F���\�[�X�𕜌�
// ================================================================
//		device->Reset() ������������Ă�ł��������B
//		release() �ŉ���������\�[�X�𕜌����邱�Ƃ��ł��܂��B
// ================================================================
bool C_tnm_d3d_resource_manager::resume()
{
	// �e�N�X�`��
	for (ARRAY<S_tnm_d3d_resource>::iterator itr = m_pd.resource_list.begin(); itr != m_pd.resource_list.end(); ++itr)	{
		if (itr->resume)	{

			if (false);

			// �e�N�X�`���̏ꍇ
			else if (itr->type == E_tnm_d3d_resource_type_texture || itr->type == E_tnm_d3d_resource_type_texture_from_file || itr->type == E_tnm_d3d_resource_type_texture_from_g00 || itr->type == E_tnm_d3d_resource_type_texture_from_moji)	{
				BSP<C_d3d_texture> texture = itr->texture.lock();
				if (texture)	{

					if (false);
					// �e�N�X�`�����쐬����
					else if (itr->type == E_tnm_d3d_resource_type_texture)	{
						if (!texture->create(itr->size.cx, itr->size.cy, itr->mip_level, itr->usage, itr->format, itr->pool))
							return false;

						// �����ɓh��Ԃ�
						if (itr->pool == D3DPOOL_DEFAULT)	{
							
							// D3DPOOL_DEFAULT �̏ꍇ�̓��b�N�ł��Ȃ��̂� UpdateTexture �ōX�V����
//							texture->clear_color_ex(C_argb(0, 0, 0, 0));
						}
						else	{

							// D3DPOOL_DEFAULT �łȂ��ꍇ�̓��b�N�ł���̂ŕ��ʂɓh��Ԃ�
							texture->clear_color_ex(C_argb(0, 0, 0, 0));
						}

						// dirty �t���O�𗧂ĂƂ���I����ōX�V�����҂ł���I
						texture->set_dirty_flag(true);
					}
					// �t�@�C����ǂݍ���
					else if (itr->type == E_tnm_d3d_resource_type_texture_from_file)	{
						if (!texture->load_file(itr->name))
							return false;
					}
					// g00 ��ǂݍ���
					else if (itr->type == E_tnm_d3d_resource_type_texture_from_g00)	{
						if (!texture->load_g00(itr->name))
							return false;
					}
				}
			}
			// �A���o���̏ꍇ
			else if (itr->type == E_tnm_d3d_resource_type_album_from_g00)	{
				BSP<C_d3d_album> album = itr->album.lock();
				if (album)	{
					if (!album->load_g00(itr->name, false))
						return false;
				}
			}
			// �[�x�X�e���V���o�b�t�@�̏ꍇ
			else if (itr->type == E_tnm_d3d_resource_type_depth_stencil_buffer)	{
				BSP<C_d3d_surface> surface = itr->surface.lock();
				if (surface)	{
					if (!surface->create_dpst_buffer(itr->size.cx, itr->size.cy))
						return false;
				}
			}
		}
	}

	return true;
}

// ****************************************************************
// Direct3D ���\�[�X�}�l�[�W���F�e�N�X�`�����쐬
// ================================================================
BSP<C_d3d_texture> C_tnm_d3d_resource_manager::create_texture(CTSTR& name, int width, int height, UINT mip_level, DWORD usage, D3DFORMAT format, D3DPOOL pool, bool resume)
{
	BSP<C_d3d_texture> texture(new C_d3d_texture);
	if (!texture->create(width, height, mip_level, usage, format, pool))
		return BSP<C_d3d_texture>();

	// ���\�[�X��o�^
	S_tnm_d3d_resource resource;
	resource.name = name;
	resource.id = m_next_id++;
	resource.type = E_tnm_d3d_resource_type_texture;
	resource.texture = texture;
	resource.size = C_size(width, height);
	resource.size_ex = texture->get_size_ex();
	resource.mip_level = mip_level;
	resource.usage = usage;
	resource.format = format;
	resource.pool = pool;
	resource.resume = resume;
	m_pd.resource_list.push_back(resource);

	return texture;
}

// ****************************************************************
// Direct3D ���\�[�X�}�l�[�W���F�e�N�X�`�����쐬�i�t�@�C����ǂݍ��ށj
// ================================================================
BSP<C_d3d_texture> C_tnm_d3d_resource_manager::create_texture_from_file(CTSTR& file_path, bool resume, bool reload)
{
	// �ēǂݍ��݋����t���O�������Ă��Ȃ��ꍇ�́A���ɓǂݍ��܂�Ă��郊�\�[�X��T��
	if (!reload)	{

		// �������\�[�X��T���o��
		for (int i = 0; i < (int)m_pd.resource_list.size(); i++)	{
			if (m_pd.resource_list[i].type == E_tnm_d3d_resource_type_texture_from_file && m_pd.resource_list[i].name == file_path)	{
				// ���\�[�X�������Ă���K�v������܂��I
				if (m_pd.resource_list[i].texture.lock())	{
					// ���\�[�X�����������̂ł����Ԃ�
					return m_pd.resource_list[i].texture.lock();
				}
			}
		}
	}

	// ���\�[�X��������Ȃ������̂ŐV�K�쐬
	BSP<C_d3d_texture> texture(new C_d3d_texture);
	if (!texture->load_file(file_path))
		return BSP<C_d3d_texture>();		// �ǂݍ��݂Ɏ��s�c�B
	
	// �V�K�Ƀ��\�[�X��o�^
	S_tnm_d3d_resource resource;
	resource.id = m_next_id++;
	resource.type = E_tnm_d3d_resource_type_texture_from_file;
	resource.texture = texture;
	resource.name = file_path;
	resource.size = texture->get_size();
	resource.size_ex = texture->get_size_ex();
	resource.resume = resume;
	m_pd.resource_list.push_back(resource);

	return texture;
}

// ****************************************************************
// Direct3D ���\�[�X�}�l�[�W���F�e�N�X�`�����쐬�imwnd ��ǂݍ��ށj
// ================================================================

//		��`�R�s�[���[�`���ł��B
//		���C�u�����Ƃ��Ă܂Ƃ߂�ׂ������B
//		src_pitch, dst_pitch �����̓s�N�Z�����ł͂Ȃ��o�C�g���Ȃ̂Œ��ӁB
void block_copy(BYTE* src, int src_pitch, int src_org_w, int src_org_h, int src_x, int src_y, int src_w, int src_h, BYTE* dst, int dst_pitch, int dst_org_w, int dst_org_h, int dst_x, int dst_y)
{
	// ��O����
	if (!(0 <= src_x && src_x <= src_x + src_w && src_x + src_w <= src_org_w))
		return;
	if (!(0 <= src_y && src_y <= src_y + src_h && src_y + src_h <= src_org_h))
		return;
	if (!(0 <= dst_x && dst_x <= dst_x + src_w && dst_x + src_w <= dst_org_w))
		return;
	if (!(0 <= dst_y && dst_y <= dst_y + src_h && dst_y + src_h <= dst_org_h))
		return;

	// 
	src += src_y * src_pitch + src_x * 4;
	dst += dst_y * dst_pitch + dst_x * 4;

	for (int y = 0; y < src_h; y++)	{
		memcpy(dst, src, src_w * 4);
		src += src_pitch;
		dst += dst_pitch;
	}
}

BSP<C_d3d_texture> C_tnm_d3d_resource_manager::create_texture_from_mwnd(CTSTR& file_path, int extend_type, C_size window_size, bool resume)
{
	C_size org_window_size = window_size;

	// �������\�[�X��T���o��
	for (int i = 0; i < (int)m_pd.resource_list.size(); i++)	{
		if (m_pd.resource_list[i].type == E_tnm_d3d_resource_type_texture_from_mwnd && m_pd.resource_list[i].name == file_path &&  m_pd.resource_list[i].size == window_size)	{
			// ���\�[�X�������Ă���K�v������܂��I
			if (m_pd.resource_list[i].texture.lock())	{
				// ���\�[�X�����������̂ł����Ԃ�
				return m_pd.resource_list[i].texture.lock();
			}
		}
	}

	// ���\�[�X��������Ȃ������̂ŐV�K�쐬
	BSP<C_d3d_album> org_pct(new C_d3d_album);
	if (!org_pct->load_g00(file_path, false))	{
		return BSP<C_d3d_texture>();		// �ǂݍ��݂Ɏ��s�c�B
	}

	// �J�b�g���𔻒�
	if (org_pct->get_texture_cnt() < 12)	{
		set_last_error(_T("�Ϙg�Ƃ��ēǂݍ��݂܂������A�J�b�g��������܂���ł����B"), _T(""));
		return BSP<C_d3d_texture>();		// �ǂݍ��݂Ɏ��s�c�B
	}

	// �؂�o���f�[�^�̑傫�����擾
	C_size cs[12];
	for (int i = 0; i < 12; i++)
		cs[i] = org_pct->get_texture(i)->get_size();

	// ======== //
	// �O �P �Q //
	// �R    �U //
	// �S    �V //
	// �T    �W //
	// �X 10 11 //
	// -------- //

	// �σp�[�c����ׂ鐔���v�Z����
	int extend_x_cnt = (window_size.cx - (cs[0].cx + cs[2].cx) + (cs[1].cx - 1)) / cs[1].cx;
	int extend_y_cnt = (window_size.cy - (cs[0].cy + cs[3].cy + cs[5].cy + cs[9].cy) + (cs[4].cy - 1)) / cs[4].cy;

	// �E�B���h�E�T�C�Y��؂�o���f�[�^�ɂ��낤�悤�ɐ�����
	window_size.cx = extend_x_cnt * cs[1].cx + (cs[0].cx + cs[2].cx);
	window_size.cy = extend_y_cnt * cs[4].cy + (cs[0].cy + cs[3].cy + cs[5].cy + cs[9].cy);

	// �J�b�g����E�B���h�E�f�[�^���쐬����I

	// �e�N�X�`�����쐬����
	BSP<C_d3d_texture> mwnd_texture(new C_d3d_texture);
	mwnd_texture->create(window_size.cx, window_size.cy, 0, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED);
	if (!mwnd_texture)
		return BSP<C_d3d_texture>();		// �ǂݍ��݂Ɏ��s�c�B

	// �e�N�X�`�������b�N
	D3DLOCKED_RECT locked_rect_dst;
	if (!mwnd_texture->lock_rect(0, &locked_rect_dst, NULL, 0))
		return BSP<C_d3d_texture>();		// �ǂݍ��݂Ɏ��s�c�B

	// �P�Q�p�[�c��W�J���Ă���
	for (int i = 0; i < 12; i++)	{

		D3DLOCKED_RECT locked_rect_src;
		if (!org_pct->get_texture(i)->lock_rect(0, &locked_rect_src, NULL, 0))
			return BSP<C_d3d_texture>();		// �ǂݍ��݂Ɏ��s�c�B

		// �킩��₷���ϐ��𐮗����܂�
		BYTE* src_org = (BYTE *)locked_rect_src.pBits;
		BYTE* dst_org = (BYTE *)locked_rect_dst.pBits;
		int src_pitch = locked_rect_src.Pitch;
		int dst_pitch = locked_rect_dst.Pitch;
		int width = cs[i].cx;
		int height = cs[i].cy;

		if (false);

		else if (i == 0)	{	// �O��
			block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, 0, 0);
		}
		else if (i == 1)	{	// �P��
			for (int e = 0; e < extend_x_cnt; e++)	{
				block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, cs[0].cx + cs[1].cx * e, 0);
			}
		}
		else if (i == 2)	{	// �Q��
			block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, cs[0].cx + cs[1].cx * extend_x_cnt, 0);
		}
		else if (i == 3)	{	// �R��
			block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, 0, cs[0].cy);
		}
		else if (i == 4)	{	// �S��
			for (int e = 0; e < extend_y_cnt; e++)	{
				block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, 0, cs[0].cy + cs[3].cy + cs[4].cy * e);
			}
		}
		else if (i == 5)	{	// �T��
			block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, 0, cs[0].cy + cs[3].cy + cs[4].cy * extend_y_cnt);
		}
		else if (i == 6)	{	// �U��
			block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, window_size.cx - cs[6].cx, cs[0].cy);
		}
		else if (i == 7)	{	// �V��
			for (int e = 0; e < extend_y_cnt; e++)	{
				block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, window_size.cx - cs[7].cx, cs[0].cy + cs[3].cy + cs[4].cy * e);
			}
		}
		else if (i == 8)	{	// �W��
			block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, window_size.cx - cs[8].cx, cs[0].cy + cs[3].cy + cs[4].cy * extend_y_cnt);
		}
		else if (i == 9)	{	// �X��
			block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, 0, window_size.cy - cs[9].cy);
		}
		else if (i == 10)	{	// �P�O��
			for (int e = 0; e < extend_x_cnt; e++)	{
				block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, cs[0].cx + cs[1].cx * e, window_size.cy - cs[10].cy);
			}
		}
		else if (i == 11)	{	// �P�P��
			block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, cs[0].cx + cs[1].cx * extend_x_cnt, window_size.cy - cs[11].cy);
		}

		if (!org_pct->get_texture(i)->unlock_rect(0))
			return BSP<C_d3d_texture>();		// �ǂݍ��݂Ɏ��s�c�B
	}

	if (!mwnd_texture->unlock_rect(0))
		return BSP<C_d3d_texture>();		// �ǂݍ��݂Ɏ��s�c�B
	
	// �V�K�Ƀ��\�[�X��o�^
	S_tnm_d3d_resource resource;
	resource.id = m_next_id++;
	resource.type = E_tnm_d3d_resource_type_texture_from_mwnd;
	resource.texture = mwnd_texture;
	resource.name = file_path;
	resource.size = org_window_size;
	resource.resume = resume;
	m_pd.resource_list.push_back(resource);

	return mwnd_texture;
}

// ****************************************************************
// Direct3D ���\�[�X�}�l�[�W���F�e�N�X�`�����쐬�i������ǂݍ��ށj
// ================================================================
BSP<C_d3d_texture> C_tnm_d3d_resource_manager::create_texture_from_moji(TCHAR moji, LOGFONT* lf, C_argb moji_color, bool resume)
{
	// �������\�[�X��T���o��
	for (int i = 0; i < (int)m_pd.resource_list.size(); i++)	{
		if (m_pd.resource_list[i].type == E_tnm_d3d_resource_type_texture_from_moji && m_pd.resource_list[i].moji == moji && m_pd.resource_list[i].color == moji_color)	{
			// ���\�[�X�������Ă���K�v������܂��I
			if (m_pd.resource_list[i].texture.lock())	{
				// ���\�[�X�����������̂ł����Ԃ�
				return m_pd.resource_list[i].texture.lock();
			}
		}
	}

	// ���\�[�X��������Ȃ������̂ŐV�K�쐬
	BSP<C_d3d_texture> texture(new C_d3d_texture);
	if (!texture->load_moji(moji, lf, moji_color))
		return BSP<C_d3d_texture>();		// �ǂݍ��݂Ɏ��s�c�B
	
	// �V�K�Ƀ��\�[�X��o�^
	S_tnm_d3d_resource resource;
	resource.id = m_next_id++;
	resource.type = E_tnm_d3d_resource_type_texture_from_moji;
	resource.texture = texture;
	resource.moji = moji;
	resource.color = moji_color;
	resource.size = texture->get_size();
	resource.size_ex = texture->get_size_ex();
	resource.resume = resume;
	m_pd.resource_list.push_back(resource);

	return texture;
}

// ****************************************************************
// Direct3D ���\�[�X�}�l�[�W���F�A���o�����쐬�i�t�@�C����ǂݍ��ށj
// ================================================================
BSP<C_d3d_album> C_tnm_d3d_resource_manager::create_album_from_g00(CTSTR& file_path, bool fuchidori, bool resume)
{
	// �������\�[�X��T���o��
	for (int i = 0; i < (int)m_pd.resource_list.size(); i++)	{
		if (m_pd.resource_list[i].type == E_tnm_d3d_resource_type_album_from_g00 && m_pd.resource_list[i].name == file_path)	{
			// ���\�[�X�������Ă���K�v������܂��I
			if (m_pd.resource_list[i].album.lock())	{
				// ���\�[�X�����������̂ł����Ԃ�
				return m_pd.resource_list[i].album.lock();
			}
		}
	}

	// ���\�[�X��������Ȃ������̂ŐV�K�쐬
	BSP<C_d3d_album> album(new C_d3d_album);
	{
		bool result = true;

		ARRAY<C_d3d_album_compose_param> param_list;

		// �t�@�C���p�X�� '|' �ŕ�������
		std::vector<TSTR> str_list;
		boost::split(str_list, file_path, boost::is_any_of(_T("|")));

		// ���ꂼ������
		for (int i = 0; i < (int)str_list.size(); i++)
		{
			TSTR one_file_path = str_list[i];

			// �f�B���N�g���ƃt�@�C�����ɕ����܂��B
			// ���̏����͕K�{�ł��B�f�B���N�g������ () �Ȃǂ���͂����̂�h�����߂ł��B
			TSTR dir_path = ::get_dir_path(one_file_path);
			TSTR file_name = ::get_file_name(one_file_path);

			// �t�@�C����������ɉ��
			C_d3d_album_compose_param param;
			for (TSTR::const_iterator f_itr = file_name.begin();;)
			{
				// �����ɒB���������
				if (f_itr == file_name.end())
				{
					// �t�@�C�����m��
					param.file_name = file_name;
					// �t���p�X�����߂�
					if (!param.file_name.empty())
					{
						param.file_path = dir_path + _T("\\") + file_name;
					}
					// ���m��
					param_list.push_back(param);
					// �I��
					break;
				}
				// ���ʂ���������
				else if (*f_itr == _T('('))
				{
					// �t�@�C�����m��
					param.file_name = TSTR(TSTR::const_iterator(file_name.begin()), f_itr);
					// �t���p�X�����߂�
					if (!param.file_name.empty())
					{
						param.file_path = dir_path + _T("\\") + param.file_name;
					}
					// ����
					++f_itr;
					// �K�{�p�����[�^��ǂݍ���
					result &= str_analize_get_integer(f_itr, file_name.end(), &param.x);
					result &= str_analize_check_moji(f_itr, file_name.end(), _T(','));
					result &= str_analize_get_integer(f_itr, file_name.end(), &param.y);
					// �ǉ��p�����[�^��ǂݍ���
					while (str_analize_check_moji(f_itr, file_name.end(), _T(',')))
					{
						if (str_analize_check_str(f_itr, file_name.end(), _T("blend")))
						{
							result &= str_analize_check_moji(f_itr, file_name.end(), _T('='));
							result &= str_analize_get_integer(f_itr, file_name.end(), &param.blend_type);
						}
						else
						{
							result &= str_analize_get_integer(f_itr, file_name.end(), &param.cut_no);
						}
					}
					result &= str_analize_check_moji(f_itr, file_name.end(), _T(')'));
					if (!result)
					{
						return BSP<C_d3d_album>();		// ��͂Ɏ��s�c�B
					}
					// ���m��
					param_list.push_back(param);
					// �I��
					break;
				}
				else	{
					++f_itr;
				}
			}
		}

		// �ǂݍ���
		if (param_list.empty())
		{
			return BSP<C_d3d_album>();		// ��͂Ɏ��s�c�B
		}
		else if (param_list.size() == 1)
		{
			// �P�� g00
			if (!album->load_g00(param_list[0].file_path, fuchidori))
				return BSP<C_d3d_album>();		// �ǂݍ��݂Ɏ��s�c�B
		}
		else
		{
			// ���� g00
			if (!album->load_g00_composed(param_list))
				return BSP<C_d3d_album>();		// �ǂݍ��݂Ɏ��s�c�B
		}
	}

	// �e�N�X�`���̍ő�T�C�Y�����߂�
	C_size texture_size_max = C_size(0, 0);
	C_size texture_size_ex_max = C_size(0, 0);
	for (int i = 0; i < album->get_texture_cnt(); i++)	{
		if (album->get_texture(i))	{
			C_size texture_size = album->get_texture(i)->get_size();
			C_size texture_size_ex = album->get_texture(i)->get_size_ex();

			if (texture_size.cx * texture_size.cy >= texture_size_max.cx * texture_size_max.cy)
				texture_size_max = texture_size;
			if (texture_size_ex.cx * texture_size_ex.cy >= texture_size_ex_max.cx * texture_size_ex_max.cy)
				texture_size_ex_max = texture_size_ex;
		}
	}

	// �V�K�Ƀ��\�[�X��o�^
	S_tnm_d3d_resource resource;
	resource.id = m_next_id++;
	resource.type = E_tnm_d3d_resource_type_album_from_g00;
	resource.album = album;
	resource.name = file_path;
	resource.texture_cnt = album->get_texture_cnt();
	resource.size = texture_size_max;
	resource.size_ex = texture_size_ex_max;
	resource.resume = resume;
	m_pd.resource_list.push_back(resource);

	return album;
}

// ****************************************************************
// Direct3D ���\�[�X�}�l�[�W���F�e�N�X�`�����쐬�i�[�x�X�e���V���o�b�t�@���쐬�j
// ================================================================
BSP<C_d3d_surface> C_tnm_d3d_resource_manager::create_dpst_buffer(CTSTR& name, int width, int height, bool resume)
{
	BSP<C_d3d_surface> surface(new C_d3d_surface);
	if (!surface->create_dpst_buffer(width, height))
		return BSP<C_d3d_surface>();

	// ���\�[�X��o�^
	S_tnm_d3d_resource resource;
	resource.id = m_next_id++;
	resource.type = E_tnm_d3d_resource_type_depth_stencil_buffer;
	resource.surface = surface;
	resource.name = name;
	resource.resume = resume;
	resource.size = C_size(width, height);
	m_pd.resource_list.push_back(resource);

	return surface;
}

// ****************************************************************
// Direct3D ���\�[�X�}�l�[�W���F���b�V�����쐬�i�w�t�@�C����ǂݍ��ށj
// ================================================================
BSP<C_d3d_mesh> C_tnm_d3d_resource_manager::create_mesh_from_x(CTSTR& file_path, DWORD FVF, bool resume)
{
	// �������\�[�X��T���o��
	for (int i = 0; i < (int)m_pd.resource_list.size(); i++)	{
		if (m_pd.resource_list[i].type == E_tnm_d3d_resource_type_mesh_from_x && m_pd.resource_list[i].name == file_path)	{
			// ���\�[�X�������Ă���K�v������܂��I
			if (m_pd.resource_list[i].mesh.lock())	{
				// ���\�[�X�����������̂ł����Ԃ�
				return m_pd.resource_list[i].mesh.lock();
			}
		}
	}

	// ���\�[�X��������Ȃ������̂ŐV�K�쐬
	BSP<C_d3d_mesh> mesh(new C_d3d_mesh);
	ARRAY<TSTR> texture_name_list;
	if (!mesh->load_x(file_path, FVF, texture_name_list))
		return BSP<C_d3d_mesh>();		// �ǂݍ��݂Ɏ��s�c�B

	// ���b�V���ɕK�v�ȃe�N�X�`����ǂݍ���
	for (int i = 0; i < (int)texture_name_list.size(); i++)	{
		if (!texture_name_list[i].empty())	{
			BSP<C_d3d_texture> texture = create_texture_from_file(get_dir_path(file_path) + _T("\\") + texture_name_list[i], resume, false);
			if (!texture)
				return BSP<C_d3d_mesh>();		// �ǂݍ��݂Ɏ��s�c�B

			// ���b�V���Ƀe�N�X�`����ݒ肷��
			mesh->set_texture(i, texture);
		}
	}

	// �V�K�Ƀ��\�[�X��o�^
	S_tnm_d3d_resource resource;
	resource.id = m_next_id++;
	resource.type = E_tnm_d3d_resource_type_mesh_from_x;
	resource.mesh = mesh;
	resource.name = file_path;
	resource.FVF = FVF;
	resource.resume = resume;
	m_pd.resource_list.push_back(resource);

	return mesh;
}

// ****************************************************************
// Direct3D ���\�[�X�}�l�[�W���F�t���[�����쐬�i�w�t�@�C����ǂݍ��ށj
// ================================================================
BSP<C_d3d_frame_set> C_tnm_d3d_resource_manager::create_frame_set_from_x(CTSTR& file_path, bool resume)
{
	// �������\�[�X��T���o��
	for (int i = 0; i < (int)m_pd.resource_list.size(); i++)	{
		if (m_pd.resource_list[i].type == E_tnm_d3d_resource_type_frame_from_x && m_pd.resource_list[i].name == file_path)	{
			// ���\�[�X�������Ă���K�v������܂��I
			if (m_pd.resource_list[i].frame_set.lock())	{
				// ���\�[�X�����������̂ł����Ԃ�
				return m_pd.resource_list[i].frame_set.lock();
			}
		}
	}

	// ���\�[�X��������Ȃ������̂ŐV�K�쐬
	ARRAY< BSP<C_d3d_mesh> > mesh_list;
	ARRAY<TSTR> texture_name_list;
	BSP<C_d3d_frame_set> frame_set(new C_d3d_frame_set);
	if (!frame_set->load_x(file_path, mesh_list, texture_name_list))
		return BSP<C_d3d_frame_set>();		// �ǂݍ��݂Ɏ��s�c�B

	// �t���[���ɕK�v�ȃe�N�X�`����ǂݍ���
	int texture_no = 0;
	for (int m = 0; m < (int)mesh_list.size(); m++)	{
		for (int mtrl_no = 0; mtrl_no < mesh_list[m]->get_mtrl_cnt(); mtrl_no++)	{
			if (!texture_name_list[texture_no].empty())	{
				TSTR texture_path = get_dir_path(file_path) + _T("\\") + texture_name_list[texture_no];
				BSP<C_d3d_texture> texture = create_texture_from_file(texture_path, resume, false);
				if (!texture)
					return BSP<C_d3d_frame_set>();		// �ǂݍ��݂Ɏ��s�c�B

				// ���b�V���Ƀe�N�X�`����ݒ肷��
				mesh_list[m]->set_texture(mtrl_no, texture);
			}

			// ���̃e�N�X�`����
			texture_no ++;
		}
	}

	// �V�K�Ƀ��\�[�X��o�^
	S_tnm_d3d_resource resource;
	resource.id = m_next_id++;
	resource.type = E_tnm_d3d_resource_type_frame_from_x;
	resource.frame_set = frame_set;
	resource.name = file_path;
	resource.resume = resume;
	m_pd.resource_list.push_back(resource);

	return frame_set;
}
