#pragma		once

// ****************************************************************
// ‚k‚y‚r‚rƒcƒŠ[
// ================================================================
class Clzss_tree
{

public:
	void	ready(DWORD tree_size_);												// €”õ
	void	connect(DWORD target);													// Ú‘±
	void	replace(DWORD target, DWORD next);										// “ü‚ê‘Ö‚¦
	bool	additional_connect(DWORD& target, DWORD next, int matching_result);		// ’Ç‰ÁÚ‘±
	DWORD	get_root_big();															// ƒ‹[ƒg‚Ì‚a‚h‚f‚ğæ“¾

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

