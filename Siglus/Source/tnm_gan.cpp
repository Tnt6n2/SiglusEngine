#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_save_00.h"
#include	"data/tnm_gan.h"
#include	"data/tnm_gan_manager.h"
#include	"engine/eng_dir.h"
#include	"engine/ifc_error.h"

// �������f�`�m�֘A
// �����߂����Ȃ��ƂˁB
// ���V�^�̂f�`�m����肽���B�i�c�[�������Ȃ��Ɓc�j

// ****************************************************************
// �R���X�g���N�^
// ================================================================
C_tnm_gan::C_tnm_gan()
{
	init();
}

// ****************************************************************
// �f�X�g���N�^
// ================================================================
C_tnm_gan::~C_tnm_gan()
{
}

// ****************************************************************
// ������
// ================================================================
void C_tnm_gan::init()
{
	work.p_gan_data.reset();
	work.p_pat_data = NULL;

	work.gan_name.clear();
	work.now_time = 0;
	work.anm_set_no = 0;
	work.next_anm_set_no = 0;

	work.anm_start = false;
	work.anm_pause = false;
	work.anm_loop_flag = false;
	work.anm_real_time_flag = false;

	work.next_anm_flag = false;
	work.next_anm_loop_flag = false;
	work.next_anm_real_time_flag = false;
}

// ****************************************************************
// ���
// ================================================================
void C_tnm_gan::free()
{
	init();
}

// ****************************************************************
// �Z�[�u
// ================================================================
void C_tnm_gan::save(C_tnm_save_stream& stream)
{
	stream.save(work.gan_name);
	stream.save(work.now_time);
	stream.save(work.anm_set_no);
	stream.save(work.next_anm_set_no);

	stream.save(work.anm_start);
	stream.save(work.anm_pause);
	stream.save(work.anm_loop_flag);
	stream.save(work.anm_real_time_flag);
	stream.save(work.next_anm_flag);
	stream.save(work.next_anm_loop_flag);
	stream.save(work.next_anm_real_time_flag);
}

// ****************************************************************
// ���[�h
// ================================================================
void C_tnm_gan::load(C_tnm_save_stream& stream)
{
	stream.load(work.gan_name);
	stream.load(work.now_time);
	stream.load(work.anm_set_no);
	stream.load(work.next_anm_set_no);

	stream.load(work.anm_start);
	stream.load(work.anm_pause);
	stream.load(work.anm_loop_flag);
	stream.load(work.anm_real_time_flag);
	stream.load(work.next_anm_flag);
	stream.load(work.next_anm_loop_flag);
	stream.load(work.next_anm_real_time_flag);

	// �č\�z
	load_gan_only(work.gan_name);
}

// ****************************************************************
// �R�s�[
// ================================================================
void C_tnm_gan::copy(const C_tnm_gan* src)
{
	work.p_gan_data = src->work.p_gan_data;
	work.p_pat_data = src->work.p_pat_data;

	work.gan_name = src->work.gan_name;
	work.now_time = src->work.now_time;
	work.anm_set_no = src->work.anm_set_no;
	work.next_anm_set_no = src->work.next_anm_set_no;

	work.anm_start = src->work.anm_start;
	work.anm_pause = src->work.anm_pause;
	work.anm_loop_flag = src->work.anm_loop_flag;
	work.anm_real_time_flag = src->work.anm_real_time_flag;
	work.next_anm_flag = src->work.next_anm_flag;
	work.next_anm_loop_flag = src->work.next_anm_loop_flag;
	work.next_anm_real_time_flag = src->work.next_anm_real_time_flag;
}

// ****************************************************************
// �ǂݍ���
// ================================================================
bool C_tnm_gan::load_gan(CTSTR& file_name)
{
	free();								// ���
	return load_gan_only(file_name);	// �ǂݍ��݂̂�
}

// ****************************************************************
// �ǂݍ��݂̂݁i�t�@�C���݂̂̕ύX�ł��B�A�j���̏�Ԃ�����������܂���j
// ================================================================
bool C_tnm_gan::load_gan_only(CTSTR& gan_name)
{
	if (gan_name.empty())
		return true;

	work.gan_name = gan_name;

	TSTR gan_path = tnm_find_gan(gan_name);

	work.p_gan_data = G_gan_manager.load(gan_path);
	if (!work.p_gan_data.get())	{
		tnm_set_error(TNM_ERROR_TYPE_FILE_NOT_FOUND, gan_name + _T(".gan ��������܂���ł����B"));
		return false;
	}

	return true;
}

// ****************************************************************
// �A�j���J�n
// ================================================================
void C_tnm_gan::start_anm(int anm_set_no, bool anm_loop_flag, bool anm_real_time_flag)
{
	work.now_time = 0;
	work.anm_start = true;
	work.anm_pause = false;
	work.anm_set_no = anm_set_no;
	work.anm_loop_flag = anm_loop_flag;
	work.anm_real_time_flag = anm_real_time_flag;
	work.next_anm_flag = false;
}

