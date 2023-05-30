#include	"pch.h"

#include	"tnm_element_code.h"
#include	"ifc_eng.h"

#include	"data/tnm_save_00.h"

#include	"element/elm_pcm_event.h"
#include	"element/elm_sound.h"

#include	"engine/ifc_proc_stack.h"
#include	"engine/ifc_sound.h"

// ****************************************************************
// ���ʉ��C�x���g�F������
// ================================================================
void C_elm_pcm_event::init(S_element element, int form, CTSTR& name)
{
	// ���̏�����
	C_tnm_element::init(element, form, name, NULL);

	// �ď�����
	reinit();
}

// ****************************************************************
// ���ʉ��C�x���g�F�ď�����
// ================================================================
void C_elm_pcm_event::reinit()
{
	m_type = TNM_PCM_EVENT_TYPE_NONE;
	m_pcm_buf_no = -1;
	m_volume_type = TNM_VOLUME_TYPE_PCM;
	m_chara_no = -1;
	m_bgm_fade_target = false;
	m_bgm_fade2_target = false;
	m_real_flag = false;
	m_time_type = false;
	m_line_list.clear();
	m_cur_time = 0;
	m_cur_line_no = -1;
	m_next_time = 0;
	m_last_line_no = -1;
}

// ****************************************************************
// ���ʉ��C�x���g�F�Z�[�u
// ================================================================
void C_elm_pcm_event::save(C_tnm_save_stream& stream)
{
	stream.save(m_type);

	// ���[�v�⃉���_���̏ꍇ�̂݃��C����ۑ�����
	if (m_type == TNM_PCM_EVENT_TYPE_LOOP || m_type == TNM_PCM_EVENT_TYPE_RANDOM)	{
		stream.save(m_pcm_buf_no);
		stream.save(m_volume_type);
		stream.save(m_chara_no);
		stream.save(m_bgm_fade_target);
		stream.save(m_bgm_fade2_target);
		stream.save(m_bgm_fade_source);
		stream.save(m_real_flag);
		stream.save(m_time_type);
		stream.save(m_line_list.size());
		for (int i = 0; i < (int)m_line_list.size(); i++)	{
			stream.save(m_line_list[i].file_name);
			stream.save(m_line_list[i].min_time);
			stream.save(m_line_list[i].max_time);
			stream.save(m_line_list[i].probability);
		}
	}
}

// ****************************************************************
// ���ʉ��C�x���g�F���[�h
// ================================================================
void C_elm_pcm_event::load(C_tnm_save_stream& stream)
{
	// �ꎞ�ϐ��Ƀ��[�h
	int type = stream.load_ret<int>();

	// ���[�v�⃉���_���̏ꍇ�̂݃��C����ǂݍ���
	if (type == TNM_PCM_EVENT_TYPE_LOOP || type == TNM_PCM_EVENT_TYPE_RANDOM)	{
		stream.load(m_pcm_buf_no);
		stream.load(m_volume_type);
		stream.load(m_chara_no);
		stream.load(m_bgm_fade_target);
		stream.load(m_bgm_fade2_target);
		stream.load(m_bgm_fade_source);
		stream.load(m_real_flag);
		stream.load(m_time_type);
		m_line_list.resize(stream.load_ret<size_t>());
		for (int i = 0; i < (int)m_line_list.size(); i++)	{
			stream.load(m_line_list[i].file_name);
			stream.load(m_line_list[i].min_time);
			stream.load(m_line_list[i].max_time);
			stream.load(m_line_list[i].probability);
		}

		// �Đ����ĊJ����
		switch (type)	{
			case TNM_PCM_EVENT_TYPE_LOOP:	start_loop(m_pcm_buf_no, m_volume_type, m_chara_no, m_bgm_fade_target, m_bgm_fade2_target, m_bgm_fade_source, m_real_flag, m_time_type);	break;
			case TNM_PCM_EVENT_TYPE_RANDOM:	start_random(m_pcm_buf_no, m_volume_type, m_chara_no, m_bgm_fade_target, m_bgm_fade2_target, m_bgm_fade_source, m_real_flag, m_time_type);	break;
		}
	}
}

