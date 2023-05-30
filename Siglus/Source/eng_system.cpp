#include	"pch.h"
#include	"ifc_eng.h"
#include	"data/tnm_ini.h"
#include	"data/tnm_wnd.h"
#include	"engine/eng_graphics.h"
#include	"engine/ifc_error.h"
#include	"engine/eng_emote.h"

// ****************************************************************
// �f�B�X�N�h���C�u����������
// ================================================================
TCHAR tnm_search_disc_drive()
{
	int res = 0;

	// �������d�v������
	// �f�B�X�N�h���C�u���������邽�߁A�ꎞ�I�ɃG���[���������
	// �ꕔ�̊��ɂ����āA�t�@�C����T���ɍs�������Łu�f�B�X�N������v���Čx�����o��̂ł��B
	int error_mode = SetErrorMode(SEM_FAILCRITICALERRORS);

	// �h���C�u�𒲂ׂ�
	for (TCHAR letter = _T('A'); letter <= _T('Z'); letter ++)	{
		TSTR drive_path = TSTR() + letter + _T(":\\");
		if (::GetDriveType(drive_path.c_str()) == DRIVE_CDROM)	{
			TSTR env_path = drive_path + Gp_ini->disc_mark;
			if (file_get_type(env_path) == 1)	{
				res = letter;	// �h���C�u���^�[����
				break;
			}
		}
	}

	// �������d�v������
	// �G���[�����̐ݒ�����ɖ߂��܂��B����Y�ꂽ�炦�炢���Ƃ�ȁc�B
	SetErrorMode(error_mode);

	return res;
}

// ****************************************************************
// wmi ����
// ================================================================
#include	<Wbemidl.h>
#include	<comdef.h>

#pragma comment(lib, "wbemuuid.lib")

// ****************************************************************
// wmi�F�I�u�W�F�N�g���擾����
// ================================================================
TSTR tnm_wmi_get_object(IWbemClassObject* wbem_object, CTSTR& property_name_t)
{
	TSTR result;

	// �v���p�e�B�̖��O��ݒ肷��
	const BSTR property_name_b = (BSTR)property_name_t.c_str();

	// �v���p�e�B�̒l���擾����
	VARIANT property_value;
	_bstr_t display_name;
	VariantInit(&property_value);
	if (wbem_object->Get(property_name_b, 0, &property_value, 0, 0) == WBEM_S_NO_ERROR)	{

		if (property_value.vt == 1)	{			// NULL �̏ꍇ
		}
		else if (property_value.vt == 8200)	{	// �z��̏ꍇ
		}
		else	{		// �����ȊO�̏ꍇ�͒l�𕶎���Ƃ��Ď擾����
			display_name = property_value;
			result += display_name;
		}

		VariantClear(&property_value);
	}

	return result;
}

// ****************************************************************
// �X�y�b�N�����擾����FCPU ���FWMI �o�[�W����
// ================================================================
// ��WMI �𗘗p���� CPU �����擾���郋�[�`��
// ���ꕔ���iXP�ACore2 (Walf�Ȃ񂿂��j�j�Ő���Ɏ擾�ł��܂���ł����I
// KB953955 �𓖂Ă�ƏC������܂��B���[�U�ɓ��ĂĂ��炤�킯�ɂ͂����Ȃ��̂Ń{�c�ł��B
// Windows Update �ōX�V���Ă��ꂽ�炢���̂Ɂc�B

