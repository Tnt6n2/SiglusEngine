#include	"pch.h"
#include	"BS.h"
#include	"code.h"
#include	"element.h"
#include	"tnm_common_header.h"

#define		add_out_txt(s)	if (m_is_test)	add_out_txt(s)

std::map<TSTR, int> G_name_cntr;

// ****************************************************************
// バイナリセーバー - Bynary Saver -
// ================================================================

// 値判定
inline bool is_value(int form)
{
	switch (form)	{
		case FM_VOID:
		case FM_INT:
		case FM_STR:
		case FM_INTLIST:
		case FM_STRLIST:
			return true;
		default:
			return false;
	};
}

// 参照判定
inline bool is_reference(int form)
{
	return !is_value(form);
}

// 参照外し
int dereference(int form)
{
	if (form == FM_INTREF)		return FM_INT;
	if (form == FM_STRREF)		return FM_STR;
	if (form == FM_INTLISTREF)	return FM_INTLIST;
	if (form == FM_STRLISTREF)	return FM_STRLIST;
	return form;
}


// ****************************************************************
// コンパイルメイン
// ================================================================
bool C_tnms_binary_saver::compile(S_tnms_ia_data* piad, S_tnms_la_data* plad, S_tnms_sa_data* psad, S_tnms_bs_data* pbsd, bool is_test)
{
	int offset = 0;

	m_piad = piad;
	m_plad = plad;
	m_psad = psad;
	m_pbsd = pbsd;
	m_is_test = is_test;

	// バイナリセーバーデータのクリア
	m_pbsd->out_scn.clear();
	m_pbsd->out_dbg.clear();
	m_pbsd->out_txt.clear();

	// 出力データのクリア
	out_scn.scn.clear();
	out_scn.str_list.clear();
	out_scn.label_list.clear();
	out_scn.z_label_list.clear();
	out_scn.cmd_label_list.clear();
	out_scn.scn_prop_list.clear();
	out_scn.scn_prop_name_list.clear();
	out_scn.call_prop_name_list.clear();
	out_scn.scn_cmd_list.clear();
	out_scn.scn_cmd_name_list.clear();
	out_scn.namae_list.clear();
	out_scn.read_flag_list.clear();
	out_txt.clear();
	loop_label.clear();
	cur_read_flag_no = 0;

	// 文字列リストを作成

	// ソースを解読しにくくするため、文字列を並び替える
	int str_cnt = (int)m_plad->str_list.size();
	ARRAY<int> str_sort_index(str_cnt);
	for (int i = 0; i < str_cnt; i++)	{
		str_sort_index[i] = i;
	}
	std::random_shuffle(str_sort_index.begin(), str_sort_index.end());

	// 文字列を並び替えつつ詰め込んでいく
	offset = 0;
	out_scn.str_list.resize(str_cnt);
	out_scn.str_index_list.resize(str_cnt);
	for (int i = 0; i < str_cnt; i++)	{
		out_scn.str_list[i] = m_plad->str_list[str_sort_index[i]];
		out_scn.str_index_list[str_sort_index[i]].offset = offset;
		out_scn.str_index_list[str_sort_index[i]].size = (int)m_plad->str_list[str_sort_index[i]].size();
		offset += (int)m_plad->str_list[str_sort_index[i]].size();
	}

	//offset = 0;
	//out_scn.str_list.resize((int)m_plad->str_list.size());
	//out_scn.str_index_list.resize((int)m_plad->str_list.size());
	//for (int i = 0; i < (int)m_plad->str_list.size(); i++)	{
	//	out_scn.str_list[i] = m_plad->str_list[i];
	//	out_scn.str_index_list[i].offset = offset;
	//	out_scn.str_index_list[i].size = (int)m_plad->str_list[i].size();
	//	offset += (int)m_plad->str_list[i].size();
	//}

	// ラベルリスト、Ｚラベルリストを確保（コンパイル時に構築される）
	out_scn.label_list.resize(m_plad->label_list.size());
	out_scn.z_label_list.resize(TNM_Z_LABEL_CNT, 0);

	// シーンプロパティリストを作成
	out_scn.scn_prop_list.resize((int)m_piad->property_list.size() - m_piad->inc_property_cnt);
	for (int i = 0; i < (int)m_piad->property_list.size() - m_piad->inc_property_cnt; i++)	{
		int property_id = i + m_piad->inc_property_cnt;
		out_scn.scn_prop_list[i].form = m_piad->property_list[property_id]->form;
		out_scn.scn_prop_list[i].size = m_piad->property_list[property_id]->size;
	}

	// シーンプロパティ名リストを作成
	offset = 0;
	out_scn.scn_prop_name_index_list.resize((int)m_piad->property_list.size() - m_piad->inc_property_cnt);
	out_scn.scn_prop_name_list.resize((int)m_piad->property_list.size() - m_piad->inc_property_cnt);
	for (int i = 0; i < (int)m_piad->property_list.size() - m_piad->inc_property_cnt; i++)	{
		int property_id = i + m_piad->inc_property_cnt;
		out_scn.scn_prop_name_list[i] = m_piad->property_list[property_id]->name;
		out_scn.scn_prop_name_index_list[i].offset = offset;
		out_scn.scn_prop_name_index_list[i].size = (int)m_piad->property_list[property_id]->name.size();
		offset += (int)m_piad->property_list[property_id]->name.size();
	}

	// シーンコマンドリストを確保（コンパイル時に構築される）
	out_scn.scn_cmd_list.resize((int)m_piad->command_list.size() - m_piad->inc_command_cnt);

	// シーンコマンド名リストを作成
	offset = 0;
	out_scn.scn_cmd_name_index_list.resize((int)m_piad->command_list.size() - m_piad->inc_command_cnt);
	out_scn.scn_cmd_name_list.resize((int)m_piad->command_list.size() - m_piad->inc_command_cnt);
	for (int i = 0; i < (int)m_piad->command_list.size() - m_piad->inc_command_cnt; i++)	{
		int command_id = i + m_piad->inc_command_cnt;
		out_scn.scn_cmd_name_list[i] = m_piad->command_list[command_id]->name;
		out_scn.scn_cmd_name_index_list[i].offset = offset;
		out_scn.scn_cmd_name_index_list[i].size = (int)m_piad->command_list[command_id]->name.size();
		offset += (int)m_piad->command_list[command_id]->name.size();
	}

	// コールプロパティ名リストを作成
	offset = 0;
	out_scn.call_prop_name_index_list.resize((int)m_psad->call_prop_name_list.size());
	out_scn.call_prop_name_list.resize((int)m_psad->call_prop_name_list.size());
	for (int i = 0; i < (int)m_psad->call_prop_name_list.size(); i++)	{
		out_scn.call_prop_name_list[i] = m_psad->call_prop_name_list[i];
		out_scn.call_prop_name_index_list[i].offset = offset;
		out_scn.call_prop_name_index_list[i].size = (int)m_psad->call_prop_name_list[i].size();
		offset += (int)m_psad->call_prop_name_list[i].size();
	}

	// ss コンパイル
	if (!bs_ss(m_psad->root))
		return false;

	// ================================================================
	// シーンデータを結合
	// ----------------------------------------------------------------

	// ヘッダ
	out_scn.header.header_size = sizeof(S_tnm_scn_header);
	m_pbsd->out_scn.push(out_scn.header);
	// 文字列インデックスリスト
	out_scn.header.str_index_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.str_index_cnt = (int)out_scn.str_index_list.size();
	m_pbsd->out_scn.push_array(out_scn.str_index_list.get(), (int)out_scn.str_index_list.size());
	// 文字列リスト
	out_scn.header.str_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.str_cnt = (int)out_scn.str_list.size();
	for (int i = 0; i < (int)out_scn.str_list.size(); i++)	{
		TSTR str = m_plad->str_list[str_sort_index[i]];
		int str_len = (int)str.size();
		for (int j = 0; j < str_len; j++)	{
			WORD moji = ((WORD)str[j]) ^ (28807 * str_sort_index[i]);
			m_pbsd->out_scn.push(moji);
		}
		//m_pbsd->out_scn.push_str(out_scn.str_list[i]);
	}
	// シーン
	out_scn.header.scn_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.scn_size = out_scn.scn.size();
	m_pbsd->out_scn.push_array(out_scn.scn.get(), (int)out_scn.scn.size());
	// ラベルリスト
	out_scn.header.label_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.label_cnt = (int)out_scn.label_list.size();
	m_pbsd->out_scn.push_array(out_scn.label_list.get(), (int)out_scn.label_list.size());
	// Ｚラベルリスト
	out_scn.header.z_label_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.z_label_cnt = (int)out_scn.z_label_list.size();
	m_pbsd->out_scn.push_array(out_scn.z_label_list.get(), (int)out_scn.z_label_list.size());
	// コマンドラベルリスト
	out_scn.header.cmd_label_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.cmd_label_cnt = (int)out_scn.cmd_label_list.size();
	m_pbsd->out_scn.push_array(out_scn.cmd_label_list.get(), (int)out_scn.cmd_label_list.size());
	// シーンプロパティリスト
	out_scn.header.scn_prop_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.scn_prop_cnt = (int)out_scn.scn_prop_list.size();
	m_pbsd->out_scn.push_array(out_scn.scn_prop_list.get(), (int)out_scn.scn_prop_list.size());
	// シーンプロパティ名インデックスリスト
	out_scn.header.scn_prop_name_index_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.scn_prop_name_index_cnt = (int)out_scn.scn_prop_name_index_list.size();
	m_pbsd->out_scn.push_array(out_scn.scn_prop_name_index_list.get(), (int)out_scn.scn_prop_name_index_list.size());
	// シーンプロパティ名リスト
	out_scn.header.scn_prop_name_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.scn_prop_name_cnt = (int)out_scn.scn_prop_name_list.size();
	for (int i = 0; i < (int)out_scn.scn_prop_name_list.size(); i++)
		m_pbsd->out_scn.push_str(out_scn.scn_prop_name_list[i]);
	// シーンコマンドリスト
	out_scn.header.scn_cmd_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.scn_cmd_cnt = (int)out_scn.scn_cmd_list.size();
	m_pbsd->out_scn.push_array(out_scn.scn_cmd_list.get(), (int)out_scn.scn_cmd_list.size());
	// シーンコマンド名インデックスリスト
	out_scn.header.scn_cmd_name_index_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.scn_cmd_name_index_cnt = (int)out_scn.scn_cmd_name_index_list.size();
	m_pbsd->out_scn.push_array(out_scn.scn_cmd_name_index_list.get(), (int)out_scn.scn_cmd_name_index_list.size());
	// シーンコマンド名リスト
	out_scn.header.scn_cmd_name_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.scn_cmd_name_cnt = (int)out_scn.scn_cmd_name_list.size();
	for (int i = 0; i < (int)out_scn.scn_cmd_name_list.size(); i++)
		m_pbsd->out_scn.push_str(out_scn.scn_cmd_name_list[i]);
	// コールプロパティ名インデックスリスト
	out_scn.header.call_prop_name_index_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.call_prop_name_index_cnt = (int)out_scn.call_prop_name_index_list.size();
	m_pbsd->out_scn.push_array(out_scn.call_prop_name_index_list.get(), (int)out_scn.call_prop_name_index_list.size());
	// コールプロパティ名リスト
	out_scn.header.call_prop_name_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.call_prop_name_cnt = (int)out_scn.call_prop_name_list.size();
	for (int i = 0; i < (int)out_scn.call_prop_name_list.size(); i++)
		m_pbsd->out_scn.push_str(out_scn.call_prop_name_list[i]);

	// 【名前】リスト
	out_scn.header.namae_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.namae_cnt = (int)out_scn.namae_list.size();
	m_pbsd->out_scn.push_array(out_scn.namae_list.get(), (int)out_scn.namae_list.size());
	// 既読フラグ情報
	out_scn.header.read_flag_list_ofs = (int)m_pbsd->out_scn.size();
	out_scn.header.read_flag_cnt = (int)out_scn.read_flag_list.size();
	m_pbsd->out_scn.push_array(out_scn.read_flag_list.get(), (int)out_scn.read_flag_list.size());
	// ヘッダを修正
	*(S_tnm_scn_header *)m_pbsd->out_scn.get() = out_scn.header;


	// ================================================================
	// シーンテキストデータを結合
	// ----------------------------------------------------------------
	m_pbsd->out_txt = out_txt;

	return true;
}

