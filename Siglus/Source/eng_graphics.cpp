#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_def.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_lexer.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_local_data.h"
#include	"data/tnm_cg_table.h"
#include	"data/tnm_sprite.h"

#include	"engine/eng_dir.h"
#include	"engine/eng_scene.h"
#include	"engine/ifc_error.h"
#include	"engine/eng_graphics.h"

#include	<png.h>

// ****************************************************************
// Direct3D ���\�[�X�}�l�[�W���[
// ================================================================
C_tnm_d3d_resource_manager	G_rsm;

// ****************************************************************
// Direct3D ���_�t�H�[�}�b�g
// ================================================================
const DWORD	S_vtx_d2_filter::FVF		= D3DFVF_XYZRHW                 | D3DFVF_DIFFUSE;
const DWORD	S_vtx_d3_filter::FVF		= D3DFVF_XYZ                    | D3DFVF_DIFFUSE;
const DWORD S_vtx_d2_pct::FVF			= D3DFVF_XYZRHW                 | D3DFVF_DIFFUSE | D3DFVF_TEX1;
const DWORD S_vtx_d2_pct_mask::FVF		= D3DFVF_XYZRHW                 | D3DFVF_DIFFUSE | D3DFVF_TEX2;
const DWORD S_vtx_d3_pct::FVF			= D3DFVF_XYZ    | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
const DWORD S_vtx_d2_screen::FVF		= D3DFVF_XYZRHW                 | D3DFVF_DIFFUSE | D3DFVF_TEX1;
const DWORD S_vtx_d3_screen::FVF		= D3DFVF_XYZ                    | D3DFVF_DIFFUSE | D3DFVF_TEX1;
const DWORD S_vtx_d2_mask_wipe::FVF		= D3DFVF_XYZRHW                 | D3DFVF_DIFFUSE | D3DFVF_TEX2;

// ****************************************************************
// ��`�X�v���C�g���ʂ̒��_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@
// ================================================================
BSP<C_d3d_vertex_buffer>	G_rect_vertex_buffer_d2_filter;
BSP<C_d3d_vertex_buffer>	G_rect_vertex_buffer_d3_filter;
BSP<C_d3d_vertex_buffer>	G_rect_vertex_buffer_d2_pct;
BSP<C_d3d_vertex_buffer>	G_rect_vertex_buffer_d2_pct_mask;
BSP<C_d3d_vertex_buffer>	G_rect_vertex_buffer_d3_pct;
BSP<C_d3d_vertex_buffer>	G_rect_vertex_buffer_d2_screen;
BSP<C_d3d_vertex_buffer>	G_rect_vertex_buffer_d3_screen;
BSP<C_d3d_vertex_buffer>	G_rect_vertex_buffer_d2_mask_wipe;
BSP<C_d3d_index_buffer>		G_rect_index_buffer;

// ****************************************************************
// ��`�p�̃C���f�b�N�X�o�b�t�@���X�V����
// ================================================================
bool tnm_update_rect_index_buffer(int rect_cnt)
{
	int index_cnt = rect_cnt * 6;
	int index_buffer_size = index_cnt * sizeof(WORD);

	// �C���f�b�N�X�o�b�t�@������Ȃ��ꍇ�Ɋg������
	if (G_rect_index_buffer->get_buffer_size() < index_buffer_size)	{

		// �C���f�b�N�X�o�b�t�@���쐬����
		if (!G_rect_index_buffer->create_index_buffer(index_cnt, 0, D3DPOOL_MANAGED))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("��`�p�̋��p�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂����B\n") + get_last_error_msg());
			return false;
		}

		// �C���f�b�N�X�o�b�t�@�Ɉꊇ���ď�������
		WORD* p_locked_index_buffer;
		if (!G_rect_index_buffer->lock(0, index_buffer_size, (void **)&p_locked_index_buffer, 0))	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("��`�p�̋��p�C���f�b�N�X�o�b�t�@�̃��b�N�Ɏ��s���܂����B\n") + get_last_error_msg());
			return false;
		}

		for (int i = 0; i < rect_cnt; i++)	{
			p_locked_index_buffer[i * 6 + 0] = i * 4 + 0;
			p_locked_index_buffer[i * 6 + 1] = i * 4 + 1;
			p_locked_index_buffer[i * 6 + 2] = i * 4 + 2;
			p_locked_index_buffer[i * 6 + 3] = i * 4 + 2;
			p_locked_index_buffer[i * 6 + 4] = i * 4 + 1;
			p_locked_index_buffer[i * 6 + 5] = i * 4 + 3;
		}

		if (!G_rect_index_buffer->unlock())	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("��`�p�̋��p�C���f�b�N�X�o�b�t�@�̃A�����b�N�Ɏ��s���܂����B\n\n") + get_last_error_msg());
			return false;
		}

		// �C���f�b�N�X�o�b�t�@�T�C�Y���L��
		Gp_global->index_buffer_size = index_buffer_size;
	}

	return true;
}

