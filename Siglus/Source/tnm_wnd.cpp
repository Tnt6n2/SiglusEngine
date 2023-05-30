#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_input.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_icon.h"
#include	"engine/eng_init.h"
#include	"engine/eng_input.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_msgbox.h"
#include	"engine/eng_display.h"
#include	"engine/eng_etc.h"
#include	"../resource.h"

// ****************************************************************
// �E�B���h�E�v���V�[�W��
// ================================================================
LRESULT C_tnm_wnd::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	// ���͊֌W�̃��b�Z�[�W����
	Gp_input->set_state_by_wm(msg, wp, lp);

	switch (msg)
	{
		case WM_CREATE:			if (!on_create())				return FALSE;	break;
		case WM_CLOSE:			if (!on_close())				return FALSE;	break;
		case WM_PAINT:			if (!on_paint())				return FALSE;	break;
		case WM_COMMAND:		if (!on_command(wp, lp))		return FALSE;	break;
		case WM_SETCURSOR:		if (!on_set_cursor(wp, lp))		return FALSE;	break;

//		case WM_MENUSELECT:		m_wnd_menu.update();	break;		// ���j���[���X�V�i�����j���[�̑I�𒆂ɑI�����Ă��鍀�ڂ��ς��x�ɑ����Ă���̂œK�؂ł͂Ȃ��j
		case WM_INITMENU:
			if ((HMENU)wp == m_wnd_menu.get_handle())	{
				m_wnd_menu.update();		// ���j���[���X�V
			}
			break;

		case WM_SYSKEYDOWN:

			// F10 �̓f�t�H���g�̏����ɉ񂳂Ȃ�
			if (wp == VK_F10)	{
				return FALSE;
			}
			//if (wp == VK_RETURN)	{
			//	return 0;	// �x������炳�Ȃ�
			//}
			break;

		case WM_SYSCOMMAND:

			// �V�X�e���R�}���h�ɓ������ꍇ�͎��Ԃ��~�߂�
			if (!Gp_global->alt_menu_wait_flag)	{
				Gp_global->alt_menu_wait_flag = true;
				// �������߂�ƁA�t���[�������Ŏ����I�� false �ɖ߂�
			}

			// �����蒆��I�[�g���[�h���̓X�N���[���Z�[�o���N�������Ȃ�
			if (wp == SC_SCREENSAVE && (tnm_is_skipping() || tnm_is_auto_mode()))	{
				return 1;	// �X�N���[���Z�[�o���N�������Ȃ��ꍇ�� 1 ��Ԃ�
			}
			break;

#if 0	

			// �ȉ��A�����R�[�h
			// ��𑜓x�̃��j�^�� Aero �X�i�b�v����ƁA�E�B���h�E�T�C�Y���␳����A�A�X�y�N�g��Ƃ����ς���Ă��炢���ƂɂȂ�
			// �ǂ��ɂ����������AWM_SIZE �������ł��Ȃ���ɁA���j���[���O�����Ƃ��� WM_SIZE �ȂǂƋ�ʂł����c
			// ����������߂�B

		case WM_SIZE:
			info_box(tostr(LOWORD(lp)) + _T(",") + tostr(HIWORD(lp)) + _T(",") + 
				(wp == SIZE_MAXHIDE ? _T("SIZE_MAXHIDE")
				: wp == SIZE_MAXIMIZED ? _T("SIZE_MAXIMIZED")
				: wp == SIZE_MAXSHOW ? _T("SIZE_MAXSHOW")
				: wp == SIZE_MINIMIZED ? _T("SIZE_MINIMIZED")
				: wp == SIZE_RESTORED ? _T("SIZE_RESTORED") : _T("???")));
			break;
#endif

		case WM_EXITSIZEMOVE:
		{
			// �T�C�Y�ύX�������ꍇ�͋����Ńt���[�Ɉڍs���܂�
			tnm_change_screen_size_mode_free(get_window_rect().left_top(), get_client_rect().size(), false);

			break;
		}

		case WM_ENTERSIZEMOVE:
		{
			// �T�C�Y�ύX�O�̃E�B���h�E�T�C�Y�ƃN���C�A���g�T�C�Y�̍������擾���Ă���
			m_diff.cx = get_window_rect().width() - get_client_rect().width();
			m_diff.cy = get_window_rect().height() - get_client_rect().height();
			break;
		}

		case WM_SIZING:
		{
			C_size old_client_size;
			C_size new_client_size;

			C_rect* screen_rect = (C_rect *)lp;
			old_client_size.cx = screen_rect->width() - m_diff.cx;
			old_client_size.cy = screen_rect->height() - m_diff.cy;

			switch (wp)
			{
				case WMSZ_TOPLEFT:
				{
					float x_rate = (float)old_client_size.cx / Gp_ini->screen_size.cx;
					float y_rate = (float)old_client_size.cy / Gp_ini->screen_size.cy;
					float rate = std::max(x_rate, y_rate);
					new_client_size.cx = std::max((int)ceil(Gp_ini->screen_size.cx * rate), 16);
					new_client_size.cy = std::max((int)ceil(Gp_ini->screen_size.cy * rate), 16);

					screen_rect->left = screen_rect->left - (new_client_size.cx - old_client_size.cx);
					screen_rect->top = screen_rect->top - (new_client_size.cy - old_client_size.cy);
					break;
				}
				case WMSZ_TOP:
				{
					float rate = (float)old_client_size.cy / Gp_ini->screen_size.cy;
					new_client_size.cx = std::max((int)ceil(Gp_ini->screen_size.cx * rate), 16);
					new_client_size.cy = std::max((int)ceil(Gp_ini->screen_size.cy * rate), 16);

					screen_rect->left = screen_rect->left - (new_client_size.cx - old_client_size.cx) / 2;
					screen_rect->right = screen_rect->right + (new_client_size.cx - old_client_size.cx) / 2;
					screen_rect->top = screen_rect->top - (new_client_size.cy - old_client_size.cy);
					break;
				}
				case WMSZ_TOPRIGHT:
				{
					float x_rate = (float)old_client_size.cx / Gp_ini->screen_size.cx;
					float y_rate = (float)old_client_size.cy / Gp_ini->screen_size.cy;
					float rate = std::max(x_rate, y_rate);
					new_client_size.cx = std::max((int)ceil(Gp_ini->screen_size.cx * rate), 16);
					new_client_size.cy = std::max((int)ceil(Gp_ini->screen_size.cy * rate), 16);

					screen_rect->right = screen_rect->right + (new_client_size.cx - old_client_size.cx);
					screen_rect->top = screen_rect->top - (new_client_size.cy - old_client_size.cy);
					break;
				}
				case WMSZ_LEFT:
				{
					float rate = (float)old_client_size.cx / Gp_ini->screen_size.cx;
					new_client_size.cx = std::max((int)ceil(Gp_ini->screen_size.cx * rate), 16);
					new_client_size.cy = std::max((int)ceil(Gp_ini->screen_size.cy * rate), 16);

					screen_rect->left = screen_rect->left - (new_client_size.cx - old_client_size.cx);
					screen_rect->bottom = screen_rect->bottom + (new_client_size.cy - old_client_size.cy);
					break;
				}
				case WMSZ_RIGHT:
				{
					float rate = (float)old_client_size.cx / Gp_ini->screen_size.cx;
					new_client_size.cx = std::max((int)ceil(Gp_ini->screen_size.cx * rate), 16);
					new_client_size.cy = std::max((int)ceil(Gp_ini->screen_size.cy * rate), 16);

					screen_rect->right = screen_rect->right + (new_client_size.cx - old_client_size.cx);
					screen_rect->bottom = screen_rect->bottom + (new_client_size.cy - old_client_size.cy);
					break;
				}
				case WMSZ_BOTTOMLEFT:
				{
					float x_rate = (float)old_client_size.cx / Gp_ini->screen_size.cx;
					float y_rate = (float)old_client_size.cy / Gp_ini->screen_size.cy;
					float rate = std::max(x_rate, y_rate);
					new_client_size.cx = std::max((int)ceil(Gp_ini->screen_size.cx * rate), 16);
					new_client_size.cy = std::max((int)ceil(Gp_ini->screen_size.cy * rate), 16);

					screen_rect->left = screen_rect->left - (new_client_size.cx - old_client_size.cx);
					screen_rect->bottom = screen_rect->bottom + (new_client_size.cy - old_client_size.cy);
					break;
				}
				case WMSZ_BOTTOM:
				{
					float rate = (float)old_client_size.cy / Gp_ini->screen_size.cy;
					new_client_size.cx = std::max((int)ceil(Gp_ini->screen_size.cx * rate), 16);
					new_client_size.cy = std::max((int)ceil(Gp_ini->screen_size.cy * rate), 16);

					screen_rect->left = screen_rect->left - (new_client_size.cx - old_client_size.cx) / 2;
					screen_rect->right = screen_rect->right + (new_client_size.cx - old_client_size.cx) / 2;
					screen_rect->bottom = screen_rect->bottom + (new_client_size.cy - old_client_size.cy);
					break;
				}
				case WMSZ_BOTTOMRIGHT:
				{
					float x_rate = (float)old_client_size.cx / Gp_ini->screen_size.cx;
					float y_rate = (float)old_client_size.cy / Gp_ini->screen_size.cy;
					float rate = std::max(x_rate, y_rate);
					new_client_size.cx = std::max((int)ceil(Gp_ini->screen_size.cx * rate), 16);
					new_client_size.cy = std::max((int)ceil(Gp_ini->screen_size.cy * rate), 16);

					screen_rect->right = screen_rect->right + (new_client_size.cx - old_client_size.cx);
					screen_rect->bottom = screen_rect->bottom + (new_client_size.cy - old_client_size.cy);
					break;
				}
			}

			// �T�C�Y�ύX�O�̃E�B���h�E�T�C�Y�ƃN���C�A���g�T�C�Y�̍������X�V
			m_diff.cx = get_window_rect().width() - get_client_rect().width();
			m_diff.cy = get_window_rect().height() - get_client_rect().height();

			return TRUE;
		}

		case WM_ENTERMENULOOP:

			// ���j���[���J�����ꍇ�͎��Ԃ��~�߂�
			if (!Gp_global->alt_menu_wait_flag)	{
				Gp_global->alt_menu_wait_flag = true;
				// �������߂�ƁA�t���[�������Ŏ����I�� false �ɖ߂�
			}
			break;
	}

	return C_window::window_proc(msg, wp, lp);
}

