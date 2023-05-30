#include	"pch.h"

#include	"data/tnm_gan_data.h"
#include	"data/tnm_gan_manager.h"

// ****************************************************************
// �f�`�m�}�l�[�W���̎���
// ================================================================
C_tnm_gan_manager	G_gan_manager;

// ****************************************************************
// �R���X�g���N�^
// ================================================================
C_tnm_gan_manager::C_tnm_gan_manager()
{
	work.map_max_cnt = 0;
}

// ****************************************************************
// �ǂݍ���
// ================================================================
BSP<C_gan_data> C_tnm_gan_manager::load(CTSTR& file_path)
{
	// ���łɓǂݍ��܂�Ă��Ȃ�����������
	GAN_DATA_MAP::iterator itr = work.data_map.find(file_path);
	if (itr != work.data_map.end())	{	return itr->second;	}

	// �ǂݍ���
	GAN_DATA_PAIR data_pair;
	data_pair.second = BSP<C_gan_data>(new C_gan_data);
	if (!data_pair.second->load(file_path))	{
		return BSP<C_gan_data>();	// �ǂݍ��݃G���[
	}

	// �}�b�v�ɒǉ�
	data_pair.first = file_path;
	work.data_map.insert(data_pair);

	// �ő�}�b�v��
	int now_cnt = (int )work.data_map.size();
	if (work.map_max_cnt < now_cnt)	{	work.map_max_cnt = now_cnt;	}

	return data_pair.second;
}

// ****************************************************************
// ���\�[�X�f�[�^�𐮗�����
// ================================================================
void C_tnm_gan_manager::organize()
{
	if (work.data_map.empty())	{	return;	}
	for (GAN_DATA_MAP::iterator itr = work.data_map.begin(); itr != work.data_map.end();)	{
		if (itr->second.use_count() == 1)	{
			itr = work.data_map.erase(itr);
		}
		else	{
			++itr;
		}
	}
}

// ****************************************************************
// ���݂̕����}�b�v�����擾����
// ================================================================
int C_tnm_gan_manager::get_map_now_cnt()
{
	return (int)work.data_map.size();
}

// ****************************************************************
// �ő�̕����}�b�v�����擾����
// ================================================================
int C_tnm_gan_manager::get_map_max_cnt()
{
	return work.map_max_cnt;
}



