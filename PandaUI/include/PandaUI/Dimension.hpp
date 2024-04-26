#pragma once

#include "PandaUI/Base.hpp"

namespace PandaUI {

enum class DimensionUnit {
    Auto,
    Points,
    Fraction,
};

class Dimension {
public:
    DimensionUnit unit;
    float value;

    static Dimension createDimension(DimensionUnit unit, float value) {
        if (unit == DimensionUnit::Auto) {
            PND_ASSERT(value == 0, "Dimension auto value must be 0.");
        } else if (unit == DimensionUnit::Points) {
            PND_ASSERT_F(
                value >= 0 && !isinf(value), "Value must be a real positive integer: %f.", value
            );
        } else if (unit == DimensionUnit::Fraction) {
            PND_ASSERT_F(
                0 <= value && value <= 1.0,
                "Dimension fraction value (%f) must be between 0 and 1.",
                value
            );
        }
        return {unit, value};
    }

    Dimension()
        : unit(DimensionUnit::Points)
        , value(0) {}

    Dimension(DimensionUnit unit, float value)
        : unit(unit)
        , value(value) {}
};

} // namespace PandaUI