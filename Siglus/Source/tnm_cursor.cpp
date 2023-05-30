#include	"pch.h"
#include	"ifc_eng.h"

#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_cursor.h"
#include	"data/tnm_local_data.h"

#include	"engine/eng_dir.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_dialog.h"
#include	"engine/eng_editbox.h"
#include	"engine/ifc_error.h"
#include	"engine/eng_etc.h"

// ****************************************************************
// �J�[�\���\����
// ================================================================
S_cursor::S_cursor()
{
	h_icon = NULL;
	h_mask = NULL;
}
S_cursor::~S_cursor()
{
	if (h_mask)	{
		::DeleteObject(h_mask);
		h_mask = NULL;
	}
	if (h_icon)	{
		::DestroyIcon(h_icon);
		h_icon = NULL;
	}
}

// ****************************************************************
// �J�[�\��
// ================================================================
C_tnm_cursor::C_tnm_cursor()
{
	m_cursor_pat_cnt = 0;
}

// ****************************************************************
// �J�[�\���F�J�[�\�����쐬����
// ================================================================
void C_tnm_cursor::create_cursor(int cursor_no)
{
	if (cursor_no < 0 || Gp_ini->mouse_cursor_cnt <= cursor_no)
		return;

	// ���܂� g00 �����Ή����Ă��܂���B
	TSTR file_name = Gp_ini->mouse_cursor[cursor_no].file_name;
	if (file_name.empty())
		return;
	
	int pct_type = 0;
	TSTR file_path = tnm_find_g00(file_name, &pct_type);

	if (file_path.empty())
	{
		tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("�}�E�X�J�[�\�� ") + file_name + _T(" ��������܂���ł����B\n") + file_name);
		return;
	}
	if (pct_type != 1)
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�}�E�X�J�[�\���� g00 �����Ή����Ă��܂���B\n") + file_name);
		return;
	}

	// �A���o���� g00 ��ǂݍ���
	C_album album;
	if (!::load_g00_to_album(&album, file_path))
	{
		tnm_set_error(TNM_ERROR_TYPE_PCT_NOT_FOUND, _T("�}�E�X�J�[�\�� ") + file_name + _T(" ���ǂݍ��߂܂���ł����B\n") + get_last_error_msg());
		return;
	}

	// �T�C�Y�𒲂ׂ�i32�~32 �Œ�j
	for (int i = 0; i < album.get_pct_cnt(); i++)
	{
		BSP<C_pct> pct = album.get_pct(i);
		if (pct)
		{
			if (pct->get_width() != 32 || pct->get_height() != 32)
			{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�}�E�X�J�[�\���̃T�C�Y�� 32�~32 �łȂ���΂����܂���B\n") + file_name + _T(" (patno = ") + tostr(i) + _T(")"));
			}
		}
	}

	// �A�C�R�����J�b�g��������������
	m_cursor_pat_cnt = (int)album.get_pct_cnt();
	m_cursor_pat_list = BSA<S_cursor>(new S_cursor[m_cursor_pat_cnt]);

	// �e�J�b�g�f�[�^��W�J����
	for (int i = 0; i < album.get_pct_cnt(); i++)
	{
		BSP<C_pct> pct = album.get_pct(i);
		C_point center = pct->get_center();
		BSP<C_dib_chip> src_chip = pct->expand_chips();
		if (src_chip)
		{
			// �A�C�R���p�̃r�b�g�}�b�v���쐬
			C_dib dib_icon(true);
			dib_icon.create(32, 32, 32);	// 32�~32 �Œ�Ȃ̂ł��B
			{
				int w = std::min(src_chip->get_width(), 32);	// 32�~32�ɐ����B
				int h = std::min(src_chip->get_height(), 32);	// 32�~32�ɐ����B
				for (int y = 0; y < h; y++)	{
					C_argb* dst = (C_argb *)dib_icon.get_ptr(0, y);
					C_argb* src = (C_argb *)src_chip->get_ptr(0, y);
					memcpy(dst, src, w * sizeof(C_argb));
				}
			}

			// �}�X�N�p�̋�r�b�g�}�b�v���쐬
			m_cursor_pat_list[i].h_mask = ::CreateBitmap(32, 32, 1, 1, NULL);

			// �A�C�R���̏���ݒ�
			ICONINFO info;
			info.fIcon = FALSE;			// FALSE: �J�[�\��
			info.xHotspot = center.x;	// ���S���W���z�b�g�X�|�b�g�Ƃ��Ĉ���
			info.yHotspot = center.y;	// ���S���W���z�b�g�X�|�b�g�Ƃ��Ĉ���
			info.hbmColor = dib_icon.get_handle();
			info.hbmMask = m_cursor_pat_list[i].h_mask;

			// �A�C�R�����쐬
			m_cursor_pat_list[i].h_icon = ::CreateIconIndirect(&info);
			if (m_cursor_pat_list[i].h_icon == NULL)
			{
				tnm_set_error(TNM_ERROR_TYPE_FATAL, str_format(_T("%d �Ԗڂ̃}�E�X�J�[�\���̍쐬�Ɏ��s���܂����B\n"), i) + get_last_error_msg());
			}
		}
	}
}

