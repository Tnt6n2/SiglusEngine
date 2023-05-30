#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_def.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_input.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_mov.h"
#include	"engine/eng_dir.h"
#include	"engine/eng_input.h"
#include	"engine/eng_config.h"
#include	"engine/ifc_error.h"
#include	"dialog/cfg_wnd_modal_movtype.h"


// ****************************************************************
// ���[�r�[�Fwmp �ł̍Đ��� ATL ���g���܂�
// ================================================================
#define _ATL_APARTMENT_THREADED

#include	<atlbase.h>
#include	<atlhost.h>
#include	<lzss.h>

#include	"wmp.h"

// �����o�ϐ��ɂ������Ƃ���c�ł��w�b�_���c��オ��̂łƂ肠���������ɁB
// C_elm_mov �̃C���X�^���X�𕡐���������΂��I
// impl �p�^�[�����g���ĉB������ׂ������ˁB
static	CAxWindow					Gwmp_wndView = 0;
static	CComPtr<IAxWinHostWindow>	Gwmp_spHost = 0;
static	CComPtr<IWMPPlayer>			Gwmp_spWMPPlayer = 0;
static	CComPtr<IWMPPlayer2>		Gwmp_spWMPPlayerE = 0;
static	CComPtr<IWMPControls>		Gwmp_spWMPControls = 0;
static	CComPtr<IWMPSettings>		Gwmp_spWMPSettings = 0;

const int TNM_MCI_VOLUME_MAX = 1000;
const int TNM_WMP_VOLUME_MAX = 100;

// ****************************************************************
// ���[�r�[�F������
// ================================================================
void C_elm_mov::init(S_element element, int form, CTSTR& name)
{
	// ���̏�����
	C_tnm_element::init(element, form, name, NULL);

	// �p�����[�^�̏�����
	m_method = -1;

	// wmp �p�����[�^
	m_volume = 255;
	m_wmp_ready = false;
}

// ****************************************************************
// ���[�r�[�F�ď�����
// ================================================================
void C_elm_mov::reinit()
{
	close();

	m_method = -1;
	m_wmp_ready = false;
}

// ****************************************************************
// ���[�r�[�F���
// ================================================================
void C_elm_mov::free()
{
	// ����
	close();
}

// ****************************************************************
// ���[�r�[�F�Z�[�u
// ================================================================
void C_elm_mov::save(C_tnm_save_stream& stream)
{
	stream.save(m_param.file_name);
}

// ****************************************************************
// ���[�r�[�F���[�h
// ================================================================
void C_elm_mov::load(C_tnm_save_stream& stream)
{
	stream.load(m_param.file_name);

	// �č\�z
	restruct();
}

