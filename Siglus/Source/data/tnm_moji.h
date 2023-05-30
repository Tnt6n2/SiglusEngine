#pragma		once

// ****************************************************************
// 文字タイプ
// ================================================================
const int TNM_MOJI_TYPE_MOJI = 0;
const int TNM_MOJI_TYPE_EMOJI_A = 1;
const int TNM_MOJI_TYPE_EMOJI_B = 2;


// ****************************************************************
// 文字
// ================================================================
struct C_tnm_moji
{
	int		type;		// 文字タイプ
	int		code;		// 文字
	int		size;		// 文字サイズ
	int		color;		// 文字色
	int		shadow;		// 文字影色
	int		fuchi;		// 文字縁色
	C_point	pos;		// 文字位置
};

