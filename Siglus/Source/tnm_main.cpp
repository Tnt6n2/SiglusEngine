#include	"pch.h"
#include	"tona3_main.h"
#include	"tnm_app.h"
#include	"..\resource.h"

#include	<atlbase.h>
#include	<atlhost.h>

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

static		CComModule	G_com_module;

// ****************************************************************
// ���C��
// ================================================================
int tona_main()
{
	// �A�v���P�[�V�����̖��O�̐ݒ�iini �̕ۑ��ȂǂɎg�p���܂��j
	G_app.app_name = _T("siglus_engine");

	// �^�C�}�[�̐��x���グ��
	TIMECAPS caps = {0};
	timeGetDevCaps(&caps, sizeof(caps));
	timeBeginPeriod(caps.wPeriodMin);

	// �����n��̏�����
	srand((UINT)time(NULL));

	// COM �̏�����
	G_com_module.Init(ObjectMap, G_app.h_inst, &LIBID_ATLLib);

	// ATL ������
	AtlAxWinInit();

	// Siglus �A�v���P�[�V����
	{
		// �{�̂͑傫�����ăX�^�b�N�ɏ��܂���B���I�ɍ��܂��B
		BSP<C_tnm_app> tnm_app(new C_tnm_app);
		
		// �A�v���P�[�V�����̃��C��
		tnm_app->main();
	}

	// COM ���W���[�������
	G_com_module.Term();

	// �^�C�}�[�̐��x�����ɖ߂�
	timeEndPeriod(caps.wPeriodMin);

	// ����I��
	return EXIT_SUCCESS;
}