// ****************************************************************
// ���_�o�b�t�@���X�V����
// ================================================================
bool tnm_update_vertex_list(ARRAY<C_tnm_sprite *>& sprite_list, ARRAY<C_tnm_sprite *>& sprite_list_ret)
{
	enum E_type
	{
		E_type_none = -1,
		E_d2_filter = 0,
		E_d3_filter,
		E_d2_pct,
		E_d2_pct_mask,
		E_d3_pct,
		E_d2_mask_wipe,
		E_type_max,
	};

	struct	S_type
	{
		ARRAY<BYTE> vertex_buffer_data;
		C_d3d_vertex_buffer* p_vertex_buffer;
		int vertex_cnt;
		int index_cnt;
		int FVF;
		int FVF_size;

		S_type()
		{
			vertex_cnt = 0;
			index_cnt = 0;
		}

		void set_param(C_d3d_vertex_buffer* p_vertex_buffer_, int FVF_, int FVF_size_)
		{
			p_vertex_buffer = p_vertex_buffer_;
			FVF = FVF_;
			FVF_size = FVF_size_;
		}
	};

	S_type DATA[E_type_max];


	// ����ݒ�
	DATA[E_d2_filter].set_param(G_rect_vertex_buffer_d2_filter.get(), S_vtx_d2_filter::FVF, sizeof(S_vtx_d2_filter));
	DATA[E_d3_filter].set_param(G_rect_vertex_buffer_d3_filter.get(), S_vtx_d3_filter::FVF, sizeof(S_vtx_d3_filter));
	DATA[E_d2_pct].set_param(G_rect_vertex_buffer_d2_pct.get(), S_vtx_d2_pct::FVF, sizeof(S_vtx_d2_pct));
	DATA[E_d2_pct_mask].set_param(G_rect_vertex_buffer_d2_pct_mask.get(), S_vtx_d2_pct_mask::FVF, sizeof(S_vtx_d2_pct_mask));
	DATA[E_d3_pct].set_param(G_rect_vertex_buffer_d3_pct.get(), S_vtx_d3_pct::FVF, sizeof(S_vtx_d3_pct));
	DATA[E_d2_mask_wipe].set_param(G_rect_vertex_buffer_d2_mask_wipe.get(), S_vtx_d2_mask_wipe::FVF, sizeof(S_vtx_d2_mask_wipe));

	// �قƂ�ǂ̃X�v���C�g�� S_vtx_d2_pct ���Ǝv���̂ŁA�X�v���C�g�����������炩���� reserve ���Ă����܂��B
	DATA[E_d2_pct].vertex_buffer_data.reserve(sprite_list.size() * 4 * sizeof(S_vtx_d2_pct));

	// �`�悷��X�v���C�g�݂̂����X�g�ɒǉ����Ă���
	for (ARRAY<C_tnm_sprite *>::iterator itr = sprite_list.begin(); itr != sprite_list.end(); ++itr)	{
		E_type type = E_type_none;

		if (false);
		else if (*itr && (*itr)->rp.disp && (*itr)->rp.d2_rect && (*itr)->get_FVF() == S_vtx_d2_filter::FVF)	type = E_d2_filter;
		else if (*itr && (*itr)->rp.disp && (*itr)->rp.d3_rect && (*itr)->get_FVF() == S_vtx_d3_filter::FVF)	type = E_d3_filter;
		else if (*itr && (*itr)->rp.disp && (*itr)->rp.d2_rect && (*itr)->get_FVF() == S_vtx_d2_pct::FVF)		type = E_d2_pct;
		else if (*itr && (*itr)->rp.disp && (*itr)->rp.d2_rect && (*itr)->get_FVF() == S_vtx_d2_pct_mask::FVF)	type = E_d2_pct_mask;
		else if (*itr && (*itr)->rp.disp && (*itr)->rp.d3_rect && (*itr)->get_FVF() == S_vtx_d3_pct::FVF)		type = E_d3_pct;
		else if (*itr && (*itr)->rp.disp && (*itr)->rp.d2_rect && (*itr)->get_FVF() == S_vtx_d2_mask_wipe::FVF)	{
			type = E_d2_mask_wipe;
		}
		else if (*itr && (*itr)->rp.disp && !(*itr)->rp.d2_rect && !(*itr)->rp.d3_rect)	{	// ���̑�
			sprite_list_ret.push_back(*itr);
		}

		if (type != E_type_none)	{

			// ���_���𒸓_�o�b�t�@�W�J����
			DATA[type].vertex_buffer_data.resize((DATA[type].vertex_cnt + 4) * DATA[type].FVF_size);
			if ((*itr)->set_d2_vertex_param(DATA[type].vertex_cnt, 4, DATA[type].index_cnt, 6, 2, DATA[type].vertex_buffer_data.get(), NULL, NULL))	{
				sprite_list_ret.push_back(*itr);
				DATA[type].vertex_cnt += 4;		// �S���_�����₷
				DATA[type].index_cnt += 6;		// �U�C���f�b�N�X�����₷
			}
		}
	}

	// ���_�̏���
	for (int type = 0; type < E_type_max; type++)	{

		if (DATA[type].vertex_cnt > 0)	{
		
			// ���_�o�b�t�@������Ă��Ȃ���΍�蒼��
			if (!DATA[type].p_vertex_buffer || DATA[type].p_vertex_buffer->get_vertex_count() < DATA[type].vertex_cnt)	{
				if (!DATA[type].p_vertex_buffer->create_vertex_buffer(DATA[type].vertex_cnt, DATA[type].FVF, DATA[type].FVF_size, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DPOOL_SYSTEMMEM))
					return false;
			}

			// ���_�o�b�t�@�Ɉꊇ���ď�������
			void* p_locked_vertex_buffer;
			if (!DATA[type].p_vertex_buffer->lock(0, DATA[type].vertex_cnt * DATA[type].FVF_size, &p_locked_vertex_buffer, D3DLOCK_DISCARD))
				return false;

			memcpy(p_locked_vertex_buffer, DATA[type].vertex_buffer_data.get(), DATA[type].vertex_cnt * DATA[type].FVF_size);
			if (!DATA[type].p_vertex_buffer->unlock())
				return false;
		}
	}

	// ���_�o�b�t�@�T�C�Y���L��
	int total_vertex_buffer_size = 0;
	for (int type = 0; type < E_type_max; type++)	{
		total_vertex_buffer_size += DATA[type].vertex_cnt * DATA[type].FVF_size;
	}
	if (total_vertex_buffer_size > Gp_global->max_vertex_buffer_size)
		Gp_global->max_vertex_buffer_size = total_vertex_buffer_size;

	// �C���f�b�N�X�o�b�t�@�̏���
	if (sprite_list_ret.size() > 0)	{

		// �C���f�b�N�X�o�b�t�@���g������
		tnm_update_rect_index_buffer((int)sprite_list_ret.size());
	}

	return true;
}

