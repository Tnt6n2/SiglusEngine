#pragma		once

// ****************************************************************
// �G���[�^�C�v
// ================================================================
enum TNM_ERROR_TYPE
{
	TNM_ERROR_TYPE_NONE,	// �G���[�Ȃ�

	TNM_ERROR_TYPE_INFO,	// �K���G���[�{�b�N�X���o���ׂ�����
	TNM_ERROR_TYPE_FATAL,	// �K���G���[�{�b�N�X���o���ׂ�����
	TNM_ERROR_TYPE_DEBUG,	// �f�o�b�O�ł̂݃G���[�{�b�N�X���o���ׂ�����
	TNM_ERROR_TYPE_WARNING,	// �f�o�b�O�_�C�A���O�ɕ\�����邾���̂���
	TNM_ERROR_TYPE_SUCCESS,	// �f�o�b�O�_�C�A���O�ɕ\�����邾���̂���

	TNM_ERROR_TYPE_DIVIDED_BY_ZERO,		// �O���Z
	TNM_ERROR_TYPE_OUT_OF_RANGE,		// �͈͊O�A�N�Z�X
	TNM_ERROR_TYPE_OVERFLOW,			// �I�[�o�[�t���[
	TNM_ERROR_TYPE_FILE_NOT_FOUND,		// �t�@�C���G���[
	TNM_ERROR_TYPE_PCT_NOT_FOUND,		// �摜�t�@�C���G���[
	TNM_ERROR_TYPE_SOUND_NOT_FOUND,		// ���t�@�C���G���[
	TNM_ERROR_TYPE_KOE_NOT_FOUND,		// ���t�@�C���G���[
	TNM_ERROR_TYPE_PAT_NO,				// �p�^�[���ԍ��G���[
};

// ****************************************************************
// �ėp
// ================================================================
const int	TNM_USE_DEFAULT = -2;
const int	TNM_EMPTY = -1;

// ****************************************************************
// �P��
// ================================================================
const int	TNM_SCALE_UNIT = 1000;
const int	TNM_ANGLE_UNIT = 10;
const int	TNM_WORLD_UNIT = 1;

// ****************************************************************
// �t���O���i�E�B���h�E���j���[��f�o�b�O�E�B���h�E�̌��Ɋւ��j
// ================================================================
const int	TNM_FLAG_CNT = 13;		// A,B,C,D,E,F,G,Z,X,S,M,L,K

// ****************************************************************
// ���[���h
// ================================================================
const int	TNM_WORLD_NONE = -1;

// ****************************************************************
// �X�e�[�W
// ================================================================
const int	TNM_STAGE_BACK = 0;
const int	TNM_STAGE_FRONT = 1;
const int	TNM_STAGE_NEXT = 2;
const int	TNM_STAGE_CNT = 3;

// ****************************************************************
// �g�[���J�[�u
// ================================================================
const int	TNM_TONE_CURVE_NONE = -1;

// ****************************************************************
// �}�X�N
// ================================================================
const int	TNM_MASK_NONE = -1;

// ****************************************************************
// ���C�g
// ================================================================
const int	TNM_LIGHT_NONE = -1;

// ****************************************************************
// �������@
// ================================================================
const int	TNM_BLEND_TYPE_NORMAL = 0;
const int	TNM_BLEND_TYPE_ADD    = 1;
const int	TNM_BLEND_TYPE_SUB    = 2;
const int	TNM_BLEND_TYPE_MUL    = 3;
const int	TNM_BLEND_TYPE_SCREEN = 4;
const int	TNM_BLEND_TYPE_MAX    = 5;

// ****************************************************************
// �e�N�X�`���X�e�[�W
// ================================================================
const int	TNM_TEXTURE_STAGE_PCT			= 0;
const int	TNM_TEXTURE_STAGE_MASK			= 1;
const int	TNM_TEXTURE_STAGE_TONE_CURVE	= 2;
const int	TNM_TEXTURE_STAGE_FOG			= 3;

