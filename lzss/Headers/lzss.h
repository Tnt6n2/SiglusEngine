#pragma		once

// ****************************************************************
// �C���N���[�h
// ================================================================
#include	"tpc_angou.h"

#include	"tpc_lzss.h"

#include	"lzss_tree.h"
#include	"lzss_tree_find.h"

#include	"lzss_pack.h"
#include	"lzss_unpack.h"
#include	"lzss_pack_easy.h"

#include	"lzss32_pack.h"
#include	"lzss32_unpack.h"
#include	"lzss32_pack_easy.h"

// ****************************************************************
// ���C�u����
// ================================================================
#ifdef _DEBUG
	#pragma		comment(lib,"lzss_d.lib")
#else
	#pragma		comment(lib,"lzss.lib")
#endif

