#include "mvImGuiManager.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include "mvGraphics.h"

namespace DearPy3D {

	mvImGuiManager::mvImGuiManager(GLFWwindow* window)
	{

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = mvGraphics::GetContext().getInstance();
        init_info.PhysicalDevice = mvGraphics::GetContext().getPhysicalDevice();
        init_info.Device = mvGraphics::GetContext().getLogicalDevice();
        init_info.QueueFamily = mvGraphics::GetContext().getLogicalDevice().getGraphicsQueueFamily();
        init_info.Queue = mvGraphics::GetContext().getLogicalDevice().getGraphicsQueue();
        init_info.PipelineCache = nullptr;
        init_info.DescriptorPool = mvGraphics::GetContext().getDescriptorPool();
        init_info.Allocator = nullptr;
        init_info.MinImageCount = mvGraphics::GetContext().getSwapChain().getMinImageCount();
        init_info.ImageCount = mvGraphics::GetContext().getSwapChain().getMinImageCount();
        init_info.CheckVkResultFn = nullptr;
        ImGui_ImplVulkan_Init(&init_info, mvGraphics::GetContext().getSwapChain().getRenderPass());

        // Upload Fonts
        {
            // Use any command queue
            VkCommandBuffer command_buffer = mvGraphics::GetContext().getLogicalDevice().beginSingleTimeCommands();
            ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
            mvGraphics::GetContext().getLogicalDevice().endSingleTimeCommands(command_buffer);
            ImGui_ImplVulkan_DestroyFontUploadObjects();
        }

	}

	mvImGuiManager::~mvImGuiManager()
	{
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}

    void mvImGuiManager::resize()
    {
        vkDeviceWaitIdle(mvGraphics::GetContext().getLogicalDevice());
        ImGui_ImplVulkan_SetMinImageCount(mvGraphics::GetContext().getSwapChain().getMinImageCount());
    }

	void mvImGuiManager::beginFrame() const
	{
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
	}

	void mvImGuiManager::endFrame() const
	{
        ImGui::Render();

        ImDrawData* main_draw_data = ImGui::GetDrawData();

        // Record dear imgui primitives into command buffer
        ImGui_ImplVulkan_RenderDrawData(main_draw_data, mvGraphics::GetContext().getSwapChain().getCurrentCommandBuffer());
	}

}