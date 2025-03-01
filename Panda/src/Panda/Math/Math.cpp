//
// Created by Michael Andreichev on 06.02.2024.
//

#include "Panda/Math/Math.hpp"

#include <Foundation/Foundation.hpp>

namespace Panda {

namespace Math {

    glm::vec3 scaleVec(const glm::vec3 &v, float desiredLength) {
        return v * desiredLength / length(v);
    }

    glm::vec3 smoothDamp(
        glm::vec3 current,
        glm::vec3 target,
        glm::vec3 &currentVelocity,
        float smoothTime,
        float deltaTime,
        float maxSpeed
    ) {
        float output_x = 0.f;
        float output_y = 0.f;
        float output_z = 0.f;

        // Based on Game Programming Gems 4 Chapter 1.10
        smoothTime = Foundation::max(0.0001F, smoothTime);
        float omega = 2.F / smoothTime;

        float x = omega * deltaTime;
        float exp = 1.F / (1.F + x + 0.48F * x * x + 0.235F * x * x * x);

        float change_x = current.x - target.x;
        float change_y = current.y - target.y;
        float change_z = current.z - target.z;
        glm::vec3 originalTo = target;

        // Clamp maximum speed
        float maxChange = maxSpeed * smoothTime;

        float maxChangeSq = maxChange * maxChange;
        float sqrmag = change_x * change_x + change_y * change_y + change_z * change_z;
        if (sqrmag > maxChangeSq) {
            float mag = (float)sqrt(sqrmag);
            change_x = change_x / mag * maxChange;
            change_y = change_y / mag * maxChange;
            change_z = change_z / mag * maxChange;
        }

        target.x = current.x - change_x;
        target.y = current.y - change_y;
        target.z = current.z - change_z;

        float temp_x = (currentVelocity.x + omega * change_x) * deltaTime;
        float temp_y = (currentVelocity.y + omega * change_y) * deltaTime;
        float temp_z = (currentVelocity.z + omega * change_z) * deltaTime;

        currentVelocity.x = (currentVelocity.x - omega * temp_x) * exp;
        currentVelocity.y = (currentVelocity.y - omega * temp_y) * exp;
        currentVelocity.z = (currentVelocity.z - omega * temp_z) * exp;

        output_x = target.x + (change_x + temp_x) * exp;
        output_y = target.y + (change_y + temp_y) * exp;
        output_z = target.z + (change_z + temp_z) * exp;

        float origMinusCurrent_x = originalTo.x - current.x;
        float origMinusCurrent_y = originalTo.y - current.y;
        float origMinusCurrent_z = originalTo.z - current.z;
        float outMinusOrig_x = output_x - originalTo.x;
        float outMinusOrig_y = output_y - originalTo.y;
        float outMinusOrig_z = output_z - originalTo.z;

        if (origMinusCurrent_x * outMinusOrig_x + origMinusCurrent_y * outMinusOrig_y +
                origMinusCurrent_z * outMinusOrig_z >
            0) {
            output_x = originalTo.x;
            output_y = originalTo.y;
            output_z = originalTo.z;

            currentVelocity.x = (output_x - originalTo.x) / deltaTime;
            currentVelocity.y = (output_y - originalTo.y) / deltaTime;
            currentVelocity.z = (output_z - originalTo.z) / deltaTime;
        }

        return glm::vec3(output_x, output_y, output_z);
    }

} // namespace Math

} // namespace Panda