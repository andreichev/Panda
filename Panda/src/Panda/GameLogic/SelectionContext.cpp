#include "Panda/GameLogic/SelectionContext.hpp"

namespace Panda {

int SelectionContext::s_selectedAssetsCount = 0;
std::unordered_set<path_t> SelectionContext::s_selectedFiles;
std::unordered_set<UUID> SelectionContext::s_selectedEntities;
std::unordered_set<UUID> SelectionContext::s_manipulatingEntities;
glm::vec3 SelectionContext::s_medianPosition;
glm::vec3 SelectionContext::s_medianScale;
glm::quat SelectionContext::s_medianRotation;
glm::mat4 SelectionContext::s_medianMatrix;

} // namespace Panda