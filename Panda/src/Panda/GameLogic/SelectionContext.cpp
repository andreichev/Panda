#include "Panda/GameLogic/SelectionContext.hpp"

namespace Panda {

std::unordered_set<UUID> SelectionContext::m_selectedAssets;
std::unordered_set<UUID> SelectionContext::m_selectedEntities;
std::unordered_set<UUID> SelectionContext::m_manipulatingEntities;
glm::vec3 SelectionContext::m_medianPosition;
glm::vec3 SelectionContext::m_medianScale;
glm::quat SelectionContext::m_medianRotation;
glm::mat4 SelectionContext::m_medianMatrix;

} // namespace Panda