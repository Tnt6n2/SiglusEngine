#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_global_data.h"
#include	"element/elm_object.h"
#include	"engine/eng_graphics.h"

static int	APPEAR_TIME_LEN = 1000;
static int	DISAPPEAR_TIME_LEN = 1000;
static int	CHANGE_TIME_LEN = 3000;		// �؂�ւ��ɂ����鎞��
static int	SCREEN_RATE = 1;			// ���z��ʃT�C�Y�i���Ǔ��{�ɂȂ�܂����c 2�ȏ�ɂ���ƁAcnt = 1 �̂Ƃ��ɕ����\�����ꂿ�Ⴄ�̂Łc�j


// ****************************************************************
// �I�u�W�F�N�g�F���p�����[�^�̏�����
// ================================================================
void C_elm_object::init_weather_param()
{
	m_op.weather.type = TNM_OBJECT_WEATHER_TYPE_NONE;
	m_op.weather.cnt = 0;
	m_op.weather.pat_mode = 0;
	m_op.weather.pat_no_00 = 0;
	m_op.weather.pat_no_01 = 0;
	m_op.weather.pat_time = 0;
	m_op.weather.move_time_x = 0;
	m_op.weather.move_time_y = 0;
	m_op.weather.sin_time_x = 0;
	m_op.weather.sin_time_y = 0;
	m_op.weather.sin_power_x = 0;
	m_op.weather.sin_power_y = 0;
	m_op.weather.center_x = 0;
	m_op.weather.center_y = 0;
	m_op.weather.center_rotate = 0;
	m_op.weather.appear_range = 0;
	m_op.weather.zoom_min = 0;
	m_op.weather.zoom_max = 0;
	m_op.weather.scale_x = 0;
	m_op.weather.scale_y = 0;
	m_op.weather.active_time = 0;
	m_op.weather.real_time_flag = false;

	// ��ƃp�����[�^
	m_weather.cnt_max = 0;
}

