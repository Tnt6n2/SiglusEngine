#include	"pch.h"
#include	"data/tnm_rsc_manager.h"
#include	"engine/ifc_error.h"

#include	<boost/algorithm/string.hpp>

// ****************************************************************
// Direct3D リソース：リソースデータ：コンストラクタ
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
// Direct3D リソースマネージャ：コンストラクタ
// ================================================================
C_tnm_d3d_resource_manager::C_tnm_d3d_resource_manager()
{
	m_next_id = 0;
}

// ****************************************************************
// Direct3D リソースマネージャ：リソースデータをクリアする
// ================================================================
void C_tnm_d3d_resource_manager::clear()
{
	m_pd.resource_list.clear();
}

// ****************************************************************
// Direct3D リソースマネージャ：リソースデータを整理する
// ================================================================
void C_tnm_d3d_resource_manager::organize()
{
	for (ARRAY<S_tnm_d3d_resource>::iterator itr = m_pd.resource_list.begin(); itr != m_pd.resource_list.end();)	{
		bool erase_flag = false;

		if (false);
		// テクスチャの場合
		else if (itr->type == E_tnm_d3d_resource_type_texture || itr->type == E_tnm_d3d_resource_type_texture_from_file || itr->type == E_tnm_d3d_resource_type_texture_from_g00 || itr->type == E_tnm_d3d_resource_type_texture_from_moji || itr->type == E_tnm_d3d_resource_type_texture_from_mwnd)	{
			BSP<C_d3d_texture> texture = itr->texture.lock();
			if (!texture)	{
				itr = m_pd.resource_list.erase(itr);
				erase_flag = true;
			}
		}
		// アルバムの場合
		else if (itr->type == E_tnm_d3d_resource_type_album_from_g00)	{
			BSP<C_d3d_album> album = itr->album.lock();
			if (!album)	{
				itr = m_pd.resource_list.erase(itr);
				erase_flag = true;
			}
		}
		// 深度ステンシルバッファの場合
		else if (itr->type == E_tnm_d3d_resource_type_depth_stencil_buffer)	{
			BSP<C_d3d_surface> surface = itr->surface.lock();
			if (!surface)	{
				itr = m_pd.resource_list.erase(itr);
				erase_flag = true;
			}
		}
		// メッシュの場合
		else if (itr->type == E_tnm_d3d_resource_type_mesh_from_x)	{
			BSP<C_d3d_mesh> mesh = itr->mesh.lock();
			if (!mesh)	{
				itr = m_pd.resource_list.erase(itr);
				erase_flag = true;
			}
		}
		// フレームの場合
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
// Direct3D リソースマネージャ：リソース一時解放
// ================================================================
//		リソースのリストは維持します。
//		device->Reset() の前に必ず呼んでください。
//		resume() を呼び出すことで、リソースを復元することができます。
// ================================================================
void C_tnm_d3d_resource_manager::release()
{
	// リソースを一時解放
	for (ARRAY<S_tnm_d3d_resource>::iterator itr = m_pd.resource_list.begin(); itr != m_pd.resource_list.end(); ++itr)	{
		if (itr->resume)	{

			if (false);
			// テクスチャの場合
			else if (itr->type == E_tnm_d3d_resource_type_texture || itr->type == E_tnm_d3d_resource_type_texture_from_file || itr->type == E_tnm_d3d_resource_type_texture_from_g00 || itr->type == E_tnm_d3d_resource_type_texture_from_moji)	{
				BSP<C_d3d_texture> texture = itr->texture.lock();
				if (texture)
					texture->release();
			}
			// アルバムの場合
			else if (itr->type == E_tnm_d3d_resource_type_album_from_g00)	{
				for (int i = 0; i < (int)itr->album.lock()->get_texture_cnt(); i++)	{
					BSP<C_d3d_texture> texture = itr->album.lock()->get_texture(i);
					if (texture)
						texture->release();
				}
			}
			// 深度ステンシルバッファの場合
			else if (itr->type == E_tnm_d3d_resource_type_depth_stencil_buffer)	{
				BSP<C_d3d_surface> surface = itr->surface.lock();
				if (surface)
					surface->release();
			}
		}
	}
}

// ****************************************************************
// Direct3D リソースマネージャ：リソースを復元
// ================================================================
//		device->Reset() が成功したら呼んでください。
//		release() で解放したリソースを復元することができます。
// ================================================================
bool C_tnm_d3d_resource_manager::resume()
{
	// テクスチャ
	for (ARRAY<S_tnm_d3d_resource>::iterator itr = m_pd.resource_list.begin(); itr != m_pd.resource_list.end(); ++itr)	{
		if (itr->resume)	{

			if (false);

			// テクスチャの場合
			else if (itr->type == E_tnm_d3d_resource_type_texture || itr->type == E_tnm_d3d_resource_type_texture_from_file || itr->type == E_tnm_d3d_resource_type_texture_from_g00 || itr->type == E_tnm_d3d_resource_type_texture_from_moji)	{
				BSP<C_d3d_texture> texture = itr->texture.lock();
				if (texture)	{

					if (false);
					// テクスチャを作成する
					else if (itr->type == E_tnm_d3d_resource_type_texture)	{
						if (!texture->create(itr->size.cx, itr->size.cy, itr->mip_level, itr->usage, itr->format, itr->pool))
							return false;

						// 透明に塗りつぶす
						if (itr->pool == D3DPOOL_DEFAULT)	{
							
							// D3DPOOL_DEFAULT の場合はロックできないので UpdateTexture で更新する
//							texture->clear_color_ex(C_argb(0, 0, 0, 0));
						}
						else	{

							// D3DPOOL_DEFAULT でない場合はロックできるので普通に塗りつぶす
							texture->clear_color_ex(C_argb(0, 0, 0, 0));
						}

						// dirty フラグを立てとくよ！これで更新が期待できる！
						texture->set_dirty_flag(true);
					}
					// ファイルを読み込む
					else if (itr->type == E_tnm_d3d_resource_type_texture_from_file)	{
						if (!texture->load_file(itr->name))
							return false;
					}
					// g00 を読み込む
					else if (itr->type == E_tnm_d3d_resource_type_texture_from_g00)	{
						if (!texture->load_g00(itr->name))
							return false;
					}
				}
			}
			// アルバムの場合
			else if (itr->type == E_tnm_d3d_resource_type_album_from_g00)	{
				BSP<C_d3d_album> album = itr->album.lock();
				if (album)	{
					if (!album->load_g00(itr->name, false))
						return false;
				}
			}
			// 深度ステンシルバッファの場合
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
// Direct3D リソースマネージャ：テクスチャを作成
// ================================================================
BSP<C_d3d_texture> C_tnm_d3d_resource_manager::create_texture(CTSTR& name, int width, int height, UINT mip_level, DWORD usage, D3DFORMAT format, D3DPOOL pool, bool resume)
{
	BSP<C_d3d_texture> texture(new C_d3d_texture);
	if (!texture->create(width, height, mip_level, usage, format, pool))
		return BSP<C_d3d_texture>();

	// リソースを登録
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
// Direct3D リソースマネージャ：テクスチャを作成（ファイルを読み込む）
// ================================================================
BSP<C_d3d_texture> C_tnm_d3d_resource_manager::create_texture_from_file(CTSTR& file_path, bool resume, bool reload)
{
	// 再読み込み強制フラグが立っていない場合は、既に読み込まれているリソースを探す
	if (!reload)	{

		// 同じリソースを探し出す
		for (int i = 0; i < (int)m_pd.resource_list.size(); i++)	{
			if (m_pd.resource_list[i].type == E_tnm_d3d_resource_type_texture_from_file && m_pd.resource_list[i].name == file_path)	{
				// リソースが生きている必要があります！
				if (m_pd.resource_list[i].texture.lock())	{
					// リソースが見つかったのでそれを返す
					return m_pd.resource_list[i].texture.lock();
				}
			}
		}
	}

	// リソースが見つからなかったので新規作成
	BSP<C_d3d_texture> texture(new C_d3d_texture);
	if (!texture->load_file(file_path))
		return BSP<C_d3d_texture>();		// 読み込みに失敗…。
	
	// 新規にリソースを登録
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
// Direct3D リソースマネージャ：テクスチャを作成（mwnd を読み込む）
// ================================================================

//		矩形コピールーチンです。
//		ライブラリとしてまとめるべきかも。
//		src_pitch, dst_pitch だけはピクセル数ではなくバイト数なので注意。
void block_copy(BYTE* src, int src_pitch, int src_org_w, int src_org_h, int src_x, int src_y, int src_w, int src_h, BYTE* dst, int dst_pitch, int dst_org_w, int dst_org_h, int dst_x, int dst_y)
{
	// 例外処理
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

	// 同じリソースを探し出す
	for (int i = 0; i < (int)m_pd.resource_list.size(); i++)	{
		if (m_pd.resource_list[i].type == E_tnm_d3d_resource_type_texture_from_mwnd && m_pd.resource_list[i].name == file_path &&  m_pd.resource_list[i].size == window_size)	{
			// リソースが生きている必要があります！
			if (m_pd.resource_list[i].texture.lock())	{
				// リソースが見つかったのでそれを返す
				return m_pd.resource_list[i].texture.lock();
			}
		}
	}

	// リソースが見つからなかったので新規作成
	BSP<C_d3d_album> org_pct(new C_d3d_album);
	if (!org_pct->load_g00(file_path, false))	{
		return BSP<C_d3d_texture>();		// 読み込みに失敗…。
	}

	// カット数を判定
	if (org_pct->get_texture_cnt() < 12)	{
		set_last_error(_T("可変枠として読み込みましたが、カット数が足りませんでした。"), _T(""));
		return BSP<C_d3d_texture>();		// 読み込みに失敗…。
	}

	// 切り出しデータの大きさを取得
	C_size cs[12];
	for (int i = 0; i < 12; i++)
		cs[i] = org_pct->get_texture(i)->get_size();

	// ======== //
	// ０ １ ２ //
	// ３    ６ //
	// ４    ７ //
	// ５    ８ //
	// ９ 10 11 //
	// -------- //

	// 可変パーツを並べる数を計算する
	int extend_x_cnt = (window_size.cx - (cs[0].cx + cs[2].cx) + (cs[1].cx - 1)) / cs[1].cx;
	int extend_y_cnt = (window_size.cy - (cs[0].cy + cs[3].cy + cs[5].cy + cs[9].cy) + (cs[4].cy - 1)) / cs[4].cy;

	// ウィンドウサイズを切り出しデータにそろうように整える
	window_size.cx = extend_x_cnt * cs[1].cx + (cs[0].cx + cs[2].cx);
	window_size.cy = extend_y_cnt * cs[4].cy + (cs[0].cy + cs[3].cy + cs[5].cy + cs[9].cy);

	// カットからウィンドウデータを作成する！

	// テクスチャを作成する
	BSP<C_d3d_texture> mwnd_texture(new C_d3d_texture);
	mwnd_texture->create(window_size.cx, window_size.cy, 0, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED);
	if (!mwnd_texture)
		return BSP<C_d3d_texture>();		// 読み込みに失敗…。

	// テクスチャをロック
	D3DLOCKED_RECT locked_rect_dst;
	if (!mwnd_texture->lock_rect(0, &locked_rect_dst, NULL, 0))
		return BSP<C_d3d_texture>();		// 読み込みに失敗…。

	// １２パーツを展開していく
	for (int i = 0; i < 12; i++)	{

		D3DLOCKED_RECT locked_rect_src;
		if (!org_pct->get_texture(i)->lock_rect(0, &locked_rect_src, NULL, 0))
			return BSP<C_d3d_texture>();		// 読み込みに失敗…。

		// わかりやすく変数を整理します
		BYTE* src_org = (BYTE *)locked_rect_src.pBits;
		BYTE* dst_org = (BYTE *)locked_rect_dst.pBits;
		int src_pitch = locked_rect_src.Pitch;
		int dst_pitch = locked_rect_dst.Pitch;
		int width = cs[i].cx;
		int height = cs[i].cy;

		if (false);

		else if (i == 0)	{	// ０番
			block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, 0, 0);
		}
		else if (i == 1)	{	// １番
			for (int e = 0; e < extend_x_cnt; e++)	{
				block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, cs[0].cx + cs[1].cx * e, 0);
			}
		}
		else if (i == 2)	{	// ２番
			block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, cs[0].cx + cs[1].cx * extend_x_cnt, 0);
		}
		else if (i == 3)	{	// ３番
			block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, 0, cs[0].cy);
		}
		else if (i == 4)	{	// ４番
			for (int e = 0; e < extend_y_cnt; e++)	{
				block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, 0, cs[0].cy + cs[3].cy + cs[4].cy * e);
			}
		}
		else if (i == 5)	{	// ５番
			block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, 0, cs[0].cy + cs[3].cy + cs[4].cy * extend_y_cnt);
		}
		else if (i == 6)	{	// ６番
			block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, window_size.cx - cs[6].cx, cs[0].cy);
		}
		else if (i == 7)	{	// ７番
			for (int e = 0; e < extend_y_cnt; e++)	{
				block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, window_size.cx - cs[7].cx, cs[0].cy + cs[3].cy + cs[4].cy * e);
			}
		}
		else if (i == 8)	{	// ８番
			block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, window_size.cx - cs[8].cx, cs[0].cy + cs[3].cy + cs[4].cy * extend_y_cnt);
		}
		else if (i == 9)	{	// ９番
			block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, 0, window_size.cy - cs[9].cy);
		}
		else if (i == 10)	{	// １０番
			for (int e = 0; e < extend_x_cnt; e++)	{
				block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, cs[0].cx + cs[1].cx * e, window_size.cy - cs[10].cy);
			}
		}
		else if (i == 11)	{	// １１番
			block_copy(src_org, src_pitch, width, height, 0, 0, width, height, dst_org, dst_pitch, window_size.cx, window_size.cy, cs[0].cx + cs[1].cx * extend_x_cnt, window_size.cy - cs[11].cy);
		}

		if (!org_pct->get_texture(i)->unlock_rect(0))
			return BSP<C_d3d_texture>();		// 読み込みに失敗…。
	}

	if (!mwnd_texture->unlock_rect(0))
		return BSP<C_d3d_texture>();		// 読み込みに失敗…。
	
	// 新規にリソースを登録
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
// Direct3D リソースマネージャ：テクスチャを作成（文字を読み込む）
// ================================================================
BSP<C_d3d_texture> C_tnm_d3d_resource_manager::create_texture_from_moji(TCHAR moji, LOGFONT* lf, C_argb moji_color, bool resume)
{
	// 同じリソースを探し出す
	for (int i = 0; i < (int)m_pd.resource_list.size(); i++)	{
		if (m_pd.resource_list[i].type == E_tnm_d3d_resource_type_texture_from_moji && m_pd.resource_list[i].moji == moji && m_pd.resource_list[i].color == moji_color)	{
			// リソースが生きている必要があります！
			if (m_pd.resource_list[i].texture.lock())	{
				// リソースが見つかったのでそれを返す
				return m_pd.resource_list[i].texture.lock();
			}
		}
	}

	// リソースが見つからなかったので新規作成
	BSP<C_d3d_texture> texture(new C_d3d_texture);
	if (!texture->load_moji(moji, lf, moji_color))
		return BSP<C_d3d_texture>();		// 読み込みに失敗…。
	
	// 新規にリソースを登録
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
// Direct3D リソースマネージャ：アルバムを作成（ファイルを読み込む）
// ================================================================
BSP<C_d3d_album> C_tnm_d3d_resource_manager::create_album_from_g00(CTSTR& file_path, bool fuchidori, bool resume)
{
	// 同じリソースを探し出す
	for (int i = 0; i < (int)m_pd.resource_list.size(); i++)	{
		if (m_pd.resource_list[i].type == E_tnm_d3d_resource_type_album_from_g00 && m_pd.resource_list[i].name == file_path)	{
			// リソースが生きている必要があります！
			if (m_pd.resource_list[i].album.lock())	{
				// リソースが見つかったのでそれを返す
				return m_pd.resource_list[i].album.lock();
			}
		}
	}

	// リソースが見つからなかったので新規作成
	BSP<C_d3d_album> album(new C_d3d_album);
	{
		bool result = true;

		ARRAY<C_d3d_album_compose_param> param_list;

		// ファイルパスを '|' で分割する
		std::vector<TSTR> str_list;
		boost::split(str_list, file_path, boost::is_any_of(_T("|")));

		// それぞれを解析
		for (int i = 0; i < (int)str_list.size(); i++)
		{
			TSTR one_file_path = str_list[i];

			// ディレクトリとファイル名に分けます。
			// この処理は必須です。ディレクトリ中の () などが解析されるのを防ぐためです。
			TSTR dir_path = ::get_dir_path(one_file_path);
			TSTR file_name = ::get_file_name(one_file_path);

			// ファイル名をさらに解析
			C_d3d_album_compose_param param;
			for (TSTR::const_iterator f_itr = file_name.begin();;)
			{
				// 末尾に達しちゃった
				if (f_itr == file_name.end())
				{
					// ファイル名確定
					param.file_name = file_name;
					// フルパスを求める
					if (!param.file_name.empty())
					{
						param.file_path = dir_path + _T("\\") + file_name;
					}
					// 情報確定
					param_list.push_back(param);
					// 終了
					break;
				}
				// 括弧が見つかった
				else if (*f_itr == _T('('))
				{
					// ファイル名確定
					param.file_name = TSTR(TSTR::const_iterator(file_name.begin()), f_itr);
					// フルパスを求める
					if (!param.file_name.empty())
					{
						param.file_path = dir_path + _T("\\") + param.file_name;
					}
					// 次へ
					++f_itr;
					// 必須パラメータを読み込む
					result &= str_analize_get_integer(f_itr, file_name.end(), &param.x);
					result &= str_analize_check_moji(f_itr, file_name.end(), _T(','));
					result &= str_analize_get_integer(f_itr, file_name.end(), &param.y);
					// 追加パラメータを読み込む
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
						return BSP<C_d3d_album>();		// 解析に失敗…。
					}
					// 情報確定
					param_list.push_back(param);
					// 終了
					break;
				}
				else	{
					++f_itr;
				}
			}
		}

		// 読み込む
		if (param_list.empty())
		{
			return BSP<C_d3d_album>();		// 解析に失敗…。
		}
		else if (param_list.size() == 1)
		{
			// 単一 g00
			if (!album->load_g00(param_list[0].file_path, fuchidori))
				return BSP<C_d3d_album>();		// 読み込みに失敗…。
		}
		else
		{
			// 合成 g00
			if (!album->load_g00_composed(param_list))
				return BSP<C_d3d_album>();		// 読み込みに失敗…。
		}
	}

	// テクスチャの最大サイズを求める
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

	// 新規にリソースを登録
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
// Direct3D リソースマネージャ：テクスチャを作成（深度ステンシルバッファを作成）
// ================================================================
BSP<C_d3d_surface> C_tnm_d3d_resource_manager::create_dpst_buffer(CTSTR& name, int width, int height, bool resume)
{
	BSP<C_d3d_surface> surface(new C_d3d_surface);
	if (!surface->create_dpst_buffer(width, height))
		return BSP<C_d3d_surface>();

	// リソースを登録
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
// Direct3D リソースマネージャ：メッシュを作成（Ｘファイルを読み込む）
// ================================================================
BSP<C_d3d_mesh> C_tnm_d3d_resource_manager::create_mesh_from_x(CTSTR& file_path, DWORD FVF, bool resume)
{
	// 同じリソースを探し出す
	for (int i = 0; i < (int)m_pd.resource_list.size(); i++)	{
		if (m_pd.resource_list[i].type == E_tnm_d3d_resource_type_mesh_from_x && m_pd.resource_list[i].name == file_path)	{
			// リソースが生きている必要があります！
			if (m_pd.resource_list[i].mesh.lock())	{
				// リソースが見つかったのでそれを返す
				return m_pd.resource_list[i].mesh.lock();
			}
		}
	}

	// リソースが見つからなかったので新規作成
	BSP<C_d3d_mesh> mesh(new C_d3d_mesh);
	ARRAY<TSTR> texture_name_list;
	if (!mesh->load_x(file_path, FVF, texture_name_list))
		return BSP<C_d3d_mesh>();		// 読み込みに失敗…。

	// メッシュに必要なテクスチャを読み込む
	for (int i = 0; i < (int)texture_name_list.size(); i++)	{
		if (!texture_name_list[i].empty())	{
			BSP<C_d3d_texture> texture = create_texture_from_file(get_dir_path(file_path) + _T("\\") + texture_name_list[i], resume, false);
			if (!texture)
				return BSP<C_d3d_mesh>();		// 読み込みに失敗…。

			// メッシュにテクスチャを設定する
			mesh->set_texture(i, texture);
		}
	}

	// 新規にリソースを登録
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
// Direct3D リソースマネージャ：フレームを作成（Ｘファイルを読み込む）
// ================================================================
BSP<C_d3d_frame_set> C_tnm_d3d_resource_manager::create_frame_set_from_x(CTSTR& file_path, bool resume)
{
	// 同じリソースを探し出す
	for (int i = 0; i < (int)m_pd.resource_list.size(); i++)	{
		if (m_pd.resource_list[i].type == E_tnm_d3d_resource_type_frame_from_x && m_pd.resource_list[i].name == file_path)	{
			// リソースが生きている必要があります！
			if (m_pd.resource_list[i].frame_set.lock())	{
				// リソースが見つかったのでそれを返す
				return m_pd.resource_list[i].frame_set.lock();
			}
		}
	}

	// リソースが見つからなかったので新規作成
	ARRAY< BSP<C_d3d_mesh> > mesh_list;
	ARRAY<TSTR> texture_name_list;
	BSP<C_d3d_frame_set> frame_set(new C_d3d_frame_set);
	if (!frame_set->load_x(file_path, mesh_list, texture_name_list))
		return BSP<C_d3d_frame_set>();		// 読み込みに失敗…。

	// フレームに必要なテクスチャを読み込む
	int texture_no = 0;
	for (int m = 0; m < (int)mesh_list.size(); m++)	{
		for (int mtrl_no = 0; mtrl_no < mesh_list[m]->get_mtrl_cnt(); mtrl_no++)	{
			if (!texture_name_list[texture_no].empty())	{
				TSTR texture_path = get_dir_path(file_path) + _T("\\") + texture_name_list[texture_no];
				BSP<C_d3d_texture> texture = create_texture_from_file(texture_path, resume, false);
				if (!texture)
					return BSP<C_d3d_frame_set>();		// 読み込みに失敗…。

				// メッシュにテクスチャを設定する
				mesh_list[m]->set_texture(mtrl_no, texture);
			}

			// 次のテクスチャへ
			texture_no ++;
		}
	}

	// 新規にリソースを登録
	S_tnm_d3d_resource resource;
	resource.id = m_next_id++;
	resource.type = E_tnm_d3d_resource_type_frame_from_x;
	resource.frame_set = frame_set;
	resource.name = file_path;
	resource.resume = resume;
	m_pd.resource_list.push_back(resource);

	return frame_set;
}
