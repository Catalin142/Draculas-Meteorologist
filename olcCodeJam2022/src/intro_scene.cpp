#include "intro_scene.h"

#include "engine/Core/Application.h"
#include "engine/Utils/Input.h"
#include "engine/Utils/Random.h"

int IntroScene::m_NoOfPages = 0;
std::vector<std::string> IntroScene::m_IntroText;

void IntroScene::onAttach()
{
	m_Camera = std::make_shared<Camera>(screen_size::width, screen_size::height, glm::vec3(screen_size::width / 2.0f, screen_size::height / 2.0f, 5.0f));

	m_PressEnter = std::make_shared<Texture>("res/tex/press_enter.png");
	m_TextBox.tex = std::make_shared<Texture>("res/tex/text_box.png", GL_NEAREST);

	m_MainScreen = std::make_shared<Texture>("res/tex/bg.png");
	m_MainPage = std::make_shared<Texture>("res/tex/intro.png", GL_NEAREST);

	m_TextBox.height = 170;
	m_TextBox.width = 615;
	m_TextBox.pos = { 940, 270 };
	m_TextBox.offset_x = 10;
}

void IntroScene::onUpdate(float deltaTime)
{
	Renderer::clear(0.0f, 0.0f, 0.0f);

	Renderer::beginScene(m_Camera);
	
	if (m_LayerState == state::MENU)
	{
		Renderer::drawQuad(m_MainScreen, { 1.0f, 1.0f, 1.0f, 1.0f }, { screen_size::width / 2.0f, screen_size::height / 2.0f, 0.0f }, 
			{ screen_size::width, screen_size::height, 1.0f });

		if (isPressed(GLFW_KEY_ENTER))
		{
			m_LayerState = state::STORY;
			set_text(m_TextBox, m_IntroText[m_CurrentPage], FontManager::getFont("Basic"), { 28.0f, 28.0f });
			m_Transition = true;
		}

		Renderer::drawQuad(m_PressEnter, { 1.0f, 1.0f, 1.0f, 1.0f }, { screen_size::width - 300.0f, screen_size::height / 2.0f + 170.0f, 1.0f }, 
			{ 250.0f, 60.0f, 1.0f });
	}
	else
	{
		Renderer::drawQuad(m_MainPage, { 1.0f, 1.0f, 1.0f, 1.0f }, { screen_size::width / 2.0f, screen_size::height / 2.0f, 0.0f },
			{ screen_size::width, screen_size::height, 1.0f });

		/*std::string date = std::ctime(&current_date);

		for (int i = 0; i < 2; i++)
		{
			auto pos = date.find_last_of(' ');
			date = date.substr(0, pos);
		}
		date += " 1985";

		float width = FontManager::getFont("Basic")->getTextWidth(date, {25.0f, 25.0f});
		Renderer::drawString(date, FontManager::getFont("Basic"), { screen_size::width / 2.0f - width / 2.0f, screen_size::height - 140.0f, 1.0f},
			{ 25.0f, 25.0f, 1.0f });

		Renderer::drawString("VOL " + std::to_string(vol), FontManager::getFont("Basic"), { 50.0f, screen_size::height - 140.0f, 1.0f },
			{ 25.0f, 25.0f, 1.0f });

		float no_width = FontManager::getFont("Basic")->getTextWidth("NO. " + std::to_string(no), { 25.0f, 25.0f });
		Renderer::drawString("NO. " + std::to_string(no), FontManager::getFont("Basic"), { screen_size::width - 50.0f - no_width, screen_size::height - 140.0f, 1.0f },
			{ 25.0f, 25.0f, 1.0f });*/

		m_CurrentTime += deltaTime;
		if (m_CurrentTime > m_MaxTime)
		{
			bool reset = false;

			if (isPressed(GLFW_KEY_RIGHT))
				m_CurrentPage++, reset = true;
			else if (isPressed(GLFW_KEY_LEFT))
				m_CurrentPage--, reset = true;

			if (reset)
			{
				if (m_CurrentPage < 0)
					m_CurrentPage = 0;
				else if (m_CurrentPage > m_NoOfPages)
					m_CurrentPage = m_NoOfPages;

				m_CurrentTime = 0.0f;
				set_text(m_TextBox, m_IntroText[m_CurrentPage], FontManager::getFont("Basic"), { 28.0f, 28.0f });
			}
		}

		draw_text_box(m_TextBox, FontManager::getFont("Basic"), { 28.0f, 28.0f });

		/*float text_width = FontManager::getFont("Basic")->getTextWidth(m_IntroText[m_CurrentPage], { 28.0f, 28.0f });
		Renderer::drawQuad({ 1.0f, 1.0f, 1.0f, 1.0f }, { screen_size::width / 2.0f, screen_size::height - 50.0f, 1.0f },
			{ screen_size::width - 50.0f, 50.0f, 1.0f });

		Renderer::drawString(m_IntroText[m_CurrentPage], FontManager::getFont("Basic"), 
			{ screen_size::width / 2.0f - text_width / 2.0f , screen_size::height - 60.0f, 1.0f },
			{ 28.0f, 28.0f, 1.0f });*/

		Renderer::drawQuad(m_PressEnter, { 1.0f, 1.0f, 1.0f, 1.0f }, { screen_size::width - 160.0f, screen_size::height - 50.0f, 1.0f },
			{ 250.0f, 60.0f, 1.0f });

		if (isReleased(GLFW_KEY_ENTER) && m_Transition == true)
			m_Transition = false;

		if (isPressed(GLFW_KEY_ENTER) && m_Transition == false)
		{
			Application::Get()->pushLayer(std::make_shared<GameScene>());
			return;
		}
		
	}
	Renderer::endScene();
}

#include <fstream>
void IntroScene::readIntro()
{
	m_IntroText.clear();
	m_IntroText.resize(10);
	std::ifstream file("res/intro.txt");

	if (file.good())
	{
		std::string word;
		while (file >> word)
		{
			if (word == "br")
				m_NoOfPages++;
			else
			{
				m_IntroText[m_NoOfPages] += word + " ";
			}
		}

		file.close();
	}
}