// ****************************************************************
// バイナリセーバー：<ss>
// ================================================================
bool C_tnms_binary_saver::bs_ss(BSP<TNMS_SA_NODE_SS> ss)
{
	// <文*>
	for (int s = 0; s < (int)ss->sentense_list.size(); s++)	{
		if (!bs_sentence(ss->sentense_list[s]))
			return false;
	}

	return true;
}

// ****************************************************************
// バイナリセーバー：<ブロック>
// ================================================================
bool C_tnms_binary_saver::bs_block(BSP<TNMS_SA_NODE_BLOCK> block)
{
	// <文*>
	for (int s = 0; s < (int)block->sentense_list.size(); s++)	{
		if (!bs_sentence(block->sentense_list[s]))
			return false;
	}

	return true;
}

// ****************************************************************
// バイナリセーバー：<文>
// ================================================================
bool C_tnms_binary_saver::bs_sentence(BSP<TNMS_SA_NODE_SENTENCE> sentence)
{
	// 行番号
	out_scn.scn.push(CD_NL);
	out_scn.scn.push(sentence->node_line);
	add_out_txt(_T("CD_NL: ") + tostr(sentence->node_line));

	// 選択肢文開始
	if (sentence->is_include_sel)	{
		out_scn.scn.push(CD_SEL_BLOCK_START);
	}

	// 本体
	if (!bs_sentence_sub(sentence))	{
		return false;
	}

	// 選択肢文終了
	if (sentence->is_include_sel)	{
		out_scn.scn.push(CD_SEL_BLOCK_END);
	}

	return true;
}
bool C_tnms_binary_saver::bs_sentence_sub(BSP<TNMS_SA_NODE_SENTENCE> sentence)
{
	// 文
	switch (sentence->node_type)	{
		case TNMS_SA_NODE_TYPE_SENTENCE_LABEL:
			return bs_label(sentence->label);
		case TNMS_SA_NODE_TYPE_SENTENCE_Z_LABEL:
			return bs_z_label(sentence->z_label);
		case TNMS_SA_NODE_TYPE_SENTENCE_DEF_PROP:
			return bs_def_prop(sentence->def_prop);
		case TNMS_SA_NODE_TYPE_SENTENCE_DEF_CMD:
			return bs_def_cmd(sentence->def_cmd);
		case TNMS_SA_NODE_TYPE_SENTENCE_GOTO:
			return bs_goto(sentence->Goto);
		case TNMS_SA_NODE_TYPE_SENTENCE_RETURN:
			return bs_return(sentence->Return);
		case TNMS_SA_NODE_TYPE_SENTENCE_IF:
			return bs_if(sentence->If);
		case TNMS_SA_NODE_TYPE_SENTENCE_FOR:
			return bs_for(sentence->For);
		case TNMS_SA_NODE_TYPE_SENTENCE_WHILE:
			return bs_while(sentence->While);
		case TNMS_SA_NODE_TYPE_SENTENCE_CONTINUE:
			return bs_continue(sentence->Continue);
		case TNMS_SA_NODE_TYPE_SENTENCE_BREAK:
			return bs_break(sentence->Break);
		case TNMS_SA_NODE_TYPE_SENTENCE_SWITCH:
			return bs_switch(sentence->Switch);
		case TNMS_SA_NODE_TYPE_SENTENCE_ASSIGN:
			return bs_assign(sentence->assign);
		case TNMS_SA_NODE_TYPE_SENTENCE_COMMAND:
			return bs_command(sentence->command);
		case TNMS_SA_NODE_TYPE_SENTENCE_TEXT:
			return bs_text(sentence->text);
		case TNMS_SA_NODE_TYPE_SENTENCE_NAME:
			return bs_name(sentence->name);
		case TNMS_SA_NODE_TYPE_SENTENCE_EOF:
			return bs_eof(sentence->eof);
	}

	return false;
}

