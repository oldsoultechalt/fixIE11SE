#include <windows.h>
#include <stdio.h>
#include <tchar.h>

BOOL SetIE11RedirectionRegistry()
{
    HKEY hKey;
    DWORD value = 1;
    BOOL success = TRUE;

    // Write to HKLM
    if (RegCreateKeyEx(HKEY_LOCAL_MACHINE,
        TEXT("SOFTWARE\\Policies\\Microsoft\\Internet Explorer\\Main"),
        0, NULL, 0, KEY_SET_VALUE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        RegSetValueEx(hKey, TEXT("NotifyDisableIEOptions"), 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        RegSetValueEx(hKey, TEXT("DisableIEEdgeRedirection"), 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        RegCloseKey(hKey);
    } else {
        success = FALSE;
    }

    // Write to HKCU
    if (RegCreateKeyEx(HKEY_CURRENT_USER,
        TEXT("Software\\Microsoft\\Internet Explorer\\Main"),
        0, NULL, 0, KEY_SET_VALUE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        RegSetValueEx(hKey, TEXT("NotifyDisableIEOptions"), 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        RegSetValueEx(hKey, TEXT("DisableIEEdgeRedirection"), 0, REG_DWORD, (BYTE*)&value, sizeof(value));
        RegCloseKey(hKey);
    } else {
        success = FALSE;
    }

    return success;
}

BOOL IsWindows11OrNewer()
{
    HKEY hKey;
    TCHAR productName[256];
    DWORD size = sizeof(productName);
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
        TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"),
        0, KEY_READ, &hKey) != ERROR_SUCCESS)
        return FALSE;

    if (RegQueryValueEx(hKey, TEXT("ProductName"), NULL, NULL, (LPBYTE)productName, &size) != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return FALSE;
    }

    RegCloseKey(hKey);

    // Basic check
    if (_tcsstr(productName, TEXT("Windows 11")))
        return TRUE;
    return FALSE;
}

int main()
{
    MessageBox(NULL, TEXT("IE11 Launcher Tool with Anti-Redirection Fix"), TEXT("IE11 Tool"), MB_OK | MB_ICONINFORMATION);

    if (IsWindows11OrNewer()) {
        MessageBox(NULL,
            TEXT("You're running Windows 11.\n\nIE11 is fully removed in most builds.\n")
            TEXT("Workarounds:\n• Use Edge IE Mode\n• Use a Windows 10 VM\n\nThis tool may not work, but will attempt to re-enable legacy support."),
            TEXT("Windows 11 Detected"), MB_OK | MB_ICONWARNING);
    } else {
        MessageBox(NULL,
            TEXT("Windows 10 detected.\nAttempting to disable Edge redirection."),
            TEXT("Info"), MB_OK | MB_ICONINFORMATION);
    }

    if (SetIE11RedirectionRegistry()) {
        MessageBox(NULL, TEXT("Redirection registry values set.\nTrying to launch IE11..."), TEXT("Success"), MB_OK | MB_ICONINFORMATION);
        ShellExecute(NULL, TEXT("open"), TEXT("C:\\Program Files\\Internet Explorer\\iexplore.exe"), NULL, NULL, SW_SHOWNORMAL);
    } else {
        MessageBox(NULL, TEXT("Failed to set registry keys. Try running as Administrator."), TEXT("Error"), MB_OK | MB_ICONERROR);
    }

    return 0;
}