// ****************************************************************
// �I�u�W�F�N�g�F���I�u�W�F�N�g�̍쐬
// ================================================================
bool C_elm_object::create_weather(TSTR file_path)
{
	// �^�C�v�ʂ̃p�����[�^������������
	init_type(true);

	// �p�����[�^��ݒ�
	m_op.type = TNM_OBJECT_TYPE_WEATHER;
	m_op.file_path = file_path;

	// ���I�u�W�F�N�g���č\�z����
	if (!restruct_weather())
		return false;

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F���p�����[�^�̐ݒ�F�^�C�v�`
// ================================================================
bool C_elm_object::set_weather_param_type_a(int cnt, int pat_mode, int pat_no_00, int pat_no_01, int pat_time, int move_time_x, int move_time_y, int sin_time_x, int sin_power_x, int sin_time_y, int sin_power_y, int scale_x, int scale_y, int active_time, bool real_time)
{
	// ���I�u�W�F�N�g�łȂ������牽�����Ȃ�
	if (m_op.type != TNM_OBJECT_TYPE_WEATHER)
		return true;

	// ���X�Ɋ���ύX�����邽�߁A�^�C�v��������Ă͂����Ȃ�

	// �p�����[�^��ݒ�
	m_op.weather.type = TNM_OBJECT_WEATHER_TYPE_A;
	m_op.weather.cnt = cnt * SCREEN_RATE * SCREEN_RATE;
	m_op.weather.pat_mode = pat_mode;
	m_op.weather.pat_no_00 = pat_no_00;
	m_op.weather.pat_no_01 = pat_no_01;
	m_op.weather.pat_time = pat_time;
	m_op.weather.move_time_x = move_time_x;
	m_op.weather.move_time_y = move_time_y;
	m_op.weather.sin_time_x = sin_time_x;
	m_op.weather.sin_time_y = sin_time_y;
	m_op.weather.sin_power_x = sin_power_x;
	m_op.weather.sin_power_y = sin_power_y;
	m_op.weather.scale_x = scale_x;
	m_op.weather.scale_y = scale_y;
	m_op.weather.active_time = active_time;
	m_op.weather.real_time_flag = real_time;

	// ���I�u�W�F�N�g���č\�z����
	if (!restruct_weather())
		return false;

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F���p�����[�^�̐ݒ�F�^�C�v�a
// ================================================================
bool C_elm_object::set_weather_param_type_b(int cnt, int pat_mode, int pat_no_00, int pat_no_01, int pat_time, int center_x, int center_y, int appear_range, int move_time, int sin_time_x, int sin_power_x, int sin_time_y, int sin_power_y, int center_rotate, int zoom_min, int zoom_max, int scale_x, int scale_y, bool real_time)
{
	// ���I�u�W�F�N�g�łȂ������牽�����Ȃ�
	if (m_op.type != TNM_OBJECT_TYPE_WEATHER)
		return true;

	// ���X�Ɋ���ύX�����邽�߁A�^�C�v��������Ă͂����Ȃ�

	// �p�����[�^��ݒ�
	m_op.weather.type = TNM_OBJECT_WEATHER_TYPE_B;
	m_op.weather.cnt = cnt;
	m_op.weather.pat_mode = pat_mode;
	m_op.weather.pat_no_00 = pat_no_00;
	m_op.weather.pat_no_01 = pat_no_01;
	m_op.weather.pat_time = pat_time;
	m_op.weather.center_x = center_x;
	m_op.weather.center_y = center_y;
	m_op.weather.appear_range = appear_range;
	m_op.weather.move_time_x = move_time;
	m_op.weather.move_time_y = move_time;
	m_op.weather.sin_time_x = sin_time_x;
	m_op.weather.sin_time_y = sin_time_y;
	m_op.weather.sin_power_x = sin_power_x;
	m_op.weather.sin_power_y = sin_power_y;
	m_op.weather.center_rotate = center_rotate;
	m_op.weather.zoom_min = zoom_min;
	m_op.weather.zoom_max = zoom_max;
	m_op.weather.scale_x = scale_x;
	m_op.weather.scale_y = scale_y;
	m_op.weather.real_time_flag = real_time;

	// ���I�u�W�F�N�g���č\�z����
	if (!restruct_weather())
		return false;

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F���I�u�W�F�N�g���č\�z
// ================================================================
bool C_elm_object::restruct_weather()
{
	int i;

	// ���I�u�W�F�N�g�łȂ���Ή������Ȃ�
	if (m_op.type != TNM_OBJECT_TYPE_WEATHER)
		return true;

	// �e�N�X�`����ǂݍ���
	m_album = tnm_load_pct_d3d(m_op.file_path, false, m_op_def.set_cg_table);
	if (!m_album)	{
		m_op.file_path.clear();		// ���s�����̂Ńt�@�C�������N���A
		return false;
	}

	// ���݂̗��Ɋւ��čč\�z�t���O�𗧂Ă�
	for (int i = 0; i < (int)m_weather.sub.size(); i++)	{
		m_weather.sub[i].restruct_flag = true;
	}

	// ��ƃp�����[�^����������
	int old_cnt = (int)m_weather.sub.size();
	if (m_op.weather.cnt > old_cnt)	{
		m_weather.sub.resize(m_op.weather.cnt);
		for (i = old_cnt; i < m_op.weather.cnt; i++)	{
			setup_weather_param(i, 0);		// init ����
		}
	}

	// ��ƃp�����[�^�̍ő�l���X�V
	if (m_op.weather.cnt > m_weather.cnt_max)	{
		m_weather.cnt_max = m_op.weather.cnt;
	}

	// �X�v���C�g�̌�
	int sprite_cnt;
	switch (m_op.weather.type)	{
		case TNM_OBJECT_WEATHER_TYPE_A:		sprite_cnt = m_weather.cnt_max * 4;		break;		// �㉺���E��
		case TNM_OBJECT_WEATHER_TYPE_B:		sprite_cnt = m_weather.cnt_max;			break;
		default:							sprite_cnt = 0;							break;
	}

	// �X�v���C�g����������
	// ���͌��炳�Ȃ��I�I�}�Ɍ����Ă��܂��̂�h�����߁B
	int old_sprite_cnt = (int)m_sprite_list.size();
	if (sprite_cnt > old_sprite_cnt)	{
		m_sprite_list.resize(sprite_cnt);
		for (i = old_sprite_cnt; i < sprite_cnt; i++)	{
			if (!m_sprite_list[i].create_polygon_no_init())
				return tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���I�u�W�F�N�g�̍쐬�Ɏ��s���܂����B"));

			m_sprite_list[i].set_vertex_buffer(G_rect_vertex_buffer_d2_pct);
			m_sprite_list[i].set_index_buffer(G_rect_index_buffer);
		}
	}

	return true;
}

// ****************************************************************
// �I�u�W�F�N�g�F���p�����[�^���Z�b�g�A�b�v
// ================================================================
void C_elm_object::setup_weather_param(int sub_no, int init_state)
{
	C_elm_object_work_param_weather_sub* p_sub = &m_weather.sub[sub_no];
	C_size scr_size = Gp_global->game_screen_size;

	if (m_op.weather.type == TNM_OBJECT_WEATHER_TYPE_A)	{

		p_sub->state = init_state;				// init sleep
		p_sub->move_start_pos_x = rand() % scr_size.cx * SCREEN_RATE;
		p_sub->move_start_pos_y = rand() % scr_size.cy * SCREEN_RATE;//init_state == 0 ? rand() % scr_size.cy : 0;
		p_sub->move_time_x = m_op.weather.move_time_x * (rand() % 20 + 90) / 100;		// �X�O���`�P�P�O��
		p_sub->move_time_y = m_op.weather.move_time_y * (rand() % 20 + 90) / 100;		// �X�O���`�P�P�O��
		p_sub->move_cur_time = rand();
		p_sub->sin_time_x = m_op.weather.sin_time_x * (rand() % 20 + 90) / 100;			// �X�O���`�P�P�O��
		p_sub->sin_time_y = m_op.weather.sin_time_y * (rand() % 20 + 90) / 100;			// �X�O���`�P�P�O��
		p_sub->sin_power_x = m_op.weather.sin_power_x * (rand() % 20 + 90) / 100;		// �X�O���`�P�P�O��
		p_sub->sin_power_y = m_op.weather.sin_power_y * (rand() % 20 + 90) / 100;		// �X�O���`�P�P�O��
		p_sub->sin_cur_time = rand();
		p_sub->scale_x = m_op.weather.scale_x;
		p_sub->scale_y = m_op.weather.scale_y;

		p_sub->active_time_len = m_op.weather.active_time * (rand() % 40 + 80) / 100;		// �W�O���`�P�Q�O��
		p_sub->state_time_len = init_state == 0 ? rand() % CHANGE_TIME_LEN : p_sub->active_time_len;
		p_sub->state_cur_time = 0;
		p_sub->real_time_flag = m_op.weather.real_time_flag;
		p_sub->restruct_flag = false;
	}
	else if (m_op.weather.type == TNM_OBJECT_WEATHER_TYPE_B)	{

		int max_distance_x,	max_distance_y, max_distance;
		max_distance_x = m_op.weather.center_x > scr_size.cx / 2 ? m_op.weather.center_x : scr_size.cx - m_op.weather.center_x;
		max_distance_y = m_op.weather.center_y > scr_size.cy / 2 ? m_op.weather.center_y : scr_size.cy - m_op.weather.center_y;
		max_distance = (int)sqrt((double)max_distance_x * max_distance_x + (double)max_distance_y * max_distance_y);

		int appear_distance = rand() % max_distance * m_op.weather.appear_range / 100;
		double appear_radian = (double)rand() / RAND_MAX * D3DX_PI * 2;

		p_sub->state = init_state;				// init sleep
		p_sub->move_start_distance = appear_distance;
		p_sub->move_start_degree = (int)(appear_radian / D3DX_PI * 1800);
		p_sub->move_time_x = abs(m_op.weather.move_time_x) * (rand() % 40 + 80) / 100;	// �W�O���`�P�Q�O��
		p_sub->move_time_y = abs(m_op.weather.move_time_y) * (rand() % 40 + 80) / 100;	// �W�O���`�P�Q�O��
		p_sub->sin_time_x = m_op.weather.sin_time_x * (rand() % 20 + 90) / 100;			// �X�O���`�P�P�O��
		p_sub->sin_time_y = m_op.weather.sin_time_y * (rand() % 20 + 90) / 100;			// �X�O���`�P�P�O��
		p_sub->sin_power_x = m_op.weather.sin_power_x * (rand() % 20 + 90) / 100;		// �X�O���`�P�P�O��
		p_sub->sin_power_y = m_op.weather.sin_power_y * (rand() % 20 + 90) / 100;		// �X�O���`�P�P�O��
		p_sub->sin_cur_time = rand();
		p_sub->center_rotate = m_op.weather.center_rotate * (rand() % 20 + 90) / 100;	// �X�O���`�P�P�O��
		p_sub->zoom_min = m_op.weather.zoom_min;
		p_sub->zoom_max = m_op.weather.zoom_max;
		p_sub->scale_x = m_op.weather.scale_x;
		p_sub->scale_y = m_op.weather.scale_y;

		p_sub->active_time_len = abs(m_op.weather.move_time_x) * max_distance / 1000 - APPEAR_TIME_LEN;
		p_sub->state_time_len = init_state == 0 ? rand() % (APPEAR_TIME_LEN + p_sub->active_time_len + DISAPPEAR_TIME_LEN) : p_sub->active_time_len;
		p_sub->state_cur_time = 0;
		p_sub->move_cur_time = p_sub->state_time_len == 0 ? 0 : rand() % p_sub->state_time_len;
		p_sub->real_time_flag = m_op.weather.real_time_flag;
		p_sub->restruct_flag = false;
	}
}

// ****************************************************************
// �I�u�W�F�N�g�F���I�u�W�F�N�g�̎��Ԃ�i�߂�
// ================================================================
void C_elm_object::update_weather_time(int past_game_time, int past_real_time)
{
	int i;

	if (m_op.type == TNM_OBJECT_TYPE_WEATHER)	{

		for (i = 0; i < m_weather.cnt_max; i++)	{
			C_elm_object_work_param_weather_sub* p_sub = &m_weather.sub[i];

			// �^�C�v�`�E�^�C�v�a
			if (m_op.weather.type == TNM_OBJECT_WEATHER_TYPE_A || m_op.weather.type == TNM_OBJECT_WEATHER_TYPE_B)	{

				// ���S���ł������ɑ΂��Ă͉������Ȃ�
				if (!(i >= m_op.weather.cnt && p_sub->state == 0))	{
					int past_time = (p_sub->real_time_flag ? past_real_time : past_game_time);

					// ���Ԃ�i�߂�
					p_sub->state_cur_time += past_time;
					p_sub->move_cur_time += past_time;
					p_sub->sin_cur_time += past_time;

					// �č\�z����ׂ��Ȃ̂� state_time_len ���R�b�ȏ�c���Ă闱�͂R�b�����ɐݒ�
					if (i >= m_op.weather.cnt || p_sub->restruct_flag)	{
						if (p_sub->state == 2 && p_sub->state_time_len - p_sub->state_cur_time >= CHANGE_TIME_LEN)	{
							p_sub->state_cur_time = p_sub->state_time_len - rand() % CHANGE_TIME_LEN;
						}
					}
					// �X�e�[�g���Ԃ��߂���
					while (p_sub->state_cur_time - p_sub->state_time_len > 0)	{
						int amari_time = p_sub->state_cur_time - p_sub->state_time_len;

						// ���̃X�e�[�g��
						p_sub->state = (p_sub->state + 1) % 4;
						// sleep �ɂȂ�΍ăZ�b�g�A�b�v
						if (p_sub->state == 0)	{
							if (i >= m_op.weather.cnt)	{
								break;	// ���S����
							}
							else	{
								setup_weather_param(i, 1);			// appear ����
							}
						}
						// appear �ɂȂ����ꍇ���Ԃ�����������
						if (p_sub->state == 1)	{
							p_sub->move_cur_time = amari_time;	// ���܂莞�Ԃ�i�߂�
						}
						// �X�e�[�g�̎��Ԃ�ݒ�
						switch (p_sub->state)	{
							case 1:		p_sub->state_time_len = APPEAR_TIME_LEN;		break;	// appear
							case 2:		p_sub->state_time_len = p_sub->active_time_len;	break;	// active
							case 3:		p_sub->state_time_len = DISAPPEAR_TIME_LEN;		break;	// disappear
						}
						// �X�e�[�g�̌��ݎ��Ԃ�␳����
						p_sub->state_cur_time = amari_time;
					}
				}
			}
		}
	}
}

// ****************************************************************
// �I�u�W�F�N�g�F���I�u�W�F�N�g�̃t���[������
// ================================================================
#define		__MASHIRO_SUMMER	0

void C_elm_object::weather_frame()
{
	int i;

	C_size scr_size = Gp_global->game_screen_size;

	// ���̐ݒ�
	for (i = 0; i < m_weather.cnt_max; i++)	{
		C_elm_object_work_param_weather_sub* p_sub = &m_weather.sub[i];

		// sleep ���͉������Ȃ�
		if (p_sub->state > 0)	{

			// �p�^�[���ԍ������߂�
			int pat_no = 0;
			if (m_op.weather.pat_mode == 0)	{
				pat_no = m_op.weather.pat_no_00;
			}
			else if (m_op.weather.pat_mode == 1)	{	// ���[�v
				if (m_op.weather.pat_time > 0)	{
					int pat_time = (DWORD)p_sub->move_cur_time % m_op.weather.pat_time;
					int min = std::min(m_op.weather.pat_no_00, m_op.weather.pat_no_01);
					int max = std::max(m_op.weather.pat_no_00, m_op.weather.pat_no_01);
					pat_no = pat_time * (max - min + 1) / m_op.weather.pat_time + min;
				}
			}
			else if (m_op.weather.pat_mode == 2)	{
				int min = std::min(m_op.weather.pat_no_00, m_op.weather.pat_no_01);
				int max = std::max(m_op.weather.pat_no_00, m_op.weather.pat_no_01);
				pat_no = rand() % (max - min + 1) + min;
			}
			// �e�N�X�`����ݒ肷��
			if (0 <= pat_no && pat_no < m_album->get_texture_cnt())	{
				int x = 0, y = 0, tr = 255, scale_x = TNM_SCALE_UNIT, scale_y = TNM_SCALE_UNIT;

				// �o��
				switch (p_sub->state)	{
					case 1:		tr = tr * linear_limit(p_sub->state_cur_time, 0, 0, p_sub->state_time_len, 255) / 255;	break;	// appear
					case 3:		tr = tr * linear_limit(p_sub->state_cur_time, 0, 255, p_sub->state_time_len, 0) / 255;	break;	// disappear
				}

				// �^�C�v�`
				if (m_op.weather.type == TNM_OBJECT_WEATHER_TYPE_A)	{

#if __MASHIRO_SUMMER
					// �i���܂���T�}�[����j��ɃX�v���C�g�v�Z�����Ă���
					int sprite_no = i * 4;
					trp_to_rp(&m_sprite_list[sprite_no].rpex, &m_sprite_list[sprite_no].rp);
					x += (int)m_sprite_list[sprite_no].rp.pos.x;
					y += (int)m_sprite_list[sprite_no].rp.pos.y;
#endif
					// �����ʒu
					x += p_sub->move_start_pos_x;
					y += p_sub->move_start_pos_y;

					// �ړ�
					if (p_sub->move_time_x != 0)	{
						x += (int)(1000.0f / p_sub->move_time_x * (DWORD)p_sub->move_cur_time);
					}
					if (p_sub->move_time_y != 0)	{
						y += (int)(1000.0f / p_sub->move_time_y * (DWORD)p_sub->move_cur_time);
					}

					// �h��
					if (p_sub->sin_time_x > 0 && p_sub->sin_power_x > 0)	{
						double sin_time = (double)((DWORD)p_sub->sin_cur_time % p_sub->sin_time_x);
						double radian = linear(sin_time, (double)0.0f, (double)0.0f, (double)p_sub->sin_time_x, (double)(2 * D3DX_PI));
						x += (int)(sin(radian) * p_sub->sin_power_x);
					}
					if (p_sub->sin_time_y > 0 && p_sub->sin_power_y > 0)	{
						double sin_time = (double)((DWORD)p_sub->sin_cur_time % p_sub->sin_time_y);
						double radian = linear(sin_time, (double)0.0f, (double)0.0f, (double)p_sub->sin_time_y, (double)(2 * D3DX_PI));
						y += (int)(sin(radian) * p_sub->sin_power_y);
					}

					// ����␳
					x = x > 0 ? x % (scr_size.cx * SCREEN_RATE) : (scr_size.cx * SCREEN_RATE) - ((-x) % (scr_size.cx * SCREEN_RATE));
					y = y > 0 ? y % (scr_size.cy * SCREEN_RATE) : (scr_size.cy * SCREEN_RATE) - ((-y) % (scr_size.cy * SCREEN_RATE));

					// �X�P�[��
					scale_x = p_sub->scale_x;
					scale_y = p_sub->scale_y;

					// �[�ɂ��������ꍇ�̓X�v���C�g�𑝂₷
					bool over_l = false, over_r = false, over_u = false, over_d = false;
					int pct_w = m_album->get_texture(pat_no)->get_width();
					int pct_h = m_album->get_texture(pat_no)->get_height();
					int center_x = m_album->get_texture(pat_no)->get_center().x;
					int center_y = m_album->get_texture(pat_no)->get_center().y;
					if (x - center_x * scale_x / TNM_SCALE_UNIT < 0)
						over_l = true;
					if (x + (pct_w - center_x) * scale_x / TNM_SCALE_UNIT >= scr_size.cx * SCREEN_RATE)
						over_r = true;
					if (y - center_y * scale_y / TNM_SCALE_UNIT < 0)
						over_u = true;
					if (y + (pct_h - center_y) * scale_y / TNM_SCALE_UNIT >= scr_size.cy * SCREEN_RATE)
						over_d = true;

#if __MASHIRO_SUMMER
					// �X�v���C�g�ɐݒ�
					m_sprite_list[sprite_no].set_texture(m_album->get_texture(pat_no));
					m_sprite_list[sprite_no].rp.size_fit_to_texture = 0;
					m_sprite_list[sprite_no].rp.pos.x = (float)x;		// ������I
					m_sprite_list[sprite_no].rp.pos.y = (float)y;		// ������I
					m_sprite_list[sprite_no].rp.scale.x *= (float)scale_x / TNM_SCALE_UNIT;
					m_sprite_list[sprite_no].rp.scale.y *= (float)scale_y / TNM_SCALE_UNIT;
					m_sprite_list[sprite_no].rp.tr = m_sprite_list[sprite_no].rp.tr * tr / 255;
					if (over_l || over_r)	{	// ���E�ɂ͂ݏo����
						trp_to_rp(&m_sprite_list[sprite_no + 1].rpex, &m_sprite_list[sprite_no + 1].rp);
						m_sprite_list[sprite_no + 1].set_texture(m_album->get_texture(pat_no));
						m_sprite_list[sprite_no + 1].rp.size_fit_to_texture = 0;
						m_sprite_list[sprite_no + 1].rp.pos.x = (float)x + (over_l ? scr_size.cx : - scr_size.cx);		// ������I
						m_sprite_list[sprite_no + 1].rp.pos.y = (float)y;												// ������I
						m_sprite_list[sprite_no + 1].rp.scale.x *= (float)scale_x / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 1].rp.scale.y *= (float)scale_y / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 1].rp.tr = m_sprite_list[sprite_no + 1].rp.tr * tr / 255;
					}
					if (over_u || over_d)	{	// �㉺�ɂ͂ݏo����
						trp_to_rp(&m_sprite_list[sprite_no + 2].rpex, &m_sprite_list[sprite_no + 2].rp);
						m_sprite_list[sprite_no + 2].set_texture(m_album->get_texture(pat_no));
						m_sprite_list[sprite_no + 2].rp.size_fit_to_texture = 0;
						m_sprite_list[sprite_no + 2].rp.pos.x = (float)x;												// ������I
						m_sprite_list[sprite_no + 2].rp.pos.y = (float)y + (over_u ? scr_size.cy : - scr_size.cy);		// ������I
						m_sprite_list[sprite_no + 2].rp.scale.x *= (float)scale_x / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 2].rp.scale.y *= (float)scale_y / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 2].rp.tr = m_sprite_list[sprite_no + 2].rp.tr * tr / 255;
					}
					if ((over_l || over_r) && (over_u || over_d))	{	// �㉺�ɂ����E�ɂ��͂ݏo����
						trp_to_rp(&m_sprite_list[sprite_no + 3].rpex, &m_sprite_list[sprite_no + 3].rp);
						m_sprite_list[sprite_no + 3].set_texture(m_album->get_texture(pat_no));
						m_sprite_list[sprite_no + 3].rp.size_fit_to_texture = 0;
						m_sprite_list[sprite_no + 3].rp.pos.x = (float)x + (over_l ? scr_size.cx : - scr_size.cx);		// ������I
						m_sprite_list[sprite_no + 3].rp.pos.y = (float)y + (over_u ? scr_size.cy : - scr_size.cy);		// ������I
						m_sprite_list[sprite_no + 3].rp.scale.x *= (float)scale_x / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 3].rp.scale.y *= (float)scale_y / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 3].rp.tr = m_sprite_list[sprite_no + 3].rp.tr * tr / 255;
					}
