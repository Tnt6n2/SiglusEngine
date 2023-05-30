#pragma		once

// ****************************************************************
// 演算子コード
// ================================================================
const BYTE	OP_NONE = 0x00;
const BYTE	OP_PLUS = 0x01;
const BYTE	OP_MINUS = 0x02;
const BYTE	OP_MULTIPLE = 0x03;
const BYTE	OP_DIVIDE = 0x04;
const BYTE	OP_AMARI = 0x05;

const BYTE	OP_EQUAL = 0x10;
const BYTE	OP_NOT_EQUAL = 0x11;
const BYTE	OP_GREATER = 0x12;
const BYTE	OP_GREATER_EQUAL = 0x13;
const BYTE	OP_LESS = 0x14;
const BYTE	OP_LESS_EQUAL = 0x15;

const BYTE	OP_LOGICAL_AND = 0x20;
const BYTE	OP_LOGICAL_OR = 0x21;

const BYTE	OP_TILDE = 0x30;
const BYTE	OP_AND = 0x31;
const BYTE	OP_OR = 0x32;
const BYTE	OP_HAT = 0x33;
const BYTE	OP_SL = 0x34;
const BYTE	OP_SR = 0x35;
const BYTE	OP_SR3 = 0x36;

// ****************************************************************
// 命令コード
// ================================================================
typedef		BYTE	T_tnm_code;

const T_tnm_code	CD_NONE = 0x00;

const T_tnm_code	CD_NL = 0x01;			// 改行
const T_tnm_code	CD_PUSH = 0x02;			// stack push
const T_tnm_code	CD_POP = 0x03;			// stack pop
const T_tnm_code	CD_COPY = 0x04;			// stack copy
const T_tnm_code	CD_PROPERTY = 0x05;		// プロパティ展開
const T_tnm_code	CD_COPY_ELM = 0x06;		// エレメントコピー
const T_tnm_code	CD_DEC_PROP = 0x07;		// プロパティを定義
const T_tnm_code	CD_ELM_POINT = 0x08;	// エレメント開始ポイントを設定
const T_tnm_code	CD_ARG = 0x09;			// 引数を展開する

const T_tnm_code	CD_GOTO = 0x10;			// ジャンプ
const T_tnm_code	CD_GOTO_TRUE = 0x11;	// 真のときジャンプ
const T_tnm_code	CD_GOTO_FALSE = 0x12;	// 偽のときジャンプ
const T_tnm_code	CD_GOSUB = 0x13;		// gosub （戻り値：int）
const T_tnm_code	CD_GOSUBSTR = 0x14;		// gosub （戻り値：str）
const T_tnm_code	CD_RETURN = 0x15;		// return
const T_tnm_code	CD_EOF = 0x16;			// シーン終了

const T_tnm_code	CD_ASSIGN = 0x20;		// 代入
const T_tnm_code	CD_OPERATE_1 = 0x21;	// 単項演算子
const T_tnm_code	CD_OPERATE_2 = 0x22;	// 二項演算子

const T_tnm_code	CD_COMMAND = 0x30;			// コマンド
const T_tnm_code	CD_TEXT = 0x31;				// テキスト
const T_tnm_code	CD_NAME = 0x32;				// 名前
const T_tnm_code	CD_SEL_BLOCK_START = 0x33;	// 選択肢ブロック開始
const T_tnm_code	CD_SEL_BLOCK_END = 0x34;	// 選択肢ブロック終了
