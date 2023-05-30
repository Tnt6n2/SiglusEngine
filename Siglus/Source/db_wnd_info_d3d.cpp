#include	"pch.h"
#include	"ifc_eng.h"
#include	"dialog/db_wnd_info_d3d.h"
#include	"data/tnm_global_data.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�FDirect3D
// ================================================================

#define		REGIST_WINDOW_NAME_D3D_DEVICE		_T("db_wnd_info_d3d_device")
#define		REGIST_WINDOW_NAME_D3D_RESOURCE		_T("db_wnd_info_d3d_resource")

// ****************************************************************
// �f�o�b�O���E�B���h�E�FDirect3D - �f�o�C�X
// ================================================================

// ������
void C_db_wnd_info_d3d_device::init()
{
	regist(REGIST_WINDOW_NAME_D3D_DEVICE, false, true, -1);

	// �L���v�V�������o�^
	regist_caption_name(_T("Direct3D �f�o�C�X���"));

	// ���X�g�r���[����
	listview_add_column(_T("state"), _T("�X�e�[�g"), 100);
	listview_add_column(_T("value"), _T("�l"), 100);
}

// �������t�@���N�V����
void C_db_wnd_info_d3d_device::on_init_func()
{
	// ���ڂ�ǉ�
	lv.add_item(0, _T("�r�f�I������"));
	lv.add_item(0, _T("�r�f�I�������g�p��"));
	lv.add_item(0, _T("�f�B�X�v���C�A�_�v�^"));
	lv.add_item(0, _T("�f�o�C�X�^�C�v"));
	lv.add_item(0, _T("���_�������@"));
	lv.add_item(0, _T("���_�V�F�[�_�̃o�[�W����"));
	lv.add_item(0, _T("���_�V�F�[�_�̒萔���W�X�^��"));
	lv.add_item(0, _T("�s�N�Z���V�F�[�_�̃o�[�W����"));
	lv.add_item(0, _T("�ő�e�N�X�`���T�C�Y"));
	lv.add_item(0, _T("2�̗ݏ�ȊO�̃e�N�X�`��"));
	lv.add_item(0, _T("ø������گ�ݸ� BORDER"));
	lv.add_item(0, _T("Я��ϯ�߂̎�������"));
	lv.add_item(0, _T("Я��ϯ�߂̎�������(X8R8G8B8 ø���)"));
	lv.add_item(0, _T("Я��ϯ�߂̎�������(A8R8G8B8 ø���)"));
	lv.add_item(0, _T("̨����̌���(X8R8G8B8 ø���)"));
	lv.add_item(0, _T("̨����̌���(A8R8G8B8 ø���)"));

	// �f�o�C�X�̏����擾
	const C_d3d_device_state& state = G_d3d.device.state();

	// �����l��ݒ�
	int item_no = 0;
	lv.set_cell_text(item_no++, 1, tostr((double)state.vm_size / 1024 / 1024) + _T(" MByte"));
	lv.set_cell_text(item_no++, 1, tostr((double)state.vm_size / 1024 / 1024) + _T(" MByte"));
	lv.set_cell_text(item_no++, 1, get_str_d3d_display_adapter(state.device_caps.AdapterOrdinal));
	lv.set_cell_text(item_no++, 1, get_str_D3DDEVTYPE(state.device_caps.DeviceType));
	lv.set_cell_text(item_no++, 1, get_str_d3d_vertex_process_type(state.vertex_process_type));
	lv.set_cell_text(item_no++, 1, tostr(D3DSHADER_VERSION_MAJOR(state.device_caps.VertexShaderVersion)) + _T(".") + tostr(D3DSHADER_VERSION_MINOR(state.device_caps.VertexShaderVersion)));
	lv.set_cell_text(item_no++, 1, tostr(state.device_caps.MaxVertexShaderConst));
	lv.set_cell_text(item_no++, 1, tostr(D3DSHADER_VERSION_MAJOR(state.device_caps.PixelShaderVersion)) + _T(".") + tostr(D3DSHADER_VERSION_MINOR(state.device_caps.PixelShaderVersion)));
	lv.set_cell_text(item_no++, 1, tostr(C_size(state.device_caps.MaxTextureWidth, state.device_caps.MaxTextureHeight)));
	lv.set_cell_text(item_no++, 1, (state.device_caps.TextureCaps & D3DPTEXTURECAPS_POW2) ? _T("No") : _T("Yes"));
	lv.set_cell_text(item_no++, 1, (state.device_caps.TextureAddressCaps & D3DPTADDRESSCAPS_BORDER) ? _T("Yes") : _T("No"));
	lv.set_cell_text(item_no++, 1, (state.device_caps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP) ? _T("Yes") : _T("No"));
	lv.set_cell_text(item_no++, 1, get_str_D3DERR(state.hr_D3DUSAGE_AUTOGENMIPMAP_X8R8G8B8_for_texture));
	lv.set_cell_text(item_no++, 1, get_str_D3DERR(state.hr_D3DUSAGE_AUTOGENMIPMAP_A8R8G8B8_for_texture));
	lv.set_cell_text(item_no++, 1, get_str_D3DERR(state.hr_D3DUSAGE_QUERY_FILTER_X8R8G8B8_for_texture));
	lv.set_cell_text(item_no++, 1, get_str_D3DERR(state.hr_D3DUSAGE_QUERY_FILTER_A8R8G8B8_for_texture));
}

