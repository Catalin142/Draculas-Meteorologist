#include <memory>
#include <iostream>

#include "Application.h"
#include "engine/Graphics/Renderer.h"
#include "engine/Utils/Random.h"

float Time::deltaTime = 0;
float screen_size::width = 0;
float screen_size::height = 0;

Application* Application::m_Instance;

Application::Application(const char* name, unsigned int width, unsigned int height)
{
	if (!glfwInit())
		std::cout << "glfw" << std::endl;
	m_Window = std::make_shared<Window>(name, width, height);
	screen_size::width = width;
	screen_size::height = height;

	Renderer::Init();
	Random::Init();

	m_Instance = this;
}

Application::~Application()
{
	
}

void Application::pushLayer(const std::shared_ptr<Layer>& layer)
{
	if (m_Layer)
		m_Layer.reset();
	m_Layer = layer;
	layer->onAttach();
}

void Application::Run()
{
	float currentTime = (float)glfwGetTime();
	float lastTime = currentTime;
	while (m_Window->isRunning() && m_Running)
	{
		currentTime = (float)glfwGetTime();
		Time::deltaTime = currentTime - lastTime;

		if (m_Layer->m_isActive)
			m_Layer->onUpdate(Time::deltaTime);

		m_Window->Update();

		lastTime = currentTime;
	}
}

void Application::Close()
{
	m_Running = false;
}
