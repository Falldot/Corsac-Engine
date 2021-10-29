#pragma once

#ifndef UNICODE
    #define UNICODE
#endif

#include <windows.h>

typedef HWND corsac_window;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void corsac_create_window();

void corsac_window_border(HWND hwnd);

void corsac_input();