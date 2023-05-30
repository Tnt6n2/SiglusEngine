#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_save_00.h"
#include	"element/elm_editbox.h"
#include	"engine/eng_display.h"
#include	"engine/eng_editbox.h"

// ****************************************************************
// �G�f�B�b�g�{�b�N�X�F�v���V�[�W��
// ================================================================
LRESULT C_tnm_editbox::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_SYSKEYDOWN:
			if (wp == VK_RETURN)	{
				if ((lp >> 29) & 1)	{
					tnm_switch_screen_size_mode(false);		// Alt+Enter ���̓X�N���[�����[�h�ؑ�
				}

				// Enter �L�[�ł́A�f�t�H���g�̓�������s���܂���B
				// ���s����ƁAWindows �̃G���[���ʉ������Ă��܂�����ł��B
				return TRUE;
			}
			break;

		case WM_KEYDOWN:
			if (wp == VK_TAB)	{
				if (GetAsyncKeyState(VK_SHIFT) & 0x8000)	{
					tnm_editbox_move_focus_backward();		// Shift+Tab: �O��
				}
				else	{
					tnm_editbox_move_focus_forward();		// Tab: ����
				}
			}
			if (wp == VK_RETURN)	{
				m_action_flag = TNM_EDITBOX_DECIDED;		// Enter �L�[�̏����iAlt+Enter ���� WM_SYSKEYDOWN ��ʂ邽�߁A�����͒ʂ�܂���j

				// Enter �L�[�ł́A�f�t�H���g�̓�������s���܂���B
				// ���s����ƁAWindows �̃G���[���ʉ������Ă��܂�����ł��B
				return TRUE;
			}
			if (wp == VK_ESCAPE)	{
				m_action_flag = TNM_EDITBOX_CANCELED;		// Escape �L�[�̏���
			}
			break;

		case WM_CHAR:
			// ���̏������Ȃ��ƁATab�AEnter�AESC �L�[�� Windows �̌x���������Ă��܂�
			if (wp == VK_TAB || wp == VK_RETURN || wp == VK_ESCAPE)	{
				return FALSE;
			}
			break;
	}

	return Cc_edit::window_proc(msg, wp, lp);
}

// ****************************************************************
// �G�������g�F�G�f�B�b�g�{�b�N�X
// ================================================================
C_elm_editbox::C_elm_editbox()
{
	m_h_font = NULL;
}
C_elm_editbox::~C_elm_editbox()
{
	destroy();
}

// ****************************************************************
// �G�������g�F�G�f�B�b�g�{�b�N�X�F������
// ================================================================
void C_elm_editbox::init()
{
	// �ď�����
	reinit();
}

// ****************************************************************
// �G�������g�F�G�f�B�b�g�{�b�N�X�F�ď�����
// ================================================================
void C_elm_editbox::reinit()
{
	destroy();
	m_editbox.init();

	// �p�����[�^
	m_created = false;
	m_rect = C_rect(0, 0, 0, 0);
}

// ****************************************************************
// �G�������g�F�G�f�B�b�g�{�b�N�X�F�Z�[�u
// ================================================================
void C_elm_editbox::save(C_tnm_save_stream& stream)
{
	stream.save(m_created);
	stream.save(m_rect);
	stream.save(m_moji_size);
}

// ****************************************************************
// �G�������g�F�G�f�B�b�g�{�b�N�X�F���[�h
// ================================================================
void C_elm_editbox::load(C_tnm_save_stream& stream)
{
	stream.load(m_created);
	stream.load(m_rect);
	stream.load(m_moji_size);

	// �č\�z
	if (m_created)
		create(m_rect, m_moji_size);
}

// ****************************************************************
// �G�������g�F�G�f�B�b�g�{�b�N�X�F����
// ================================================================

// �쐬
void C_elm_editbox::create(C_rect rect, int moji_size)
{
	destroy();

	m_editbox.init();		// ���肵���t���O������������
	m_editbox.create(_T("siglus_editbox"), _T(""), rect.left, rect.top, rect.width(), rect.height(), false, WS_CHILD | ES_AUTOHSCROLL, 0, false, false, G_app.h_wnd);
	m_created = true;
	m_rect = rect;
	m_moji_size = moji_size;

	// �\������O�ɋ�`���E�B���h�E�T�C�Y�ɍ��킹�čX�V����
	update_rect();

	m_editbox.set_show(true);
	m_editbox.set_focus();
}