// ****************************************************************
// �{�^���̏��
// ================================================================
const int	TNM_BTN_STATE_NORMAL   = 0;
const int	TNM_BTN_STATE_HIT      = 1;
const int	TNM_BTN_STATE_PUSH     = 2;
const int	TNM_BTN_STATE_SELECT   = 3;
const int	TNM_BTN_STATE_DISABLE  = 4;
const int	TNM_BTN_STATE_MAX      = 5;

// ****************************************************************
// �Z�[�u�T���l�C���̃^�C�v
// ================================================================
const int	TNM_SAVE_THUMB_TYPE_BMP = 0;
const int	TNM_SAVE_THUMB_TYPE_PNG = 1;

// ****************************************************************
// �L���v�`���[�̃^�C�v
// ================================================================
const int	TNM_CAPTURE_TYPE_NONE = 0;
const int	TNM_CAPTURE_TYPE_SAVE_THUMB = 1;
const int	TNM_CAPTURE_TYPE_OBJECT = 2;
const int	TNM_CAPTURE_TYPE_THUMB = 3;
const int	TNM_CAPTURE_TYPE_BUFFER = 4;
const int	TNM_CAPTURE_TYPE_TWEET = 5;

// ****************************************************************
// �L���v�`���[�D��x
// ================================================================
const int	TNM_CAPTURE_PRIOR_NONE		= 0;	// �L���v�`���[�Ȃ�
const int	TNM_CAPTURE_PRIOR_SAVE		= 1;	// �Z�[�u�_�C�A���O�Ȃ�
const int	TNM_CAPTURE_PRIOR_END		= 2;	// �Q�[���I��
const int	TNM_CAPTURE_PRIOR_CAPTURE	= 3;	// �L���v�`���[�R�}���h

// ****************************************************************
// �E�B���h�E�g
// ================================================================
//const int	TNM_MWND_WAKU_NONE = -1;			// �g�Ȃ�

// ****************************************************************
// ���O�\���^�C�v
// ================================================================
const int	TNM_MWND_NAME_DISP_TYPE_NAME = 0;
const int	TNM_MWND_NAME_DISP_TYPE_MSG = 1;
const int	TNM_MWND_NAME_DISP_TYPE_NONE = 2;

// ****************************************************************
// ���O���ʃ^�C�v
// ================================================================
const int	TNM_MWND_NAME_BRACKET_TYPE_NONE = 0;
const int	TNM_MWND_NAME_BRACKET_TYPE_SUMI = 1;

// ****************************************************************
// ���O�\���^�C�v�i���b�Z�[�W�o�b�N�j
// ================================================================
const int	TNM_MSGBK_NAME_DISP_TYPE_DEFAULT = -1;
const int	TNM_MSGBK_NAME_DISP_TYPE_OFF = 0;
const int	TNM_MSGBK_NAME_DISP_TYPE_ON = 1;

// ****************************************************************
// ���O���ʃ^�C�v
// ================================================================
const int	TNM_MSGBK_NAME_BRACKET_TYPE_NONE = 0;
const int	TNM_MSGBK_NAME_BRACKET_TYPE_SUMI = 1;

// ****************************************************************
// �I�����A�C�e���̏��
// ================================================================
const int	TNM_SEL_ITEM_TYPE_OFF = 0;
const int	TNM_SEL_ITEM_TYPE_ON = 1;
const int	TNM_SEL_ITEM_TYPE_READ = 2;
const int	TNM_SEL_ITEM_TYPE_HIDE = 3;

// ****************************************************************
// BGM
// ================================================================
const int	TNM_BGM_START_POS_INI = -1;	// BGM �̊J�n�ʒu�Fini �ɏ]��

