#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_config_data.h"
#include	"data/tnm_lexer.h"
#include	"engine/eng_config.h"
#include	"engine/eng_scene.h"
#include	"engine/eng_etc.h"
#include	"dialog/db_wnd_info_trace_text_window.h"
#include	"../resource.h"

// ****************************************************************
// �R���X�g���N�^
// ================================================================
C_db_wnd_info_trace_text_window::C_db_wnd_info_trace_text_window()
{
	init_current_draw_state();
}

// ****************************************************************
// �f�X�g���N�^
// ================================================================
C_db_wnd_info_trace_text_window::~C_db_wnd_info_trace_text_window()
{
}

// ****************************************************************
// ���b�Z�[�W�v���V�[�W���i���E�B���h�E�v���V�[�W���ł͂Ȃ��j
// ================================================================
LRESULT C_db_wnd_info_trace_text_window::message_proc(UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)	{
		case WM_LBUTTONDOWN:	set_focus_draw_target(wp, lp);	break;
		case WM_RBUTTONDOWN:	set_focus_draw_target(wp, lp);	break;
		case WM_MOUSEWHEEL:		on_mousewheel(wp, lp);			break;
		case WM_VSCROLL:		on_vscroll(wp, lp);				break;
	}
	return FALSE;
}

// ****************************************************************
// �쐬
// ================================================================
void C_db_wnd_info_trace_text_window::create(HWND parent_h_wnd, C_rect rect, int page_cnt)
{
	work.parent_h_wnd = parent_h_wnd;

	draw_target.create(_T(""), _T(""), 0, 0, 100, 100, false, WS_CHILD | WS_GROUP | WS_TABSTOP, WS_EX_CLIENTEDGE, false, false, work.parent_h_wnd);
	v_scroll.create(_T(""), _T(""), 0, 0, 100, 100, false, WS_CHILD | WS_GROUP | SBS_VERT, 0, false, false, work.parent_h_wnd);
	h_scroll.create(_T(""), _T(""), 0, 0, 100, 100, false, WS_CHILD | WS_GROUP, 0, false, false, work.parent_h_wnd);

	resize_text_window_rect(rect);

	draw_target.set_show(true);
	v_scroll.set_show(true);
	h_scroll.set_show(true);

	alloc_seen_file(page_cnt);

	set_focus_draw_target();
}

// ****************************************************************
// �e�L�X�g�E�B���h�E�����T�C�Y����
// ================================================================
void C_db_wnd_info_trace_text_window::resize_text_window_rect(C_rect rect)
{
	int v_width = GetSystemMetrics(SM_CXVSCROLL);
	int h_height = GetSystemMetrics(SM_CYHSCROLL);

	C_rect wr;

	wr = rect;
	wr.right = rect.right - v_width;
	wr.bottom = rect.bottom - h_height;
	draw_target.set_window_rect(wr);
	draw_target.invalidate_rect(NULL, false);

	wr = rect;
	wr.left = rect.right - v_width;
	wr.bottom = rect.bottom - h_height;
	v_scroll.set_window_rect(wr);
	v_scroll.invalidate_rect(NULL, false);

	wr = rect;
	wr.top = rect.bottom - h_height;
	wr.right = rect.right - v_width;
	h_scroll.set_window_rect(wr);
	h_scroll.invalidate_rect(NULL, false);
}

// ****************************************************************
// �V�[���ԍ����擾����
// ================================================================
int C_db_wnd_info_trace_text_window::get_scn_no(int page_no)
{
	if (work.seen_state.empty() || !(0 <= page_no && page_no < (int)work.seen_state.size()))	{	return -1;	}
	ARRAY<SEEN_STATE>::iterator itr = work.seen_state.begin() + page_no;
	return itr->draw_state.scn_no;
}

// ****************************************************************
// �`��^�[�Q�b�g�̍��W�ɕϊ�����
// ================================================================

C_point C_db_wnd_info_trace_text_window::change_draw_target_pos(C_point pos)
{
	ClientToScreen(work.parent_h_wnd, &pos);
	ScreenToClient(draw_target.get_handle(), &pos);
	return pos;
}

