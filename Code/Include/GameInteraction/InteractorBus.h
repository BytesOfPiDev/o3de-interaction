#pragma once

#include <AzCore/Component/ComponentBus.h>

namespace bop
{
    //! @brief Notifications that are sent out to any interactors.
    //!
    //! Typically, @class InteractableComponent will send out these notifications to the entity that is interacting with it.
    class InteractorNotifications
    {
    public:
        AZ_DISABLE_COPY_MOVE(InteractorNotifications); // NOLINT
        AZ_RTTI(InteractorNotifications, "FADF4DDC-EF5C-46AA-836E-462845D33AE8"); // NOLINT

        InteractorNotifications() = default;
        virtual ~InteractorNotifications() = default;

        //! @brief Signaled when an entity is available for interaction.
        // 1 @param availableEntityId The entity that is available for interaction.
        virtual void OnInteractionAvailable(AZ::EntityId const& availableEntityId) = 0;
        //! @brief Signaled when an entity is no longer available for interaction.
        //! @param unavailableEntityId The entity that is no longer available for interaction.
        virtual void OnInteractionUnavailable(AZ::EntityId const& unavailableEntityId) = 0;
    };

    using InteractorNotificationBus = AZ::EBus<InteractorNotifications, AZ::ComponentBus>;

} // namespace bop
