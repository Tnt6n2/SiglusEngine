#pragma		once

// ****************************************************************
// �R�[�h
// ================================================================

#ifdef __SIGLUS
#include	"tnm_code.h"
#include	"tnm_form_code.h"
#include	"tnm_element_code.h"
#endif

// Kinetic �̈ꕔ�͋��L�ł��Ă܂���B�������|���I�I�I�I

#ifdef __KINETIC

// �R�[�h�� Siglus ���g���܂킵�i�댯�I�j
#include	"tnm_code.h"

// �t�H�[���R�[�h�͋��L���ł��Ă܂��B
#define		FORM(name, code)	\
const int	FM_##name = code;
#include	"def_form_Kinetic.h"
#undef		FORM

// �G�������g�͋��L���ł��ĂȂ��̂Œ��ł��ł��B
#define		create_elm_code(owner, group, code)		(((owner) << 24) + ((group) << 16) + (code))
#define		get_elm_owner(code)						(((code) >> 24) & 0x00ff)
#define		get_elm_group(code)						(((code) >> 16) & 0x00ff)
#define		get_elm_code(code)						(((code) >>  0) & 0xffff)

const int	ELM_OWNER_USER_PROP = 127;		// ���[�U�v���p�e�B
const int	ELM_OWNER_USER_CMD = 126;		// ���[�U�R�}���h
const int	ELM_OWNER_CALL_PROP = 125;		// �R�[���v���p�e�B
const int	ELM_OWNER_CALL_CMD = 124;		// �R�[���R�}���h�i���݂��Ȃ��j
const int	ELM_ARRAY = -1;					// �z��
//const int	ELM___SET = -2;					// ���
//const int	ELM___TRANS = -3;				// �ϊ�
const int	ELM_CURRENT = -4;				// �J�����g
const int	ELM_UP = -5;					// �J�����g���P��

// �G�������g�R�[�h�͋��L���ł��Ă��܂��B
#define		ELEMENT(type, parent, form, name, owner, group, code, arg_str)	\
const int	ELM_##parent##_##name = ((owner << 24) + (group << 16) + code);
#include	"def_element_Kinetic.h"
#undef		ELEMENT

#endif

