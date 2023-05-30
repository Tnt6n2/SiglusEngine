#include	"pch.h"

#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_tool.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_screen.h"
#include	"engine/ifc_proc_stack.h"
#include	"engine/eng_graphics.h"

// ****************************************************************
// �X�N���[���F������
// ================================================================
void C_elm_screen::init(S_element element, CTSTR& name)
{
	// ���N���X�̏�����
	C_tnm_element::init(element, FM_SCREEN, name, NULL);

	// �G�t�F�N�g��������
	m_effect_list.init(element + S_element(ELM_SCREEN_EFFECT), FM_EFFECTLIST, name + _T(".effect"), Gp_ini->effect_cnt, false, NULL);

	// �N�G�C�N��������
	m_quake_list.init(element + S_element(ELM_SCREEN_QUAKE), FM_QUAKELIST, name + _T(".quake"), Gp_ini->quake_cnt, false, NULL);

	// �ď�����
	reinit();
}

// ****************************************************************
// �X�N���[���F�ď�����
// ================================================================
void C_elm_screen::reinit()
{
	// �G�t�F�N�g
	m_effect_list.reinit();

	// �V�F�C�N
	m_shake.init();

	// �N�G�C�N
	m_quake_list.reinit();
}

// ****************************************************************
// �X�N���[���F�Z�[�u
// ================================================================
void C_elm_screen::save(C_tnm_save_stream& stream)
{
	// �G�t�F�N�g
	m_effect_list.save(stream);

	// �V�F�C�N
	stream.save(m_shake);

	// �N�G�C�N
	m_quake_list.save(stream);
}

// ****************************************************************
// �X�N���[���F���[�h
// ================================================================
void C_elm_screen::load(C_tnm_save_stream& stream)
{
	// �G�t�F�N�g
	m_effect_list.load(stream);

	// �V�F�C�N
	stream.load(m_shake);

	// �N�G�C�N
	m_quake_list.load(stream);
}

// ****************************************************************
// �X�N���[���F���Ԃ�i�߂�
// ================================================================
void C_elm_screen::update_time(int past_game_time, int past_real_time)
{
	// �G�t�F�N�g
	for (ARRAY<C_elm_effect>::iterator itr_effect = m_effect_list.get_sub().begin(); itr_effect != m_effect_list.get_sub().end(); ++itr_effect)
		itr_effect->update_time(past_game_time, past_real_time);

	// �V�F�C�N
	m_shake.update_time(past_game_time);

	// �N�G�C�N
	for (ARRAY<C_elm_quake>::iterator itr_quake = m_quake_list.get_sub().begin(); itr_quake != m_quake_list.get_sub().end(); ++itr_quake)
		itr_quake->update_time(past_game_time);
}

// ****************************************************************
// �X�N���[���F�t���[������
// ================================================================
void C_elm_screen::frame()
{
	// �G�t�F�N�g
	for (ARRAY<C_elm_effect>::iterator itr_effect = m_effect_list.get_sub().begin(); itr_effect != m_effect_list.get_sub().end(); ++itr_effect)
		itr_effect->frame();

	// �V�F�C�N
	m_shake.frame();

	// �N�G�C�N
	for (ARRAY<C_elm_quake>::iterator itr_quake = m_quake_list.get_sub().begin(); itr_quake != m_quake_list.get_sub().end(); ++itr_quake)
		itr_quake->frame();
}

// ****************************************************************
// �G�t�F�N�g�F������
// ================================================================
void C_elm_effect::init(S_element element, CTSTR& name)
{
	// ���N���X�̏�����
	C_tnm_element::init(element, FM_EFFECT, name, NULL);

	// �ď�����
	reinit();
}

// ****************************************************************
// �G�t�F�N�g�F�ď�����
// ================================================================
void C_elm_effect::reinit()
{
	// �p�����[�^
	m_param.x.init(0);
	m_param.y.init(0);
	m_param.z.init(0);
	m_param.mono.init(0);
	m_param.reverse.init(0);
	m_param.bright.init(0);
	m_param.dark.init(0);
	m_param.color_r.init(0);
	m_param.color_g.init(0);
	m_param.color_b.init(0);
	m_param.color_rate.init(0);
	m_param.color_add_r.init(0);
	m_param.color_add_g.init(0);
	m_param.color_add_b.init(0);

	m_param.begin_sorter.order = 0;
	m_param.end_sorter.order = 0;
	m_param.begin_sorter.layer = INT_MIN;
	m_param.end_sorter.layer = INT_MAX;
	m_param.wipe_copy = 0;
	m_param.wipe_erase = 0;
}

