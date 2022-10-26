#pragma once

#include <map>
#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

/////// Модули

#include "Modules/Loader.h"

//////////////////////////////////////////////////////

// Глобальный массив текстур
static std::map <std::string, SDL_Texture*> CRSC_Textures;

// Загрузить текстуру (id, путь)
void CRSC_LoadTexture(std::string id, const char* path);
// Удалить текстуру (id)
void CRSC_DestroyTexture(std::string id);
// Удалить все текстуры
void CRSC_DestroyAllTexture();
// Получить текстуру (id)
SDL_Texture* CRSC_GetTexture(std::string id);

/////////////////////////////////////////////////////

// Глобальный массив шрифтов
static std::map <std::string, TTF_Font*> CRSC_Fonts;

// Загрузить шрифт (id, путь)
void CRSC_LoadFont(std::string id, std::string path, int fontSize);
// Удалить шрифт (id)
void CRSC_DestroyFont(std::string id);
// Удалить все шрифты
void CRSC_DestroyAllFonts();
// Получить шрифт по id (id)
TTF_Font* CRSC_GetFont(std::string id);


