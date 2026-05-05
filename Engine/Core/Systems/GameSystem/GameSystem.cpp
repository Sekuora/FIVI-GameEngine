#include "GameSystem.hpp"

#include <ECS.hpp>

#include <Log.hpp>

#include <GlobalPointerAlias.hpp>
#include <GlobalCastAlias.hpp>

// Components
#include <TransformComponent.hpp>
#include <RigidBodyComponent.hpp>
#include <SpriteComponent.hpp>
#include <AnimationComponent.hpp>
#include <BoxColliderComponent.hpp>
#include <KeyBoardControllerComponent.hpp>
#include <CameraFollowComponent.hpp>
#include <ProjectileEmitterComponent.hpp>
#include <HealthComponent.hpp>

// Systems
#include <RenderSystem.hpp>
#include <MovementSystem.hpp>
#include <AnimationSystem.hpp>
#include <CollisionSystem.hpp>
#include <RenderColliderSystem.hpp>
#include <DamageSystem.hpp>
#include <KeyboardControlSystem.hpp>
#include <CameraMovementSystem.hpp>
#include <ProjectileEmitSystem.hpp>
#include <ProjectileLifeCycleSystem.hpp>

// Bridges
#include <AssetBridge.hpp>
#include <EventBridge.hpp>

#include <SDL3/SDL.h>
#include <SDL3/SDL_time.h>
#include <SDL3_image/SDL_image.h>
#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <cassert>

int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth;
int Game::mapHeight;

using std::fstream;
using std::getline;
using std::isdigit;

using std::atoi;

using glm::vec2;

const uint16_t FIVI_fps = 1000;
const float FIVI_milliseconds = 1000.0f;
const float FIVI_millisecondsPerFrame = FIVI_milliseconds / FIVI_fps;

Game::Game()
{
    isRunning = false;

    isDebug = false;

    ecsBridge = newU<ECSBridge>();

    assetBridge = newU<AssetBridge>();

    eventBridge = newU<EventBridge>();

    window = nullptr;
    renderer = nullptr;

    windowTitle = "Fivi Game";
    windowWidth = 1920;
    windowHeight = 1080;

    frameTimeToWait = 0;

    deltaTime = 0;

    Log::Custom<false>("Game constructor called!", Color_Cyan, "System");
}

Game::~Game()
{
    Log::Custom<false>("Game destructor called!", Color_Cyan, "System");
}

void Game::Start()
{
    // Init SDL Video

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        Log::Error("There was an error initializing SDL.");
        assert(SDL_WasInit(SDL_INIT_VIDEO));
        return;
    }

    // Create Window and Renderer

    if (!SDL_CreateWindowAndRenderer(windowTitle, windowWidth, windowHeight, SDL_WINDOW_BORDERLESS, &window, &renderer))
    {
        Log::Error("There was an error creating a window and renderer. ");
        assert(window);
        return;
    }

    // True Fulscreen
    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    // Log::Default("Game window set to fullscreen!");


    // Camera view
    camera.x = 0;
    camera.y = 0;
    camera.w = windowWidth;
    camera.h = windowHeight;

    


    isRunning = true;
}