// ****************************************************************
// �G�t�F�N�g�F�Z�[�u
// ================================================================
void C_elm_effect::save(C_tnm_save_stream& stream)
{
	// �p�����[�^
	stream.save(m_param);
}

// ****************************************************************
// �G�t�F�N�g�F���[�h
// ================================================================
void C_elm_effect::load(C_tnm_save_stream& stream)
{
	// �p�����[�^
	stream.load(m_param);
}

// ****************************************************************
// �G�t�F�N�g�F�R�s�[
// ================================================================
void C_elm_effect::copy(const C_elm_effect* p_src)
{
	// �p�����[�^
	m_param = p_src->m_param;
}

// ****************************************************************
// �G�t�F�N�g�F���Ԃ�i�߂�
// ================================================================
void C_elm_effect::update_time(int past_game_time, int past_real_time)
{
	m_param.x.update_time(past_game_time, past_real_time);
	m_param.y.update_time(past_game_time, past_real_time);
	m_param.z.update_time(past_game_time, past_real_time);
	m_param.mono.update_time(past_game_time, past_real_time);
	m_param.reverse.update_time(past_game_time, past_real_time);
	m_param.bright.update_time(past_game_time, past_real_time);
	m_param.dark.update_time(past_game_time, past_real_time);
	m_param.color_r.update_time(past_game_time, past_real_time);
	m_param.color_g.update_time(past_game_time, past_real_time);
	m_param.color_b.update_time(past_game_time, past_real_time);
	m_param.color_rate.update_time(past_game_time, past_real_time);
	m_param.color_add_r.update_time(past_game_time, past_real_time);
	m_param.color_add_g.update_time(past_game_time, past_real_time);
	m_param.color_add_b.update_time(past_game_time, past_real_time);
}

// ****************************************************************
// �G�t�F�N�g�F�t���[������
// ================================================================
void C_elm_effect::frame()
{
	m_param.x.frame();
	m_param.y.frame();
	m_param.z.frame();
	m_param.mono.frame();
	m_param.reverse.frame();
	m_param.bright.frame();
	m_param.dark.frame();
	m_param.color_r.frame();
	m_param.color_g.frame();
	m_param.color_b.frame();
	m_param.color_rate.frame();
	m_param.color_add_r.frame();
	m_param.color_add_g.frame();
	m_param.color_add_b.frame();
}

// ****************************************************************
// �G�t�F�N�g�F�����_�[�f�[�^���擾
// ================================================================
bool C_elm_effect::is_use()
{
	if (m_param.x.get_total_value() != 0)			return true;
	if (m_param.y.get_total_value() != 0)			return true;
	if (m_param.z.get_total_value() != 0)			return true;
	if (m_param.mono.get_total_value() != 0)		return true;
	if (m_param.reverse.get_total_value() != 0)		return true;
	if (m_param.bright.get_total_value() != 0)		return true;
	if (m_param.dark.get_total_value() != 0)		return true;
	if (m_param.color_r.get_total_value() != 0)		return true;
	if (m_param.color_g.get_total_value() != 0)		return true;
	if (m_param.color_b.get_total_value() != 0)		return true;
	if (m_param.color_rate.get_total_value() != 0)	return true;
	if (m_param.color_add_r.get_total_value() != 0)	return true;
	if (m_param.color_add_g.get_total_value() != 0)	return true;
	if (m_param.color_add_b.get_total_value() != 0)	return true;

	return false;
}