// ****************************************************************
// SE �^�C�v
// ================================================================
const int	TNM_SE_TYPE_SELECT = 0;		// �I����
const int	TNM_SE_TYPE_DECIDE = 1;		// ���艹
const int	TNM_SE_TYPE_CANCEL = 2;		// �L�����Z����
const int	TNM_SE_TYPE_WARNING = 3;	// �x����
const int	TNM_SE_TYPE_SAVE = 4;		// �Z�[�u��
const int	TNM_SE_TYPE_LOAD = 5;		// ���[�h��
const int	TNM_SE_TYPE_MENU = 6;		// �^�C�g���ɖ߂鉹
const int	TNM_SE_TYPE_PREV_SEL = 7;	// �O�̑I�����ɖ߂鉹

// ****************************************************************
// �{�����[���^�C�v
// ================================================================
const int	TNM_VOLUME_TYPE_NONE = -1;
const int	TNM_VOLUME_TYPE_BGM = 0;
const int	TNM_VOLUME_TYPE_KOE = 1;
const int	TNM_VOLUME_TYPE_PCM = 2;
const int	TNM_VOLUME_TYPE_SE = 3;
const int	TNM_VOLUME_TYPE_MOV = 4;
//const int	TNM_VOLUME_TYPE_ALL = 5;		// �S�̂�5�ԂɂȂ�܂����B���r���[�ł��B
const int	TNM_VOLUME_TYPE_EXSOUND = 16;	// �ǉ��{�����[��
const int	TNM_VOLUME_TYPE_MAX = 32;

// ****************************************************************
// �V�X�e���F���[�r�[
// ================================================================
const int	TNM_MOVIE_FILE_TYPE_NONE = 0;
const int	TNM_MOVIE_FILE_TYPE_AVI = 1;
const int	TNM_MOVIE_FILE_TYPE_MPG = 2;
const int	TNM_MOVIE_FILE_TYPE_WMV = 3;

// ****************************************************************
// �V�X�e���F�Z�[�u�w�b�_
// ================================================================
const int	TNM_SAVE_APPEND_DIR_MAX_LEN = 256;
const int	TNM_SAVE_APPEND_NAME_MAX_LEN = 256;
const int	TNM_SAVE_TITLE_MAX_LEN = 256;
const int	TNM_SAVE_MESSAGE_MAX_LEN = 256;
const int	TNM_SAVE_FULL_MESSAGE_MAX_LEN = 256;
const int	TNM_SAVE_COMMENT_MAX_LEN = 256;
const int	TNM_SAVE_COMMENT2_MAX_LEN = 256;
const int	TNM_SAVE_FLAG_MAX_CNT = 256;

// ****************************************************************
// �V�X�e���F�Z�[�u����
// ================================================================
//const int	TNM_SAVE_HISTORY_MAX_CNT = 100;

// ****************************************************************
// �V�X�e���F���[�h�A�^�C�v�Ȃǂ̒�`
// ================================================================

// ��ʃ��[�h
const int	TNM_SCREEN_SIZE_MODE_WINDOW = 0;		// 50%, 75%, 100%, ...
const int	TNM_SCREEN_SIZE_MODE_FULL = 1;			// �t���X�N���[��
const int	TNM_SCREEN_SIZE_MODE_FREE = 2;			// ���R�T�C�Y

// �t���X�N���[���̕\�����[�h
const int	TNM_FULLSCREEN_MODE_NORMAL = 0;			// ����
const int	TNM_FULLSCREEN_MODE_FIT = 1;			// �S���
const int	TNM_FULLSCREEN_MODE_UDLRFIT = 2;		// �S��ʃt�B�b�g
const int	TNM_FULLSCREEN_MODE_UDFIT = 3;			// �㉺�t�B�b�g
const int	TNM_FULLSCREEN_MODE_LRFIT = 4;			// ���E�t�B�b�g
const int	TNM_FULLSCREEN_MODE_CUSTOM = 5;			// �J�X�^��

