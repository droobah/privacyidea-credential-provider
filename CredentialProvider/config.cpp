#include "config.h"
#include "githash.h"

namespace Configuration
{

	CONFIGURATION*& Get()
	{
		static struct CONFIGURATION *conf = NULL;

		return conf;
	}

	void Default()
	{
		struct CONFIGURATION*& conf = Get();

		ZERO(conf->hostname);
		ZERO(conf->path);
		ZERO(conf->login_text);
		ZERO(conf->otp_text);
		ZERO(conf->v1_bitmap_path);
		ZERO(conf->v2_bitmap_path);

		// DEFAULT IS SAFE MODE, NO ERRORS WILL BE IGNORED 
		conf->ssl_ignore_unknown_ca = 0;
		conf->ssl_ignore_invalid_cn = 0;

		conf->custom_port = 0;
		conf->hide_domainname = 0;
		conf->hide_fullname = 0;

		conf->two_step_hide_otp = 0;
		conf->two_step_send_empty_password = 0;
		conf->two_step_send_password = 0;

		conf->release_log = 0;

		conf->win_ver_major = 0;
		conf->win_ver_minor = 0;

		conf->hide_otp_sleep_s = 0;
	}

	void Init()
	{
		DebugPrintLn(__FUNCTION__);

		struct CONFIGURATION*& conf = Get();

		conf = (struct CONFIGURATION*) malloc(sizeof(struct CONFIGURATION));

		Default();
	}

	void Deinit()
	{
		DebugPrintLn(__FUNCTION__);

		struct CONFIGURATION*& conf = Get();

		Default();

		free(conf);
		conf = NULL;
	}

	///////////////////// SPECIFIC CONFIGURATION

