
#include "GameInteractionSystemComponent.h"

#include <GameInteraction/GameInteractionTypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace GameInteraction
{
    AZ_COMPONENT_IMPL(GameInteractionSystemComponent, "GameInteractionSystemComponent",
        GameInteractionSystemComponentTypeId);

    void GameInteractionSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GameInteractionSystemComponent, AZ::Component>()
                ->Version(0)
                ;
        }
    }

    void GameInteractionSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("GameInteractionService"));
    }

    void GameInteractionSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("GameInteractionService"));
    }

    void GameInteractionSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void GameInteractionSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    GameInteractionSystemComponent::GameInteractionSystemComponent()
    {
        if (GameInteractionInterface::Get() == nullptr)
        {
            GameInteractionInterface::Register(this);
        }
    }

    GameInteractionSystemComponent::~GameInteractionSystemComponent()
    {
        if (GameInteractionInterface::Get() == this)
        {
            GameInteractionInterface::Unregister(this);
        }
    }

    void GameInteractionSystemComponent::Init()
    {
    }

    void GameInteractionSystemComponent::Activate()
    {
        GameInteractionRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
    }

    void GameInteractionSystemComponent::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
        GameInteractionRequestBus::Handler::BusDisconnect();
    }

    void GameInteractionSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }

} // namespace GameInteraction
