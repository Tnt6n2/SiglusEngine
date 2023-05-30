#pragma		once

// ****************************************************************
// �f�o�b�O���E�B���h�E�F�`���
// ================================================================
class C_db_wnd_info_trace_draw_target : public Cc_picture
{
public:
	C_db_wnd_info_trace_draw_target();						// �R���X�g���N�^
	~C_db_wnd_info_trace_draw_target();						// �f�X�g���N�^
	bool		alloc_text_bmp();							// �e�L�X�g�a�l�o�F�m��
	void		draw_fill_rect(int x1, int y1, int x2, int y2, HBRUSH brush);				// ��`�`��
	void		draw_fill_rect(int x1, int y1, int x2, int y2, BYTE r, BYTE g, BYTE b);		// ��`�`��
	void		copy_rect(int x1, int y1, int x2, int y2, int dst_x, int dst_y);			// ��`�R�s�[
	void		draw_moji(TCHAR moji, int x, int y, HFONT hfont, COLORREF rgb);				// �����`��i�X�V�����͍s���܂���j
	void		draw_moji(TCHAR moji, int x, int y, HFONT hfont, BYTE r, BYTE g, BYTE b);	// �����`��i�X�V�����͍s���܂���j

	C_dib		text_bmp;
	HDC			text_mdc;

private:
	void		init_text_bmp();							// �e�L�X�g�a�l�o�F������
	void		free_text_bmp();							// �e�L�X�g�a�l�o�F�J��

protected:
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);
};


