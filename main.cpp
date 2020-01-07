#include "Context.hpp"
#include "VulkanRender.hpp"

const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
    0, 1, 1, 2, 2, 3, 3, 0
};

int main()
{
    std::vector<double> x1;
    std::vector<double> y1;
    for(double i=0.0; i< 2.0; i+=0.01)
    {
        x1.push_back(i);
        y1.push_back(i*i); 

    }
    VulkanRender render;
    render.drawLine(Line(x1,y1));
    // Context context;
    // try{
    //     Window w = context.createWindow();
    //     w.addData(vertices,indices);
    //     while (!glfwWindowShouldClose(w.m_window)) {
    //         glfwPollEvents();
    //         w.draw(indices.size());
    //     }
    // } catch (const std::exception& e) {
    //     std::cerr << e.what() << std::endl;
    //     return EXIT_FAILURE;
    // }


    return EXIT_SUCCESS;
    
}