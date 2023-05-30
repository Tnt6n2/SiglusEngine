#pragma		once

// ****************************************************************
// Direct3D リソースマネージャ
// ================================================================
//		Direct3D の管理下に置くリソースは、全てここから作ってください。
//		デバイス消失時の復元などに使用できます。
// ================================================================

enum E_tnm_d3d_resource_type
{
	E_tnm_d3d_resource_type_none,
	E_tnm_d3d_resource_type_texture,
	E_tnm_d3d_resource_type_texture_from_file,
	E_tnm_d3d_resource_type_texture_from_g00,
	E_tnm_d3d_resource_type_texture_from_moji,
	E_tnm_d3d_resource_type_texture_from_mwnd,
	E_tnm_d3d_resource_type_album_from_g00,
	E_tnm_d3d_resource_type_depth_stencil_buffer,
	E_tnm_d3d_resource_type_mesh_from_x,
	E_tnm_d3d_resource_type_frame_from_x,
};

const TCHAR* const G_d3d_resource_type_define[] = 
{
	_T("???"),
	_T("ﾃｸｽﾁｬ"),
	_T("ﾃｸｽﾁｬ(ﾌｧｲﾙ)"),
	_T("ﾃｸｽﾁｬ(g00)"),
	_T("ﾃｸｽﾁｬ(文字)"),
	_T("ﾃｸｽﾁｬ(mwnd)"),
	_T("ｱﾙﾊﾞﾑ(g00)"),
	_T("深度ｽﾃﾝｼﾙﾊﾞｯﾌｧ"),
	_T("ﾒｯｼｭ(x)"),
	_T("ﾌﾚｰﾑ(x)"),
};

// ****************************************************************
// Direct3D リソース
// ================================================================
struct S_tnm_d3d_resource
{
	S_tnm_d3d_resource();

	int			type;
	int			id;
	C_size		size;
	C_size		size_ex;
	UINT		mip_level;
	DWORD		usage;
	D3DFORMAT	format;
	D3DPOOL		pool;
	TSTR		name;			// 識別名（ファイル名など）
	int			texture_cnt;	// テクスチャ数（アルバムの場合）
	TCHAR		moji;			// 文字
	C_argb		color;			// 文字の色
	int			extend_type;	// ＭＷの拡張タイプ
	DWORD		FVF;			// メッシュのＦＶＦ
	bool		resume;			// 復元フラグ

	BWP<C_d3d_texture>		texture;
	BWP<C_d3d_surface>		surface;
	BWP<C_d3d_album>		album;
	BWP<C_d3d_mesh>			mesh;
	BWP<C_d3d_frame_set>	frame_set;
};

// ****************************************************************
// Direct3D リソース：管理データ
// ================================================================
struct S_tnm_d3d_resource_data
{
	ARRAY<S_tnm_d3d_resource>	resource_list;
};

// ****************************************************************
// Direct3D リソースマネージャ
// ================================================================
class C_tnm_d3d_resource_manager
{
public:
	C_tnm_d3d_resource_manager();

	void	clear();
	void	organize();			// 整理する（１秒に１回くらい呼んであげてください）
	void	release();			// リソースの解放（device->Reset() の前に呼び出す）
	bool	resume();			// リソースの再構築（device->Reset() が成功したら呼び出す）

	const S_tnm_d3d_resource_data&	pd()	{	return m_pd;	};	// public data を取得する（情報ウィンドウに表示したりするのに使用）

	BSP<C_d3d_texture>		create_texture(CTSTR& name, int width, int height, UINT mip_level, DWORD usage, D3DFORMAT format, D3DPOOL pool, bool resume);
	BSP<C_d3d_texture>		create_texture_from_file(CTSTR& file_path, bool resume, bool reload);
	BSP<C_d3d_texture>		create_texture_from_moji(TCHAR moji, LOGFONT* lf, C_argb moji_color, bool resume);
	BSP<C_d3d_texture>		create_texture_from_mwnd(CTSTR& file_path, int extend_type, C_size window_size, bool resume);
	BSP<C_d3d_album>		create_album_from_g00(CTSTR& file_path, bool fuchidori, bool resume);
	BSP<C_d3d_surface>		create_dpst_buffer(CTSTR& name, int width, int height, bool resume);
	BSP<C_d3d_mesh>			create_mesh_from_x(CTSTR& path, DWORD FVF, bool resume);
	BSP<C_d3d_frame_set>	create_frame_set_from_x(CTSTR& path, bool resume);

private:
	S_tnm_d3d_resource_data	m_pd;			// public data
	int						m_next_id;
};


