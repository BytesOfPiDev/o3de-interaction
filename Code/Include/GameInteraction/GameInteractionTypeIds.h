
#pragma once

namespace GameInteraction
{
    // System Component TypeIds
    inline constexpr const char* GameInteractionSystemComponentTypeId = "{EE1FAC5A-0F11-49C0-956E-5257A69FB18C}";
    inline constexpr const char* GameInteractionEditorSystemComponentTypeId = "{7A70EEDB-3B4D-4CF7-99D6-46662AFF5744}";

    // Module derived classes TypeIds
    inline constexpr const char* GameInteractionModuleInterfaceTypeId = "{D81C21F2-8993-403F-8276-C2E0774EBE38}";
    inline constexpr const char* GameInteractionModuleTypeId = "{19F4AA3C-D68E-41D9-9D6B-EC5F35F138D5}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* GameInteractionEditorModuleTypeId = GameInteractionModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* GameInteractionRequestsTypeId = "{184FEC4E-F90B-4AD2-A191-71FCB093CB7C}";
} // namespace GameInteraction
