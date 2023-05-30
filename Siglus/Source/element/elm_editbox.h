#pragma		once

#include	"element/elm_list.h"

// ****************************************************************
// �G�f�B�b�g�{�b�N�X�F�萔
// ================================================================
const int	TNM_EDITBOX_NOT_DECIDED = 0;
const int	TNM_EDITBOX_DECIDED = 1;
const int	TNM_EDITBOX_CANCELED = -1;

// ****************************************************************
// �G�f�B�b�g�{�b�N�X
// ================================================================
class C_tnm_editbox : public Cc_edit
{
public:
	void		init()				{	m_action_flag = TNM_EDITBOX_NOT_DECIDED;	}
	void		clear_input()		{	m_action_flag = TNM_EDITBOX_NOT_DECIDED;	}
	int			get_action_flag()	{	return m_action_flag;						}

protected:
	LRESULT		window_proc(UINT msg, WPARAM wp, LPARAM lp);

private:
	int			m_action_flag;
};

// ****************************************************************
// �G�������g�F�G�f�B�b�g�{�b�N�X
// ================================================================
class C_elm_editbox : public C_tnm_element
{
public:
	C_elm_editbox();
	~C_elm_editbox();

	void	init();								// ������
	void	reinit();							// �ď�����
	void	save(C_tnm_save_stream& stream);	// �Z�[�u
	void	load(C_tnm_save_stream& stream);	// ���[�h
	void	frame();							// �t���[������

	void	create(C_rect rect, int moji_size);	// �쐬
	void	destroy();							// �j��
	void	update_rect();						// ��`���X�V
	void	set_text(CTSTR& text);				// �e�L�X�g��ݒ�
	TSTR	get_text();							// �e�L�X�g���擾
	void	set_focus();						// �t�H�[�J�X��ݒ�
	void	clear_input();						// ���͂��N���A

	// �쐬����Ă��邩�𔻒�
	bool is_created()	{	return m_created;	}

	// ���肳��Ă��邩�𔻒�
	bool is_decided()	{	return m_editbox.get_action_flag() == TNM_EDITBOX_DECIDED;		}

	// �L�����Z������Ă��邩�𔻒�
	bool is_canceled()	{	return m_editbox.get_action_flag() == TNM_EDITBOX_CANCELED;		}

	// �G�f�B�b�g�{�b�N�X�{�̂��擾
	C_tnm_editbox& editbox()	{	return m_editbox;	}

private:
	void	release_font();

	bool			m_created;
	C_rect			m_rect;
	int				m_moji_size;
	HFONT			m_h_font;
	C_tnm_editbox	m_editbox;
};

// ****************************************************************
// �G�������g�F�G�f�B�b�g�{�b�N�X���X�g
// ================================================================
class C_elm_editbox_list : public C_elm_list<C_elm_editbox>
{
protected:
	void	_init(int begin, int end);								// �T�u�̏�����
	void	_reinit(int begin, int end);							// �T�u�̍ď�����
	void	_save(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃Z�[�u
	void	_load(C_tnm_save_stream& stream, int begin, int end);	// �T�u�̃��[�h

};