// ����t�@���N�V����
void C_db_wnd_info_d3d_device::on_close_func()
{
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_d3d_device::frame()
{
	if (!m_h_wnd)
		return;

	// �ω�����l��ݒ�
	{
		STATE cd;

		cd.video_memory_use_size = Gp_global->video_memory_use_size;
		cd.video_memory_use_max_size = Gp_global->video_memory_use_max_size;

		if (frame_first_flag || cd.video_memory_use_size != m_ld.video_memory_use_size || cd.video_memory_use_max_size != m_ld.video_memory_use_max_size)
			lv.set_cell_text(1, 1, tostr(cd.video_memory_use_size) + _T("MB / ") + tostr(cd.video_memory_use_max_size) + _T("MB"));

		// �f�[�^���L��
		m_ld = cd;
	}

	// ����t���O�����낷
	frame_first_flag = false;
}

// ****************************************************************
// �f�o�b�O���E�B���h�E�FDirect3D - ���\�[�X
// ================================================================

// ������
void C_db_wnd_info_d3d_resource::init()
{
	regist(REGIST_WINDOW_NAME_D3D_RESOURCE, false, true, -1);

	// �L���v�V�������o�^
	regist_caption_name(_T("Direct3D ���\�[�X���"));

	// ���X�g�r���[����
	listview_add_column(_T("id"), _T("ID"), 50);
	listview_add_column(_T("type"), _T("����"), 50);
	listview_add_column(_T("enable"), _T("�L��"), 50);
	listview_add_column(_T("name"), _T("���O"), 50);
	listview_add_column(_T("moji"), _T("����"), 50);
	listview_add_column(_T("color"), _T("�F"), 50);
	listview_add_column(_T("texture_cnt"), _T("ø�����"), 50);
	listview_add_column(_T("size"), _T("����"), 50);
	listview_add_column(_T("size_ex"), _T("����ex"), 50);
	listview_add_column(_T("mip_level"), _T("Я������"), 50);
	listview_add_column(_T("usage"), _T("�g����"), 50);
	listview_add_column(_T("format"), _T("̫�ϯ�"), 50);
	listview_add_column(_T("pool"), _T("�Ǘ�"), 50);
}

// �������t�@���N�V����
void C_db_wnd_info_d3d_resource::on_init_func()
{
}

// ����t�@���N�V����
void C_db_wnd_info_d3d_resource::on_close_func()
{
}

// ****************************************************************
// �t���[��
// ================================================================
void C_db_wnd_info_d3d_resource::frame()
{
	if (!m_h_wnd)
		return;

	STATE now_state;

	// �f�[�^�̍X�V
	now_state.resource_list = G_rsm.pd().resource_list;

	// �A�C�e�����̍X�V
	if (frame_first_flag || now_state.resource_list.size() != last_state.resource_list.size())	{
		lv.set_item_cnt((int)now_state.resource_list.size());
	}

	// �f�[�^���X�V����Ă���΃��X�g�r���[���X�V
	for (int i = 0; i < (int)now_state.resource_list.size(); i++)	{
		bool change = frame_first_flag || i >= (int)last_state.resource_list.size();

		if (change || now_state.resource_list[i].id != last_state.resource_list[i].id)	{
			lv.set_cell_text(i, 0, tostr(now_state.resource_list[i].id));
		}
		if (change || now_state.resource_list[i].type != last_state.resource_list[i].type)	{
			lv.set_cell_text(i, 1, G_d3d_resource_type_define[now_state.resource_list[i].type]);
		}

		if (false);
		else if (now_state.resource_list[i].type == E_tnm_d3d_resource_type_texture || now_state.resource_list[i].type == E_tnm_d3d_resource_type_texture_from_g00 || now_state.resource_list[i].type == E_tnm_d3d_resource_type_texture_from_moji)	{
			if (change || now_state.resource_list[i].texture.lock() != last_state.resource_list[i].texture.lock())	{
				bool enable = now_state.resource_list[i].texture.lock() ? true : false;
				int use_count = (int)now_state.resource_list[i].texture.use_count();
				lv.set_cell_text(i, 2, tostr(enable) + _T("(") + tostr(use_count) + _T(")"));
			}
		}
		else if (now_state.resource_list[i].type == E_tnm_d3d_resource_type_album_from_g00)	{
			if (change || now_state.resource_list[i].album.lock() != last_state.resource_list[i].album.lock())	{
				bool enable = now_state.resource_list[i].album.lock() ? true : false;
				int use_count = (int)now_state.resource_list[i].album.use_count();
				lv.set_cell_text(i, 2, tostr(enable) + _T("(") + tostr(use_count) + _T(")"));
			}
		}
		else if (now_state.resource_list[i].type == E_tnm_d3d_resource_type_depth_stencil_buffer)	{
			if (change || now_state.resource_list[i].surface.lock() != last_state.resource_list[i].surface.lock())	{
				bool enable = now_state.resource_list[i].surface.lock() ? true : false;
				int use_count = (int)now_state.resource_list[i].surface.use_count();
				lv.set_cell_text(i, 2, tostr(enable) + _T("(") + tostr(use_count) + _T(")"));
			}
		}

		if (change || now_state.resource_list[i].name != last_state.resource_list[i].name)	{
			lv.set_cell_text(i, 3, now_state.resource_list[i].name);
		}
		if (change || now_state.resource_list[i].moji != last_state.resource_list[i].moji)	{
			lv.set_cell_text(i, 4, tostr_moji(now_state.resource_list[i].moji));
		}
		if (change || now_state.resource_list[i].color != last_state.resource_list[i].color)	{
			lv.set_cell_text(i, 5, tostr(now_state.resource_list[i].color));
		}
		if (change || now_state.resource_list[i].texture_cnt != last_state.resource_list[i].texture_cnt)	{
			lv.set_cell_text(i, 6, tostr(now_state.resource_list[i].texture_cnt));
		}
		if (change || now_state.resource_list[i].size != last_state.resource_list[i].size)	{
			lv.set_cell_text(i, 7, tostr(now_state.resource_list[i].size));
		}
		if (change || now_state.resource_list[i].size_ex != last_state.resource_list[i].size_ex)	{
			lv.set_cell_text(i, 8, tostr(now_state.resource_list[i].size_ex));
		}
		if (change || now_state.resource_list[i].mip_level != last_state.resource_list[i].mip_level)	{
			lv.set_cell_text(i, 9, tostr(now_state.resource_list[i].mip_level));
		}
		if (change || now_state.resource_list[i].usage != last_state.resource_list[i].usage)	{
			lv.set_cell_text(i, 10, get_str_D3DUSAGE(now_state.resource_list[i].usage));
		}
		if (change || now_state.resource_list[i].format != last_state.resource_list[i].format)	{
			lv.set_cell_text(i, 11, get_str_D3DFORMAT(now_state.resource_list[i].format));
		}
		if (change || now_state.resource_list[i].pool != last_state.resource_list[i].pool)	{
			lv.set_cell_text(i, 12, get_str_D3DPOOL(now_state.resource_list[i].pool));
		}
	}

	// �f�[�^���L��
	last_state = now_state;

	// ����t���O�����낷
	frame_first_flag = false;
}
