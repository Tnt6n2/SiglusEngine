#pragma		once


// ****************************************************************
// �}�E�X�J�[�\��
// ================================================================
//		�J���[�ɂ��邽�߁A�J�[�\�������ǃA�C�R�����g���̂ł��B
// ================================================================
struct S_cursor
{
	S_cursor();
	~S_cursor();

	HICON		h_icon;
	HBITMAP		h_mask;

private:
	// �R�s�[�֎~
	S_cursor(S_cursor&);
	S_cursor& operator = (S_cursor&);
};

class C_tnm_cursor
{
public:
	C_tnm_cursor();

	void	create_cursor(int cursor_no);

public:
	BSA<S_cursor>	m_cursor_pat_list;
	int				m_cursor_pat_cnt;
};

class C_tnm_cursor_list
{
public:
	ARRAY<C_tnm_cursor>		sub;
};

// ****************************************************************
// �}�E�X�J�[�\���F�t���[������
// ================================================================
void	tnm_cursor_frame(int cur_time);
