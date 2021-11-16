//
// Created by Falldot on 13.11.2021.
//

#ifndef CORSAC_LOADER_H
#define CORSAC_LOADER_H

#pragma once

#include <SDL.h>
#include <SDL_image.h>

namespace corsac
{
    class Surface
    {
        SDL_Surface* surf;

    public:
        explicit Surface(SDL_Surface* path);
        explicit Surface(const std::string& path);
        ~Surface();

        SDL_Surface* c_surf() noexcept;
    };

    Surface::Surface(const std::string& path) {
        surf = IMG_Load(path.c_str());
    }

    Surface::Surface(SDL_Surface* surf) : surf(surf)
    {}

    Surface::~Surface() {
        if (surf)
        {
            SDL_FreeSurface(surf);
            surf = nullptr;
        }
    }

    inline SDL_Surface *Surface::c_surf() noexcept {
        return surf;
    }
}

#endif //CORSAC_LOADER_H