void C_elm_effect::get_rp(S_d3d_render_param* p_rp)
{
	p_rp->disp = true;
	p_rp->pos.x = (float)m_param.x.get_total_value();
	p_rp->pos.y = (float)m_param.y.get_total_value();
	p_rp->pos.z = (float)m_param.z.get_total_value();
	p_rp->mono = m_param.mono.get_total_value();
	p_rp->reverse = m_param.reverse.get_total_value();
	p_rp->bright = m_param.bright.get_total_value();
	p_rp->dark = m_param.dark.get_total_value();
	p_rp->color_r = m_param.color_r.get_total_value();
	p_rp->color_g = m_param.color_g.get_total_value();
	p_rp->color_b = m_param.color_b.get_total_value();
	p_rp->color_rate = m_param.color_rate.get_total_value();
	p_rp->color_add_r = m_param.color_add_r.get_total_value();
	p_rp->color_add_g = m_param.color_add_g.get_total_value();
	p_rp->color_add_b = m_param.color_add_b.get_total_value();
}

// ****************************************************************
// �G�t�F�N�g���X�g�F�T�u�̏�����
// ================================================================
void C_elm_effect_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"));
}

// ****************************************************************
// �G�t�F�N�g���X�g�F�T�u�̍ď�����
// ================================================================
void C_elm_effect_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// �G�t�F�N�g���X�g�F�T�u�̃Z�[�u
// ================================================================
void C_elm_effect_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// �G�t�F�N�g���X�g�F�T�u�̃��[�h
// ================================================================
void C_elm_effect_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}

// ****************************************************************
// �V�F�C�N�F������
// ================================================================
void S_tnm_shake::init()
{
	m_no = -1;
	m_cur_time = 0;

	// ��ƒl
	m_cur_shake_x = 0;
	m_cur_shake_y = 0;
}

// ****************************************************************
// �V�F�C�N�F�R�s�[���쐬
// ================================================================
void S_tnm_shake::copy(const S_tnm_shake* src)
{
	m_no = src->m_no;
	m_cur_time = src->m_cur_time;
}

// ****************************************************************
// �V�F�C�N�F�J�n
// ================================================================
void S_tnm_shake::start(int shake_no, bool wait_flag)
{
	if (shake_no < 0 || Gp_ini->shake_cnt <= shake_no)
		return;

	// �p�����[�^��ݒ�
	m_no = shake_no;
	m_cur_time = 0;

	// �҂���Ԃɓ���
	if (wait_flag)	{
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_SHAKE_WAIT;
		tnm_push_proc(proc);
	}
}

// ****************************************************************
// �V�F�C�N�F�I��
// ================================================================
void S_tnm_shake::end()
{
	m_no = -1;
}

// ****************************************************************
// �V�F�C�N�F���Ԃ�i�߂�
// ================================================================
void S_tnm_shake::update_time(int past_time)
{
	// ���Ԃ�i�߂�
	m_cur_time += past_time;
}

// ****************************************************************
// �V�F�C�N�F�t���[������
// ================================================================
void S_tnm_shake::frame()
{
	// �͈͊O�␳
	if (m_no < 0 || Gp_ini->shake_cnt <= m_no)
		m_no = -1;

	// �V�F�C�N���̏���
	if (m_no >= 0)	{

		// ini �̏����擾���Ă���
		C_tnm_ini::Sshake* p_ini = &Gp_ini->shake[m_no];

		// ���݂���s���I�h�����߂�
		int total_time = 0, cur_period = -1, period_time = 0;
		for (int p = 0; p < (int)p_ini->sub.size(); p++)	{
			period_time = m_cur_time - total_time;	// �s���I�h���ł̎���
			if (period_time < p_ini->sub[p].time)	{
				cur_period = p;
				break;
			}
			total_time += p_ini->sub[p].time;
		}

		// ���v���Ԃ��߂��Ă�����V�F�C�N�I��
		if (cur_period == -1)	{
			end();
		}
		else	{

			// �V�F�C�N���W�����߂�
			m_cur_shake_x = p_ini->sub[cur_period].x;
			m_cur_shake_y = p_ini->sub[cur_period].y;
		}
	}

	// �V�F�C�N�I���Ȃ�p�����[�^��������
	if (m_no == -1)
		init();
}

// ****************************************************************
// �N�G�C�N�F������
// ================================================================
void C_elm_quake::init(S_element element, CTSTR& name)
{
	// ���N���X�̏�����
	C_tnm_element::init(element, FM_QUAKE, name, NULL);

	// �ď�����
	reinit();
}

