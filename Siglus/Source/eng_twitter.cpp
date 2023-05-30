#include	"pch.h"

#include	<wininet.h>

#pragma		comment(lib, "wininet.lib")

#include	<atlbase.h>		// ATL基本機能用ヘッダファイル
#include	<atlcom.h>		// ATL COM共通用ヘッダファイル
#include	<atlhost.h>		// ATL コンテナ機能サポート用ヘッダファイル
#include	<EXDISPID.H>

#include	<list>
#include	<boost/algorithm/string.hpp>

#include	<pico_json.h>

#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"data/tnm_global_data.h"
#include	"data/tnm_dir.h"
#include	"data/tnm_save_00.h"
#include	"engine/eng_graphics.h"
#include	"engine/eng_dialog.h"
#include	"engine/ifc_proc_stack.h"

#include	"localize/localize.h"

#include	"../resource.h"

const TSTR TWITTER_API_DOMAIN = _T("api.twitter.com");
const TSTR TWITTER_UPLOAD_DOMAIN = _T("upload.twitter.com");
const TSTR TWITTER_USER_AGENT = _T("SiglusEngine/1.0.0");
const TSTR TWITTER_SIGNATURE_METHOD = _T("HMAC-SHA1");
const TSTR TWITTER_OATH_VERSION = _T("1.0");

static HINTERNET G_h_inet = NULL;
static HWND G_h_auth_dlg = NULL;
static CComQIPtr<IWebBrowser2> G_p_browser;

TSTR G_oauth_request_token = _T("");
TSTR G_oauth_request_token_secret = _T("");
TSTR G_oauth_access_token = _T("");
TSTR G_oauth_access_token_secret = _T("");
TSTR G_twitter_user_id = _T("");
TSTR G_twitter_user_name = _T("");
TSTR G_twitter_screen_name = _T("");

bool twitter_authorize();
bool twitter_authorize_2(CTSTR& query_str);
bool twitter_authorize_3();

// ****************************************************************
// ペアの比較
// ================================================================
class C_pair_comp {
public:
	bool operator()(const std::pair<TSTR, TSTR>& riLeft, const std::pair<TSTR, TSTR>& riRight) const {
		return riLeft.first < riRight.first;
	}
};

// ****************************************************************
// クエリーをマップに
// ================================================================
std::map<TSTR, TSTR> query_str_to_map(CTSTR& query_str) {
	std::map<TSTR, TSTR> query;

	std::list<TSTR> response_split_A;
	boost::split(response_split_A, query_str, boost::is_any_of(_T("&")));
	for (std::list<TSTR>::iterator itr = response_split_A.begin(); itr != response_split_A.end(); ++itr) {
		std::list<TSTR> response_split_B;
		boost::split(response_split_B, *itr, boost::is_any_of(_T("=")));
		if (response_split_B.size() == 2) {
			std::list<TSTR>::iterator itr_B = response_split_B.begin();
			TSTR key = *itr_B++;
			TSTR value = *itr_B++;
			query.insert(std::pair<TSTR, TSTR>(key, value));
		}
	}

	return query;
}

// ****************************************************************
// Twitter 認証ダイアログ：コールバック
// ================================================================

// 数字は任意。詳しくはIDispatchImplのドキュメント参照
#define SINKID_COUNTEREVENTS 0

// シンク（イベント受付窓口）クラス
class ATL_NO_VTABLE CIESink :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispEventImpl<SINKID_COUNTEREVENTS, CIESink, &DIID_DWebBrowserEvents2>
{
private:
	CComPtr<IUnknown> m_pUnkIE;

public:
	CIESink() {}

	HRESULT AdviseToIE(CComPtr<IUnknown> pUnkIE)
	{
		m_pUnkIE = pUnkIE;

		// タイプ ライブラリへのポインタやバージョン情報の設定
		AtlGetObjectSourceInterface(pUnkIE, &m_libid, &m_iid, &m_wMajorVerNum, &m_wMinorVerNum);
		// イベントをDispatch
		HRESULT hr = this->DispEventAdvise(pUnkIE);

		return hr;
	}