C_point C_db_wnd_info_trace_text_window::change_draw_target_pos(int x, int y)
{
	return change_draw_target_pos(C_point(x, y));
}

C_point C_db_wnd_info_trace_text_window::change_draw_target_pos(LPARAM lp)
{
	return change_draw_target_pos(C_point((int)(lp & 0xFFFF), (int)((lp >> 16) & 0xFFFF)));
}


// ****************************************************************
// �`��^�[�Q�b�g�Ƀ}�E�X������Ă��邩���肷��
// ================================================================

bool C_db_wnd_info_trace_text_window::check_draw_target_on_mouse(C_point pos)
{
	pos = change_draw_target_pos(pos);
	C_rect rect = draw_target.get_client_rect();
	if (rect.is_in(pos))	{	return true;	}
	return false;
}

bool C_db_wnd_info_trace_text_window::check_draw_target_on_mouse(int x, int y)
{
	return check_draw_target_on_mouse(C_point(x, y));
}

bool C_db_wnd_info_trace_text_window::check_draw_target_on_mouse(LPARAM lp)
{
	return check_draw_target_on_mouse(C_point((int)(lp & 0xFFFF), (int)((lp >> 16) & 0xFFFF)));
}

// ****************************************************************
// �`��^�[�Q�b�g���t�H�[�J�X����
// ================================================================

void C_db_wnd_info_trace_text_window::set_focus_draw_target()
{
	draw_target.set_focus();
}

void C_db_wnd_info_trace_text_window::set_focus_draw_target(WPARAM wp, LPARAM lp)
{
	if (!check_draw_target_on_mouse(lp))	{	return;	}
	set_focus_draw_target();

	if (work.seen_state.empty() || !(0 <= work.current_page_no && work.current_page_no < (int)work.seen_state.size()))	{	return;	}
	C_point pos = change_draw_target_pos(lp);
	ARRAY<SEEN_STATE>::iterator seen_state_itr = work.seen_state.begin() + work.current_page_no;
	seen_state_itr->draw_state.focus_line_no = seen_state_itr->draw_state.draw_top_line_no + (pos.y / work.moji_state.y_space) + 1;
	re_draw_seen_file();
}

// ****************************************************************
// �`��^�[�Q�b�g���t�H�[�J�X����Ă��邩���肷��
// ================================================================
bool C_db_wnd_info_trace_text_window::check_focus_draw_target()
{
	return (GetFocus() == draw_target.get_handle());
}

// ****************************************************************
// �}�E�X�z�C�[��
// ================================================================
void C_db_wnd_info_trace_text_window::on_mousewheel(WPARAM wp, LPARAM lp)
{
	if (GetFocus() != draw_target.get_handle() && !check_draw_target_on_mouse(lp))	{	return;	}

	int delta = GET_WHEEL_DELTA_WPARAM(wp);
	if (delta == 0)	{	return;	}

	int scroll_cnt = work.current_draw_state.draw_line_cnt;
	if (scroll_cnt < 0)	{	return;	}
	scroll_cnt /= 10;		// �P�^�P�O�s�X�N���[��
	if (scroll_cnt < 1)	{	scroll_cnt = 1;	}

	int pos = v_scroll.get_scroll_pos();
	if (delta > 0)	{	pos -= scroll_cnt;	}
	else			{	pos += scroll_cnt;	}
	v_scroll.set_scroll_pos(pos);
	re_draw_seen_file(v_scroll.get_scroll_pos());
}

// ****************************************************************
// �c�X�N���[��
// ================================================================
void C_db_wnd_info_trace_text_window::on_vscroll(WPARAM wp, LPARAM lp)
{
	if ((HWND)lp != v_scroll.get_handle())	{	return;	}
	int now_pos = v_scroll.get_scroll_pos();
	v_scroll.scroll_natural(wp);
	if (now_pos != v_scroll.get_scroll_pos())	{
		re_draw_seen_file(v_scroll.get_scroll_pos());
	}
}

// ****************************************************************
// �V�[���t�@�C���̊m��
// ================================================================
void C_db_wnd_info_trace_text_window::alloc_seen_file(int alloc_cnt)
{
	work.seen_state.resize(alloc_cnt);
}

