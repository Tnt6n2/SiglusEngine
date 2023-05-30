#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_local_data.h"
#include	"element/elm_sound.h"
#include	"dialog/db_wnd_info_koe.h"
#include	"engine/ifc_sound.h"
#include	"../resource.h"

// ****************************************************************
// �f�o�b�O���E�B���h�E�F���ǃt���O
// ================================================================

#define		REGIST_WINDOW_NAME		_T("db_wnd_info_koe")

// ������
void C_db_wnd_info_koe::init()
{
	regist(REGIST_WINDOW_NAME, false, true, IDD_DB_WIN_INFO_KOE);
}

// �E�B���h�E�v���V�[�W��
LRESULT C_db_wnd_info_koe::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
	case WM_INITDIALOG:		if (!on_init())					return FALSE;	break;
	case WM_CLOSE:			if (!on_close())				return FALSE;	break;
	case WM_COMMAND:		if (!on_command(wp, lp))		return FALSE;	break;
	case WM_NOTIFY:			if (!on_notify(wp, lp))			return FALSE;	break;
	case WM_LBUTTONDOWN:	if (!on_lbutton_down(wp, lp))	return FALSE;	break;
	case WM_SIZE:			if (!on_size(wp, lp))			return FALSE;	break;
	}

	return C_db_wnd_info::window_proc(msg, wp, lp);
}

// ������
bool C_db_wnd_info_koe::on_init()
{
	// ������
	m_first = true;
	m_click_pos = 0;

	// �o�C���h
	bind_control(&m_stc_file, IDC_STC_KOE, _T("stc_koe"), 8);
	bind_control(&m_progress, IDC_PRG_KOE, _T("prg_koe"), 8);
	bind_control(&m_picture, IDC_PCT_KOE, _T("pct_koe"), 5);
	bind_control(&m_picture_rect, IDC_STC_PCT_KOE_RECT, _T("stc_pct_koe_rect"), 5);
	bind_control(&m_stc_koe_length, IDC_STC_KOE_LENGTH, _T("stc_koe_length"), 3);
	bind_control(&m_stc_koe_now, IDC_STC_KOE_NOW, _T("stc_koe_now"), 2);
	bind_control(&m_button_copy, IDC_BTN_COPY, _T("btn_copy"), 1);
	bind_control(&m_chk_copy, IDC_CHK_COPY, _T("chk_copy"), 9);
	
	// �s�N�`���R���g���[���̃f�t�H���g�̃T�C�Y��ݒ�
	m_picture.set_def_rltv_rect(m_picture_rect.get_def_rltv_rect());

	// �E�B���h�E��`�̓ǂݍ���
	load_ini_window_rect();
	m_chk_copy.load_ini(false);

	// �E�B���h�E�\��
	update();
	set_show(true);

	// WM_SIZE �𑗂�
	send_wm_size();

	return true;
}

// ����
bool C_db_wnd_info_koe::on_close()
{
	// �E�B���h�E��`�̕ۑ�
	m_chk_copy.save_ini();
	save_ini_window_rect();

	return true;
}


// �R�}���h
bool C_db_wnd_info_koe::on_command(WPARAM wp, LPARAM lp)
{
	int ctrl_id = LOWORD(wp);
	if (ctrl_id == IDC_BTN_COPY)
	{
		::str_copy_to_clip_board_in_MB(tostr(m_click_pos));
	}

	return true;
}

// �ʒm
bool C_db_wnd_info_koe::on_notify(WPARAM wp, LPARAM lp)
{
	return true;
}

// �T�C�Y���ύX���ꂽ
bool C_db_wnd_info_koe::on_size(WPARAM wp, LPARAM lp)
{
	update_view();

	return true;
}

// ���N���b�N
bool C_db_wnd_info_koe::on_lbutton_down(WPARAM wp, LPARAM lp)
{
	C_rect rect = m_picture.get_rltv_rect();

	m_click_pos = (int)((double)(m_ld.mills_max) * (LOWORD(lp) - rect.left) / rect.width());
	m_click_pos = limit(0, m_click_pos, m_ld.mills_max);

	m_stc_koe_now.set_text(_T("�د��ʒu = ") + tostr(m_click_pos) + _T(" �ؕb"));

	tnm_play_koe(m_ld.koe_no, m_ld.chara_no, TNM_JITAN_RATE_NORMAL, m_click_pos, false, false, false, true, false);

	if (m_chk_copy.get_check())
	{
		::str_copy_to_clip_board_in_MB(tostr(m_click_pos));
	}

	return true;
}

// ****************************************************************
// �X�e�[�^�X�ǂݍ���
// ================================================================
void C_db_wnd_info_koe::load_state()
{
}

// ****************************************************************
// �X�e�[�^�X�ۑ�
// ================================================================
void C_db_wnd_info_koe::save_state()
{
	C_ini_file::body().save_bool(_T("copy_by_click"), m_chk_copy.get_check(), REGIST_WINDOW_NAME);
}

