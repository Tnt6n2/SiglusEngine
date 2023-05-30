#pragma		once

// ****************************************************************
// Direct3D ���\�[�X�}�l�[�W��
// ================================================================
//		Direct3D �̊Ǘ����ɒu�����\�[�X�́A�S�Ă����������Ă��������B
//		�f�o�C�X�������̕����ȂǂɎg�p�ł��܂��B
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
	_T("ø���"),
	_T("ø���(̧��)"),
	_T("ø���(g00)"),
	_T("ø���(����)"),
	_T("ø���(mwnd)"),
	_T("�����(g00)"),
	_T("�[�x��ݼ��ޯ̧"),
	_T("ү��(x)"),
	_T("�ڰ�(x)"),
};

// ****************************************************************
// Direct3D ���\�[�X
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
	TSTR		name;			// ���ʖ��i�t�@�C�����Ȃǁj
	int			texture_cnt;	// �e�N�X�`�����i�A���o���̏ꍇ�j
	TCHAR		moji;			// ����
	C_argb		color;			// �����̐F
	int			extend_type;	// �l�v�̊g���^�C�v
	DWORD		FVF;			// ���b�V���̂e�u�e
	bool		resume;			// �����t���O

	BWP<C_d3d_texture>		texture;
	BWP<C_d3d_surface>		surface;
	BWP<C_d3d_album>		album;
	BWP<C_d3d_mesh>			mesh;
	BWP<C_d3d_frame_set>	frame_set;
};

// ****************************************************************
// Direct3D ���\�[�X�F�Ǘ��f�[�^
// ================================================================
struct S_tnm_d3d_resource_data
{
	ARRAY<S_tnm_d3d_resource>	resource_list;
};

// ****************************************************************
// Direct3D ���\�[�X�}�l�[�W��
// ================================================================
class C_tnm_d3d_resource_manager
{
public:
	C_tnm_d3d_resource_manager();

	void	clear();
	void	organize();			// ��������i�P�b�ɂP�񂭂炢�Ă�ł����Ă��������j
	void	release();			// ���\�[�X�̉���idevice->Reset() �̑O�ɌĂяo���j
	bool	resume();			// ���\�[�X�̍č\�z�idevice->Reset() ������������Ăяo���j

	const S_tnm_d3d_resource_data&	pd()	{	return m_pd;	};	// public data ���擾����i���E�B���h�E�ɕ\�������肷��̂Ɏg�p�j

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