	void Read()
	{
		DebugPrintLn(__FUNCTION__);

		struct CONFIGURATION*& conf = Get();

		char buffer[2];

		// Read config
		readRegistryValueString(CONF_HOSTNAME, sizeof(conf->hostname), conf->hostname);

		if (readRegistryValueString(CONF_PATH, sizeof(conf->path), conf->path) <= 1) // 1 = size of a char NULL-terminator in byte
			strcpy_s(conf->path, sizeof(conf->path), CONFIG_DEFAULT_EMPTY_PATH);

		if (readRegistryValueString(CONF_LOGIN_TEXT, sizeof(conf->login_text), conf->login_text) <= 1) // 1 = size of a char NULL-terminator in byte
			strcpy_s(conf->login_text, sizeof(conf->login_text), CONFIG_DEFAULT_LOGIN_TEXT);

		if (readRegistryValueString(CONF_OTP_TEXT, sizeof(conf->otp_text), conf->otp_text) <= 1) // 1 = size of a char NULL-terminator in byte
			strcpy_s(conf->otp_text, sizeof(conf->otp_text), CONFIG_DEFAULT_OTP_TEXT);

		readRegistryValueString(CONF_V1_BITMAP_PATH, sizeof(conf->v1_bitmap_path), conf->v1_bitmap_path);
		readRegistryValueString(CONF_V2_BITMAP_PATH, sizeof(conf->v2_bitmap_path), conf->v2_bitmap_path);

		// Check for custom port
		if (readRegistryValueString(CONF_CUSTOM_PORT, sizeof(buffer), buffer) <= 1) // 1 = size of a char NULL-terminator in byte
			conf->custom_port = 0; // if NULL
		else
		{
			conf->custom_port = buffer[0] - 0x30;
		}

		wstring customPort = getRegistry(L"custom_port");
		customPort.empty() ? conf->custom_port = 0 : conf->custom_port = stoi(customPort.c_str());

		/*if (customPort.empty())
		{
			conf->custom_port = 0;
		}
		else
		{
			conf->custom
		} */

		// HIDE TWO STEP OTP
		if (readRegistryValueString(CONF_TWO_STEP_HIDE_OTP, sizeof(buffer), buffer) <= 1) // 1 = size of a char NULL-terminator in byte
			conf->two_step_hide_otp = 0; // if NULL
		else
		{
			conf->two_step_hide_otp = buffer[0] - 0x30;
		}

		// SEND DOMAIN PASSWORD TWO STEP
		if (readRegistryValueString(CONF_TWO_STEP_SEND_PASSWORD, sizeof(buffer), buffer) <= 1) // 1 = size of a char NULL-terminator in byte
			conf->two_step_send_password = 0; // if NULL
		else
		{
			conf->two_step_send_password = buffer[0] - 0x30;
		}

		// SEND EMPTY PASSWORD TWO STEP
		if (readRegistryValueString(CONF_TWO_STEP_SEND_EMPTY_PASSWORD, sizeof(buffer), buffer) <= 1) // 1 = size of a char NULL-terminator in byte
			conf->two_step_send_empty_password = 0; // if NULL
		else
		{
			conf->two_step_send_empty_password = buffer[0] - 0x30;
		}

		// SSL IGNORE UNKNOWN CA
		if (readRegistryValueString(CONF_SSL_IGNORE_UNKNOWN_CA, sizeof(buffer), buffer) <= 1) // 1 = size of a char NULL-terminator in byte
			conf->ssl_ignore_unknown_ca = 0; // if NULL
		else
		{
			conf->ssl_ignore_unknown_ca = buffer[0] - 0x30;
		}

		// SSL IGNORE INVALID CN
		if (readRegistryValueString(CONF_SSL_IGNORE_INVALID_CN, sizeof(buffer), buffer) <= 1) // 1 = size of a char NULL-terminator in byte
			conf->ssl_ignore_invalid_cn = 0; // if NULL
		else
		{
			conf->ssl_ignore_invalid_cn = buffer[0] - 0x30;
		}

		// HIDE USERNAME
		if (readRegistryValueString(CONF_HIDE_FULLNAME, sizeof(buffer), buffer) <= 1) // 1 = size of a char NULL-terminator in byte
			conf->hide_fullname = 0; // if NULL
		else
		{
			conf->hide_fullname = buffer[0] - 0x30;
		}

		// HIDE DOMAIN NAME
		if (readRegistryValueString(CONF_HIDE_DOMAINNAME, sizeof(buffer), buffer) <= 1) // 1 = size of a char NULL-terminator in byte
			conf->hide_domainname = 0; // if NULL
		else
		{
			conf->hide_domainname = buffer[0] - 0x30;
		}

		// RELEASE LOG
		if (readRegistryValueString(CONF_RELEASE_LOG, sizeof(buffer), buffer) <= 1) // 1 = size of a char NULL-terminator in byte
			conf->release_log = 0; // if NULL
		else
		{
			conf->release_log = buffer[0] - 0x30;
		}

		// SENSITIVE DATA LOGGING (ONLY POSSIBLE IN DEBUG)
		if (readRegistryValueString(CONF_LOG_SENSITIVE, sizeof(buffer), buffer) <= 1) // 1 = size of a char NULL-terminator in byte
			conf->log_sensitive = 0; // if NULL
		else
		{
			conf->log_sensitive = buffer[0] - 0x30;
		}

		// NO DEFAULT
		if (readRegistryValueString(CONF_NO_DEFAULT, sizeof(buffer), buffer) <= 1) // 1 = size of a char NULL-terminator in byte
			conf->no_default = 0; // if NULL
		else
		{
			conf->no_default = buffer[0] - 0x30;
		}

		wstring sleeps = getRegistry(L"sleep");
		sleeps.empty() ? conf->hide_otp_sleep_s = 0 : conf->hide_otp_sleep_s = stoi(sleeps.c_str());

		////////////// READ END //////////////

		// check if both hides are 1. If so, set to hide full name
		if (conf->hide_domainname && conf->hide_fullname)
			conf->hide_domainname = 0;

		// check if both sending passwords are 1. If so, set to send empty password
		if (conf->two_step_send_empty_password && conf->two_step_send_password)
			conf->two_step_send_password = 0;

		// Read and save the windows version
		OSVERSIONINFOEX info;
		ZeroMemory(&info, sizeof(OSVERSIONINFOEX));
		info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		GetVersionEx((LPOSVERSIONINFO)&info);
		conf->win_ver_major = info.dwMajorVersion;
		conf->win_ver_minor = info.dwMinorVersion;
	}

