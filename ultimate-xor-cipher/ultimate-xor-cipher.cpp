// ultimate-xor-cipher.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "ultimate-xor-cipher.h"
#include "CommDlg.h"
#include "stdio.h"

#define MAX_LOADSTRING  100
#define BUFFERSIZE      5
#define KEYSIZE         262144

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hWindows;                                  // the biggest window

HWND hwndTextBox;
HWND hwndButton;
HWND hwndBrowser;
HWND hwndStatus;
WCHAR szInputFile[MAX_PATH];
WCHAR szKeyPath[MAX_PATH] = { 0 };
WCHAR szCurrKeyFullName[MAX_PATH] = { 0 };
WCHAR szRemainKeyPromt[512] = { 0 };
DWORD dwCurrMaxKeyCode;
DWORD dwMaxKeyCode;
DWORD dwMinKeyCode;
DWORD dwRemainKeyInMB;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
// INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


BOOL GetFileInput();
BOOL Processor();
BOOL InitKey();


// mini function
BOOL UpdateRemainKey();
BOOL GetDirPathFromFullName(WCHAR* out, WCHAR* in);
BOOL GetNameFromFullName(WCHAR* out, WCHAR* in);
BOOL GetExtFromFullName(WCHAR* in, WCHAR* out);
BOOL RemoveExtFromFullName(WCHAR* in, WCHAR* out);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    if (!InitKey())
        return 1;

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ULTIMATEXORCIPHER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ULTIMATEXORCIPHER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ULTIMATEXORCIPHER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ULTIMATEXORCIPHER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWindows = CreateWindowExW(WS_EX_ACCEPTFILES,szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 550, 200, nullptr, nullptr, hInstance, nullptr);

   if (!hWindows)
   {
      return FALSE;
   }

   ShowWindow(hWindows, nCmdShow);
   UpdateWindow(hWindows);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
		    hwndTextBox = CreateWindow(L"Edit",
			    L"",
			    WS_BORDER | WS_CHILD | WS_VISIBLE,
			    10, 40, 400, 20,
			    hWnd,
			    NULL,
			    NULL,
			    NULL);
            
            hwndButton = CreateWindow(L"Button",
                L"Thực thi",
                WS_BORDER | WS_CHILD | WS_VISIBLE,
                210, 70, 100, 20,
                hWnd,
                (HMENU) 9999,
                NULL,
                NULL);

            hwndBrowser = CreateWindow(L"Button",
                L"Tìm kiếm",
                WS_BORDER | WS_CHILD | WS_VISIBLE,
                420, 40, 100, 20,
                hWnd,
                (HMENU)9998,
                NULL,
                NULL);
            hwndStatus = CreateWindow(L"Static", 
                L"",
                WS_CHILD | WS_VISIBLE,
                10, 0, 250, 20,
                hWnd,
                (HMENU)9997,
                NULL,
                NULL);
            UpdateRemainKey();
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case 9999:
                memset(szInputFile, 0, MAX_PATH * sizeof(WCHAR));
                GetWindowText(hwndTextBox, szInputFile, MAX_PATH);
                if (szInputFile[0] == 0)
                {
                    GetFileInput();
                    break;
                }
                if (Processor() == FALSE)
                    break;
                break;
            case 9998:
                GetFileInput();
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    // Get only 1 file and check extension
    case WM_DROPFILES:
    {
        HDROP hDropInfo = (HDROP)wParam;
        WCHAR sItem[MAX_PATH];
        UINT uNumOfFile = DragQueryFile(hDropInfo, 0xFFFFFFFF, (LPWSTR)sItem, sizeof(sItem));
        if (uNumOfFile != 1)
        {
            MessageBox(NULL, L"01 file only", L"Program", MB_ICONWARNING);
            break;
        }
        else
        {
            DragQueryFile(hDropInfo, 0, (LPWSTR)sItem, sizeof(sItem));
            if (GetFileAttributes(sItem) & FILE_ATTRIBUTE_DIRECTORY)
                break;
            else
                SetWindowText(hwndTextBox, sItem);
        }
        DragFinish(hDropInfo);
    }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    UNREFERENCED_PARAMETER(lParam);
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        return (INT_PTR)TRUE;
//
//    case WM_COMMAND:
//        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//        {
//            EndDialog(hDlg, LOWORD(wParam));
//            return (INT_PTR)TRUE;
//        }
//        break;
//    }
//    return (INT_PTR)FALSE;
//}

