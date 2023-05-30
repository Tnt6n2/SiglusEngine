#pragma		once

// ****************************************************************
// �g���L�[
// ================================================================
const int	VK_EX_DECIDE = 0x100;
const int	VK_EX_CANCEL = 0x101;
const int	VK_EX_KEYBOARD_DECIDE = 0x102;

// ****************************************************************
// �C���v�b�g�̃x�[�X
// ================================================================
class C_tnm_input_base : public NT3::C_std_input
{
public:
	void	clear_ex_key(int ex_key);

	bool	is_ex_key_down(int ex_key);
	bool	is_ex_key_up(int ex_key);

	bool	check_ex_key_down(int ex_key);
	bool	check_ex_key_up(int ex_key);
	bool	check_ex_key_down_up(int ex_key);

	bool	use_ex_key_down(int ex_key);
	bool	use_ex_key_up(int ex_key);
	bool	use_ex_key_down_up(int ex_key);
};

// ****************************************************************
// �C���v�b�g
// ================================================================
class C_tnm_input : public C_tnm_input_base
{
};

// ****************************************************************
// �X�N���v�g�C���v�b�g
// ================================================================
class C_tnm_script_input : public C_tnm_input_base
{
public:
	void	frame();
};