// ****************************************************************
// �V�[���t�@�C���̃V�t�g
// ================================================================
void C_db_wnd_info_trace_text_window::shift_seen_file()
{
	if (work.seen_state.empty())	{	return;	}
	int seen_state_cnt = (int)work.seen_state.size();
	if (seen_state_cnt < 2)	{	return;	}
	ARRAY<SEEN_STATE>::iterator src, dst;
	for (int i = seen_state_cnt - 1; i > 0; i--)	{
		src = work.seen_state.begin() + (i - 1);
		dst = work.seen_state.begin() + i;
		*dst = *src;
	}
}

// ****************************************************************
// �J�����g�̕`�����������
// ================================================================
void C_db_wnd_info_trace_text_window::init_current_draw_state()
{
	work.current_draw_state.init();
	work.current_page_no = -1;
	work.no_data_draw_flag = false;
}

// ****************************************************************
// �`��N���A
// ================================================================
void C_db_wnd_info_trace_text_window::draw_clear()
{
	if (!work.no_data_draw_flag)	{
		draw_target.draw_fill_rect(0, 0, draw_target.text_bmp.get_width(), draw_target.text_bmp.get_height(), 128, 128, 128);
		init_current_draw_state();
		work.no_data_draw_flag = true;
	}
}

// ****************************************************************
// �V�[���t�@�C���̕`��
// ================================================================
void C_db_wnd_info_trace_text_window::draw_seen_file(int page_no, int scn_no, int draw_top_line_no, int target_line_no, CTSTR& font_name, int font_size, bool is_target)
{
	// ��񂪖���
	if (
		scn_no < 0 ||
		font_name.empty() ||
		font_size <= 0 ||
		work.seen_state.empty() ||
		!(0 <= page_no && page_no < (int)work.seen_state.size())
	)	{	draw_clear();	return;	}		// �������Ŗ߂�

	// �`���̂a�l�o�m��
	bool is_bmp_resize = draw_target.alloc_text_bmp();

	// �y�[�W�ԍ����ς����
	bool is_page_change = (work.current_page_no != page_no) ? true : false;

	// �J�����g�̃V�[�����C�e���[�^
	ARRAY<SEEN_STATE>::iterator seen_state_itr = work.seen_state.begin() + page_no;

	// �O�ԃy�[�W�̃V�[���ԍ����ύX���ꂽ
	bool is_zero_page_scn_change = (page_no == 0 && is_target && scn_no != seen_state_itr->draw_state.scn_no) ? true : false;

	// ���łɓǂݍ��܂�Ă��邩
	int seen_state_cnt = (int)work.seen_state.size();
	if (is_zero_page_scn_change && seen_state_cnt >= 2)	{
		ARRAY<SEEN_STATE>::iterator itr;
		for (int i = 1; i < (seen_state_cnt - 1); i++)	{		// (seen_state_cnt - 1) = �Ō�̃V�[���X�^�b�N���̓V�t�g����Ə�����̂Ŏg���Ȃ�
			itr = work.seen_state.begin() + i;
			if (scn_no == itr->draw_state.scn_no)	{
				shift_seen_file();
				*seen_state_itr = *(itr + 1);
				break;
			}
		}
	}

	// �V�[���ύX
	bool is_seen_change = false;
	TSTR full_path_name;
	if (seen_state_itr->draw_state.scn_no != scn_no)	{
		// �V�[���t�@�C���̃V�t�g
		shift_seen_file();
		seen_state_itr->free();
		// �V�[���ǂݍ���
		full_path_name = Gp_config->ss_path + _T("\\") + Gp_lexer->get_scn_name(scn_no) + _T(".ss");
		seen_state_itr->load(scn_no);
		is_seen_change = true;
	}

	// �f�[�^������
	if (seen_state_itr->text_data.empty())	{	draw_clear();	return;	}		// �������Ŗ߂�

	// �t�H���g�ύX
	bool is_font_change = work.moji_state.set(font_name, font_size);

	// �\���\�ȍs�����Z�o
	int draw_line_cnt = draw_target.text_bmp.get_height() / work.moji_state.y_space;

	// �`�敝������
	if (draw_line_cnt <= 0)	{	draw_clear();	return;	}		// �������Ŗ߂�

	// �擪�s
	int lim1, lim2;
	if (is_target)	{
		if (draw_line_cnt < 8 || seen_state_itr->draw_state.draw_top_line_no == -99999)	{
			draw_top_line_no = target_line_no - (draw_line_cnt >> 1);
		}
		else	{
			int alpha = limit(4, (draw_line_cnt / 2) - 1, 7);
			lim1 = seen_state_itr->draw_state.draw_top_line_no + alpha;
			lim2 = seen_state_itr->draw_state.draw_end_line_no - alpha;
			if (target_line_no < lim1)		{	draw_top_line_no = seen_state_itr->draw_state.draw_top_line_no - (lim1 - target_line_no);	}
			else if (target_line_no > lim2)	{	draw_top_line_no = seen_state_itr->draw_state.draw_top_line_no + (target_line_no - lim2);	}
			else							{	draw_top_line_no = seen_state_itr->draw_state.draw_top_line_no;						}
		}
	}

	// �ŏI�s
	int draw_end_line_no = draw_top_line_no + draw_line_cnt - 1;

	// �X�N���[���o�[����
	if (is_seen_change || is_font_change)	{
		int alpha = draw_line_cnt >> 1;
		v_scroll.set_scroll_info(-alpha, (int)seen_state_itr->text_data.size() + alpha, draw_line_cnt);
	}
	if (draw_top_line_no != v_scroll.get_scroll_pos())	{	v_scroll.set_scroll_pos(draw_top_line_no);	}

	// �\�[�X���g�p�����񐔂��J�E���g
	if (is_target)	{	seen_state_itr->draw_cnt++;	}

	// �`����̍X�V
	seen_state_itr->draw_state.scn_no = scn_no;							// �V�[���ԍ�
	if (is_target)	{	
		seen_state_itr->draw_state.target_line_no = target_line_no;		// �^�[�Q�b�g�̍s�ԍ�
	}
	if (is_zero_page_scn_change)	{
		seen_state_itr->draw_state.in_line_no = target_line_no;			// �h�m�̍s�ԍ�
	}
	seen_state_itr->draw_state.draw_top_line_no = draw_top_line_no;		// �`��̐擪�s�ԍ�
	seen_state_itr->draw_state.draw_end_line_no = draw_end_line_no;		// �`��̍ŏI�s�ԍ�
	seen_state_itr->draw_state.draw_line_cnt = draw_line_cnt;			// �`��̍s��

	// �`��
	bool redisp_flag = (is_bmp_resize || is_page_change || is_seen_change || is_font_change) ? true : false;
	draw_text(seen_state_itr, redisp_flag);

	// ���ݕ`�悳��Ă�����
	work.current_draw_state = seen_state_itr->draw_state;

	work.no_data_draw_flag = false;
	work.current_page_no = page_no;
}