// ****************************************************************
// �N�G�C�N�F�ď�����
// ================================================================
void C_elm_quake::reinit()
{
	// �ʏ�p�����[�^
	m_param.type = TNM_QUAKE_TYPE_NONE;
	m_param.vec = TNM_QUAKE_VEC_U;
	m_param.power = 0;
	m_param.cur_time = 0;
	m_param.total_time = 0;
	m_param.end_flag = 0;
	m_param.end_cur_time = 0;
	m_param.end_total_time = 0;
	m_param.cnt = 0;
	m_param.end_cnt = 0;
	m_param.center_x = 0;
	m_param.center_y = 0;
	m_param.begin_order = 0;
	m_param.end_order = 0;

	// �ꎞ�p�����[�^
	m_work.cur_pos_x = 0;
	m_work.cur_pos_y = 0;
	m_work.cur_scale = TNM_SCALE_UNIT;
	m_work.cur_rotate = 0;
}

// ****************************************************************
// �N�G�C�N�F�Z�[�u
// ================================================================
void C_elm_quake::save(C_tnm_save_stream& stream)
{
	stream.save(m_param);
}

// ****************************************************************
// �N�G�C�N�F���[�h
// ================================================================
void C_elm_quake::load(C_tnm_save_stream& stream)
{
	stream.load(m_param);
}

// ****************************************************************
// �N�G�C�N�F�R�s�[
// ================================================================
void C_elm_quake::copy(const C_elm_quake* src)
{
	// �ʏ�p�����[�^
	m_param = src->m_param;
}

// ****************************************************************
// �N�G�C�N�F�uvec�v�J�n
// ================================================================
void C_elm_quake::start_vec(int vec, int power, int time, int cnt, int end_cnt, int begin_order, int end_order, bool wait_flag, bool key_flag)
{
	end(0);

	m_param.type = TNM_QUAKE_TYPE_VEC;
	m_param.vec = vec;
	m_param.power = power;
	m_param.cur_time = 0;
	m_param.total_time = time;
	m_param.cnt = cnt;
	m_param.end_cnt = end_cnt;
	m_param.begin_order = begin_order;
	m_param.end_order = end_order;

	// �҂���Ԃɓ���
	if (wait_flag)	{
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_QUAKE_WAIT;
		proc.element = get_element();
		proc.key_skip_enable_flag = key_flag;
		tnm_push_proc(proc);
	}
}

// ****************************************************************
// �N�G�C�N�F�udir�v�J�n
// ================================================================
void C_elm_quake::start_dir(int vec, int power, int time, int cnt, int end_cnt, int begin_order, int end_order, bool wait_flag, bool key_flag)
{
	end(0);

	m_param.type = TNM_QUAKE_TYPE_DIR;
	m_param.vec = vec;
	m_param.power = power;
	m_param.cur_time = 0;
	m_param.total_time = time;
	m_param.cnt = cnt;
	m_param.end_cnt = end_cnt;
	m_param.begin_order = begin_order;
	m_param.end_order = end_order;

	// �҂���Ԃɓ���
	if (wait_flag)	{
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_QUAKE_WAIT;
		proc.element = get_element();
		proc.key_skip_enable_flag = key_flag;
		tnm_push_proc(proc);
	}
}

// ****************************************************************
// �N�G�C�N�F�uzoom�v�J�n
// ================================================================
void C_elm_quake::start_zoom(int power, int time, int cnt, int end_cnt, int center_x, int center_y, int begin_order, int end_order, bool wait_flag, bool key_flag)
{
	end(0);

	m_param.type = TNM_QUAKE_TYPE_ZOOM;
	m_param.power = power;
	m_param.cur_time = 0;
	m_param.total_time = time;
	m_param.cnt = cnt;
	m_param.end_cnt = end_cnt;
	m_param.center_x = center_x;
	m_param.center_y = center_y;
	m_param.begin_order = begin_order;
	m_param.end_order = end_order;

	// �҂���Ԃɓ���
	if (wait_flag)	{
		C_tnm_proc proc;
		proc.type = TNM_PROC_TYPE_QUAKE_WAIT;
		proc.element = get_element();
		proc.key_skip_enable_flag = key_flag;
		tnm_push_proc(proc);
	}
}

