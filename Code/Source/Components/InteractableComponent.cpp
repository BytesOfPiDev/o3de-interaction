#include "Components/InteractableComponent.h"

#include <AzCore/Console/ILogger.h>
#include <AzFramework/Entity/GameEntityContextComponent.h>
#include <AzFramework/Physics/Common/PhysicsSimulatedBodyEvents.h>
#include <AzFramework/Physics/PhysicsSystem.h>

#include "GameInteraction/InteractorBus.h"
#include "InteractableComponent.h"

namespace bop
{
    namespace Helper
    {
        inline auto GetEntityName(AZ::EntityId const& entityId) -> AZStd::string
        {
            AZStd::string result{};
            AzFramework::GameEntityContextRequestBus::BroadcastResult(
                result, &AzFramework::GameEntityContextRequestBus::Events::GetEntityName, entityId);

            return AZStd::move(result);
        }
    } // namespace Helper

    void InteractableComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<InteractableComponent, AZ::Component>()->Version(1);

            if (auto editContext = serialize->GetEditContext())
            {
                editContext->Class<InteractableComponent>("InteractableComponent", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "BoP/Interaction")
                    ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, true);
            }
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->Class<InteractableComponent>("InteractableComponent")
                ->Attribute(AZ::Script::Attributes::Category, "BoP/Interaction")
                ->RequestBus("InteractableRequestBus")
                ->NotificationBus("InteractorNotificationBus");
        }
    }

    void InteractableComponent::CheckForSimulatedBody()
    {
        auto* const simulatedBody = [this]() -> AzPhysics::SimulatedBody*
        {
            AzPhysics::SimulatedBody* result = nullptr;
            AzPhysics::SimulatedBodyComponentRequestsBus::EventResult(
                result, GetEntityId(), &AzPhysics::SimulatedBodyComponentRequestsBus::Events::GetSimulatedBody);
            return result;
        }();

        if (simulatedBody)
        {
            simulatedBody->RegisterOnTriggerEnterHandler(m_onTriggerEnterHandler);
            simulatedBody->RegisterOnTriggerExitHandler(m_onTriggerExitHandler);
            AZ_Trace("InteractableComponent", "Found simulated body.\n");
            return;
        }
        AZ_Trace("InteractableComponent", "Simulated body required, but none found. Checking again soon...\n");
        m_tickEvent.Requeue(AZ::TimeMs(5));
    }

    auto InteractableComponent::GetOtherEntityId(AzPhysics::TriggerEvent const& triggerEvent) const -> AZ::EntityId
    {
        return GetEntityId() == triggerEvent.m_triggerBody->GetEntityId() ? triggerEvent.m_otherBody->GetEntityId()
                                                                          : triggerEvent.m_triggerBody->GetEntityId();
    }

    void InteractableComponent::Init()
    {
        m_onTriggerEnterHandler = AzPhysics::SimulatedBodyEvents::OnTriggerEnter::Handler(
            [this](AzPhysics::SimulatedBodyHandle, AzPhysics::TriggerEvent const& triggerEvent) -> void
            {
                // It's possible the "other" entity could be us if our entity has multiple colliders.
                // Only call trigger if it is not the entity that owns us.
                OnTriggerEnter(triggerEvent);
            });

        m_onTriggerExitHandler = AzPhysics::SimulatedBodyEvents::OnTriggerExit::Handler(
            [this](AzPhysics::SimulatedBodyHandle, AzPhysics::TriggerEvent const& triggerEvent) -> void
            {
                // It's possible the "other" entity could be us if our entity has multiple colliders.
                // Only call trigger if it is not the entity that owns us.
                OnTriggerExit(triggerEvent);
            });

        // Despite listing a rigid body as a required service, a simulated body handle isn't guaranteed to exist after the component
        // providing the service has activated. For example, the PhysX Character Component creates the handle after getting the
        // OnEntityActivated activated notification for the entity it is attached to. As a result, we'll check periodically until we get the
        // handle, which *should* be guaranteed to be created since we've listed rigid bodies as a required service.
        m_tickEvent.Enqueue(AZ::TimeMs(5));
        // NOLINTNEXTLINE
        AZLOG(LOG_Interaction, "InteractableComponent[%s] initialized.", GetEntity()->GetName().c_str());
    }

    void InteractableComponent::Activate()
    {
        // NOLINTNEXTLINE
        AZLOG(LOG_Interaction, "InteractableComponent[%s] activated.", GetEntity()->GetName().c_str());
    }

    void InteractableComponent::Deactivate()
    {
        m_onTriggerEnterHandler.Disconnect();
        m_onTriggerExitHandler.Disconnect();

        // NOLINTNEXTLINE
        AZLOG(LOG_Interaction, "InteractableComponent[%s] deactivated.", GetEntity()->GetName().c_str());
    }

    void InteractableComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("InteractableService"));
    }

    void InteractableComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("InteractableService"));
    }

    void InteractableComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        required.push_back(AZ_CRC_CE("PhysicsRigidBodyService"));
        required.push_back(AZ_CRC_CE("PhysicsTriggerService"));
    }

    void InteractableComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& /*dependent*/)
    {
    }

    void InteractableComponent::OnTriggerEnter(AzPhysics::TriggerEvent const& triggerEvent)
    {
        // Check which body is not us to determine if we're the "trigger" or the "other" body.
        // Not sure why we're not always the trigger if the event is being sent to us, but testing
        // showed our entity as the other.
        auto const otherEntityId = GetOtherEntityId(triggerEvent);
        m_entitiesInTriggerArea.insert(otherEntityId);

        AZStd::string otherEntityName = Helper::GetEntityName(otherEntityId);
        // NOLINTNEXTLINE
        AZLOG(
            LOG_Interaction,
            "InteractableComponent['%s'] Entity [%s] entered the trigger area.",
            GetEntity()->GetName().c_str(),
            otherEntityName.c_str());

        InteractorNotificationBus::Event(otherEntityId, &InteractorNotificationBus::Events::OnInteractionAvailable, GetEntityId());
    }

    void InteractableComponent::OnTriggerExit(AzPhysics::TriggerEvent const& triggerEvent)
    {
        auto const otherEntityId = GetOtherEntityId(triggerEvent);
        m_entitiesInTriggerArea.insert(otherEntityId);

        auto otherEntityName = Helper::GetEntityName(otherEntityId);
        // NOLINTNEXTLINE
        AZLOG(
            LOG_Interaction,
            "InteractableComponent['%s'] Entity [%s] exited the trigger area.",
            GetEntity()->GetName().c_str(),
            otherEntityName.c_str());

        InteractorNotificationBus::Event(otherEntityId, &InteractorNotificationBus::Events::OnInteractionUnavailable, GetEntityId());
    }

    void InteractableComponent::Interact(AZ::EntityId const& /*interactorEntityId*/){};

} // namespace bop
