
#pragma once

#include <GameInteraction/GameInteractionTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace GameInteraction
{
    class GameInteractionRequests
    {
    public:
        AZ_RTTI(GameInteractionRequests, GameInteractionRequestsTypeId);
        virtual ~GameInteractionRequests() = default;
        // Put your public methods here
    };

    class GameInteractionBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using GameInteractionRequestBus = AZ::EBus<GameInteractionRequests, GameInteractionBusTraits>;
    using GameInteractionInterface = AZ::Interface<GameInteractionRequests>;

} // namespace GameInteraction
