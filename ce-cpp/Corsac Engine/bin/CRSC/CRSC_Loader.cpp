#include "CRSC_Loader.h"

void CRSC_LoadTexture(std::string id, const char* path)
{
	CRSC_Textures.emplace(id, Load(path));
}

void CRSC_DestroyTexture(std::string id)
{
	SDL_DestroyTexture(CRSC_Textures[id]);
}

void CRSC_DestroyAllTexture()
{
	for (auto& t : CRSC_Textures)
	{
		SDL_DestroyTexture(t.second);
	}
}

SDL_Texture* CRSC_GetTexture(std::string id)
{
	return CRSC_Textures[id];
}

void CRSC_LoadFont(std::string id, std::string path, int fontSize)
{
	CRSC_Fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}
void CRSC_DestroyFont(std::string id)
{
	TTF_CloseFont(CRSC_Fonts[id]);
}
void CRSC_DestroyAllFonts()
{
	for (auto& f : CRSC_Fonts)
	{
		TTF_CloseFont(f.second);
	}
}
TTF_Font* CRSC_GetFont(std::string id)
{
	return CRSC_Fonts[id];
}

