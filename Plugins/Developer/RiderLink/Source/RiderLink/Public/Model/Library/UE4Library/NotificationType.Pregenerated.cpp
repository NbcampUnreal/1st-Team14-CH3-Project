//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a RdGen v1.13.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------
#include "NotificationType.Pregenerated.h"

namespace JetBrains {
namespace EditorPlugin {
std::string to_string(const NotificationType & value)
{
    switch(value) {
    case NotificationType::Message: return "Message";
    case NotificationType::Error: return "Error";
    default: return std::to_string(static_cast<int32_t>(value));
    }
}
}
}