// �t�H���g�̎��
const int	TNM_FONT_TYPE_MS_GOTHIC = 0;			// �l�r�S�V�b�N
const int	TNM_FONT_TYPE_MS_MINTYOU = 1;			// �l�r����
const int	TNM_FONT_TYPE_MEIRYO = 2;				// ���C���I
const int	TNM_FONT_TYPE_ORIGINAL = 3;				// �Ǝ��t�H���g

// �t�H���g�̖��O
const TSTR	TNM_FONT_NAME_MS_GOTHIC = _T("�l�r �S�V�b�N");		// �l�r�S�V�b�N
const TSTR	TNM_FONT_NAME_MS_MINTYOU = _T("�l�r ����");			// �l�r����
const TSTR	TNM_FONT_NAME_MS_MEIRYO = _T("���C���I");			// ���C���I

// �t�H���g�̉e���[�h
const int	TNM_FONT_SHADOW_MODE_NONE = 0;				// �e�Ȃ�
const int	TNM_FONT_SHADOW_MODE_SHADOW = 1;			// �e����
const int	TNM_FONT_SHADOW_MODE_FUTIDORU = 2;			// �����
const int	TNM_FONT_SHADOW_MODE_FUTIDORU_SHADOW = 3;	// ������ĉe����

// ���Z�Đ�
const int	TNM_JITAN_RATE_NORMAL = 100;			// �W����

// �������[�h
const int	TNM_KOE_MODE_NORMAL = 0;				// ��������i���͂���j
const int	TNM_KOE_MODE_TEXTONLY = 1;				// �����Ȃ��i���͂̂݁j
const int	TNM_KOE_MODE_KOEONLY = 2;				// ��������i���͂Ȃ��j

// �L�����N�^�[�����̃`�F�b�N���[�h
const int	TNM_CHRKOE_CHECK_MODE_NONE = 0;			// ������ԂŃ`�F�b�N���Ȃ�
const int	TNM_CHRKOE_CHECK_MODE_LOOK = 1;			// ���J��ԂŃ`�F�b�N���Ȃ�
const int	TNM_CHRKOE_CHECK_MODE_AUTO = 2;			// ������ԂŃ`�F�b�N����

// ���[�r�[�̍Đ����@
const int	TNM_MOVIE_PLAY_TYPE_MCI = 0;			// MCI
const int	TNM_MOVIE_PLAY_TYPE_WMP = 1;			// Windows Media Player

// ****************************************************************
// �V�X�e���F�o�b�t�@�̌�
// ================================================================

// ���[�J���ėp�X�C�b�`�̐�
const int	TNM_LOCAL_EXTRA_SWITCH_CNT = 4;			// ���[�J���ėp�X�C�b�`�̐�

// ���[�J���ėp���[�h�̐�
const int	TNM_LOCAL_EXTRA_MODE_CNT = 4;			// ���[�J���ėp���[�h�̐�

// ���[�J���ėp���[�h�̃A�C�e�����i���ڐ��j
const int	TNM_LOCAL_EXTRA_MODE_ITEM_CNT = 8;		// ���[�J���ėp���[�h�̃A�C�e�����i���ڐ��j

// �I�u�W�F�N�g�\���̐�
const int	TNM_OBJECT_DISP_CNT = 4;				// �I�u�W�F�N�g�\���̐�

// �O���[�o���ėp�X�C�b�`�̐�
const int	TNM_GLOBAL_EXTRA_SWITCH_CNT = 4;		// �O���[�o���ėp�X�C�b�`�̐�

// �O���[�o���ėp���[�h�̐�
const int	TNM_GLOBAL_EXTRA_MODE_CNT = 4;			// �O���[�o���ėp���[�h�̐�

// �O���[�o���ėp���[�h�̃A�C�e�����i���ڐ��j
const int	TNM_GLOBAL_EXTRA_MODE_ITEM_CNT = 8;		// �O���[�o���ėp���[�h�̃A�C�e�����i���ڐ��j

