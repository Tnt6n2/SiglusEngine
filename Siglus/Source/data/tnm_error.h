#pragma		once

#include	"data/tnm_def.h"

// ****************************************************************
// �G���[�\����
// ================================================================
struct S_tnm_error
{
	TNM_ERROR_TYPE	error_type;
	TSTR			error_str;
};

// ****************************************************************
// �G���[����
// ================================================================
class C_tnm_error
{
public:
	void	init();
	bool	set_error(TNM_ERROR_TYPE error_type, CTSTR& error_str);

	ARRAY<S_tnm_error>	m_error_list;
	TNM_ERROR_TYPE		m_last_error;

private:
	ARRAY<TSTR>			m_rest_error_list;
};