BOOL GetFileInput()
{
    OPENFILENAME ofn;       // common dialog box structure
    TCHAR szFile[260] = { 0 };       // if using TCHAR macros

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWindows;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = _T("All\0*.*");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE)
    {
        SetWindowText(hwndTextBox, ofn.lpstrFile);
        return TRUE;
    }
    return FALSE;
}

BOOL Processor() {
    char buff[4096];
    char buff_key[4096];
    DWORD dwBytesRead, dwBytesWritten, dwKeyBytesRead;
    DWORD ibuffCount = 0;
    DWORD dwCurrKeyCode;

    WCHAR szTempString[MAX_PATH]        = { 0 };
    WCHAR szOutputFullName[MAX_PATH]    = { 0 };
    WCHAR szKeyFullName[MAX_PATH]       = { 0 };
    WCHAR szOutputPath[MAX_PATH]        = { 0 };

    HANDLE hKey = INVALID_HANDLE_VALUE;
    HANDLE hSavedKey = INVALID_HANDLE_VALUE;
    HANDLE hOutput;
    HANDLE hInput;
    LARGE_INTEGER lgInputSize;

    BOOL isDec;
    
    // kiem tra file input la loai nao
    GetExtFromFullName(szInputFile, szTempString);
    if (!wcscmp(szTempString, L"enc")) {
        isDec = TRUE;
    }
    else {
        isDec = FALSE;        
    }

    // Open the source file
    hInput = CreateFile(szInputFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hInput == INVALID_HANDLE_VALUE) {
        MessageBox(NULL, L"Không tìm thấy file!", L"Program", MB_ICONWARNING);
        return FALSE;
    }

    hSavedKey = CreateFile(szCurrKeyFullName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSavedKey == INVALID_HANDLE_VALUE) {
        CloseHandle(hInput);
        return FALSE;
    }

    ReadFile(hSavedKey, buff, 7, &dwBytesRead, NULL);
    dwCurrKeyCode = atoi(buff);

    if (isDec)
    {
        ReadFile(hInput, buff, 7, &dwBytesRead, NULL);
        dwCurrKeyCode = atoi(buff);
    }
    else {
        GetFileSizeEx(hInput, &lgInputSize);
        if ((lgInputSize.LowPart / 1024 / 1024 / 4) > dwRemainKeyInMB) {
            MessageBox(NULL, L"Không đủ khóa!", L"Program", MB_ICONWARNING);
            return FALSE;
        }
    }

    // tao file output
    if (!isDec) {
        GetNameFromFullName(szInputFile, szTempString);
        GetDirPathFromFullName(szInputFile, szOutputFullName);
        wcscat_s(szOutputFullName, MAX_PATH, L"\\");
        wcscat_s(szOutputFullName, MAX_PATH, szTempString);
        wcscat_s(szOutputFullName, MAX_PATH, L".enc");
    }
    else {
        RemoveExtFromFullName(szInputFile, szOutputFullName);
    }

    

    // Create a new file
    hOutput = CreateFile(szOutputFullName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (!isDec)
    {
        sprintf_s(buff, "%d", dwCurrKeyCode);
        WriteFile(hOutput, buff, strlen(buff), &dwBytesWritten, NULL);
    }

    if (hOutput == INVALID_HANDLE_VALUE) {
        CloseHandle(hInput);
        CloseHandle(hOutput);
        return FALSE;
    }

    

    // Copy contents
    bool ok = true;
    do {
        if (!ReadFile(hInput, buff, sizeof(buff), &dwBytesRead, NULL)) {
            ok = false;
            break;
        }

        if (ibuffCount % 64 == 0)
        {
            CloseHandle(hKey);
            dwCurrKeyCode = dwCurrKeyCode + 1;
            wcscpy_s(szKeyFullName, MAX_PATH, szKeyPath);
            swprintf_s(szTempString,L"%d", dwCurrKeyCode);
            wcscat_s(szKeyFullName, MAX_PATH, szTempString);
            hKey = CreateFile(szKeyFullName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hKey == INVALID_HANDLE_VALUE) {
                return FALSE;
            }
        }

        if (!ReadFile(hKey, buff_key, sizeof(buff_key), &dwKeyBytesRead, NULL)) {
            ok = false;
            break;
        }

        for (DWORD i = 0; i < dwBytesRead; i++)
        { 
            if (isDec) {
                if ((UINT)buff[i] < (UINT)buff_key[i])
                    buff[i] = (buff[i] + 0x100 - buff_key[i]);
                else
                    buff[i] = (buff[i] - buff_key[i]);
            }
            else
            {
                buff[i] = (buff[i] + buff_key[i]) & 0xff;
            }
        }

        // Check for EOF reached
        if (dwBytesRead == 0) {
            break;
        }

        // Write file, check for error
        if (!WriteFile(hOutput, buff, dwBytesRead, &dwBytesWritten, NULL)) {
            // printf("Target file not written to. Error %u", GetLastError());
            ok = false;
            break;
        }

        ibuffCount++;
    } while (true);

    // Copy complete

    CloseHandle(hInput);
    CloseHandle(hKey);
    CloseHandle(hOutput);

    
    if (!ok) {
        DeleteFile(szOutputFullName);
        return FALSE;
    }

    SetFilePointer(hSavedKey, 0, NULL, FILE_BEGIN);
    if (dwCurrKeyCode > dwCurrMaxKeyCode)
        dwCurrMaxKeyCode = dwCurrKeyCode;
    sprintf_s(buff, "%d", dwCurrMaxKeyCode + 1);
    WriteFile(hSavedKey, buff, 7, &dwBytesWritten, NULL);
    CloseHandle(hSavedKey);
    UpdateRemainKey();
    return TRUE;
}

BOOL InitKey()
{
    char buff[4096];
    DWORD dwBytesRead, dwBytesWritten;
    WCHAR szKeyInitFullName[MAX_PATH] = { 0 };
    WCHAR szTempString[MAX_PATH] = { 0 };
    HANDLE hSavedKey = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA ffdKey;
    HANDLE hF;


    GetModuleFileName(hInst, szTempString, MAX_PATH);
    GetDirPathFromFullName(szTempString, szKeyPath);

    wcscat_s(szKeyPath, MAX_PATH, L"\\keypath\\");
    wcscpy_s(szKeyInitFullName, MAX_PATH, szKeyPath);
    wcscat_s(szKeyInitFullName, MAX_PATH, L"init.txt");
    hF = CreateFile(szKeyInitFullName, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hF == INVALID_HANDLE_VALUE)
        return false;
    ReadFile(hF, buff, 7, &dwBytesRead, NULL);
    dwMinKeyCode = atoi(buff);
    ReadFile(hF, buff, 7, &dwBytesRead, NULL);
    dwMaxKeyCode = atoi(buff);
    CloseHandle(hF);

    GetDirPathFromFullName(szTempString, szCurrKeyFullName);
    wcscat_s(szCurrKeyFullName, MAX_PATH, L"\\current-key.dat");
    
    // dup
    hSavedKey = CreateFile(szCurrKeyFullName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSavedKey == INVALID_HANDLE_VALUE) {
        CloseHandle(hSavedKey);
        return EXIT_FAILURE;
    }
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        ReadFile(hSavedKey, buff, 7, &dwBytesRead, NULL);
        dwCurrMaxKeyCode = atoi(buff);
        CloseHandle(hSavedKey);
    }
    else {
        dwCurrMaxKeyCode = dwMinKeyCode;
        sprintf_s(buff, "%d", dwCurrMaxKeyCode);
        WriteFile(hSavedKey, buff, 7, &dwBytesWritten, NULL);
        CloseHandle(hSavedKey);
    }
    return true;
}

