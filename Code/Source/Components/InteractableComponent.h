#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/Component/EntityBus.h>
#include <AzCore/EBus/ScheduledEvent.h>
#include <AzFramework/Physics/Collision/CollisionEvents.h>
#include <AzFramework/Physics/Common/PhysicsSimulatedBodyEvents.h>
#include <AzFramework/Physics/Components/SimulatedBodyComponentBus.h>

#include "GameInteraction/GameInteractionTypeIds.h"
#include "GameInteraction/InteractableBus.h"

namespace AzPhysics
{
    class SimulatedBodyComponentRequests;
}

namespace bop
{
    class InteractableComponent
        : public AZ::Component
        , private InteractableRequestBus::Handler
    {
    public:
        AZ_COMPONENT(InteractableComponent, InteractableComponentTypeId); // NOLINT

        static void Reflect(AZ::ReflectContext* context);

        void Init() override;
        void Activate() override;
        void Deactivate() override;

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

    protected:
        void OnTriggerEnter(AzPhysics::TriggerEvent const& triggerEvent);
        void OnTriggerExit(AzPhysics::TriggerEvent const& triggerEvent);

        void Interact(AZ::EntityId const& interactorEntityId) override;

    private:
        void CheckForSimulatedBody();
        //! @brief Helper function to find out which body/entity is the "other" trigger.
        auto GetOtherEntityId(AzPhysics::TriggerEvent const& triggerEvent) const -> AZ::EntityId;

    private:
        AzPhysics::SimulatedBodyEvents::OnTriggerEnter::Handler m_onTriggerEnterHandler;
        AzPhysics::SimulatedBodyEvents::OnTriggerExit::Handler m_onTriggerExitHandler;
        //! The entities that are currently in the trigger area.
        AZStd::unordered_set<AZ::EntityId> m_entitiesInTriggerArea{};
        AZ::ScheduledEvent m_tickEvent{ [this]()
                                        {
                                            CheckForSimulatedBody();
                                        },
                                        AZ::Name("InteractionComponentCheckForSimBody") };
    };
} // namespace bop
