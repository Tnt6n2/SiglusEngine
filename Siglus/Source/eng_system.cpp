#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"engine/eng_graphics.h"
#include	"engine/ifc_error.h"
#include	"engine/eng_emote.h"

// ****************************************************************
// ディスクドライブを検索する
// ================================================================
TCHAR tnm_search_disc_drive()
{
	int res = 0;

	// ★★★重要★★★
	// ディスクドライブを検索するため、一時的にエラーを回避する
	// 一部の環境において、ファイルを探しに行くだけで「ディスクを入れろ」って警告が出るのです。
	int error_mode = SetErrorMode(SEM_FAILCRITICALERRORS);

	// ドライブを調べる
	for (TCHAR letter = _T('A'); letter <= _T('Z'); letter ++)	{
		TSTR drive_path = TSTR() + letter + _T(":\\");
		if (::GetDriveType(drive_path.c_str()) == DRIVE_CDROM)	{
			TSTR env_path = drive_path + Gp_ini->disc_mark;
			if (file_get_type(env_path) == 1)	{
				res = letter;	// ドライブレター決定
				break;
			}
		}
	}

	// ★★★重要★★★
	// エラー処理の設定を元に戻します。これ忘れたらえらいことやな…。
	SetErrorMode(error_mode);

	return res;
}

// ****************************************************************
// wmi 準備
// ================================================================
#include	<Wbemidl.h>
#include	<comdef.h>

#pragma comment(lib, "wbemuuid.lib")

// ****************************************************************
// wmi：オブジェクトを取得する
// ================================================================
TSTR tnm_wmi_get_object(IWbemClassObject* wbem_object, CTSTR& property_name_t)
{
	TSTR result;

	// プロパティの名前を設定する
	const BSTR property_name_b = (BSTR)property_name_t.c_str();

	// プロパティの値を取得する
	VARIANT property_value;
	_bstr_t display_name;
	VariantInit(&property_value);
	if (wbem_object->Get(property_name_b, 0, &property_value, 0, 0) == WBEM_S_NO_ERROR)	{

		if (property_value.vt == 1)	{			// NULL の場合
		}
		else if (property_value.vt == 8200)	{	// 配列の場合
		}
		else	{		// それら以外の場合は値を文字列として取得する
			display_name = property_value;
			result += display_name;
		}

		VariantClear(&property_value);
	}

	return result;
}

// ****************************************************************
// スペック情報を取得する：CPU 名：WMI バージョン
// ================================================================
// ↓WMI を利用して CPU 名を取得するルーチン
// ★一部環境（XP、Core2 (Walfなんちゃら））で正常に取得できませんでした！
// KB953955 を当てると修正されます。ユーザに当ててもらうわけにはいかないのでボツです。
// Windows Update で更新してくれたらいいのに…。

#if 0
TSTR tnm_get_spec_info(IWbemServices* wbem_services, TSTR service_name, TSTR property_name_t)
{
	TSTR result;

	service_name = _T("SELECT * FROM ") + service_name;

	IEnumWbemClassObject *wbem_enumerator = NULL;	// 各種初期化
	wbem_services->ExecQuery(bstr_t("WQL"), bstr_t(service_name.c_str()), WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY, NULL, &wbem_enumerator);	// プロパティ読み込み
	IWbemClassObject *wbem_object = NULL;
	ULONG returned = 0;

	// Win32_Processor の情報を列挙する
	while (wbem_enumerator)	{
		wbem_enumerator->Next(WBEM_INFINITE, 1, &wbem_object, &returned);
		if (0 == returned)
			break;

		// プロパティの名前を設定する
		const BSTR property_name_b = (BSTR)property_name_t.c_str();

		// プロパティの値を取得する
		VARIANT property_value;
		_bstr_t display_name;
		VariantInit(&property_value);
		if (wbem_object->Get(property_name_b, 0, &property_value, 0, 0) == WBEM_S_NO_ERROR)	{

			if (property_value.vt == 1)	{			// NULL の場合
			}
			else if (property_value.vt == 8200)	{	// 配列の場合
			}
			else	{		// それら以外の場合は値を文字列として取得する
				display_name = property_value;
				result += display_name;
			}

			VariantClear(&property_value);
		}

		if (!result.empty())
			break;
	}
	wbem_object->Release();
	wbem_enumerator->Release();

	// 連続するスペースを省略する
	TSTR result_tmp = result;
	result.clear();
	TCHAR prev_moji = 0;
	for (TSTR::iterator itr = result_tmp.begin(); itr != result_tmp.end(); ++itr)	{
		if (prev_moji == _T(' ') && *itr == _T(' '))	{
		}
		else	{
			result.push_back(*itr);
		}
		prev_moji = *itr;
	}

	return result;
}

