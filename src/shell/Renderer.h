#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <list>
#include <unordered_map>

class Renderer
{
public:
    bool Initialize(SDL_Renderer* sdlRenderer);
    void Shutdown();

    void Clear();
    void Present();

    void DrawRect(int x, int y, int w, int h, SDL_Color color);
    void DrawRectOutline(int x, int y, int w, int h, int thickness, SDL_Color color);
    void DrawText(const std::string& text, int x, int y, int size, SDL_Color color);

    SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }

private:
    TTF_Font* GetFont(int size);
    bool ResolveFontPath();

    struct TextureKey
    {
        std::string text;
        int size;
        Uint32 rgba; // packed colour

        bool operator==(const TextureKey& o) const
        {
            return size == o.size && rgba == o.rgba && text == o.text;
        }
    };

    struct TextureKeyHash
    {
        std::size_t operator()(const TextureKey& k) const noexcept
        {
            std::size_t h = std::hash<std::string>{}(k.text);
            h ^= std::hash<int>{}(k.size)   + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= std::hash<Uint32>{}(k.rgba) + 0x9e3779b9 + (h << 6) + (h >> 2);

            return h;
        }
    };

    struct CachedTexture
    {
        SDL_Texture* texture = nullptr;
        int w = 0;
        int h = 0;
    };

    const CachedTexture* GetCachedTexture(const std::string& text, int size, SDL_Color color);
    void EvictOldest();

    using KeyList = std::list<TextureKey>;
    KeyList m_CacheOrder;

    struct CacheEntry
    {
        CachedTexture  cached;
        KeyList::iterator orderIt;
    };

    std::unordered_map<TextureKey, CacheEntry, TextureKeyHash> m_TextureCache;

    static constexpr std::size_t MAX_TEXTURE_CACHE = 128;

    SDL_Renderer* m_Renderer = nullptr;
    std::unordered_map<int, TTF_Font*> m_FontCache;
    std::string m_FontPath;
};