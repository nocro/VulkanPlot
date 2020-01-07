#ifndef WINDOW_HPP
#define WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Device.hpp"
#include <memory>
#include "Semaphores.hpp"
#include "UniformBuffer.hpp"
#include <fstream>
#include <chrono>







VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
std::vector<char> readFile(const std::string& filename);


// demander le nom des shaders a la construction de la window ? 
class Window
{
public:
    Window(VkInstance& instance, int width = 800, int height = 600);

    // demande les vertices suivit des indices (les indices sont sur 16 bits mais faura changer ca)
    void addData(std::vector<Vertex> vertices, std::vector<uint16_t> indices);
    void draw(std::size_t nbPoint);
    
    template<typename T>
    void draw(std::size_t nbPoint, T uniform_data)
    {
        std::cout << "drawing " << nbPoint << " points " << std::endl;
        if(!m_haveData)
        {
            std::cout<< " no data to print" << std::endl;
            // a la long ecrire ca dans la fenetre
            return;
        }
        else 
        {
            
            vkWaitForFences(m_device->m_device, 1, &m_semaphores->inFlightFences[m_currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
            uint32_t imageIndex;
            VkResult result = vkAcquireNextImageKHR(m_device->m_device, m_swapChain, std::numeric_limits<uint64_t>::max(), m_semaphores->imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);
            
            if (result == VK_ERROR_OUT_OF_DATE_KHR) {
                recreateSwapChain(nbPoint);
                return;
            } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
                throw std::runtime_error("failed to acquire swap chain image!");
            }
            
            updateUniformBuffer(imageIndex, uniform_data);

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

            VkSemaphore waitSemaphores[] = {m_semaphores->imageAvailableSemaphores[m_currentFrame]};
            VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = waitSemaphores;
            submitInfo.pWaitDstStageMask = waitStages;

            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];

            VkSemaphore signalSemaphores[] = {m_semaphores->renderFinishedSemaphores[m_currentFrame]};
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = signalSemaphores;

            vkResetFences(m_device->m_device, 1, &m_semaphores->inFlightFences[m_currentFrame]);

            if (vkQueueSubmit(m_device->m_graphicsQueue, 1, &submitInfo, m_semaphores->inFlightFences[m_currentFrame]) != VK_SUCCESS) 
            {
                throw std::runtime_error("failed to submit draw command buffer!");
            }

            

            VkPresentInfoKHR presentInfo = {};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = signalSemaphores;

            VkSwapchainKHR swapChains[] = {m_swapChain};
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = swapChains;

            presentInfo.pImageIndices = &imageIndex;

            result = vkQueuePresentKHR(m_device->m_presentQueue, &presentInfo);
            

            if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_framebufferResized) {
                m_framebufferResized = false;
                recreateSwapChain(nbPoint); 
            } else if (result != VK_SUCCESS) {
                throw std::runtime_error("failed to present swap chain image!");
            }

            m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;


            vkDeviceWaitIdle(m_device->m_device);

        }
    }
    void createUniformBuffers(VkDeviceSize bufferSize);

    template<typename T>
    void updateUniformBuffer(uint32_t imageIndex, T data)
    {
        std::cout << "update uniform buffer"<< std::endl;
        if( m_have_uniform == false )// changer pour utiliser des trucs de vulkans (ak init avec un null che pas quoi )
        {
            std::cout << "no uniform buffer find, useless to update " <<  std::endl;
            return;
        }
        void* newData;
        vkMapMemory(m_device->m_device, m_uniformBuffers[imageIndex].m_bufferMemory, 0, sizeof(data), 0, &newData);
            memcpy(newData, &data, sizeof(data));
        vkUnmapMemory(m_device->m_device, m_uniformBuffers[imageIndex].m_bufferMemory);

    }

    ~Window();

private:

    /*
        pour moi 
    */ 
    bool  m_haveData = false;
    bool m_have_uniform= false;
    bool m_haveSwapChain = false;

    VkShaderModule createShaderModule(const std::vector<char>& code);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    void createSurface();
    void createDescriptorSetLayout();
    void createCommandPool();


    void createSwapChain();
    void createImageViews();
    void createRenderPass();
    void createGraphicsPipeline();
    void createFramebuffers();
    // on met la tailles des donnée a evoyer; dans l'exemple c'est une structure connu a l'avance
    // void createUniformBuffers(VkDeviceSize bufferSize);
    void createDescriptorPool();
    // on a encore besoin de la taille de l'uniforme fuffer 
    void createDescriptorSets(VkDeviceSize bufferSize);
    // nbPoint = nombres d'indice ak indices.size()
    void createCommandBuffers(std::size_t nbPoint);

    
    void cleanupSwapChain();
    void recreateSwapChain(std::size_t nbPoint);
    void updateUniformBuffer(uint32_t currentImage);
    /*void createUniformBuffers();
    void createDescriptorSets();*/

    size_t m_currentFrame = 0;
    
    

public:
    int m_width; 
    int m_height;
    bool m_framebufferResized = false;
    VkInstance &r_instance;

    GLFWwindow* m_window;
    VkSurfaceKHR m_surface;

    // c'est un pointeur 
    std::unique_ptr<Device> m_device;

    std::unique_ptr<Semaphores> m_semaphores;
    VkDescriptorSetLayout m_descriptorSetLayout;
    VkCommandPool m_commandPool;

    
    // on les prends au moment du draw 
    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<IndexBuffer> indexBuffer; 

    std::vector<UniformBuffer> m_uniformBuffers; 

    // pour afficher 
    VkSwapchainKHR m_swapChain;
    std::vector<VkImage> m_swapChainImages;
    VkFormat m_swapChainImageFormat;

    // extent ca veux dire étendue 
    VkExtent2D m_swapChainExtent;
    std::vector<VkImageView> m_swapChainImageViews;
    std::vector<VkFramebuffer> m_swapChainFramebuffers;

    VkRenderPass m_renderPass;
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_graphicsPipeline;

    VkDescriptorPool m_descriptorPool; 
    std::vector<VkDescriptorSet> m_descriptorSets;

    std::vector<VkCommandBuffer> m_commandBuffers;

    

    

private: 
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        app->m_framebufferResized = true;
    }
    
};







#endif