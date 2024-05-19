//
// Created by Michael Andreichev on 04.05.2024.
//

#pragma once

#include <string>

namespace Panda {

class Encoder {};

class Encodable {
    void encode(Encoder e);
};

class Decodable {};

class Codable : public Encodable, Decodable {};

} // namespace Panda
