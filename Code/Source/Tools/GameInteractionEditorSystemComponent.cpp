
#include <AzCore/Serialization/SerializeContext.h>
#include "GameInteractionEditorSystemComponent.h"

#include <GameInteraction/GameInteractionTypeIds.h>

namespace GameInteraction
{
    AZ_COMPONENT_IMPL(GameInteractionEditorSystemComponent, "GameInteractionEditorSystemComponent",
        GameInteractionEditorSystemComponentTypeId, BaseSystemComponent);

    void GameInteractionEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GameInteractionEditorSystemComponent, GameInteractionSystemComponent>()
                ->Version(0);
        }
    }

    GameInteractionEditorSystemComponent::GameInteractionEditorSystemComponent() = default;

    GameInteractionEditorSystemComponent::~GameInteractionEditorSystemComponent() = default;

    void GameInteractionEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("GameInteractionEditorService"));
    }

    void GameInteractionEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("GameInteractionEditorService"));
    }

    void GameInteractionEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void GameInteractionEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void GameInteractionEditorSystemComponent::Activate()
    {
        GameInteractionSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void GameInteractionEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        GameInteractionSystemComponent::Deactivate();
    }

} // namespace GameInteraction
