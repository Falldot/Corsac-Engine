//
// Created by Falldot on 13.11.2021.
//

#ifndef CORSAC_WINDOW_H
#define CORSAC_WINDOW_H

#pragma once

#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>

#include "loader.h"

namespace corsac
{
    class Window
    {
        SDL_Window* context;
        uint16_t x_;
        uint16_t y_;

    public:
        explicit Window(const std::string& title = "", uint16_t width = 1200, uint16_t height = 800);

        ~Window();

        void Show() noexcept;
        void Hide() noexcept;

        [[nodiscard]] constexpr auto x() const& -> const uint16_t&  { return x_; }
        [[nodiscard]] constexpr auto x() &      -> uint16_t&        { return x_; }

        [[nodiscard]] constexpr auto y() const& -> const uint16_t&  { return y_; }
        [[nodiscard]] constexpr auto y() &      -> uint16_t&        { return y_; }

        void Resize(uint16_t width, uint16_t height) noexcept;

        void SetPosition(uint16_t posX, uint16_t posY) noexcept;
        void SetOpacity(float_t opacity) noexcept;
        void SetBordered(bool value) noexcept;
        void SetGrab(bool value) noexcept;
        void SetFullscreen(uint8_t value) noexcept;
        void SetTitle(const std::string& title) const noexcept;

        void SetIcon(corsac::Surface surf) noexcept;
        void SetIcon(const std::string& path) const noexcept;
    };

    Window::Window(const std::string& title, uint16_t width, uint16_t height)
        : x_(width), y_(height)
    {
        context = SDL_CreateWindow(
                title.c_str(),
                SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
                width, height,
                0
        );
    }

    Window::~Window()
    {
        if (context) {
            SDL_DestroyWindow(context);
            context = nullptr;
        }
    }

    inline void Window::Show() noexcept
    {
        SDL_ShowWindow(context);
    }

    inline void Window::Hide() noexcept
    {
        SDL_HideWindow(context);
    }

    inline void Window::Resize(uint16_t width, uint16_t height) noexcept
    {
        SDL_SetWindowSize(context, width, height);
        x_ = width; y_ = height;
    }

    inline void Window::SetPosition(uint16_t posX, uint16_t posY) noexcept
    {
        SDL_SetWindowPosition(context, posX, posY);
    }

    inline void Window::SetTitle(const std::string& title) const noexcept
    {
        SDL_SetWindowTitle(context, title.c_str());
    }

    inline void Window::SetIcon(corsac::Surface surf) noexcept
    {
        SDL_SetWindowIcon(context, surf.c_surf());
    }

    inline void Window::SetIcon(const std::string& path) const noexcept
    {
        SDL_SetWindowIcon(context, corsac::Surface(path).c_surf());
    }

    void Window::SetOpacity(float_t opacity) noexcept
    {
        SDL_SetWindowOpacity(context, opacity);
    }

    void Window::SetBordered(bool value) noexcept
    {
        SDL_SetWindowBordered(context, SDL_bool(value));
    }

    void Window::SetGrab(bool value) noexcept
    {
        SDL_SetWindowGrab(context, SDL_bool(value));
    }

    void Window::SetFullscreen(uint8_t value) noexcept
    {
        SDL_SetWindowFullscreen(context, value);
    }
}

#endif //CORSAC_WINDOW_H
