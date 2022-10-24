//
// Created by gaetz on 09/10/2022.
//

#ifndef RENDER_VULKAN_PIPELINE_H
#define RENDER_VULKAN_PIPELINE_H

#include "Shader.h"
#include "RenderData.h"
#include "Mesh.h"

namespace engine::render::vulkan::vkInit {

    struct GraphicsPipelineIn {
        vk::Device device;
        str vertexShaderPath;
        str fragmentShaderPath;
        vk::Extent2D swapchainExtent;
        vk::Format swapchainImageFormat;
        vk::DescriptorSetLayout descriptorSetLayout;
    };


    struct GraphicsPipelineOut {
        vk::PipelineLayout layout;
        vk::RenderPass renderPass;
        vk::Pipeline pipeline;
    };

    vk::PipelineLayout makePipelineLayout(vk::Device device, vk::DescriptorSetLayout descriptorSetLayout) {
        vk::PipelineLayoutCreateInfo layoutCreateInfo {};
        layoutCreateInfo.flags = vk::PipelineLayoutCreateFlags();

        layoutCreateInfo.setLayoutCount = 1;
        layoutCreateInfo.pSetLayouts = &descriptorSetLayout;

        layoutCreateInfo.pushConstantRangeCount = 1;
        vk::PushConstantRange pushConstantInfo;
        pushConstantInfo.offset = 0;
        pushConstantInfo.size = sizeof(vkUtils::ObjectData);
        pushConstantInfo.stageFlags = vk::ShaderStageFlagBits::eVertex;
        layoutCreateInfo.pPushConstantRanges = &pushConstantInfo;

        auto layoutRes = device.createPipelineLayout(layoutCreateInfo);
        GASSERT_MSG(layoutRes.result == vk::Result::eSuccess, "Vulkan could not create pipeline layout");
        return layoutRes.value;
    }