// ****************************************************************
// �摜��ǂݍ���
// ================================================================

// �摜�̑��݂𔻒肷��
bool tnm_check_pct(CTSTR& file_name, bool set_error)
{
	int file_type;

	TSTR file_path = tnm_find_g00(Gp_dir->base_dir, _T("g00"), file_name, &file_type, true);

	// �t�@�C����������Ȃ�����
	if (file_path.empty())
	{
		if (set_error)	{
			tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("�摜�t�@�C�� ") + file_name + _T(" ��������܂���ł����B"));
		}

		return false;
	}

	return true;
}

// �s�N�`�����Q�c�A���o���ɓǂݍ���
BSP<C_album> tnm_load_pct(CTSTR& file_name, bool set_cg_table)
{
	int file_type;

	TSTR file_path = tnm_find_g00(Gp_dir->base_dir, _T("g00"), file_name, &file_type, true);

	// �t�@�C����������Ȃ�����
	if (file_path.empty())
	{
		tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("�摜�t�@�C�� ") + file_name + _T(" ��������܂���ł����B"));
		return BSP<C_album>();
	}

	// �摜��ǂݍ���
	BSP<C_album> album(new C_album);
	switch (file_type)	{

	// g00 �̏ꍇ�̓A���o���ɒ��ړǂݍ���
	case 1:
		if (!load_g00_to_album(album.get(), file_path))	{
			tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("�摜�t�@�C�� ") + file_path + _T(" ���J���܂���ł����B"));
			return BSP<C_album>();
		}
		break;

	// ���̑��̏ꍇ�̓s�N�`���ɓǂݍ���ł���A���o���ɒǉ��i���������j
	default:
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�Ή����Ă��Ȃ��`���ł��B"));
		break;
	}

	// �b�f�e�[�u���ɓo�^����
	if (set_cg_table && !Gp_local->pod.cg_table_off_flag)
	{
		Gp_cg_table->set_flag_value_from_name(file_name, 1);
	}

	return album;
}

// �s�N�`�����R�c�A���o���ɓǂݍ���
bool tnm_load_pct_d3d_sub_split_file_name(TSTR original_file_name, ARRAY<C_d3d_album_compose_param>& param_list)
{
	bool result = true;
	TSTR::const_iterator end = original_file_name.end();

	// �X�y�[�X���폜
	::str_erase_moji(original_file_name, _T(' '));

	// ���
	C_d3d_album_compose_param param;
	TSTR::const_iterator word_begin_itr = original_file_name.begin();
	for (TSTR::const_iterator f_itr = original_file_name.begin();;)
	{
		if (f_itr == original_file_name.end())
		{
			// �t�@�C�����m��
			if (param.file_name.empty())
			{
				param.file_name = TSTR(word_begin_itr, f_itr);
			}
			// ���m��
			param_list.push_back(param);
			// �I��
			break;
		}
		else if (*f_itr == _T('|'))
		{
			// �t�@�C�����m��
			if (param.file_name.empty())
			{
				param.file_name = TSTR(word_begin_itr, f_itr);
			}
			// ���m��
			param_list.push_back(param);
			// ����������
			param.init();
			// ����
			++ f_itr;
			word_begin_itr = f_itr;
		}
		else if (*f_itr == _T('('))
		{
			// �t�@�C�����m��
			if (param.file_name.empty())
			{
				param.file_name = TSTR(word_begin_itr, f_itr);
			}
			// ����
			++ f_itr;
			// ���W��ǂݍ���
			result &= str_analize_get_integer(f_itr, end, &param.x);
			result &= str_analize_check_moji(f_itr, end, _T(','));
			result &= str_analize_get_integer(f_itr, end, &param.y);
			while (str_analize_check_moji(f_itr, end, _T(',')))
			{
				if (str_analize_check_str(f_itr, end, _T("blend")))
				{
					result &= str_analize_check_moji(f_itr, end, _T('='));
					result &= str_analize_get_integer(f_itr, end, &param.blend_type);
				}
				else
				{
					result &= str_analize_get_integer(f_itr, end, &param.cut_no);
				}
			}
			result &= str_analize_check_moji(f_itr, end, _T(')'));
			if (!result)
			{
				return false;		// ��͂Ɏ��s�c�B
			}
			word_begin_itr = f_itr;
		}
		else
		{
			++ f_itr;
		}
	}

	return true;
}