// ****************************************************************
// �E�B���h�E���\�z���ꂽ
// ================================================================
//		���C���E�B���h�E�ɐݒ肵�܂��B
//		�ۑ�������`�����o���܂��B
// ================================================================
bool C_tnm_wnd::on_create()
{
	// ���A�C�R����ݒ肷��
	// ��window.ini �ǂݍ��݂���ɍs��Ȃ��Ɛݒ肳��Ȃ��ꍇ������܂��B�����s���B���ӁI�I
	set_icon(Gp_icon->get_large_icon());
	set_small_icon(Gp_icon->get_small_icon());

	// IME �̑Ώۂ���͂���
	::ImmAssociateContext(m_h_wnd, NULL);	// �� ���ꂪ�����B���̃E�B���h�E�̂� IME ���I�t�ɂ���B
	//ImmDisableIME(0);						// �� ����͊ԈႢ�B�A�v������ IME ���I�t�ɂ��Ă��܂��B�G�f�B�b�g�{�b�N�X�ւ̓��͂��ł��Ȃ��I
	// ���C���E�B���h�E�ɐݒ�
	set_main_window();
	// ini ����E�B���h�E��`���擾
	load_ini_window_rect();

	return true;
}

// ****************************************************************
// �E�B���h�E��������
// ================================================================
//		�E�B���h�E��`��ۑ����܂��B
//		�E�B���h�E�������钼�O�̏������s���܂��B
// ================================================================
bool C_tnm_wnd::on_close()
{
	// �I���m�F
	if (!Gp_global->game_end_no_warning)	{

		int res = tnm_game_warning_box(Gp_ini->config.warninginfo.gameend_warning_str);
		if (res == IDNO)
			return false;	// WM_CLOSE ���L�����Z������
	}

	// �G���h�Z�[�u���s���Ă��Ȃ��Ȃ�s��
	if (!Gp_global->game_end_save_done && Gp_ini->end_save_cnt > 0)	{
		Gp_global->game_end_save_done = true;	// ���s���Ă��������[�v�ɂȂ�Ȃ��悤�A��Ƀt���O�𗧂Ă�

		tnm_syscom_end_game(false, false, false);
		return false;		// WM_CLOSE ���L�����Z������
	}

	// ini �ɃE�B���h�E��`��ۑ�
	save_ini_window_rect();
	// �E�B���h�E�������钼�O�̉���������s���܂��B
	tnm_free();

	return true;
}