// �y�[�W�ύX�̃V�[���t�@�C���̍ĕ`��
void C_db_wnd_info_trace_text_window::change_draw_seen_file(int page_no)
{
	if (work.seen_state.empty() || !(0 <= page_no && page_no < (int)work.seen_state.size()))	{	return;	}
	ARRAY<SEEN_STATE>::iterator seen_state_itr = work.seen_state.begin() + page_no;
	draw_seen_file(page_no, seen_state_itr->draw_state.scn_no, seen_state_itr->draw_state.draw_top_line_no, seen_state_itr->draw_state.target_line_no, work.moji_state.font_name, work.moji_state.font_size, false);
}

// �V�[���t�@�C���̍ĕ`��
void C_db_wnd_info_trace_text_window::re_draw_seen_file(int top_line_no)
{
	draw_seen_file(work.current_page_no, work.current_draw_state.scn_no, top_line_no, work.current_draw_state.target_line_no, work.moji_state.font_name, work.moji_state.font_size, false);
}

// �V�[���t�@�C���̍ĕ`��
void C_db_wnd_info_trace_text_window::re_draw_seen_file(CTSTR& font_name, int font_size)
{
	draw_seen_file(work.current_page_no, work.current_draw_state.scn_no, work.current_draw_state.draw_top_line_no, work.current_draw_state.target_line_no, font_name, font_size, false);
}

