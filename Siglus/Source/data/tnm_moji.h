#pragma		once

// ****************************************************************
// �����^�C�v
// ================================================================
const int TNM_MOJI_TYPE_MOJI = 0;
const int TNM_MOJI_TYPE_EMOJI_A = 1;
const int TNM_MOJI_TYPE_EMOJI_B = 2;


// ****************************************************************
// ����
// ================================================================
struct C_tnm_moji
{
	int		type;		// �����^�C�v
	int		code;		// ����
	int		size;		// �����T�C�Y
	int		color;		// �����F
	int		shadow;		// �����e�F
	int		fuchi;		// �������F
	C_point	pos;		// �����ʒu
};

