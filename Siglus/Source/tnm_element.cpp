#include	"pch.h"

#include	"data/tnm_element.h"

// ****************************************************************
// ÉGÉåÉÅÉìÉg
// ================================================================
S_element create_element(int code_00)
{
	S_element element;
	element.code[0] = code_00;
	element.code_cnt = 1;
	return element;
}

S_element create_element(int code_00, int code_01)
{
	S_element element;
	element.code[0] = code_00;
	element.code[1] = code_01;
	element.code_cnt = 2;
	return element;
}

S_element create_element(int code_00, int code_01, int code_02)
{
	S_element element;
	element.code[0] = code_00;
	element.code[1] = code_01;
	element.code[2] = code_02;
	element.code_cnt = 3;
	return element;
}

S_element create_element(int code_00, int code_01, int code_02, int code_03)
{
	S_element element;
	element.code[0] = code_00;
	element.code[1] = code_01;
	element.code[2] = code_02;
	element.code[3] = code_03;
	element.code_cnt = 4;
	return element;
}

S_element create_element(int code_00, int code_01, int code_02, int code_03, int code_04)
{
	S_element element;
	element.code[0] = code_00;
	element.code[1] = code_01;
	element.code[2] = code_02;
	element.code[3] = code_03;
	element.code[4] = code_04;
	element.code_cnt = 5;
	return element;
}

S_element create_element(int code_00, int code_01, int code_02, int code_03, int code_04, int code_05)
{
	S_element element;
	element.code[0] = code_00;
	element.code[1] = code_01;
	element.code[2] = code_02;
	element.code[3] = code_03;
	element.code[4] = code_04;
	element.code[5] = code_05;
	element.code_cnt = 6;
	return element;
}

S_element create_element(int* begin, int* end)
{
	S_element element;
	memcpy(element.code, begin, (end - begin) * sizeof(int));
	element.code_cnt = (int)(end - begin);
	return element;
}