// ****************************************************************
// �ĕ`������߂�ꂽ
// ================================================================
//		Direct3D �𗘗p���ĕ`����s���܂��B
// ================================================================
bool C_tnm_wnd::on_paint()
{
	// Direct3D �̃f�o�C�X����������Ă���ꍇ
	// Direct3D �̃f�o�C�X���������Ă��Ȃ��ꍇ
	if (G_d3d.device.body() && !G_d3d.device.is_lost())	{

		// �X�V��`���N���A���܂��B
		::ValidateRect(m_h_wnd, NULL);

		// �X�V��`�𖳎����āA��ʑS�̂�`�悵�܂��B
		G_d3d.device.present(NULL);
	}

	// Direct3D �̃f�o�C�X����������Ă��Ȃ��ꍇ
	else	{

		// �X�V��`���N���A���邽�߂ɁA�ȉ��̏����͕K�{�ł��B
		PAINTSTRUCT ps;
		HDC h_dc = BeginPaint(m_h_wnd, &ps);

		// ���ɓh��Ԃ��܂��B
		HBRUSH h_brush = (HBRUSH)GetStockObject(BLACK_BRUSH);
		SelectObject(h_dc, h_brush);
		FillRect(h_dc, &ps.rcPaint, h_brush);

		// �X�V��`���N���A���܂��B
		EndPaint(m_h_wnd, &ps);
	}

	// �f�t�H���g�̏��������s���Ȃ�
	return false;
}

