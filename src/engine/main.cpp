#include <iostream>

#include "window.h"


int main()
{
    corsac::Window window;

    window.SetTitle("My Game");
    window.Resize(1200, 800);
    window.Show();

    SDL_Delay(3000);

    std::cout << window.x() << std::endl;

    return 0;
}
