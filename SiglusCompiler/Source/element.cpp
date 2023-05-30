#include	"pch.h"
#include	"code.h"
#include	"element.h"

// ****************************************************************
// 型定義
// ================================================================
#pragma optimize("", off)
static S_tnms_form_def Gtnms_form_def_list[] = 
{
	#define		FORM(name, code)	\
	{_T(#name), code}, 
#ifdef __SIGLUS
	#include	"def_form_Siglus.h"
#endif
#ifdef __KINETIC
	#include	"def_form_Kinetic.h"
#endif
	#undef		FORM
};
#pragma optimize("", on)

// ****************************************************************
// エレメント定義
// ================================================================
#pragma optimize("", off)
static S_tnms_element_def Gtnms_element_def_list[] = 
{
	#define		ELEMENT(type, parent, form, name, owner, group, code, arg_str)	\
	{TNMS_ELEMENT_TYPE_##type, FM_##parent, FM_##form, _T(#name), owner, group, code, _T(arg_str)}, 
#ifdef __SIGLUS
	#include	"def_element_Siglus.h"
#endif
#ifdef __KINETIC
	#include	"def_element_Kinetic.h"
#endif
	#undef		ELEMENT
};
#pragma optimize("", on)

// ****************************************************************
// システム定義の型テーブルを作る
//		def_element.h を解析してテーブルを作ります。
// ================================================================
void C_tnms_form_table::create_system_form_table()
{
	// 現在のテーブルをクリア
	form_map_by_code.clear();
	form_map_by_name.clear();

	// 型マップを作成
	for (int f = 0; f < sizeof_array(Gtnms_form_def_list); f++)	{
		S_tnms_form_def* def = &Gtnms_form_def_list[f];
		S_tnms_form form;

		form.code = def->code;
		form.name = str_to_lower_ret(def->name);
		add_form(&form);
	}

	// エレメントマップを作成
	for (int e = 0; e < sizeof_array(Gtnms_element_def_list); e++)	{
		S_tnms_element_def* def = &Gtnms_element_def_list[e];
		S_tnms_element element;

		element.type = def->type;
		element.name = str_to_lower_ret(def->name);
		element.form = def->form;
		element.code = ((int)def->owner << 24) + ((int)def->group << 16) + def->code;

		// 引数テーブル解析
		int arg_list_id;
		TSTR arg_form;
		C_str_analizer ana(def->arg_info.begin(), def->arg_info.end(), _T(""));

		while (1)	{
			if (!ana.get_integer(&arg_list_id))		break;		// 0
			if (!ana.check_moji(_T(':')))			break;		// :

			// 引数リスト解析
			S_tnms_arg_list arg_list;
			while (1)	{
				if (ana.check_moji(_T(';')))		break;	// ;

				if (false);
				else if (ana.check_str(_T("__argsref")))	{
					S_tnms_arg arg;
					arg.form = FM___ARGSREF;
					arg_list.arg_list.push_back(arg);
					ana.check_moji(_T(','));
				}
				else if (ana.check_str(_T("__args")))	{
					S_tnms_arg arg;
					arg.form = FM___ARGS;
					arg_list.arg_list.push_back(arg);
					ana.check_moji(_T(','));
				}
				else	{
					S_tnms_arg arg;

					if (arg_list_id != -1)	{

						// 式引数（例：int）
						if (!ana.get_word(arg_form))		break;	// int
					}
					else	{

						// 名前つき引数（例：0=type=int）
						if (!ana.get_num(&arg.id))			break;	// 0
						if (!ana.check_moji(_T('=')))		break;	// =
						if (!ana.get_word(arg.name))		break;	// type
						if (!ana.check_moji(_T('=')))		break;	// =
						if (!ana.get_word(arg_form))		break;	// int
					}

					arg.form = get_form_code_by_name(arg_form);
					arg_list.arg_list.push_back(arg);
					ana.check_moji(_T(','));
				}
			}

			// 引数リストが完成したので追加する
			element.add_arg_list(arg_list_id, arg_list);
		}

		// 型の情報を取得
		S_tnms_form* form = get_form_by_code(def->parent);
		// 型にエレメントを登録
		form->add_element(&element);
	}

	// call 型を保存しておく
	form_call = form_map_by_code[FM_CALL];
}

// ****************************************************************
// 型テーブル：型を追加
// ================================================================
void C_tnms_form_table::add_form(S_tnms_form* form)
{
	// エレメントをエレメントテーブルに登録
	form_map_by_code.insert(std::pair<int, S_tnms_form>(form->code, *form));
	form_map_by_name.insert(std::pair<TSTR, S_tnms_form>(form->name, *form));
}


// ****************************************************************
// 型テーブル：型の名前から型を取得
// ================================================================
S_tnms_form* C_tnms_form_table::get_form_by_name(CTSTR& name)
{
	std::map<TSTR, S_tnms_form>::iterator itr = form_map_by_name.find(name);
	if (itr == form_map_by_name.end())
		return NULL;

	return &itr->second;
}

// ****************************************************************
// 型テーブル：型コードから型を取得
// ================================================================
S_tnms_form* C_tnms_form_table::get_form_by_code(int code)
{
	return &form_map_by_code[code];
}

// ****************************************************************
// 型テーブル：エレメントを検索してエレメントの情報を取得する
// ================================================================
S_tnms_element* C_tnms_form_table::find_element_info(CTSTR& element_name, int *ret_parent_form_code)
{
	// 先頭エレメントの情報を探す
	S_tnms_element* element_info = NULL;
	// 先頭なので FM_CALL / FM_SCENE / FM_GLOBAL の３パターンが考えられる
	int parent_form_code = FM_CALL;
	if (!get_element_info(parent_form_code, element_name, &element_info))	{
		parent_form_code = FM_SCENE;
		if (!get_element_info(parent_form_code, element_name, &element_info))	{
			parent_form_code = FM_GLOBAL;
			if (!get_element_info(parent_form_code, element_name, &element_info))
				return NULL;
		}
	}
	if (ret_parent_form_code)	{	*ret_parent_form_code = parent_form_code;	}
	return element_info;
}

// ****************************************************************
// 型テーブル：エレメントを検索して見つかれば true を返す
// ================================================================
bool C_tnms_form_table::check_element_info(CTSTR& element_name)
{
	return (find_element_info(element_name, NULL)) ? true : false;
}

// ****************************************************************
// 型テーブル：エレメントの情報を取得する
// ================================================================
bool C_tnms_form_table::get_element_info(int parent_form_code, TSTR name, S_tnms_element** element_info)
{
	// 親の型とエレメントの情報を取得
	S_tnms_form* form_info = get_form_by_code(parent_form_code);
	if (!form_info)
		return false;

	// エレメントの情報を取得
	*element_info = form_info->get_element_by_name(name);
	if (!*element_info)
		return false;

	return true;
}

// ****************************************************************
// 型：クリア
// ================================================================
void S_tnms_form::clear()
{
	element_map_by_code.clear();
	element_map_by_name.clear();
}

// ****************************************************************
// 型：エレメントを追加
// ================================================================
void S_tnms_form::add_element(S_tnms_element* element)
{
	// エレメントをエレメントテーブルに登録
	element_map_by_code.insert(std::pair<int, S_tnms_element>(element->code, *element));
	element_map_by_name.insert(std::pair<TSTR, S_tnms_element>(element->name, *element));
}

// ****************************************************************
// 型：エレメントの名前からエレメントを取得
// ================================================================
S_tnms_element* S_tnms_form::get_element_by_name(CTSTR& name)
{
	std::map<TSTR, S_tnms_element>::iterator itr = element_map_by_name.find(name);
	if (itr == element_map_by_name.end())
		return NULL;

	return &itr->second;
}

// ****************************************************************
// 型：エレメントコードからエレメントを取得
// ================================================================
S_tnms_element* S_tnms_form::get_element_by_code(int code)
{
	return &element_map_by_code[code];
}

// ****************************************************************
// エレメント：引数リストを追加
// ================================================================
void S_tnms_element::add_arg_list(int arg_list_id, S_tnms_arg_list& arg_list)
{
	// エレメントをエレメントテーブルに登録
	arg_map.insert(std::pair<int, S_tnms_arg_list>(arg_list_id, arg_list));
}

// ****************************************************************
// 型名 → 型に変換
// ================================================================
int get_form_code_by_name(CTSTR& name)
{
	if (false);
	else if (name == _T("void"))			return FM_VOID;
	else if (name == _T("voidlist"))		return FM_VOIDLIST;
	else if (name == _T("int"))				return FM_INT;
	else if (name == _T("intlist"))			return FM_INTLIST;
	else if (name == _T("intlistlist"))		return FM_INTLISTLIST;
	else if (name == _T("intref"))			return FM_INTREF;
	else if (name == _T("intlistref"))		return FM_INTLISTREF;
	else if (name == _T("str"))				return FM_STR;
	else if (name == _T("strlist"))			return FM_STRLIST;
	else if (name == _T("strlistlist"))		return FM_STRLISTLIST;
	else if (name == _T("strref"))			return FM_STRREF;
	else if (name == _T("strlistref"))		return FM_STRLISTREF;
	else if (name == _T("intevent"))		return FM_INTEVENT;
	else if (name == _T("inteventlist"))	return FM_INTEVENTLIST;
	else if (name == _T("allevent"))		return FM_ALLEVENT;
	else if (name == _T("global"))			return FM_GLOBAL;
	else if (name == _T("globallist"))		return FM_GLOBALLIST;
	else if (name == _T("scene"))			return FM_SCENE;
	else if (name == _T("scenelist"))		return FM_SCENELIST;
	else if (name == _T("call"))			return FM_CALL;
	else if (name == _T("calllist"))		return FM_CALLLIST;
	else if (name == _T("math"))			return FM_MATH;
	else if (name == _T("file"))			return FM_FILE;
	else if (name == _T("cgtable"))			return FM_CGTABLE;
	else if (name == _T("database"))		return FM_DATABASE;
	else if (name == _T("databaselist"))	return FM_DATABASELIST;
	else if (name == _T("counter"))			return FM_COUNTER;
	else if (name == _T("counterlist"))		return FM_COUNTERLIST;
	else if (name == _T("frameaction"))		return FM_FRAMEACTION;
	else if (name == _T("frameactionlist"))	return FM_FRAMEACTIONLIST;
	else if (name == _T("world"))			return FM_WORLD;
	else if (name == _T("worldlist"))		return FM_WORLDLIST;
	else if (name == _T("stage"))			return FM_STAGE;
	else if (name == _T("stagelist"))		return FM_STAGELIST;
	else if (name == _T("object"))			return FM_OBJECT;
	else if (name == _T("objectlist"))		return FM_OBJECTLIST;
	else if (name == _T("objectevent"))		return FM_OBJECTEVENT;
	else if (name == _T("objecteventlist"))	return FM_OBJECTEVENTLIST;
	else if (name == _T("mwnd"))			return FM_MWND;
	else if (name == _T("mwndlist"))		return FM_MWNDLIST;
	else if (name == _T("group"))			return FM_GROUP;
	else if (name == _T("grouplist"))		return FM_GROUPLIST;
	else if (name == _T("screen"))			return FM_SCREEN;
	else if (name == _T("quake"))			return FM_QUAKE;
	else if (name == _T("quakelist"))		return FM_QUAKELIST;
	else if (name == _T("editbox"))			return FM_EDITBOX;
	else if (name == _T("editboxlist"))		return FM_EDITBOXLIST;
	else if (name == _T("bgm"))				return FM_BGM;
	else if (name == _T("bgmlist"))			return FM_BGMLIST;
	else if (name == _T("koe"))				return FM_KOE;
	else if (name == _T("koelist"))			return FM_KOELIST;
	else if (name == _T("pcm"))				return FM_PCM;
	else if (name == _T("pcmch"))			return FM_PCMCH;
	else if (name == _T("pcmchlist"))		return FM_PCMCHLIST;
	else if (name == _T("se"))				return FM_SE;
	else if (name == _T("mov"))				return FM_MOV;
	else if (name == _T("pcmevent"))		return FM_PCMEVENT;
	else if (name == _T("pcmeventlist"))	return FM_PCMEVENTLIST;
	else if (name == _T("mouse"))			return FM_MOUSE;
	else if (name == _T("key"))				return FM_KEY;
	else if (name == _T("keylist"))			return FM_KEYLIST;
	else if (name == _T("input"))			return FM_INPUT;
	else if (name == _T("syscom"))			return FM_SYSCOM;
	else if (name == _T("syscommenu"))		return FM_SYSCOMMENU;
	else if (name == _T("mwndbtn"))			return FM_MWNDBTN;
	else if (name == _T("script"))			return FM_SCRIPT;
	else if (name == _T("system"))			return FM_SYSTEM;
	else if (name == _T("excall"))			return FM_EXCALL;
	else
		return -1;
}
