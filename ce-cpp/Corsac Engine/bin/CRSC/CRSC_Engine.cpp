#include "CRSC_Engine.h"

CRSC_Engine::CRSC_Engine(){}
CRSC_Engine::~CRSC_Engine(){}

void CRSC_Engine::Init(const char* GameName, const char* OrgName)
{
	log = new CRSC_Logs();
	if (!(app = new CRSC_App())) CRSC_Logs::set("Could not create application!", "");
	app->Setup("Corsac Game", "Corsac");

	run = true;
}

void CRSC_Engine::AddScene(CRSC_Scene* scene)
{
	std::unique_ptr<CRSC_Scene> s(scene);
	scenes.emplace_back(std::move(s));
}

void CRSC_Engine::Setup()
{
	//while (run)
	//{
		for (auto& s : scenes)
		{
			fpsTimer.start();
			while (s->run)
			{
				Uint32 now = SDL_GetTicks();

				float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);

				if (next_game_step <= now) {

					int computer_is_too_slow_limit = 10;

					while ((next_game_step <= now) && (computer_is_too_slow_limit--)) {
						s->Event();
						s->Update(avgFPS);
						next_game_step += time_step_ms;
					}

					interpolation = float(SDL_GetTicks() + time_step_ms - next_game_step)
						/ float(time_step_ms);

					s->Render();
				}
				else {
					if (FPS_Limit) {
						SDL_Delay(next_game_step - now);
					}
				}
				++countedFrames;
			}
		}
	//}
}

void CRSC_Engine::Exit()
{
	run = false;
	Destroy();
}

void CRSC_Engine::Destroy()
{
	app->Clear();
}