#pragma once

#include <ECS.hpp>

#include <AssetBridge.hpp>
#include <EventBridge.hpp>

#include <GlobalPointerAlias.hpp>
#include <cstdint>



class SDL_Window;
class SDL_Renderer;
class SDL_Rect;

using std::unique_ptr;




/**
 *  @author Firagic Entertainment
 *  @brief  Game Class. Defines core game features.
 *  @details Manage game from start to end. For now processes input and rendering.
 *  Planned to separate features in the future.
*/
class Game
{

public:

    // Properties //
    static int windowWidth;
    static int windowHeight;
    static int mapWidth;
    static int mapHeight;
    const char* windowTitle;

    // Methods //
    
    Game();
    ~Game();

    /** Pre Process Setup */
    void Setup();

    /** Entry Point */
    void Start();

    /** Runtime game process */
    void Run();

    /** Cleanup and end game processes */
    void End();

    /** Handle user input */
    void ProcessInput();

    /** Update game cycles */
    void Update();

    /** Render game to screen */
    void Render();

    /** Load Level before Setup */
    void LoadLevel(int level);
    

private:
/**
	* @brief SDL Window used for Fivi Engine Games
	**/
	SDL_Window* window;

	/**
	* @brief SDL Renderer for game Window
	**/
	SDL_Renderer* renderer;

    
	/**
	* @brief SDL Camera for game Window
	**/
    SDL_FRect camera;

    /**
	* @brief Check Is Running
	**/
	bool isRunning;


    bool isDebug;

    /**
     * @brief Store previous frame milliseconds
     */
    uint64_t millisecondsPreviousFrame;

    /**
	* @brief The difference in ticks since the last frame in terms of seconds
	*/
	double deltaTime;

    /**
	* @brief Calculate frame time to wait depending on last frame delivery speed
	*/
    int frameTimeToWait;

    Unique<ECSBridge> ecsBridge;

    Unique<AssetBridge> assetBridge;

    Unique<EventBridge> eventBridge;

};