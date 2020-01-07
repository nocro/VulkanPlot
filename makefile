VULKAN_SDK_PATH = /home/ubuntu/Documents/bibliotheque/1.1.106.0/x86_64

CFLAGS = -std=c++17 -Wfatal-errors -I$(VULKAN_SDK_PATH)/include -Wall
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan


Test: main context device window semaphores buffer uniformBuffer vulkanRender line settings 
	g++ -o exec  main.o context.o semaphores.o buffer.o uniformBuffer.o device.o window.o line.o settings.o vulkanRender.o  $(LDFLAGS) -Wall


main: main.cpp 
	g++ -c $(CFLAGS) -pedantic -o main.o  main.cpp

.PHONY:	test clean

test: Test
	./exec

window: Window.cpp
	g++ -c $(CFLAGS) -o window.o Window.cpp 

buffer: Buffer.cpp
	g++ -c $(CFLAGS) -o buffer.o Buffer.cpp

uniformBuffer: UniformBuffer.cpp
	g++ -c $(CFLAGS) -o uniformBuffer.o UniformBuffer.cpp

system: System.cpp
	g++ -c $(CFLAGS) -o system.o System.cpp

device: Device.cpp
	g++ -c $(CFLAGS) -o device.o Device.cpp

context: Context.cpp
	g++ -c $(CFLAGS) -o context.o Context.cpp

semaphores: Semaphores.cpp
	g++ -c $(CFLAGS) -o semaphores.o Semaphores.cpp

vulkanRender: VulkanRender.cpp 
	g++ -c $(CFLAGS) -o vulkanRender.o VulkanRender.cpp

line: line.cpp
	g++ -c $(CFLAGS) -o line.o line.cpp

settings: settings.cpp
	g++ -c $(CFLAGS) -o settings.o settings.cpp

clean:
	rm *.o

shader:
	./compile.sh
