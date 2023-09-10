
#include <GameInteraction/GameInteractionTypeIds.h>
#include <GameInteractionModuleInterface.h>
#include "GameInteractionEditorSystemComponent.h"

namespace GameInteraction
{
    class GameInteractionEditorModule
        : public GameInteractionModuleInterface
    {
    public:
        AZ_RTTI(GameInteractionEditorModule, GameInteractionEditorModuleTypeId, GameInteractionModuleInterface);
        AZ_CLASS_ALLOCATOR(GameInteractionEditorModule, AZ::SystemAllocator);

        GameInteractionEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                GameInteractionEditorSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<GameInteractionEditorSystemComponent>(),
            };
        }
    };
}// namespace GameInteraction

AZ_DECLARE_MODULE_CLASS(Gem_GameInteraction, GameInteraction::GameInteractionEditorModule)
