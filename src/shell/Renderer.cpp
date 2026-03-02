#include "Renderer.h"
#include <iostream>

bool Renderer::Initialize(SDL_Renderer* sdlRenderer)
{
    m_Renderer = sdlRenderer;

    if (TTF_Init() != 0)
    {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << "\n";
        
        return false;
    }

    m_Font = TTF_OpenFont(FONT_PATH, 24);
    if (!m_Font)
    {
        std::cerr << "Font load failed: " << TTF_GetError() << "\n";

        return false;
    }

    return true;
}

void Renderer::Shutdown()
{
    if (m_Font) { TTF_CloseFont(m_Font); m_Font = nullptr; }
    TTF_Quit();
}

void Renderer::Clear()
{
    SDL_SetRenderDrawColor(m_Renderer, 15, 15, 25, 255);
    SDL_RenderClear(m_Renderer);
}

void Renderer::Present()
{
    SDL_RenderPresent(m_Renderer);
}

void Renderer::DrawRect(int x, int y, int w, int h, SDL_Color color)
{
    SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = { x, y, w, h };
    SDL_RenderFillRect(m_Renderer, &rect);
}

void Renderer::DrawText(const std::string& text, int x, int y, int size, SDL_Color color)
{
    TTF_Font* font = TTF_OpenFont(FONT_PATH, size);
    if (!font)
    {
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface)
    {
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
    if (texture)
    {
        SDL_Rect dst = { x, y, surface->w, surface->h };
        SDL_RenderCopy(m_Renderer, texture, nullptr, &dst);
        SDL_DestroyTexture(texture);
    }

    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
}