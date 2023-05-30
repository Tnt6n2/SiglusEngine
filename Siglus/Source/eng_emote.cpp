#include	"pch.h"
#include	"ifc_eng.h"

#if __USE_EMOTE

#pragma comment(lib, "emotedriver.lib")

#include	"engine/eng_emote.h"
#include	"engine/eng_graphics.h"
#include	"data/tnm_timer.h"

#include	"iemote.h"

//BSP<C_d3d_texture> G_emote_texture;
//BSP<C_d3d_surface> G_emote_dpst_buffer;

//------------------------------------------------
// E-mote�f�[�^�t�@�C��
//static const wchar_t *MOTION_DATA_PATH = L"X:\\Work\\���݂�38.psb";

//------------------------------------------------
// ��ʃT�C�Y
//static const int SCREEN_WIDTH = 1280;
//static const int SCREEN_HEIGHT = 720;

//------------------------------------------------
// D3D
#define FVF_LVERTEX     (D3DFVF_XYZ    | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//------------------------------------------------
// �ϊ�
static const float MSTOF60THS = 60.0f / 1000.0f;  // ms��1/60�b�J�E���g�֕ϊ��B
static const float F60THSTOMS = 1000.0f / 60.0f;  // 1/60�b�J�E���g��ms�֕ϊ��B

//------------------------------------------------
// Emote�֘A
IEmoteDevice *sEmoteDevice = NULL;

//------------------------------------------------
// �֐��錾
void EmoteNewPlayer(int numFiles, const wchar_t **filenames, IEmotePlayer** player);

//------------------------------------------------
// ������
void EmoteInit(void)
{
	// E-mote�f�o�C�X�쐬
	IEmoteDevice::InitParam param;
	memset(&param, 0, sizeof(param));
	param.d3dDevice = G_d3d.device.body();
	// �������m�ہE����֐��̎w���NULL�̂܂܂Ȃ�f�t�H���g��malloc/free���g����
	// param.objAllocator.alloc = &ObjAlloc;
	// param.objAllocator.free =  &ObjFree;
	sEmoteDevice = EmoteCreate(param);
	//  sEmoteDevice->SetMaskRegionClipping(true);
	sEmoteDevice->SetShaderModel(IEmoteDevice::SHADER_MODEL_2);
}

//------------------------------------------------
// �v���C��������
void EmoteNewPlayer(int numFiles, const wchar_t **filenames, IEmotePlayer** player)
{
//	int numFiles = 2;
//	const wchar_t* _filenames[] = {
//			_T("X:\\��Siglus �J����\\�T���v����\\exe\\dat\\bup_sa01_02����_��.psb"),
//			_T("X:\\��Siglus �J����\\�T���v����\\exe\\dat\\bup_sa01_02����_����.psb"),
//			_T("X:\\��Siglus �J����\\�T���v����\\exe\\dat\\bup_sa01_02����_tl.psb"),
//	};

//	int numFiles = 1;
//	const wchar_t* _filenames[] = {
//		_T("X:\\��Siglus �J����\\�T���v����\\exe\\dat\\bup_sa01_02����_����.psb")
//	};

	emote_uint32_t emoteObjectSize[256];
	emote_uint8_t *emoteObjectImage[256];

	// E-mtoe�f�[�^�t�@�C���ǂݍ���
	for (int i = 0; i < numFiles; i++) {
		HANDLE handle = CreateFile(filenames[i],
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		emoteObjectSize[i] = GetFileSize(handle, NULL);
		emoteObjectImage[i] = new BYTE[emoteObjectSize[i]];
		DWORD dummy;
		ReadFile(handle, emoteObjectImage[i], emoteObjectSize[i], &dummy, NULL);
		CloseHandle(handle);

#if 0
		// E-mote�t�@�C���C���[�W�Ɏ��O�Ƀt�B���^�������s��
		EmoteFilterTexture(emoteObjectImage[i], emoteObjectSize[i], GrayscaleFilter);
#endif
	}

	// �v���C�����쐬����
	sEmoteDevice->CreatePlayer(numFiles, (const emote_uint8_t **)&emoteObjectImage, (const emote_uint32_t *)&emoteObjectSize, player);

	// �����̏I������t�@�C���C���[�W��j��
	for (int i = 0; i < numFiles; i++)
		delete[] emoteObjectImage[i];

	// �\���J�n
	(*player)->Show();
}

//------------------------------------------------
// �����_�[�X�e�[�g��������
void
D3DInitRenderState(int width, int height, float rep_x, float rep_y)
{
	if (G_d3d.device.body() == NULL)
		return;

	D3DXMATRIX matTemp;

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	// ���s�ړ�
	D3DXMatrixMultiply(&matWorld, &matWorld, D3DXMatrixTranslation(&matTemp, -rep_x, rep_y, 0.0f));
	
	D3DXMATRIX matProj;
	D3DXMatrixIdentity(&matProj);

	float aspect = 1.0f * width / height;
	matProj._11 = 1.0f / aspect;
	matProj._34 = 1.0f;
	matProj._44 = 0.0f;
	matProj._41 = 0.0f;
	matProj._42 = 0.0f;

	float scale = 1.0f;
	float fw = (float)width;
	float fh = (float)height;

	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3(-0.5f, 0.5f, -fh / 2),
		&D3DXVECTOR3(-0.5f, 0.5f, 0),
		&D3DXVECTOR3(0, -1, 0));

	matView._11 = -matView._11;

	// E-mote�h���C�o�́A�ȉ�3�̍s��l�ɉ����Ďˉe���s���B
	G_d3d.device->SetTransform(D3DTS_WORLD, &matWorld);
	G_d3d.device->SetTransform(D3DTS_VIEW, &matView);
	G_d3d.device->SetTransform(D3DTS_PROJECTION, &matProj);

	// �����_�����O�^�[�Q�b�g�`��p�ɍŒ���̃p�����[�^������
	G_d3d.device->SetFVF(FVF_LVERTEX);
	G_d3d.device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	G_d3d.device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	G_d3d.device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	G_d3d.device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	G_d3d.device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	G_d3d.device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	G_d3d.device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	G_d3d.device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	G_d3d.device->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
	G_d3d.device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
	G_d3d.device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
	G_d3d.device->SetRenderState(D3DRS_LIGHTING, FALSE);
	G_d3d.device->SetRenderState(D3DRS_ZENABLE, FALSE);
	G_d3d.device->SetRenderState(D3DRS_ALPHAREF, 0x00);
	G_d3d.device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	G_d3d.device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	G_d3d.device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	G_d3d.device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	G_d3d.device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	G_d3d.device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	G_d3d.device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	G_d3d.device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	G_d3d.device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	G_d3d.device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	G_d3d.device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
}