// �V�[���t�@�C���̍ĕ`��
void C_db_wnd_info_trace_text_window::re_draw_seen_file()
{
	draw_seen_file(work.current_page_no, work.current_draw_state.scn_no, work.current_draw_state.draw_top_line_no, work.current_draw_state.target_line_no, work.moji_state.font_name, work.moji_state.font_size, false);
}

// ****************************************************************
// �e�L�X�g�̕`��
// ================================================================
void C_db_wnd_info_trace_text_window::draw_text(ARRAY<SEEN_STATE>::iterator seen_state_itr, bool redisp_flag)
{
	int guidbar_width = work.moji_state.y_space;
	int draw_width = draw_target.text_bmp.get_width();
	int draw_height = draw_target.text_bmp.get_height();

	// ��`�R�s�[
	if (
		redisp_flag == false &&
		seen_state_itr->draw_state.scn_no == work.current_draw_state.scn_no &&
		work.current_draw_state.draw_top_line_no != -99999 &&
		(work.current_draw_state.draw_top_line_no - seen_state_itr->draw_state.draw_line_cnt) < seen_state_itr->draw_state.draw_top_line_no &&
		(work.current_draw_state.draw_top_line_no + seen_state_itr->draw_state.draw_line_cnt) > seen_state_itr->draw_state.draw_top_line_no
	)	{
		// ���Ɉړ�����
		if(work.current_draw_state.draw_top_line_no < seen_state_itr->draw_state.draw_top_line_no){
			int move_line_cnt = seen_state_itr->draw_state.draw_top_line_no - work.current_draw_state.draw_top_line_no;
			int move_line_size = move_line_cnt * work.moji_state.y_space;
			int x1 = guidbar_width;
			int y1 = move_line_size;
			int x2 = draw_width;
			int y2 = draw_height;
			int dst_x = guidbar_width;
			int dst_y = 0;
			draw_target.copy_rect(x1, y1, x2, y2, dst_x, dst_y);
		}
		// ��Ɉړ�����
		else if(work.current_draw_state.draw_top_line_no > seen_state_itr->draw_state.draw_top_line_no){
			int move_line_cnt = work.current_draw_state.draw_top_line_no - seen_state_itr->draw_state.draw_top_line_no;
			int move_line_size = move_line_cnt * work.moji_state.y_space;
			int x1 = guidbar_width;
			int y1 = 0;
			int x2 = draw_width;
			int y2 = draw_height - move_line_size;
			int dst_x = guidbar_width;
			int dst_y = move_line_size;
			draw_target.copy_rect(x1, y1, x2, y2, dst_x, dst_y);
		}
	}
	else	{	redisp_flag = true;	}		// �ĕ`��t���O���n�m�ɂ���

	// �����F
	COLORREF rgb_moji_black = RGB(0, 0, 0);
	COLORREF rgb_moji_white = RGB(255, 255, 255);

	// �u���V�쐬
	HBRUSH brush_outside = CreateSolidBrush(RGB(240,240,240));
	HBRUSH brush_guidbar = CreateSolidBrush(RGB(220,220,220));

	HBRUSH brush_white = CreateSolidBrush(RGB(255,255,255));
	HBRUSH brush_light_yellow = CreateSolidBrush(RGB(255,255,64));
	HBRUSH brush_light_blue = CreateSolidBrush(RGB(220,220,255));
	HBRUSH brush_light_red = CreateSolidBrush(RGB(255,210,210));

	// �`��
	ARRAY<TSTR>::iterator text_itr;
	ARRAY<int>::iterator now_state_itr;
	ARRAY<int>::iterator last_state_itr;
	TSTR::iterator itr, end_itr;
	int draw_x;
	int draw_y = 0;
	int x_space;
	int y_space = work.moji_state.y_space;
	int line_no = seen_state_itr->draw_state.draw_top_line_no;
	int x1, y1, x2, y2;
	COLORREF rgb_moji_color;
	HBRUSH brush_back_ground;
	int alpha;
	while (1)	{
		if (draw_y >= draw_height)	{	break;	}

		// �K�C�h�o�[
		x1 = 0;
		y1 = draw_y;
		x2 = x1 + guidbar_width;
		y2 = y1 + y_space;
		draw_target.draw_fill_rect(x1, y1, x2, y2, brush_guidbar);

		if (0 <= line_no && line_no < (int)seen_state_itr->text_data.size())	{
			text_itr = seen_state_itr->text_data.begin() + line_no;
			now_state_itr = seen_state_itr->text_state_now.begin() + line_no;
			last_state_itr = seen_state_itr->text_state_last.begin() + line_no;

			// �e�L�X�g����ݒ肷��
			alpha = line_no + 1;		// ��񂪂P�s����Ă���̂�␳����
			if (alpha == seen_state_itr->draw_state.focus_line_no)			{	*now_state_itr = 2;	}
			else if (alpha == seen_state_itr->draw_state.target_line_no)	{	*now_state_itr = 3;	}
			else if (alpha == seen_state_itr->draw_state.in_line_no)		{	*now_state_itr = 4;	}
			else if (alpha == seen_state_itr->draw_state.out_line_no)		{	*now_state_itr = 5;	}
			else															{	*now_state_itr = 1;	}

			if (redisp_flag || *now_state_itr != *last_state_itr)	{

				// �F
				switch (*now_state_itr)	{
					case 2:		rgb_moji_color = rgb_moji_black;	brush_back_ground = brush_light_blue;	break;		// focus
					case 3:		rgb_moji_color = rgb_moji_black;	brush_back_ground = brush_light_yellow;	break;		// target
					case 4:		rgb_moji_color = rgb_moji_black;	brush_back_ground = brush_light_red;	break;		// in
					case 5:		rgb_moji_color = rgb_moji_black;	brush_back_ground = brush_light_red;	break;		// out
					default:	rgb_moji_color = rgb_moji_black;	brush_back_ground = brush_white;		break;		// (normal)
				}

				// ����
				draw_x = guidbar_width;
				end_itr = text_itr->end();
				for (itr = text_itr->begin(); itr != end_itr; ++itr)	{
					if (draw_x >= draw_width)	{	break;	}
					if (*itr == _T('\t'))	{
						x_space = work.moji_state.x_space  * 2;
						if ((draw_x + x_space) >= 0)	{
							x1 = draw_x;
							x2 = x1 + x_space;
							draw_target.draw_fill_rect(x1, y1, x2, y2, brush_back_ground);
						}
						draw_x += x_space;
					}
					else	{
						x_space = is_zenkaku(*itr) ? work.moji_state.x_space : work.moji_state.x_space / 2;
						if ((draw_x + x_space) >= 0)	{
							x1 = draw_x;
							x2 = x1 + x_space;
							draw_target.draw_fill_rect(x1, y1, x2, y2, brush_back_ground);
							draw_target.draw_moji(*itr, draw_x, draw_y + 1, work.moji_state.h_font, rgb_moji_color);		// draw_y + 1 = �Z���^�[�z�u
						}
						draw_x += x_space;
					}
				}

				// �s�̕����ȍ~�̗]�������̓h��Ԃ�
				x1 = x2;
				x2 = draw_width;
				draw_target.draw_fill_rect(x1, y1, x2, y2, brush_back_ground);

			}

			// ���r���[�ȕ`��̏ꍇ�A�e�L�X�g���� 0 �ɕ␳����
			if (y2 >= draw_height)	{
				*now_state_itr = 0;
			}
		}
		else	{
			// �͈͊O
			x1 = guidbar_width;
			x2 = draw_width;
			draw_target.draw_fill_rect(x1, y1, x2, y2, brush_outside);
		}
		draw_y += y_space;
		line_no++;
	}

	// �u���V�J��
	DeleteObject(brush_outside);
	DeleteObject(brush_guidbar);
	DeleteObject(brush_white);
	DeleteObject(brush_light_yellow);
	DeleteObject(brush_light_blue);
	DeleteObject(brush_light_red);

	// �e�L�X�g���̃R�s�[
	seen_state_itr->copy_text_state();
}

