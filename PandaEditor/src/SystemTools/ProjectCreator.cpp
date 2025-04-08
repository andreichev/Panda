//
// Created by Michael Andreichev on 08.09.2024.
//

#include "ProjectCreator.hpp"

#include <Fern/Fern.hpp>
#include <fstream>
#include <sstream>

namespace Panda {

std::string replaceAll(std::string str, const std::string &from, const std::string &to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

void copyAndReplace(
    const path_t &src, const path_t &dest, const std::string &oldText, const std::string &newText
) {
    namespace fs = std::filesystem;
    for (const auto &entry : fs::recursive_directory_iterator(src)) {
        const auto &path = entry.path();
        auto relativePath = fs::relative(path, src);
        auto newPath = dest / relativePath;

        std::string newFilename = replaceAll(newPath.filename().string(), oldText, newText);
        newPath = newPath.parent_path() / newFilename;

        if (fs::is_directory(path)) {
            fs::create_directories(newPath);
        } else {
            std::ifstream inFile(path, std::ios::binary);
            std::ofstream outFile(newPath, std::ios::binary);

            std::stringstream buffer;
            buffer << inFile.rdbuf();
            std::string content = buffer.str();

            content = replaceAll(content, oldText, newText);

            outFile << content;

            inFile.close();
            outFile.close();
        }
    }
}

void ProjectCreator::createProject(const std::string &name, const Panda::path_t &path) {
    std::filesystem::create_directory(path);
    std::filesystem::create_directory(path / "Assets");
    std::filesystem::create_directory(path / "Scripts");
    path_t scriptingProjectPath = path / "Scripts" / name;
    std::filesystem::create_directory(scriptingProjectPath);

    copyAndReplace(
        Fern::getStaticResourcesPath() / "Templates" / "NewCppProject",
        scriptingProjectPath,
        "___PROJECTNAME___",
        name
    );
}

} // namespace Panda