	wstring Configuration::getRegistry(wstring name)
	{
		DWORD dwRet;
		HKEY hKey;

		dwRet = RegOpenKeyEx(
			HKEY_LOCAL_MACHINE,
			_T("SOFTWARE\\Netknights GmbH\\PrivacyIDEA-CP"),
			NULL,
			KEY_QUERY_VALUE,
			&hKey);
		if (dwRet != ERROR_SUCCESS)
		{
			return L"";
		}

		const DWORD SIZE = 1024;
		TCHAR szValue[SIZE] = _T("");
		DWORD dwValue = SIZE;
		DWORD dwType = 0;
		dwRet = RegQueryValueEx(
			hKey,
			name.c_str(),
			NULL,
			&dwType,
			(LPBYTE)&szValue,
			&dwValue);
		if (dwRet != ERROR_SUCCESS)
		{
			return L"";
		}

		if (dwType != REG_SZ)
		{
			return L"";
		}
		RegCloseKey(hKey);
		hKey = NULL;
		return wstring(szValue);
	}

	void PrintConfig() {
		// Log the current config
		DebugPrintLn("--- CP Version ---");
		DebugPrintLn(VER_FILE_VERSION_STR);
		DebugPrintLn(GIT_HASH);
		DebugPrintLn("Windows Version Major:"); DebugPrintLn(Get()->win_ver_major);
		DebugPrintLn("Minor:"); DebugPrintLn(Get()->win_ver_minor);
		DebugPrintLn("CONFIG LOADED SUCCESSFULLY:");
		DebugPrintLn("Hostname:"); DebugPrintLn(Get()->hostname);
		DebugPrintLn("Path:"); DebugPrintLn(Get()->path);
		DebugPrintLn("Custom port:"); DebugPrintLn(Get()->custom_port);
		DebugPrintLn("Login text:"); DebugPrintLn(Get()->login_text);
		DebugPrintLn("OTP text:");	DebugPrintLn(Get()->otp_text);
		DebugPrintLn("Hide only domain:"); DebugPrintLn(Get()->hide_domainname);
		DebugPrintLn("Hide full name:"); DebugPrintLn(Get()->hide_fullname);
		DebugPrintLn("SSL ignore invalid CN:");	DebugPrintLn(Get()->ssl_ignore_invalid_cn);
		DebugPrintLn("SSL ignore unknown CA:");	DebugPrintLn(Get()->ssl_ignore_unknown_ca);
		DebugPrintLn("2Step hide OTP:"); DebugPrintLn(Get()->two_step_hide_otp);
		DebugPrintLn("2Step send domain PW"); DebugPrintLn(Get()->two_step_send_password);
		DebugPrintLn("2Step send empty PW"); DebugPrintLn(Get()->two_step_send_empty_password);
		DebugPrintLn("Release Version Log:"); DebugPrintLn(Get()->release_log);
		DebugPrintLn("No default:"); DebugPrintLn(Get()->no_default);
	}

	DWORD SaveValueString(CONF_VALUE conf_value, char* value, int size)
	{
		DebugPrintLn(__FUNCTION__);

		return writeRegistryValueString(conf_value, value, size);
	}

	DWORD SaveValueInteger(CONF_VALUE conf_value, int value)
	{
		DebugPrintLn(__FUNCTION__);

		return writeRegistryValueInteger(conf_value, value);
	}

} // Namespace Configuration