//
// Created by 田淙宇 on 2018/10/22.
//

#ifndef GRENDER_PAINTER_H
#define GRENDER_PAINTER_H

#include <stdlib.h>
#include <windows.h>
#include <string>
#include <tchar.h>
#include "Matrix.h"

using std::string;






class RenderDevice{
public:
    //初始化窗口
    int initWindow(int w,int h, const TCHAR* title){
        //定义窗口类
        WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)screen_events, 0, 0, 0,
                        NULL, NULL, NULL, NULL, _T("SCREEN3.1415926") };
        //定义位图信息
        BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB,DWORD(w*h*4), 0, 0, 0, 0 }  };
        RECT rect = { 0, 0, w, h };
        int wx, wy, sx, sy;
        LPVOID ptr;
        HDC hDC;
        closeWindow();
        //定义背景
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wc.hInstance = GetModuleHandle(NULL);
        //定义光标
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        //窗口类注册不成功则返回-1
        if (!RegisterClass(&wc)) return -1;
        screen_handle = CreateWindow(_T("SCREEN3.1415926"),title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
        if (screen_handle == NULL) return -2;
        //定义是否退出的变量
        screen_exit = 0;
        //获得绘图句柄
        hDC = GetDC(screen_handle);
        screen_dc = CreateCompatibleDC(hDC);
        ReleaseDC(screen_handle, hDC);
        screen_hb = CreateDIBSection(screen_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
        if (screen_hb == NULL) return -3;
        screen_ob = (HBITMAP)SelectObject(screen_dc, screen_hb);
        screen_fb = (unsigned char*)ptr;
        screen_w = w;
        screen_h = h;
        screen_pitch = w * 4;
        AdjustWindowRect(&rect, GetWindowLong(screen_handle, GWL_STYLE), 0);
        wx = rect.right - rect.left;
        wy = rect.bottom - rect.top;
        sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
        sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
        if (sy < 0) sy = 0;
        SetWindowPos(screen_handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
        SetForegroundWindow(screen_handle);
        ShowWindow(screen_handle, SW_NORMAL);
        dispatchWindow();
        memset(screen_keys, 0, sizeof(int) * 512);
        memset(screen_fb, 0, w * h * 4);
        return 0;
    }

    //关闭窗口
    int closeWindow(){
        if (screen_dc) {
            if (screen_ob) {
                SelectObject(screen_dc, screen_ob);
                screen_ob = NULL;
            }
            DeleteDC(screen_dc);
            screen_dc = NULL;
        }
        if (screen_hb) {
            DeleteObject(screen_hb);
            screen_hb = NULL;
        }
        if (screen_handle) {
            CloseWindow(screen_handle);
            screen_handle = NULL;
        }
        return 0;
    }
    //事件启动函数
    void dispatchWindow(){
        MSG msg;
        while (1) {
            if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
            if (!GetMessage(&msg, NULL, 0, 0)) break;
            DispatchMessage(&msg);
        }
    }
    //每帧刷新函数
    void updateWindow(){
        HDC hDC = GetDC(screen_handle);
        //贴图
        BitBlt(hDC, 0, 0, screen_w, screen_h, screen_dc, 0, 0, SRCCOPY);
        ReleaseDC(screen_handle, hDC);
        dispatchWindow();
    }
    //事件函数
    static LRESULT screen_events(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
        switch (msg) {
            case WM_CLOSE: screen_exit = 1; break;
            case WM_KEYDOWN: screen_keys[wParam & 511] = 1; break;
            case WM_KEYUP: screen_keys[wParam & 511] = 0; break;
            default: return DefWindowProc(hWnd, msg, wParam, lParam);
        }
        return 0;
    }
    //判断是否退出
    bool exit()const{
        return screen_exit!=0;
    }
    //返回帧缓存指针
    unsigned char* frameBuffer(){
        return screen_fb;
    }

    int keyState(int key)const{
        return screen_keys[key];
    }
private:

    int screen_w, screen_h;
    static int screen_exit;
    int screen_mx = 0, screen_my = 0, screen_mb = 0;
    static int screen_keys[512];	// 当前键盘按下状态
    static HWND screen_handle;		// 主窗口 HWND
    static HDC screen_dc;			// 配套的 HDC
    static HBITMAP screen_hb;		// DIB
    static HBITMAP screen_ob;		// 老的 BITMAP
    unsigned char *screen_fb = NULL;		// frame buffer
    long screen_pitch = 0;
};
int RenderDevice::screen_keys[512];
int RenderDevice::screen_exit=0;
HWND RenderDevice::screen_handle=NULL;
HDC RenderDevice::screen_dc=NULL;
HBITMAP RenderDevice::screen_hb=NULL;
HBITMAP RenderDevice::screen_ob=NULL;


#endif //GRENDER_PAINTER_H