// ****************************************************************
// �R�}���h
// ================================================================
//		���j���[�Ȃǂ̏������s���܂��B
// ================================================================
bool C_tnm_wnd::on_command(WPARAM wp, LPARAM lp)
{
	// �E�B���h�E���j���[
	if (HIWORD(wp) == 0)
		m_wnd_menu.on_command(LOWORD(wp));

	// �Q�[���������I���i�R���p�C������ċN�������߂�ꂽ�Ƃ��ɔ��ł���j
	else if (HIWORD(wp) == IDC_END)	{

		// �x���Ȃ��ŃQ�[�����I������t���O
		Gp_global->game_end_no_warning = true;
		// �E�B���h�E����邱�ƂŃQ�[�����I������
		send_message(WM_CLOSE, 0, 0);
	}

	return true;
}

// ****************************************************************
// �J�[�\����ݒ�
// ================================================================
//		�}�E�X�J�[�\�����������Ƃ��ɌĂяo����܂��B
// ================================================================
bool C_tnm_wnd::on_set_cursor(WPARAM wp, LPARAM lp)
{
	// �J�[�\�����č\�z����
	//Gp_cursor->restruct_cursor();

	// ������Ȃ��ł��B�t���[�������őS�Ęd���܂��B
	// ���[�_���_�C�A���O���o�Ă���Ԃɂ����ʂ�ƁA
	// �P�u�����Q�[���̃J�[�\�����o�āA�����ɃV�X�e���J�[�\���ɖ߂��Ă��܂�����ł��B

	// �ǂ����Ă������K�v������ꍇ�́A
	// �t���[���������Łu���[�_���_�C�A���O���o�Ă���Ԃ̓V�X�e���C���ɂ���v���������Ă��������B

	return true;
}