// ****************************************************************
// �t���[������
// ================================================================
void C_db_wnd_info_koe::frame()
{
	S_param cd;

	if (!m_h_wnd)
		return;

	// �p�����[�^���擾
	cd.koe_no = Gp_sound->m_koe.get_koe_no();
	cd.chara_no = Gp_sound->m_koe.get_chara_no();

	if (m_first)
	{
		cd.mills_max = Gp_sound->m_koe.get_length_by_millsecond();
		cd.mills_now = Gp_sound->m_koe.get_play_pos_by_millsecond();
		cd.prg_max = Gp_sound->m_koe.get_length() / 1024;
		cd.prg_now = Gp_sound->m_koe.get_play_pos() / 1024;
	}
	else
	{
		int mills_max = Gp_sound->m_koe.get_length_by_millsecond();
		int mills_now = Gp_sound->m_koe.get_play_pos_by_millsecond();
		int prg_max = Gp_sound->m_koe.get_length() / 1024;
		int prg_now = Gp_sound->m_koe.get_play_pos() / 1024;

		if (cd.koe_no == m_ld.koe_no && cd.chara_no == m_ld.chara_no && m_ld.prg_max > 0 && prg_max == 0)
		{
			cd.mills_max = m_ld.mills_max;		// �O��̒l���g��
			cd.mills_now = m_ld.mills_max;		// �O��̍ő���g���i�Ō�܂ōĐ����ꂽ�Ƃ݂Ȃ��j
			cd.prg_max = m_ld.prg_max;			// �O��̒l���g��
			cd.prg_now = m_ld.prg_max;			// �O��̍ő���g���i�Ō�܂ōĐ����ꂽ�Ƃ݂Ȃ��j
		}
		else
		{
			cd.mills_max = mills_max;
			cd.mills_now = mills_now;
			cd.prg_max = prg_max;
			cd.prg_now = prg_now;
		}
	}

	// ���t�@�C��
	if (m_first || cd.koe_no != m_ld.koe_no || cd.chara_no != m_ld.chara_no)
	{
		if (cd.koe_no >= 0)
		{
			if (cd.chara_no >= 0)
			{
				m_stc_file.set_text(str_format(_T("KOE(%08d,%03d)"), cd.koe_no, cd.chara_no));
			}
			else
			{
				m_stc_file.set_text(str_format(_T("KOE(%08d)"), cd.koe_no));
			}

			update_view();
		}
		else
		{
			m_stc_file.clear_text();
		}
	}

	// �v���O���X
	if (m_first || cd.prg_max != m_ld.prg_max || cd.prg_now != m_ld.prg_now)
	{
		m_progress.set_max_prg(cd.prg_max + 1);
		m_progress.set_prg(cd.prg_now + 1);
		m_progress.set_prg(cd.prg_now);
		m_progress.set_max_prg(cd.prg_max);

		m_stc_koe_length.set_text(tostr(cd.mills_now) + _T(" / ") + tostr(cd.mills_max) + _T(" �ؕb"));
	}

	// ����̏���ۑ�
	m_ld = cd;

	// ����t���O���~�낷
	m_first = false;
}

// ****************************************************************
// �r���[���X�V����
// ================================================================
void C_db_wnd_info_koe::update_view()
{
	int cw = m_picture.get_client_rect().width();
	int ch = m_picture.get_client_rect().height();
	if (cw > 0 && ch > 0)
	{
		// �s�N�`�����쐬
		m_bitmap.create(cw, ch, 32);
		m_bitmap.clear_color(C_argb(255, 0, 0, 255));

		// �X�g���[�����擾
		C_sound_stream_base* p_stream = Gp_sound->m_koe.get_stream(0);
		if (p_stream)
		{
			int sample_cnt = p_stream->get_sample_cnt();
			int byte = p_stream->get_bit_cnt_per_sample() / 8;
			int channnel = p_stream->get_channel_cnt();

			// �f�[�^�ւ̃|�C���^���擾
			BYTE* p = p_stream->get_ptr();
			if (p)
			{
				for (int x = 0; x < cw; x++)
				{
					int first_sample = sample_cnt * x / cw;
					int last_sample = sample_cnt * (x + 1) / cw;
					int cnt = last_sample - first_sample;

					if (byte == 1)
					{
						int max_value = 0;

						for (int i = 0; i < cnt; i++)
						{
							max_value = std::max(abs(*(signed char *)p), max_value);

							p += channnel;
						}

						m_bitmap.draw_rect(x, ch / 2 - max_value * ch / 256, 1, max_value * ch / 128, C_argb(255, 0, 255, 0), true, false);
					}
					else if (byte == 2)
					{
						int max_value = 0;

						for (int i = 0; i < cnt; i++)
						{
							max_value = std::max(abs(*(short int *)p), max_value);

							p += channnel * 2;
						}

						m_bitmap.draw_rect(x, ch / 2 - max_value * ch / 65536, 1, max_value * ch / 32768, C_argb(255, 0, 255, 0), true, false);
					}
				}
			}
		}

		m_picture.set_bitmap(m_bitmap.get_handle());
	}
}