BOOL UpdateRemainKey()
{
    dwRemainKeyInMB = (dwMaxKeyCode - dwCurrMaxKeyCode) / 4;
    swprintf_s(szRemainKeyPromt, L"Kích thước khóa còn lại: %d MB", dwRemainKeyInMB);
    SetWindowText(hwndStatus, szRemainKeyPromt);
    return TRUE;
}

BOOL GetDirPathFromFullName(WCHAR* in, WCHAR* out) {
    wcscpy_s(out,MAX_PATH, in);
    WCHAR* p = wcsrchr(out, L'\\');
    if (p) p[0] = 0;
    else
        return FALSE;
    return TRUE;
}

BOOL GetNameFromFullName(WCHAR* in, WCHAR* out) {
    int len;
    WCHAR* p = wcsrchr(in, L'\\');
    len = p - in;
    if (p)
        wcscpy_s(out, MAX_PATH - len, p + 1);
    else
        return FALSE;
    return TRUE;
}

BOOL GetExtFromFullName(WCHAR* in, WCHAR* out)
{
    int len;
    WCHAR* p = wcsrchr(in, L'.');
    len = p - in;
    if (p)
        wcscpy_s(out, MAX_PATH - len, p + 1);
    else
        return FALSE;
    return TRUE;
}

BOOL RemoveExtFromFullName(WCHAR* in, WCHAR* out)
{
    wcscpy_s(out, MAX_PATH, in);
    WCHAR* p = wcsrchr(out, L'.');
    if (p) p[0] = 0;
    else
        return FALSE;
    return TRUE;
}
