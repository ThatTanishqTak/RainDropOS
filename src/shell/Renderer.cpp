#include "Renderer.h"

#include <filesystem>
#include <iostream>

static const char* FONT_CANDIDATES[] =
{
    "assets/fonts/JetBrainsMonoNerdFont-Regular.ttf",
    "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
    "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf",
    "/usr/share/fonts/truetype/freefont/FreeMono.ttf",
    nullptr
};

bool Renderer::ResolveFontPath()
{
    for (const char** candidate = FONT_CANDIDATES; *candidate != nullptr; ++candidate)
    {
        if (std::filesystem::exists(*candidate))
        {
            m_FontPath = *candidate;
            std::cout << "Renderer: using font " << m_FontPath << "\n";
    
            return true;
        }
    }

    std::cerr << "Renderer: no usable font found — text will not render\n";

    return false;
}

bool Renderer::Initialize(SDL_Renderer* sdlRenderer)
{
    m_Renderer = sdlRenderer;

    if (TTF_Init() != 0)
    {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << "\n";

        return false;
    }

    ResolveFontPath();

    for (int size : { 15, 16, 18, 20, 38, 40 })
    {
        GetFont(size);
    }

    return true;
}

void Renderer::Shutdown()
{
    // Destroy all cached textures.
    for (auto& [key, entry] : m_TextureCache)
    {
        SDL_DestroyTexture(entry.cached.texture);
    }
    m_TextureCache.clear();
    m_CacheOrder.clear();

    for (auto& [size, font] : m_FontCache)
    {
        TTF_CloseFont(font);
    }
    m_FontCache.clear();

    TTF_Quit();
}

TTF_Font* Renderer::GetFont(int size)
{
    if (m_FontPath.empty())
    {
        return nullptr;
    }

    auto it = m_FontCache.find(size);
    if (it != m_FontCache.end())
    {
        return it->second;
    }

    TTF_Font* font = TTF_OpenFont(m_FontPath.c_str(), size);
    if (!font)
    {
        std::cerr << "Font load failed (size " << size << "): " << TTF_GetError() << "\n";

        return nullptr;
    }

    m_FontCache[size] = font;
    
    return font;
}

void Renderer::EvictOldest()
{
    if (m_CacheOrder.empty())
    {
        return;
    }

    const TextureKey& oldest = m_CacheOrder.front();
    auto it = m_TextureCache.find(oldest);
    if (it != m_TextureCache.end())
    {
        SDL_DestroyTexture(it->second.cached.texture);
        m_TextureCache.erase(it);
    }
    m_CacheOrder.pop_front();
}

const Renderer::CachedTexture* Renderer::GetCachedTexture(
    const std::string& text, int size, SDL_Color color)
{
    // Pack colour into a single Uint32 for use as part of the cache key.
    Uint32 rgba = (Uint32(color.r) << 24) | (Uint32(color.g) << 16) | (Uint32(color.b) <<  8) |  Uint32(color.a);

    TextureKey key{ text, size, rgba };

    auto it = m_TextureCache.find(key);
    if (it != m_TextureCache.end())
    {
        return &it->second.cached;
    }

    // Cache miss — create the texture.
    TTF_Font* font = GetFont(size);
    if (!font)
    {
        return nullptr;
    }

    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface)
    {
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
    int w = surface->w;
    int h = surface->h;
    SDL_FreeSurface(surface);

    if (!texture)
    {
        return nullptr;
    }

    if (m_TextureCache.size() >= MAX_TEXTURE_CACHE)
    {
        EvictOldest();
    }

    m_CacheOrder.push_back(key);
    CacheEntry& entry = m_TextureCache[key];
    entry.cached = { texture, w, h };
    entry.orderIt = std::prev(m_CacheOrder.end());

    return &entry.cached;
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

void Renderer::DrawRectOutline(int x, int y, int w, int h, int thickness, SDL_Color color)
{
    SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);

    SDL_Rect top = { x, y, w, thickness };
    SDL_Rect bottom = { x, y + h - thickness, w, thickness };
    SDL_Rect left = { x, y, thickness, h };
    SDL_Rect right = { x + w - thickness, y, thickness, h };

    SDL_RenderFillRect(m_Renderer, &top);
    SDL_RenderFillRect(m_Renderer, &bottom);
    SDL_RenderFillRect(m_Renderer, &left);
    SDL_RenderFillRect(m_Renderer, &right);
}

void Renderer::DrawText(const std::string& text, int x, int y, int size, SDL_Color color)
{
    const CachedTexture* cached = GetCachedTexture(text, size, color);
    if (!cached)
    {
        return;
    }

    SDL_Rect dst = { x, y, cached->w, cached->h };
    SDL_RenderCopy(m_Renderer, cached->texture, nullptr, &dst);
}