// *****************************************************************************************************************************
// �摜���e�N�X�`���ɓǂݍ��݂܂��B
// =============================================================================================================================
//
//		���͂͂���Ȋ����ł��B
//
//			dir_path = "Q:\\game"
//			dir_path = "C:\\MyDocuments\\game\\savedata", rltv_dir_path = ""
//			file_name = "bg001"
//			file_name = "bst001|eye001(100,100)"
//
//		find_append == true �̏ꍇ�̓A�y���h���������A"\\g00" ��t�����܂��B
//		�����炭 g00 �t�H���_�̏ꍇ�͌�������Asavedata �t�H���_�̏ꍇ�͌����Ȃ��ł��傤�B
//
//		new_file_path = "Q:\\game\\g00\\bst001.g00|Q:\\game\\g00\\eye001.g00(0,0)"
//		���ŏI�I�ɂ���Ȍ`�̃p�X���쐬���A���\�[�X�}�l�[�W���[�ɓn���܂��B�A�y���h�����ނ̂łP�P���t���p�X�ɂȂ�킯�ł��B
//
// =============================================================================================================================
BSP<C_d3d_album> tnm_load_pct_d3d_sub(CTSTR& base_dir, CTSTR& sub_dir, CTSTR& file_name, bool fuchidori, bool reload, bool set_cg_table, bool find_append)
{
	BSP<C_d3d_album> album;

	// �����e�N�X�`���̏ꍇ
	// �K���u�t�@�C�����v�Ŕ��肷�邱�ƁB�t�@�C���p�X�Ŕ��肷��ƁA�C���X�g�[���f�B���N�g���� '|' �������Ă�ƃo�O��܂��B
	if (file_name.find(_T('|')) != TSTR::npos)
	{
		// �t�@�C�����𕪊�
		ARRAY<C_d3d_album_compose_param> param_list;
		if (!tnm_load_pct_d3d_sub_split_file_name(file_name, param_list))
		{
			tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("�摜�t�@�C�� ") + file_name + _T(" �̕������͂Ɏ��s���܂����B"));
			return BSP<C_d3d_album>();
		}

		// �t�@�C���`�F�b�N�ig00 �̂ݑΉ��j
		for (int i = 0; i < (int)param_list.size(); i++)
		{
			// �P�ڂ̃t�@�C���ȊO�̓t�@�C�����Ȃ��͋�����Ȃ�
			if (i > 0 && param_list[i].file_name.empty())
			{
				tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, tostr(i) + _T("�ڂ̎w��̉摜�t�@�C��������ł��B"));
				return BSP<C_d3d_album>();
			}

			// �t�@�C����������ꍇ
			if (!param_list[i].file_name.empty())
			{
				int file_type;

				TSTR file_path = tnm_find_g00(base_dir, sub_dir, param_list[i].file_name, &file_type, find_append);

				if (file_path.empty())
				{
					tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("�摜�t�@�C�� ") + param_list[i].file_name + _T(" ��������܂���ł����B"));
					return BSP<C_d3d_album>();
				}
				else if (file_type != 1)
				{
					tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("g00 �ȊO�̉摜") + param_list[i].file_name + _T(" ���w�肳��Ă��܂��B�����e�N�X�`���� g00 �̂ݑΉ����Ă��܂��B"));
					return BSP<C_d3d_album>();
				}

				// �t���p�X�����߂�
				param_list[i].file_path = file_path;
			}
		}

		// ���߂ăt�@�C���p�X���쐬����
		TSTR new_file_path;
		for (int i = 0; i < (int)param_list.size(); i++)
		{
			if (i == 0 && param_list[i].file_path.empty())
			{
				new_file_path += _T("(") + tostr(param_list[i].x) + _T(",") + tostr(param_list[i].y) + _T(",") + tostr(param_list[i].cut_no) + _T(",blend=") + tostr(param_list[i].blend_type) + _T(")");
			}
			else if (i == 0)
			{
				new_file_path += param_list[i].file_path;
				new_file_path += _T("(") + tostr(param_list[i].x) + _T(",") + tostr(param_list[i].y) + _T(",") + tostr(param_list[i].cut_no) + _T(",blend=") + tostr(param_list[i].blend_type) + _T(")");
			}
			else
			{
				new_file_path += _T("|") + param_list[i].file_path;
				new_file_path += _T("(") + tostr(param_list[i].x) + _T(",") + tostr(param_list[i].y) + _T(",") + tostr(param_list[i].cut_no) + _T(",blend=") + tostr(param_list[i].blend_type) + _T(")");
			}
		}

		// �����e�N�X�`����ǂݍ���
		album = G_rsm.create_album_from_g00(new_file_path, fuchidori, false);
		if (!album)
		{
			tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("�����摜 ") + file_name + _T(" ���J���܂���ł����B\n") + get_last_error_msg());
			return BSP<C_d3d_album>();
		}

		// �b�f�e�[�u���ɓo�^����
		if (set_cg_table && !Gp_local->pod.cg_table_off_flag)
		{
			for (int i = 0; i < (int)param_list.size(); i++)
			{
				Gp_cg_table->set_flag_value_from_name(param_list[i].file_name, 1);
			}
		}
	}

	// �ʏ�摜
	else	{

		int file_type;

		TSTR file_path = tnm_find_g00(base_dir, sub_dir, file_name, &file_type, find_append);

		if (file_path.empty())
		{
			tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("�摜�t�@�C�� ") + file_name + _T(" ��������܂���ł����B"));
			return BSP<C_d3d_album>();
		}

		// �摜��ǂݍ���
		BSP<C_d3d_texture> texture;
		switch (file_type)	{

			// g00 �̏ꍇ�̓A���o���ɒ��ړǂݍ���
			case 1:
				album = G_rsm.create_album_from_g00(file_path, fuchidori, false);
				if (!album)	{
					tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("�摜�t�@�C�� ") + file_name + _T(" ���J���܂���ł����B\n") + get_last_error_msg());
					return BSP<C_d3d_album>();
				}
				break;

			// ���̑��̏ꍇ�̓e�N�X�`���ɓǂݍ���ł���A���o���ɒǉ�
			default:
				texture = G_rsm.create_texture_from_file(file_path, false, reload);
				if (!texture)	{
					tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("�摜�t�@�C�� ") + file_name + _T(" ���J���܂���ł����B\n") + get_last_error_msg());
					return BSP<C_d3d_album>();
				}

				// �A���o���ɒǉ�����
				album = BSP<C_d3d_album>(new C_d3d_album);
				album->add_texture(texture);
				break;
		}

		// �b�f�e�[�u���ɓo�^����
		if (set_cg_table && !Gp_local->pod.cg_table_off_flag)
		{
			Gp_cg_table->set_flag_value_from_name(file_name, 1);
		}
	}

	return album;
}

