#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_rmenu.h"
#include	"data/tnm_wnd_menu.h"
#include	"data/tnm_dlg_manager.h"
#include	"data/tnm_flag_ini.h"
#include	"engine/eng_syscom.h"
#include	"engine/eng_dialog.h"

// ****************************************************************
// �h�c��`
// ================================================================

// �V�X�e��
const int	IDM_SYSTEM									= 1000;
const int	IDM_SYSTEM_SYSCOMMENU						= 1001;
const int	IDM_SYSTEM_CONFIG							= 1002;
const int	IDM_SYSTEM_SAVE_MODAL_DIALOG				= 1003;
const int	IDM_SYSTEM_LOAD_MODAL_DIALOG				= 1004;
const int	IDM_SYSTEM_CONFIG_SOLO						= 1005;
const int	IDM_SYSTEM_CONFIG_MODAL						= 1006;
const int	IDM_SYSTEM_CONFIG_SUPPORT					= 1007;
const int	IDM_SYSTEM_DEBUG							= 1008;
const int	IDM_SYSTEM_TWEET_SOLO_DIALOG				= 1009;

const int	IDM_SYSTEM_CONFIG_SOLO_SCREEN				= 1200;
const int	IDM_SYSTEM_CONFIG_SOLO_FONT_MSGSPD			= 1201;
const int	IDM_SYSTEM_CONFIG_SOLO_FONT					= 1202;
const int	IDM_SYSTEM_CONFIG_SOLO_MSGSPD				= 1203;
const int	IDM_SYSTEM_CONFIG_SOLO_VOLUME_BGMFADE		= 1204;
const int	IDM_SYSTEM_CONFIG_SOLO_VOLUME				= 1205;
const int	IDM_SYSTEM_CONFIG_SOLO_BGMFADE				= 1206;
const int	IDM_SYSTEM_CONFIG_SOLO_KOEMODE_CHRKOE		= 1207;
const int	IDM_SYSTEM_CONFIG_SOLO_KOEMODE				= 1208;
const int	IDM_SYSTEM_CONFIG_SOLO_CHRKOE				= 1209;
const int	IDM_SYSTEM_CONFIG_SOLO_MWNDBK				= 1210;
const int	IDM_SYSTEM_CONFIG_SOLO_AUTOMODE				= 1211;
const int	IDM_SYSTEM_CONFIG_SOLO_JITAN				= 1212;
const int	IDM_SYSTEM_CONFIG_SOLO_ELSE					= 1213;
const int	IDM_SYSTEM_CONFIG_SOLO_SYSTEM				= 1214;
const int	IDM_SYSTEM_CONFIG_SOLO_MOVTYPE				= 1215;

const int	IDM_SYSTEM_CONFIG_SOLO_ALL_OPEN				= 1250;
const int	IDM_SYSTEM_CONFIG_SOLO_ALL_CLOSE			= 1251;

const int	IDM_SYSTEM_CONFIG_MODAL_SCREEN				= 1300;
const int	IDM_SYSTEM_CONFIG_MODAL_FONT_MSGSPD			= 1301;
const int	IDM_SYSTEM_CONFIG_MODAL_FONT				= 1302;
const int	IDM_SYSTEM_CONFIG_MODAL_MSGSPD				= 1303;
const int	IDM_SYSTEM_CONFIG_MODAL_VOLUME_BGMFADE		= 1304;
const int	IDM_SYSTEM_CONFIG_MODAL_VOLUME				= 1305;
const int	IDM_SYSTEM_CONFIG_MODAL_BGMFADE				= 1306;
const int	IDM_SYSTEM_CONFIG_MODAL_KOEMODE_CHRKOE		= 1307;
const int	IDM_SYSTEM_CONFIG_MODAL_KOEMODE				= 1308;
const int	IDM_SYSTEM_CONFIG_MODAL_CHRKOE				= 1309;
const int	IDM_SYSTEM_CONFIG_MODAL_MWNDBK				= 1310;
const int	IDM_SYSTEM_CONFIG_MODAL_AUTOMODE			= 1311;
const int	IDM_SYSTEM_CONFIG_MODAL_JITAN				= 1312;
const int	IDM_SYSTEM_CONFIG_MODAL_ELSE				= 1313;
const int	IDM_SYSTEM_CONFIG_MODAL_SYSTEM				= 1314;
const int	IDM_SYSTEM_CONFIG_MODAL_MOVTYPE				= 1315;

const int	IDM_SYSTEM_DEBUG_SYSCOMMENU_FULL_MODE		= 1400;
const int	IDM_SYSTEM_DEBUG_CONFIG_FULL_MODE			= 1401;
const int	IDM_SYSTEM_DEBUG_SAVE_SOLO_DIALOG			= 1402;
const int	IDM_SYSTEM_DEBUG_LOAD_SOLO_DIALOG			= 1403;

// �f�o�b�O
const int	IDM_DEBUG									= 2000;
const int	IDM_DEBUG_RESTART							= 2001;		// �ŏ�����n�߂�
const int	IDM_DEBUG_TRACE								= 2002;		// �g���[�X
const int	IDM_DEBUG_DRAW_OBJECT_HIT_RECT				= 2003;		// �I�u�W�F�N�g�������`�̕\��

// �E�B���h�E
const int	IDM_WINDOW									= 3000;
const int	IDM_WINDOW_DB_WND_INFO_DEBUG				= 3001;		// �f�o�b�O�ݒ�
const int	IDM_WINDOW_DB_WND_INFO_SWITCH				= 3002;		// �X�C�b�`
const int	IDM_WINDOW_DB_WND_INFO_SCN_CONTROL			= 3003;		// �V�[���R���g���[��
const int	IDM_WINDOW_DB_WND_INFO_CALL_HISTORY			= 3004;		// �Ăяo������
const int	IDM_WINDOW_DB_WND_INFO_SAVE_HISTORY			= 3005;		// �Z�[�u�|�C���g����
const int	IDM_WINDOW_DB_WND_INFO_TRACE				= 3006;		// �g���[�X
const int	IDM_WINDOW_DB_WND_INFO_ERROR				= 3007;		// �G���[���
const int	IDM_WINDOW_DB_WND_INFO_SYSTEM				= 3008;		// �V�X�e�����
const int	IDM_WINDOW_DB_WND_INFO_INPUT				= 3009;		// ���͏��
const int	IDM_WINDOW_DB_WND_INFO_SCRIPT				= 3010;		// �X�N���v�g���
const int	IDM_WINDOW_DB_WND_INFO_NAMAE				= 3011;		// �y���O�z���
const int	IDM_WINDOW_DB_WND_INFO_READ_FLAG			= 3012;		// ���ǃt���O���
const int	IDM_WINDOW_DB_WND_INFO_CG_TABLE				= 3013;		// �b�f�e�[�u�����
const int	IDM_WINDOW_DB_WND_INFO_BGM_TABLE			= 3014;		// �a�f�l�e�[�u�����
const int	IDM_WINDOW_DB_WND_INFO_ELEMENT				= 3015;		// �G�������g���
const int	IDM_WINDOW_DB_WND_INFO_FLAG					= 3016;		// �t���O���
const int	IDM_WINDOW_DB_WND_INFO_FLAG_INI				= 3017;		// Flag.ini ���
const int	IDM_WINDOW_DB_WND_INFO_USER_FLAG			= 3018;		// ���[�U�t���O���
const int	IDM_WINDOW_DB_WND_INFO_USER_CALL_FLAG		= 3019;		// ���[�U�R�[���t���O���
const int	IDM_WINDOW_DB_WND_INFO_NAMAE_FLAG			= 3020;		// ���O�t���O���
const int	IDM_WINDOW_DB_WND_INFO_COUNTER				= 3021;		// �J�E���^���
const int	IDM_WINDOW_DB_WND_INFO_BUTTON				= 3022;		// �{�^�����
const int	IDM_WINDOW_DB_WND_INFO_GROUP				= 3023;		// �O���[�v���
const int	IDM_WINDOW_DB_WND_INFO_WORLD				= 3024;		// ���[���h���
const int	IDM_WINDOW_DB_WND_INFO_OBJECT				= 3025;		// �I�u�W�F�N�g���
const int	IDM_WINDOW_DB_WND_INFO_MWND					= 3026;		// ���b�Z�[�W�E�B���h�E���
const int	IDM_WINDOW_DB_WND_INFO_SOUND				= 3027;		// �T�E���h���
const int	IDM_WINDOW_DB_WND_INFO_VOLUME_CHANNEL		= 3028;		// �{�����[���`�����l�����
const int	IDM_WINDOW_DB_WND_INFO_KOE                  = 3029;		// ���E�H�b�`���
const int	IDM_WINDOW_DB_WND_INFO_TEST                 = 3030;		// �e�X�g
const int	IDM_WINDOW_DB_WND_INFO_TEST_TREEVIEW		= 3031;		// �e�X�g�c���[�r���[

// �E�B���h�E�FDirect3D ���
const int	IDM_WINDOW_DB_WND_INFO_D3D_DEVICE			= 3100;		// Direct3D �f�o�C�X���
const int	IDM_WINDOW_DB_WND_INFO_D3D_RESOURCE			= 3101;		// Direct3D ���\�[�X���

// �E�B���h�E�F�V�X�e�����
const int	IDM_WINDOW_DB_WND_INFO_SYSTEM_D3D			= 3200;		// Direct3D���
const int	IDM_WINDOW_DB_WND_INFO_SYSTEM_SYSTEM		= 3201;		// �V�X�e�����
const int	IDM_WINDOW_DB_WND_INFO_SYSTEM_DIR			= 3202;		// �f�B���N�g�����
const int	IDM_WINDOW_DB_WND_INFO_SYSTEM_STACK			= 3203;		// �X�^�b�N���
const int	IDM_WINDOW_DB_WND_INFO_SYSTEM_PROC_STACK	= 3204;		// �v���Z�X�X�^�b�N���

// �E�B���h�E�F�t���O
const int	IDM_WINDOW_DB_WND_INFO_FLAG_TOP				= 3300;		// �t���O

// �E�B���h�E�FFlag.ini
const int	IDM_WINDOW_DB_WND_INFO_FLAG_INI_TOP			= 3320;		// Flag.ini

// �E�B���h�E�F�I�u�W�F�N�g
const int	IDM_WINDOW_DB_WND_INFO_OBJECT_TOP			= 3350;		// �I�u�W�F�N�g

// �R�}���h
const int	IDM_COMMAND									= 4000;

// �V�X�e���R�}���h���j���[
const int	IDM_SYSTEM_COMMAND_MENU						= 5000;


// ****************************************************************
// �E�B���h�E���j���[
// ================================================================