// ****************************************************************
// �J�[�\���F�t���[������
// ================================================================
void tnm_cursor_frame(int cur_time)
{
	HCURSOR h_cursor = NULL;

	// �N���C�A���g�O�ɂ���Ƃ��̓V�X�e���ɂ܂�����̂ŉ������Ȃ�
	C_point mp = Gp_wnd->screen_to_client(get_mouse_pos());
	C_rect cr = Gp_wnd->get_client_rect();
	if (!(cr.left <= mp.x && mp.x < cr.right && cr.top <= mp.y && mp.y < cr.bottom))
		return;

	// ���̃_�C�A���O���ɂ���Ƃ��̓V�X�e���ɂ܂�����̂ŉ������Ȃ�
	if (tnm_dlg_check_mouse_over())
		return;

	// ���̃G�f�B�b�g�{�b�N�X���ɂ���Ƃ��̓V�X�e���ɂ܂�����̂ŉ������Ȃ�
	if (tnm_editbox_check_mouse_over())
		return;

	// �\���t���O
	bool disp_flag = true;

	// �J�[�\����\�����Ȃ��ꍇ
	if (!tnm_is_cursor_disp())
		disp_flag = false;

	// �ŏI�l�F�J�[�\����\������ꍇ�̂ݏ���
	if (disp_flag)	{

		int cursor_no = Gp_local->pod.cursor_no;

		// Windows �f�t�H���g�̃J�[�\���̏ꍇ
		if (cursor_no < 0 || Gp_ini->mouse_cursor_cnt <= cursor_no || !Gp_cursor_list->sub[cursor_no].m_cursor_pat_list)
		{
			h_cursor = LoadCursor(NULL, IDC_ARROW);
		}

		// �摜����ǂݍ��ޏꍇ
		else
		{
			int pat_no = 0;

			// �A�j���[�V��������
			int anime_speed = Gp_ini->mouse_cursor[cursor_no].anime_speed;
			pat_no = (anime_speed == 0) ? 0 : (DWORD)cur_time / anime_speed % Gp_cursor_list->sub[cursor_no].m_cursor_pat_cnt;
				// ��(DWORD) �͏d�v�ł��Bint �̂܂܂��ƁA�������}�C�i�X�̂Ƃ��A% ���Z�q�̓}�C�i�X�l��Ԃ��Ă��܂��܂��B

			// �Y������J�[�\���̃n���h�����擾
			h_cursor = Gp_cursor_list->sub[cursor_no].m_cursor_pat_list[pat_no].h_icon;
		}
	}

	// �J�[�\�����ς������ύX����
	if (::GetCursor() != h_cursor)
		::SetCursor(h_cursor);
}
