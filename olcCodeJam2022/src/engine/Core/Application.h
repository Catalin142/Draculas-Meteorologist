#pragma once
#include <vector>
#include <memory>
#include <chrono>

#include "Window.h"
#include "Layer.h"

struct screen_size
{
	static float width;
	static float height;
};

struct Time
{
	static float deltaTime;
};

static std::time_t current_date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

class Application
{
public:
	Application(const char* name, unsigned int width, unsigned int height);
	virtual ~Application();

	std::shared_ptr<Window>& getWindow() { return m_Window; }
	void pushLayer(const std::shared_ptr<Layer>& layer);

	void Run();

	static Application* Get() { return m_Instance; }
	void Close();

private:
	std::shared_ptr<Window> m_Window;
	std::shared_ptr<Layer> m_Layer;

	static Application* m_Instance;

	bool m_Running = true;

};