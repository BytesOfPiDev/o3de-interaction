
#include <GameInteraction/GameInteractionTypeIds.h>
#include <GameInteractionModuleInterface.h>
#include "GameInteractionSystemComponent.h"

namespace GameInteraction
{
    class GameInteractionModule
        : public GameInteractionModuleInterface
    {
    public:
        AZ_RTTI(GameInteractionModule, GameInteractionModuleTypeId, GameInteractionModuleInterface);
        AZ_CLASS_ALLOCATOR(GameInteractionModule, AZ::SystemAllocator);
    };
}// namespace GameInteraction

AZ_DECLARE_MODULE_CLASS(Gem_GameInteraction, GameInteraction::GameInteractionModule)