// ****************************************************************
// バイナリセーバー：<ラベル>
// ================================================================
bool C_tnms_binary_saver::bs_label(BSP<TNMS_SA_NODE_LABEL> label)
{
	out_scn.label_list[label->label->atom.opt] = (int)out_scn.scn.size();
	add_out_txt(_T("#label, ") + tostr(label->label->atom.opt));

	return true;
}

// ****************************************************************
// バイナリセーバー：<Ｚラベル>
// ================================================================
bool C_tnms_binary_saver::bs_z_label(BSP<TNMS_SA_NODE_Z_LABEL> z_label)
{
	// Ｚラベルはラベルとしても登録されている！
	out_scn.label_list[z_label->z_label->atom.subopt] = (int)out_scn.scn.size();	// ラベル番号は subopt を使う
	out_scn.z_label_list[z_label->z_label->atom.opt] = (int)out_scn.scn.size();		// Ｚラベル番号は opt を使う
	add_out_txt(_T("#z-label, ") + tostr(z_label->z_label->atom.opt));

	return true;
}

// ****************************************************************
// バイナリセーバー：<プロパティ定義文>
// ================================================================
bool C_tnms_binary_saver::bs_def_prop(BSP<TNMS_SA_NODE_DEF_PROP> def_prop)
{
	// 添え字
	if (def_prop->form_code == FM_INTLIST || def_prop->form_code == FM_STRLIST)	{

		// 添え字があれば添え字
		if (def_prop->form->index)	{
			if (!bs_exp(def_prop->form->index, true))	// 値を要求する
				return false;
		}
		// 添え字がなければ 0
		else	{
			out_scn.scn.push(CD_PUSH);
			out_scn.scn.push(FM_INT);
			out_scn.scn.push(0);
		}
	}

	out_scn.scn.push(CD_DEC_PROP);
	out_scn.scn.push(def_prop->form_code);	// 型
	out_scn.scn.push(def_prop->prop_id);	// プロパティＩＤ

	// コールプロパティ名
//	out_dbg.call_prop_name_list


	return true;
}

// ****************************************************************
// バイナリセーバー：<コマンド定義文>
// ================================================================
bool C_tnms_binary_saver::bs_def_cmd(BSP<TNMS_SA_NODE_DEF_CMD> def_cmd)
{
	// ラベルの仮作成
	int label_no_end = (int)out_scn.label_list.size();
	out_scn.label_list.push_back(0);

	// コマンド外からここへ突っ込んだ場合はコマンド後へジャンプする
	out_scn.scn.push(CD_GOTO);
	out_scn.scn.push(label_no_end);

	// コマンドラベルの設定（リンカが inc コマンドを組み立てるときに必要になります）
	S_tnm_scn_cmd_label cmd_label;
	cmd_label.cmd_id = def_cmd->cmd_id;
	cmd_label.offset = (int)out_scn.scn.size();
	out_scn.cmd_label_list.push_back(cmd_label);

	// <プロパティ宣言>
	for (int p = 0; p < (int)def_cmd->prop_list.size(); p++)	{
		if (!bs_def_prop(def_cmd->prop_list[p]))
			return false;
	}

	// <引数の展開>
	out_scn.scn.push(CD_ARG);

	// <ブロック>
	if (!bs_block(def_cmd->block))
		return false;

	// ダミーの return 文を置いておく
	// return 文がなかった場合はこの return 文によってコマンドから抜ける
	out_scn.scn.push(CD_RETURN);
	out_scn.scn.push(0);	// 引数の個数 = 0
	add_out_txt(_T("CD_RETURN"));

	// ラベルの設定
	out_scn.label_list[label_no_end] = (int)out_scn.scn.size();

	// シーンコマンドリスト
	if (def_cmd->cmd_id >= m_piad->inc_command_cnt)
		out_scn.scn_cmd_list[def_cmd->cmd_id - m_piad->inc_command_cnt].offset = cmd_label.offset;

	return true;
}

// ****************************************************************
// バイナリセーバー：<goto 文／gosub 文>
// ================================================================
bool C_tnms_binary_saver::bs_goto(BSP<TNMS_SA_NODE_GOTO> Goto)
{
	// goto 文
	if (Goto->node_type == TNMS_SA_NODE_TYPE_GOTO_GOTO)	{

		// goto ラベル
		if (Goto->node_sub_type == TNMS_SA_NODE_TYPE_GOTO_LABEL)	{
			out_scn.scn.push(CD_GOTO);
			out_scn.scn.push(Goto->label->atom.opt);		// opt を使う
			add_out_txt(_T("CD_GOTO: ") + tostr(Goto->label->atom.opt));
		}
		// goto Ｚラベル
		else	{
			out_scn.scn.push(CD_GOTO);
			out_scn.scn.push(Goto->z_label->atom.subopt);	// subopt を使う
			add_out_txt(_T("CD_GOTO: ") + tostr(Goto->z_label->atom.subopt));
		}
	}
	// gosub 文
	else {
		// gosub 式
		if (!bs_goto_exp(Goto))
			return false;

		// 戻り値を捨てる
		if (Goto->node_type == TNMS_SA_NODE_TYPE_GOTO_GOSUB)	{
			out_scn.scn.push(CD_POP);
			out_scn.scn.push(FM_INT);
			add_out_txt(_T("CD_POP, ") + tostr_form(FM_INT));
		}
		else	{
			out_scn.scn.push(CD_POP);
			out_scn.scn.push(FM_STR);
			add_out_txt(_T("CD_POP, ") + tostr_form(FM_STR));
		}
	}

	return true;
}

// ****************************************************************
// バイナリセーバー：<goto／gosub 式>
// ================================================================
bool C_tnms_binary_saver::bs_goto_exp(BSP<TNMS_SA_NODE_GOTO> Goto)
{
	// 引数
	if (!bs_arg_list(Goto->arg_list, true))		// true: 値を要求
		return false;

	// gosub / gosubstr
	if (Goto->node_type == TNMS_SA_NODE_TYPE_GOTO_GOSUB)	{

		// ラベル番号を取得（ラベルの場合は opt、Ｚラベルの場合は subopt に入っている）
		int label_no = Goto->node_sub_type == TNMS_SA_NODE_TYPE_GOTO_LABEL ? Goto->label->atom.opt : Goto->z_label->atom.subopt;

		out_scn.scn.push(CD_GOSUB);
		out_scn.scn.push(label_no);		// ラベル番号

		// 引数の数と型を並べておく
		out_scn.scn.push((int)Goto->arg_list->arg.size());
		for (int i = 0; i < (int)Goto->arg_list->arg.size(); i++)	{
			int form = dereference(Goto->arg_list->arg[i]->exp->tmp_form);
			out_scn.scn.push(form);
		}

		add_out_txt(_T("CD_GOSUB, ") + tostr(label_no) + _T(", ") + tostr((int)Goto->arg_list->arg.size()));
	}
	else	{

		// ラベル番号を取得（ラベルの場合は opt、Ｚラベルの場合は subopt に入っている）
		int label_no = Goto->node_sub_type == TNMS_SA_NODE_TYPE_GOTO_LABEL ? Goto->label->atom.opt : Goto->z_label->atom.subopt;

		out_scn.scn.push(CD_GOSUBSTR);
		out_scn.scn.push(label_no);		// ラベル番号

		// 引数の数と型を並べておく
		out_scn.scn.push((int)Goto->arg_list->arg.size());
		for (int i = 0; i < (int)Goto->arg_list->arg.size(); i++)	{
			int form = dereference(Goto->arg_list->arg[i]->exp->tmp_form);
			out_scn.scn.push(form);
		}

		add_out_txt(_T("CD_GOSUBSTR, ") + tostr(label_no) + _T(", ") + tostr((int)Goto->arg_list->arg.size()));
	}


	return true;
}

