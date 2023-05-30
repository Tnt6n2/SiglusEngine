#pragma		once

// ���k�P��
#define		NWA_UNIT_SAMPLE_CNT			512

// �m�v�`�w�b�_
struct NWA_HEADER_STRUCT	{
	WORD	channels;				// �`�����l���i1=���� / 2=��ڵ�j
	WORD	bits_per_sample;		// �r�b�g�i8bit / 16bit�j
	DWORD	samples_per_sec;		// ���g���i11025 / 22050 / 44100�j

	int		pack_mod;				// ���k���[�h�i-1=�����k  0=��i��  1=���i��  2=���i��  3=��荂�i��  4=����荂�i��  5=������荂�i���j
	int		zero_mod;				// �������k���[�h�i0=�������Ȃ�  1=���������������k����j

	DWORD	unit_cnt;				// ���j�b�g��

	DWORD	original_size;			// �I���W�i���T�C�Y
	DWORD	pack_size;				// ���k��̃T�C�Y
	DWORD	sample_cnt;				// �T���v�����i�`�����l�����Ƃɐ����Ă��܂��j

	DWORD	unit_sample_cnt;		// ���k�P�ʂ̃T���v����
	DWORD	last_sample_cnt;		// �Ō�̈��k�̃T���v����
	DWORD	last_sample_pack_size;	// �Ō�̈��k�T�C�Y
};

// �m�v�`�p�̓W�J�p�̃e�[�u���f�[�^
struct NWA_TABLE_STRUCT		{
	int offset;
};

// ****************************************************************
// �m�v�`���k
// ================================================================
class Cnwa_pack
{

public:
				Cnwa_pack();						// �R���X�g���N�^

	bool		ready(BYTE* src_data_, DWORD src_data_size, WORD channels, WORD bits_per_sample, DWORD samples_per_sec, int pack_mod, int zero_mod, DWORD break_out_size_);		// ����
	bool		proc(BYTE* dst_buf_, DWORD dst_buf_size_);		// �v���Z�X

	DWORD		src_complate_size;					// ���������T�C�Y
	DWORD		dst_buf_need_size;					// ����Ȃ��o�b�t�@��
	DWORD		pack_size;							// ���k�T�C�Y

private:

	// �v���Z�X�֐�
	bool		_proc_start();						// �v���Z�X �X�^�[�g
	bool		_proc_copy();						// �v���Z�X �R�s�[
	bool		_proc_end();						// �v���Z�X �I������
	bool		_proc_check_dst_buf_size();			// �m�v�`���k �v���Z�X �o�͐�T�C�Y�𔻒�

	// ���k�֐�
	DWORD		_pack_func_pack(BYTE* src_p, DWORD src_size);	// ���k�֐� �{��

	void		_pack_func_pack16m();				// ���k�֐� �P�U�r�b�g�E���m����
	void		_pack_func_pack16s();				// ���k�֐� �P�U�r�b�g�E�X�e���I
	void		_pack_func_pack16(int *nowsmp);		// ���k�֐� �P�U�r�b�g

	void		_pack_func_set_bit(DWORD add_bit_cnt, DWORD set_bit_dat);	// ���k�֐� �r�b�g�t���O�ݒ�

	bool		_pack_func_zero_proc();				// ���k�֐� �y�d�q�n�v���Z�X
	bool		_pack_func_zero_check();			// ���k�֐� �y�d�q�n����
	void		_pack_func_zero_set();				// ���k�֐� �y�d�q�n�ݒ�
	void		_pack_func_set_bit_zero_one();		// ���k�֐� �y�d�q�n�t���O�P��ݒ�
	void		_pack_func_set_bit_zero();			// ���k�֐� �y�d�q�n�t���O��ݒ�

	void		_pack_func_dat_set0(DWORD sign);	// ���k�֐� �f�[�^�Z�b�g�O
	void		_pack_func_dat_set1(DWORD sign);	// ���k�֐� �f�[�^�Z�b�g�P
	void		_pack_func_dat_set2(DWORD sign);	// ���k�֐� �f�[�^�Z�b�g�Q
	void		_pack_func_dat_set3(DWORD sign);	// ���k�֐� �f�[�^�Z�b�g�R
	void		_pack_func_dat_set4(DWORD sign);	// ���k�֐� �f�[�^�Z�b�g�S
	void		_pack_func_dat_set5(DWORD sign);	// ���k�֐� �f�[�^�Z�b�g�T

	// �֐��e�[�u��
	typedef			void (Cnwa_pack::*_PACK_FUNC)(DWORD);
	_PACK_FUNC		_pack_func_dat_set[6];

	// �v���Z�X�X�e�b�v
	enum PROC_STEP{
		__START,
		__PACK,
		__COPY,
		__LAST_COPY,
		__END,
		__COMPLATE,
	};
	PROC_STEP	_proc_step;					// �v���Z�X�X�e�b�v

	// �w�b�_
	NWA_HEADER_STRUCT	_header;

	DWORD		_byte_per_sample;			// �P�T���v���̃o�C�g�T�C�Y

	// ���j�b�g���
	DWORD		_unit_index;				// ���j�b�g�̃C���f�b�N�X
	DWORD		_unit_size;					// ���j�b�g�̃T�C�Y
	DWORD		_last_unit_size;			// �Ō�̃��j�b�g�̃T�C�Y

	BYTE*		_src_data;					// �\�[�X�f�[�^�|�C���^

	// �o�͐�o�b�t�@
	BYTE*		_dst_buf;					// �o�͐�o�b�t�@
	DWORD		_dst_buf_size;				// �o�͐�o�b�t�@�T�C�Y
	DWORD		_dst_buf_additional;		// �o�͐�o�b�t�@�̒ǉ��T�C�Y

	DWORD		_pack_buf_size;				// ���k�o�b�t�@�T�C�Y

	// �����T�C�Y�ɉ����ă��[�v����
	DWORD		_break_out_size;			// ���[�v���������T�C�Y
	DWORD		_break_out_last;			// �O��̃��[�v�����l

	// ���k�����ϐ�
	struct PACK{
		BYTE*		src_p;					// �\�[�X�f�[�^�|�C���^
		UINT_PTR	src_end;				// �\�[�X�f�[�^�I���ʒu
		std::vector<BYTE>	dst;			// �o�͐�o�b�t�@
		BYTE*		dst_p;					// �o�͐�o�b�t�@�|�C���^
		DWORD		dst_bit_size;			// �o�͐�o�b�t�@�̃r�b�g�T�C�Y
		DWORD		zero_cnt;				// �y�d�q�n�̐�
		DWORD		transition;				// �T���v���l�̕ψ�
	} _pack;
};

