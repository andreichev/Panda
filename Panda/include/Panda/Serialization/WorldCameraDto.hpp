#pragma once

#include "Panda/GameLogic/Components/BaseComponents.hpp"

#include <Rain/Rain.hpp>
#include <Rain/Codable.hpp>

namespace Panda {

struct WorldCameraDto : public Rain::Codable {
    std::string projectionType;
    float fieldOfView;
    float orthoSize;
    float zNear;
    float zFar;

    RAIN_FIELDS_BEGIN(WorldCameraDto)
    RAIN_FIELD(projectionType)
    RAIN_FIELD(fieldOfView)
    RAIN_FIELD(orthoSize)
    RAIN_FIELD(zNear)
    RAIN_FIELD(zFar)
    RAIN_FIELDS_END
};

struct CameraComponentDto : public Rain::Codable {
    bool isPrimary;
    WorldCameraDto camera;

    RAIN_FIELDS_BEGIN(CameraComponentDto)
    RAIN_FIELD(isPrimary)
    RAIN_FIELD(camera)
    RAIN_FIELDS_END
};

} // namespace Panda