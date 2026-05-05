#pragma once

#include <ECS.hpp>
#include <SDL3/SDL.h>

#include <EventBridge.hpp>

#include <GlobalCastAlias.hpp>
#include <GlobalPointerAlias.hpp>
#include <GlobalSDLKeysAlias.hpp>

#include <Log.hpp>

#include <KeyPressedEvent.hpp>
#include <ShootProjectileEvent.hpp>

#include <SpriteComponent.hpp>
#include <RigidBodyComponent.hpp>
#include <KeyBoardControllerComponent.hpp>

using std::string;
using std::to_string;


class KeyboardControlSystem : public System
{




public:

    EventBridge &eventBridge;

    KeyboardControlSystem(Unique<EventBridge> &eventBridge) : eventBridge(*eventBridge)
    {
      
        SetRequiredComponent<SpriteComponent>();
        SetRequiredComponent<RigidBodyComponent>();
        SetRequiredComponent<KeyboardControllerComponent>();
    }   

    void SubscribeToEvents()
    {
        eventBridge.Subscribe<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
    }

    void OnKeyPressed(KeyPressedEvent &event)
    {

        for (Entity entity: GetEntities())
        {
            SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();
            RigidBodyComponent& rb = entity.GetComponent<RigidBodyComponent>();
            const KeyboardControllerComponent keyboardControl = entity.GetComponent<KeyboardControllerComponent>();


            switch (event.symbol)
            {
                case key::up:

                    rb.velocity = keyboardControl.upVelocity;
                    sprite.srcRect.y = sprite.height * 0;


                break;

                case key::down:
                    rb.velocity = keyboardControl.downVelocity;
                    sprite.srcRect.y = sprite.height * 2;
                    

                break;

                case key::left:
                    rb.velocity = keyboardControl.leftVelocity;
                    sprite.srcRect.y = sprite.height * 3;
                    

                break;

                case key::right:
                    rb.velocity = keyboardControl.rightVelocity;
                    sprite.srcRect.y = sprite.height * 1;
                break;

                case key::space:
                    eventBridge.Broadcast<ShootProjectileEvent>(true);
                    Log::Custom<false>("Is Shooting", Color_Magenta, "Keyboard Control System");
                break;



            }
        }

        string keyCode = to_string(event.symbol);
        string keySymbol(1, event.symbol);

        Log::Custom<false>("[" + keyCode + "] " + keySymbol, Color_Magenta, "Keyboard Control System");
    };

};