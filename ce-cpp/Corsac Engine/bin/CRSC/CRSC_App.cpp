#include "CRSC_App.h"

SDL_Window*		CRSC_App::Window	= nullptr;
SDL_Renderer*	CRSC_App::Renderer	= nullptr;

int CRSC_App::Width	= 1800;
int CRSC_App::Height = 1000;

const char* CRSC_App::AppName = "Corsac Game";
const char* CRSC_App::OrgName = "Corsac Engine";

char* CRSC_App::AppPath;
char* CRSC_App::UserPath;

int CRSC_App::Setup(const char* AppName, const char* OrgName)
{
	this->AppName = AppName;
	this->OrgName = OrgName;

	this->AppPath = SDL_GetBasePath();
	this->UserPath = SDL_GetPrefPath(OrgName, AppName);

	// Инициализируем SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
		CRSC_Logs::set("SDL could not initialize! SDL_Error: %s", SDL_GetError());
		return 1;
	}

	// Инициализируем IMG
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		CRSC_Logs::set("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return 1;
	}

	// Инициализируем TTF
	if (TTF_Init() == -1) {
		CRSC_Logs::set("TTF_Init: %s\n", TTF_GetError());
		return 1;
	}

	//FindSize();

	// Создаем окно
	this->Window = SDL_CreateWindow(AppName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (this->Window == NULL) {
		CRSC_Logs::set("Could not create window: %s", SDL_GetError());
		return 1;
	}

	// Создаем поверхность рендера
	this->Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	if (this->Renderer == NULL) {
		CRSC_Logs::set("Could not create renderer: %s", SDL_GetError());
		return 1;
	}

	SetFull(0);
	SetScaleQuality("0");

	// Окрашиваем экран в черный
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Renderer);
	return 0;
}
void CRSC_App::FindSize()
{
	SDL_DisplayMode dm;
	if (SDL_GetCurrentDisplayMode(0, &dm) != 0) CRSC_Logs::set("Could not get current display mode", SDL_GetError());
	this->Width = dm.w;
	this->Height = dm.h;
}
void CRSC_App::SetFull(Uint32 flag)
{
	if (SDL_SetWindowFullscreen(Window, flag) != 0) CRSC_Logs::set("Could notset screen mode", SDL_GetError());
}

void CRSC_App::SetSize(int w, int h)
{
	SDL_SetWindowSize(Window, w, h);
}

void CRSC_App::SetScaleQuality(const char* flag)
{
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, flag)) CRSC_Logs::set("Could not set scale quality", SDL_GetError());
}

void CRSC_App::Clear()
{
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
