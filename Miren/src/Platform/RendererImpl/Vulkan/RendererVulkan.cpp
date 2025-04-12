//
// Created by Admin on 14.03.2022.
//

#include "RendererVulkan.hpp"

#include <Foundation/Assert.hpp>
#include <Foundation/Allocator.hpp>
#include <Foundation/Logger.hpp>
#include <vulkan/vulkan.h>

const char *getResultToString(VkResult result) {
    switch (result) {
        case VK_SUCCESS:
            return "VK_SUCCESS";
        case VK_NOT_READY:
            return "VK_NOT_READY";
        case VK_TIMEOUT:
            return "VK_TIMEOUT";
        case VK_EVENT_SET:
            return "VK_EVENT_SET";
        case VK_EVENT_RESET:
            return "VK_EVENT_RESET";
        case VK_INCOMPLETE:
            return "VK_INCOMPLETE";
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            return "VK_ERROR_OUT_OF_HOST_MEMORY";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
        case VK_ERROR_INITIALIZATION_FAILED:
            return "VK_ERROR_INITIALIZATION_FAILED";
        case VK_ERROR_DEVICE_LOST:
            return "VK_ERROR_DEVICE_LOST";
        case VK_ERROR_MEMORY_MAP_FAILED:
            return "VK_ERROR_MEMORY_MAP_FAILED";
        case VK_ERROR_LAYER_NOT_PRESENT:
            return "VK_ERROR_LAYER_NOT_PRESENT";
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            return "VK_ERROR_EXTENSION_NOT_PRESENT";
        case VK_ERROR_FEATURE_NOT_PRESENT:
            return "VK_ERROR_FEATURE_NOT_PRESENT";
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            return "VK_ERROR_INCOMPATIBLE_DRIVER";
        case VK_ERROR_TOO_MANY_OBJECTS:
            return "VK_ERROR_TOO_MANY_OBJECTS";
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            return "VK_ERROR_FORMAT_NOT_SUPPORTED";
        case VK_ERROR_FRAGMENTED_POOL:
            return "VK_ERROR_FRAGMENTED_POOL";
        case VK_ERROR_UNKNOWN:
            return "VK_ERROR_UNKNOWN";
        case VK_ERROR_OUT_OF_POOL_MEMORY:
            return "VK_ERROR_OUT_OF_POOL_MEMORY";
        case VK_ERROR_INVALID_EXTERNAL_HANDLE:
            return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
        case VK_ERROR_FRAGMENTATION:
            return "VK_ERROR_FRAGMENTATION";
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
            return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
        case VK_PIPELINE_COMPILE_REQUIRED:
            return "VK_PIPELINE_COMPILE_REQUIRED";
        case VK_ERROR_SURFACE_LOST_KHR:
            return "VK_ERROR_SURFACE_LOST_KHR";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
        case VK_SUBOPTIMAL_KHR:
            return "VK_SUBOPTIMAL_KHR";
        case VK_ERROR_OUT_OF_DATE_KHR:
            return "VK_ERROR_OUT_OF_DATE_KHR";
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
        case VK_ERROR_VALIDATION_FAILED_EXT:
            return "VK_ERROR_VALIDATION_FAILED_EXT";
        case VK_ERROR_INVALID_SHADER_NV:
            return "VK_ERROR_INVALID_SHADER_NV";
        case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
            return "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
            return "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
            return "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
            return "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
            return "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
            return "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
            return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
        case VK_ERROR_NOT_PERMITTED_KHR:
            return "VK_ERROR_NOT_PERMITTED_KHR";
        case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
            return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
        case VK_THREAD_IDLE_KHR:
            return "VK_THREAD_IDLE_KHR";
        case VK_THREAD_DONE_KHR:
            return "VK_THREAD_DONE_KHR";
        case VK_OPERATION_DEFERRED_KHR:
            return "VK_OPERATION_DEFERRED_KHR";
        case VK_OPERATION_NOT_DEFERRED_KHR:
            return "VK_OPERATION_NOT_DEFERRED_KHR";
        case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR:
            return "VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR";
        case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
            return "VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
        case VK_INCOMPATIBLE_SHADER_BINARY_EXT:
            return "VK_INCOMPATIBLE_SHADER_BINARY_EXT";
        default:
            return "UNKNOWN_VK_RESULT";
    }
}

#if MIREN_LOG_ENABLED == 1
#    define VK_CHECK(result)                                                                       \
        if (result != VK_SUCCESS) {                                                                \
            LOG_ERROR("Vulkan error: %s", getResultToString(result));                              \
            assert(false);                                                                         \
        }
#else
#    define VK_CHECK(result)                                                                       \
        if (result != VK_SUCCESS) { assert(false); }
#endif