// ****************************************************************
// バイナリセーバー：<return 文>
// ================================================================
bool C_tnms_binary_saver::bs_return(BSP<TNMS_SA_NODE_RETURN> Return)
{
	// 引数ありの場合
	if (Return->node_type == TNMS_SA_NODE_TYPE_RETURN_WITH_ARG)	{
		if (!bs_exp(Return->exp, true))		// 値を要求する
			return false;
	}

	// return
	out_scn.scn.push(CD_RETURN);
	add_out_txt(_T("CD_RETURN"));

	// 引数の数と型を並べておく（ラベル番号の分を忘れずに）
	if (Return->node_type == TNMS_SA_NODE_TYPE_RETURN_WITH_ARG)	{
		int form = dereference(Return->exp->node_form);
		out_scn.scn.push(1);	// 引数１個
		out_scn.scn.push(form);
	}
	else	{
		out_scn.scn.push(0);	// 引数０個
	}

	return true;
}

// ****************************************************************
// バイナリセーバー：<if 文>
// ================================================================
bool C_tnms_binary_saver::bs_if(BSP<TNMS_SA_NODE_IF> If)
{
	// エンドラベルの仮作成
	int label_no_end = (int)out_scn.label_list.size();
	out_scn.label_list.push_back(0);

	int if_cnt = (int)If->sub.size();
	for (int i = 0; i < if_cnt; i++)	{

		// "if" | "elseif"
		if (If->sub[i].If->atom.type == TNMS_LA_ATOM_IF || If->sub[i].If->atom.type == TNMS_LA_ATOM_ELSEIF)	{
			// 条件ラベルの仮作成
			int label_no_if = (int)out_scn.label_list.size();
			out_scn.label_list.push_back(0);
			// <条件式>
			if (!bs_exp(If->sub[i].cond, true))		// 値を要求する
				return false;
			// 条件式が偽なら条件ラベルにジャンプ
			out_scn.scn.push(CD_GOTO_FALSE);
			out_scn.scn.push(label_no_if);
			// <ブロック>
			for (int s = 0; s < (int)If->sub[i].block.size(); s++)	{
				if (!bs_sentence(If->sub[i].block[s]))
					return false;
			}
			// エンドラベルにジャンプ
			out_scn.scn.push(CD_GOTO);
			out_scn.scn.push(label_no_end);
			// 条件ラベルの設定
			out_scn.label_list[label_no_if] = (int)out_scn.scn.size();
		}
		// "else"
		else	{
			// <ブロック>
			for (int s = 0; s < (int)If->sub[i].block.size(); s++)	{
				if (!bs_sentence(If->sub[i].block[s]))
					return false;
			}
		}
	}

	// エンドラベルの設定
	out_scn.label_list[label_no_end] = (int)out_scn.scn.size();

	return true;
}

// ****************************************************************
// バイナリセーバー：<for 文>
// ================================================================
bool C_tnms_binary_saver::bs_for(BSP<TNMS_SA_NODE_FOR> For)
{
	// ３つのラベルを仮作成
	int label_size = (int)out_scn.label_list.size();
	int label_no_init = label_size;			// [初期化ラベル]
	int label_no_loop = label_size + 1;		// [ループラベル]
	int label_no_out = label_size + 2;		// [脱出ラベル]
	out_scn.label_list.push_back(0);
	out_scn.label_list.push_back(0);
	out_scn.label_list.push_back(0);

	// ループラベルの追加
	LOOP loop = {label_no_loop, label_no_out};
	loop_label.push_back(loop);

	// <初期化文>
	for (int s = 0; s < (int)For->init.size(); s++)	{
		if (!bs_sentence(For->init[s]))
			return false;
	}
	// 初期化ラベルにジャンプ
	out_scn.scn.push(CD_GOTO);
	out_scn.scn.push(label_no_init);
	// ラベルの設定 [ループラベル]
	out_scn.label_list[label_no_loop] = (int)out_scn.scn.size();
	// <ループ文>
	for (int s = 0; s < (int)For->loop.size(); s++)	{
		if (!bs_sentence(For->loop[s]))
			return false;
	}
	// ラベルの設定 [初期化ラベル]
	out_scn.label_list[label_no_init] = (int)out_scn.scn.size();
	// <条件式>
	if (!bs_exp(For->cond, true))		// 値を要求する
		return false;
	// 条件式が偽なら脱出ラベルにジャンプ
	out_scn.scn.push(CD_GOTO_FALSE);
	out_scn.scn.push(label_no_out);
	// <ブロック>
	for (int s = 0; s < (int)For->block.size(); s++)	{
		if (!bs_sentence(For->block[s]))
			return false;
	}
	// ループラベルにジャンプ
	out_scn.scn.push(CD_GOTO);
	out_scn.scn.push(label_no_loop);
	// ラベルの設定 [脱出ラベル]
	out_scn.label_list[label_no_out] = (int)out_scn.scn.size();

	// ループラベルの削除
	loop_label.pop_back();

	return true;
}

// ****************************************************************
// バイナリセーバー：<while 文>
// ================================================================
bool C_tnms_binary_saver::bs_while(BSP<TNMS_SA_NODE_WHILE> While)
{
	// ２つのラベルを仮作成
	int label_size = (int)out_scn.label_list.size();
	int label_no_loop = label_size;		// [ループラベル]
	int label_no_out = label_size + 1;	// [脱出ラベル]
	out_scn.label_list.push_back(0);
	out_scn.label_list.push_back(0);

	// ループラベルの追加
	LOOP loop = {label_no_loop, label_no_out};
	loop_label.push_back(loop);

	// ラベルの設定 [ループラベル]
	out_scn.label_list[label_no_loop] = (int)out_scn.scn.size();
	// <条件式>
	if (!bs_exp(While->cond, true))		// 値を要求する
		return false;
	// 条件式が偽なら脱出ラベルにジャンプ
	out_scn.scn.push(CD_GOTO_FALSE);
	out_scn.scn.push(label_no_out);
	// <ブロック>
	for (int s = 0; s < (int)While->block.size(); s++)	{
		if (!bs_sentence(While->block[s]))
			return false;
	}
	// ループラベルにジャンプ
	out_scn.scn.push(CD_GOTO);
	out_scn.scn.push(label_no_loop);
	// ラベルの設定 [脱出ラベル]
	out_scn.label_list[label_no_out] = (int)out_scn.scn.size();

	// ループラベルの削除
	loop_label.pop_back();

	return true;
}

// ****************************************************************
// バイナリセーバー：<continue 文>
// ================================================================
bool C_tnms_binary_saver::bs_continue(BSP<TNMS_SA_NODE_CONTINUE> Continue)
{
	if (loop_label.empty())
		return error(TNMSERR_BS_CONTINUE_NO_LOOP, Continue->Continue->atom);

	// ラベルを取り出す
	int label_no = loop_label.back().Continue;

	out_scn.scn.push(CD_GOTO);
	out_scn.scn.push(label_no);

	return true;
}

// ****************************************************************
// バイナリセーバー：<break 文>
// ================================================================
bool C_tnms_binary_saver::bs_break(BSP<TNMS_SA_NODE_BREAK> Break)
{
	if (loop_label.empty())
		return error(TNMSERR_BS_BREAK_NO_LOOP, Break->Break->atom);

	// ラベルを取り出す
	int label_no = loop_label.back().Break;

	out_scn.scn.push(CD_GOTO);
	out_scn.scn.push(label_no);

	return true;
}

