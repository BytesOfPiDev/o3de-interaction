
#pragma once

#include <AzToolsFramework/API/ToolsApplicationAPI.h>

#include <Clients/GameInteractionSystemComponent.h>

namespace GameInteraction
{
    /// System component for GameInteraction editor
    class GameInteractionEditorSystemComponent
        : public GameInteractionSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = GameInteractionSystemComponent;
    public:
        AZ_COMPONENT_DECL(GameInteractionEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        GameInteractionEditorSystemComponent();
        ~GameInteractionEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace GameInteraction