// �j��
void C_elm_editbox::destroy()
{
	m_editbox.set_show(false);
	m_editbox.destroy();
	release_font();
	m_created = false;
}

// �t�H���g�����
void C_elm_editbox::release_font()
{
	if (m_h_font)	{
		::DeleteObject(m_h_font);
		m_h_font = NULL;
	}
}

// �e�L�X�g��ݒ�
void C_elm_editbox::set_text(CTSTR& text)
{
	m_editbox.set_text(text);

	// �e�L�X�g��ύX�����ꍇ�A�J�[�\���𖖔��Ɉړ�����
	int moji_cnt_max = (int)m_editbox.send_message(EM_GETLIMITTEXT, 0, 0);
	m_editbox.set_sel(moji_cnt_max, moji_cnt_max);
}

// �e�L�X�g���擾
TSTR C_elm_editbox::get_text()
{
	return m_editbox.get_text();
}

// �t�H�[�J�X��ݒ肷��
void C_elm_editbox::set_focus()
{
	m_editbox.set_focus();
}

// ���͂��N���A����
void C_elm_editbox::clear_input()
{
	m_editbox.clear_input();
}

// ****************************************************************
// �G�������g�F�G�f�B�b�g�{�b�N�X�F�t���[������
// ================================================================
void C_elm_editbox::frame()
{
	// �G�f�B�b�g�{�b�N�X��\����������F
	bool show_flag = m_created								// ����Ă���
		&& Gp_global->change_display_mode_proc_cnt == 0		// �𑜓x�؂�ւ����łȂ�
//		&& !Gp_excall->is_ready()							// �d�w�R�[�����������łȂ�
		;

	m_editbox.set_show(show_flag);
}

// ****************************************************************
// �G�������g�F�G�f�B�b�g�{�b�N�X�F��`���X�V
// ================================================================
void C_elm_editbox::update_rect()
{
	// �Q�[���X�N���[���T�C�Y�ɍ��킹�ċ�`���X�V���܂��B
	C_rect new_rect;
	new_rect.left = m_rect.left * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx  + Gp_global->total_game_screen_pos.x;
	new_rect.top = m_rect.top * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
	new_rect.right = m_rect.right * Gp_global->total_game_screen_size.cx / Gp_global->game_screen_size.cx + Gp_global->total_game_screen_pos.x;
	new_rect.bottom = m_rect.bottom * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy + Gp_global->total_game_screen_pos.y;
	m_editbox.set_window_rect(new_rect);

	// ����ɕ����T�C�Y���ύX���܂��B
	release_font();
	int new_moji_size = m_moji_size * Gp_global->total_game_screen_size.cy / Gp_global->game_screen_size.cy;	// �����T�C�Y�̓Q�[���X�N���[���̏c���ɂ��킹�܂��B
	m_h_font = ::CreateFont(new_moji_size, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("�l�r �S�V�b�N"));
	m_editbox.send_message(WM_SETFONT, (WPARAM)m_h_font, (LPARAM)MAKELPARAM(TRUE, 0));
}

// ****************************************************************
// �G�������g�F�G�f�B�b�g�{�b�N�X���X�g�F�T�u�̏�����
// ================================================================
void C_elm_editbox_list::_init(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].init();
}

// ****************************************************************
// �G�������g�F�G�f�B�b�g�{�b�N�X���X�g�F�T�u�̍ď�����
// ================================================================
void C_elm_editbox_list::_reinit(int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].reinit();
}

// ****************************************************************
// �G�������g�F�G�f�B�b�g�{�b�N�X���X�g�F�T�u�̃Z�[�u
// ================================================================
void C_elm_editbox_list::_save(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].save(stream);
}

// ****************************************************************
// �G�������g�F�G�f�B�b�g�{�b�N�X���X�g�F�T�u�̃��[�h
// ================================================================
void C_elm_editbox_list::_load(C_tnm_save_stream& stream, int begin, int end)
{
	for (int i = begin; i < end; i++)
		m_sub[i].load(stream);
}

