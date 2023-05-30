#pragma		once

// ****************************************************************
// �k�y�r�r���k
// ================================================================

#define		LZSS_BUF_REALLOC_SIZE		(1024*256)
#define		LZSS_PROG_CHECK_COUNT		(1024 * 10)

class Clzss_pack
{

public:
	void		ready(BYTE* src_data_, DWORD src_data_cnt_, DWORD break_out_cnt_);	// ����
	bool		proc(BYTE* dst_buf_, DWORD dst_buf_size_);		// ���k�v���Z�X

	DWORD		src_complate_cnt;					// ����������
	DWORD		dst_buf_need_size;					// ����Ȃ��o�b�t�@��
	DWORD		pack_size;							// ���k�T�C�Y

private:

	bool		_proc_copy();						// �v���Z�X ���k�f�[�^���o�b�t�@�ɃR�s�[����
	bool		_proc_last_copy();					// �v���Z�X �Ō�̈��k�f�[�^���o�b�t�@�ɃR�s�[����
	bool		_proc_end();						// �v���Z�X �I������
	bool		_make_pack_data();					// �f�[�^��
	bool		_copy_pack_buf(bool last_flag);		// �f�[�^���o�b�t�@�ɃR�s�[����

	// �v���Z�X�X�e�b�v
	enum PROC_STEP{
		__PACK,
		__COPY,
		__LAST_COPY,
		__END,
		__COMPLATE,
	};
	PROC_STEP	_proc_step;					// �v���Z�X�X�e�b�v

	DWORD		_break_out_cnt;				// ���[�v����������
	DWORD		_break_out_last;			// �O��̃��[�v�����l

	BYTE*		_src_data;					// �\�[�X�f�[�^
	DWORD		_src_data_cnt;				// �\�[�X�f�[�^�T�C�Y

	BYTE*		_dst_buf;					// �o�͐�o�b�t�@
	DWORD		_dst_buf_size;				// �o�͐�o�b�t�@�T�C�Y

	DWORD		_sizeof_data_size;
	DWORD		_window_size;
	DWORD		_length_bit_count;
	DWORD		_break_even;
	DWORD		_look_ahead_size;

	DWORD		_pack_buf_size;				// ���k�o�b�t�@�T�C�Y
	DWORD		_pack_bit_count;			// ���k�r�b�g�J�E���g

	DWORD		_pack_data_count;			// ���k�f�[�^�J�E���g
	BYTE		_pack_data[1 + (2 * 8)];	// ���k�f�[�^�F�ő� = flag(�P�r�b�g �~ �W) + coad(�Q�o�C�g �~ �W)

	Clzss_tree_find<BYTE>	_tree_find;		// �c���[����

	DWORD		_replace_cnt;				// ����ւ����i�����p�ϐ��j

};