// �s�N�`�����R�c�A���o���ɓǂݍ��ށig00 �t�H���_�j
BSP<C_d3d_album> tnm_load_pct_d3d(CTSTR& file_name, bool fuchidori, bool set_cg_table)
{
	return tnm_load_pct_d3d_sub(Gp_dir->base_dir, _T("g00"), file_name, fuchidori, false, set_cg_table, true);
}

// �s�N�`�����R�c�A���o���ɓǂݍ��ށisave �t�H���_�j
BSP<C_d3d_album> tnm_load_pct_d3d_from_save_thumb(int save_no)
{
	TSTR file_name = Gp_dir->get_save_thumb_file_name(save_no);

	// �������ݎ��͊g���q����菜��
	::erase_extension(file_name);

	// �������I�Ƀ����[�h����I
	return tnm_load_pct_d3d_sub(Gp_dir->save_dir, _T(""), file_name, false, true, false, false);
}

// �s�N�`�����R�c�A���o���ɓǂݍ��ށisave �t�H���_�j
BSP<C_d3d_album> tnm_load_pct_d3d_from_thumb(int thumb_no)
{
	TSTR file_name = Gp_dir->get_thumb_file_name(thumb_no);

	// �������ݎ��͊g���q����菜��
	::erase_extension(file_name);

	// �������I�Ƀ����[�h����I
	return tnm_load_pct_d3d_sub(Gp_dir->save_dir, _T(""), file_name, false, true, false, false);
}

// ****************************************************************
// �X�v���C�g�p�����[�^�A�����_�[�p�����[�^����������i�p�~�\��j
// ================================================================
void tnm_affect_sp_and_rp(S_tnm_sprite_render_param* rpex, const S_tnm_sprite_render_param* parent_rpex, S_d3d_render_param* rp, const S_d3d_render_param* parent_rp)
{
	// �e�̂ƂȂ܃X�v���C�g�p�����[�^��������
	if (rpex && parent_rpex)	{

		rpex->sorter = rpex->sorter + S_tnm_sorter(parent_rpex->sorter.order, parent_rpex->sorter.layer);

		// ���[���h
		if (rpex->p_world == NULL)	{	// �q���f�t�H���g�ꍇ�̂ݐe�������p��
			rpex->p_world = parent_rpex->p_world;
		}
	}

	// �e�̃����_�[�p�����[�^��������
	if (rp && parent_rp)	{

		rp->disp = rp->disp && parent_rp->disp;
		rp->pos.x = (rp->pos.x - parent_rp->center.x) * parent_rp->scale.x + parent_rp->center.x;			// scale
		rp->pos.y = (rp->pos.y - parent_rp->center.y) * parent_rp->scale.y + parent_rp->center.y;			// scale
		rp->pos.z = (rp->pos.z - parent_rp->center.z) * parent_rp->scale.z + parent_rp->center.z;			// scale
		{
			float tmp_x = rp->pos.x;
			float tmp_y = rp->pos.y;
			rp->pos.x = (tmp_x - parent_rp->center.x) * cos(parent_rp->rotate.z) - (tmp_y - parent_rp->center.y) * sin(parent_rp->rotate.z) + parent_rp->center.x;		// rotate
			rp->pos.y = (tmp_x - parent_rp->center.x) * sin(parent_rp->rotate.z) + (tmp_y - parent_rp->center.y) * cos(parent_rp->rotate.z) + parent_rp->center.y;		// rotate
		}
		rp->pos.x += parent_rp->pos.x;																		// move
		rp->pos.y += parent_rp->pos.y;																		// move
		rp->pos.z += parent_rp->pos.z;																		// move
		rp->scale.x *= parent_rp->scale.x;
		rp->scale.y *= parent_rp->scale.y;
		rp->scale.z *= parent_rp->scale.z;
		rp->rotate.x += parent_rp->rotate.x;
		rp->rotate.y += parent_rp->rotate.y;
		rp->rotate.z += parent_rp->rotate.z;
		rp->tr = rp->tr * parent_rp->tr / 255;
		rp->mono = 255 - (255 - rp->mono) * (255 - parent_rp->mono) / 255;
		rp->reverse = 255 - (255 - rp->reverse) * (255 - parent_rp->reverse) / 255;
		rp->bright = 255 - (255 - rp->bright) * (255 - parent_rp->bright) / 255;
		rp->dark = 255 - (255 - rp->dark) * (255 - parent_rp->dark) / 255;
		
		// �F�ω�
		if (rp->color_rate + parent_rp->color_rate > 0)	{
			int parent_rate = parent_rp->color_rate * 255 * 255 / (255 * 255 - (255 - rp->color_rate) * (255 - parent_rp->color_rate));
			rp->color_r = (rp->color_r * (255 - parent_rate) + parent_rp->color_r * parent_rate) / 255;
			rp->color_g = (rp->color_g * (255 - parent_rate) + parent_rp->color_g * parent_rate) / 255;
			rp->color_b = (rp->color_b * (255 - parent_rate) + parent_rp->color_b * parent_rate) / 255;
		}
		rp->color_rate = 255 - (255 - rp->color_rate) * (255 - parent_rp->color_rate) / 255;

		// �F���Z
		rp->color_add_r = limit(0, rp->color_add_r + parent_rp->color_add_r, 255);
		rp->color_add_g = limit(0, rp->color_add_g + parent_rp->color_add_g, 255);
		rp->color_add_b = limit(0, rp->color_add_b + parent_rp->color_add_b, 255);

		// �\����`
		if (parent_rp->dst_clip_use)	{
			if (rp->dst_clip_use)	{		// �e���q���g���ꍇ�͍�������
				rp->dst_clip.left = std::max(rp->dst_clip.left, parent_rp->dst_clip.left);
				rp->dst_clip.top = std::max(rp->dst_clip.top, parent_rp->dst_clip.top);
				rp->dst_clip.right = std::min(rp->dst_clip.right, parent_rp->dst_clip.right);
				rp->dst_clip.bottom = std::min(rp->dst_clip.bottom, parent_rp->dst_clip.bottom);
			}
			else	{
				rp->dst_clip_use = true;	// �e�������g���ꍇ�͈����p��
				rp->dst_clip = parent_rp->dst_clip;
			}
		}

		// �g�[���J�[�u
		if (rp->tone_curve_no == TNM_TONE_CURVE_NONE)	{	// �q���f�t�H���g�̏ꍇ�̂ݐe�������p��
			rp->tone_curve_no = parent_rp->tone_curve_no;
			rp->tone_curve_sat = parent_rp->tone_curve_sat;
		}

		// �������@
		if (rp->blend_type == E_d3d_blend_type_alpha)	{	// �q���f�t�H���g�ꍇ�̂ݐe�������p��
			rp->blend_type = parent_rp->blend_type;
		}
	}
}