namespace Miren {

RendererVulkan::RendererVulkan(Fern::GraphicsContext *ctx)
    : m_uselessVao(0)
    , m_frameBuffers()
    , m_shaders()
    , m_indexBuffers()
    , m_vertexLayouts()
    , m_vertexBuffers()
    , m_textures()
    , m_ctx(ctx) {
    MIREN_LOG("Make instance...");
    uint32_t version;
    vkEnumerateInstanceVersion(&version);
    MIREN_LOG(
        "System can support Vulkan version: %d.%d.%d.%d",
        VK_API_VERSION_VARIANT(version),
        VK_API_VERSION_MAJOR(version),
        VK_API_VERSION_MINOR(version),
        VK_API_VERSION_PATCH(version)
    );

    // patch = 0
    version &= ~(0xFFFU);

    VkResult result = VK_SUCCESS;
    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Panda";
    appInfo.pEngineName = "Panda";
    appInfo.applicationVersion = version;
    appInfo.apiVersion = version;
    //    appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo instanceCreateInfo;

    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;


    // На macOS MoltenVK требует определенных расширений
    const char* extensions[] = {
        VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME, // Обязательно для MoltenVK
        VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME // Часто требуется
    };

    instanceCreateInfo.enabledExtensionCount = sizeof(extensions) / sizeof(extensions[0]);
    instanceCreateInfo.ppEnabledExtensionNames = extensions;

    // Устанавливаем флаг только если поддерживается
    instanceCreateInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance);
    VK_CHECK(result);
}

RendererVulkan::~RendererVulkan() {
    vkDestroyInstance(m_instance, nullptr);
}

RendererType RendererVulkan::getRendererType() const {
    return RendererType::Vulkan;
}

void RendererVulkan::createFrameBuffer(
    FrameBufferHandle handle, FrameBufferSpecification specification
) {}

void RendererVulkan::readFrameBuffer(
    FrameBufferHandle handle, int attachIndex, int x, int y, int width, int height, void *data
) {}

void RendererVulkan::deleteFrameBuffer(FrameBufferHandle handle) {}

void RendererVulkan::createProgram(ProgramHandle handle, ProgramCreate create) {}

void RendererVulkan::deleteShader(ProgramHandle handle) {}

void RendererVulkan::createTexture(TextureHandle handle, TextureCreate create) {}

void RendererVulkan::resizeTexture(TextureHandle handle, uint32_t width, uint32_t height) {}

void RendererVulkan::deleteTexture(TextureHandle handle) {}

void RendererVulkan::createIndexBuffer(
    IndexBufferHandle handle,
    Foundation::Memory indices,
    BufferElementType elementType,
    size_t count
) {
    m_indexBuffers[handle.id].create(indices.data, elementType, count, false);
    indices.release();
}

void RendererVulkan::createDynamicIndexBuffer(
    IndexBufferHandle handle,
    Foundation::Memory indices,
    BufferElementType elementType,
    size_t count
) {}

void RendererVulkan::updateDynamicIndexBuffer(
    IndexBufferHandle handle, Foundation::Memory indices, size_t count
) {}

void RendererVulkan::deleteIndexBuffer(IndexBufferHandle handle) {}

void RendererVulkan::createVertexBuffer(
    VertexBufferHandle handle,
    Foundation::Memory data,
    uint32_t size,
    VertexLayoutHandle layoutHandle
) {}

void RendererVulkan::createDynamicVertexBuffer(
    VertexBufferHandle handle,
    Foundation::Memory data,
    uint32_t size,
    VertexLayoutHandle layoutHandle
) {}

void RendererVulkan::updateDynamicVertexBuffer(
    VertexBufferHandle handle, Foundation::Memory data, uint32_t size
) {}

void RendererVulkan::deleteVertexBuffer(VertexBufferHandle handle) {}

void RendererVulkan::createVertexLayout(VertexLayoutHandle handle, VertexBufferLayoutData layout) {}

void RendererVulkan::deleteVertexLayout(VertexLayoutHandle handle) {}

void RendererVulkan::readTexture(Miren::TextureHandle handle, void *data) {}

void RendererVulkan::setUniform(const Uniform &uniform) {
    switch (uniform.type) {
        case UniformType::Sampler:
            return;
        case UniformType::Mat3:
            return;
        case UniformType::Mat4:
            return;
        case UniformType::Vec4:
            return;
        case UniformType::Count:
            return;
    }
    LOG_ERROR("UNIFORM TYPE IS UNDEFINED");
}

void RendererVulkan::setTexture(TextureHandle handle, uint32_t slot) {}

void RendererVulkan::submit(Frame *frame) {}

void RendererVulkan::viewChanged(View &view) {}

void RendererVulkan::submit(RenderDraw *draw) {
    // TODO: Capture time
}

} // namespace Miren