// ���j���[�쐬
void C_tnm_wnd_menu::create()
{
	// �V�X�e���F�V�X�e���R�}���h���j���[
	sub_menu_system_syscommenu.create();

	// �V�X�e���F�ʂ̊��ݒ�
	sub_menu_system_config_solo.create();
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_SCREEN, _T("��ʂ̐ݒ�"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_VOLUME_BGMFADE, _T("���ʂ̐ݒ�"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_VOLUME, _T(" �� ���ʂ̐ݒ�"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_BGMFADE, _T(" �� �a�f�l�t�F�[�h�̐ݒ�"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_FONT_MSGSPD, _T("���͂̐ݒ�"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_FONT, _T(" �� �t�H���g�̐ݒ�"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_MSGSPD, _T(" �� �������x�̐ݒ�"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_MWNDBK, _T("�E�B���h�E�̔w�i�F�̐ݒ�"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_KOEMODE_CHRKOE, _T("�����̐ݒ�"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_KOEMODE, _T(" �� �������[�h�̐ݒ�"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_CHRKOE, _T(" �� �L�����N�^�[�����̐ݒ�"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_AUTOMODE, _T("�I�[�g���[�h�̐ݒ�"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_JITAN, _T("���Z�Đ��̐ݒ�"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_ELSE, _T("���̑��̐ݒ�"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_SYSTEM, _T("�V�X�e���̐ݒ�"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_MOVTYPE, _T("���[�r�[�̍Đ����@�̐ݒ�"), true);

	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_ALL_OPEN, _T("���S�ĊJ��"), true);
	sub_menu_system_config_solo.add_item(IDM_SYSTEM_CONFIG_SOLO_ALL_CLOSE, _T("���S�ĕ���"), true);

	// �V�X�e���F�ʂ̊��ݒ�i���[�_���Łj
	sub_menu_system_config_modal.create();
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_SCREEN, _T("��ʂ̐ݒ�i���ݒ��ύX����ƃo�O�遨�Ή������j"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_VOLUME_BGMFADE, _T("���ʂ̐ݒ�i�����ʂ����A���^�C���ŕς��Ȃ������Ƃ��������j"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_VOLUME, _T(" �� ���ʂ̐ݒ�i�����ʂ����A���^�C���ŕς��Ȃ������Ƃ��������j"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_BGMFADE, _T(" �� �a�f�l�t�F�[�h�̐ݒ�i�����ʂ����A���^�C���ŕς��Ȃ������Ƃ��������j"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_FONT_MSGSPD, _T("���͂̐ݒ�i���t�H���g�����A���^�C���ŕς��Ȃ����Ή������j"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_FONT, _T(" �� �t�H���g�̐ݒ�i���t�H���g�����A���^�C���ŕς��Ȃ����Ή������j"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_MSGSPD, _T(" �� �������x�̐ݒ�"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_MWNDBK, _T("�E�B���h�E�̔w�i�F�̐ݒ�i���w�i�F�����A���^�C���ŕς��Ȃ��������p�T���v���\���j"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_KOEMODE_CHRKOE, _T("�����̐ݒ�"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_KOEMODE, _T(" �� �������[�h�̐ݒ�"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_CHRKOE, _T(" �� �L�����N�^�[�����̐ݒ�"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_AUTOMODE, _T("�I�[�g���[�h�̐ݒ�"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_JITAN, _T("���Z�Đ��̐ݒ�"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_ELSE, _T("���̑��̐ݒ�"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_SYSTEM, _T("�V�X�e���̐ݒ�"), true);
	sub_menu_system_config_modal.add_item(IDM_SYSTEM_CONFIG_MODAL_MOVTYPE, _T("���[�r�[�̍Đ����@�̐ݒ�i���T�|�[�g�p�j"), true);

	// �V�X�e���F�f�o�b�O�p�F�S�V�X�e���R�}���h���j���[
	sub_menu_system_debug_syscommenu_full_mode.create();

	// �V�X�e���F�f�o�b�O�p
	sub_menu_system_debug.create();
	sub_menu_system_debug.add_sub_menu(IDM_SYSTEM_SYSCOMMENU, _T("�S�V�X�e���R�}���h���j���[�i���������j"), sub_menu_system_debug_syscommenu_full_mode.get_handle(), true);
	sub_menu_system_debug.add_item(IDM_SYSTEM_DEBUG_CONFIG_FULL_MODE, _T("�S���ݒ�"), true);
	sub_menu_system_debug.add_item(IDM_SYSTEM_DEBUG_SAVE_SOLO_DIALOG, _T("�����ŃZ�[�u�_�C�A���O"), true);
	sub_menu_system_debug.add_item(IDM_SYSTEM_DEBUG_LOAD_SOLO_DIALOG, _T("�����Ń��[�h�_�C�A���O"), true);

	// �V�X�e��
	sub_menu_system.create();
	sub_menu_system.add_sub_menu(IDM_SYSTEM_DEBUG_SYSCOMMENU_FULL_MODE, _T("�V�X�e���R�}���h���j���["), sub_menu_system_syscommenu.get_handle(), true);
	sub_menu_system.add_item(IDM_SYSTEM_CONFIG, _T("���ݒ�"), true);
	sub_menu_system.add_item(IDM_SYSTEM_SAVE_MODAL_DIALOG, _T("�Z�[�u�_�C�A���O"), true);
	sub_menu_system.add_item(IDM_SYSTEM_LOAD_MODAL_DIALOG, _T("���[�h�_�C�A���O"), true);
	sub_menu_system.add_sub_menu(IDM_SYSTEM_CONFIG_SOLO, _T("�ʂ̊��ݒ�"), sub_menu_system_config_solo.get_handle(), true);
	sub_menu_system.add_sub_menu(IDM_SYSTEM_CONFIG_MODAL, _T("�ʂ̊��ݒ�i���[�_���Łj"), sub_menu_system_config_modal.get_handle(), true);
	sub_menu_system.add_item(IDM_SYSTEM_TWEET_SOLO_DIALOG, _T("�c�C�[�g�_�C�A���O"), true);
	sub_menu_system.add_sub_menu(IDM_SYSTEM_DEBUG, _T("���f�o�b�O�p"), sub_menu_system_debug.get_handle(), true);

	// �f�o�b�O
	sub_menu_debug.create();
	sub_menu_debug.add_item(IDM_DEBUG_RESTART, _T("�ŏ�����n�߂� ( F1 )"), true);
	sub_menu_debug.add_item(IDM_DEBUG_TRACE, _T("�g���[�X ( F3 )"), true);
	sub_menu_debug.add_item(IDM_DEBUG_DRAW_OBJECT_HIT_RECT, _T("�I�u�W�F�N�g�������`�̕\��"), true);

	// �E�B���h�E�FDirect3D ���
	sub_menu_window_d3d.create();
	sub_menu_window_d3d.add_item(IDM_WINDOW_DB_WND_INFO_D3D_DEVICE, _T("�f�o�C�X���"), true);
	sub_menu_window_d3d.add_item(IDM_WINDOW_DB_WND_INFO_D3D_RESOURCE, _T("���\�[�X���"), true);

	// �E�B���h�E�F�V�X�e�����
	sub_menu_window_system.create();
	sub_menu_window_system.add_sub_menu(IDM_WINDOW_DB_WND_INFO_SYSTEM_D3D, _T("Direct3D ���"), sub_menu_window_d3d.get_handle(), true);
	sub_menu_window_system.add_item(IDM_WINDOW_DB_WND_INFO_SYSTEM_SYSTEM, _T("�V�X�e�����"), true);
	sub_menu_window_system.add_item(IDM_WINDOW_DB_WND_INFO_SYSTEM_DIR, _T("�f�B���N�g�����"), true);
	sub_menu_window_system.add_item(IDM_WINDOW_DB_WND_INFO_SYSTEM_STACK, _T("�X�^�b�N���"), true);
	sub_menu_window_system.add_item(IDM_WINDOW_DB_WND_INFO_SYSTEM_PROC_STACK, _T("�v���Z�X�X�^�b�N���"), true);

	// �E�B���h�E�F�t���O
	sub_menu_window_flag.create();
	TSTR flag_name[] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E"), _T("F"), _T("X"), _T("S"), _T("G"), _T("Z"), _T("M"), _T("L"), _T("K")};
	for (int i = 0; i < TNM_FLAG_CNT; i++)	{
		sub_menu_window_flag.add_item(IDM_WINDOW_DB_WND_INFO_FLAG_TOP + i, str_format(_T("�t���O%s"), flag_name[i].c_str()));
	}

	// �E�B���h�E�FFlag.ini
	sub_menu_window_flag_ini.create();
	for (int i = 0; i < 20; i++)	{
		if (Gp_flag_ini->page[i].title.empty())	{
			sub_menu_window_flag_ini.add_item(IDM_WINDOW_DB_WND_INFO_FLAG_INI_TOP + i, str_format(_T("�t���O%02d"), i));
		}
		else	{
			sub_menu_window_flag_ini.add_item(IDM_WINDOW_DB_WND_INFO_FLAG_INI_TOP + i, Gp_flag_ini->page[i].title);
		}
	}

	// �E�B���h�E�F�I�u�W�F�N�g
	sub_menu_window_object.create();
	TSTR object_name[] = {_T("back.object"), _T("front.object"), _T("next.object"), _T("excall.back.object"), _T("excall.front.object"), _T("excall.next.object")};
	for (int i = 0; i < 6; i++)	{
		sub_menu_window_object.add_item(IDM_WINDOW_DB_WND_INFO_OBJECT_TOP + i, object_name[i]);
	}

	// �E�B���h�E
	sub_menu_window.create();
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_DEBUG, _T("�f�o�b�O�ݒ�"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_SWITCH, _T("�f�o�b�O�X�C�b�`"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_SCN_CONTROL, _T("�V�[���R���g���[��"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_CALL_HISTORY, _T("�Ăяo������"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_SAVE_HISTORY, _T("�Z�[�u�|�C���g����"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_TRACE, _T("�g���[�X�E�B���h�E"), true);
	sub_menu_window.add_separator();
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_ERROR, _T("�G���[��� ( F8 )"), true);
	sub_menu_window.add_sub_menu(IDM_WINDOW_DB_WND_INFO_SYSTEM, _T("�V�X�e�����"), sub_menu_window_system.get_handle(), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_SCRIPT, _T("�Q�[�����"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_INPUT, _T("���͏��"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_BUTTON, _T("�{�^�����"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_NAMAE, _T("�y���O�z���"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_READ_FLAG, _T("���ǃt���O���"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_CG_TABLE, _T("�b�f�e�[�u�����"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_BGM_TABLE, _T("�a�f�l�e�[�u�����"), true);
	sub_menu_window.add_separator();
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_ELEMENT, _T("���G�������g���"), true);
	sub_menu_window.add_sub_menu(IDM_WINDOW_DB_WND_INFO_FLAG, _T("�t���O���"), sub_menu_window_flag.get_handle(), true);
	sub_menu_window.add_sub_menu(IDM_WINDOW_DB_WND_INFO_FLAG_INI, _T("Flag.ini ���"), sub_menu_window_flag_ini.get_handle(), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_USER_FLAG, _T("���[�U�t���O���"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_USER_CALL_FLAG, _T("���[�U�R�[���t���O���"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_NAMAE_FLAG, _T("���O�t���O���"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_COUNTER, _T("�J�E���^���"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_WORLD, _T("���[���h���"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_GROUP, _T("�O���[�v���"), true);
	sub_menu_window.add_sub_menu(IDM_WINDOW_DB_WND_INFO_OBJECT, _T("�I�u�W�F�N�g���"), sub_menu_window_object.get_handle(), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_MWND, _T("���b�Z�[�W�E�B���h�E���"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_SOUND, _T("�T�E���h���"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_VOLUME_CHANNEL, _T("�{�����[���`�����l�����"), true);
	sub_menu_window.add_item(IDM_WINDOW_DB_WND_INFO_KOE, _T("���E�H�b�`���"), true);

	// �R�}���h
	sub_menu_command.create();

	// �{��
	C_menu::create();
	add_sub_menu(IDM_SYSTEM, _T("����"), sub_menu_system.get_handle(), true);
	add_sub_menu(IDM_DEBUG, _T("���ޯ��"), sub_menu_debug.get_handle(), true);
	add_sub_menu(IDM_WINDOW, _T("����޳"), sub_menu_window.get_handle(), true);
	add_sub_menu(IDM_COMMAND, _T("�����"), sub_menu_command.get_handle(), true);
}

// ���j���[�j��
void C_tnm_wnd_menu::destroy()
{
	// �V�X�e��
	sub_menu_system_debug_syscommenu_full_mode.destroy();
	sub_menu_system_debug.destroy();
	sub_menu_system_config_modal.destroy();
	sub_menu_system_config_solo.destroy();
	sub_menu_system_syscommenu.destroy();
	sub_menu_system.destroy();

	// �f�o�b�O
	sub_menu_debug.destroy();

	// �E�B���h�E
	sub_menu_window_system.destroy();
	sub_menu_window_d3d.destroy();
	sub_menu_window.destroy();

	// �R�}���h
	sub_menu_command.destroy();

	// �{��
	C_menu::destroy();
}

// ****************************************************************
// �R�}���h
// ================================================================
void C_tnm_wnd_menu::on_command(DWORD menu_id)
{
	switch (menu_id)	{

		// �V�X�e���F���ݒ�
		case IDM_SYSTEM_CONFIG:
			sub_menu_system.get_check(IDM_SYSTEM_CONFIG) ? Gp_dlg_mng->cfg_wnd_config_base.close() : Gp_dlg_mng->cfg_wnd_config_base.open();
			break;

		// �V�X�e���F�Z�[�u�_�C�A���O
		case IDM_SYSTEM_SAVE_MODAL_DIALOG:
			tnm_syscom_open_save(true, true, true);	// �i���Z�[�u�F�E�B���h�E���j���[�j�_�C�A���O���Ă΂��ꍇ�̓L���v�`���[����
			break;

		// �V�X�e���F���[�h�_�C�A���O
		case IDM_SYSTEM_LOAD_MODAL_DIALOG:
			tnm_syscom_open_load(true, true);
			break;

		// �V�X�e���F�ʂ̊��ݒ�
		case IDM_SYSTEM_CONFIG_SOLO_SCREEN:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_SCREEN) ? Gp_dlg_mng->cfg_wnd_solo_screen.close() : Gp_dlg_mng->cfg_wnd_solo_screen.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_FONT_MSGSPD:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_FONT_MSGSPD) ? Gp_dlg_mng->cfg_wnd_solo_font_msgspd.close() : Gp_dlg_mng->cfg_wnd_solo_font_msgspd.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_FONT:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_FONT) ? Gp_dlg_mng->cfg_wnd_solo_font.close() : Gp_dlg_mng->cfg_wnd_solo_font.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_MSGSPD:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_MSGSPD) ? Gp_dlg_mng->cfg_wnd_solo_msgspd.close() : Gp_dlg_mng->cfg_wnd_solo_msgspd.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_VOLUME_BGMFADE:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_VOLUME_BGMFADE) ? Gp_dlg_mng->cfg_wnd_solo_volume_bgmfade.close() : Gp_dlg_mng->cfg_wnd_solo_volume_bgmfade.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_VOLUME:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_VOLUME) ? Gp_dlg_mng->cfg_wnd_solo_volume.close() : Gp_dlg_mng->cfg_wnd_solo_volume.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_BGMFADE:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_BGMFADE) ? Gp_dlg_mng->cfg_wnd_solo_bgmfade.close() : Gp_dlg_mng->cfg_wnd_solo_bgmfade.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_KOEMODE_CHRKOE:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_KOEMODE_CHRKOE) ? Gp_dlg_mng->cfg_wnd_solo_koemode_chrkoe.close() : Gp_dlg_mng->cfg_wnd_solo_koemode_chrkoe.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_KOEMODE:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_KOEMODE) ? Gp_dlg_mng->cfg_wnd_solo_koemode.close() : Gp_dlg_mng->cfg_wnd_solo_koemode.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_CHRKOE:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_CHRKOE) ? Gp_dlg_mng->cfg_wnd_solo_chrkoe.close() : Gp_dlg_mng->cfg_wnd_solo_chrkoe.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_MWNDBK:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_MWNDBK) ? Gp_dlg_mng->cfg_wnd_solo_mwndbk.close() : Gp_dlg_mng->cfg_wnd_solo_mwndbk.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_AUTOMODE:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_AUTOMODE) ? Gp_dlg_mng->cfg_wnd_solo_automode.close() : Gp_dlg_mng->cfg_wnd_solo_automode.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_JITAN:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_JITAN) ? Gp_dlg_mng->cfg_wnd_solo_jitan.close() : Gp_dlg_mng->cfg_wnd_solo_jitan.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_ELSE:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_ELSE) ? Gp_dlg_mng->cfg_wnd_solo_else.close() : Gp_dlg_mng->cfg_wnd_solo_else.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_SYSTEM:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_SYSTEM) ? Gp_dlg_mng->cfg_wnd_solo_system.close() : Gp_dlg_mng->cfg_wnd_solo_system.open();
			break;
		case IDM_SYSTEM_CONFIG_SOLO_MOVTYPE:
			sub_menu_system_config_solo.get_check(IDM_SYSTEM_CONFIG_SOLO_MOVTYPE) ? Gp_dlg_mng->cfg_wnd_solo_movtype.close() : Gp_dlg_mng->cfg_wnd_solo_movtype.open();
			break;

		// �V�X�e���F�ʂ̊��ݒ�F�S�ĊJ��
		case IDM_SYSTEM_CONFIG_SOLO_ALL_OPEN:
			Gp_dlg_mng->cfg_wnd_solo_screen.open();				// �\���F��ʃ��[�h
			Gp_dlg_mng->cfg_wnd_solo_font_msgspd.open();		// �\���F���b�Z�[�W�i�t�H���g�A�������x�j
			Gp_dlg_mng->cfg_wnd_solo_font.open();				// �\���F�t�H���g
			Gp_dlg_mng->cfg_wnd_solo_msgspd.open();				// �\���F�������x
			Gp_dlg_mng->cfg_wnd_solo_volume_bgmfade.open();		// �\���F���ʁi���ʁA�a�f�l�t�F�[�h�j
			Gp_dlg_mng->cfg_wnd_solo_volume.open();				// �\���F����
			Gp_dlg_mng->cfg_wnd_solo_bgmfade.open();			// �\���F�a�f�l�t�F�[�h
			Gp_dlg_mng->cfg_wnd_solo_koemode_chrkoe.open();		// �\���F�����i�������[�h�A�L�����N�^�[�����j
			Gp_dlg_mng->cfg_wnd_solo_koemode.open();			// �\���F�������[�h
			Gp_dlg_mng->cfg_wnd_solo_chrkoe.open();				// �\���F�L�����N�^�[����
			Gp_dlg_mng->cfg_wnd_solo_mwndbk.open();				// �\���F�E�B���h�E�w�i�F
			Gp_dlg_mng->cfg_wnd_solo_automode.open();			// �\���F�I�[�g���[�h
			Gp_dlg_mng->cfg_wnd_solo_jitan.open();				// �\���F���Z�Đ�
			Gp_dlg_mng->cfg_wnd_solo_else.open();				// �\���F���̑�
			Gp_dlg_mng->cfg_wnd_solo_system.open();				// �\���F�V�X�e��
			Gp_dlg_mng->cfg_wnd_solo_movtype.open();			// �\���F���[�r�[�̍Đ����@
			break;

		// �V�X�e���F�ʂ̊��ݒ�F�S�ĕ���
		case IDM_SYSTEM_CONFIG_SOLO_ALL_CLOSE:
			Gp_dlg_mng->cfg_wnd_solo_screen.close();			// �\���F��ʃ��[�h
			Gp_dlg_mng->cfg_wnd_solo_font_msgspd.close();		// �\���F���b�Z�[�W�i�t�H���g�A�������x�j
			Gp_dlg_mng->cfg_wnd_solo_font.close();				// �\���F�t�H���g
			Gp_dlg_mng->cfg_wnd_solo_msgspd.close();			// �\���F�������x
			Gp_dlg_mng->cfg_wnd_solo_volume_bgmfade.close();	// �\���F���ʁi���ʁA�a�f�l�t�F�[�h�j
			Gp_dlg_mng->cfg_wnd_solo_volume.close();			// �\���F����
			Gp_dlg_mng->cfg_wnd_solo_bgmfade.close();			// �\���F�a�f�l�t�F�[�h
			Gp_dlg_mng->cfg_wnd_solo_koemode_chrkoe.close();	// �\���F�����i�������[�h�A�L�����N�^�[�����j
			Gp_dlg_mng->cfg_wnd_solo_koemode.close();			// �\���F�������[�h
			Gp_dlg_mng->cfg_wnd_solo_chrkoe.close();			// �\���F�L�����N�^�[����
			Gp_dlg_mng->cfg_wnd_solo_mwndbk.close();			// �\���F�E�B���h�E�w�i�F
			Gp_dlg_mng->cfg_wnd_solo_automode.close();			// �\���F�I�[�g���[�h
			Gp_dlg_mng->cfg_wnd_solo_jitan.close();				// �\���F���Z�Đ�
			Gp_dlg_mng->cfg_wnd_solo_else.close();				// �\���F���̑�
			Gp_dlg_mng->cfg_wnd_solo_system.close();			// �\���F�V�X�e��
			Gp_dlg_mng->cfg_wnd_solo_movtype.close();			// �\���F���[�r�[�̍Đ����@
			break;

		// �V�X�e���F�ʂ̊��ݒ�i���[�_���Łj
		case IDM_SYSTEM_CONFIG_MODAL_SCREEN:
			{
				C_cfg_wnd_modal_screen cfg_wnd_modal_screen;
				cfg_wnd_modal_screen.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_FONT_MSGSPD:
			{
				C_cfg_wnd_modal_font_msgspd cfg_wnd_modal_font_msgspd;
				cfg_wnd_modal_font_msgspd.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_FONT:
			{
				C_cfg_wnd_modal_font cfg_wnd_modal_font;
				cfg_wnd_modal_font.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_MSGSPD:
			{
				C_cfg_wnd_modal_msgspd cfg_wnd_modal_msgspd;
				cfg_wnd_modal_msgspd.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_VOLUME_BGMFADE:
			{
				Ccfg_wnd_modal_volume_bgmfade cfg_wnd_modal_volume_bgmfade;
				cfg_wnd_modal_volume_bgmfade.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_VOLUME:
			{
				Ccfg_wnd_modal_volume cfg_wnd_modal_volume;
				cfg_wnd_modal_volume.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_BGMFADE:
			{
				Ccfg_wnd_modal_bgmfade cfg_wnd_modal_bgmfade;
				cfg_wnd_modal_bgmfade.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_KOEMODE_CHRKOE:
			{
				Ccfg_wnd_modal_koemode_chrkoe cfg_wnd_modal_koemode_chrkoe;
				cfg_wnd_modal_koemode_chrkoe.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_KOEMODE:
			{
				Ccfg_wnd_modal_koemode cfg_wnd_modal_koemode;
				cfg_wnd_modal_koemode.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_CHRKOE:
			{
				Ccfg_wnd_modal_chrkoe cfg_wnd_modal_chrkoe;
				cfg_wnd_modal_chrkoe.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_MWNDBK:
			{
				Ccfg_wnd_modal_mwndbk cfg_wnd_modal_mwndbk;
				cfg_wnd_modal_mwndbk.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_AUTOMODE:
			{
				Ccfg_wnd_modal_automode cfg_wnd_modal_automode;
				cfg_wnd_modal_automode.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_JITAN:
			{
				Ccfg_wnd_modal_jitan cfg_wnd_modal_jitan;
				cfg_wnd_modal_jitan.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_ELSE:
			{
				Ccfg_wnd_modal_else cfg_wnd_modal_else;
				cfg_wnd_modal_else.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_SYSTEM:
			{
				Ccfg_wnd_modal_system cfg_wnd_modal_system;
				cfg_wnd_modal_system.open_modal();
			}
			break;
		case IDM_SYSTEM_CONFIG_MODAL_MOVTYPE:
			{
				Ccfg_wnd_modal_movtype cfg_wnd_modal_movtype;				// �T�|�[�g�p�̃��[�_���`���́u���[�r�[�̍Đ����@�v�̐ݒ���Ăяo��
				bool res = cfg_wnd_modal_movtype.open_modal_result_bool();	// res : false = �Đ����Ȃ�
			}
			break;

		// �V�X�e���F�����Ńc�C�[�g�_�C�A���O
		case IDM_SYSTEM_TWEET_SOLO_DIALOG:
			sub_menu_system.get_check(IDM_SYSTEM_TWEET_SOLO_DIALOG) ? Gp_dlg_mng->sys_wnd_solo_tweet.close() : Gp_dlg_mng->sys_wnd_solo_tweet.open();
			break;

		// �V�X�e���F�f�o�b�O�p�F�S���ݒ�
		case IDM_SYSTEM_DEBUG_CONFIG_FULL_MODE:
			sub_menu_system.get_check(IDM_SYSTEM_DEBUG_CONFIG_FULL_MODE) ? Gp_dlg_mng->cfg_wnd_config_base_full_mode.close() : Gp_dlg_mng->cfg_wnd_config_base_full_mode.open();
			break;

		// �V�X�e���F�f�o�b�O�p�F�����ŃZ�[�u�_�C�A���O
		case IDM_SYSTEM_DEBUG_SAVE_SOLO_DIALOG:
			sub_menu_system.get_check(IDM_SYSTEM_DEBUG_SAVE_SOLO_DIALOG) ? Gp_dlg_mng->sys_wnd_solo_save.close() : Gp_dlg_mng->sys_wnd_solo_save.open();
			break;

		// �V�X�e���F�f�o�b�O�p�F�����Ń��[�h�_�C�A���O
		case IDM_SYSTEM_DEBUG_LOAD_SOLO_DIALOG:
			sub_menu_system.get_check(IDM_SYSTEM_DEBUG_LOAD_SOLO_DIALOG) ? Gp_dlg_mng->sys_wnd_solo_load.close() : Gp_dlg_mng->sys_wnd_solo_load.open();
			break;

		// �f�o�b�O
		case IDM_DEBUG_RESTART:					// �ŏ�����n�߂�
			tnm_syscom_restart_from_start();
			break;
		case IDM_DEBUG_TRACE:					// �g���[�X
			tnm_syscom_change_break();
			break;

		// �E�B���h�E
		case IDM_WINDOW_DB_WND_INFO_INPUT:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_INPUT) ? Gp_dlg_mng->db_wnd_info_input.close() : Gp_dlg_mng->db_wnd_info_input.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_DEBUG:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_DEBUG) ? Gp_dlg_mng->db_wnd_info_debug.close() : Gp_dlg_mng->db_wnd_info_debug.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_SCN_CONTROL:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_SCN_CONTROL) ? Gp_dlg_mng->db_wnd_info_scn_control.close() : Gp_dlg_mng->db_wnd_info_scn_control.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_CALL_HISTORY:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_CALL_HISTORY) ? Gp_dlg_mng->db_wnd_info_call_history.close() : Gp_dlg_mng->db_wnd_info_call_history.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_SAVE_HISTORY:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_SAVE_HISTORY) ? Gp_dlg_mng->db_wnd_info_save_history.close() : Gp_dlg_mng->db_wnd_info_save_history.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_TRACE:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_TRACE) ? Gp_dlg_mng->db_wnd_info_trace.close() : Gp_dlg_mng->db_wnd_info_trace.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_ERROR:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_ERROR) ? Gp_dlg_mng->db_wnd_info_error.close() : Gp_dlg_mng->db_wnd_info_error.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_SCRIPT:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_SCRIPT) ? Gp_dlg_mng->db_wnd_info_script.close() : Gp_dlg_mng->db_wnd_info_script.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_NAMAE:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_NAMAE) ? Gp_dlg_mng->db_wnd_info_namae.close() : Gp_dlg_mng->db_wnd_info_namae.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_CG_TABLE:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_CG_TABLE) ? Gp_dlg_mng->db_wnd_info_cg_table.close() : Gp_dlg_mng->db_wnd_info_cg_table.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_BGM_TABLE:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_BGM_TABLE) ? Gp_dlg_mng->db_wnd_info_bgm_table.close() : Gp_dlg_mng->db_wnd_info_bgm_table.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_READ_FLAG:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_READ_FLAG) ? Gp_dlg_mng->db_wnd_info_read_flag.close() : Gp_dlg_mng->db_wnd_info_read_flag.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_ELEMENT:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_ELEMENT) ? Gp_dlg_mng->db_wnd_info_element.close() : Gp_dlg_mng->db_wnd_info_element.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_USER_FLAG:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_USER_FLAG) ? Gp_dlg_mng->db_wnd_info_user_flag.close() : Gp_dlg_mng->db_wnd_info_user_flag.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_USER_CALL_FLAG:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_USER_CALL_FLAG) ? Gp_dlg_mng->db_wnd_info_user_call_flag.close() : Gp_dlg_mng->db_wnd_info_user_call_flag.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_NAMAE_FLAG:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_NAMAE_FLAG) ? Gp_dlg_mng->db_wnd_info_namae_flag.close() : Gp_dlg_mng->db_wnd_info_namae_flag.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_COUNTER:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_COUNTER) ? Gp_dlg_mng->db_wnd_info_counter.close() : Gp_dlg_mng->db_wnd_info_counter.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_BUTTON:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_BUTTON) ? Gp_dlg_mng->db_wnd_info_btn.close() : Gp_dlg_mng->db_wnd_info_btn.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_GROUP:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_GROUP) ? Gp_dlg_mng->db_wnd_info_group.close() : Gp_dlg_mng->db_wnd_info_group.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_WORLD:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_WORLD) ? Gp_dlg_mng->db_wnd_info_world.close() : Gp_dlg_mng->db_wnd_info_world.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_MWND:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_MWND) ? Gp_dlg_mng->db_wnd_info_mwnd.close() : Gp_dlg_mng->db_wnd_info_mwnd.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_SOUND:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_SOUND) ? Gp_dlg_mng->db_wnd_info_sound.close() : Gp_dlg_mng->db_wnd_info_sound.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_VOLUME_CHANNEL:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_VOLUME_CHANNEL) ? Gp_dlg_mng->db_wnd_info_system_sound.close() : Gp_dlg_mng->db_wnd_info_system_sound.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_KOE:
			sub_menu_window.get_check(IDM_WINDOW_DB_WND_INFO_KOE) ? Gp_dlg_mng->db_wnd_info_koe.close() : Gp_dlg_mng->db_wnd_info_koe.open();
			break;

		// �E�B���h�E�FDirect3D ���
		case IDM_WINDOW_DB_WND_INFO_D3D_DEVICE:
			sub_menu_window_d3d.get_check(IDM_WINDOW_DB_WND_INFO_D3D_DEVICE) ? Gp_dlg_mng->db_wnd_info_d3d_device.close() : Gp_dlg_mng->db_wnd_info_d3d_device.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_D3D_RESOURCE:
			sub_menu_window_d3d.get_check(IDM_WINDOW_DB_WND_INFO_D3D_RESOURCE) ? Gp_dlg_mng->db_wnd_info_d3d_resource.close() : Gp_dlg_mng->db_wnd_info_d3d_resource.open();
			break;

		// �E�B���h�E�F�V�X�e�����
		case IDM_WINDOW_DB_WND_INFO_SYSTEM_SYSTEM:
			sub_menu_window_system.get_check(IDM_WINDOW_DB_WND_INFO_SYSTEM_SYSTEM) ? Gp_dlg_mng->db_wnd_info_system.close() : Gp_dlg_mng->db_wnd_info_system.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_SYSTEM_DIR:
			sub_menu_window_system.get_check(IDM_WINDOW_DB_WND_INFO_SYSTEM_DIR) ? Gp_dlg_mng->db_wnd_info_dir.close() : Gp_dlg_mng->db_wnd_info_dir.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_SYSTEM_STACK:
			sub_menu_window_system.get_check(IDM_WINDOW_DB_WND_INFO_SYSTEM_STACK) ? Gp_dlg_mng->db_wnd_info_stack.close() : Gp_dlg_mng->db_wnd_info_stack.open();
			break;
		case IDM_WINDOW_DB_WND_INFO_SYSTEM_PROC_STACK:
			sub_menu_window_system.get_check(IDM_WINDOW_DB_WND_INFO_SYSTEM_PROC_STACK) ? Gp_dlg_mng->db_wnd_info_proc_stack.close() : Gp_dlg_mng->db_wnd_info_proc_stack.open();
			break;

		// �X�e�[�^�X

		default:
			// �t���O
			if (IDM_WINDOW_DB_WND_INFO_FLAG_TOP <= menu_id && menu_id < IDM_WINDOW_DB_WND_INFO_FLAG_TOP + TNM_FLAG_CNT)	{
				int dialog_id = menu_id - IDM_WINDOW_DB_WND_INFO_FLAG_TOP;
				sub_menu_window_flag.get_check(menu_id) ? Gp_dlg_mng->db_wnd_info_flag[dialog_id].close() : Gp_dlg_mng->db_wnd_info_flag[dialog_id].open();
			}
			// Flag.ini
			if (IDM_WINDOW_DB_WND_INFO_FLAG_INI_TOP <= menu_id && menu_id < IDM_WINDOW_DB_WND_INFO_FLAG_INI_TOP + 20)	{
				int dialog_id = menu_id - IDM_WINDOW_DB_WND_INFO_FLAG_INI_TOP;
				sub_menu_window_flag_ini.get_check(menu_id) ? Gp_dlg_mng->db_wnd_info_flag_ini[dialog_id].close() : Gp_dlg_mng->db_wnd_info_flag_ini[dialog_id].open();
			}
			// �I�u�W�F�N�g
			if (IDM_WINDOW_DB_WND_INFO_OBJECT_TOP <= menu_id && menu_id < IDM_WINDOW_DB_WND_INFO_OBJECT_TOP + 6)	{
				int dialog_id = menu_id - IDM_WINDOW_DB_WND_INFO_OBJECT_TOP;
				sub_menu_window_object.get_check(menu_id) ? Gp_dlg_mng->db_wnd_info_object[dialog_id].close() : Gp_dlg_mng->db_wnd_info_object[dialog_id].open();
			}
			// �V�X�e���R�}���h���j���[
			if (IDM_WINDOW_DB_WND_INFO_SYSTEM_SYSTEM <= menu_id && menu_id < IDM_SYSTEM_COMMAND_MENU + 1000)	{
				tnm_rmenu_execute(menu_id - IDM_SYSTEM_COMMAND_MENU);
			}
			break;
	}
}

// ****************************************************************
// ���j���[�X�V
// ================================================================
void C_tnm_wnd_menu::update()
{
	// �V�X�e���F�V�X�e���R�}���h���j���[
	sub_menu_system_syscommenu.remove_all_item();
	tnm_rmenu_create_funk(sub_menu_system_syscommenu, IDM_SYSTEM_COMMAND_MENU);

	// �V�X�e���F���ݒ�
	sub_menu_system.set_check(IDM_SYSTEM_CONFIG, Gp_dlg_mng->cfg_wnd_config_base.get_handle() ? true : false);											// ���ݒ�

	// �V�X�e���F�Z�[�u�_�C�A���O

	// �V�X�e���F���[�h�_�C�A���O

	// �V�X�e���F�ʂ̊��ݒ�
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_SCREEN, Gp_dlg_mng->cfg_wnd_solo_screen.get_handle() ? true : false);					// �\���F��ʃ��[�h
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_FONT_MSGSPD, Gp_dlg_mng->cfg_wnd_solo_font_msgspd.get_handle() ? true : false);		// �\���F���b�Z�[�W�i�t�H���g�A�������x�j
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_FONT, Gp_dlg_mng->cfg_wnd_solo_font.get_handle() ? true : false);						// �\���F�t�H���g
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_MSGSPD, Gp_dlg_mng->cfg_wnd_solo_msgspd.get_handle() ? true : false);					// �\���F�������x
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_VOLUME_BGMFADE, Gp_dlg_mng->cfg_wnd_solo_volume_bgmfade.get_handle() ? true : false);	// �\���F���ʁi���ʁA�a�f�l�t�F�[�h�j
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_VOLUME, Gp_dlg_mng->cfg_wnd_solo_volume.get_handle() ? true : false);					// �\���F����
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_BGMFADE, Gp_dlg_mng->cfg_wnd_solo_bgmfade.get_handle() ? true : false);				// �\���F�a�f�l�t�F�[�h
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_KOEMODE_CHRKOE, Gp_dlg_mng->cfg_wnd_solo_koemode_chrkoe.get_handle() ? true : false);	// �\���F�����i�������[�h�A�L�����N�^�[�����j
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_KOEMODE, Gp_dlg_mng->cfg_wnd_solo_koemode.get_handle() ? true : false);				// �\���F�������[�h
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_CHRKOE, Gp_dlg_mng->cfg_wnd_solo_chrkoe.get_handle() ? true : false);					// �\���F�L�����N�^�[����
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_MWNDBK, Gp_dlg_mng->cfg_wnd_solo_mwndbk.get_handle() ? true : false);					// �\���F�E�B���h�E�w�i�F
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_AUTOMODE, Gp_dlg_mng->cfg_wnd_solo_automode.get_handle() ? true : false);				// �\���F�I�[�g���[�h
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_JITAN, Gp_dlg_mng->cfg_wnd_solo_jitan.get_handle() ? true : false);					// �\���F���Z�Đ�
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_ELSE, Gp_dlg_mng->cfg_wnd_solo_else.get_handle() ? true : false);						// �\���F���̑�
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_SYSTEM, Gp_dlg_mng->cfg_wnd_solo_system.get_handle() ? true : false);					// �\���F�V�X�e��
	sub_menu_system_config_solo.set_check(IDM_SYSTEM_CONFIG_SOLO_MOVTYPE, Gp_dlg_mng->cfg_wnd_solo_movtype.get_handle() ? true : false);				// �\���F���[�r�[�̍Đ����@

	// �V�X�e���F�ʂ̊��ݒ�i���[�_���Łj

	// �V�X�e���F�c�C�[�g�_�C�A���O
	sub_menu_system.set_check(IDM_SYSTEM_TWEET_SOLO_DIALOG, Gp_dlg_mng->sys_wnd_solo_tweet.get_handle() ? true : false);								// �c�C�[�g�_�C�A���O

	// �V�X�e���F�f�o�b�O�p�F�S�V�X�e���R�}���h���j���[
	sub_menu_system_debug_syscommenu_full_mode.remove_all_item();
	tnm_rmenu_create_funk(sub_menu_system_debug_syscommenu_full_mode, IDM_SYSTEM_COMMAND_MENU);

	// �V�X�e���F�f�o�b�O�p�F�S���ݒ�
	sub_menu_system_debug.set_check(IDM_SYSTEM_DEBUG_CONFIG_FULL_MODE, Gp_dlg_mng->cfg_wnd_config_base_full_mode.get_handle() ? true : false);			// �S���ݒ�

	// �V�X�e���F�f�o�b�O�p�F�����ŃZ�[�u�_�C�A���O
	sub_menu_system_debug.set_check(IDM_SYSTEM_DEBUG_SAVE_SOLO_DIALOG, Gp_dlg_mng->sys_wnd_solo_save.get_handle() ? true : false);						// �����ŃZ�[�u�_�C�A���O

	// �V�X�e���F�f�o�b�O�p�F�����Ń��[�h�_�C�A���O
	sub_menu_system_debug.set_check(IDM_SYSTEM_DEBUG_LOAD_SOLO_DIALOG, Gp_dlg_mng->sys_wnd_solo_load.get_handle() ? true : false);						// �����Ń��[�h�_�C�A���O

	// �E�B���h�E
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_INPUT, Gp_dlg_mng->db_wnd_info_input.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_DEBUG, Gp_dlg_mng->db_wnd_info_debug.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_SCN_CONTROL, Gp_dlg_mng->db_wnd_info_scn_control.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_SAVE_HISTORY, Gp_dlg_mng->db_wnd_info_save_history.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_TRACE, Gp_dlg_mng->db_wnd_info_trace.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_ERROR, Gp_dlg_mng->db_wnd_info_error.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_NAMAE, Gp_dlg_mng->db_wnd_info_namae.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_CG_TABLE, Gp_dlg_mng->db_wnd_info_cg_table.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_BGM_TABLE, Gp_dlg_mng->db_wnd_info_bgm_table.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_READ_FLAG, Gp_dlg_mng->db_wnd_info_read_flag.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_ELEMENT, Gp_dlg_mng->db_wnd_info_element.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_USER_FLAG, Gp_dlg_mng->db_wnd_info_user_flag.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_USER_CALL_FLAG, Gp_dlg_mng->db_wnd_info_user_call_flag.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_NAMAE_FLAG, Gp_dlg_mng->db_wnd_info_namae_flag.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_COUNTER, Gp_dlg_mng->db_wnd_info_counter.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_BUTTON, Gp_dlg_mng->db_wnd_info_btn.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_GROUP, Gp_dlg_mng->db_wnd_info_group.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_WORLD, Gp_dlg_mng->db_wnd_info_world.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_MWND, Gp_dlg_mng->db_wnd_info_mwnd.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_SOUND, Gp_dlg_mng->db_wnd_info_sound.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_VOLUME_CHANNEL, Gp_dlg_mng->db_wnd_info_system_sound.get_handle() ? true : false);
	sub_menu_window.set_check(IDM_WINDOW_DB_WND_INFO_KOE, Gp_dlg_mng->db_wnd_info_koe.get_handle() ? true : false);

	// �E�B���h�E�FDirect3D ���
	sub_menu_window_d3d.set_check(IDM_WINDOW_DB_WND_INFO_D3D_DEVICE, Gp_dlg_mng->db_wnd_info_d3d_device.get_handle() ? true : false);
	sub_menu_window_d3d.set_check(IDM_WINDOW_DB_WND_INFO_D3D_RESOURCE, Gp_dlg_mng->db_wnd_info_d3d_resource.get_handle() ? true : false);

	// �E�B���h�E�F�V�X�e�����
	sub_menu_window_system.set_check(IDM_WINDOW_DB_WND_INFO_SYSTEM_SYSTEM, Gp_dlg_mng->db_wnd_info_system.get_handle() ? true : false);
	sub_menu_window_system.set_check(IDM_WINDOW_DB_WND_INFO_SYSTEM_DIR, Gp_dlg_mng->db_wnd_info_dir.get_handle() ? true : false);
	sub_menu_window_system.set_check(IDM_WINDOW_DB_WND_INFO_SYSTEM_STACK, Gp_dlg_mng->db_wnd_info_stack.get_handle() ? true : false);
	sub_menu_window_system.set_check(IDM_WINDOW_DB_WND_INFO_SYSTEM_PROC_STACK, Gp_dlg_mng->db_wnd_info_proc_stack.get_handle() ? true : false);

	// �E�B���h�E�F�t���O
	for (int i = 0; i < TNM_FLAG_CNT; i++)	{
		sub_menu_window_flag.set_check(IDM_WINDOW_DB_WND_INFO_FLAG_TOP + i, Gp_dlg_mng->db_wnd_info_flag[i].get_handle() ? true : false);
	}
	// �E�B���h�E�FFlag.ini
	for (int i = 0; i < 20; i++)	{
		sub_menu_window_flag_ini.set_check(IDM_WINDOW_DB_WND_INFO_FLAG_INI_TOP + i, Gp_dlg_mng->db_wnd_info_flag_ini[i].get_handle() ? true : false);
	}
	// �E�B���h�E�F�I�u�W�F�N�g
	for (int i = 0; i < 6; i++)	{
		sub_menu_window_object.set_check(IDM_WINDOW_DB_WND_INFO_OBJECT_TOP + i, Gp_dlg_mng->db_wnd_info_object[i].get_handle() ? true : false);
	}
}

