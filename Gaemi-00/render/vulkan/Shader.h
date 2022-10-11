//
// Created by gaetz on 08/10/2022.
//

#ifndef RENDER_VULKAN_SHADER_H
#define RENDER_VULKAN_SHADER_H

#include "../../Defines.h"
#include "../../Log.h"
#include <vector>
#include <fstream>
#include <vulkan/vulkan.hpp>

using std::vector;

namespace engine::render::vulkan::vkUtils {

    vector<char> readFile(str filename) {
        std::ifstream file {filename, std::ios::ate | std::ios::binary };
        if (!file.is_open()) {
            LOG(LogLevel::Error) << "Shader file " << filename << " could not be opened";
        }
        size_t filesize = static_cast<size_t>(file.tellg());
        vector<char> buffer(filesize);
        file.seekg(0);
        file.read(buffer.data(), filesize);
        file.close();
        return buffer;
    }

    vk::ShaderModule createModule(str filename, vk::Device device) {
        vector<char> sourceCode = readFile(filename);
        vk::ShaderModuleCreateInfo moduleCreateInfo {};
        moduleCreateInfo.flags = vk::ShaderModuleCreateFlags();
        moduleCreateInfo.codeSize = sourceCode.size();
        moduleCreateInfo.pCode = reinterpret_cast<const u32*>(sourceCode.data());
        auto shaderModuleRes = device.createShaderModule(moduleCreateInfo);
        GASSERT_MSG(shaderModuleRes.result == vk::Result::eSuccess, "Vulkan could not create shader module for shader " + filename)
        return shaderModuleRes.value;
    }

}

#endif //RENDER_VULKAN_SHADER_H
