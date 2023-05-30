#pragma		once

// ****************************************************************
// �����}�l�[�W��
// ================================================================
class	C_tnm_moji_manager;
extern	C_tnm_moji_manager	G_moji_manager;

// ****************************************************************
// �����}�l�[�W��
// ================================================================
//		�悭�g�������i�Ђ炪�ȂȂǁj�̃e�N�X�`����
//		�L�����Ă������ƂŁA��������}��܂��B
// ================================================================
class C_tnm_moji_manager
{
public:

	// �R���X�g���N�^
	C_tnm_moji_manager()
	{
		moji_map_max_cnt = 0;
		moji_memory_now_size = 0;
		moji_memory_max_size = 0;
		moji_load_counter = 0;
	}

	// �����e�N�X�`�������[�h����
	BSP<C_d3d_texture>	load_moji(int moji_type, int moji, int moji_size, C_argb moji_color, MOJI_FACE_TYPE moji_face_type, bool tategaki, int buf_no);

	void		clear();								// �N���A����
	int			get_moji_map_use_cnt();					// �g�p���̕����}�b�v�����擾����
	int			get_moji_map_now_cnt();					// ���݂̕����}�b�v�����擾����
	int			get_moji_map_max_cnt();					// �ő�̕����}�b�v�����擾����

	__int64		get_moji_memory_use_size();				// �g�p���̕����������[�T�C�Y���擾����
	__int64		get_moji_memory_now_size();				// ���݂̕����������[�T�C�Y���擾����
	__int64		get_moji_memory_max_size();				// �ő�̕����������[�T�C�Y���擾����

	int		get_moji_kind_useful_cnt();		// �L�v�ȕ��������擾����
	int		get_moji_kind_unuseful_cnt();	// ��L�v�ȕ��������擾����

private:

	// �����e�N�X�`�����쐬����
	BSP<C_d3d_texture>	create_moji(int moji_type, int moji, int moji_size, C_argb moji_color, MOJI_FACE_TYPE moji_face_type, bool tategaki);

	struct S_moji
	{
		int				moji_type;
		int				moji;
		int				size;
		C_argb			color;
		MOJI_FACE_TYPE	face_type;

		bool operator < (const S_moji& rhs) const
		{
			if (moji_type < rhs.moji_type)	return true;
			if (moji_type > rhs.moji_type)	return false;
			if (moji < rhs.moji)			return true;
			if (moji > rhs.moji)			return false;
			if (size < rhs.size)			return true;
			if (size > rhs.size)			return false;
			if (color < rhs.color)			return true;
			if (color > rhs.color)			return false;
			if (face_type < rhs.face_type)	return true;
			return false;
			//return moji < rhs.moji || (moji == rhs.moji && (size < rhs.size || (size == rhs.size && (color < rhs.color || (color == rhs.color && type < rhs.type)))));
		}
	};
	struct S_texture
	{
		BSP<C_d3d_texture>	texture;
		int					moji_size;
		bool				useful;
		int					load_counter;
		int					texture_memory;
	};
	typedef		std::pair<S_moji, S_texture >		MOJI_PAIR;
	typedef		std::map<S_moji, S_texture >		MOJI_MAP;
	MOJI_MAP	moji_map;

	int		moji_map_max_cnt;			// �����}�b�v�̍ő吔
	int		moji_load_counter;			// �����̃��[�h�J�E���^�[

	__int64		moji_memory_now_size;	// ���݂̕����������[�T�C�Y
	__int64		moji_memory_max_size;	// �ő�̕����������[�T�C�Y

	int		moji_kind_useful_cnt;		// �L�v�ȕ�����
	int		moji_kind_unuseful_cnt;		// ��L�v�ȕ�����
};