	BEGIN_COM_MAP(CIESink)
		COM_INTERFACE_ENTRY_IID(DIID_DWebBrowserEvents2, CIESink)
	END_COM_MAP()

	BEGIN_SINK_MAP(CIESink)
		SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2, OnBeforeNavigate2)
		SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2, DISPID_NEWWINDOW2, OnNewWindow2)
		SINK_ENTRY_EX(SINKID_COUNTEREVENTS, DIID_DWebBrowserEvents2, DISPID_ONQUIT, OnQuit)
	END_SINK_MAP()

	// OnBeforeNavigate2
	void _stdcall OnBeforeNavigate2(IDispatch** ppDisp, VARIANT* pvUrl, VARIANT*, VARIANT* pvTarget, VARIANT*, VARIANT*, VARIANT_BOOL *pbCancel)
	{
		TSTR response = pvUrl->bstrVal;

		// コールバック！
		if (!Gp_ini->twitter_callback_url.empty() && response.substr(0, Gp_ini->twitter_callback_url.size()) == Gp_ini->twitter_callback_url) {

			// 処理をキャンセルする
			*pbCancel = TRUE;
			// ダイアログを閉じる
			::SendMessage(G_h_auth_dlg, WM_CLOSE, 0, 0);
			// 認証処理を続ける
			twitter_authorize_2(response.substr(Gp_ini->twitter_callback_url.size() + 1));
		}

		return;
	}

	// OnNewWindow2
	void _stdcall OnNewWindow2(IDispatch** ppDisp, VARIANT_BOOL *pbCancel)
	{
		// キャンセルする（新しいウィンドウは開かない）
		*pbCancel = VARIANT_TRUE;
		return;
	}

	// OnQuit
	void _stdcall OnQuit()
	{
		DispEventUnadvise(m_pUnkIE);
		return;
	}
};

// ****************************************************************
// Twitter 認証：ダイアログ
// ================================================================

