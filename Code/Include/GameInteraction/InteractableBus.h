#pragma once

#include <AzCore/Component/ComponentBus.h>

namespace bop
{
    //! @brief Used to configure interactable functionality on an entity.
    //!
    //! @todo Implement requests for disabling/enabling interaction.
    class InteractableRequests
    {
    public:
        AZ_DISABLE_COPY_MOVE(InteractableRequests); // NOLINT
        AZ_RTTI(InteractableRequests, "97B88B2C-6549-4ABD-AE4C-CA615745FA9B"); // NOLINT

        InteractableRequests() = default;
        virtual ~InteractableRequests() = default;

        //! @brief Signals that an entity is interacting with this interactable.
        //@param interactorEntityId The entity that is interacting with this interactable.
        //!
        //! @note No real work is done other than sending out notifications about the interaction. Listeners to those notifications are the
        //! ones responsible for implementing any interesting functionality when someone interacts with the interactable.
        virtual void Interact(AZ::EntityId const& interactorEntityId) = 0;
    };

    using InteractableRequestBus = AZ::EBus<InteractableRequests, AZ::ComponentBus>;
} // namespace bop