// ****************************************************************
// �e�� trp ������
// ================================================================
void tnm_add_parent_trp(S_tnm_render_param* trp, const S_tnm_render_param* parent_trp)
{
	if (!trp || !parent_trp)
		return;

	// �`��
	trp->disp = trp->disp && parent_trp->disp;

	// �\�[�^�[
	trp->sorter = trp->sorter + S_tnm_sorter(parent_trp->sorter.order, parent_trp->sorter.layer);

	// ���[���h
	if (trp->p_world == NULL)	// �q���f�t�H���g�ꍇ�̂ݐe�������p��
	{
		trp->p_world = parent_trp->p_world;
	}

	// �ʒu���

#if 0	// �q���̉�]�ɑΉ��ł��Ȃ��Ď��s�����o�[�W����
	trp->center_x += parent_trp->center_x - trp->pos_x;
	trp->center_y += parent_trp->center_y - trp->pos_y;
	trp->center_z += parent_trp->center_z - trp->pos_z;
	trp->pos_x = parent_trp->pos_x;
	trp->pos_y = parent_trp->pos_y;
	trp->pos_z = parent_trp->pos_z;
	trp->scale_x *= parent_trp->scale_x;
	trp->scale_y *= parent_trp->scale_y;
	trp->scale_z *= parent_trp->scale_z;
	trp->rotate_x += parent_trp->rotate_x;
	trp->rotate_y += parent_trp->rotate_y;
	trp->rotate_z += parent_trp->rotate_z;

#else	// �e�Ǝq���̗�������]���Ă����܂��������킹���o�[�W����
	trp->pos_x = (trp->pos_x - parent_trp->center_rep_x) * parent_trp->scale_x + parent_trp->center_rep_x;			// scale
	trp->pos_y = (trp->pos_y - parent_trp->center_rep_y) * parent_trp->scale_y + parent_trp->center_rep_y;			// scale
	trp->pos_z = (trp->pos_z - parent_trp->center_rep_z) * parent_trp->scale_z + parent_trp->center_rep_z;			// scale
	{
		float tmp_x = trp->pos_x;
		float tmp_y = trp->pos_y;
		trp->pos_x = (tmp_x - parent_trp->center_rep_x) * cos(parent_trp->rotate_z) - (tmp_y - parent_trp->center_rep_y) * sin(parent_trp->rotate_z) + parent_trp->center_rep_x;		// rotate
		trp->pos_y = (tmp_x - parent_trp->center_rep_x) * sin(parent_trp->rotate_z) + (tmp_y - parent_trp->center_rep_y) * cos(parent_trp->rotate_z) + parent_trp->center_rep_y;		// rotate
	}
	trp->pos_x += parent_trp->pos_x;																		// move
	trp->pos_y += parent_trp->pos_y;																		// move
	trp->pos_z += parent_trp->pos_z;																		// move
	trp->scale_x *= parent_trp->scale_x;
	trp->scale_y *= parent_trp->scale_y;
	trp->scale_z *= parent_trp->scale_z;
	trp->rotate_x += parent_trp->rotate_x;
	trp->rotate_y += parent_trp->rotate_y;
	trp->rotate_z += parent_trp->rotate_z;
#endif

	// �\���͈�
	if (parent_trp->dst_clip_use)	{
		if (trp->dst_clip_use)	{		// �e���q���g���ꍇ�͍�������
			trp->dst_clip_left = std::min(trp->dst_clip_left, parent_trp->dst_clip_left);
			trp->dst_clip_top = std::min(trp->dst_clip_top, parent_trp->dst_clip_top);
			trp->dst_clip_right = std::max(trp->dst_clip_right, parent_trp->dst_clip_right);
			trp->dst_clip_bottom = std::max(trp->dst_clip_bottom, parent_trp->dst_clip_bottom);
		}
		else	{
			trp->dst_clip_use = true;	// �e�������g���ꍇ�͈����p��
			trp->dst_clip_left = parent_trp->dst_clip_left;
			trp->dst_clip_top = parent_trp->dst_clip_top;
			trp->dst_clip_right = parent_trp->dst_clip_right;
			trp->dst_clip_bottom = parent_trp->dst_clip_bottom;
		}
	}

	// �F���
	trp->tr = trp->tr * parent_trp->tr / 255;
	trp->mono = 255 - (255 - trp->mono) * (255 - parent_trp->mono) / 255;
	trp->reverse = 255 - (255 - trp->reverse) * (255 - parent_trp->reverse) / 255;
	trp->bright = 255 - (255 - trp->bright) * (255 - parent_trp->bright) / 255;
	trp->dark = 255 - (255 - trp->dark) * (255 - parent_trp->dark) / 255;

	// �F�ω�
	if (trp->color_rate + parent_trp->color_rate > 0)	{
		int parent_rate = parent_trp->color_rate * 255 * 255 / (255 * 255 - (255 - trp->color_rate) * (255 - parent_trp->color_rate));
		trp->color_r = (trp->color_r * (255 - parent_rate) + parent_trp->color_r * parent_rate) / 255;
		trp->color_g = (trp->color_g * (255 - parent_rate) + parent_trp->color_g * parent_rate) / 255;
		trp->color_b = (trp->color_b * (255 - parent_rate) + parent_trp->color_b * parent_rate) / 255;
	}
	trp->color_rate = 255 - (255 - trp->color_rate) * (255 - parent_trp->color_rate) / 255;

	// �F���Z
	trp->color_add_r = limit(0, trp->color_add_r + parent_trp->color_add_r, 255);
	trp->color_add_g = limit(0, trp->color_add_g + parent_trp->color_add_g, 255);
	trp->color_add_b = limit(0, trp->color_add_b + parent_trp->color_add_b, 255);

	// �}�X�N
	if (trp->mask_no == TNM_MASK_NONE)	{				// �q���f�t�H���g�̏ꍇ�̂ݐe�������p��
		trp->mask_no = parent_trp->mask_no;
	}
	// �g�[���J�[�u
	if (trp->tone_curve_no == TNM_TONE_CURVE_NONE)	{	// �q���f�t�H���g�̏ꍇ�̂ݐe�������p��
		trp->tone_curve_no = parent_trp->tone_curve_no;
	}
	// ���̑�
	//trp->culling = trp->culling && parent_trp->culling;
	//trp->alpha_test = trp->alpha_test && parent_trp->alpha_test;
	//trp->alpha_blend = trp->alpha_blend && parent_trp->alpha_blend;

	// �������@
	if (trp->blend_type == TNM_BLEND_TYPE_NORMAL)	{	// �q���f�t�H���g�ꍇ�̂ݐe�������p��
		trp->blend_type = parent_trp->blend_type;
	}
}

