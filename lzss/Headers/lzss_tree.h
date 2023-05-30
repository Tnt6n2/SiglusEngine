#pragma		once

// ****************************************************************
// �k�y�r�r�c���[
// ================================================================
class Clzss_tree
{

public:
	void	ready(DWORD tree_size_);												// ����
	void	connect(DWORD target);													// �ڑ�
	void	replace(DWORD target, DWORD next);										// ����ւ�
	bool	additional_connect(DWORD& target, DWORD next, int matching_result);		// �ǉ��ڑ�
	DWORD	get_root_big();															// ���[�g�̂a�h�f���擾

private:

	struct TREE{
		DWORD parent;
		DWORD sml;
		DWORD big;
	};

	std::vector<TREE>	tree;
	DWORD tree_size;
	DWORD root;
	DWORD unused;
};

