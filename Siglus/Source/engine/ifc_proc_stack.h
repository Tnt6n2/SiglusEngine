#pragma		once

#include	"data/tnm_proc.h"

// ****************************************************************
// �v���Z�X�X�^�b�N�F�C���^�[�t�F�[�X
// ================================================================

// �v���Z�X���v�b�V��
void	tnm_push_proc(TNM_PROC_TYPE type);

// �v���Z�X���v�b�V��
void	tnm_push_proc(TNM_PROC_TYPE type, int option);

// �v���Z�X���v�b�V��
void	tnm_push_proc(C_tnm_proc proc);

// �v���Z�X���|�b�v
void	tnm_pop_proc();