// ****************************************************************
// バイナリセーバー：<switch 文>
// ================================================================
bool C_tnms_binary_saver::bs_switch(BSP<TNMS_SA_NODE_SWITCH> Switch)
{
	// 参照外し
	int form_l = dereference(Switch->cond->node_form);

	// ラベルを仮作成
	int label_size = (int)out_scn.label_list.size();
	int label_no_out = label_size;										// [脱出ラベル]
	int label_no_case = label_size + 1;									// [caseラベル]
	int label_no_default = label_size + 1 + (int)Switch->Case.size();	// [defaultラベル]
	for (int i = 0; i < (int)Switch->Case.size() + 1; i++)	{
		out_scn.label_list.push_back(0);
	}
	if (Switch->Default)	{
		out_scn.label_list.push_back(0);
	}

	// <条件式>
	if (!bs_exp(Switch->cond, true))		// 値を要求する
		return false;
	// <case 文*>
	for (int c = 0; c < (int)Switch->Case.size(); c++)	{

		// 参照外し
		int form_r = dereference(Switch->Case[c]->value->node_form);

		// 条件式をコピー
		out_scn.scn.push(CD_COPY);
		out_scn.scn.push(form_l);
		// <比較値>
		if (!bs_exp(Switch->Case[c]->value, true))		// 値を要求する
			return false;

		// 比較
		out_scn.scn.push(CD_OPERATE_2);			// 二項演算
		out_scn.scn.push(form_l);				// 左式の型
		out_scn.scn.push(form_r);				// 右式の型
		out_scn.scn.push(OP_EQUAL);				// 等号
		// 条件式が真ならcaseラベルにジャンプ
		out_scn.scn.push(CD_GOTO_TRUE);
		out_scn.scn.push(label_no_case + c);
	}
	// 条件式を破棄
	out_scn.scn.push(CD_POP);
	out_scn.scn.push(form_l);
	// 脱出ラベルまたはデフォルトラベルにジャンプ
	out_scn.scn.push(CD_GOTO);
	out_scn.scn.push(Switch->Default ? label_no_default : label_no_out);

	// <case 文*>
	for (int c = 0; c < (int)Switch->Case.size(); c++)	{
		// ラベルを設置 [caseラベル]
		out_scn.label_list[label_no_case + c] = (int)out_scn.scn.size();
		// 条件式を破棄
		out_scn.scn.push(CD_POP);
		out_scn.scn.push(form_l);
		// <ブロック>
		for (int s = 0; s < (int)Switch->Case[c]->block.size(); s++)	{
			if (!bs_sentence(Switch->Case[c]->block[s]))
				return false;
		}
		// 脱出ラベルにジャンプ
		out_scn.scn.push(CD_GOTO);
		out_scn.scn.push(label_no_out);
	}
	// <default 文>
	if (Switch->Default)	{
		// ラベルを設置 [defaultラベル]
		out_scn.label_list[label_no_default] = (int)out_scn.scn.size();
		// <ブロック>
		for (int s = 0; s < (int)Switch->Default->block.size(); s++)	{
			if (!bs_sentence(Switch->Default->block[s]))
				return false;
		}
		// 脱出ラベルにジャンプ
		out_scn.scn.push(CD_GOTO);
		out_scn.scn.push(label_no_out);
	}
	// ラベルを設置 [脱出ラベル]
	out_scn.label_list[label_no_out] = (int)out_scn.scn.size();

	return true;
}

// ****************************************************************
// バイナリセーバー：<代入文>
// ================================================================
// ★型についての注意
//		a = b	→	assign->left->form = (a), assign->right->form = (b)
//		a += b	→	assign->left->form = (a), assign->right->form = (b), assign->equal_form = (a + b)
// ================================================================
bool C_tnms_binary_saver::bs_assign(BSP<TNMS_SA_NODE_ASSIGN> assign)
{
	// 左辺
	if (!bs_left(assign->left))
		return false;

#if 0
	// __set コマンドを使う場合はエレメントとして積んでおく
	if (assign->set_flag)	{
		out_scn.scn.push(CD_PUSH);
		out_scn.scn.push(FM_INT);
		out_scn.scn.push(ELM___SET);
	}
#endif

	// += など、演算が必要な場合、
	// 左辺のエレメントリストをコピーしてプロパティ展開をしておく
	if (assign->equal->atom.opt != OP_NONE)	{
		out_scn.scn.push(CD_COPY_ELM);
		out_scn.scn.push(CD_PROPERTY);
		add_out_txt(_T("CD_COPY_EL"));
		add_out_txt(_T("CD_PROPERTY"));
	}

	// 右辺
	if (!bs_exp(assign->right, assign->set_flag ? false : true))		// 値を要求する
		return false;

	// += などの演算を行う
	int form_l = dereference(assign->left->node_form);
	int form_r = dereference(assign->right->node_form);
	if (assign->equal->atom.opt != OP_NONE)	{
		out_scn.scn.push(CD_OPERATE_2);			// 二項演算
		out_scn.scn.push(form_l);				// 左式の型
		out_scn.scn.push(form_r);				// 右式の型
		add_out_txt(_T("CD_OPERATE_2, ") + tostr_form(form_l) + _T(", ") + tostr_form(form_r));
		bs_assign_operator(assign->equal);		// 演算
	}

	// 代入
	int form_r_2 = dereference(assign->equal_form);
	out_scn.scn.push(CD_ASSIGN);
	out_scn.scn.push(assign->left->node_form);	// 左式の型は参照型のまま
	out_scn.scn.push(form_r_2);					// 右式の型は計算結果の型を使う
	out_scn.scn.push(assign->al_id);			// 右式の型は計算結果の型を使う
	add_out_txt(_T("CD_ASSIGN, ") + tostr_form(assign->left->node_form) + _T(", ") + tostr_form(assign->node_form));

	return true;
}

// ****************************************************************
// バイナリセーバー：<コマンド文>
// ================================================================
bool C_tnms_binary_saver::bs_command(BSP<TNMS_SA_NODE_COMMAND> command)
{
	// コマンド
	if (!bs_elm_exp(command->command, true))	// 値を要求する
		return false;

	// 戻り値を捨てる
	out_scn.scn.push(CD_POP);
	out_scn.scn.push(command->command->node_form);
	add_out_txt(_T("CD_POP, ") + tostr_form(command->command->node_form));

	return true;
}

// ****************************************************************
// バイナリセーバー：<テキスト文>
// ================================================================
bool C_tnms_binary_saver::bs_text(BSP<TNMS_SA_NODE_ATOM> text)
{
	// ★メッセージブロックを割り当てる特殊なコマンド
	bs_push_msg_block();

	// 引数（文字列ＩＤ）
	out_scn.scn.push(CD_PUSH);
	out_scn.scn.push(FM_STR);
	out_scn.scn.push(text->atom.opt);

	// テキストコマンド
	out_scn.scn.push(CD_TEXT);

	// テキスト文に既読フラグを割り当てる
	out_scn.scn.push(cur_read_flag_no ++);

	// 既読フラグ情報
	S_tnm_scn_read_flag read_flag;
	read_flag.line_no = text->atom.line;
	out_scn.read_flag_list.push_back(read_flag);

	// テキスト
	add_out_txt(_T("CD_TEXT"));

	return true;
}