// ****************************************************************
// �N�G�C�N�F�I���itime �b�����ďI���j
// ================================================================
void C_elm_quake::end(int time)
{
	m_param.end_flag = 1;
	m_param.end_cur_time = 0;
	m_param.end_total_time = time;

	// �w�肪�O�b�Ȃ�ď�����
	if (time == 0)
		reinit();
}

// ****************************************************************
// �N�G�C�N�F�҂�
// ================================================================
void C_elm_quake::wait(bool key_skip_flag)
{
	C_tnm_proc proc;
	proc.type = TNM_PROC_TYPE_QUAKE_WAIT;
	proc.element = get_element();
	proc.key_skip_enable_flag = key_skip_flag;
	tnm_push_proc(proc);
}

// ****************************************************************
// �N�G�C�N�F����
// ================================================================
int C_elm_quake::check()
{
	return
		m_param.type == TNM_QUAKE_TYPE_NONE ? 0 :	// �h��Ă��Ȃ��Ȃ�O
		m_param.end_flag == 1 ? 2 :					// �I���h�ꒆ�Ȃ�Q
		1;											// �h��Ă���Ȃ�P
}

// ****************************************************************
// �N�G�C�N�F���Ԃ�i�߂�
// ================================================================
void C_elm_quake::update_time(int past_time)
{
	// ���Ԃ����Z
	m_param.cur_time += past_time;
	m_param.end_cur_time += past_time;
}

