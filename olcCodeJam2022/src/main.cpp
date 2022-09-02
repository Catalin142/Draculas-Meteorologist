#include <iostream>

#include "Engine/Core/Application.h"
#include "intro_scene.h"
#include "game_scene.h"

class olcCodeJam : public Application
{
public:
	olcCodeJam(const char* name, unsigned int width, unsigned int height) : Application(name, width, height) 
	{ 
		FontManager::addFont("Basic", "res/fonts/basic_font.ttf", 120.0f);
		FontManager::addFont("Headtitle", "res/fonts/headtitle_font.ttf", 120.0f);
		IntroScene::readIntro();
		GameScene::loadArticles();

		pushLayer(std::make_shared<IntroScene>());
	}
};

int main()
{
	Application* app = new olcCodeJam("Dracula's meteorologist", 1366, 768);
	app->Run();

	delete app;
	return 0;
}