// ****************************************************************
// ���[�r�[�F�č\�z
// ================================================================
void C_elm_mov::restruct()
{
	int err = 0;
	int movie_type = 0;

	// �t�@�C��������Ȃ�I��
	if (m_param.file_name.empty())
		return;

	// �t�@�C����T��
	TSTR file_path = tnm_find_mov(Gp_dir->base_dir, _T("mov"), m_param.file_name, &movie_type);
	if (file_path.empty())
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���[�r�[�t�@�C�� ") + m_param.file_name + _T(" ��������܂���ł����B"));
		return;
	}

	// ���[�r�[�Đ����ɗ������ꍇ�̕⏕����
	if (check_file_playing())	{

		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���[�r�[�̍Đ����ɗ��������̕⏕�������s���܂��B"));

		// �T�|�[�g�_�C�A���O�ōĐ����@��ύX����
		Ccfg_wnd_modal_movtype config_mov_wnd;
		bool res = config_mov_wnd.open_modal_result_bool();

		// �u�Đ����Ȃ��v��I�񂾏ꍇ�͍Đ����Ȃ�
		if (!res)	{
			close();
			return;
		}
	}

	// ���݂̃��\�b�h���w��
	m_method = Gp_system_config->movie_type;

	// �Đ����������t�@�C�����쐬����
	create_file_playing();

	tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���[�r�[�̍Đ����J�n���܂��B"));

	if (false);

	// mci �ɂ��Đ�
	else if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{

		TSTR file_type_str;
		switch (movie_type)	{
		case TNM_MOVIE_FILE_TYPE_AVI:	file_type_str = _T("avivideo");		break;
		case TNM_MOVIE_FILE_TYPE_MPG:	file_type_str = _T("MPEGVideo");	break;
		case TNM_MOVIE_FILE_TYPE_WMV:	file_type_str = _T("MPEGVideo");	break;
		default:						file_type_str = _T("MPEGVideo");	break;
		}

		// ���[�r�[���J��
		err = mciSendString(str_format(_T("open \"%s\" type %s alias SiglusMCIMovie wait"), file_path.c_str(), file_type_str.c_str()).c_str(), NULL, 0, NULL);
		if (err)	{

			TCHAR err_str[256];
			mciGetErrorString(err, err_str, 256);
			tnm_set_error(TNM_ERROR_TYPE_FATAL, file_path + _T("�̃I�[�v���Ɏ��s���܂����B\n\n��ڍׁ�\n") + err_str);
		}

		else	{

			// ���[�r�[�E�B���h�E���쐬����i�����ł͂܂��\������܂���B�j
			if (!Gp_movie_wnd->create())	{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���[�r�[�E�B���h�E�̍쐬�Ɏ��s���܂����B"));
				return;
			}

			// ���[�r�[�E�B���h�E�𖳌��ɂ���i�}�E�X�J�[�\����������Ȃ����邽�߁j
			Gp_movie_wnd->set_enable(false);

			// ���[�r�[���Đ�����E�B���h�E���w�肷��
			err = mciSendString((_T("window SiglusMCIMovie handle ") + tostr((UINT)Gp_movie_wnd->get_handle()) + _T(" wait")).c_str(), NULL, 0, NULL);
			if (err)	{

				TCHAR err_str[256];
				mciGetErrorString(err, err_str, 256);
				tnm_set_error(TNM_ERROR_TYPE_FATAL, TSTR() + _T("���[�r�[���E�B���h�E�Ɋ��蓖�Ă�̂Ɏ��s���܂����B") + err_str);
			}
			else	{
				C_point pos = m_emp.get_calc_pos();
				C_size size = m_emp.get_calc_size();

				// �E�B���h�E�̈ʒu�ƃT�C�Y��ύX����
				Gp_movie_wnd->set_window_rect(C_rect::by_size(pos, size));

				// ���[�r�[�̃T�C�Y���w�肷��
				err = mciSendString(str_format(_T("put SiglusMCIMovie destination at 0 0 %d %d wait"), size.cx, size.cy).c_str(), NULL, 0, NULL);
				if (err)	{

					TCHAR err_str[256];
					mciGetErrorString(err, err_str, 256);
					tnm_set_error(TNM_ERROR_TYPE_FATAL, TSTR() + _T("���[�r�[�̈ʒu�̐ݒ�Ɏ��s���܂����B") + err_str);
				}
				else	{

					// ���[�r�[���Đ�����
					err = mciSendString(_T("play SiglusMCIMovie"), NULL, 0, NULL);
					if (err)	{

						TCHAR err_str[256];
						mciGetErrorString(err, err_str, 256);
						tnm_set_error(TNM_ERROR_TYPE_FATAL, TSTR() + _T("���[�r�[�̍Đ��Ɏ��s���܂����B") + err_str);
					}
					else	{

						// �{�����[���𒲐�����
						int mci_volume = m_volume * TNM_MCI_VOLUME_MAX / 255;
						err = mciSendString(str_format(_T("setaudio SiglusMCIMovie left volume to %d"), mci_volume).c_str(), NULL, 0, NULL);
						err = mciSendString(str_format(_T("setaudio SiglusMCIMovie right volume to %d"), mci_volume).c_str(), NULL, 0, NULL);
						if (err)	{

							TCHAR err_str[256];
							mciGetErrorString(err, err_str, 256);
							tnm_set_error(TNM_ERROR_TYPE_FATAL, TSTR() + _T("���[�r�[�̍Đ��Ɏ��s���܂����B") + err_str);
						}
						else	{

							// �E�B���h�E��\������
							Gp_movie_wnd->set_show(true);
							tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���[�r�[�̍Đ����J�n���܂����B"));
						}
					}
				}
			}
		}
	}

	// wmp �ɂ��Đ�
	else if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{

		C_point pos = m_emp.get_calc_pos();
		C_size size = m_emp.get_calc_size();
		C_rect rect = C_rect::by_size(pos, size);

		//Gwmp_wndView.Create(G_app.h_wnd, rect, NULL, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0);				// wmp �E�B���h�E�쐬
		//Gwmp_wndView.Create(G_app.h_wnd, C_rect(0, 0, rect.width() - 10, rect.height() - 10), NULL, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0);	// wmp �E�B���h�E�쐬
		Gwmp_wndView.Create(G_app.h_wnd, C_rect(0, 0, rect.width() - 10, rect.height() - 10), NULL, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0);	// wmp �E�B���h�E�쐬

		// �� -10 ���āH
		// �����I����A�m���Ƀ��T�C�Y���N�������߁Arect �Ƃ͈Ⴄ�l������K�v������
		// (0, 0, 0, 0) ���ƁA������ɂȂ񂩔��g���\�������B��B
		// �Ƃ肠���� -10 ����Ă݂����肭���������炱��ł�����B

		if (Gwmp_wndView.m_hWnd != NULL){

			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���[�r�[�E�B���h�E���쐬���܂����B"));
			int wmp_volume = m_volume * TNM_WMP_VOLUME_MAX / 255;

			int res = 0;
			EnableWindow(Gwmp_wndView.m_hWnd, FALSE);										// wmp �E�B���h�E��ł̃N���b�N���֎~
			res = Gwmp_wndView.QueryHost(&Gwmp_spHost);										// �z�X�g�N�G��
			if(res >= 0){res = Gwmp_spHost->CreateControl(CComBSTR(_T("{6BF52A52-394A-11d3-B153-00C04F79FAA6}")), Gwmp_wndView, 0);}		// �R���g���[���쐬
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���[�r�[�̃R���g���[�����쐬���܂����B"));
			if(res >= 0){res = Gwmp_wndView.QueryControl(&Gwmp_spWMPPlayer);}				// �R���g���[���N�G��
			if(res >= 0){res = Gwmp_spWMPPlayer.QueryInterface(&Gwmp_spWMPPlayerE);}		// ��ʃv���C���[�N�G��
			if(res >= 0){res = Gwmp_spWMPPlayerE->put_uiMode(A2WBSTR("none"));}				// ���[�U�[�C���^�[�t�F�C�X�֎~
			if(res >= 0){res = Gwmp_spWMPPlayerE->put_enableContextMenu(FALSE);}			// �R���e�L�X�g���j���[�֎~
			if(res >= 0){res = Gwmp_spWMPPlayerE->put_enabled(FALSE);}						// �R���g���[�����֎~
			if(res >= 0){res = Gwmp_spWMPPlayerE->put_stretchToFit(TRUE);}					// �E�B���h�E�T�C�Y�Ƀt�B�b�g������
			if(res >= 0){res = Gwmp_spWMPPlayerE->get_controls(&Gwmp_spWMPControls);}		// �R���g���[���擾
			if(res >= 0){res = Gwmp_spWMPPlayerE->get_settings(&Gwmp_spWMPSettings);}		// �Z�b�e�B���O�擾
			if(res >= 0){res = Gwmp_spWMPSettings->put_volume(wmp_volume);}					// ���ʂ�ݒ�
			tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���[�r�[�̊e��ݒ���s���܂����B"));
			if(res >= 0){res = Gwmp_spWMPPlayerE->put_URL(OLE2BSTR(file_path.c_str()));}	// �Đ�����t�@�C�����֘A�t����
			if(res >= 0){

				// �����t���O�𗧂Ă�
				m_wmp_ready = true;
				tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���[�r�[�t�@�C�����J���܂����B"));
			}
		}
	}

	// ���[�r�[���Đ��ł��Ă��Ȃ���Β�~
	if (get_state() != TNM_MOVIE_PLAY_STATE_PLAY)	{
		close();

		tnm_set_error(TNM_ERROR_TYPE_DEBUG, _T("���[�r�[���Đ��ł��܂���ł����B"));
	}
}

// ****************************************************************
// ���[�r�[�F�Đ�
// ================================================================
void C_elm_mov::play(CTSTR& file_name, int px, int py, int sx, int sy)
{
	// �p�����[�^��ݒ�
	m_param.file_name = file_name;
	m_emp.set_pos(px, py);
	m_emp.set_size(sx, sy);

	// �č\�z
	restruct();
}

// ****************************************************************
// ���[�r�[�F����
// ================================================================
void C_elm_mov::close()
{
	// �p�����[�^���N���A
	m_param.file_name.clear();

	// ����
	if (false);

	// mci �ɂ���~
	else if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{

		// �E�B���h�E���\���ɂ���
		Gp_movie_wnd->set_show(false);

		// ��~����
		mciSendString(_T("close SiglusMCIMovie wait"), NULL, 0, NULL);

		// ���[�r�[�E�B���h�E���I������
		Gp_movie_wnd->close();
			
		// �Đ����������t�@�C�����폜����
		delete_file_playing();

		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���[�r�[���~���܂����B"));
	}

	// wmp �ɂ���~
	else if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{

		// �܂��̓E�B���h�E���B��
		//::ShowWindow(Gwmp_wndView.m_hWnd, SW_HIDE);
		//Gwmp_wndView.MoveWindow(0, 0, 0, 0, FALSE);
		//Gwmp_spWMPPlayerE->

		// ���ɒ�~
		if (Gwmp_spWMPControls)	{
			Gwmp_spWMPControls->stop();
		}
		if (Gwmp_spWMPPlayerE)	{
			Gwmp_spWMPPlayerE->close();
		}
		if (Gwmp_spWMPPlayer)	{
			Gwmp_spWMPPlayer->close();
		}
		if (Gwmp_spWMPSettings)	{
			Gwmp_spWMPSettings.Release();
			Gwmp_spWMPSettings = 0;
		}
		if (Gwmp_spWMPControls)	{
			Gwmp_spWMPControls.Release();
			Gwmp_spWMPControls = 0;
		}
		if (Gwmp_spWMPPlayerE)	{
			Gwmp_spWMPPlayerE.Release();
			Gwmp_spWMPPlayerE = 0;
		}
		if (Gwmp_spWMPPlayer)	{
			Gwmp_spWMPPlayer.Release();
			Gwmp_spWMPPlayer = 0;
		}
		if (Gwmp_spHost)	{
			Gwmp_spHost.Release();
			Gwmp_spHost = 0;
		}
		if (Gwmp_wndView)	{
			Gwmp_wndView.DestroyWindow();
			Gwmp_wndView = 0;
		}

		// �Đ����������t�@�C�����폜����
		delete_file_playing();

		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���[�r�[���~���܂����B"));
	}

	// �p�����[�^��������
	m_method = -1;
	m_wmp_ready = false;
}

// ****************************************************************
// ���[�r�[�F�t���[������
// ================================================================
void C_elm_mov::frame()
{
	if (m_method == -1)	{
		return;
	}

	// �Đ����ɂȂ�Ε\������i�������̊Ԃ͕\�����Ȃ��̂ŏ�ɊĎ�����K�v������̂ł��B�j
	if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{
		if (m_wmp_ready)	{
			if (get_wmp_state_detail() == wmppsPlaying)	{

				// ���������I�I�E�B���h�E��\������
				Sleep(100);		// Vista �ň�u�S�~�������΍�Bwmp �̃o�[�W�����̂������Ȃ��H

				Gwmp_wndView.ShowWindow(SW_SHOW);	// �����ŃE�B���h�E��\������
				tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("���[�r�[�̍Đ����J�n���܂����B"));
				m_wmp_ready = false;

				// �����Ń{�����[�����Đݒ肵�Ă݂�i���ʂȂ������̂ł�߂�j
				//long volume = 0;
				//Gwmp_spWMPSettings->get_volume(&volume);
				//Gwmp_spWMPSettings->put_volume(volume - 10);
				//Gwmp_spWMPSettings->put_volume(volume);

				// �����Ń��T�C�Y
				update_rect_org();
			}
		}

		//C_point pos = m_emp.get_calc_pos();
		//C_size size = m_emp.get_calc_size();
		//C_rect rect = C_rect::by_size(pos, size);
		//Gwmp_wndView.ResizeClient(rect.width(), rect.height(), FALSE);

		// ������
		//if (Gp_input->now.keyboard.key[_T('Q')].is_down())	{
			//Gwmp_wndView.ShowWindow(SW_HIDE);
		//}

	}

	// ���[�r�[���I�����Ă���Ε���
	if (get_state() == TNM_MOVIE_PLAY_STATE_STOP)	{
		close();
	}
}

// ****************************************************************
// ���[�r�[�F�\����`��ύX����
// ================================================================
/*
void C_elm_mov::update_rect()
{
	if (false);

	// mci
	else if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{
		C_point pos = Gp_global->total_game_screen_pos;
		C_size size = Gp_global->total_game_screen_size;

		// �E�B���h�E�̈ʒu�ƃT�C�Y��ύX����
		Gp_movie_wnd->set_window_rect(C_rect::by_size(pos, size));

		// ���[�r�[�̃T�C�Y���w�肷��
		MCI_ANIM_RECT_PARMS marp;
		ZeroMemory(&marp, sizeof(marp));
		marp.dwCallback = (PTR)Gapp.h_wnd;
		marp.rc = C_rect(C_point(0, 0), size);	// right,bottom �ɉE�����W�łȂ��T�C�Y�����Ȃ��Ƃ��܂������Ȃ��I�i�ʒu�� 0, 0 �ɂȂ����̂Ŋ֌W�Ȃ����c�j
		mciSendCommand(m_mci_device_id, MCI_PUT, MCI_WAIT | MCI_ANIM_RECT | MCI_ANIM_PUT_DESTINATION, (PTR)&marp);
	}

	// wmp
	else if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{
		if (Gwmp_wndView && Gwmp_wndView.m_hWnd)	{
			C_rect rect = C_rect::by_size(Gp_global->total_game_screen_pos, Gp_global->total_game_screen_size);
			::SetWindowPos(Gwmp_wndView.m_hWnd, 0, rect.left, rect.top, rect.width(), rect.height(), SWP_NOZORDER | SWP_NOACTIVATE);	// �ʒu�Ƒ傫��
		}
	}

	update_rect_org();
}
*/

void C_elm_mov::update_rect_org()
{
	if (false);

	// mci
	else if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{

		//C_point pos = Gp_global->total_game_screen_pos;
		//C_size size = Gp_global->total_game_screen_size;
		C_point pos = m_emp.get_calc_pos();
		C_size size = m_emp.get_calc_size();

		// �E�B���h�E�̈ʒu�ƃT�C�Y��ύX����
		Gp_movie_wnd->set_window_rect(C_rect::by_size(pos, size));

		// ���[�r�[�̃T�C�Y���w�肷��
		//MCI_ANIM_RECT_PARMS marp;
		//ZeroMemory(&marp, sizeof(marp));
		//marp.dwCallback = (PTR)G_app.h_wnd;
		//marp.rc = C_rect(C_point(0, 0), size);	// right,bottom �ɉE�����W�łȂ��T�C�Y�����Ȃ��Ƃ��܂������Ȃ��I�i�ʒu�� 0, 0 �ɂȂ����̂Ŋ֌W�Ȃ����c�j
		//mciSendCommand(m_mci_device_id, MCI_PUT, MCI_WAIT | MCI_ANIM_RECT | MCI_ANIM_PUT_DESTINATION, (PTR)&marp);
		mciSendString(str_format(_T("put SiglusMCIMovie destination at 0 0 %d %d wait"), size.cx, size.cy).c_str(), NULL, 0, NULL);
	}

	// wmp
	else if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{
		if (Gwmp_wndView && Gwmp_wndView.m_hWnd)	{
			C_point pos = m_emp.get_calc_pos();
			C_size size = m_emp.get_calc_size();
			C_rect rect = C_rect::by_size(pos, size);
			//Gwmp_wndView.ResizeClient(rect.width(), rect.height(), TRUE);
			Gwmp_wndView.MoveWindow(&rect, FALSE);
		}
	}
}

void C_elm_mov::change_rect(const C_point& pos, const C_size& size)
{
	m_emp.set_pos_and_size(pos, size);
	update_rect_org();
}

void C_elm_mov::add_rect(const C_point& pos, const C_size& size)
{
	m_emp.add_pos(pos);
	m_emp.add_size(size);
	update_rect_org();
}

void C_elm_mov::set_size_rate(float x, float y)
{
	m_emp.set_size_rate(x, y);
}

void C_elm_mov::set_total_game_screen_pos(const C_point& pos)
{
	m_emp.set_total_game_screen_pos(pos);
}

// ****************************************************************
// ���[�r�[�F�{�����[����ݒ肷��
// ================================================================
void C_elm_mov::set_volume(int volume)
{
	m_volume = volume;

	// mci
	if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{

		int mci_volume = m_volume * TNM_MCI_VOLUME_MAX / 255;
		mciSendString(str_format(_T("setaudio SiglusMCIMovie left volume to %d"), mci_volume).c_str(), NULL, 0, NULL);
		mciSendString(str_format(_T("setaudio SiglusMCIMovie right volume to %d"), mci_volume).c_str(), NULL, 0, NULL);
	}
	// wmp
	else if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{
		if (Gwmp_spWMPSettings)	{

			int wmp_volume = m_volume * TNM_WMP_VOLUME_MAX / 255;
			Gwmp_spWMPSettings->put_volume(wmp_volume);
		}
	}
}

// ****************************************************************
// ���[�r�[�F�Đ����Ȃ�ꎞ��~
// ================================================================
void C_elm_mov::pause()
{
	if (false);	

	// mci
	else if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{

		//MCI_GENERIC_PARMS mgp;
		//ZeroMemory(&mgp, sizeof(mgp));
		//mciSendCommand(m_mci_device_id, MCI_PAUSE, 0, (PTR)&mgp);
		mciSendString(_T("pause SiglusMCIMovie wait"), NULL, 0, NULL);
	}
	// wmp
	else if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{
		if(Gwmp_spWMPControls) {
			Gwmp_spWMPControls->pause();
		}
	}
}

// ****************************************************************
// ���[�r�[�F�ꎞ��~���Ȃ�Đ��ĊJ
// ================================================================
void C_elm_mov::resume()
{
	if (false);	

	// mci
	else if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{

		//MCI_GENERIC_PARMS mgp;
		//ZeroMemory(&mgp, sizeof(mgp));
		//mciSendCommand(m_mci_device_id, MCI_RESUME, 0, (PTR)&mgp);
		mciSendString(_T("resume SiglusMCIMovie wait"), NULL, 0, NULL);
	}
	// wmp
	else if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{
		if(Gwmp_spWMPControls) {
			Gwmp_spWMPControls->play();
		}
	}
}

// ****************************************************************
// ���[�r�[�F��Ԃ��擾
// ================================================================
int C_elm_mov::get_state()
{
	if (false);

	// mci
	else if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{

		//MCI_STATUS_PARMS msp;
		//ZeroMemory(&msp, sizeof(msp));
		//msp.dwItem = MCI_STATUS_MODE;
		//mciSendCommand(m_mci_device_id, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (PTR)&msp);

		TCHAR mci_buf[256] = {0};
		mciSendString(_T("status SiglusMCIMovie mode wait"), mci_buf, 256, NULL);

		return convert_play_state(mci_buf);
	}

	// wmp
	else if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{

		if (Gwmp_spWMPPlayerE)	{
			WMPPlayState state;
			int res = Gwmp_spWMPPlayerE->get_playState(&state);
			if (res >= 0){
				return convert_play_state(state);
			}
		}
		return TNM_MOVIE_PLAY_STATE_STOP;
	}

	// �Đ����Ă��Ȃ�
	else	{

		return TNM_MOVIE_PLAY_STATE_STOP;
	}
}

// ****************************************************************
// ���[�r�[�FMCI,WMP�`���̍Đ���Ԃ�Ǝ��`���ɕϊ�����
// ================================================================
int C_elm_mov::convert_play_state(CTSTR& str)
{
	int ret = TNM_MOVIE_PLAY_STATE_STOP;

	if (false);

	// mci
	else if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{

		if (false);
		else if (str.substr(0, 9) == _T("not ready"))	ret = TNM_MOVIE_PLAY_STATE_STOP;
		else if (str.substr(0, 7) == _T("stopped"))		ret = TNM_MOVIE_PLAY_STATE_STOP;
		else if (str.substr(0, 7) == _T("playing"))		ret = TNM_MOVIE_PLAY_STATE_PLAY;
		else if (str.substr(0, 6) == _T("paused"))		ret = TNM_MOVIE_PLAY_STATE_PAUSE;
	}

	return ret;
}

int C_elm_mov::convert_play_state(int state)
{
	int retv = TNM_MOVIE_PLAY_STATE_STOP;

	if (false);

	// mci
	else if (m_method == TNM_MOVIE_PLAY_TYPE_MCI)	{
		//MCI_MODE_NOT_READY
		//MCI_MODE_STOP     
		//MCI_MODE_PLAY     
		//MCI_MODE_RECORD   
		//MCI_MODE_SEEK     
		//MCI_MODE_PAUSE    
		//MCI_MODE_OPEN     
		switch (state) {
			case MCI_MODE_STOP:  retv = TNM_MOVIE_PLAY_STATE_STOP;	break;
			case MCI_MODE_PLAY:  retv = TNM_MOVIE_PLAY_STATE_PLAY;	break;
			case MCI_MODE_PAUSE: retv = TNM_MOVIE_PLAY_STATE_PAUSE;	break;
		}
	}
	// wmp
	else if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{
		//wmppsUndefined	= 0,
		//wmppsStopped	= wmppsUndefined + 
		//wmppsPaused	= wmppsStopped + 1,
		//wmppsPlaying	= wmppsPaused + 1,
		//wmppsScanForward	= wmppsPlaying 
		//wmppsScanReverse	= wmppsScanForw
		//wmppsBuffering	= wmppsScanReverse 
		//wmppsWaiting	= wmppsBuffering + 
		//wmppsMediaEnded	= wmppsWaiting + 1,
		//wmppsTransitioning	= wmppsMediaEnd
		//wmppsReady	= wmppsTransitioning + 
		//wmppsReconnecting	= wmppsReady + 
		//wmppsLast	= wmppsReconnecting + 1
		switch (state) {
			case wmppsStopped:			retv = TNM_MOVIE_PLAY_STATE_STOP;	break;
			case wmppsPlaying:			retv = TNM_MOVIE_PLAY_STATE_PLAY;	break;
			case wmppsPaused:			retv = TNM_MOVIE_PLAY_STATE_PAUSE;	break;
			case wmppsTransitioning:	retv = TNM_MOVIE_PLAY_STATE_PLAY;	break;	// STATE���₷�ׂ�����
			case wmppsWaiting:			retv = TNM_MOVIE_PLAY_STATE_PLAY;	break;	// STATE���₷�ׂ�����
		}
	}

	return retv;
}

// ****************************************************************
// ���[�r�[�Fwmp �̃X�e�[�g�ڍׂ��擾����
// ================================================================
int C_elm_mov::get_wmp_state_detail()
{
	// wmp �̏ꍇ�̂ݏ���
	if (m_method == TNM_MOVIE_PLAY_TYPE_WMP)	{
		if (Gwmp_spWMPPlayerE)	{

			WMPPlayState state;
			Gwmp_spWMPPlayerE->get_playState(&state);
			return (int)state;
		}
	}

	return -1;
}

// ****************************************************************
// ���[�r�[�F�Đ������𔻒肷��
// ================================================================
//		���������u�Đ����v�Ɋ܂݂܂��B
// ================================================================
bool C_elm_mov::is_playing()
{	
	int state = get_state();
	return state == TNM_MOVIE_PLAY_STATE_PLAY || state == TNM_MOVIE_PLAY_STATE_PAUSE;
}

// ****************************************************************
// ���[�r�[�F���������𔻒肷��
// ================================================================
bool C_elm_mov::is_readying()
{
	if (get_wmp_state_detail() == (int)wmppsTransitioning)
		return true;

	return false;
}

// ****************************************************************
// ���[�r�[�F�{�����[�����擾����
// ================================================================
int C_elm_mov::get_volume()
{
	return m_volume;
}
int C_elm_mov::get_volume_real()
{
	if (Gwmp_spWMPSettings)	{
		long volume = 0;
		Gwmp_spWMPSettings->get_volume(&volume);
		return volume;
	}
	return 0;
}

// ****************************************************************
// ���[�r�[�F�Đ����������t�@�C�����쐬����
// ================================================================
void C_elm_mov::create_file_playing()
{
	C_file::write_full_text_in_MB(Gp_dir->get_movie_playing_file_path(), _T("playing"));
}

// ****************************************************************
// ���[�r�[�F�Đ����������t�@�C�����폜����
// ================================================================
void C_elm_mov::delete_file_playing()
{
	filesys_delete_file(Gp_dir->get_movie_playing_file_path());
}

// ****************************************************************
// ���[�r�[�F�Đ����������t�@�C�����`�F�b�N����
// ================================================================
bool C_elm_mov::check_file_playing()
{
	int res = file_get_type(Gp_dir->get_movie_playing_file_path());
	if (res == 1)
		return true;	// �t�@�C�������݂���

	return false;
}

// ****************************************************************
// ���[�r�[�p�����[�^
// ================================================================
const C_size C_elm_mov_param::DEF_MOV_SIZE = C_size(640, 480);

C_elm_mov_param::C_elm_mov_param()
:m_pos(C_point(0, 0))
,m_size(DEF_MOV_SIZE)
,m_size_rate(1.0f, 1.0f)
,m_total_game_screen_pos(0, 0)
{
}

C_elm_mov_param::C_elm_mov_param(const C_point& pos, const C_size& size)
:m_pos(pos)
,m_size(size)
,m_size_rate(1.0f, 1.0f)
,m_total_game_screen_pos(0, 0)
{
}

void C_elm_mov_param::add_pos(const C_point& add)
{
	m_pos += add;
}
void C_elm_mov_param::add_pos(int x, int y)
{
	m_pos.x += x;
	m_pos.y += y;
}
void C_elm_mov_param::add_size(const C_size& add)
{
	m_size.cx += add.cx;
	m_size.cy += add.cy;
}
void C_elm_mov_param::add_size(int x, int y)
{
	m_size.cx += x;
	m_size.cy += y;
}

// �A�N�Z�T���\�b�h
void C_elm_mov_param::set_pos(const C_point& pos)
{
	m_pos = pos;
}
void C_elm_mov_param::set_pos(int x, int y)
{
	m_pos.x = x;
	m_pos.y = y;
}
void C_elm_mov_param::set_size(const C_size& size)
{
	m_size = size;
}
void C_elm_mov_param::set_size(int x, int y)
{
	m_size.cx = x;
	m_size.cy = y;
}
void C_elm_mov_param::set_pos_and_size(const C_point& pos, const C_size& size)
{
	m_pos = pos;
	m_size = size;
}
void C_elm_mov_param::set_pos_and_size(int px, int py, int sx, int sy)
{
	m_pos.x = px;
	m_pos.y = py;
	m_size.cx = sx;
	m_size.cy = sy;
}

void C_elm_mov_param::set_size_rate(float rate_x, float rate_y)
{
	m_size_rate.x = rate_x;
	m_size_rate.y = rate_y;
}

void C_elm_mov_param::set_total_game_screen_pos(const C_point& pos)
{
	m_total_game_screen_pos = pos;
}

C_point C_elm_mov_param::get_pos()
{
	return m_pos;
}
C_size C_elm_mov_param::get_size()
{
	return m_size;
}

D3DXVECTOR2 C_elm_mov_param::get_size_rate()
{
	return m_size_rate;
}

C_point C_elm_mov_param::get_calc_pos()
{
	C_point retv = m_pos;

	// �g�k�T�C�Y�䗦���|����
	retv.x = (LONG)(retv.x * m_size_rate.x) + m_total_game_screen_pos.x;
	retv.y = (LONG)(retv.y * m_size_rate.y) + m_total_game_screen_pos.y;

	return retv;
}

C_size C_elm_mov_param::get_calc_size()
{
	C_size retv = m_size;

	// �g�k�T�C�Y�䗦���|����
	retv.cx = (LONG)(retv.cx * m_size_rate.x);
	retv.cy = (LONG)(retv.cy * m_size_rate.y);

	return retv;
}



// ****************************************************************
// ���[�r�[�E�B���h�E�F
// ================================================================
LRESULT C_tnm_movie_wnd::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	// ���͊֌W�̃��b�Z�[�W����
	Gp_input->set_state_by_wm(msg, wp, lp);

	switch (msg)	{
		case WM_PAINT:		if (!on_paint())	return FALSE;	break;
	}

	return C_window::window_proc(msg, wp, lp);
}

bool C_tnm_movie_wnd::create()
{
	C_point pos = Gp_global->total_game_screen_pos;
	C_size size = Gp_global->total_game_screen_size;
	if (!C_window::create(_T("siglus_engine_movie_wnd"), _T("siglus_engine_movie_wnd"), _T(""), pos.x, pos.y, size.cx, size.cy, -1, -1, 0, WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, false, false, Gp_wnd->get_handle()))	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("���[�r�[�E�B���h�E�̍\�z�Ɏ��s���܂����B\n") + get_last_error_msg());
		return false;
	}

	return true;
}

bool C_tnm_movie_wnd::on_paint()
{
	PAINTSTRUCT ps;
	HDC dc = BeginPaint(m_h_wnd, &ps);
	EndPaint(m_h_wnd, &ps);

	return true;
}