// ****************************************************************
// �������
// ================================================================

// �R���X�g���N�^
C_db_wnd_info_trace_text_window::MOJI_STATE::MOJI_STATE()
{
	init();
}

// �f�X�g���N�^
C_db_wnd_info_trace_text_window::MOJI_STATE::~MOJI_STATE()
{
	free();
}

// ������
void C_db_wnd_info_trace_text_window::MOJI_STATE::init()
{
	font_name.clear();
	font_size = 0;
	h_font = NULL;
}

// �J��
void C_db_wnd_info_trace_text_window::MOJI_STATE::free()
{
	if (h_font)	{	DeleteObject(h_font);	}
	init();
}

// �ݒ�
bool C_db_wnd_info_trace_text_window::MOJI_STATE::set(CTSTR& name, int size)
{
	if (font_name == name && font_size == size)	{	return false;	}

	free();

	font_name = name;
	font_size = size;
	x_space = font_size + 2;
	y_space = font_size + 2;

	h_font = CreateFont(
		font_size, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, font_name.c_str());

	return true;
}


// ****************************************************************
// �`����\����
// ================================================================

// �R���X�g���N�^
C_db_wnd_info_trace_text_window::DRAW_STATE::DRAW_STATE()
{
	init();
}

// ������
void C_db_wnd_info_trace_text_window::DRAW_STATE::init()
{
	scn_no = -1;					// �V�[���ԍ�
	target_line_no = -1;			// �^�[�Q�b�g�̍s�ԍ�
	focus_line_no = -1;				// �t�H�[�J�X�̍s�ԍ�
	in_line_no = -1;				// �h�m�̍s�ԍ�
	out_line_no = -1;				// �n�t�s�̍s�ԍ�
	draw_top_line_no = -99999;		// �`��̐擪�s�ԍ�
	draw_end_line_no = -1;			// �`��̍ŏI�s�ԍ�
	draw_line_cnt = 0;				// �`��̍s��
}