// ****************************************************************
// trp �� rp �ɕϊ�
// ================================================================
void tnm_trp_to_rp(const S_tnm_render_param* trp, S_tnm_sprite_render_param* rpex, S_d3d_render_param* rp)
{
	// �����_�[�p�����[�^�d�w
	rpex->sorter = trp->sorter;
	rpex->p_world = trp->p_world;

	// �����_�[�p�����[�^
	rp->disp = trp->disp ? true : false;
	rp->d3_billboard = false;
	rp->pos.x = trp->pos_x + trp->center_rep_x;				// �ʒu�i�w�j
	rp->pos.y = trp->pos_y + trp->center_rep_y;				// �ʒu�i�x�j
	rp->pos.z = trp->pos_z + trp->center_rep_z;				// �ʒu�i�y�j
	rp->center.x = trp->center_x + trp->center_rep_x;		// ���S�i�w�j
	rp->center.y = trp->center_y + trp->center_rep_y;		// ���S�i�x�j
	rp->center.z = trp->center_z + trp->center_rep_z;		// ���S�i�y�j
	rp->scale.x = trp->scale_x;								// �g�k�i�w�j
	rp->scale.y = trp->scale_y;								// �g�k�i�x�j
	rp->scale.z = trp->scale_z;								// �g�k�i�y�j
	rp->rotate.x = trp->rotate_x;							// ��]�i�w�j
	rp->rotate.y = trp->rotate_y;							// ��]�i�x�j
	rp->rotate.z = trp->rotate_z;							// ��]�i�y�j
	rp->dst_clip_use = trp->dst_clip_use ? true : false;	// �\���͈͎g�p
	rp->dst_clip.left = trp->dst_clip_left;					// �\���͈�
	rp->dst_clip.top = trp->dst_clip_top;					// �\���͈�
	rp->dst_clip.right = trp->dst_clip_right;				// �\���͈�
	rp->dst_clip.bottom = trp->dst_clip_bottom;				// �\���͈�
	rp->src_clip_use = trp->src_clip_use ? true : false;	// �\���͈͎g�p
	rp->src_clip.left = trp->src_clip_left;					// �\���͈�
	rp->src_clip.top = trp->src_clip_top;					// �\���͈�
	rp->src_clip.right = trp->src_clip_right;				// �\���͈�
	rp->src_clip.bottom = trp->src_clip_bottom;				// �\���͈�

	// �s�N�Z��
	rp->tr = trp->tr;										// �s�����x
	rp->mono = trp->mono;									// ���m�N��
	rp->reverse = trp->reverse;								// ���]�x
	rp->bright = trp->bright;								// ���x
	rp->dark = trp->dark;									// �Óx
	rp->color_r = trp->color_r;								// �F�ϊ��i�q�j
	rp->color_g = trp->color_g;								// �F�ϊ��i�f�j
	rp->color_b = trp->color_b;								// �F�ϊ��i�a�j
	rp->color_rate = trp->color_rate;						// �F�ϊ��i���x�j
	rp->color_add_r = trp->color_add_r;						// �F���Z�i�q�j
	rp->color_add_g = trp->color_add_g;						// �F���Z�i�f�j
	rp->color_add_b = trp->color_add_b;						// �F���Z�i�a�j

	// ���̑�
	rp->culling = trp->culling;
	rp->alpha_test = trp->alpha_test;
	rp->alpha_blend = trp->alpha_blend;

	// �������@
	switch (trp->blend_type)	{
		case TNM_BLEND_TYPE_NORMAL:	rp->blend_type = E_d3d_blend_type_alpha;	break;
		case TNM_BLEND_TYPE_ADD:	rp->blend_type = E_d3d_blend_type_add;		break;
		case TNM_BLEND_TYPE_SUB:	rp->blend_type = E_d3d_blend_type_sub;		break;
		case TNM_BLEND_TYPE_MUL:	rp->blend_type = E_d3d_blend_type_mul;		break;
		case TNM_BLEND_TYPE_SCREEN:	rp->blend_type = E_d3d_blend_type_screen;	break;
		default:					rp->blend_type = E_d3d_blend_type_alpha;	break;
	}
}

