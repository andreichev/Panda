#include "Panda/ImGui/ImGuiFonts.hpp"
#include "Panda/Assets/AssetHandler.hpp"
#include "Panda/Assets/AssetLoaderEditor.hpp"

#include <Foundation/PlatformDetection.hpp>
#include <unordered_map>
#include <Fern/Fern.hpp>

namespace Panda {

static std::unordered_map<std::string, ImFont *> s_fonts;

void Fonts::add(const FontConfiguration &config, bool isDefault) {
    PND_ASSERT_F(
        s_fonts.find(config.fontName) == s_fonts.end(),
        "Tried to add font with name '%s' but that name is already taken!",
        config.fontName.c_str()
    );
    ImFontConfig imguiFontConfig;
    imguiFontConfig.MergeMode = config.mergeWithLast;
    imguiFontConfig.OversampleH = 4;
    imguiFontConfig.OversampleV = 4;
    auto &io = ImGui::GetIO();
#ifdef PLATFORM_ANDROID
    path_t fontPath = path_t("default-fonts") / config.fileName;
    auto inputStream = Fern::createStaticResourceReader(fontPath);
    size_t fileSize = inputStream->totalSize();
    PND_ASSERT(fileSize != 0, "UNKNOWN ASSET SIZE");
    void *fontMem = malloc(fileSize);
    inputStream->readData(fontMem, fileSize);
    ImFont *font = io.Fonts->AddFontFromMemoryTTF(
        fontMem,
        fileSize,
        config.size,
        &imguiFontConfig,
        config.glyphRanges == nullptr ? io.Fonts->GetGlyphRangesDefault() : config.glyphRanges
    );
    Fern::disposeResourceReader(inputStream);
#else
    path_t fontPath = Fern::getStaticResourcesPath() / "default-fonts" / config.fileName;
    ImFont *font = io.Fonts->AddFontFromFileTTF(
        fontPath.string().c_str(),
        config.size,
        &imguiFontConfig,
        config.glyphRanges == nullptr ? io.Fonts->GetGlyphRangesDefault() : config.glyphRanges
    );
#endif
    PND_ASSERT(font != nullptr, "Failed to load font file!");
    s_fonts[config.fontName] = font;
    if (isDefault) { io.FontDefault = font; }
}

ImFont *Fonts::get(const std::string &fontName) {
    PND_ASSERT(s_fonts.find(fontName) != s_fonts.end(), "Failed to find font with that name!");
    return s_fonts.at(fontName);
}

void Fonts::pushFont(const std::string &fontName) {
    const auto &io = ImGui::GetIO();
    if (s_fonts.find(fontName) == s_fonts.end()) {
        ImGui::PushFont(io.FontDefault);
        return;
    }
    ImGui::PushFont(s_fonts.at(fontName));
}

void Fonts::popFont() {
    ImGui::PopFont();
}

} // namespace Panda
