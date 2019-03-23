#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include <sstream>

//热键ID
#define IDK_BEGIN                               40012
#define IDK_END                                 40013

#define IDT_TIMER                                 40020

//鼠标坐标
POINT pt = {0,0};

//单选按钮
HWND leftButton;
HWND rightButton;

HWND intervalEdit;
int interval = 10;

HWND stateText;
const TCHAR* soff = TEXT("未启用");
const TCHAR* son = TEXT("启用中");

using std::stringstream;
stringstream ss;

//鼠标左右键选择
int MOUSEEVENTF_DOWN;
int MOUSEEVENTF_UP;

void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT nTimerid, DWORD dwTime);
BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void timerClick(HWND hwndDlg);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    InitCommonControls();
    LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICONT));
    return DialogBox(hInstance, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        stateText = GetDlgItem(hwndDlg, IDT_STATE);
        SetWindowText(stateText, soff);

        ss << interval;
        intervalEdit = GetDlgItem(hwndDlg, IDE_INTERVAL);
        SendMessage(intervalEdit, EM_REPLACESEL, 0,
                    (LPARAM)ss.str().c_str());

        leftButton = GetDlgItem(hwndDlg, IDR_LEFT);
        rightButton = GetDlgItem(hwndDlg, IDR_RIGHT);

        SendMessage(leftButton, BM_SETCHECK, BST_CHECKED, 0);
        MOUSEEVENTF_DOWN = MOUSEEVENTF_LEFTDOWN;
        MOUSEEVENTF_UP=MOUSEEVENTF_LEFTUP;

        //注册热键
        RegisterHotKey(hwndDlg, IDK_BEGIN, MOD_CONTROL, 0x53);
        RegisterHotKey(hwndDlg, IDK_END, MOD_CONTROL, 0x45);

        SetWindowPos(hwndDlg, HWND_TOPMOST, 200, 200, 0, 0, SWP_NOSIZE);
    }
    return TRUE;

    case WM_CLOSE:
    {
        SetWindowText(stateText, soff);
        KillTimer(hwndDlg, IDT_TIMER);
        //注销热键
        UnregisterHotKey(hwndDlg, IDK_BEGIN);
        UnregisterHotKey(hwndDlg, IDK_END);

        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        case IDB_BEGIN:
            timerClick(hwndDlg);
            break;
        case IDB_END:
            SetWindowText(stateText, soff);
            KillTimer(hwndDlg, IDT_TIMER);
            break;
        }
    }
    return TRUE;
    case WM_HOTKEY:
    {
        switch(wParam)
        {
        case IDK_BEGIN:
            timerClick(hwndDlg);
            break;

        case IDK_END:
            SetWindowText(stateText, soff);
            KillTimer(hwndDlg, IDT_TIMER);
            break;
        }
    }
    return TRUE;
    }
    return FALSE;
}

void timerClick(HWND hwndDlg)
{
    TCHAR inc[11];
//            GetWindowText(intervalEdit, inc, 10);
    SendMessage(intervalEdit,WM_GETTEXT,10,(LPARAM)inc);

    //清空strinstream
    ss.clear();
    ss.str("");

    ss << inc;
    ss >> interval;

    GetCursorPos(&pt);
    if(IsDlgButtonChecked(hwndDlg, IDR_LEFT))
    {
        MOUSEEVENTF_DOWN = MOUSEEVENTF_LEFTDOWN;
        MOUSEEVENTF_UP=MOUSEEVENTF_LEFTUP;
    }
    else
    {
        MOUSEEVENTF_DOWN = MOUSEEVENTF_RIGHTDOWN;
        MOUSEEVENTF_UP=MOUSEEVENTF_RIGHTUP;
    }

    SetWindowText(stateText, son);
    SetTimer(hwndDlg, IDT_TIMER, interval, TimerProc);
}


void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT nTimerid, DWORD dwTime)
{
    //鼠标连击
    mouse_event(MOUSEEVENTF_DOWN, pt.x, pt.y, 0, 0);
    mouse_event(MOUSEEVENTF_UP, pt.x, pt.y, 0, 0);

}