// ****************************************************************
// bmp ���쐬
// ================================================================
bool tnm_create_bmp_from_texture(ARRAY<BYTE>& buffer, int width, int height, D3DLOCKED_RECT* p_rect)
{
	// �r�b�g�}�b�v�w�b�_�\����
	// ���̂Q���\���̂ɂ܂Ƃ߂Ă͂����܂���B
	// BITMAPFILEHEADER �� 14byte �����Ȃ����߁A2byte �قǃp�f�B���O�ɂ��󂫂��ł��Ă��܂��܂��B
	BITMAPFILEHEADER file_header = { 0 };
	BITMAPINFOHEADER info_header = { 0 };

	file_header.bfType = MAKEWORD('B', 'M');
	file_header.bfSize = sizeof(file_header) + sizeof(info_header) + width * height * 4;
	file_header.bfReserved1 = 0;
	file_header.bfReserved2 = 0;
	file_header.bfOffBits = sizeof(file_header) + sizeof(info_header);
	info_header.biSize = sizeof(info_header);
	info_header.biWidth = width;
	info_header.biHeight = -height;
	info_header.biPlanes = 1;
	info_header.biBitCount = 32;
	info_header.biCompression = 0;
	info_header.biSizeImage = 0;
	info_header.biXPelsPerMeter = 0;
	info_header.biYPelsPerMeter = 0;
	info_header.biClrUsed = 0;
	info_header.biClrImportant = 0;

	// �r�b�g�f�[�^���擾����
	buffer.resize(sizeof(file_header) + sizeof(info_header) + width * height * 4);
	BYTE* p = buffer.get();
	*(BITMAPFILEHEADER *)p = file_header;	p += sizeof(file_header);
	*(BITMAPINFOHEADER *)p = info_header;	p += sizeof(info_header);
	for (int y = 0; y < height; y++)	{
		BYTE* dst = buffer.get() + file_header.bfOffBits + width * 4 * y;
		BYTE* src = (BYTE *)p_rect->pBits + p_rect->Pitch * y;
		memcpy(dst, src, width * 4);
	}

	return true;
}

// ****************************************************************
// png ���쐬���t�@�C���ɕۑ�
// ================================================================
bool tnm_create_png_from_texture_and_save_to_file(CTSTR& file_path, int width, int height, D3DLOCKED_RECT* p_rect, bool use_alpha)
{
	BYTE* p_src;
	BYTE* p_dst;
	BYTE* p_dst_goal;

	// png ���쐬
	ARRAY<BYTE> png_buffer(width * height * sizeof(C_argb));;
	if (use_alpha)	{
		for (int y = 0; y < height; y++)	{
			p_src = (BYTE *)p_rect->pBits + y * p_rect->Pitch;
			p_dst = png_buffer.get() + y * width * sizeof(C_argb);
			p_dst_goal = p_dst + width * sizeof(C_argb);
			while (p_dst != p_dst_goal)	{
				*p_dst++ = *p_src++;
				*p_dst++ = *p_src++;
				*p_dst++ = *p_src++;
				*p_dst++ = *p_src++;
			}
		}
	}
	else	{
		for (int y = 0; y < height; y++)	{
			p_src = (BYTE *)p_rect->pBits + y * p_rect->Pitch;
			p_dst = png_buffer.get() + y * width * sizeof(C_argb);
			p_dst_goal = p_dst + width * sizeof(C_argb);
			while (p_dst != p_dst_goal)	{
				*p_dst++ = *p_src++;
				*p_dst++ = *p_src++;
				*p_dst++ = *p_src++;
				*p_dst++ = 255;	p_src++;
			}
		}
	}

	// png �̗�f�[�^��ݒ�
	ARRAY<png_byte *> p_row(height);
	for (int y = 0; y < height; y++)
		p_row[y] = png_buffer.get() + y * width * sizeof(C_argb);

	C_file file;
	if (!file.open(file_path, _T("wb")))
		return false;

	png_structp png_str = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop png_info = png_create_info_struct(png_str);
	png_init_io(png_str, file.get_fp());
	//png_set_filter(png_str, PNG_FILTER_TYPE_BASE, PNG_FILTER_SUB);
	png_set_IHDR(png_str, png_info, width, height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_str, png_info);
	png_set_bgr(png_str);
	png_write_image(png_str, p_row.get());
	png_write_end(png_str, png_info);
	png_destroy_write_struct(&png_str, &png_info);

	return true;
}