// ****************************************************************
// �N�G�C�N�F�t���[������
// ================================================================
void C_elm_quake::frame()
{
	if (m_param.type >= 0)	{

		// ��O����
		if (m_param.total_time <= 0)	{
			reinit();	// �ď�����
		}
		else	{

			// �������t���O
			bool loop = (m_param.cnt == 0 && m_param.end_cnt == 0);

			// ���Ԃ��߂��Ă���ΏI��
			if (!loop && m_param.cur_time - (m_param.total_time * (m_param.cnt + m_param.end_cnt)) >= 0)	{
				reinit();	// �ď�����
			}
			else	{

				// �^�C�v�Fvec
				if (m_param.type == TNM_QUAKE_TYPE_VEC)	{
					int power = m_param.power;

					int x_sign = 0, y_sign = 0;
					if (m_param.vec == TNM_QUAKE_VEC_U)		{	x_sign = 0;		y_sign = -1;	}
					if (m_param.vec == TNM_QUAKE_VEC_RU)	{	x_sign = 1;		y_sign = -1;	}
					if (m_param.vec == TNM_QUAKE_VEC_R)		{	x_sign = 1;		y_sign = 0;		}
					if (m_param.vec == TNM_QUAKE_VEC_RD)	{	x_sign = 1;		y_sign = 1;		}
					if (m_param.vec == TNM_QUAKE_VEC_D)		{	x_sign = 0;		y_sign = 1;		}
					if (m_param.vec == TNM_QUAKE_VEC_LD)	{	x_sign = -1;	y_sign = 1;		}
					if (m_param.vec == TNM_QUAKE_VEC_L)		{	x_sign = -1;	y_sign = 0;		}
					if (m_param.vec == TNM_QUAKE_VEC_LU)	{	x_sign = -1;	y_sign = -1;	}

					int jump_cur_time = m_param.cur_time % m_param.total_time;
					if (jump_cur_time < m_param.total_time / 4)	{				// �i�ނP
						int time = jump_cur_time;
						m_work.cur_pos_x = speed_up_limit(time, 0, 0, m_param.total_time / 4, power / 2) * x_sign;
						m_work.cur_pos_y = speed_up_limit(time, 0, 0, m_param.total_time / 4, power / 2) * y_sign;
					}
					else if (jump_cur_time < m_param.total_time / 2)	{		// �i�ނQ
						int time = jump_cur_time - m_param.total_time / 4;
						m_work.cur_pos_x = speed_down_limit(time, 0, power / 2, m_param.total_time / 4, power) * x_sign;
						m_work.cur_pos_y = speed_down_limit(time, 0, power / 2, m_param.total_time / 4, power) * y_sign;
					}
					else if (jump_cur_time < m_param.total_time * 3 / 4)	{	// �߂�P
						int time = jump_cur_time - m_param.total_time / 2;
						m_work.cur_pos_x = speed_up_limit(time, 0, power, m_param.total_time / 4, power / 2) * x_sign;
						m_work.cur_pos_y = speed_up_limit(time, 0, power, m_param.total_time / 4, power / 2) * y_sign;
					}
					else if (jump_cur_time < m_param.total_time)	{			// �߂�Q
						int time = jump_cur_time - m_param.total_time * 3 / 4;
						m_work.cur_pos_x = speed_down_limit(time, 0, power / 2, m_param.total_time / 4, 0) * x_sign;
						m_work.cur_pos_y = speed_down_limit(time, 0, power / 2, m_param.total_time / 4, 0) * y_sign;
					}
				}

				// �^�C�v�Fdir
				else if (m_param.type == TNM_QUAKE_TYPE_DIR)	{
					int power = m_param.power;

					int x_sign = 0, y_sign = 0;
					if (m_param.vec == TNM_QUAKE_VEC_U)		{	x_sign = 0;		y_sign = -1;	}
					if (m_param.vec == TNM_QUAKE_VEC_RU)	{	x_sign = 1;		y_sign = -1;	}
					if (m_param.vec == TNM_QUAKE_VEC_R)		{	x_sign = 1;		y_sign = 0;		}
					if (m_param.vec == TNM_QUAKE_VEC_RD)	{	x_sign = 1;		y_sign = 1;		}
					if (m_param.vec == TNM_QUAKE_VEC_D)		{	x_sign = 0;		y_sign = 1;		}
					if (m_param.vec == TNM_QUAKE_VEC_LD)	{	x_sign = -1;	y_sign = 1;		}
					if (m_param.vec == TNM_QUAKE_VEC_L)		{	x_sign = -1;	y_sign = 0;		}
					if (m_param.vec == TNM_QUAKE_VEC_LU)	{	x_sign = -1;	y_sign = -1;	}

					// �h��
					int jump_cur_time = m_param.cur_time % m_param.total_time;
					if (jump_cur_time < m_param.total_time / 4)	{				// �i�ނP
						int time = jump_cur_time;
						m_work.cur_pos_x = speed_down_limit(time, 0, 0, m_param.total_time / 4, power / 2) * x_sign;
						m_work.cur_pos_y = speed_down_limit(time, 0, 0, m_param.total_time / 4, power / 2) * y_sign;
					}
					else if (jump_cur_time < m_param.total_time / 2)	{		// �߂�P
						int time = jump_cur_time - m_param.total_time / 4;
						m_work.cur_pos_x = speed_up_limit(time, 0, power / 2, m_param.total_time / 4, 0) * x_sign;
						m_work.cur_pos_y = speed_up_limit(time, 0, power / 2, m_param.total_time / 4, 0) * y_sign;
					}
					else if (jump_cur_time < m_param.total_time * 3 / 4)	{	// �i�ނQ
						int time = jump_cur_time - m_param.total_time / 2;
						m_work.cur_pos_x = speed_down_limit(time, 0, 0, m_param.total_time / 4, - power / 2) * x_sign;
						m_work.cur_pos_y = speed_down_limit(time, 0, 0, m_param.total_time / 4, - power / 2) * y_sign;
					}
					else if (jump_cur_time < m_param.total_time)	{			// �߂�Q
						int time = jump_cur_time - m_param.total_time * 3 / 4;
						m_work.cur_pos_x = speed_up_limit(time, 0, - power / 2, m_param.total_time / 4, 0) * x_sign;
						m_work.cur_pos_y = speed_up_limit(time, 0, - power / 2, m_param.total_time / 4, 0) * y_sign;
					}
				}
	
				// �^�C�v�Fzoom
				else if (m_param.type == TNM_QUAKE_TYPE_ZOOM)	{
					int power = limit(0, m_param.power, 255);
					int scale = 256 * TNM_SCALE_UNIT / (256 - power);
					int half_scale = (scale - TNM_SCALE_UNIT) / 2 + TNM_SCALE_UNIT;

					int jump_cur_time = m_param.cur_time % m_param.total_time;
					if (jump_cur_time < m_param.total_time / 4)			{		// �傫���Ȃ�
						int time = jump_cur_time;
						m_work.cur_scale = speed_up_limit(time, 0, TNM_SCALE_UNIT, m_param.total_time / 4, half_scale);
					}
					else if (jump_cur_time < m_param.total_time / 2)		{	// �傫���Ȃ�
						int time = jump_cur_time - m_param.total_time / 4;
						m_work.cur_scale = speed_down_limit(time, 0, half_scale, m_param.total_time / 4, scale);
					}
					else if (jump_cur_time < m_param.total_time * 3 / 4)	{	// �傫���Ȃ�
						int time = jump_cur_time - m_param.total_time / 2;
						m_work.cur_scale = speed_up_limit(time, 0, scale, m_param.total_time / 4, half_scale);
					}
					else	{													// �������Ȃ�
						int time = jump_cur_time - m_param.total_time * 3 / 4;
						m_work.cur_scale = speed_down_limit(time, 0, half_scale, m_param.total_time / 4, TNM_SCALE_UNIT);
					}
				}

				// �G���h�h��␳
				if (!loop && (m_param.cur_time - m_param.total_time * m_param.cnt >= 0))	{
					m_work.cur_pos_x = (int)((double)m_work.cur_pos_x * linear_limit<double, double>(m_param.cur_time, m_param.total_time * m_param.cnt, 1, m_param.total_time * (m_param.cnt + m_param.end_cnt), 0));
					m_work.cur_pos_y = (int)((double)m_work.cur_pos_y * linear_limit<double, double>(m_param.cur_time, m_param.total_time * m_param.cnt, 1, m_param.total_time * (m_param.cnt + m_param.end_cnt), 0));
					m_work.cur_scale = (int)((double)(m_work.cur_scale - TNM_SCALE_UNIT) * linear_limit<double, double>(m_param.cur_time, m_param.total_time * m_param.cnt, 1, m_param.total_time * (m_param.cnt + m_param.end_cnt), 0) + TNM_SCALE_UNIT);
				}

				// �I������
				if (m_param.end_flag)	{
					if (m_param.end_cur_time - m_param.end_total_time < 0)	{
						m_work.cur_pos_x = (int)((double)m_work.cur_pos_x * linear_limit<double, double>(m_param.end_cur_time, 0, 1, m_param.end_total_time, 0));
						m_work.cur_pos_y = (int)((double)m_work.cur_pos_y * linear_limit<double, double>(m_param.end_cur_time, 0, 1, m_param.end_total_time, 0));
						m_work.cur_scale = (int)((double)(m_work.cur_scale - TNM_SCALE_UNIT) * linear_limit<double, double>(m_param.end_cur_time, 0, 1, m_param.end_total_time, 0) + TNM_SCALE_UNIT);
					}
					else	{
						reinit();
					}
				}
			}
		}
	}

	// �N�G�C�N�I���Ȃ�p�����[�^��������
	if (m_param.type == TNM_QUAKE_TYPE_NONE)
		reinit();
}