TSTR tnm_get_spec_info_for_chihaya_bench_sub()
{
#if 1

	IWbemLocator *wbem_locator = NULL;
	IWbemServices *wbem_services = NULL;
	CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, reinterpret_cast<LPVOID *>(&wbem_locator));	// 接続開始
	wbem_locator->ConnectServer(_bstr_t(L"\\\\.\\ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &wbem_services);
	CoSetProxyBlanket(wbem_services, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

	// Win32_Processor
	TSTR os_name = tnm_get_spec_info(wbem_services, _T("Win32_OperatingSystem"), _T("Caption"));
	TSTR os_version_name = tnm_get_spec_info(wbem_services, _T("Win32_OperatingSystem"), _T("CSDVersion"));
	TSTR cpu_name = tnm_get_spec_info(wbem_services, _T("Win32_Processor"), _T("Name"));

	wbem_services->Release();
	wbem_locator->Release();

	TSTR result = _T("OS = ") + os_name	+ (os_version_name.empty() ? _T("\n") : (_T(" (") + os_version_name + _T(")\n")))
		+ _T("CPU = ") + cpu_name;

	return result;

#else
	IWbemLocator *wbem_locator = NULL;
	IWbemServices *wbem_services = NULL;
	IEnumWbemClassObject *wbem_enumerator = NULL;	// 各種初期化
//	CoInitializeEx(0, COINIT_MULTITHREADED);
//	CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, reinterpret_cast<LPVOID *>(&wbem_locator));	// 接続開始
	wbem_locator->ConnectServer(_bstr_t(L"\\\\.\\ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &wbem_services);
	CoSetProxyBlanket(wbem_services, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	wbem_services->ExecQuery(bstr_t("WQL"), bstr_t("SELECT * FROM Win32_Processor"), WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY, NULL, &wbem_enumerator);	// プロパティ読み込み
	IWbemClassObject *wbem_object = NULL;
	ULONG returned = 0;

	// Win32_Processor の情報を列挙する
	while (wbem_enumerator)	{
		wbem_enumerator->Next(WBEM_INFINITE, 1, &wbem_object, &returned);
		if (0 == returned)
			break;

		TSTR result;

		// プロパティ一覧を取得する
		SAFEARRAY* p_property_name_list = NULL;
		if (wbem_object->GetNames(NULL, 0, NULL, &p_property_name_list) == WBEM_S_NO_ERROR)	{
			long lb = 0, ub = 0;
			BSTR property_name;

			// プロパティ一覧からプロパティを順に取得する
			SafeArrayGetLBound(p_property_name_list, 1, &lb);
			SafeArrayGetUBound(p_property_name_list, 1, &ub);
			for (long i = lb; i <= ub; i++)	{

				if (SafeArrayGetElement(p_property_name_list, &i, (void *)&property_name) == S_OK)	{

					// プロパティの名前を取得する
					VARIANT property_value;
					_bstr_t display_name;
					VariantInit(&property_value);
					if (wbem_object->Get(property_name, 0, &property_value, 0, 0) == WBEM_S_NO_ERROR)	{

						if (property_value.vt == 1)	{			// NULL の場合
							result += TSTR(property_name)+ _T(" : ") + _T("NULL\n");
						}
						else if (property_value.vt == 8200)	{	// 配列の場合
							result += TSTR(property_name)+ _T(" : ") + _T("配列\n");
						}
						else	{		// それら以外の場合は値を文字列として取得する
							display_name = property_value;
							result += TSTR(property_name)+ _T(" : ") + MBSTR_to_TSTR(static_cast<char *>(display_name)) + _T("\n");
						}

						VariantClear(&property_value);
					}
				}

				if (i % 20 == 19)	{
					info_box(result);
					result.clear();
				}
			}
			info_box(result);

			SafeArrayDestroy(p_property_name_list);
		}
	}
	wbem_object->Release();
	wbem_enumerator->Release();
	wbem_services->Release();
	wbem_locator->Release();
//	CoUninitialize();
	return 0;

#endif
}
#endif

// ****************************************************************
// スペック情報を取得する：OS
// ================================================================
//		WMI を使います。
// ================================================================
bool tnm_get_os_name(TSTR& name)
{
	bool ret = true;
	HRESULT hr;
	TSTR tmp_str;

	IWbemLocator *wbem_locator = NULL;
	IWbemServices *wbem_services = NULL;
	IEnumWbemClassObject *wbem_enumerator = NULL;	// 各種初期化

	// 接続開始
	hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, reinterpret_cast<LPVOID *>(&wbem_locator));
	if (FAILED(hr) || wbem_locator == NULL)	{
		set_last_error_hr(hr, _T("CoCreateInstance"));		// HRESULT エラー
		ret = false;
	}
	else	{

		hr = wbem_locator->ConnectServer(_bstr_t(L"\\\\.\\ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &wbem_services);
		if (FAILED(hr) || wbem_services == NULL)	{
			set_last_error_wbem(hr, _T("IWbemLocator::ConnectServer"));		// WBEM エラー
			ret = false;
		}
		else	{

			hr = CoSetProxyBlanket(wbem_services, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
			if (FAILED(hr))	{
				set_last_error_hr(hr, _T("CoSetProxyBlanket"));		// HRESULT エラー
				ret = false;
			}
			else	{

				hr = wbem_services->ExecQuery(bstr_t("WQL"), bstr_t("SELECT * FROM Win32_OperatingSystem"), WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY, NULL, &wbem_enumerator);
				if (FAILED(hr) || wbem_enumerator == NULL)	{
					set_last_error_wbem(hr, _T("IWbemServices::ExecQuery"));		// WBEM エラー
					ret = false;
				}
				else	{

					// Win32_Processor の情報を列挙する
					while (wbem_enumerator)	{
						IWbemClassObject *wbem_object = NULL;
						ULONG returned = 0;

						// 列挙子からオブジェクトを取得
						wbem_enumerator->Next(WBEM_INFINITE, 1, &wbem_object, &returned);
						if (0 == returned)
							break;

						// プロパティの文字列をを取得
						tmp_str += tnm_wmi_get_object(wbem_object, _T("Caption"));

						TSTR service_pack = tnm_wmi_get_object(wbem_object, _T("CSDVersion"));
						if (!service_pack.empty())
							tmp_str += _T(" ") + service_pack;

						TSTR architecture = tnm_wmi_get_object(wbem_object, _T("OSArchitecture"));
						if (!architecture.empty())
							tmp_str += _T(" ") + architecture;

						// オブジェクトを解放
						wbem_object->Release();
					}

					// Unicode にしか存在しない (R)、(TM) を文字列に直す
					// こいつらは Shift-JIS にしたときにうまく表示されないのです。
					// Vista のみ OS 名にこれらの文字がつきます。7 だとつきません。
					for (int i = 0; i < (int)tmp_str.size(); i++)	{
						if (false);
						else if (tmp_str[i] == 0x2122)	{	// (TM)
							name += _T("(TM)");
						}
						else if (tmp_str[i] == 0x00AE)	{	// (R)
							name += _T("(R)");
						}
						else	{
							name += tmp_str[i];
						}
					}

					wbem_enumerator->Release();
				}
			}

			wbem_services->Release();
		}

		wbem_locator->Release();
	}

	return ret;
}

// ****************************************************************
// スペック情報を取得する：CPU 名
// ================================================================
//		こっちを採用します。
// ================================================================
TSTR tnm_erase_continuous_space_ret(TSTR str)
{
	TCHAR last_moji = 0;
	for (int i = 0; i < (int)str.size();)	{
		if (last_moji == _T(' ') && str[i] == _T(' '))	{
			str.erase(str.begin() + i);
		}
		else	{
			last_moji = str[i];
			i++;
		}
	}

	return str;
}

TSTR tnm_get_cpu_name()
{
	// レジストリキーを調べます。
	TSTR key_name = _T("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0");
	TSTR value_name = _T("ProcessorNameString");
	TSTR value_str;
	reg_get_value_str(HKEY_LOCAL_MACHINE, key_name, value_name, value_str);
	if (value_str.empty())
		return _T("不明な CPU");

	// 連続したスペースを省く
	value_str = tnm_erase_continuous_space_ret(value_str);

	return value_str;
}

// ****************************************************************
// デバイスの状態をチェックする
// ================================================================
bool tnm_check_device()
{
	// デバイスが消失している場合
	if (G_d3d.device.is_lost())	{
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("★Direct3D デバイスが消失しました。！"));

		// E-mote を解放します。
#if __USE_EMOTE
		tnm_emote_on_device_lost();
#endif

		// Direct3D エフェクトを一時解放します。
		if (!Gp_wnd->m_view.m_default_effect.on_lost_device())	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("エフェクトの解放に失敗しました。\n") + get_last_error_msg());
			return false;
		}

		// Direct3D テキストを一時解放します。
		if (!G_debug_text.on_lost_device())	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("テキストの解放に失敗しました。\n") + get_last_error_msg());
			return false;
		}
	
		// リソースを一時解放します。
		G_rsm.release();

		// デバイスが復元するまで繰り返す
		while (1)	{

			// デバイスを復元します。
			if (!G_d3d.device.restore())
				throw _T("デバイスの復元に失敗しました。\n") + get_last_error_msg();		// 致命的エラー！

			// デバイスが消失している場合は再度繰り返します。
			if (G_d3d.device.is_lost())	{
				Sleep(100);
				continue;
			}

			// 成功
			break;
		}

		// ステートを初期化します。
		G_d3d.state.init();

		// ステートを初期設定を行います。
		if (!G_d3d.state.set_template())
			return false;

		// リソースを復元します。
		if (!G_rsm.resume())
			return false;

		// Direct3D テキストを復元します。
		if (!G_debug_text.on_reset_device())	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("テキストの復元に失敗しました。\n") + get_last_error_msg());
			return false;
		}

		// Direct3D エフェクトを復元します。
		if (!Gp_wnd->m_view.m_default_effect.on_reset_device())	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("エフェクトの復元に失敗しました。\n") + get_last_error_msg());
			return false;
		}

		// 警告
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("★Direct3D デバイスを復元しました。"));
	}

	return true;
}
