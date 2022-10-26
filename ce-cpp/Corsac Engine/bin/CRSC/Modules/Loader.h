#pragma once
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "../CRSC_App.h"
#include "../CRSC_Loader.h"
#include "../CRSC_Logs.h"

SDL_Texture* Load(const char* path);
SDL_Texture* Load(std::string text, std::string font, SDL_Color textColor);

void Query(SDL_Texture* tex, Uint32* format, int* access, int* w, int* h);

void Draw(SDL_Texture* tex, SDL_Rect* src, SDL_Rect* dest);
void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip = SDL_FLIP_NONE);