static BOOL CALLBACK DialogFunc(HWND hDlg, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg)
	{
		// ダイアログボックスの初期化、このメッセージに飛んできた時点で、
		// すでにダイアログボックス上には、ActiveXコントロールの作成が完了している。
	case WM_INITDIALOG:
	{
		G_h_auth_dlg = hDlg;

		// ダイアログの大きさを取得
		RECT dlg_client_rect;
		::GetClientRect(hDlg, &dlg_client_rect);
		int dlg_client_width = dlg_client_rect.right - dlg_client_rect.left;
		int dlg_client_height = dlg_client_rect.bottom - dlg_client_rect.top;

		// IE ウィンドウを作成
		CAxWindow ie_window;
		HWND h_wnd_ie = ie_window.Create(hDlg, C_rect(0, 0, dlg_client_width, dlg_client_height), _T("Shell.Explorer.2"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

		// ブラウザを取得
		CComPtr<IUnknown> p_unk_IE;
		if (AtlAxGetControl(h_wnd_ie, &p_unk_IE) == S_OK)
		{
			// グローバルに保存
			G_p_browser = p_unk_IE;
			if (G_p_browser)
			{
				// シンクオブジェクトの作成
				CComObject<CIESink>* sink;
				CComObject<CIESink>::CreateInstance(&sink);
				// イベントを Dispatch するよう、IE コントロールにお願い
				HRESULT hr = sink->AdviseToIE(p_unk_IE);
				if (SUCCEEDED(hr))
				{
					// 認証画面を開く
					CComVariant vempty, vUrl((_T("https://api.twitter.com/oauth/authenticate?oauth_token=") + url_encode(G_oauth_request_token)).c_str());
					G_p_browser->Navigate2(&vUrl, &vempty, &vempty, &vempty, &vempty);
				}
			}
		}
	}
	break;

	case WM_CLOSE:
	{
		// 閉じるボタンを押されたらダイアログを終わる。
		EndDialog(hDlg, 0);
	}
	break;

	case WM_DESTROY:
	{
		// ダイアログが破棄される前に、フレックスグリッドコントロールを破棄する。
		// スマートポインタクラスのメソッドを使用する場合は、
		//  .演算子を使用する～。下記も参照してね～。
		G_p_browser.Release();
	}
	break;
	}

	return 0;
}

// ****************************************************************
// Twitter リクエスト
// ================================================================
bool twitter_request(TSTR request_page, CTSTR& domain, CTSTR& request_method, ARRAY<std::pair<TSTR, TSTR>> request_param_list, ARRAY<std::pair<TSTR, TSTR>> oauth_param_list, TSTR http_header, CTSTR& body_data, CTSTR& token_secret, ARRAY<BYTE>& response_buf) {
	HINTERNET hSession = NULL;
	HINTERNET hRequest = NULL;

	// 初回の接続の場合はインターネットに接続
	if (G_h_inet == NULL)
	{
		G_h_inet = InternetOpen(TWITTER_USER_AGENT.c_str(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (G_h_inet == NULL) {
			set_last_error_win(_T("InternetOpen"));
			error_box(_T("インターネットに接続できませんでした。\n\n") + get_last_error_msg());
			return false;
		}
	}

	// リクエストアドレス
	TSTR request_address = _T("https://") + domain + request_page;

	// 署名用のパラメータリスト
	ARRAY<std::pair<TSTR, TSTR>> authorization_param_list;
	authorization_param_list.insert(authorization_param_list.end(), request_param_list.begin(), request_param_list.end());
	authorization_param_list.insert(authorization_param_list.end(), oauth_param_list.begin(), oauth_param_list.end());
	// 署名のためにパラメータを並び替え
	std::sort(authorization_param_list.begin(), authorization_param_list.end(), C_pair_comp());
	// 署名を作成
	TSTR signature_key = url_encode(Gp_ini->twitter_api_secret) + _T("&") + url_encode(token_secret);
	TSTR signature_param;
	for (ARRAY<std::pair<TSTR, TSTR>>::iterator itr = authorization_param_list.begin(); itr != authorization_param_list.end(); ++itr) {
		if (signature_param != _T("")) {
			signature_param += _T("&");
		}
		signature_param += url_encode(itr->first) + _T("=") + url_encode(itr->second);
	}
	TSTR signature_value = url_encode(request_method) + _T("&") + url_encode(request_address) + _T("&") + url_encode(signature_param);
	TSTR signature = hmac_base64_encode(signature_key, signature_value);
	// 署名をパラメータに加える
	authorization_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_signature"), signature));

	// Authorization ヘッダを作成
	TSTR auth_header = _T("");
	for (ARRAY<std::pair<TSTR, TSTR>>::iterator itr = authorization_param_list.begin(); itr != authorization_param_list.end(); ++itr) {
		if (auth_header != _T("")) {
			auth_header += _T(",");
		}
		auth_header += url_encode(itr->first) + _T("=\"") + url_encode(itr->second) + _T("\"");
	}
	auth_header = _T("OAuth ") + auth_header;

	// クエリーパラメータを付与（リクエストパラメータは POST であってもここで与える！！）
	TSTR query_params = _T("");
	for (ARRAY<std::pair<TSTR, TSTR>>::iterator itr = request_param_list.begin(); itr != request_param_list.end(); ++itr) {
		if (query_params != _T("")) {
			query_params += _T("&");
		}
		query_params += url_encode(itr->first) + _T("=") + url_encode(itr->second);
	}
	if (query_params != _T("")) {
		request_page += _T("?") + query_params;
	}

	// HTTP ヘッダを作成
	if (!http_header.empty()) {
		http_header += _T("\r\n");
	}
	http_header += _T("Authorization: ") + auth_header;

	// Body データをバイト列に変換
	MBSTR mb_body_data = TSTR_to_UTF8(body_data);

	// サーバーへ接続
	hSession = InternetConnect(G_h_inet, domain.c_str(), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (hSession == NULL) {
		set_last_error_win(_T("InternetConnect"));
		error_box(_T("サーバーへの接続に失敗しました。\n\n") + get_last_error_msg());
		goto twitter_request_error;
	}

	// リクエストを作成
	hRequest = HttpOpenRequest(hSession, request_method.c_str(), request_page.c_str(), NULL, NULL, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_SECURE, 0);
	if (hRequest == NULL) {
		set_last_error_win(_T("HttpSendRequest"));
		error_box(_T("サーバーへのリクエストが開けませんでした。\n\n") + get_last_error_msg());
		goto twitter_request_error;
	}

	// 送信
	if (HttpSendRequest(hRequest, http_header.c_str(), http_header.size(), (mb_body_data.empty() ? NULL : (void *)mb_body_data.c_str()), mb_body_data.length()) == FALSE) {
		set_last_error_win(_T("HttpSendRequest"));
		error_box(_T("リクエストの送信に失敗しました。\nインターネットに繋がっていない可能性があります。\n\n") + get_last_error_msg());
		goto twitter_request_error;
	}

	// レスポンスの長さを調べる
	DWORD response_buf_size_text_size = 1024;
	TCHAR response_buf_size_text[1024] = { 0 };

	HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH, response_buf_size_text, &response_buf_size_text_size, NULL);
	DWORD response_size = str_to_int(response_buf_size_text, 0);
	response_buf.resize(response_size);

	// レスポンスを読み込み
	if (!InternetReadFile(hRequest, response_buf.get(), response_size, &response_size)) {
		set_last_error_win(_T("InternetReadFile"));
		error_box(_T("サーバーからのレスポンスが読み込めませんでした。\n\n") + get_last_error_msg());
		goto twitter_request_error;
	}

	// 終了処理
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hSession);

	return true;

twitter_request_error:

	if (hRequest)
	{
		InternetCloseHandle(hRequest);
	}
	if (hSession)
	{
		InternetCloseHandle(hSession);
	}

	return false;
}

// ****************************************************************
// Twitter 認証済みかどうかを判定
// ================================================================
bool twitter_is_authorized()
{
	if (G_oauth_access_token.empty()) return false;
	if (G_oauth_access_token_secret.empty()) return false;
	if (G_twitter_user_id.empty()) return false;
	if (G_twitter_user_name.empty()) return false;
	if (G_twitter_screen_name.empty()) return false;

	return true;
}

// ****************************************************************
// Twitter 設定を読み込み
// ================================================================
bool twitter_load_state(C_tnm_save_stream& stream)
{
	G_oauth_access_token.clear();
	G_oauth_access_token_secret.clear();
	G_twitter_user_id.clear();
	G_twitter_user_name.clear();
	G_twitter_screen_name.clear();

	TSTR path = _T("SOFTWARE\\VisualArts\\SiglusEngine\\") + Gp_ini->game_name;

	reg_get_value_str(HKEY_CURRENT_USER, path, _T("oauth_access_token"), G_oauth_access_token);
	reg_get_value_str(HKEY_CURRENT_USER, path, _T("oauth_access_token_secret"), G_oauth_access_token_secret);
	reg_get_value_str(HKEY_CURRENT_USER, path, _T("twitter_user_id"), G_twitter_user_id);
	reg_get_value_str(HKEY_CURRENT_USER, path, _T("twitter_user_name"), G_twitter_user_name);
	reg_get_value_str(HKEY_CURRENT_USER, path, _T("twitter_screen_name"), G_twitter_screen_name);

	return true;
}

// ****************************************************************
// Twitter 設定を保存
// ================================================================
bool twitter_save_state(C_tnm_save_stream& stream)
{
	TSTR path = _T("SOFTWARE\\VisualArts\\SiglusEngine\\") + Gp_ini->game_name;

	if (!reg_set_value_str(HKEY_CURRENT_USER, path, _T("oauth_access_token"), G_oauth_access_token))
	{
		message_box(_T("レジストリキーが設定できませんでした。\n") + get_last_error_msg(), Gp_ini->game_name, MB_ICONWARNING);
		return false;
	}
	if (!reg_set_value_str(HKEY_CURRENT_USER, path, _T("oauth_access_token_secret"), G_oauth_access_token_secret))
	{
		message_box(_T("レジストリキーが設定できませんでした。\n") + get_last_error_msg(), Gp_ini->game_name, MB_ICONWARNING);
		return false;
	}
	if (!reg_set_value_str(HKEY_CURRENT_USER, path, _T("twitter_user_id"), G_twitter_user_id))
	{
		message_box(_T("レジストリキーが設定できませんでした。\n") + get_last_error_msg(), Gp_ini->game_name, MB_ICONWARNING);
		return false;
	}
	if (!reg_set_value_str(HKEY_CURRENT_USER, path, _T("twitter_user_name"), G_twitter_user_name))
	{
		message_box(_T("レジストリキーが設定できませんでした。\n") + get_last_error_msg(), Gp_ini->game_name, MB_ICONWARNING);
		return false;
	}
	if (!reg_set_value_str(HKEY_CURRENT_USER, path, _T("twitter_screen_name"), G_twitter_screen_name))
	{
		message_box(_T("レジストリキーが設定できませんでした。\n") + get_last_error_msg(), Gp_ini->game_name, MB_ICONWARNING);
		return false;
	}

	return true;
}

// ****************************************************************
// Twitter 認証プロセス１
// ================================================================
bool twitter_authorize()
{
	// 認証をクリア
	G_oauth_access_token.clear();
	G_oauth_access_token_secret.clear();
	G_twitter_user_id.clear();
	G_twitter_user_name.clear();
	G_twitter_screen_name.clear();
	// ダイアログを更新
	tnm_dlg_update_twitter_dialog();

	// パラメータリストを作成
	ARRAY<std::pair<TSTR, TSTR>> oauth_param_list;
	oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_callback"), Gp_ini->twitter_callback_url));
	oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_consumer_key"), Gp_ini->twitter_api_key));
	oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_signature_method"), TWITTER_SIGNATURE_METHOD));
	oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_timestamp"), tostr(time(NULL))));
	oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_nonce"), tostr(rand())));
	oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_version"), TWITTER_OATH_VERSION));

	// Twitter リクエスト
	ARRAY<BYTE> response_buf;
	if (!twitter_request(_T("/oauth/request_token"), TWITTER_API_DOMAIN, _T("POST"), ARRAY<std::pair<TSTR, TSTR>>(), oauth_param_list, _T(""), _T(""), _T(""), response_buf))
		return false;

	TSTR response = MBSTR_to_TSTR(MBSTR(response_buf.get(), response_buf.get() + response_buf.size()));

	// レスポンスを解析
	std::map<TSTR, TSTR> query = query_str_to_map(response);
	// リクエストトークンをゲット！
	G_oauth_request_token = query[_T("oauth_token")];
	G_oauth_request_token_secret = query[_T("oauth_token_secret")];

	// 認証ダイアログを開く
	AtlAxDialogBox(G_app.h_inst, MAKEINTRESOURCE(IDD_TWITTER_AUTHORIZE), G_app.h_wnd, DialogFunc, 0);

	return true;
}

// ****************************************************************
// Twitter 認証プロセス２
// ================================================================
bool twitter_authorize_2(CTSTR& query_str)
{
	// レスポンスを解析
	std::map<TSTR, TSTR> query = query_str_to_map(query_str);

	// 許可だった場合
	if (query.find(_T("oauth_verifier")) != query.end())
	{
		TSTR oauth_token = query[_T("oauth_token")];
		TSTR oauth_verifier = query[_T("oauth_verifier")];

		// パラメータリストを作成
		ARRAY<std::pair<TSTR, TSTR>> oauth_param_list;
		oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_consumer_key"), Gp_ini->twitter_api_key));
		oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_token"), oauth_token));
		oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_verifier"), oauth_verifier));
		oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_signature_method"), TWITTER_SIGNATURE_METHOD));
		oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_timestamp"), tostr(time(NULL))));
		oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_nonce"), tostr(rand())));
		oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_version"), TWITTER_OATH_VERSION));

		// Twitter リクエスト
		ARRAY<BYTE> response_buf;
		if (!twitter_request(_T("/oauth/access_token"), TWITTER_API_DOMAIN, _T("POST"), ARRAY<std::pair<TSTR, TSTR>>(), oauth_param_list, _T(""), _T(""), G_oauth_request_token_secret, response_buf))
			return false;

		TSTR response = MBSTR_to_TSTR(MBSTR(response_buf.get(), response_buf.get() + response_buf.size()));

		// レスポンスを解析
		std::map<TSTR, TSTR> query = query_str_to_map(response);
		// アクセストークンをゲット！
		G_oauth_access_token = query[_T("oauth_token")];
		G_oauth_access_token_secret = query[_T("oauth_token_secret")];
		G_twitter_user_id = query[_T("user_id")];
		G_twitter_screen_name = query[_T("screen_name")];
		// 次の処理へ
		twitter_authorize_3();
	}
	// 許可が出なかった場合
	else
	{
		error_box(_T("認証に失敗しました。\n\n") + query_str);
	}

	return true;
}

// ****************************************************************
// Twitter 認証プロセス３
// ================================================================
bool twitter_authorize_3()
{
	// パラメータリストを作成
	ARRAY<std::pair<TSTR, TSTR>> oauth_param_list;
	oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_consumer_key"), Gp_ini->twitter_api_key));
	oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_token"), G_oauth_access_token));
	oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_signature_method"), TWITTER_SIGNATURE_METHOD));
	oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_timestamp"), tostr(time(NULL))));
	oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_nonce"), tostr(rand())));
	oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_version"), TWITTER_OATH_VERSION));

	ARRAY<std::pair<TSTR, TSTR>> request_param_list;
	request_param_list.push_back(std::pair<TSTR, TSTR>(_T("user_id"), G_twitter_user_id));

	// リクエスト
	ARRAY<BYTE> response_buf;
	if (!twitter_request(_T("/1.1/users/show.json"), TWITTER_API_DOMAIN, _T("GET"), request_param_list, oauth_param_list, _T(""), _T(""), G_oauth_access_token_secret, response_buf))
		return false;

	MBSTR response_mb = MBSTR(response_buf.get(), response_buf.get() + response_buf.size());

	// レスポンスを解析
	pico_json::value v1;
	const char *p1 = (char *)response_buf.get(), *p2 = (char *)(response_buf.get() + response_buf.size());
	pico_json::parse(v1, (const char *)(p1), (const char *)(p2));		// ↑この変換は p1 を左辺値にするために必須です
	pico_json::object o1 = v1.get<pico_json::object>();
	if (o1.find("name") != o1.end())
	{
		// ユーザー名をゲット！
		G_twitter_user_name = UTF8_to_TSTR(o1["name"].to_str());
		// 成功！
		message_box(LCL_STR_TWITTER_DIALOG_STR_AUTH_SUCCESS, Gp_ini->game_name, MB_OK);
		// ダイアログを更新
		tnm_dlg_update_twitter_dialog();
	}
	else
	{
		error_box(_T("ユーザー名が取得できませんでした。"));
	}

	return true;
}

// ****************************************************************
// Twitter 投稿
// ================================================================
bool twitter_tweet(CTSTR& text, CTSTR& image_path)
{
	ARRAY<BYTE> image_buf;
	if (!C_file::read_full_data(image_path, image_buf))
	{
		error_box(image_path + _T(" が読み込めませんでした。"));
		return false;
	}

	TSTR image_base64 = base64_encode(image_buf);

	TSTR boundary = _T("S-i-g-l-u-s--") + tostr(rand()) + tostr(rand());

	TSTR body_data;
	body_data += _T("--") + boundary + _T("\r\n");
	body_data += _T("Content-Disposition: form-data; name=\"media_data\"; \r\n");
	body_data += _T("\r\n");
	body_data += image_base64 + _T("\r\n");
	body_data += _T("--") + boundary + _T("--\r\n");
	body_data += _T("\r\n");

	// HTTP ヘッダを作成
	TSTR http_header = _T("Content-Type: multipart/form-data; boundary=" + boundary);

	// パラメータリストを作成
	ARRAY<std::pair<TSTR, TSTR>> oauth_param_list;
	oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_consumer_key"), Gp_ini->twitter_api_key));
	oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_token"), G_oauth_access_token));
	oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_signature_method"), TWITTER_SIGNATURE_METHOD));
	oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_timestamp"), tostr(time(NULL))));
	oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_nonce"), tostr(rand())));
	oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_version"), TWITTER_OATH_VERSION));

	// リクエスト
	ARRAY<BYTE> response_buf;
	if (!twitter_request(_T("/1.1/media/upload.json"), TWITTER_UPLOAD_DOMAIN, _T("POST"), ARRAY<std::pair<TSTR, TSTR>>(), oauth_param_list, http_header, body_data, G_oauth_access_token_secret, response_buf))
		return false;

	TSTR response = MBSTR_to_TSTR(MBSTR(response_buf.get(), response_buf.get() + response_buf.size()));

	// メディアIDを取り出すよ
	int media_id_pos = response.find(_T("\"media_id_string\":"));
	if (media_id_pos == TSTR::npos) {
		error_box(_T("media_id が見つかりませんでした。"));
	}
	else {
		int quote_start_pos = response.find(_T("\""), media_id_pos + 18);
		int quote_end_pos = response.find(_T("\""), quote_start_pos + 1);
		TSTR media_id_string = response.substr(quote_start_pos + 1, quote_end_pos - quote_start_pos - 1);

		// パラメータリストを作成
		ARRAY<std::pair<TSTR, TSTR>> oauth_param_list;
		oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_consumer_key"), Gp_ini->twitter_api_key));
		oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_token"), G_oauth_access_token));
		oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_signature_method"), TWITTER_SIGNATURE_METHOD));
		oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_timestamp"), tostr(time(NULL))));
		oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_nonce"), tostr(rand())));
		oauth_param_list.push_back(std::pair<TSTR, TSTR>(_T("oauth_version"), TWITTER_OATH_VERSION));

		ARRAY<std::pair<TSTR, TSTR>> request_param_list;
		request_param_list.push_back(std::pair<TSTR, TSTR>(_T("status"), text));
		request_param_list.push_back(std::pair<TSTR, TSTR>(_T("media_ids"), media_id_string));

		// リクエスト
		ARRAY<BYTE> response_buf;
		if (!twitter_request(_T("/1.1/statuses/update.json"), TWITTER_API_DOMAIN, _T("POST"), request_param_list, oauth_param_list, _T(""), _T(""), G_oauth_access_token_secret, response_buf))
			return false;

		TSTR response = MBSTR_to_TSTR(MBSTR(response_buf.get(), response_buf.get() + response_buf.size()));
		int error_pos = response.find(_T("\"errors\":"));
		if (error_pos != TSTR::npos)
		{
			int message_pos = response.find(_T("\"message\":"));
			if (message_pos != TSTR::npos)
			{
				int quote_start_pos = response.find(_T("\""), message_pos + 10);
				int quote_end_pos = response.find(_T("\""), quote_start_pos + 1);
				TSTR message_string = response.substr(quote_start_pos + 1, quote_end_pos - quote_start_pos - 1);
				// 失敗！
				error_box(_T("投稿に失敗しました。\n\n") + message_string);
			}
		}
		else
		{
			// 成功！
			message_box(LCL_STR_TWITTER_DIALOG_STR_POST_SUCCESS, Gp_ini->game_name, MB_OK);
			// ダイアログに通知
			tnm_dlg_twitter_dialog_on_success_tweet();
		}
	}

	return true;
}

// ****************************************************************
// Twitter 処理開始
// ================================================================
bool tnm_twitter_start()
{
	// ダイアログを閉じる
	tnm_dlg_close_twitter_dialog();

	// テクスチャを画像に保存する
	TSTR out_path = Gp_dir->get_tweet_image_file_path();

	// テクスチャをロックする
	D3DLOCKED_RECT locked_rect;
	if (!Gp_wnd->m_capture_texture_for_tweet->lock_rect(0, &locked_rect, NULL, D3DLOCK_DISCARD))
	{
		tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("ローカルファイルへのキャプチャーに失敗しました。\n") + get_last_error_msg());
	}
	else
	{
		// ローカルに保存する
		tnm_create_png_from_texture_and_save_to_file(out_path, Gp_ini->screen_size.cx, Gp_ini->screen_size.cy, &locked_rect, false);
		// テクスチャをアンロック
		Gp_wnd->m_capture_texture_for_tweet->unlock_rect(0);
		// Twitter ダイアログを開く
		tnm_dlg_open_twitter_dialog();
	}

	return true;
}