void Game::LoadLevel(int level)
{

    // Add systems to game
    ecsBridge->AddSystem<MovementSystem>();

    // Add Render System
    ecsBridge->AddSystem<RenderSystem>();

    // Add Animation System
    ecsBridge->AddSystem<AnimationSystem>();

    // Add Collision System
    ecsBridge->AddSystem<CollisionSystem>();

    // Add Assets to the Asset Store
    ecsBridge->AddSystem<RenderColliderSystem>();

    // Add Damage System
    ecsBridge->AddSystem<DamageSystem>();

    // Add Keyboard Control System
    ecsBridge->AddSystem<KeyboardControlSystem>(eventBridge);

    // Add Camera Movement System
    ecsBridge->AddSystem<CameraMovementSystem>();


    // Add Projectile Management systems
    ecsBridge->AddSystem<ProjectileEmitSystem>(ecsBridge);
    ecsBridge->AddSystem<ProjectileLifeCycleSystem>();

    // Load Textures

    assetBridge->AddTexture(renderer, "tank-image", "../../../Games/EngineShowcaseGame/Resources/images/tank-panther-right.png");
    assetBridge->AddTexture(renderer, "truck-image", "../../../Games/EngineShowcaseGame/Resources/images/truck-ford-right.png");
    assetBridge->AddTexture(renderer, "chopper-image", "../../../Games/EngineShowcaseGame/Resources/images/chopper-spritesheet.png");
    assetBridge->AddTexture(renderer, "radar-image", "../../../Games/EngineShowcaseGame/Resources/images/radar.png");

    assetBridge->AddTexture(renderer, "bullet-image", "../../../Games/EngineShowcaseGame/Resources/images/bullet.png");

    // Load Tilemap
    assetBridge->AddTexture(renderer, "jungle-image", "../../../Games/EngineShowcaseGame/Resources/tilemaps/jungle.png");



    int tileSize = 32;
    double tileScale = 3.0;
    int mapNumCols = 25.0;
    int mapNumRows = 20.0;

    string tilemapPath = "../../../Games/EngineShowcaseGame/Resources/tilemaps/jungle.map";

    fstream mapFile;

    mapFile.open(tilemapPath);

    for (int y = 0; y < mapNumRows; y++)
    {
        for (int x = 0; x < mapNumCols; x++)
        {
            char character;

            // Read first digit for Y index
            mapFile.get(character);

            int srcRectY = atoi(&character) * tileSize;

            // Read second digit for X index
            mapFile.get(character);
            int srcRectX = atoi(&character) * tileSize;

            // Skip the trailing comma or delimiter
            mapFile.ignore();

            Entity tile = ecsBridge->CreateEntity();

            // Corrected the vec2 constructor and position calculation
            tile.AddComponent<TransformComponent>(
                vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)),
                vec2(tileScale, tileScale),
                0.0);

            // Add sprite component with the calculated source rectangle
            tile.AddComponent<SpriteComponent>("jungle-image", tileSize, tileSize, 0, false, srcRectX, srcRectY);
        }
    }

 

    mapFile.close();
    
    mapWidth = mapNumCols * tileSize * tileScale;
    
    mapHeight = mapNumRows * tileSize * tileScale;
    

    // Entities

    // Chopper
    Entity chopper = ecsBridge->CreateEntity();

    // Create entity, add components
    chopper.AddComponent<TransformComponent>(vec2(10, 10), vec2(1.0, 1.0), 0.0);
    chopper.AddComponent<RigidBodyComponent>(vec2(0, 0));

    float chopperSpeedFactor = 10;
    
    chopper.AddComponent<KeyboardControllerComponent>(vec2(0, -20 * chopperSpeedFactor), vec2(0, 20 * chopperSpeedFactor), vec2(-20 * chopperSpeedFactor, 0), vec2(20 * chopperSpeedFactor, 0));
    chopper.AddComponent<CameraFollowComponent>();

    chopper.AddComponent<HealthComponent>(100);

    // Render Components
    chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 1);
    chopper.AddComponent<AnimationComponent>(2, 16, true);
    chopper.AddComponent<ProjectileEmitterComponent>(vec2(500.0, 500.0), 0, 3000, 0, true);

    // Radar
    Entity radar = ecsBridge->CreateEntity();

    // Create entity, add components


    radar.AddComponent<TransformComponent>(vec2(windowWidth - 74, 10), vec2(1.0, 1.0), 0.0);
    radar.AddComponent<RigidBodyComponent>(vec2(0, 0));

    // Render Components
    radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 2, true);
    radar.AddComponent<AnimationComponent>(8, 2, true);

    // Tank
    Entity tank = ecsBridge->CreateEntity();

    // Create entity, add components
    tank.AddComponent<TransformComponent>(vec2(10, 10), vec2(1.0, 1.0), 0.0);
    tank.AddComponent<RigidBodyComponent>(vec2(0, 0));
    tank.AddComponent<BoxColliderComponent>(32, 32);
    tank.AddComponent<ProjectileEmitterComponent>(vec2(100.0, 0.0), 5000, 3000, 0, false);

    tank.AddComponent<HealthComponent>(100);

    // Render Components
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 2);

    // Truck
    Entity truck = ecsBridge->CreateEntity();

    // Create entity, add components
    truck.AddComponent<TransformComponent>(vec2(500, 10), vec2(1, 1.0), 0.0);
    truck.AddComponent<RigidBodyComponent>(vec2(0, 0));
    truck.AddComponent<BoxColliderComponent>(32, 32);
    truck.AddComponent<ProjectileEmitterComponent>(vec2(0.0, 100.0), 2000, 5000, 0, false);
    // Render Components
    truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 1);

    truck.AddComponent<HealthComponent>(100);

}

