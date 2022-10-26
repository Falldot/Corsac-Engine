#include "Loader.h"

SDL_Texture* Load(const char* path)
{
	SDL_Surface* s = IMG_Load(path);
	if (!s) {
		CRSC_Logs::set("IMG_Load: ", IMG_GetError());
		CRSC_Logs::set(path, "");
	}
	SDL_Texture* t = SDL_CreateTextureFromSurface(CRSC_App::Renderer, s);
	if (t == NULL) {
		CRSC_Logs::set("CreateTextureFromSurface failed: ", SDL_GetError());
	}
	SDL_FreeSurface(s);
	return t;
}

SDL_Texture* Load(std::string text, std::string font, SDL_Color textColor)
{
	SDL_Surface* s = TTF_RenderText_Blended(CRSC_GetFont(font), text.c_str(), textColor);
	if (!s) {
		CRSC_Logs::set("Ttf_Load: ", TTF_GetError());
	}
	SDL_Texture*  t = SDL_CreateTextureFromSurface(CRSC_App::Renderer, s);
	if (t == NULL) {
		CRSC_Logs::set("CreateTextureFromSurface failed: ", SDL_GetError());
	}
	SDL_FreeSurface(s);
	return t;
}

void Query(SDL_Texture* tex, Uint32* format, int* access, int* w, int* h)
{
	if (SDL_QueryTexture(tex, format, access, w, h)) CRSC_Logs::set("QueryTexture failed: ", SDL_GetError());
}

void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
	if (SDL_RenderCopyEx(CRSC_App::Renderer, tex, &src, &dest, NULL, NULL, flip)) CRSC_Logs::set("RenderCopEx failed: ", SDL_GetError());
}

void Draw(SDL_Texture* tex, SDL_Rect* src, SDL_Rect* dest)
{
	if (SDL_RenderCopy(CRSC_App::Renderer, tex, src, dest)) CRSC_Logs::set("RenderCopEx failed: ", SDL_GetError());
}
