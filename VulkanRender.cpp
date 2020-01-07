#include "VulkanRender.hpp"


VulkanRender::VulkanRender()
{
    try{
        m_window = std::make_unique<Window>(m_context.m_instance) ;
    }catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}


void VulkanRender::drawLine(const Line &l)
{
    
    std::vector<Vertex> vertices;
    for( unsigned int i = 0; i< l.m_x.size(); i++ )
    {
        vertices.push_back({{l.m_x[i], l.m_y[i]}, {1.0f,1.0f,1.0f}});
    }

    std::vector<uint16_t> indices;
    for( unsigned int i = 0; i< l.m_x.size()-1; i++ )
    {
        indices.push_back(i);
        indices.push_back(i+1);
    }

    try{
        m_window->createUniformBuffers(sizeof(UniformBufferObject));
        
        m_window->addData(vertices, indices);

        UniformBufferObject ubo = {};
        ubo.color = glm::vec3(1.0,0.0,0.0);

        // m_window->updateUniformBuffer(ubo);

        m_window->draw(indices.size(), ubo);
        while (!glfwWindowShouldClose(m_window->m_window)) {
            glfwPollEvents();
            // m_window->draw(indices.size());

            
        }
    }catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    
}


VulkanRender::~VulkanRender()
{

}

// void VulkanRender::draw()
// {

// }

// void VulkanRender::draw(GlobalSetting s)
// {

// }


// void VulkanRender::drawHist(const Hist &h)
// {

// }


// void VulkanRender::addLine(const Line &l)
// {

// }

// void VulkanRender::addHist(const Hist&h)
// {

// }

// void VulkanRender::clear()
// {

// }

// void VulkanRender::addHistFollowX(const Hist &h)
// {

// }
// void VulkanRender::addHistOther(const Hist &h)
// {

// }

// void VulkanRender::makeGlobalSettings(GlobalSetting s)
// {

// }