#pragma		once

// ****************************************************************
// �f�`�m�f�[�^
// ================================================================

// �p�^�[���f�[�^
struct S_gan_pat_data
{
	int		pat_no;
	int		x;
	int		y;
	int		z;
	BYTE	tr;
	int		wait;
	int		keika_time;		// �p�^�[���̐擪����̌o�ߎ���

	// �R���X�g���N�^
	S_gan_pat_data()
	{
		pat_no = 0;
		x = 0;
		y = 0;
		z = 0;
		tr = 255;
		wait = 0;
		keika_time = 0;
	}
};

// �Z�b�g�f�[�^
struct S_gan_set_data
{
	ARRAY<S_gan_pat_data>	pat_list;
	int						total_time;

	// �R���X�g���N�^
	S_gan_set_data()
	{
		pat_list.clear();
		total_time = 0;
	}
};

// �{��
class C_gan_data
{

public:

	TSTR					g00_file_name;			// �G�f�B�^�[�p�̃t�@�C����
	ARRAY<S_gan_set_data>	set_list;				// �Z�b�g���X�g

	bool			load(CTSTR& file_path);			// �ǂݍ���

private:

	bool			analize(ARRAY<BYTE>& buf);		// ���
	bool			analize_func(BYTE* sp);			// ��̓t�@���N�V����
	bool			analize_func_set(BYTE* sp, BYTE** ret_sp, S_gan_set_data* set, int* keika_time);	// �Z�b�g���
	bool			analize_func_pat(BYTE* sp, BYTE** ret_sp, S_gan_pat_data* pat, int* keika_time);	// �p�^�[�����
};

