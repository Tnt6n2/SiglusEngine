#pragma		once

#include	"tnm_source_angou.h"

// ****************************************************************
// タイル暗号
// ================================================================
class C_tnms_source_angou
{
public:
	static	void	encryption(BYTE *buf, DWORD buf_size, CTSTR& buf_name, BUFFER& dst_buf);		// 暗号化

};