// ****************************************************************
// �V�X�e���F�l�̍ŏ��l�A�ő�l�Ȃǂ̒�`
// ================================================================

// ���Z�Đ�
const int	TNM_JITAN_SPEED_MIN = 100;
const int	TNM_JITAN_SPEED_MAX = 300;

// �������x
const int	TNM_MESSAGE_SPEED_MIN = 0;
const int	TNM_MESSAGE_SPEED_MAX = 100;

// �I�[�g���[�h
const int	TNM_AUTO_MODE_MOJI_WAIT_MIN = 0;	// �I�[�g���[�h��������
const int	TNM_AUTO_MODE_MOJI_WAIT_MAX = 500;	// �I�[�g���[�h��������
const int	TNM_AUTO_MODE_MIN_WAIT_MIN = 0;		// �I�[�g���[�h�ŏ�����
const int	TNM_AUTO_MODE_MIN_WAIT_MAX = 5000;	// �I�[�g���[�h�ŏ�����

// ���̂�
const int	TNM_KOE_ONLY_WAIT = 500;			// ���̂݃��[�h�Ő��̍Đ����I����Ă���̑҂�����

// ****************************************************************
// �V�X�e���R�}���h�^�C�v
// ================================================================
const int	TNM_SYSCOM_TYPE_NONE = 0;
const int	TNM_SYSCOM_TYPE_SAVE = 1;
const int	TNM_SYSCOM_TYPE_LOAD = 2;
const int	TNM_SYSCOM_TYPE_READ_SKIP = 3;
const int	TNM_SYSCOM_TYPE_AUTO_MODE = 4;
const int	TNM_SYSCOM_TYPE_RETURN_SEL = 5;
const int	TNM_SYSCOM_TYPE_HIDE_MWND = 6;
const int	TNM_SYSCOM_TYPE_MSG_BACK = 7;
const int	TNM_SYSCOM_TYPE_KOE_PLAY = 8;
const int	TNM_SYSCOM_TYPE_QUICK_SAVE = 9;
const int	TNM_SYSCOM_TYPE_QUICK_LOAD = 10;
const int	TNM_SYSCOM_TYPE_CONFIG = 11;
const int	TNM_SYSCOM_TYPE_LOCAL_EXTRA_SWITCH = 12;
const int	TNM_SYSCOM_TYPE_LOCAL_EXTRA_MODE = 13;
const int	TNM_SYSCOM_TYPE_GLOBAL_EXTRA_SWITCH = 14;
const int	TNM_SYSCOM_TYPE_GLOBAL_EXTRA_MODE = 15;

// ****************************************************************
// �V�X�e���F�Z�b�g�A�b�v�^�C�v
// ================================================================
const int	TNM_SETUP_TYPE_NONE = -1;	// �Z�b�g�A�b�v����Ă��Ȃ�
const int	TNM_SETUP_TYPE_FULL = 0;	// ���S�Z�b�g�A�b�v
const int	TNM_SETUP_TYPE_MIN = 1;		// �ŏ��Z�b�g�A�b�v

// ****************************************************************
// �v���Z�X�^�C�v
// ================================================================
enum TNM_PROC_TYPE
{
#define		PROC(type, id, key_wait)		TNM_PROC_TYPE_##type = id,
#include	"tnm_proc_def_00.h"
#undef		PROC
};

// ****************************************************************
// �v���Z�X��
// ================================================================
const TCHAR* const TNM_PROC_STR[] = 
{
#define		PROC(type, id, key_wait)		_T(#type),
#include	"tnm_proc_def_00.h"
#undef		PROC
};

// ****************************************************************
// �v���Z�X�X�e�[�g
// ================================================================
struct S_tnm_proc_state
{
	bool				key_wait;
};

const S_tnm_proc_state TNM_PROC_STATE[] = 
{
#define		PROC(type, id, key_wait)		{key_wait},
#include	"tnm_proc_def_00.h"
#undef		PROC
};

