#pragma		once

class	C_gan_data;

// ****************************************************************
// �����}�l�[�W��
// ================================================================
class	C_tnm_gan_manager;
extern	C_tnm_gan_manager	G_gan_manager;

// ****************************************************************
// �f�`�m�}�l�[�W��
// ================================================================
class C_tnm_gan_manager
{
public:

	C_tnm_gan_manager();							// �R���X�g���N�^
	BSP<C_gan_data>	load(CTSTR& file_name);			// �ǂݍ���
	void			organize();						// ���\�[�X�f�[�^�𐮗�����
	int				get_map_now_cnt();				// ���݂̕����}�b�v�����擾����
	int				get_map_max_cnt();				// �ő�̕����}�b�v�����擾����

private:

	typedef		std::pair<TSTR, BSP<C_gan_data> >	GAN_DATA_PAIR;		// �f�`�m�f�[�^�y�A
	typedef		std::map<TSTR, BSP<C_gan_data> >	GAN_DATA_MAP;		// �f�`�m�f�[�^�}�b�v

	struct WORK
	{
		GAN_DATA_MAP	data_map;					// �f�`�m�f�[�^�}�b�v
		int				map_max_cnt;				// �f�`�m�f�[�^�}�b�v�}�b�v�̍ő吔
	}	work;

};



