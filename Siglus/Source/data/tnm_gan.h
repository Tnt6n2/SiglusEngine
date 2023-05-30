#pragma		once

#include	"data/tnm_gan_data.h"

class	C_tnm_save_stream;

// ****************************************************************
// �f�`�m
// ================================================================
class C_tnm_gan
{
public:

	C_tnm_gan();													// �R���X�g���N�^
	~C_tnm_gan();													// �f�X�g���N�^

	void		init();												// ������
	void		free();												// ���
	void		save(C_tnm_save_stream& stream);					// �Z�[�u
	void		load(C_tnm_save_stream& stream);					// ���[�h
	void		copy(const C_tnm_gan* src);							// �R�s�[
	void		update_time(int past_game_time, int past_real_time);						// ���Ԃ�i�߂�

	bool		load_gan(CTSTR& file_name);							// �ǂݍ���
	bool		load_gan_only(CTSTR& file_name);					// �ǂݍ��݂̂�
	void		start_anm(int anm_set_no, bool anm_loop_flag, bool anm_real_time_flag);		// �A�j���J�n
	void		next_anm(int anm_set_no, bool anm_loop_flag, bool anm_real_time_flag);		// ���̃A�j����ݒ�i���݂̃A�j���̓����V���b�g�ɂȂ�܂��j
	void		change_anm_set_no(int anm_set_no);					// �A�j���̃Z�b�g�ԍ���ύX
	void		change_anm_loop(bool anm_loop_flag);				// �A�j���̃����V���b�g�^���[�v�̕ύX
	void		pause_anm();										// �A�j���|�[�Y
	void		resume_anm();										// �A�j���|�[�Y����


	int			get_pat_data_pat_no()	{	return (work.p_pat_data) ? work.p_pat_data->pat_no : 0;	}	// �p�^�[�����擾�F�p�^�[���ԍ�
	int			get_pat_data_x()		{	return (work.p_pat_data) ? work.p_pat_data->x : 0;		}	// �p�^�[�����擾�F�w���W
	int			get_pat_data_y()		{	return (work.p_pat_data) ? work.p_pat_data->y : 0;		}	// �p�^�[�����擾�F�x���W
	int			get_pat_data_tr()		{	return (work.p_pat_data) ? work.p_pat_data->tr : 255;	}	// �p�^�[�����擾�F�����x

private:

	struct WORK
	{
		BSP<C_gan_data>	p_gan_data;
		S_gan_pat_data*	p_pat_data;

		TSTR			gan_name;
		int				now_time;
		int				anm_set_no;
		int				next_anm_set_no;

		bool			anm_start;
		bool			anm_pause;
		bool			anm_loop_flag;
		bool			anm_real_time_flag;
		bool			next_anm_flag;
		bool			next_anm_loop_flag;
		bool			next_anm_real_time_flag;
	}	work;

};