// ****************************************************************
// �V�[���t�@�C���\����
// ================================================================

// �R���X�g���N�^
C_db_wnd_info_trace_text_window::SEEN_STATE::SEEN_STATE()
{
	init();
}

// �f�X�g���N�^
C_db_wnd_info_trace_text_window::SEEN_STATE::~SEEN_STATE()
{
	free();
}

// ������
void C_db_wnd_info_trace_text_window::SEEN_STATE::init()
{
	text_data.clear();
	text_state_now.clear();
	text_state_last.clear();
	draw_cnt = 0;
	draw_state.init();
}

// �J��
void C_db_wnd_info_trace_text_window::SEEN_STATE::free()
{
	init();
}

// �ǂݍ���
bool C_db_wnd_info_trace_text_window::SEEN_STATE::load(int scn_no)
{
	if (draw_state.scn_no == scn_no)	{	return false;	}
	free();
	draw_state.scn_no = scn_no;
	C_file::read_full_text(tnm_get_scn_full_path_name(scn_no), text_data);
	if (!text_data.empty())	{
		text_state_now.resize(text_data.size());
		text_state_last.resize(text_data.size());
	}
	return true;
}

// �e�L�X�g���̃R�s�[
void C_db_wnd_info_trace_text_window::SEEN_STATE::copy_text_state()
{
	if (text_state_now.empty())	{	return;	}
	int size = sizeof(text_state_now[0]) * (int)text_state_now.size();
	CopyMemory(text_state_last.get(), text_state_now.get(), size);
	ZeroMemory(text_state_now.get(), size);
//	text_state_last = text_state_now;
//	text_state_now.clear();
//	text_state_now.resize(text_state_last.size());
}

// ****************************************************************
// �c�X�N���[���o�[�F�E�B���h�E�v���V�[�W��
// ================================================================
LRESULT C_db_wnd_info_trace_text_window::Citem_v_scroll::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
//	switch (msg)	{
//	}
	return Cc_scroll::window_proc(msg, wp, lp);
}

// ****************************************************************
// ���X�N���[���o�[�F�E�B���h�E�v���V�[�W��
// ================================================================
LRESULT C_db_wnd_info_trace_text_window::Citem_h_scroll::window_proc(UINT msg, WPARAM wp, LPARAM lp)
{
//	switch (msg)	{
//	}
	return Cc_scroll::window_proc(msg, wp, lp);
}

