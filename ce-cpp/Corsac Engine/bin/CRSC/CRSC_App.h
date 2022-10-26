#pragma once

#include <iostream>

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

#include "CRSC_Logs.h"

class CRSC_App
{
private:
	// Находит параментры экрана и устанавливает их
	void FindSize();

	int ScreenMode = 0;
public:
	static SDL_Window* Window; // Окно
	static SDL_Renderer* Renderer; // Рендер

	static int Width;	// Ширина
	static int Height;	// Высота

	static const char* AppName;  // Именование программы
	static const char* OrgName;  // Именование огранизации
	
	static char* AppPath;  // Путь до программы
	static char* UserPath; // Путь до user каталога

	// Запустить Engine
	int Setup(const char* AppName, const char* OrgName);

	// Полная очистка
	void Clear();

	// Установить FullScreen
	// SDL_WINDOW_FULLSCREEN, SDL_WINDOW_FULLSCREEN_DESKTOP or 0
	void SetFull(Uint32 flag);

	// Установить размеры окна
	void SetSize(int w, int h);

	// Установить качество скалирования текстур
	// "0", "1" or "2"
	void SetScaleQuality(const char* value);
};

