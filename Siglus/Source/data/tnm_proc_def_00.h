
// �v���Z�X��`
// �ԍ����΂��Ă͍s���܂���I�z��̃C���f�b�N�X������Ă��܂��܂��B

PROC(NONE,					0,  false)		// �������Ȃ�
PROC(SCRIPT,				1,  false)		// �X�N���v�g���s��
PROC(DISP,					2,  false)		// ��ʂ��X�V����
PROC(GAME_START_WIPE,		3,  false)		// �Q�[���J�n���̃��C�v
PROC(GAME_END_WIPE,			4,  false)		// �Q�[���I�����̃��C�v
PROC(GAME_TIMER_START,		5,  false)		// �Q�[���^�C�}�[�J�n
PROC(LOAD,					6,  false)		// ���[�h
PROC(QUICK_LOAD,			7,  false)		// �N�C�b�N���[�h
PROC(RETURN_TO_MENU,		8,  false)		// ���j���[�ɖ߂�
PROC(RETURN_TO_SEL,			9,  false)		// �O�̑I�����ɖ߂�
PROC(DUMMY_10,				10, false)		// �i���ԁj
PROC(DUMMY_11,				11, false)		// �i���ԁj
PROC(DUMMY_12,				12, false)		// �i���ԁj
PROC(DUMMY_13,				13, false)		// �i���ԁj
PROC(SEL,					14, false)		// �I����
PROC(SEL_BTN,				15, false)		// �{�^���I����
PROC(SEL_BTN_OBJ,			16, false)		// �{�^���I�u�W�F�N�g�I����
PROC(MSG_BACK,				17, false)		// ���b�Z�[�W�o�b�N
PROC(COMMAND,				18, false)		// �R�}���h�����s����
PROC(KEY_WAIT,				19, true)		// �L�[�҂�
PROC(TIME_WAIT,				20, true)		// ���ԑ҂�
PROC(COUNT_WAIT,			21, true)		// �J�E���^�҂�
PROC(MWND_OPEN_WAIT,		22, true)		// ���b�Z�[�W�E�B���h�E�J���A�j���҂�
PROC(MWND_CLOSE_WAIT,		23, true)		// ���b�Z�[�W�E�B���h�E���A�j���҂�
PROC(MWND_CLOSE_WAIT_ALL,	24, true)		// ���b�Z�[�W�E�B���h�E���A�j���҂�
PROC(MESSAGE_WAIT,			25, false)		// ���b�Z�[�W�\���I���҂��i����F���b�Z�[�W�\���J�n���ɃN���A����j
PROC(MESSAGE_KEY_WAIT,		26, true)		// ���b�Z�[�W�L�[�҂��i�L�[�҂��A�C�R����\������j
PROC(BGM_WAIT,				27, true)		// �a�f�l�҂�
PROC(BGM_FADE_OUT_WAIT,		28, true)		// �a�f�l�t�F�[�h�A�E�g�҂�
PROC(KOE_WAIT,				29, true)		// ���҂�
PROC(PCM_WAIT,				30, true)		// ���ʉ��҂�
PROC(PCMCH_WAIT,			31, true)		// ���ʉ��`�����l���҂�
PROC(PCMCH_FADE_OUT_WAIT,	32, true)		// ���ʉ��`�����l���t�F�[�h�A�E�g�҂�
PROC(PCM_EVENT_WAIT,		33, true)		// ���ʉ��C�x���g�҂�
PROC(MOV_WAIT,				34, true)		// ���[�r�[�҂�
PROC(WIPE_WAIT,				35, true)		// ���C�v�҂�
PROC(SHAKE_WAIT,			36, true)		// �V�F�C�N�҂�
PROC(QUAKE_WAIT,			37, true)		// �N�G�C�N�҂�
PROC(EVENT_WAIT,			38, true)		// �C�x���g�҂�
PROC(ALL_EVENT_WAIT,		39, true)		// �S�̃C�x���g�҂�
PROC(OBJ_MOV_WAIT,			40, true)		// ���[�r�[�I�u�W�F�N�g�҂�
PROC(SAVE_DIALOG,			41, false)		// �Z�[�u�_�C�A���O
PROC(END_LOAD,				42, false)		// �G���h���[�h
PROC(END_SAVE,				43, false)		// �G���h�Z�[�u
PROC(END_GAME,				44, false)		// �Q�[���I��
PROC(START_WARNING,			45, false)		// �N������x��
PROC(CAPTURE_ONLY,			46, false)		// �L���v�`���[
PROC(INNER_LOAD,			47, false)		// �C���i�[���[�h
PROC(EASY_LOAD,				48, false)		// �ȈՃ��[�h
PROC(BACKLOG_LOAD,			49, false)		// �o�b�N���O���[�h
PROC(TWEET_CAPTURE_AFTER,   50, false)		// �c�C�[�g�p�̃L���v�`���[�̌�
PROC(OBJ_EMOTE_WAIT,		51, true)		// �I�u�W�F�N�g E-mote �҂�
PROC(MAX, 52, false)		// 
