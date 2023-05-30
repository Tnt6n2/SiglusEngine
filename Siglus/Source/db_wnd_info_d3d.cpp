#include	"pch.h"
#include	"ifc_eng.h"
#include	"dialog/db_wnd_info_d3d.h"
#include	"data/tnm_global_data.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// デバッグ情報ウィンドウ：Direct3D
// ================================================================

#define		REGIST_WINDOW_NAME_D3D_DEVICE		_T("db_wnd_info_d3d_device")
#define		REGIST_WINDOW_NAME_D3D_RESOURCE		_T("db_wnd_info_d3d_resource")

// ****************************************************************
// デバッグ情報ウィンドウ：Direct3D - デバイス
// ================================================================

// 初期化
void C_db_wnd_info_d3d_device::init()
{
	regist(REGIST_WINDOW_NAME_D3D_DEVICE, false, true, -1);

	// キャプション名登録
	regist_caption_name(_T("Direct3D デバイス情報"));

	// リストビュー項目
	listview_add_column(_T("state"), _T("ステート"), 100);
	listview_add_column(_T("value"), _T("値"), 100);
}

// 初期化ファンクション
void C_db_wnd_info_d3d_device::on_init_func()
{
	// 項目を追加
	lv.add_item(0, _T("ビデオメモリ"));
	lv.add_item(0, _T("ビデオメモリ使用量"));
	lv.add_item(0, _T("ディスプレイアダプタ"));
	lv.add_item(0, _T("デバイスタイプ"));
	lv.add_item(0, _T("頂点処理方法"));
	lv.add_item(0, _T("頂点シェーダのバージョン"));
	lv.add_item(0, _T("頂点シェーダの定数レジスタ数"));
	lv.add_item(0, _T("ピクセルシェーダのバージョン"));
	lv.add_item(0, _T("最大テクスチャサイズ"));
	lv.add_item(0, _T("2の累乗以外のテクスチャ"));
	lv.add_item(0, _T("ﾃｸｽﾁｬｱﾄﾞﾚｯｼﾝｸﾞ BORDER"));
	lv.add_item(0, _T("ﾐｯﾌﾟﾏｯﾌﾟの自動生成"));
	lv.add_item(0, _T("ﾐｯﾌﾟﾏｯﾌﾟの自動生成(X8R8G8B8 ﾃｸｽﾁｬ)"));
	lv.add_item(0, _T("ﾐｯﾌﾟﾏｯﾌﾟの自動生成(A8R8G8B8 ﾃｸｽﾁｬ)"));
	lv.add_item(0, _T("ﾌｨﾙﾀｰの検索(X8R8G8B8 ﾃｸｽﾁｬ)"));
	lv.add_item(0, _T("ﾌｨﾙﾀｰの検索(A8R8G8B8 ﾃｸｽﾁｬ)"));

	// デバイスの情報を取得
	const C_d3d_device_state& state = G_d3d.device.state();

	// 初期値を設定
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

// 閉じるファンクション
void C_db_wnd_info_d3d_device::on_close_func()
{
}

// ****************************************************************
// フレーム
// ================================================================
void C_db_wnd_info_d3d_device::frame()
{
	if (!m_h_wnd)
		return;

	// 変化する値を設定
	{
		STATE cd;

		cd.video_memory_use_size = Gp_global->video_memory_use_size;
		cd.video_memory_use_max_size = Gp_global->video_memory_use_max_size;

		if (frame_first_flag || cd.video_memory_use_size != m_ld.video_memory_use_size || cd.video_memory_use_max_size != m_ld.video_memory_use_max_size)
			lv.set_cell_text(1, 1, tostr(cd.video_memory_use_size) + _T("MB / ") + tostr(cd.video_memory_use_max_size) + _T("MB"));

		// データを記憶
		m_ld = cd;
	}

	// 初回フラグをおろす
	frame_first_flag = false;
}

// ****************************************************************
// デバッグ情報ウィンドウ：Direct3D - リソース
// ================================================================

// 初期化
void C_db_wnd_info_d3d_resource::init()
{
	regist(REGIST_WINDOW_NAME_D3D_RESOURCE, false, true, -1);

	// キャプション名登録
	regist_caption_name(_T("Direct3D リソース情報"));

	// リストビュー項目
	listview_add_column(_T("id"), _T("ID"), 50);
	listview_add_column(_T("type"), _T("ﾀｲﾌﾟ"), 50);
	listview_add_column(_T("enable"), _T("有効"), 50);
	listview_add_column(_T("name"), _T("名前"), 50);
	listview_add_column(_T("moji"), _T("文字"), 50);
	listview_add_column(_T("color"), _T("色"), 50);
	listview_add_column(_T("texture_cnt"), _T("ﾃｸｽﾁｬ数"), 50);
	listview_add_column(_T("size"), _T("ｻｲｽﾞ"), 50);
	listview_add_column(_T("size_ex"), _T("ｻｲｽﾞex"), 50);
	listview_add_column(_T("mip_level"), _T("ﾐｯﾌﾟﾚﾍﾞﾙ"), 50);
	listview_add_column(_T("usage"), _T("使い方"), 50);
	listview_add_column(_T("format"), _T("ﾌｫｰﾏｯﾄ"), 50);
	listview_add_column(_T("pool"), _T("管理"), 50);
}

// 初期化ファンクション
void C_db_wnd_info_d3d_resource::on_init_func()
{
}

// 閉じるファンクション
void C_db_wnd_info_d3d_resource::on_close_func()
{
}

// ****************************************************************
// フレーム
// ================================================================
void C_db_wnd_info_d3d_resource::frame()
{
	if (!m_h_wnd)
		return;

	STATE now_state;

	// データの更新
	now_state.resource_list = G_rsm.pd().resource_list;

	// アイテム数の更新
	if (frame_first_flag || now_state.resource_list.size() != last_state.resource_list.size())	{
		lv.set_item_cnt((int)now_state.resource_list.size());
	}

	// データが更新されていればリストビューを更新
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

	// データを記憶
	last_state = now_state;

	// 初回フラグをおろす
	frame_first_flag = false;
}