// ****************************************************************
// �G�f�B�b�g�{�b�N�X�̃t�H�[�J�X�����Ɉړ��iTab �L�[�ɂ�鏈���j
// ================================================================
void tnm_editbox_move_focus_forward()
{
	int editbox_cnt = Gp_editbox_list->get_size();

	// �G�f�B�b�g�{�b�N�X���Ȃ���ΏI��
	if (editbox_cnt == 0)
		return;

	// �t�H�[�J�X�̂���G�f�B�b�g�{�b�N�X���擾����
	int focused_editbox_no = -1;
	HWND h_focused_wnd = ::GetFocus();
	if (h_focused_wnd)	{
		for (int i = 0; i < editbox_cnt; i++)	{
			if ((*Gp_editbox_list)[i].editbox().get_handle() == h_focused_wnd)	{
				focused_editbox_no = i;
				break;
			}
		}
	}
	if (focused_editbox_no == -1)
		return;

	// �t�H�[�J�X���ړ�����
	for (int i = focused_editbox_no; ; )	{
		i = (i + editbox_cnt + 1) % editbox_cnt;
		if (i == focused_editbox_no)	{
			return;		// ������Ȃ������̂ŏI��
		}
		if ((*Gp_editbox_list)[i].is_created())	{
			(*Gp_editbox_list)[i].set_focus();
			return;
		}
	}
}


// ****************************************************************
// �G�f�B�b�g�{�b�N�X�̃t�H�[�J�X��O�Ɉړ��iShift+Tab �L�[�ɂ�鏈���j
// ================================================================
void tnm_editbox_move_focus_backward()
{
	int editbox_cnt = Gp_editbox_list->get_size();

	// �G�f�B�b�g�{�b�N�X���Ȃ���ΏI��
	if (editbox_cnt == 0)
		return;

	// �t�H�[�J�X�̂���G�f�B�b�g�{�b�N�X���擾����
	int focused_editbox_no = -1;
	HWND h_focused_wnd = ::GetFocus();
	if (h_focused_wnd)	{
		for (int i = 0; i < editbox_cnt; i++)	{
			if ((*Gp_editbox_list)[i].editbox().get_handle() == h_focused_wnd)	{
				focused_editbox_no = i;
				break;
			}
		}
	}
	if (focused_editbox_no == -1)
		return;

	// �t�H�[�J�X���ړ�����
	for (int i = focused_editbox_no; ; )	{
		i = (i + editbox_cnt - 1) % editbox_cnt;
		if (i == focused_editbox_no)	{
			return;		// ������Ȃ������̂ŏI��
		}
		if ((*Gp_editbox_list)[i].is_created())	{
			(*Gp_editbox_list)[i].set_focus();
			return;
		}
	}
}


// ****************************************************************
// �G�f�B�b�g�{�b�N�X�Ƀ}�E�X������Ă��邩�𔻒�
// ================================================================
bool tnm_editbox_check_mouse_over()
{
	int editbox_cnt = Gp_editbox_list->get_size();
	C_point mp = get_mouse_pos();

	// �J�[�\�����ڂ��Ă��邩�𔻒�
	for (int i = 0; i < editbox_cnt; i++)	{
		if ((*Gp_editbox_list)[i].editbox().get_handle())	{
			C_rect wr = (*Gp_editbox_list)[i].editbox().get_window_rect();
			if (wr.left <= mp.x && mp.x < wr.right && wr.top <= mp.y && mp.y < wr.bottom)	{
				return true;	// ��ɏ���Ă���I
			}
		}
	}

	return false;
}

// ****************************************************************
// �G�f�B�b�g�{�b�N�X��S�Ĕ�\���ɂ���
// ================================================================
void tnm_editbox_set_hide_all()
{
	// �G�f�B�b�g�{�b�N�X��S�ĉB���܂��B�i���A�����̓t���[�������ł���Ă���܂��B�j
	int editbox_cnt = (int)Gp_editbox_list->get_size();
	for (int i = 0; i < editbox_cnt; i++)	{
		(*Gp_editbox_list)[i].editbox().set_show(false);
	}
}

