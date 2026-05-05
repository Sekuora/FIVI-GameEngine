#pragma once



#include <iostream>
#include <fstream>

#include <map>

#include <string>

#include <SDL3/SDL.h>

#include <ECS.hpp>

using std::ifstream;
using std::map;
using std::string;


class AssetBridge
{
public:

AssetBridge();
~AssetBridge();

void ClearAssets();
void AddTexture(SDL_Renderer* renderer, const string& assetId, const string& filePath);



SDL_Texture* GetTexture(const string& assetId);


private:


    map<string, SDL_Texture*> textures;


};