// ****************************************************************
// ���̃A�j����ݒ�i���݂̃A�j���̓����V���b�g�ɂȂ�܂��j
// ================================================================
void C_tnm_gan::next_anm(int anm_set_no, bool anm_loop_flag, bool anm_real_time_flag)
{
	// ���݃A�j����
	if (work.anm_start)	{
		if (work.next_anm_flag)	{	// ���̃A�j�������ɏ�������Ă���ꍇ�́A����������V���b�g�ŊJ�n����
			start_anm(work.next_anm_set_no, false, work.next_anm_real_time_flag);
		}
		else	{
			work.anm_loop_flag = false;					// ���݂̃A�j���̓����V���b�g�ɂȂ�܂�
		}
		work.next_anm_flag = true;
		work.next_anm_set_no = anm_set_no;
		work.next_anm_loop_flag = anm_loop_flag;
		work.next_anm_real_time_flag = anm_real_time_flag;
	}
	// �A�j�����Ă��Ȃ��ꍇ�͑����s
	else	{
		start_anm(anm_set_no, anm_loop_flag, anm_real_time_flag);
	}
}

// ****************************************************************
// �A�j���̃Z�b�g�ԍ���ύX
// ================================================================
void C_tnm_gan::change_anm_set_no(int anm_set_no)
{
	if (work.next_anm_flag)	{	// ���̃A�j�������ɏ�������Ă���ꍇ�́A�������ݒ肷��
		work.next_anm_set_no = anm_set_no;
	}
	else	{
		work.anm_set_no = anm_set_no;
	}
}

// ****************************************************************
// �A�j���̃����V���b�g�^���[�v�̕ύX
// ================================================================
void C_tnm_gan::change_anm_loop(bool anm_loop_flag)
{
	if (work.next_anm_flag)	{	// ���̃A�j�������ɏ�������Ă���ꍇ�́A�������ݒ肷��
		work.next_anm_loop_flag = anm_loop_flag;
	}
	else	{
		work.anm_loop_flag = anm_loop_flag;
	}
}

// ****************************************************************
// �A�j���|�[�Y
// ================================================================
void C_tnm_gan::pause_anm()
{
	work.anm_pause = true;
}

// ****************************************************************
// �A�j���|�[�Y����
// ================================================================
void C_tnm_gan::resume_anm()
{
	work.anm_pause = false;
}

// ****************************************************************
// ���Ԃ�i�߂�
// ================================================================
void C_tnm_gan::update_time(int past_game_time, int past_real_time)
{
	// ���Ԃ��}�C�i�X�̏ꍇ�͕␳�inext �ւ̐؂�ւ��ŁA�ċA�I�ɌĂ΂��ꍇ�ɋN���蓾��j
	if (past_game_time < 0)	{
		past_game_time = 0;
	}
	if (past_real_time < 0)	{
		past_real_time = 0;
	}

	// �f�`�m�f�[�^������
	if (!work.p_gan_data.get())	{
		work.p_pat_data = NULL;
		return;
	}

	// �Z�b�g�ԍ�����������
	if (!(0 <= work.anm_set_no && work.anm_set_no < (int)work.p_gan_data->set_list.size()))	{
		work.p_pat_data = NULL;
		return;
	}

	// �Z�b�g�ւ̃C�e���[�^
	ARRAY<S_gan_set_data>::iterator set_itr = work.p_gan_data->set_list.begin() + work.anm_set_no;

	// �p�^�[��������
	if (set_itr->pat_list.empty())	{
		work.p_pat_data = NULL;
		return;
	}

	// �A�j�����J�n����Ă��Ȃ��i�O�ԃp�^�[����ݒ�j
	if (!work.anm_start)	{
		work.p_pat_data = &*(set_itr->pat_list.begin());
		return;
	}

	// �A�j�����Ԃ��O�i�O�ԃp�^�[����ݒ�j
	if (set_itr->total_time <= 0)	{
		work.p_pat_data = &*(set_itr->pat_list.begin());
		return;
	}

	// ���Ԃ�i�߂�
	if (!work.anm_pause)	{
		if (work.anm_real_time_flag)	{
			work.now_time += past_real_time;
		}
		else	{
			work.now_time += past_game_time;
		}
	}

	// �����V���b�g�̏ꍇ
	if (!work.anm_loop_flag)	{
		if (work.now_time >= set_itr->total_time)	{
			if (work.next_anm_flag)	{
				// �����V���b�g���ցi���̒��� work.anm_real_time_flag ���ύX����Ă��邱�Ƃɒ��ӁI�j
				start_anm(work.next_anm_set_no, work.next_anm_loop_flag, work.next_anm_real_time_flag);
				// ���Ԃ�i�߂�
				update_time(past_game_time - (work.now_time - set_itr->total_time), past_real_time - (work.now_time - set_itr->total_time));
			}
			else	{
				// �����V���b�g�҂��i�Ō�̃p�^�[����\���j
				work.now_time = set_itr->total_time;
				work.p_pat_data = &*(set_itr->pat_list.end() - 1);
			}
			return;
		}
	}

	// ���Ԃ�␳
	work.now_time %= set_itr->total_time;

	// �A�j������
	for (ARRAY<S_gan_pat_data>::iterator pat_itr = set_itr->pat_list.begin(); pat_itr != set_itr->pat_list.end(); ++pat_itr)	{
		if (pat_itr->keika_time >= work.now_time)	{
			work.p_pat_data = &*pat_itr;
			break;
		}
	}
}



