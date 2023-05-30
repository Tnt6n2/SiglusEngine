#include	"pch.h"
#include	"lzss_tree.h"

// ****************************************************************
// ‚k‚y‚r‚rƒcƒŠ[ €”õ
// ================================================================
void Clzss_tree::ready(DWORD tree_size_)
{
	tree_size = tree_size_;
	tree.resize(tree_size + 2);
	root = tree_size;
	unused = tree_size + 1;

	for (DWORD i = 0; i < tree_size + 2; i++)	{
		tree[i].parent = unused;
		tree[i].sml    = unused;
		tree[i].big    = unused;
	}

	tree[0].parent = root;

	tree[root].parent = 0;
	tree[root].big    = 0;
}

// ****************************************************************
// ‚k‚y‚r‚rƒcƒŠ[ Ú‘±
// ================================================================
void Clzss_tree::connect(DWORD target)
{
	TREE* tt = &tree[target];
	if (tt->parent == unused)	{	return;		}

	DWORD parent = tt->parent;
	TREE* pt = &tree[parent];
	DWORD next;

	if (tt->big == unused)	{
		next = tt->sml;
		tree[next].parent = parent;
		if (pt->big == target)	{	pt->big = next;	}
		else					{	pt->sml = next;	}
		tt->parent = unused;
	}
	else if(tt->sml == unused)	{
		next = tt->big;
		tree[next].parent = parent;
		if (pt->big == target)	{	pt->big = next;	}
		else					{	pt->sml = next;	}
		tt->parent = unused;
	}
	else	{
		next = tt->sml;
		while (tree[next].big != unused)	{	next = tree[next].big;	}
		connect(next);
		replace(target, next);
	}
}

// ****************************************************************
// ‚k‚y‚r‚rƒcƒŠ[ “ü‚ê‘Ö‚¦
// ================================================================
void Clzss_tree::replace(DWORD target, DWORD next)
{
	TREE* tt = &tree[target];

	DWORD parent = tt->parent;
	TREE* pt = &tree[parent];

	if (pt->sml == target)	{	pt->sml = next;	}
	else					{	pt->big = next;	}
	tree[next] = *tt;
	tree[tt->sml].parent = next;
	tree[tt->big].parent = next;

	tt->parent = unused;
}

// ****************************************************************
// ‚k‚y‚r‚rƒcƒŠ[ ’Ç‰ÁÚ‘±
// ================================================================
bool Clzss_tree::additional_connect(DWORD& target, DWORD next, int matching_result)
{
	TREE* tt = &tree[target];
	DWORD* child;

	if (matching_result >= 0)	{	child = &tt->big;	}
	else						{	child = &tt->sml;	}

	if (*child != unused)	{
		target = *child;
		return false;
	}

	*child = next;
	tree[next].parent = target;
	tree[next].big    = unused;
	tree[next].sml    = unused;
	return true;
}

// ****************************************************************
// ‚k‚y‚r‚rƒcƒŠ[ ƒ‹[ƒg‚Ì‚a‚h‚f‚ğæ“¾
// ================================================================
DWORD Clzss_tree::get_root_big()
{
	return tree[root].big;
}

