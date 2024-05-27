#include "Primitives.hpp"

namespace Rain {

TypeId PrimitiveTypes::INT = findOrCreateType<int>().id;
TypeId PrimitiveTypes::FLOAT = findOrCreateType<float>().id;
TypeId PrimitiveTypes::STRING = findOrCreateType<std::string>().id;

} // namespace Rain