// ****************************************************************
// バイナリセーバー：<式>
// ================================================================
bool C_tnms_binary_saver::bs_exp(BSP<TNMS_SA_NODE_EXP> exp, bool need_value)
{
	if (false);
	else if (exp->node_type == TNMS_SA_NODE_TYPE_EXP_SIMPLE)	{	// 単項式
		return bs_smp_exp(exp->smp_exp, need_value);
	}
	else if (exp->node_type == TNMS_SA_NODE_TYPE_EXP_OPR_1)	{		// 単項演算

		// 参照は演算できない
		if (!need_value)
			return error(TNMSERR_BS_NEED_REFERENCE, *exp->get_first_atom());

		if (!bs_exp(exp->exp_1, true))			// 左式、値を要求する
			return false;

		int form = dereference(exp->exp_1->node_form);		// 参照外し
		out_scn.scn.push(CD_OPERATE_1);						// 二項演算
		out_scn.scn.push(form);								// 左式の型
		add_out_txt(_T("CD_OPERATE_1"));
		if (!bs_operator_1(exp->opr))						// 演算子タイプ
			return false;
	}
	else if (exp->node_type == TNMS_SA_NODE_TYPE_EXP_OPR_2)	{		// 二項演算

		// 参照は演算できない
		if (!need_value)
			return error(TNMSERR_BS_NEED_REFERENCE, *exp->get_first_atom());

		if (!bs_exp(exp->exp_1, true))				// 左式、値を要求する
			return false;
		if (!bs_exp(exp->exp_2, true))				// 右式、値を要求する
			return false;

		int form_l = dereference(exp->exp_1->node_form);	// 参照外し
		int form_r = dereference(exp->exp_1->node_form);	// 参照外し
		out_scn.scn.push(CD_OPERATE_2);						// 二項演算
		out_scn.scn.push(form_l);							// 左式の型
		out_scn.scn.push(form_r);							// 右式の型
		add_out_txt(_T("CD_OPERATE_2"));
		if (!bs_operator_2(exp->opr))						// 演算子タイプ
			return false;
	}

	// 実際の型と要求されている型が違う場合は変換コマンドを実行
	// ただし、FM_INTREF → FM_INT や FM_STRREF → FM_STR は参照外しの仕事。もう終わってるのでここでは行わない
	// １箇所にまとめるべき！！！
#if 0
	if (exp->node_form != exp->tmp_form)	{
		if (!((exp->node_form == FM_INTREF && exp->tmp_form == FM_INT) || (exp->node_form == FM_STRREF && exp->tmp_form == FM_STR)))	{

			out_scn.scn.push(CD_PUSH);
			out_scn.scn.push(FM_INT);
			out_scn.scn.push(ELM___TRANS);	// 変換
			out_scn.scn.push(CD_COMMAND);	// 変換コマンドを実行
			out_scn.scn.push(0);			// 引数リスト番号
			out_scn.scn.push(0);			// 実引数の個数
			out_scn.scn.push(0);			// 名前つき引数の個数
			out_scn.scn.push(FM_VOID);		// 戻り値の型
		}
	}
#endif

	return true;
}

// ****************************************************************
// バイナリセーバー：<式リスト>
// ================================================================
//		[式の個数]	: int
//		[式]		: すべての式
// ================================================================
bool C_tnms_binary_saver::bs_exp_list(BSP<TNMS_SA_NODE_EXP_LIST> exp_list)
{
	int exp_cnt = (int)exp_list->exp.size();
	for (int e = 0; e < exp_cnt; e ++)	{
		if (!bs_exp(exp_list->exp[e], true))	// 値を要求する
			return false;
	}

	return true;
}

// ****************************************************************
// バイナリセーバー：<単項式>
// ================================================================
bool C_tnms_binary_saver::bs_smp_exp(BSP<TNMS_SA_NODE_SMP_EXP> smp_exp, bool need_value)
{
	if (false);
	// '(' <式> ')'
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_KAKKO)	{
		return bs_exp(smp_exp->exp, need_value);
	}
	// <式リスト>
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_EXP_LIST)	{

		// 式リストは参照になれない
		if (!need_value)
			return error(TNMSERR_BS_NEED_REFERENCE, *smp_exp->get_first_atom());

		return bs_exp_list(smp_exp->exp_list);
	}
	// <goto 式>
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_GOTO)	{

		// goto は参照になれない
		if (!need_value)
			return error(TNMSERR_BS_NEED_REFERENCE, *smp_exp->get_first_atom());

		return bs_goto_exp(smp_exp->Goto);
	}
	// <エレメント式>
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_ELM_EXP)	{
		return bs_elm_exp(smp_exp->elm_exp, need_value);
	}
	// <リテラル>
	else if (smp_exp->node_type == TNMS_SA_NODE_TYPE_SMP_EXP_LITERAL)	{

		// リテラルは参照になれない
		if (!need_value)
			return error(TNMSERR_BS_NEED_REFERENCE, *smp_exp->get_first_atom());

		return bs_literal(smp_exp->Literal);
	}

	return false;
}

// ****************************************************************
// バイナリセーバー：<左辺>
// ================================================================
bool C_tnms_binary_saver::bs_left(BSP<TNMS_SA_NODE_ELM_EXP> left)
{
	// エレメント式を解析
	if (!bs_elm_list(left->elm_list))	// 左辺は値を要求しない
		return false;

	return true;
}

// ****************************************************************
// バイナリセーバー：<エレメント式>
// ================================================================
//	コマンドの場合
//		<エレメントリスト>
//	プロパティの場合
//		<エレメントリスト>
//		CD_PROPERTY
// ================================================================
bool C_tnms_binary_saver::bs_elm_exp(BSP<TNMS_SA_NODE_ELM_EXP> elm_exp, bool need_value)
{
	// エレメントリストは、
	// ・コマンドの場合：末尾のコマンドエレメントにより、直値に展開されています。
	// ・プロパティの場合：直値が求められている場合は、ここで直値に展開します。

	// 本当は、コマンドもエレメントで返すこともできるようにした方が望ましいです。
	// その場合はリファレンスの概念をしっかり組まないといけません。

	if (false);

	// コマンドの場合
	else if (elm_exp->element_type == TNMS_ELEMENT_TYPE_COMMAND)	{

		// コマンドは参照にはなれない
//		if (!need_value)
//			return error(TNMSERR_BS_NEED_REFERENCE, *elm_exp->elm_list->get_last_atom());

		// コマンドのコードを取得しておく
		int parent_form_code = elm_exp->elm_list->element.back()->element_parent_form;
		int element_code = elm_exp->elm_list->element.back()->element_code;

		// ★メッセージブロックを割り当てる特殊なコマンド
		if (
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_KOE) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SET_FACE) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SET_NAMAE) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_PRINT) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_RUBY) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_NL) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_NLI) ||
			//(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SEL) || 
			//(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SEL_CANCEL) || 
			//(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SELBTN) || 
			//(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SELBTN_CANCEL) || 
			(parent_form_code == FM_MWND && element_code == ELM_MWND_KOE) ||
			(parent_form_code == FM_MWND && element_code == ELM_MWND_SET_FACE) ||
			(parent_form_code == FM_MWND && element_code == ELM_MWND_SET_NAMAE) ||
			(parent_form_code == FM_MWND && element_code == ELM_MWND_PRINT) || 
			(parent_form_code == FM_MWND && element_code == ELM_MWND_RUBY) ||
			(parent_form_code == FM_MWND && element_code == ELM_MWND_NL) || 
			(parent_form_code == FM_MWND && element_code == ELM_MWND_NLI) || 
			//(parent_form_code == FM_MWND && element_code == ELM_MWND_SEL) ||
			//(parent_form_code == FM_MWND && element_code == ELM_MWND_SEL_CANCEL) ||
			false
			)
		{
			bs_push_msg_block();
		}

		// エレメントリストを解析
		if (!bs_elm_list(elm_exp->elm_list))
			return false;

		// ★既読フラグを割り当てる特殊なコマンド
		if (
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_PRINT) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_KOE) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_KOE_PLAY_WAIT) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_KOE_PLAY_WAIT_KEY) ||
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SEL) || 
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SEL_CANCEL) || 
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SELMSG) || 
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SELMSG_CANCEL) || 
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SELBTN) || 
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SELBTN_CANCEL) || 
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SELBTN_START) ||	// ready ではなく start に割り当てる
			(parent_form_code == FM_GLOBAL && element_code == ELM_GLOBAL_SEL_IMAGE) ||		// kinetic 専用
			(parent_form_code == FM_MWND && element_code == ELM_MWND_PRINT) || 
			(parent_form_code == FM_MWND && element_code == ELM_MWND_KOE) || 
			(parent_form_code == FM_MWND && element_code == ELM_MWND_KOE_PLAY_WAIT) || 
			(parent_form_code == FM_MWND && element_code == ELM_MWND_KOE_PLAY_WAIT_KEY) || 
			(parent_form_code == FM_MWND && element_code == ELM_MWND_SEL) ||
			(parent_form_code == FM_MWND && element_code == ELM_MWND_SEL_CANCEL) ||
			(parent_form_code == FM_MWND && element_code == ELM_MWND_SELMSG) ||
			(parent_form_code == FM_MWND && element_code == ELM_MWND_SELMSG_CANCEL) ||
			false
			)
		{
			out_scn.scn.push(cur_read_flag_no);
			add_out_txt(_T("\tread_flag_no = ") + tostr(cur_read_flag_no));
			cur_read_flag_no ++;

			// 既読フラグ情報
			S_tnm_scn_read_flag read_flag;
			read_flag.line_no = elm_exp->elm_list->element.back()->node_line;
			out_scn.read_flag_list.push_back(read_flag);
		}

		// 値を要求する場合は参照外し
		if (need_value)	{

			// 値なら何もしない
			if (is_value(elm_exp->node_form))	{}
			// 参照外しできるなら参照外し
			else if (elm_exp->node_form == FM_INTREF || elm_exp->node_form == FM_STRREF || elm_exp->node_form == FM_INTLISTREF || elm_exp->node_form == FM_STRLISTREF)	{
				out_scn.scn.push(CD_PROPERTY);
				add_out_txt(_T("CD_PROPERTY"));
			}
			// 参照外しできないならエラー
			else
				return error(TNMSERR_BS_NEED_VALUE, *elm_exp->elm_list->get_last_atom());
		}
	}

	// プロパティの場合
	else if (elm_exp->element_type == TNMS_ELEMENT_TYPE_PROPERTY)	{

		// エレメントリストを解析
		if (!bs_elm_list(elm_exp->elm_list))
			return false;

		// 値が必要な場合
		if (need_value)	{
			
			// 値なら何もしない
			if (is_value(elm_exp->node_form))	{}
			// 参照外しできるなら参照外し
			else if (elm_exp->node_form == FM_INTREF || elm_exp->node_form == FM_STRREF || elm_exp->node_form == FM_INTLISTREF || elm_exp->node_form == FM_STRLISTREF)	{
				out_scn.scn.push(CD_PROPERTY);
				add_out_txt(_T("CD_PROPERTY"));
			}
			// 参照外しできないならエラー
			else
				return error(TNMSERR_BS_NEED_VALUE, *elm_exp->elm_list->get_last_atom());
		}
	}

	return true;
}

