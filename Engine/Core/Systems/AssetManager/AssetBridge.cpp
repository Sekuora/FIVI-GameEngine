#include <AssetBridge.hpp>
#include <Log.hpp>



#include <SDL3_image/SDL_image.h>




AssetBridge::AssetBridge()
{
    Log::Custom<false>("Constructor called!", Color_White, "Asset Bridge");

}

AssetBridge::~AssetBridge()
{
    ClearAssets();
    Log::Custom<false>("Destructor called!", Color_White, "Asset Bridge");

}

void AssetBridge::ClearAssets()
{
    for(auto texture: textures)
    {
        SDL_DestroyTexture(texture.second);
    }

    textures.clear();
}

void AssetBridge::AddTexture(SDL_Renderer* renderer, const string &assetId, const string &filePath)
{
    SDL_Surface* surface = IMG_Load(filePath.c_str());

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) { SDL_Log("Failed to load texture: %s", SDL_GetError()); }

    SDL_DestroySurface(surface);

    textures.emplace(assetId, texture);

    Log::Custom<false>("New Texture Added to the asset bridge with id = " + assetId, Color_White, "Asset Bridge");

}

SDL_Texture *AssetBridge::GetTexture(const string &assetId)
{
    return textures[assetId];
}