//------------------------------------------------
// �v���C���j��
void EmoteDeletePlayer(void)
{
//	while (!sClonePlayerList.empty())
//		EmoteDeletePlayer();
}

//------------------------------------------------
// �t���[���X�V
void EmoteUpdate(IEmotePlayer* player, float ms)
{
	if (player)
	{
		//	float x_rep = std::sinf((float)(Gp_timer->local_game_time) / 2000 * D3DX_PI) * 100;
		//	sEmotePlayer->SetCoord(x_rep, 0.0f);

		// E-mote��1/60�b��1�P�ʂŋ쓮����̂Ŏ��ԒP�ʂ�ϊ��B
		player->Progress(ms * MSTOF60THS);

		//	for (emote_uint32_t i = 0; i < sClonePlayerList.size(); i++)
		//		sClonePlayerList[i]->Progress(ms * MSTOF60THS);

		//player->SetVariable("face_talk", (float)(rand() % 1000) / 200);
	}
}

//------------------------------------------------
// �`��
void EmoteDraw(IEmotePlayer* player, BSP<C_d3d_texture> texture, float rep_x, float rep_y)
{
	if (player)
	{
		D3DInitRenderState(texture->get_width(), texture->get_height(), rep_x, rep_y);

		sEmoteDevice->OnRenderTarget(texture->body());

		player->Render();

		//	for (emote_uint32_t i = 0; i < sClonePlayerList.size(); i++) {
		//		sEmoteDevice->OnRenderTarget(texture.body());
		//		sClonePlayerList[i]->Render();
		//	}
	}
}

// ****************************************************************
// E-mote �Ăяo��
// ================================================================

void tnm_emote_init()
{
	EmoteInit();

	// �e�N�X�`���쐬
//	G_emote_texture = G_rsm.create_texture(_T("emote"), 1024, 1024, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, true);
//	G_emote_dpst_buffer = G_rsm.create_dpst_buffer(_T("emote"), 1024, 1024, true);
}

void tnm_emote_free()
{
	EmoteDeletePlayer();

//	G_emote_texture.reset();
//	G_emote_dpst_buffer.reset();
}

void tnm_emote_on_device_lost()
{
	sEmoteDevice->OnDeviceLost();
}

void tnm_emote_load(ARRAY<TSTR> file_path_list, IEmotePlayer** player)
{
	ARRAY<const wchar_t*> file_path_list_c_str;

	for (int i = 0; i < (int)file_path_list.size(); i++)
	{
		file_path_list_c_str.push_back(file_path_list[i].c_str());
	}

	// E-mtoe�f�[�^�t�@�C���ǂݍ���
	EmoteNewPlayer(file_path_list_c_str.size(), file_path_list_c_str.get(), player);
}

void tnm_emote_frame(IEmotePlayer* player, float ms)
{
	EmoteUpdate(player, ms);
}

void tnm_emote_draw(IEmotePlayer* player, BSP<C_d3d_texture> texture)
{
	EmoteDraw(player, texture, 0.0f, 0.0f);
}

void tnm_emote_draw(IEmotePlayer* player, BSP<C_d3d_texture> texture, float rep_x, float rep_y)
{
	EmoteDraw(player, texture, rep_x, rep_y);
}

#endif

