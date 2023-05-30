#pragma		once

#include	"tnm_exe_angou.h"

// ****************************************************************
// �d�w�d�Z�b�g�Í�
// ================================================================

#define		TNM_EXE_ANGOU_FIND_MAX_CNT			10

#define		TNM_EXE_ANGOU_STR_MIN_LEN			8
#define		TNM_EXE_ANGOU_STR_MAX_LEN			64

class C_tnms_exe_angou
{

public:
	static		bool		check(CTSTR& exe_path);									// �d�w�d���Í�������Ă��邩���肷��
	static		bool		angou(HWND h_wnd, CTSTR& exe_path, CTSTR& angou_str);	// �d�w�d���Í�������

	static		void		make_angou_element(BYTE *angou_str, int angou_str_size, BYTE *ret_angou_element);	// �Í��v�f�쐬�i�o�C�g�����񂩂�쐬�j
	static		void		make_angou_element(CTSTR& angou_str, BYTE *ret_angou_element);						// �Í��v�f�쐬�iTSTR�^�����񂩂�쐬�j

private:
	static		int			find_angou_insert_address_func(BYTE *exe_p, int exe_size, int *ret_find_code_count, BYTE *ap[TNM_EXE_ANGOU_FIND_CODE_CNT][TNM_EXE_ANGOU_FIND_MAX_CNT]);		// �Í��̑}���ʒu��T���e�t�m�b
	static		int			find_angou_insert_address_funcfunc(BYTE *exe_p, int exe_size, int find_code_count, BYTE *find_code, BYTE **ap);			// �Í��̑}���ʒu��T���e�t�m�b�e�t�m�b

	static		int			angou_func(HWND h_wnd, CTSTR& exe_path, CTSTR& angou_str);							// �d�w�d���Í�������e�t�m�b
	static		void		err_code_msgbox(HWND h_wnd, CTSTR& exe_path, CTSTR& angou_str, int err_code);		// �G���[�R�[�h���b�Z�[�W�{�b�N�X
};
