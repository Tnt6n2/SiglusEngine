#pragma		once

#include	"nwa_pack.h"

// ���j�b�g��
int nwa_unpack_unit(BYTE* src_p, DWORD src_smp_cnt, DWORD unpack_smp_top, DWORD unpack_smp_cnt, BYTE *dst_p, NWA_HEADER_STRUCT* header);

// ****************************************************************
// �m�v�`��
// ================================================================
class Cnwa_unpack
{

public:
				Cnwa_unpack();									// �R���X�g���N�^
				~Cnwa_unpack();									// �f�X�g���N�^

	// ��֐�
	bool		open(const TCHAR* file_path, DWORD offset);		// �t�@�C���I�[�v��
	void		close();										// �t�@�C���N���[�Y
	void		set_read_sample_pos(DWORD sample_pos);			// �ǂݍ��݈ʒu��ݒ�i�T���v���l�F�X�e���I�͍��E�łP�T���v���ł��j
	DWORD		get_read_sample_pos();							// �ǂݍ��݈ʒu���擾�i�T���v���l�F�X�e���I�͍��E�łP�T���v���ł��j
	DWORD		read_sample(BYTE* buf, DWORD sample_cnt);		// �ǂݍ��݁i�T���v�����F�X�e���I�͍��E�łP�T���v���ł��j

	// ����擾�֐�
	WORD		get_info_channels();							// ���擾 �`�����l�����擾
	WORD		get_info_bits_per_sample();						// ���擾 �r�b�g�l�擾
	DWORD		get_info_samples_per_sec();						// ���擾 ���g���擾

	DWORD		get_info_sample_cnt();							// ���擾 ���T���v�����擾�i�T���v�����F�X�e���I�͍��E�łP�T���v���ł��j

	// ���k���擾�֐�
	DWORD		get_info_original_size();						// ���擾 �I���W�i���T�C�Y�擾
	DWORD		get_info_pack_size();							// ���擾 ���k�T�C�Y�擾
	int			get_info_pack_mod();							// ���擾 ���k���[�h�擾�i-1=�����k  0=��i��  1=���i��  2=���i��  3=��荂�i��  4=����荂�i��  5=������荂�i���j
	int			get_info_zero_mod();							// ���擾 �y�d�q�n���k���[�h�擾�i0=�������Ȃ�  1=���������������k����j

private:

	void		_init();										// ������
	bool		_read_unit(DWORD unit_no);						// ���j�b�g��ǂݍ���ŉ𓀂���
	bool		_read_no_pack_data(DWORD need_byte_size, BYTE* dp, DWORD* copy_byte_size);		// �����k�f�[�^�̓ǂݍ���

	struct	_PM{
		FILE*				fp;							// �t�@�C���|�C���^
		DWORD				offset;						// �t�@�C���I�t�Z�b�g
		DWORD				read_sample_pos;			// �ǂݍ��݃T���v���ʒu
	} _pm;

	struct	_INFO{
		NWA_HEADER_STRUCT	header;						// �m�v�`�w�b�_
		std::vector<DWORD>	unit_ofs_tbl;				// ���j�b�g�I�t�Z�b�g�e�[�u��
		DWORD				one_sample_byte_size;		// �P�T���v���̃o�C�g��
	} _info;

	struct	_UNPACK{
		std::vector<BYTE>	buf;						// �𓀃o�b�t�@
		DWORD				unit_no;					// �𓀃��j�b�g�ԍ�
		DWORD				unit_sample_cnt;			// �𓀃��j�b�g�̃T���v����
	} _unpack;
};

