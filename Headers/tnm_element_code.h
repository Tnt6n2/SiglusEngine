#pragma		once

// ****************************************************************
// エレメントコード
// ================================================================
#define		create_elm_code(owner, group, code)		(((owner) << 24) + ((group) << 16) + (code))
#define		get_elm_owner(code)						(((code) >> 24) & 0x00ff)
#define		get_elm_group(code)						(((code) >> 16) & 0x00ff)
#define		get_elm_code(code)						(((code) >>  0) & 0xffff)

const int	ELM_OWNER_USER_PROP		= 127;		// ユーザプロパティ
const int	ELM_OWNER_USER_CMD		= 126;		// ユーザコマンド
const int	ELM_OWNER_CALL_PROP		= 125;		// コールプロパティ
const int	ELM_OWNER_CALL_CMD		= 124;		// コールコマンド（存在しない）
const int	ELM_ARRAY				= -1;		// 配列
//const int	ELM___SET				= -2;		// 代入
//const int	ELM___TRANS				= -3;		// 変換
const int	ELM_CURRENT				= -4;		// カレント
const int	ELM_UP					= -5;		// カレントより１つ上

#define		ELEMENT(type, parent, form, name, owner, group, code, arg_str)	\
const int	ELM_##parent##_##name = ((owner << 24) + (group << 16) + code);
#include	"def_element_Siglus.h"
#undef		ELEMENT