// ****************************************************************
// ���ʉ��C�x���g�F�����V���b�g�Đ�
// ================================================================
void C_elm_pcm_event::start_oneshot(int pcm_buf_no, int volume_type, int chara_no, bool bgm_fade_target, bool bgm_fade2_target, bool bgm_fade2_source, bool real_flag, bool time_type)
{
	m_type = TNM_PCM_EVENT_TYPE_ONESHOT;
	m_pcm_buf_no = pcm_buf_no;
	m_volume_type = volume_type;
	m_chara_no = chara_no;
	m_bgm_fade_target = bgm_fade_target;
	m_bgm_fade2_target = bgm_fade2_target;
	m_bgm_fade_source = bgm_fade2_source;
	m_real_flag = real_flag;
	m_time_type = time_type;

	m_cur_time = 0;
	m_cur_line_no = -1;
	m_next_time = 0;
	m_last_line_no = -1;
}

// ****************************************************************
// ���ʉ��C�x���g�F���[�v�Đ�
// ================================================================
void C_elm_pcm_event::start_loop(int pcm_buf_no, int volume_type, int chara_no, bool bgm_fade_target, bool bgm_fade2_target, bool bgm_fade2_source, bool real_flag, bool time_type)
{
	m_type = TNM_PCM_EVENT_TYPE_LOOP;
	m_pcm_buf_no = pcm_buf_no;
	m_volume_type = volume_type;
	m_chara_no = chara_no;
	m_bgm_fade_target = bgm_fade_target;
	m_bgm_fade2_target = bgm_fade2_target;
	m_bgm_fade_source = bgm_fade2_source;
	m_real_flag = real_flag;
	m_time_type = time_type;

	m_cur_time = 0;
	m_cur_line_no = -1;
	m_next_time = 0;
	m_last_line_no = -1;
}

// ****************************************************************
// ���ʉ��C�x���g�F�����_���Đ�
// ================================================================
void C_elm_pcm_event::start_random(int pcm_buf_no, int volume_type, int chara_no, bool bgm_fade_target, bool bgm_fade2_target, bool bgm_fade2_source, bool real_flag, bool time_type)
{
	m_type = TNM_PCM_EVENT_TYPE_RANDOM;
	m_pcm_buf_no = pcm_buf_no;
	m_volume_type = volume_type;
	m_chara_no = chara_no;
	m_bgm_fade_target = bgm_fade_target;
	m_bgm_fade2_target = bgm_fade2_target;
	m_bgm_fade_source = bgm_fade2_source;
	m_real_flag = real_flag;
	m_time_type = time_type;

	m_cur_time = 0;
	m_cur_line_no = -1;
	m_next_time = 0;
	m_last_line_no = -1;
}

// ****************************************************************
// ���ʉ��C�x���g�F��~
// ================================================================
void C_elm_pcm_event::stop(bool pcm_stop_flag)
{
	// ���ʉ����~�߂�
	if (pcm_stop_flag)	{
		if (m_pcm_buf_no == -1)	{

			// �Đ����Ă��Ȃ��ꍇ�� stop �𔭍s����ƁA�o�b�t�@�ԍ��������l�� -1 �Ȃ̂ŁA
			// �ėp�o�b�t�@���~�܂��Ă��܂��܂��I�Ɓ[�߂��w�u���Ŕ��o�B
			// �Ȃ̂ōĐ����̏ꍇ�̂ݎ~�߂�悤�ɂ��܂��B
			if (check())	{
				Gp_sound->m_pcm.stop();
			}
		}
		else if (0 <= m_pcm_buf_no && m_pcm_buf_no < (int)Gp_sound->m_pcmch_list.get_size()) 	{
			Gp_sound->m_pcmch_list[m_pcm_buf_no].stop();
		}
	}

	// �C�x���g���~�߂�
	reinit();
}

// ****************************************************************
// ���ʉ��C�x���g�F�Đ������ǂ����𔻒肷��
// ================================================================
bool C_elm_pcm_event::check()
{
	return m_type != TNM_PCM_EVENT_TYPE_NONE;
}

// ****************************************************************
// ���ʉ��C�x���g�F�Đ����I���܂ő҂�
// ================================================================
void C_elm_pcm_event::wait()
{
	// �����V���b�g�ȊO�͑҂��Ƃ��ł��Ȃ�
	if (m_type != TNM_PCM_EVENT_TYPE_ONESHOT)
		return;

	// �v���Z�X��ݒ�
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_PCM_EVENT_WAIT;
	proc.element = get_element();
	tnm_push_proc(proc);
}

