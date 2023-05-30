#pragma		once

// ****************************************************************
// �g�[���J�[�u�F�T�u
// ================================================================
class C_tnm_tonecurve_sub
{
public:
	C_tnm_tonecurve_sub()
	{
		sat = 0;
		exist = 0;
	}

	int		sat;		// �ʓx
	int		exist;		// ���݃t���O
};

// ****************************************************************
// �g�[���J�[�u
// ================================================================
class C_tnm_tonecurve
{
public:
	void				init();
	bool				check_tonecurve_no(int tonecurve_no);
	BSP<C_d3d_texture>	get_texture();
	int					get_sat(int tonecurve_no);

private:
	ARRAY<C_tnm_tonecurve_sub>		m_sub_list;
	BSP<C_d3d_texture>				m_texture;
};