#else
					// �X�v���C�g�ɐݒ�
					int sprite_no = i * 4;
					trp_to_rp(&m_sprite_list[sprite_no].rpex, &m_sprite_list[sprite_no].rp);
					m_sprite_list[sprite_no].set_texture(m_album->get_texture(pat_no));
					m_sprite_list[sprite_no].rp.size_fit_to_texture = 0;
					m_sprite_list[sprite_no].rp.pos.x += (float)x;
					m_sprite_list[sprite_no].rp.pos.y += (float)y;
					m_sprite_list[sprite_no].rp.scale.x *= (float)scale_x / TNM_SCALE_UNIT;
					m_sprite_list[sprite_no].rp.scale.y *= (float)scale_y / TNM_SCALE_UNIT;
					m_sprite_list[sprite_no].rp.tr = m_sprite_list[sprite_no].rp.tr * tr / 255;
					if (over_l || over_r)	{	// ���E�ɂ͂ݏo����
						trp_to_rp(&m_sprite_list[sprite_no + 1].rpex, &m_sprite_list[sprite_no + 1].rp);
						m_sprite_list[sprite_no + 1].set_texture(m_album->get_texture(pat_no));
						m_sprite_list[sprite_no + 1].rp.size_fit_to_texture = 0;
						m_sprite_list[sprite_no + 1].rp.pos.x += (float)x + (over_l ? scr_size.cx * SCREEN_RATE : -scr_size.cx * SCREEN_RATE);
						m_sprite_list[sprite_no + 1].rp.pos.y += (float)y;
						m_sprite_list[sprite_no + 1].rp.scale.x *= (float)scale_x / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 1].rp.scale.y *= (float)scale_y / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 1].rp.tr = m_sprite_list[sprite_no + 1].rp.tr * tr / 255;
					}
					if (over_u || over_d)	{	// �㉺�ɂ͂ݏo����
						trp_to_rp(&m_sprite_list[sprite_no + 2].rpex, &m_sprite_list[sprite_no + 2].rp);
						m_sprite_list[sprite_no + 2].set_texture(m_album->get_texture(pat_no));
						m_sprite_list[sprite_no + 2].rp.size_fit_to_texture = 0;
						m_sprite_list[sprite_no + 2].rp.pos.x += (float)x;
						m_sprite_list[sprite_no + 2].rp.pos.y += (float)y + (over_u ? scr_size.cy * SCREEN_RATE : -scr_size.cy * SCREEN_RATE);
						m_sprite_list[sprite_no + 2].rp.scale.x *= (float)scale_x / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 2].rp.scale.y *= (float)scale_y / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 2].rp.tr = m_sprite_list[sprite_no + 2].rp.tr * tr / 255;
					}
					if ((over_l || over_r) && (over_u || over_d))	{	// �㉺�ɂ����E�ɂ��͂ݏo����
						trp_to_rp(&m_sprite_list[sprite_no + 3].rpex, &m_sprite_list[sprite_no + 3].rp);
						m_sprite_list[sprite_no + 3].set_texture(m_album->get_texture(pat_no));
						m_sprite_list[sprite_no + 3].rp.size_fit_to_texture = 0;
						m_sprite_list[sprite_no + 3].rp.pos.x += (float)x + (over_l ? scr_size.cx * SCREEN_RATE : -scr_size.cx * SCREEN_RATE);
						m_sprite_list[sprite_no + 3].rp.pos.y += (float)y + (over_u ? scr_size.cy * SCREEN_RATE : -scr_size.cy * SCREEN_RATE);
						m_sprite_list[sprite_no + 3].rp.scale.x *= (float)scale_x / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 3].rp.scale.y *= (float)scale_y / TNM_SCALE_UNIT;
						m_sprite_list[sprite_no + 3].rp.tr = m_sprite_list[sprite_no + 3].rp.tr * tr / 255;
					}
