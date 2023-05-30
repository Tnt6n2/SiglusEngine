
#include	<windows.h>
#include	<tchar.h>
#include	<string>
#include	<commctrl.h>

typedef	std::basic_string<char>		MBSTR;	// �}���`�o�C�g������
typedef	std::basic_string<wchar_t>	WSTR;	// ���C�h������
typedef	std::basic_string<TCHAR>	TSTR;	// ������
typedef const MBSTR					CMBSTR;
typedef const WSTR					CWSTR;
typedef const TSTR					CTSTR;

// ****************************************************************
// �������� dll
// ================================================================
#include	<tnmd.h>

static	HINSTANCE	g_h_inst = NULL;
static	int			g_major_version = 3;
static	int			g_minor_version = 0;

// ****************************************************************
// dll �G���g��
// ================================================================
extern "C" int APIENTRY DllMain(HINSTANCE h_inst, DWORD reason, LPVOID reserved)
{
	g_h_inst = h_inst;

	return TRUE;
}

// ****************************************************************
// dll �̃o�[�W�������擾
// ================================================================
int get_major_version(char* p_password_buf)
{
	memcpy(p_password_buf, "aQpbZF$ijgDSLL19hY#)An36;Ij@ln+G", 32);
	return g_major_version;
}
int get_minor_version(char* p_key_buf)
{
	memcpy(p_key_buf, "7hG)o!bNa>tvV{wcc-D>EXpPfq7Bn*sU", 32);
	return g_minor_version;
}

// ****************************************************************
// �ėp�F���b�Z�[�W�𑗂�
// ================================================================
int send_message(HWND h_wnd, UINT msg, WPARAM wp, LPARAM lp)
{
	return (int)SendMessage(h_wnd, msg, wp, lp);
}

// ****************************************************************
// ���X�g�{�b�N�X�F�A�C�e���̒ǉ�
// ================================================================
int lb_add_item_001(HWND h_lb, CTSTR &str)
{
	return (int)send_message(h_lb, LB_ADDSTRING, 0, (LPARAM)str.c_str());
}

// ****************************************************************
// ���X�g�{�b�N�X�F�A�C�e���̒ǉ��i�ʒu���w��j
// ================================================================
int lb_insert_item_001(HWND h_lb, int pos, CTSTR &str)
{
	return (int)send_message(h_lb, LB_INSERTSTRING, pos, (LPARAM)str.c_str());
}

// ****************************************************************
// ���X�g�r���[�F���͈͂ɓ���悤�ɃX�N���[��
// ================================================================
void lv_set_item_visible_001(HWND h_lv, int item_no)
{
	send_message(h_lv, LVM_ENSUREVISIBLE, item_no, TRUE);
}

// ****************************************************************
// ���X�g�r���[�F�A�C�e�����̎擾
// ================================================================
int lv_get_item_cnt(HWND h_lv)
{
	return send_message(h_lv, LVM_GETITEMCOUNT, 0, 0);
}

// ****************************************************************
// ���X�g�r���[�F�e�L�X�g�̎擾
// ================================================================
TSTR lv_get_cell_text_001(HWND h_lv, int item_no, int column_no)
{
	TCHAR t_str[MAX_PATH];

	LV_ITEM item;
	item.mask = LVIF_TEXT | LVIF_PARAM;
	item.iSubItem   = column_no;
	item.pszText    = t_str;
	item.cchTextMax = MAX_PATH;
	send_message(h_lv, LVM_GETITEMTEXT, item_no, (LPARAM)&item);

	return t_str;
}

// ****************************************************************
// ���X�g�r���[�F�e�L�X�g��ݒ�
// ================================================================
void lv_set_cell_text_001(HWND h_lv, int item_no, int column_no, CTSTR& str)
{
	if (lv_get_cell_text_001(h_lv, item_no, column_no) == str)
		return;

	LV_ITEM	item;
	item.mask     = LVIF_TEXT;
	item.iSubItem = column_no;
	item.pszText  = (LPTSTR)str.c_str();
	send_message(h_lv, LVM_SETITEMTEXT, item_no, (LPARAM)&item);
}

// ****************************************************************
// ���X�g�r���[�F�e�L�X�g��ݒ肵�ĉ��͈͂ɃX�N���[��
// ================================================================
void lv_set_cell_text_visible_001(HWND h_lv, int item_no, int column_no, CTSTR& str)
{
	lv_set_cell_text_001(h_lv, item_no, column_no, str);
	lv_set_item_visible_001(h_lv, item_no);
}

// ****************************************************************
// ���X�g�r���[�F�A�C�e���̑}��
// ================================================================
int	lv_insert_item_001(HWND h_lv, int item_no)
{
	LV_ITEM item;
	item.mask     = 0;				// ���ڂ��w�肵�Ȃ�
	item.iItem    = item_no;		// �}���ʒu
	item.iSubItem = 0;				// �T�u�A�C�e���ԍ�

	return (int)send_message(h_lv, LVM_INSERTITEM, 0, (LPARAM)&item);
}

int	lv_insert_item_002(HWND h_lv, int item_no, int column_no, CTSTR& str)
{
	item_no = lv_insert_item_001(h_lv, item_no);
	lv_set_cell_text_001(h_lv, item_no, column_no, str);

	return item_no;
}

int	lv_insert_item_visible_001(HWND h_lv, int item_no, int column_no, CTSTR& str)
{
	item_no = lv_insert_item_001(h_lv, item_no);
	lv_set_cell_text_visible_001(h_lv, item_no, column_no, str);

	return item_no;
}

// ****************************************************************
// ���X�g�r���[�F�A�C�e���̒ǉ�
// ================================================================
int	lv_add_item_001(HWND h_lv)
{
	return lv_insert_item_001(h_lv, lv_get_item_cnt(h_lv));
}

int	lv_add_item_002(HWND h_lv, int column_no, CTSTR& str)
{
	int item_no = lv_add_item_001(h_lv);
	lv_set_cell_text_001(h_lv, item_no, column_no, str);

	return item_no;
}

int	lv_add_item_visible_001(HWND h_lv, int column_no, CTSTR& str)
{
	int item_no = lv_add_item_001(h_lv);
	lv_set_cell_text_visible_001(h_lv, item_no, column_no, str);

	return item_no;
}

// ****************************************************************
// ���X�g�r���[�F�A�C�e�����̏k��
// ================================================================
void lv_reduce_item_001(HWND h_lv, int item_cnt)
{
	int item_no = item_cnt;
	while (send_message(h_lv, LVM_DELETEITEM, item_no, 0));
}

// ****************************************************************
// ���X�g�r���[�F�A�C�e�����̐ݒ�
// ================================================================
void lv_set_item_cnt_001(HWND h_lv, int item_cnt)
{
	int item_cnt_old = lv_get_item_cnt(h_lv);

	// �A�C�e���������Ȃ��Ƃ�
	if (item_cnt_old < item_cnt)	{
		for (int i = 0; i < item_cnt - item_cnt_old; i++)	{
			lv_add_item_001(h_lv);
		}
	}
	// �A�C�e�����������Ƃ�
	else if (item_cnt_old > item_cnt)	{
		lv_reduce_item_001(h_lv, item_cnt);
	}
}