// ****************************************************************
// バイナリセーバー：<エレメントリスト>
// ================================================================
bool C_tnms_binary_saver::bs_elm_list(BSP<TNMS_SA_NODE_ELM_LIST> elm_list)
{
	// エレメント開始ポイントを設定
	out_scn.scn.push(CD_ELM_POINT);
	add_out_txt(_T("CD_ELM_POINT"));

	// スコープを積む
	if (elm_list->parent_form_code == FM_CALL)	{
		out_scn.scn.push(CD_PUSH);
		out_scn.scn.push(FM_INT);
		out_scn.scn.push(ELM_GLOBAL_CUR_CALL);
		add_out_txt(_T("CD_PUSH, ") + tostr_form(FM_INT) + _T(", ") + tostr(ELM_GLOBAL_CUR_CALL));
	}

	// エレメントを順に積む
	int element_cnt = (int)elm_list->element.size();
	for (int i = 0; i < element_cnt; i++)	{
		if (!bs_element(elm_list->element[i]))
			return false;

		// 参照なら参照外し
		if (get_elm_owner(elm_list->element[i]->element_code) == ELM_OWNER_CALL_PROP)	{
			if (is_reference(elm_list->element[i]->node_form))	{
				out_scn.scn.push(CD_PROPERTY);
				add_out_txt(_T("CD_PROPERTY"));
			}
		}
	}

	return true;
}

// ****************************************************************
// バイナリセーバー：<エレメント>
// ================================================================
//	ex.	load("abc")
//		CD_PUSH		10			// load のエレメントコード
//		<引数リスト>			// 引数リスト
//		CD_COMMAND				// コマンドを実行
//		CD_PUSH		0			// 引数リスト番号
//		CD_PUSH		1			// 引数の個数
// ================================================================
bool C_tnms_binary_saver::bs_element(BSP<TNMS_SA_NODE_ELEMENT> element)
{
	if (false);

	// 通常のエレメントの場合
	else if (element->node_type == TNMS_SA_NODE_TYPE_ELEMENT_ELEMENT)	{

		// エレメントコードを積む
		out_scn.scn.push(CD_PUSH);
		out_scn.scn.push(FM_INT);
		out_scn.scn.push(element->element_code);
		add_out_txt(_T("CD_PUSH, ") + tostr_form(FM_INT) + _T(", ") + tostr(element->element_code));

		// コマンドの場合
		if (element->element_type == TNMS_ELEMENT_TYPE_COMMAND)	{

			int arg_cnt = (int)element->arg_list->arg.size();		// 実引数の個数

			// 引数リスト
			if (!bs_arg_list(element->arg_list, false))		// false: 値を要求しない
				return false;

			// 省略された引数をここで展開しておく

			// コマンドを取得
			S_tnms_form* form_info = m_piad->form_table.get_form_by_code(element->element_parent_form);
			S_tnms_element* element_info = form_info->get_element_by_code(element->element_code);
			ARRAY<S_tnms_arg>* temp_arg_list = &element_info->arg_map[element->arg_list_id].arg_list;
			if (element->arg_list->arg.size() < temp_arg_list->size())	{	// 実引数が仮引数より少ない場合
				for (ARRAY<S_tnms_arg>::iterator temp_arg = temp_arg_list->begin() + element->arg_list->arg.size(); temp_arg != temp_arg_list->end(); ++temp_arg)	{

					// "*" なら終了
					if (temp_arg->form == FM___ARGS || temp_arg->form == FM___ARGSREF)
						break;

					// デフォルト引数を入れる
					out_scn.scn.push(CD_PUSH);
					out_scn.scn.push(temp_arg->form);
					switch (temp_arg->form)	{		// デフォルト値を入れる
						case FM_INT:	out_scn.scn.push(temp_arg->def_int);	break;
						//case FM_STR:	out_scn.scn.push(temp_arg->def_str);	break;
						default:		
							return error(TNMSERR_BS_ILLEGAL_DEFAULT_ARG, element->name->atom);
					}
					arg_cnt ++;		// デフォルト引数を実引数として積んだのでカウントを増やす
				}
			}

			// コマンド
			out_scn.scn.push(CD_COMMAND);
			out_scn.scn.push(element->arg_list_id);		// 引数リスト番号
			out_scn.scn.push(arg_cnt);					// 実引数の個数

			// 引数の型を並べておく（可変長引数などの対策）
			// 逆順に並べる！（引数はスタックに逆順に積まれるため）
			TSTR arg_form_text;

			// まずは省略された引数
			if (element->arg_list->arg.size() < temp_arg_list->size())	{	// 実引数が仮引数より少ない場合
				for (S_tnms_arg* temp_arg = temp_arg_list->get_end() - 1; temp_arg >= temp_arg_list->get() + element->arg_list->arg.size(); --temp_arg)	{

					// "*" なら終了
					if (temp_arg->form == FM___ARGS || temp_arg->form == FM___ARGSREF)
						break;

					out_scn.scn.push(temp_arg->form);
					arg_form_text += tostr_form(temp_arg->form) + _T(", ");
				}
			}

			// 次に実際に与えられた実引数の変換後の型
			for (int i = (int)element->arg_list->arg.size() - 1; i >= 0; i--)	{
				out_scn.scn.push(element->arg_list->arg[i]->exp->tmp_form);
				arg_form_text += tostr_form(element->arg_list->arg[i]->exp->tmp_form) + _T(", ");
				// 型が FM_LIST の場合は、型リストを追加
				if (element->arg_list->arg[i]->exp->tmp_form == FM_LIST)	{
					out_scn.scn.push((int)element->arg_list->arg[i]->exp->smp_exp->exp_list->form_list.size());	// リストのサイズ
					// 同じく逆順に並べる
					for (int j = (int)element->arg_list->arg[i]->exp->smp_exp->exp_list->form_list.size() - 1; j >= 0; j--)	{
						int form = dereference(element->arg_list->arg[i]->exp->smp_exp->exp_list->form_list[j]);
						out_scn.scn.push(form);
					}
				}
			}

			// 名前つき引数の個数
			out_scn.scn.push(element->arg_list->named_arg_cnt);

			// 名前つき引数の引数ID
			for (int i = (int)element->arg_list->arg.size() - 1; i >= 0; i--)	{
				if (element->arg_list->arg[i]->node_type == TNMS_SA_NODE_TYPE_ARG_WITH_NAME)	{
					out_scn.scn.push(element->arg_list->arg[i]->name_id);
				}
			}

			// 戻り値の型
			out_scn.scn.push(element->node_form);

			// デバッグテキスト
			add_out_txt(_T("CD_COMMAND, al_id = ") + tostr(element->arg_list_id) + _T(", al_size = ") + tostr((int)element->arg_list->arg.size()));
			add_out_txt(_T("\targ_form_list = ") + arg_form_text + _T("ret_form = ") + tostr_form(element->node_form));
		}
	}

	// 添え字の場合
	else if (element->node_type == TNMS_SA_NODE_TYPE_ELEMENT_ARRAY)	{

		// エレメントコードを積む
		out_scn.scn.push(CD_PUSH);
		out_scn.scn.push(FM_INT);
		out_scn.scn.push(ELM_ARRAY);
		add_out_txt(_T("CD_PUSH, ") + tostr_form(FM_INT) + _T(", ELM_ARRAY"));

		// 添え字を解析
		bs_exp(element->exp, true);		// 値を要求する
	}

	return true;
}


