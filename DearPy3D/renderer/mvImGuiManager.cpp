#include "mvImGuiManager.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include "mvGraphics.h"

namespace DearPy3D {

	mvImGuiManager::mvImGuiManager(GLFWwindow* window, mvGraphics& graphics)
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
        init_info.Instance = graphics.getInstance();
        init_info.PhysicalDevice = graphics.getPhysicalDevice();
        init_info.Device = graphics.getDevice();
        init_info.QueueFamily = graphics.getGraphicsQueueFamily();
        init_info.Queue = graphics.getGraphicsQueue();
        init_info.PipelineCache = nullptr;
        init_info.DescriptorPool = graphics.getDescriptorPool();
        init_info.Allocator = nullptr;
        init_info.MinImageCount = graphics.getMinImageCount();
        init_info.ImageCount = graphics.getMinImageCount();
        init_info.CheckVkResultFn = nullptr;
        ImGui_ImplVulkan_Init(&init_info, graphics.getRenderPass());

        // Upload Fonts
        {
            // Use any command queue
            VkCommandBuffer command_buffer = graphics.beginSingleTimeCommands();
            ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
            graphics.endSingleTimeCommands(command_buffer);
            ImGui_ImplVulkan_DestroyFontUploadObjects();
        }

	}

	mvImGuiManager::~mvImGuiManager()
	{
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}

    void mvImGuiManager::resize(mvGraphics& graphics)
    {
        
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = graphics.getInstance();
        init_info.PhysicalDevice = graphics.getPhysicalDevice();
        init_info.Device = graphics.getDevice();
        init_info.QueueFamily = graphics.getGraphicsQueueFamily();
        init_info.Queue = graphics.getGraphicsQueue();
        init_info.PipelineCache = nullptr;
        init_info.DescriptorPool = graphics.getDescriptorPool();
        init_info.Allocator = nullptr;
        init_info.MinImageCount = graphics.getMinImageCount();
        init_info.ImageCount = graphics.getMinImageCount();
        init_info.CheckVkResultFn = nullptr;
        ImGui_ImplVulkan_Init(&init_info, graphics.getRenderPass());
        ImGui_ImplVulkan_SetMinImageCount(graphics.getMinImageCount());
    }

	void mvImGuiManager::beginFrame(mvGraphics& graphics) const
	{
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
	}

	void mvImGuiManager::endFrame(mvGraphics& graphics) const
	{
        ImGui::Render();

        ImDrawData* main_draw_data = ImGui::GetDrawData();

        // Record dear imgui primitives into command buffer
        ImGui_ImplVulkan_RenderDrawData(main_draw_data, graphics.getCurrentCommandBuffer());
	}

}