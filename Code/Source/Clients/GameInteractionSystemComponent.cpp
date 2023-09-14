
#include "GameInteractionSystemComponent.h"

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>

#include "GameInteraction/GameInteractionTypeIds.h"
#include "GameInteraction/InteractableBus.h"
#include "GameInteraction/InteractorBus.h"

namespace bop
{
    class InteractableRequestBusBehaviorHandler
        : public InteractableRequestBus::Handler
        , public AZ::BehaviorEBusHandler
    {
    public:
        AZ_EBUS_BEHAVIOR_BINDER_WITH_DOC(
            InteractableRequestBusBehaviorHandler,
            "{ECE33F21-D488-4FD9-A56D-866AF6B99748}",
            AZ::SystemAllocator,
            Interact,
            ({ "Interactor", "" }));

        ~InteractableRequestBusBehaviorHandler() override = default;

        void Interact(AZ::EntityId const& interactingEntityId) override
        {
            Call(FN_Interact, interactingEntityId);
        }
    };

    class InteractableNotificationBusBehaviorHandler
        : public InteractableNotificationBus::Handler
        , public AZ::BehaviorEBusHandler
    {
    public:
        AZ_EBUS_BEHAVIOR_BINDER_WITH_DOC(
            InteractableNotificationBusBehaviorHandler,
            "{28BE1FDB-80F0-48E5-B62D-B3C5FE0128DF}",
            AZ::SystemAllocator,
            OnTriggerAreaEntered,
            ({ "OnTriggerAreaEntered", "" }),
            OnTriggerAreaExited,
            ({ "OnTriggerAreaExited", "" }));

        ~InteractableNotificationBusBehaviorHandler() override = default;

        void OnTriggerAreaEntered(AZ::EntityId const& enteredEntityId) override
        {
            Call(FN_OnTriggerAreaEntered, enteredEntityId);
        }

        void OnTriggerAreaExited(AZ::EntityId const& exitedEntityId) override
        {
            Call(FN_OnTriggerAreaExited, exitedEntityId);
        }
    };

    //! @brief Used to reflect @class InteractorNotificationBus::Handler to the script context.
    class InteractorNotificationBusBehaviorHandler
        : public InteractorNotificationBus::Handler
        , public AZ::BehaviorEBusHandler
    {
    public:
        AZ_EBUS_BEHAVIOR_BINDER_WITH_DOC( // NOLINT
            InteractorNotificationBusBehaviorHandler,
            "250873CE-DC53-4E15-9FC3-ABC85CBF4FBC",
            AZ::SystemAllocator,
            OnInteractionAvailable,
            ({ "AvailableEntityId", "The entity id of an interactable that this interactor is now in the trigger area of." }),
            OnInteractionUnavailable,
            ({ "UnavailableEntityId", "The entity id of an interactable that this interactor is no longer in the trigger area of." }));

        void OnInteractionAvailable(AZ::EntityId const& availableEntityId) override
        {
            Call(FN_OnInteractionAvailable, availableEntityId);
        }

        void OnInteractionUnavailable(AZ::EntityId const& unavailableEntityId) override
        {
            Call(FN_OnInteractionUnavailable, unavailableEntityId);
        }
    };
} // namespace bop
namespace GameInteraction
{
    AZ_COMPONENT_IMPL(GameInteractionSystemComponent, "GameInteractionSystemComponent", GameInteractionSystemComponentTypeId);

    void GameInteractionSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<GameInteractionSystemComponent, AZ::Component>()->Version(0);
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->EBus<bop::InteractableRequestBus>("InteractableRequestBus")
                ->Attribute(AZ::Script::Attributes::Category, "BoP/Interaction")
                ->Handler<bop::InteractableRequestBusBehaviorHandler>()
                ->EventWithBus<bop::InteractableRequestBus>("Interact", &bop::InteractableRequests::Interact);

            behaviorContext->EBus<bop::InteractableNotificationBus>("InteractableNotificationBus")
                ->Attribute(AZ::Script::Attributes::Category, "Bop/Interaction")
                ->Handler<bop::InteractableNotificationBusBehaviorHandler>()
                ->EventWithBus<bop::InteractableNotificationBus>(
                    "OnInteractableTriggerEntered", &bop::InteractableNotificationBus::Events::OnTriggerAreaEntered)
                ->EventWithBus<bop::InteractableNotificationBus>(
                    "OnInteractableTriggerExited", &bop::InteractableNotificationBus::Events::OnTriggerAreaExited);

            behaviorContext->EBus<bop::InteractorNotificationBus>("InteractorNotificationBus")
                ->Attribute(AZ::Script::Attributes::Category, "BoP/Interaction")
                ->Handler<bop::InteractorNotificationBusBehaviorHandler>()
                ->EventWithBus<bop::InteractorNotificationBus>(
                    "OnInteractionAvailable", &bop::InteractorNotifications::OnInteractionAvailable)
                ->EventWithBus<bop::InteractorNotificationBus>(
                    "OnInteractionUnavailable", &bop::InteractorNotifications::OnInteractionUnavailable);
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