// ****************************************************************
// �N�G�C�N�F�����_�[�p�����[�^���擾
// ================================================================
void C_elm_quake::get_rp(S_d3d_render_param* p_rp)
{
	if (m_param.type >= 0)	{
		p_rp->disp = true;
		p_rp->pos.x += m_work.cur_pos_x;
		p_rp->pos.y += m_work.cur_pos_y;
		p_rp->center.x += m_param.center_x;
		p_rp->center.y += m_param.center_y;
		p_rp->scale.x *= (float)m_work.cur_scale / TNM_SCALE_UNIT;
		p_rp->scale.y *= (float)m_work.cur_scale / TNM_SCALE_UNIT;
		p_rp->rotate.z = (float)m_work.cur_rotate / TNM_ANGLE_UNIT / 360 * D3DX_PI;
	}
}

// ****************************************************************
// �N�G�C�N���X�g�F�T�u�̏�����
// ================================================================
void C_elm_quake_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init(get_element() + S_element(ELM_ARRAY, i), get_element_name() + _T("[") + tostr(i) + _T("]"));
}

// ****************************************************************
// �N�G�C�N���X�g�F�T�u�̍ď�����
// ================================================================
void C_elm_quake_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// �N�G�C�N���X�g�F�T�u�̃Z�[�u
// ================================================================
void C_elm_quake_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// �N�G�C�N���X�g�F�T�u�̃��[�h
// ================================================================
void C_elm_quake_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}
