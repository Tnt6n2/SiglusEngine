#pragma		once

#include	"data/tnm_dlg.h"

// ****************************************************************
// �f�o�b�O�E�B���h�E
// ================================================================
class C_db_wnd_info : public C_tnm_dlg
{

public:

protected:

};

// ****************************************************************
// �f�o�b�O�p�̃��X�g�R���g���[��
// ================================================================
class C_tnm_dbg_listbox : public Cc_listbox
{
public:
	int		add_item(CTSTR& str);				// �A�C�e����K���Ȉʒu�ɒǉ�
	int		insert_item(int pos, CTSTR& str);	// �A�C�e�����ʒu���w�肵�Ēǉ��i-1 �Ŗ����j�iLBS_SORT ���w�肳��Ă��Ȃ����Ɓj
};

// ****************************************************************
// �f�o�b�O�p�̃��X�g�r���[
// ================================================================
class C_tnm_dbg_listview : public Cc_listview
{
public:
	int		add_item();														// �A�C�e����ǉ�
	int		add_item(int column_no, CTSTR& str);							// �A�C�e����ǉ�
	int		add_item_visible(int column_no, CTSTR& str);					// �A�C�e����ǉ��A���͈͂ɗ���悤�ɃX�N���[��
	int		insert_item(int item_no);										// �A�C�e����}��
	int		insert_item(int item_no, int column_no, CTSTR& str);			// �A�C�e����}��
	int		insert_item_visible(int item_no, int column_no, CTSTR& str);	// �A�C�e����}���A���͈͂ɗ���悤�ɃX�N���[��
	void	reduce_item(int item_cnt);										// �A�C�e�����팸
	void	set_item_cnt(int item_cnt);										// �A�C�e������ݒ�i���₵���茸�炵���肷��j

	void	set_cell_text(int item_no, int column_no, CTSTR& str);			// �e�L�X�g��ݒ�
	void	set_cell_text_visible(int item_no, int column_no, CTSTR& str);	// �e�L�X�g��ݒ肵�ăA�C�e�������͈͂Ɉړ�
};