void Game::Setup()
{

    LoadLevel(1);
}

void Game::Run()
{

    while (isRunning)
    {
        ProcessInput();
        Update();
        Render();
       
    }
}

void Game::End()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::ProcessInput()
{
    // Handle window events
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:

            isRunning = false;
            break;

        case SDL_EVENT_KEY_DOWN:
            if (event.key.key == SDLK_ESCAPE)
            {
                isRunning = false;
            }

            if (event.key.key == SDLK_D)
            {
                isDebug = !isDebug;
            }
            
            eventBridge->Broadcast<KeyPressedEvent>(event.key.key);
            break;
        }
    }
}

void Game::Update()
{
    // High precision timing
    uint64_t currentCounter = SDL_GetPerformanceCounter();
    uint64_t elapsedTicks = currentCounter - millisecondsPreviousFrame;
    uint64_t frequency = SDL_GetPerformanceFrequency();

    // Delta Time in seconds (e.g., 0.01666 for 60fps)
    deltaTime = scast<float>(elapsedTicks) / scast<float>(frequency);

    // Update the record for the next frame
    millisecondsPreviousFrame = currentCounter;

    // Clamp deltaTime (Max 20 FPS step) to prevent physics tunneling
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    /**
     * Frame capping (Wait if we are faster than target FPS)
     * We convert target frame time to Performance Counter units for precision
     */
    float targetTicksPerFrame = (scast<float>(frequency) / FIVI_fps);
    float currentFrameTicks = scast<float>(SDL_GetPerformanceCounter() - currentCounter);

    if (currentFrameTicks < targetTicksPerFrame)
    {
        float waitTimeMs = (targetTicksPerFrame - currentFrameTicks) * 1000.0f / scast<float>(frequency);
        SDL_Delay(scast<Uint32>(waitTimeMs));
    }

    // Reset Event Subscriptions
    eventBridge->Reset();

    // Subscribe to events
    ecsBridge->GetSystem<DamageSystem>().SubscribeToEvents(eventBridge);
    ecsBridge->GetSystem<KeyboardControlSystem>().SubscribeToEvents();
    ecsBridge->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(eventBridge);

    ecsBridge->Update();

    // Update ECS
    ecsBridge->GetSystem<MovementSystem>().Update(deltaTime);

    ecsBridge->GetSystem<AnimationSystem>().Update();

    ecsBridge->GetSystem<CollisionSystem>().Update(eventBridge);

    ecsBridge->GetSystem<CameraMovementSystem>().Update(camera);

    ecsBridge->GetSystem<ProjectileEmitSystem>().Update();

    ecsBridge->GetSystem<ProjectileLifeCycleSystem>().Update();

    
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    ecsBridge->GetSystem<RenderSystem>().Update(renderer, assetBridge, camera);

    if (isDebug)
    {
        ecsBridge->GetSystem<RenderColliderSystem>().Update(renderer, camera);
    }

    SDL_RenderPresent(renderer);
}