#if 0
TSTR tnm_get_spec_info(IWbemServices* wbem_services, TSTR service_name, TSTR property_name_t)
{
	TSTR result;

	service_name = _T("SELECT * FROM ") + service_name;

	IEnumWbemClassObject *wbem_enumerator = NULL;	// �e�평����
	wbem_services->ExecQuery(bstr_t("WQL"), bstr_t(service_name.c_str()), WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY, NULL, &wbem_enumerator);	// �v���p�e�B�ǂݍ���
	IWbemClassObject *wbem_object = NULL;
	ULONG returned = 0;

	// Win32_Processor �̏���񋓂���
	while (wbem_enumerator)	{
		wbem_enumerator->Next(WBEM_INFINITE, 1, &wbem_object, &returned);
		if (0 == returned)
			break;

		// �v���p�e�B�̖��O��ݒ肷��
		const BSTR property_name_b = (BSTR)property_name_t.c_str();

		// �v���p�e�B�̒l���擾����
		VARIANT property_value;
		_bstr_t display_name;
		VariantInit(&property_value);
		if (wbem_object->Get(property_name_b, 0, &property_value, 0, 0) == WBEM_S_NO_ERROR)	{

			if (property_value.vt == 1)	{			// NULL �̏ꍇ
			}
			else if (property_value.vt == 8200)	{	// �z��̏ꍇ
			}
			else	{		// �����ȊO�̏ꍇ�͒l�𕶎���Ƃ��Ď擾����
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

	// �A������X�y�[�X���ȗ�����
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
	CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, reinterpret_cast<LPVOID *>(&wbem_locator));	// �ڑ��J�n
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
	IEnumWbemClassObject *wbem_enumerator = NULL;	// �e�평����
//	CoInitializeEx(0, COINIT_MULTITHREADED);
//	CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, reinterpret_cast<LPVOID *>(&wbem_locator));	// �ڑ��J�n
	wbem_locator->ConnectServer(_bstr_t(L"\\\\.\\ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &wbem_services);
	CoSetProxyBlanket(wbem_services, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	wbem_services->ExecQuery(bstr_t("WQL"), bstr_t("SELECT * FROM Win32_Processor"), WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY, NULL, &wbem_enumerator);	// �v���p�e�B�ǂݍ���
	IWbemClassObject *wbem_object = NULL;
	ULONG returned = 0;

	// Win32_Processor �̏���񋓂���
	while (wbem_enumerator)	{
		wbem_enumerator->Next(WBEM_INFINITE, 1, &wbem_object, &returned);
		if (0 == returned)
			break;

		TSTR result;

		// �v���p�e�B�ꗗ���擾����
		SAFEARRAY* p_property_name_list = NULL;
		if (wbem_object->GetNames(NULL, 0, NULL, &p_property_name_list) == WBEM_S_NO_ERROR)	{
			long lb = 0, ub = 0;
			BSTR property_name;

			// �v���p�e�B�ꗗ����v���p�e�B�����Ɏ擾����
			SafeArrayGetLBound(p_property_name_list, 1, &lb);
			SafeArrayGetUBound(p_property_name_list, 1, &ub);
			for (long i = lb; i <= ub; i++)	{

				if (SafeArrayGetElement(p_property_name_list, &i, (void *)&property_name) == S_OK)	{

					// �v���p�e�B�̖��O���擾����
					VARIANT property_value;
					_bstr_t display_name;
					VariantInit(&property_value);
					if (wbem_object->Get(property_name, 0, &property_value, 0, 0) == WBEM_S_NO_ERROR)	{

						if (property_value.vt == 1)	{			// NULL �̏ꍇ
							result += TSTR(property_name)+ _T(" : ") + _T("NULL\n");
						}
						else if (property_value.vt == 8200)	{	// �z��̏ꍇ
							result += TSTR(property_name)+ _T(" : ") + _T("�z��\n");
						}
						else	{		// �����ȊO�̏ꍇ�͒l�𕶎���Ƃ��Ď擾����
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
// �X�y�b�N�����擾����FOS
// ================================================================
//		WMI ���g���܂��B
// ================================================================
bool tnm_get_os_name(TSTR& name)
{
	bool ret = true;
	HRESULT hr;
	TSTR tmp_str;

	IWbemLocator *wbem_locator = NULL;
	IWbemServices *wbem_services = NULL;
	IEnumWbemClassObject *wbem_enumerator = NULL;	// �e�평����

	// �ڑ��J�n
	hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, reinterpret_cast<LPVOID *>(&wbem_locator));
	if (FAILED(hr) || wbem_locator == NULL)	{
		set_last_error_hr(hr, _T("CoCreateInstance"));		// HRESULT �G���[
		ret = false;
	}
	else	{

		hr = wbem_locator->ConnectServer(_bstr_t(L"\\\\.\\ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &wbem_services);
		if (FAILED(hr) || wbem_services == NULL)	{
			set_last_error_wbem(hr, _T("IWbemLocator::ConnectServer"));		// WBEM �G���[
			ret = false;
		}
		else	{

			hr = CoSetProxyBlanket(wbem_services, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
			if (FAILED(hr))	{
				set_last_error_hr(hr, _T("CoSetProxyBlanket"));		// HRESULT �G���[
				ret = false;
			}
			else	{

				hr = wbem_services->ExecQuery(bstr_t("WQL"), bstr_t("SELECT * FROM Win32_OperatingSystem"), WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY, NULL, &wbem_enumerator);
				if (FAILED(hr) || wbem_enumerator == NULL)	{
					set_last_error_wbem(hr, _T("IWbemServices::ExecQuery"));		// WBEM �G���[
					ret = false;
				}
				else	{

					// Win32_Processor �̏���񋓂���
					while (wbem_enumerator)	{
						IWbemClassObject *wbem_object = NULL;
						ULONG returned = 0;

						// �񋓎q����I�u�W�F�N�g���擾
						wbem_enumerator->Next(WBEM_INFINITE, 1, &wbem_object, &returned);
						if (0 == returned)
							break;

						// �v���p�e�B�̕���������擾
						tmp_str += tnm_wmi_get_object(wbem_object, _T("Caption"));

						TSTR service_pack = tnm_wmi_get_object(wbem_object, _T("CSDVersion"));
						if (!service_pack.empty())
							tmp_str += _T(" ") + service_pack;

						TSTR architecture = tnm_wmi_get_object(wbem_object, _T("OSArchitecture"));
						if (!architecture.empty())
							tmp_str += _T(" ") + architecture;

						// �I�u�W�F�N�g�����
						wbem_object->Release();
					}

					// Unicode �ɂ������݂��Ȃ� (R)�A(TM) �𕶎���ɒ���
					// ������� Shift-JIS �ɂ����Ƃ��ɂ��܂��\������Ȃ��̂ł��B
					// Vista �̂� OS ���ɂ����̕��������܂��B7 ���Ƃ��܂���B
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
// �X�y�b�N�����擾����FCPU ��
// ================================================================
//		���������̗p���܂��B
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
	// ���W�X�g���L�[�𒲂ׂ܂��B
	TSTR key_name = _T("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0");
	TSTR value_name = _T("ProcessorNameString");
	TSTR value_str;
	reg_get_value_str(HKEY_LOCAL_MACHINE, key_name, value_name, value_str);
	if (value_str.empty())
		return _T("�s���� CPU");

	// �A�������X�y�[�X���Ȃ�
	value_str = tnm_erase_continuous_space_ret(value_str);

	return value_str;
}

// ****************************************************************
// �f�o�C�X�̏�Ԃ��`�F�b�N����
// ================================================================
bool tnm_check_device()
{
	// �f�o�C�X���������Ă���ꍇ
	if (G_d3d.device.is_lost())	{
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("��Direct3D �f�o�C�X���������܂����B�I"));

		// E-mote ��������܂��B
#if __USE_EMOTE
		tnm_emote_on_device_lost();
#endif

		// Direct3D �G�t�F�N�g���ꎞ������܂��B
		if (!Gp_wnd->m_view.m_default_effect.on_lost_device())	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�G�t�F�N�g�̉���Ɏ��s���܂����B\n") + get_last_error_msg());
			return false;
		}

		// Direct3D �e�L�X�g���ꎞ������܂��B
		if (!G_debug_text.on_lost_device())	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�e�L�X�g�̉���Ɏ��s���܂����B\n") + get_last_error_msg());
			return false;
		}
	
		// ���\�[�X���ꎞ������܂��B
		G_rsm.release();

		// �f�o�C�X����������܂ŌJ��Ԃ�
		while (1)	{

			// �f�o�C�X�𕜌����܂��B
			if (!G_d3d.device.restore())
				throw _T("�f�o�C�X�̕����Ɏ��s���܂����B\n") + get_last_error_msg();		// �v���I�G���[�I

			// �f�o�C�X���������Ă���ꍇ�͍ēx�J��Ԃ��܂��B
			if (G_d3d.device.is_lost())	{
				Sleep(100);
				continue;
			}

			// ����
			break;
		}

		// �X�e�[�g�����������܂��B
		G_d3d.state.init();

		// �X�e�[�g�������ݒ���s���܂��B
		if (!G_d3d.state.set_template())
			return false;

		// ���\�[�X�𕜌����܂��B
		if (!G_rsm.resume())
			return false;

		// Direct3D �e�L�X�g�𕜌����܂��B
		if (!G_debug_text.on_reset_device())	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�e�L�X�g�̕����Ɏ��s���܂����B\n") + get_last_error_msg());
			return false;
		}

		// Direct3D �G�t�F�N�g�𕜌����܂��B
		if (!Gp_wnd->m_view.m_default_effect.on_reset_device())	{
			tnm_set_error(TNM_ERROR_TYPE_FATAL, _T("�G�t�F�N�g�̕����Ɏ��s���܂����B\n") + get_last_error_msg());
			return false;
		}

		// �x��
		tnm_set_error(TNM_ERROR_TYPE_SUCCESS, _T("��Direct3D �f�o�C�X�𕜌����܂����B"));
	}

	return true;
}
