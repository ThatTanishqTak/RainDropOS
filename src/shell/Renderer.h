#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Renderer
{
public:
    bool Initialize(SDL_Renderer* sdlRenderer);
    void Shutdown();

    void Clear();
    void Present();

    void DrawRect(int x, int y, int w, int h, SDL_Color color);
    void DrawText(const std::string& text, int x, int y, int size, SDL_Color color);

    SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }

private:
    SDL_Renderer* m_Renderer = nullptr;
    TTF_Font* m_Font = nullptr;

    static constexpr const char* FONT_PATH = "/home/thattanishqtak/.local/share/fonts/JetBrainsMono/JetBrainsMonoNerdFont-Regular.ttf";
};