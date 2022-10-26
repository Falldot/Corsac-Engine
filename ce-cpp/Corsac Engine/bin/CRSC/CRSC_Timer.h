#pragma once

#include "SDL.h"
//#include "SDL2/SDL.h"

class CRSC_Timer
{
public:
    CRSC_Timer();

    // Запустить
    void start();
    // Выключить
    void stop();
    // Останосить
    void pause();
    // Прололжить
    void unpause();

    // Получить текущие значение таймера
    Uint32 getTicks();

    // Проверить статус таймера
    bool isStarted();
    bool isPaused();

private:
    // Время старта
    Uint32 mStartTicks;
    // Время паузы
    Uint32 mPausedTicks;

    // Состаяние таймера
    bool mPaused;
    bool mStarted;
};

