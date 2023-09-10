
#include <AzCore/Memory/Memory.h>

#include "GameInteractionModuleInterface.h"

#include "Clients/GameInteractionSystemComponent.h"
#include "Components/InteractableComponent.h"
#include "GameInteraction/GameInteractionTypeIds.h"

namespace GameInteraction
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(GameInteractionModuleInterface, "GameInteractionModuleInterface", GameInteractionModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(GameInteractionModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(GameInteractionModuleInterface, AZ::SystemAllocator);

    GameInteractionModuleInterface::GameInteractionModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(
            m_descriptors.end(),
            {
                GameInteractionSystemComponent::CreateDescriptor(),
                bop::InteractableComponent::CreateDescriptor(),

            });
    }

    AZ::ComponentTypeList GameInteractionModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<GameInteractionSystemComponent>(),
        };
    }
} // namespace GameInteraction