// ****************************************************************
// バイナリセーバー：<引数リスト>
// ================================================================
//	ex.	(1, "abc")
//		<式>		// 1
//		<式>		// "abc"
// ================================================================
//		need_value: 値を要求する（参照外しを要求される）
// ================================================================
bool C_tnms_binary_saver::bs_arg_list(BSP<TNMS_SA_NODE_ARG_LIST> arg_list, bool need_value)
{
	int arg_cnt = (int)arg_list->arg.size();
	for (int a = 0; a < arg_cnt; a ++)	{
		bool need_value_arg = need_value;

		if (arg_list->arg[a]->exp->tmp_form == FM_INT || arg_list->arg[a]->exp->tmp_form == FM_STR || arg_list->arg[a]->exp->tmp_form == FM_LIST)
			need_value_arg = true;		// int や str や list 型は値が必要（参照外しを要求する）

		if (!bs_arg(arg_list->arg[a], need_value_arg))
			return false;
	}

	return true;
}

// ****************************************************************
// バイナリセーバー：<引数>
// ================================================================
bool C_tnms_binary_saver::bs_arg(BSP<TNMS_SA_NODE_ARG> arg, bool need_value)
{
	// <式>
	if (!bs_exp(arg->exp, need_value))
		return false;

	return true;
}

// ****************************************************************
// バイナリセーバー：<名前>
// ================================================================
bool C_tnms_binary_saver::bs_name(BSP<TNMS_SA_NODE_NAME> name)
{
	// ★メッセージブロックを割り当てる特殊なコマンド
	bs_push_msg_block();

	// 名前
	if (!bs_literal(name->name))
		return false;

	out_scn.scn.push(CD_NAME);
	add_out_txt(_T("CD_NAME, ") + tostr(name->name->atom.opt));

	// 【名前】情報を登録
	bool new_name = true;
	if (!out_scn.namae_list.empty())	{
		ARRAY<int>::iterator end = out_scn.namae_list.end();
		for (ARRAY<int>::iterator itr = out_scn.namae_list.begin(); itr != end; ++itr)	{
			if (out_scn.str_list[*itr] == out_scn.str_list[name->name->atom.opt])	{
				new_name = false;
				break;
			}
		}
	}
	if (new_name)
		out_scn.namae_list.push_back(name->name->atom.opt);

#if 0
	G_name_cntr[out_scn.str_list[name->name->atom.opt]] = G_name_cntr[out_scn.str_list[name->name->atom.opt]] + 1;
#endif

	return true;
}

// ****************************************************************
// バイナリセーバー：<リテラル>
// ================================================================
bool C_tnms_binary_saver::bs_literal(BSP<TNMS_SA_NODE_ATOM> Literal)
{
	// ラベルは FM_INT に変換して送る
	if (Literal->node_form == FM_LABEL)	{
		out_scn.scn.push(CD_PUSH);
		out_scn.scn.push(FM_INT);
		out_scn.scn.push(Literal->atom.opt);
		add_out_txt(_T("CD_PUSH, ") + tostr_form(FM_INT) + _T(", ") + tostr(Literal->atom.opt));
	}
	else	{
		out_scn.scn.push(CD_PUSH);
		out_scn.scn.push(Literal->node_form);
		out_scn.scn.push(Literal->atom.opt);
		add_out_txt(_T("CD_PUSH, ") + tostr_form(Literal->node_form) + _T(", ") + tostr(Literal->atom.opt));
	}

	return true;
}

// ****************************************************************
// バイナリセーバー：<代入演算子>
// ================================================================
bool C_tnms_binary_saver::bs_assign_operator(BSP<TNMS_SA_NODE_ATOM> opr)
{
	out_scn.scn.push((BYTE)opr->atom.opt);

	return true;
}

// ****************************************************************
// バイナリセーバー：<単項演算子>
// ================================================================
bool C_tnms_binary_saver::bs_operator_1(BSP<TNMS_SA_NODE_ATOM> opr)
{
	out_scn.scn.push((BYTE)opr->atom.opt);	// 演算子タイプ

	return true;
}

// ****************************************************************
// バイナリセーバー：<二項演算子>
// ================================================================
bool C_tnms_binary_saver::bs_operator_2(BSP<TNMS_SA_NODE_ATOM> opr)
{
	out_scn.scn.push((BYTE)opr->atom.opt);	// 演算子タイプ

	return true;
}

// ****************************************************************
// バイナリセーバー：<終端>
// ================================================================
bool C_tnms_binary_saver::bs_eof(BSP<TNMS_SA_NODE_ATOM> eof)
{
	out_scn.scn.push(CD_EOF);

	return true;
}

// ****************************************************************
// 型コード → 型の名前
// ================================================================
TSTR C_tnms_binary_saver::tostr_form(int form_code)
{
	return m_piad->form_table.get_form_by_code(form_code)->name;
}

// ****************************************************************
// バイナリセーバー：メッセージブロック
// ================================================================
void C_tnms_binary_saver::bs_push_msg_block()
{
	// メッセージブロック
	out_scn.scn.push(CD_ELM_POINT);
	out_scn.scn.push(CD_PUSH);
	out_scn.scn.push(FM_INT);
	out_scn.scn.push(ELM_GLOBAL_MSG_BLOCK);		// メッセージブロック
	out_scn.scn.push(CD_COMMAND);				// コマンド
	out_scn.scn.push(0);						// 引数リスト番号
	out_scn.scn.push(0);						// 引数の個数
	out_scn.scn.push(0);						// 名前つき引数の個数
	out_scn.scn.push(FM_VOID);					// 戻り値の型
}

// ****************************************************************
// バイナリセーバー：エラー
// ================================================================
bool C_tnms_binary_saver::error(TNMSERR_BS type, S_tnms_atom atom)
{
	last_error.type = type;
	last_error.atom = atom;

	return false;
}

void C_tnms_binary_saver::clear_error()
{
	last_error.type = TNMSERR_BS_NONE;
}

S_tnms_atom C_tnms_binary_saver::get_error_atom()
{
	return last_error.atom;
}

int C_tnms_binary_saver::get_error_line()
{
	return last_error.atom.line;
}

TSTR C_tnms_binary_saver::get_error_str()
{
	// エラーの設定
	switch (last_error.type)	{
		case TNMSERR_BS_ILLEGAL_DEFAULT_ARG:		return _T("対応していない型のデフォルト引数です。");
		case TNMSERR_BS_CONTINUE_NO_LOOP:			return _T("continue はループの中でしか使えません。");
		case TNMSERR_BS_BREAK_NO_LOOP:				return _T("break はループの中でしか使えません。");
		case TNMSERR_BS_NEED_REFERENCE:				return _T("左辺値が必要です。");
		case TNMSERR_BS_NEED_VALUE:					return _T("右辺値が必要です。");
	}

	return _T("バイナリセーブ：不明なエラーです。");
}