    vk::RenderPass makeRenderPass(vk::Device device, vk::Format swapchainImageFormat) {
        vk::AttachmentDescription colorAttachment {};
        colorAttachment.flags = vk::AttachmentDescriptionFlags();
        colorAttachment.format = swapchainImageFormat;
        colorAttachment.samples = vk::SampleCountFlagBits::e1;
        colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
        colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
        colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
        colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

        vk::AttachmentReference colorAttachmentRef {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

        vk::SubpassDescription subpass {};
        subpass.flags = vk::SubpassDescriptionFlags();
        subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;


        vk::SubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL; //implicit subpass before render begins/after it ends
        dependency.dstSubpass = 0; //the rendering subpass
        dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependency.srcAccessMask = vk::AccessFlagBits::eNone; //once the color attachment has finished its output, the image isn't needed and can be drawn over
        dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite; // color output waits on this, it won't occur until the write bit is set

        vk::RenderPassCreateInfo renderPassInfo {};
        renderPassInfo.flags = vk::RenderPassCreateFlags();
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;


        auto renderPassRes = device.createRenderPass(renderPassInfo);
        GASSERT_MSG(renderPassRes.result == vk::Result::eSuccess, "Vulkan could not create render pass");
        return renderPassRes.value;
    }

    GraphicsPipelineOut makeGraphicsPipeline(const GraphicsPipelineIn& specifications) {
        vk::GraphicsPipelineCreateInfo pipelineCreateInfo {};
        pipelineCreateInfo.flags = vk::PipelineCreateFlags();
        vector<vk::PipelineShaderStageCreateInfo> shaderStages;

        // Vertex input : data sent in
        vk::VertexInputBindingDescription bindingDescription = vkMesh::getPosColorBindingDescription();
        array<vk::VertexInputAttributeDescription, 3> attributeDescriptions = vkMesh::getPosColorUVAttributeDescriptions();
        vk::PipelineVertexInputStateCreateInfo vertexInputInfo {};
        vertexInputInfo.flags = vk::PipelineVertexInputStateCreateFlags();
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
        pipelineCreateInfo.pVertexInputState = &vertexInputInfo;

        // Input assembly : how to organize input data
        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo {};
        inputAssemblyCreateInfo.flags = vk::PipelineInputAssemblyStateCreateFlags();
        inputAssemblyCreateInfo.topology = vk::PrimitiveTopology::eTriangleList;
        pipelineCreateInfo.pInputAssemblyState = &inputAssemblyCreateInfo;

        // Vertex shader
        vk::ShaderModule vertexShader = vkUtils::createModule(specifications.vertexShaderPath, specifications.device);
        vk::PipelineShaderStageCreateInfo vertexShaderInfo {};
        vertexShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
        vertexShaderInfo.stage = vk::ShaderStageFlagBits::eVertex;
        vertexShaderInfo.module = vertexShader;
        vertexShaderInfo.pName = "main";
        shaderStages.push_back(vertexShaderInfo);

        // Viewport and scissors : screen extent and part of it we want to draw
        vk::Viewport viewport;
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = specifications.swapchainExtent.width;
        viewport.height = specifications.swapchainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vk::Rect2D scissors {};
        scissors.offset.x = 0.0f;
        scissors.offset.y = 0.0f;
        scissors.extent = specifications.swapchainExtent;
        vk::PipelineViewportStateCreateInfo viewportInfo;
        viewportInfo.flags = vk::PipelineViewportStateCreateFlags();
        viewportInfo.viewportCount = 1;
        viewportInfo.pViewports = &viewport;
        viewportInfo.scissorCount = 1;
        viewportInfo.pScissors = &scissors;
        pipelineCreateInfo.pViewportState = &viewportInfo;

        // Rasterizer : convert vertex positions to pixels
        vk::PipelineRasterizationStateCreateInfo rasterizationInfo {};
        rasterizationInfo.flags = vk::PipelineRasterizationStateCreateFlags();
        rasterizationInfo.depthClampEnable = VK_FALSE;
        rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        rasterizationInfo.polygonMode = vk::PolygonMode::eFill;
        rasterizationInfo.lineWidth = 1.0f;
        rasterizationInfo.cullMode = vk::CullModeFlagBits::eNone;
        rasterizationInfo.frontFace = vk::FrontFace::eCounterClockwise;
        rasterizationInfo.depthBiasEnable = VK_FALSE;
        pipelineCreateInfo.pRasterizationState = &rasterizationInfo;

        // Fragment shader
        vk::ShaderModule fragmentShader = vkUtils::createModule(specifications.fragmentShaderPath, specifications.device);
        vk::PipelineShaderStageCreateInfo fragmentShaderInfo {};
        fragmentShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
        fragmentShaderInfo.stage = vk::ShaderStageFlagBits::eFragment;
        fragmentShaderInfo.module = fragmentShader;
        fragmentShaderInfo.pName = "main";
        shaderStages.push_back(fragmentShaderInfo);

        // No more shaders, pass shader stages to the pipeline
        pipelineCreateInfo.stageCount = shaderStages.size();
        pipelineCreateInfo.pStages = shaderStages.data();

        // Multisampling
        vk::PipelineMultisampleStateCreateInfo multisampleInfo {};
        multisampleInfo.flags = vk::PipelineMultisampleStateCreateFlags();
        multisampleInfo.sampleShadingEnable = VK_FALSE;
        multisampleInfo.rasterizationSamples = vk::SampleCountFlagBits::e1;
        pipelineCreateInfo.pMultisampleState = &multisampleInfo;

        // Color blend
        vk::PipelineColorBlendAttachmentState colorBlendAttachment {};
        colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
        colorBlendAttachment.blendEnable = VK_FALSE;
        vk::PipelineColorBlendStateCreateInfo colorBlending {};
        colorBlending.flags = vk::PipelineColorBlendStateCreateFlags();
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = vk::LogicOp::eCopy;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;
        pipelineCreateInfo.pColorBlendState = &colorBlending;

        LOG(LogLevel::Trace) << "Create pipeline layout";
        vk::PipelineLayout pipelineLayout = makePipelineLayout(specifications.device, specifications.descriptorSetLayout);
        pipelineCreateInfo.layout = pipelineLayout;

        LOG(LogLevel::Trace) << "Create render pass";
        vk::RenderPass renderPass = makeRenderPass(specifications.device, specifications.swapchainImageFormat);
        pipelineCreateInfo.renderPass = renderPass;

        // When we want ot inherit from a pipeline
        pipelineCreateInfo.basePipelineHandle = nullptr;

        LOG(LogLevel::Trace) << "Create graphics pipeline";
        vk::Pipeline graphicsPipeline;
        graphicsPipeline = (specifications.device.createGraphicsPipeline(nullptr, pipelineCreateInfo)).value;

        GraphicsPipelineOut output {};
        output.layout = pipelineLayout;
        output.renderPass = renderPass;
        output.pipeline = graphicsPipeline;

        // Cleanup
        specifications.device.destroyShaderModule(vertexShader);
        specifications.device.destroyShaderModule(fragmentShader);

        return output;
    }
}


#endif //RENDER_VULKAN_PIPELINE_H