// ****************************************************************
// ���ʉ��C�x���g�F�Đ����I���܂ő҂�
// ================================================================
void C_elm_pcm_event::wait_key()
{
	// �����V���b�g�ȊO�͑҂��Ƃ��ł��Ȃ�
	if (m_type != TNM_PCM_EVENT_TYPE_ONESHOT)
		return;

	// �v���Z�X��ݒ�
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_PCM_EVENT_WAIT;
	proc.element = get_element();
	proc.key_skip_enable_flag = true;
	proc.return_value_flag = true;
	tnm_push_proc(proc);
}

// ****************************************************************
// ���ʉ��C�x���g�F�t���[������
// ================================================================
void C_elm_pcm_event::frame(int past_game_time, int past_real_time)
{
	if (m_type != TNM_PCM_EVENT_TYPE_NONE)	{
		m_cur_time += m_real_flag ? past_real_time : past_game_time;

		if (false);

		// �����V���b�g�̏ꍇ
		else if (m_type == TNM_PCM_EVENT_TYPE_ONESHOT)	{
			while (m_cur_time - m_next_time >= 0)	{
				int length_millsecond = 0;

				// ���̃��C���֐i��
				m_cur_line_no ++;
				m_cur_time = 0;		// ���Ԃ�������

				// �͈͊O�Ȃ�C�x���g�I��
				if (m_cur_line_no < 0 || m_cur_line_no >= (int)m_line_list.size())	{
					reinit();
					break;
				}

				// �����ŉ���炷
				TSTR file_name = m_line_list[m_cur_line_no].file_name;
				if (m_pcm_buf_no == -1)	{
					int used_player_no = -1;
					Gp_sound->m_pcm.play_pcm(file_name, &used_player_no);

					// ���̒������~���b�Ŏ擾
					if (used_player_no >= 0)	{
						length_millsecond = Gp_sound->m_pcm.get_player(used_player_no)->get_length_by_millsecond();
					}
				}
				else if (0 <= m_pcm_buf_no && m_pcm_buf_no < (int)Gp_sound->m_pcmch_list.get_size()) 	{
					Gp_sound->m_pcmch_list[m_pcm_buf_no].play_pcm(file_name, false, false, 0, m_volume_type, m_chara_no, m_bgm_fade_target, m_bgm_fade2_target, m_bgm_fade_source, false);

					// ���̒������~���b�Ŏ擾
					length_millsecond = Gp_sound->m_pcmch_list[m_pcm_buf_no].get_length_by_millsecond();
				}

				// ���̃��C���̑҂����Ԃ����߂�
				int min_time = std::min(m_line_list[m_cur_line_no].min_time, m_line_list[m_cur_line_no].max_time) + (m_time_type ? length_millsecond : 0);
				int max_time = std::max(m_line_list[m_cur_line_no].min_time, m_line_list[m_cur_line_no].max_time) + (m_time_type ? length_millsecond : 0);
				m_next_time = (max_time == min_time) ? max_time : rand() % (max_time - min_time) + min_time;
			}
		}

		// ���[�v�̏ꍇ
		else if (m_type == TNM_PCM_EVENT_TYPE_LOOP)	{
			while (m_cur_time - m_next_time >= 0)	{
				int length_millsecond = 0;

				// ���̃��C���֐i��
				m_cur_line_no = (m_cur_line_no + 1) % (int)m_line_list.size();
				m_cur_time = 0;		// ���Ԃ�������

				// �����ŉ���炷
				TSTR file_name = m_line_list[m_cur_line_no].file_name;
				if (m_pcm_buf_no == -1)	{
					int used_player_no = -1;
					Gp_sound->m_pcm.play_pcm(file_name, &used_player_no);

					// ���̒������~���b�Ŏ擾
					if (used_player_no >= 0)	{
						length_millsecond = Gp_sound->m_pcm.get_player(used_player_no)->get_length_by_millsecond();
					}
				}
				else if (0 <= m_pcm_buf_no && m_pcm_buf_no < (int)Gp_sound->m_pcmch_list.get_size()) 	{
					Gp_sound->m_pcmch_list[m_pcm_buf_no].play_pcm(file_name, false, false, 0, m_volume_type, m_chara_no, m_bgm_fade_target, m_bgm_fade2_target, m_bgm_fade_source, false);

					// ���̒������~���b�Ŏ擾
					length_millsecond = Gp_sound->m_pcmch_list[m_pcm_buf_no].get_length_by_millsecond();
				}

				// ���̃��C���̑҂����Ԃ����߂�
				int min_time = std::min(m_line_list[m_cur_line_no].min_time, m_line_list[m_cur_line_no].max_time) + (m_time_type ? length_millsecond : 0);
				int max_time = std::max(m_line_list[m_cur_line_no].min_time, m_line_list[m_cur_line_no].max_time) + (m_time_type ? length_millsecond : 0);
				m_next_time = (max_time == min_time) ? max_time : rand() % (max_time - min_time) + min_time;
			}
		}

		// �����_���̏ꍇ
		else if (m_type == TNM_PCM_EVENT_TYPE_RANDOM)	{
			while (m_cur_time - m_next_time >= 0)	{
				int length_millsecond = 0;

				// ���݂̃��C���ԍ����L������
				m_last_line_no = m_cur_line_no;

				// ���C�������肷��
				// �O��̃��C���͊m���������ɂȂ邱�Ƃ��l�����Čv�Z����
				int line_cnt = (int)m_line_list.size();
				int total_probability = 0;

				for (int i = 0; i < line_cnt; i++)	{
					if (i == m_last_line_no)	{
						total_probability += m_line_list[i].probability;
					}
					else	{
						total_probability += m_line_list[i].probability * 2;
					}
				}
				int cur_total_probability = 0;
				int pos = (total_probability == 0) ? 0 : rand() % total_probability;
				for (int i = 0; i < line_cnt; i++)	{
					if (i == m_last_line_no)	{
						cur_total_probability += m_line_list[i].probability;
					}
					else	{
						cur_total_probability += m_line_list[i].probability * 2;
					}
					if (pos < cur_total_probability)	{
						m_cur_line_no = i;
						break;
					}
				}
				
				// ���Ԃ�������
				m_cur_time = 0;

				// �����ŉ���炷
				TSTR file_name = m_line_list[m_cur_line_no].file_name;
				if (m_pcm_buf_no == -1)	{
					int used_player_no = -1;
					Gp_sound->m_pcm.play_pcm(file_name, &used_player_no);

					// ���̒������~���b�Ŏ擾
					if (used_player_no >= 0)	{
						length_millsecond = Gp_sound->m_pcm.get_player(used_player_no)->get_length_by_millsecond();
					}
				}
				else if (0 <= m_pcm_buf_no && m_pcm_buf_no < (int)Gp_sound->m_pcmch_list.get_size()) 	{
					Gp_sound->m_pcmch_list[m_pcm_buf_no].play_pcm(file_name, false, false, 0, m_volume_type, m_chara_no, m_bgm_fade_target, m_bgm_fade2_target, m_bgm_fade_source, false);

					// ���̒������~���b�Ŏ擾
					length_millsecond = Gp_sound->m_pcmch_list[m_pcm_buf_no].get_length_by_millsecond();
				}

				// ���̃��C���̑҂����Ԃ����߂�
				int min_time = std::min(m_line_list[m_cur_line_no].min_time, m_line_list[m_cur_line_no].max_time) + (m_time_type ? length_millsecond : 0);
				int max_time = std::max(m_line_list[m_cur_line_no].min_time, m_line_list[m_cur_line_no].max_time) + (m_time_type ? length_millsecond : 0);
				m_next_time = (max_time == min_time) ? max_time : rand() % (max_time - min_time) + min_time;
			}
		}
	}
}

// ****************************************************************
// ���ʉ��C�x���g���X�g�F�T�u�̏�����
// ================================================================
void C_elm_pcm_event_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), FM_PCMEVENT, get_element_name() + _T("[") + tostr(i) + _T("]"));
}

// ****************************************************************
// ���ʉ��C�x���g���X�g�F�T�u�̍ď�����
// ================================================================
void C_elm_pcm_event_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// ���ʉ��C�x���g���X�g�F�T�u�̃Z�[�u
// ================================================================
void C_elm_pcm_event_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// ���ʉ��C�x���g���X�g�F�T�u�̃��[�h
// ================================================================
void C_elm_pcm_event_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}