#endif
				}
				// �^�C�v�a
				else if (m_op.weather.type == TNM_OBJECT_WEATHER_TYPE_B)	{

					// ���v����
					DWORD total_time = (DWORD)(APPEAR_TIME_LEN + p_sub->active_time_len + DISAPPEAR_TIME_LEN);
					DWORD move_cur_time = p_sub->move_time_x > 0 ? (DWORD)p_sub->move_cur_time : total_time - (DWORD)p_sub->move_cur_time;

					// �␳�ʒu
					double rep_x = 0, rep_y = 0;

					// �����ʒu
					rep_x = p_sub->move_start_distance;

					// �ړ��i�w�Ɉړ����Ԃ������Ă�j
					if (p_sub->move_time_x != 0)	{
						rep_x += 1000.0f / p_sub->move_time_x / p_sub->move_time_x * move_cur_time * move_cur_time;
					}

					// �h��
					if (p_sub->sin_time_x > 0 && p_sub->sin_power_x > 0)	{
						double sin_time = (double)(p_sub->sin_cur_time % p_sub->sin_time_x);
						double radian = linear(sin_time, (double)0.0f, (double)0.0f, (double)p_sub->sin_time_x, (double)(2 * D3DX_PI));
						rep_y += sin(radian) * p_sub->sin_power_x;		// x �� y �t�ɂȂ�_�ɒ��Ӂi�i�s������ y�j
					}
					if (p_sub->sin_time_y > 0 && p_sub->sin_power_y > 0)	{
						double sin_time = (double)(p_sub->sin_cur_time % p_sub->sin_time_y);
						double radian = linear(sin_time, (double)0.0f, (double)0.0f, (double)p_sub->sin_time_y, (double)(2 * D3DX_PI));
						rep_x += sin(radian) * p_sub->sin_power_y;		// x �� y �t�ɂȂ�_�ɒ��Ӂi�i�s������ y�j
					}

					// �p�x�����߂Ă���
					double radian = D3DXToRadian((double)p_sub->move_start_degree / TNM_ANGLE_UNIT);

					// ���S��]
					double theta = rep_x * ((double)p_sub->center_rotate / TNM_ANGLE_UNIT) / 1000;
					radian += D3DXToRadian(theta);

					// ��]
					double cos_value = cos(radian);
					double sin_value = sin(radian);
					x += (int)(rep_x * cos_value - rep_y * sin_value);
					y += (int)(rep_x * sin_value + rep_y * cos_value);

					// �ʒu����
					x += m_op.weather.center_x;
					y += m_op.weather.center_y;

					// �i�s����v�Z�i0�`1000�j
					int process = (int)((double)move_cur_time * 1000 / total_time);

					// �Y�[��
					scale_x = linear(process, 0, p_sub->zoom_min, 1000, p_sub->zoom_max);
					scale_y = linear(process, 0, p_sub->zoom_min, 1000, p_sub->zoom_max);

					// �X�v���C�g�ɐݒ�
					int sprite_no = i;
					trp_to_rp(&m_sprite_list[sprite_no].rpex, &m_sprite_list[sprite_no].rp);
					m_sprite_list[sprite_no].set_texture(m_album->get_texture(pat_no));
					m_sprite_list[sprite_no].rp.size_fit_to_texture = 0;
					m_sprite_list[sprite_no].rp.pos.x += (float)x;
					m_sprite_list[sprite_no].rp.pos.y += (float)y;
					m_sprite_list[sprite_no].rp.scale.x *= (float)scale_x / TNM_SCALE_UNIT;
					m_sprite_list[sprite_no].rp.scale.y *= (float)scale_y / TNM_SCALE_UNIT;
					m_sprite_list[sprite_no].rp.tr = m_sprite_list[sprite_no].rp.tr * tr / 255;
				}
			}
		}
	}
}


