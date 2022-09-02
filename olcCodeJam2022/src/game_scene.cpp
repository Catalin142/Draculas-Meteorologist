#include "game_scene.h"
#include "engine/Core/Application.h"
#include "engine/Graphics/Renderer.h"
#include "engine/Utils/FontManager.h"
#include "engine/Utils/Random.h"
#include "engine/Utils/Input.h"

std::vector<article> GameScene::m_Articles;

void GameScene::onAttach()
{
	m_Camera = std::make_shared<Camera>(screen_size::width, screen_size::height, glm::vec3(screen_size::width / 2.0f, screen_size::height / 2.0f, 5.0f));
	m_MainPage = std::make_shared<Texture>("res/tex/main_page.png");
	m_NoPhoto = std::make_shared<Texture>("res/tex/no_photo.png", GL_NEAREST);

	m_AvailableWeather[0] = false;
	m_AvailableWeather[1] = false;
	m_AvailableWeather[2] = false;
	m_AvailableWeather[3] = false;

	m_TextBox.tex = std::make_shared<Texture>("res/tex/news_box.png", GL_NEAREST);
	m_TextBox.height = 170;
	m_TextBox.width = 640;
	m_TextBox.pos = { screen_size::width / 2.0f, screen_size::height / 2.0f - 100.0f };
	m_TextBox.offset_x = 10;

	weather_pick stormy;
	weather_pick rainy;
	weather_pick snowy;
	weather_pick sunny;

	stormy.tex = std::make_shared<Texture>("res/tex/stormy_day.png", GL_NEAREST);
	rainy.tex = std::make_shared<Texture>("res/tex/rainy_day.png", GL_NEAREST);
	snowy.tex = std::make_shared<Texture>("res/tex/snowy_day.png", GL_NEAREST);
	sunny.tex = std::make_shared<Texture>("res/tex/sunny_day.png", GL_NEAREST);

	int start_x = screen_size::width / 2.0f - 315.0f;
	stormy.pos = { start_x + 210.0f * 0, screen_size::height / 2.0f + 25.0f};
	rainy.pos = { start_x + 210.0f * 1, screen_size::height / 2.0f + 25.0f };
	snowy.pos = { start_x + 210.0f * 2, screen_size::height / 2.0f + 25.0f };
	sunny.pos = { start_x + 210.0f * 3, screen_size::height / 2.0f + 25.0f };

	stormy.type = weather::storm;
	rainy.type = weather::rain;
	snowy.type = weather::snow;
	sunny.type = weather::good;

	m_Weather.push_back(stormy);
	m_Weather.push_back(rainy);
	m_Weather.push_back(snowy);
	m_Weather.push_back(sunny);

	loadFeedback();

	Reset();
}

void GameScene::onUpdate(float deltaTime)
{
	Renderer::clear(0.1f, 0.1f, 0.1f);

	Renderer::beginScene(m_Camera);

	switch (m_State)
	{
	case layer_state::search:
		search(deltaTime);
		break;

	case layer_state::predict:
		predict(deltaTime);
		break;

	case layer_state::feedback:
		feedback(deltaTime);
		break;

	case layer_state::finish:
		finish();
		break;
	}
	

	Renderer::endScene();
}
#include <iostream>
void GameScene::Reset()
{
	m_OpenArticle = false;
	m_CorrectPredicion = false;

	m_ActiveArticles.clear();

	m_Vol = Random::Float() * 10000;
	m_NO = Random::Float() * 500;

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist3(0, 3);

	int w = dist3(rng);
	while (m_AvailableWeather[w] == true)
	{
		w = dist3(rng);
	}

	switch (w)
	{
	case 0: m_CurrentWeather = weather::rain; break;
	case 1: m_CurrentWeather = weather::good; break;
	case 2: m_CurrentWeather = weather::snow; break;
	case 3: m_CurrentWeather = weather::storm; break;
	}

	for (auto& art : m_Articles)
	{
		if (art.weather & (int)m_CurrentWeather)
			m_ActiveArticles.push_back(&art);

		if (art.weather & (int)weather::none)
		{
			float random = Random::Float();
			if (random > 0.3f)
				m_ActiveArticles.push_back(&art);
		}
	}

	std::uniform_int_distribution<std::mt19937::result_type> dists(0, m_ActiveArticles.size() - 1);
	for (int i = 0; i < m_ActiveArticles.size() / 2; i++)
	{
		int rnd = dists(rng);
		std::swap(m_ActiveArticles[i], m_ActiveArticles[rnd]);
	}

	for (auto& c : m_CorrectFeedback)
	{
		std::string t;
		for (const auto& text : c.texts)
			t += text + " ";
		c.pos = { 150.0f + Random::Float() * 900.0f, 120.0f + Random::Float() * 500.0f };
		set_text(c, t, FontManager::getFont("Basic"), { 25.0f, 25.0f });
	}
	for (auto& w : m_WrongFeedback)
	{
		std::string t;
		for (const auto& text : w.texts)
			t += text + " ";
		w.pos = { 150.0f + Random::Float() * 900.0f, 120.0f + Random::Float() * 500.0f };
		set_text(w, t, FontManager::getFont("Basic"), { 25.0f, 25.0f });
	}
}

void GameScene::drawArticles()
{
	int maxi = std::min((int)m_ActiveArticles.size(), m_MaxArticlesPerPage * (m_CurrentPage + 1));

	int start_x = 605.0f;
	int start_y = 510.0f;
	int curr = 0;
	for (int i = m_MaxArticlesPerPage * m_CurrentPage; i < maxi; i++)
	{
		Renderer::drawQuad({ 0.0f, 0.0f, 0.0f, 1.0f }, { start_x + 170.0f * curr, start_y, 0.0f },
			{ m_ArticleWidth + 10.0f, m_ArticleHeight + 10.0f, 1.0f });

		if (m_ActiveArticles[i]->use_texture)
			Renderer::drawQuad(m_ActiveArticles[i]->tex, {1.0f, 1.0f, 1.0f, 1.0f}, { start_x + 170.0f * curr, start_y, 0.0f},
				{ m_ArticleWidth, m_ArticleHeight, 1.0f });
		else
		{
			Renderer::drawQuad(m_NoPhoto, { 1.0f, 1.0f, 1.0f, 1.0f }, { start_x + 170.0f * curr, start_y, 0.0f },
				{ m_ArticleWidth, m_ArticleHeight, 1.0f });
		}

		m_ActiveArticles[i]->position = { start_x + 170.0f * curr, start_y };

		if (curr == 1)
			start_y -= 170.0f;

		curr = (curr + 1) % 2;
	}
}

void GameScene::openArticles()
{
	if (isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		int maxi = std::min((int)m_ActiveArticles.size(), m_MaxArticlesPerPage * (m_CurrentPage + 1));
		auto mouse_pos = getMouseInverted();
		for (int i = m_MaxArticlesPerPage * m_CurrentPage; i < maxi; i++)
		{
			auto& art = m_ActiveArticles[i];
			if (mouse_pos.x > art->position.x - m_ArticleWidth / 2.0f && mouse_pos.x < art->position.x + m_ArticleWidth / 2.0f &&
				mouse_pos.y > art->position.y - m_ArticleHeight / 2.0f && mouse_pos.y < art->position.y + m_ArticleHeight / 2.0f)
			{
				m_OpenArticle = true; 
				m_OpenedArticle = i;
				set_text(m_TextBox, art->text, FontManager::getFont("Basic"), { 25.0f, 25.0f });
				Mouse(false);
			}
		}
	}
}

void GameScene::drawOpenedArticle()
{
	if (isPressed(GLFW_KEY_ESCAPE))
	{
		m_OpenArticle = false;
		Mouse(true);
	}

	Renderer::drawQuad({ 0.0f, 0.0f, 0.0f, 0.7f }, { screen_size::width / 2.0f, screen_size::height / 2.0f + 50.0f, 0.0f },
		{ 650.0f, 600.0f, 1.0f });

	float width = FontManager::getFont("Basic")->getTextWidth("(esc)", { 25.0f, 25.0f });
	Renderer::drawString("(esc)", FontManager::getFont("Basic"), { screen_size::width / 2.0f - width / 2.0f, screen_size::height / 2.0f + 320.0f, 1.0f },
		{ 25.0f, 25.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

	if (m_ActiveArticles[m_OpenedArticle]->use_texture)
		Renderer::drawQuad(m_ActiveArticles[m_OpenedArticle]->tex, { 1.0f, 1.0f, 1.0f, 1.0f }, { screen_size::width / 2.0f, screen_size::height / 2.0f + 150.0f, 0.0f },
			{ m_ArticleWidth + 150.0f, m_ArticleHeight + 150.0f, 1.0f });
	else
		Renderer::drawQuad(m_NoPhoto, { 1.0f, 1.0f, 1.0f, 1.0f }, { screen_size::width / 2.0f, screen_size::height / 2.0f + 150.0f, 0.0f },
			{ m_ArticleWidth + 150.0f, m_ArticleHeight + 150.0f, 1.0f });

	draw_text_box(m_TextBox, FontManager::getFont("Basic"), { 25.0f, 25.0f });
}

void GameScene::search(float deltaTime)
{
	Renderer::drawQuad(m_MainPage, { 1.0f, 1.0f, 1.0f, 1.0f }, { screen_size::width / 2.0f, screen_size::height / 2.0f, 0.0f },
		{ screen_size::width, screen_size::height, 1.0f });

	Renderer::drawString("Press E to predict", FontManager::getFont("Basic"), { screen_size::width - 330.0f, screen_size::height - 40.0f, 1.0f },
		{ 25.0f, 25.0f, 1.0f }, {1.0f, 1.0f, 1.0f, 1.0f});

	std::string date = std::ctime(&current_date);

	for (int i = 0; i < 2; i++)
	{
		auto pos = date.find_last_of(' ');
		date = date.substr(0, pos);
	}
	date += " 1985";

	float width = FontManager::getFont("Basic")->getTextWidth(date, { 15.0f, 15.0f });
	Renderer::drawString(date, FontManager::getFont("Basic"), { screen_size::width / 2.0f - width / 2.0f + 5.0f, screen_size::height - 140.0f, 1.0f },
		{ 15.0f, 15.0f, 1.0f });

	Renderer::drawString("VOL " + std::to_string(m_Vol), FontManager::getFont("Basic"), { 500.0f + 5.0f, screen_size::height - 140.0f, 1.0f },
		{ 15.0f, 15.0f, 1.0f });

	float no_width = FontManager::getFont("Basic")->getTextWidth("NO. " + std::to_string(m_NO), { 15.0f, 15.0f });
	Renderer::drawString("NO. " + std::to_string(m_NO), FontManager::getFont("Basic"), { screen_size::width - 500.0f - no_width + 5.0f, screen_size::height - 140.0f, 1.0f },
		{ 15.0f, 15.0f, 1.0f });

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
			else if (m_CurrentPage > std::ceil(m_ActiveArticles.size() / m_MaxArticlesPerPage))
				m_CurrentPage = std::ceil(m_ActiveArticles.size() / m_MaxArticlesPerPage);

			m_CurrentTime = 0.0f;
		}
	}

	drawArticles();
	openArticles();

	if (m_OpenArticle)
		drawOpenedArticle();

	if (isPressed(GLFW_KEY_E))
	{
		Mouse(true);
		m_State = layer_state::predict;
	}
}

static bool clicked(const glm::vec2& pos, const glm::vec2& size)
{
	if (isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		auto mouse_pos = getMouseInverted();
		if (mouse_pos.x > pos.x - size.x / 2.0f && mouse_pos.x < pos.x + size.x / 2.0f &&
			mouse_pos.y > pos.y - size.y / 2.0f && mouse_pos.y < pos.y + size.y / 2.0f)
			return true;
	}
	return false;
}

void GameScene::predict(float deltaTime)
{
	float width = FontManager::getFont("Headtitle")->getTextWidth("Tommorow the weather will be:", { 65.0f, 65.0f });
	Renderer::drawString("Tommorow the weather will be:" , FontManager::getFont("Headtitle"), 
		{ screen_size::width / 2.0f - width / 2.0f, screen_size::height - 100.0f, 1.0f },
		{ 65.0f, 65.0f, 1.0f }, {1.0f, 1.0f, 1.0f, 1.0f});

	for (const auto& w : m_Weather)
	{
		Renderer::drawQuad({ 0.0f, 0.0f, 0.0f, 1.0f }, glm::vec3(w.pos, 1.0f),
			{ 200.0f + 10.0f, 200.0f + 10.0f, 1.0f });

		Renderer::drawQuad(w.tex, { 1.0f, 1.0f, 1.0f, 1.0f }, glm::vec3(w.pos, 1.0f),
			{ 200.0f, 200.0f, 1.0f });

		if (clicked(w.pos, { 200.0f, 200.0f }))
		{
			if (w.type == m_CurrentWeather)
				m_CorrectPredicion = true;

			m_State = layer_state::feedback;
		}
	}
}

void GameScene::finish()
{
	float width = FontManager::getFont("Headtitle")->getTextWidth("Congratulations, you've finished the game", { 65.0f, 65.0f });
	Renderer::drawString("Congratulations, you've finished the game", FontManager::getFont("Headtitle"),
		{ screen_size::width / 2.0f - width / 2.0f, screen_size::height / 2.0f, 1.0f },
		{ 65.0f, 65.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

	if (isPressed(GLFW_KEY_ENTER))
		Application::Get()->Close();
}

void GameScene::feedback(float deltaTime)
{
	std::string verdict = "You've got the weather";

	if (m_CorrectPredicion)
	{
		verdict += " right";
		switch (m_CurrentWeather)
		{
		case weather::rain: m_AvailableWeather[0] = true; break;
		case weather::good: m_AvailableWeather[1] = true; break;
		case weather::snow: m_AvailableWeather[2] = true; break;
		case weather::storm: m_AvailableWeather[3] = true; break;

		default:
			break;
		}

		for (const auto& c : m_CorrectFeedback)
			draw_text_box(c, FontManager::getFont("Basic"), { 15.0f, 15.0f });
	}
	else
	{
		verdict += " wrong";
		for (const auto& w : m_WrongFeedback)
			draw_text_box(w, FontManager::getFont("Basic"), { 15.0f, 15.0f });
	}

	float width = FontManager::getFont("Headtitle")->getTextWidth(verdict, { 65.0f, 65.0f });
	Renderer::drawString(verdict, FontManager::getFont("Headtitle"),
		{ screen_size::width / 2.0f - width / 2.0f, screen_size::height - 100.0f, 1.0f },
		{ 65.0f, 65.0f, 1.0f }, {1.0f, 1.0f, 1.0f, 1.0f});

	Renderer::drawString("Press R to play again", FontManager::getFont("Basic"), { screen_size::width - 330.0f, 40.0f, 1.0f },
		{ 25.0f, 25.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });

	if (isPressed(GLFW_KEY_R))
	{
		bool next = false;
		for (int i = 0; i < 4; i++)
			if (m_AvailableWeather[i] == false)
				next = true;

		if (next)
		{
			Reset();
			m_State = layer_state::search;
		}
		else
		{
			m_State = layer_state::finish;
		}
	}
}

#include <fstream>
void GameScene::loadArticles()
{
	std::ifstream str("res/articles.txt");

	std::string line;
	int index = -1;
	while (std::getline(str, line))
	{
		if (!line.empty())
		{
			if (line[line.size() - 1] == '.')
			{
				index++;
				article new_article;

				line = line.substr(0, line.size() - 1);
				new_article.name = line;

				std::string text;
				std::getline(str, text);
				new_article.text = text;

				m_Articles.push_back(new_article);
			}
			else
			{
				if (line == "tex")
				{
					m_Articles[index].tex = std::make_shared<Texture>("res/tex/normal/" + m_Articles[index].name + ".png", GL_NEAREST);
					m_Articles[index].use_texture = true;
				}

				else if (line[0] == '-')
				{
					line.erase(0, 2);
					int w = 0x0;
					if (line.find('p') != std::string::npos)
						w |= (int)weather::rain;
					if (line.find('f') != std::string::npos)
						w |= (int)weather::storm;
					if (line.find('s') != std::string::npos)
						w |= (int)weather::good;
					if (line.find('z') != std::string::npos)
						w |= (int)weather::snow;

					m_Articles[index].weather = w;
				}
			}
		}
	}
	str.close();
}

void GameScene::loadFeedback()
{
	std::ifstream c("res/feedback_c.txt");

	auto feedback_tex = std::make_shared<Texture>("res/tex/feedback.png", GL_NEAREST);

	std::string line;
	while (std::getline(c, line))
	{
		text_box tb;
		tb.tex = feedback_tex;
		tb.height = 40;
		tb.width = 300;
		tb.pos = {150.0f + Random::Float() * 900.0f, 120.0f + Random::Float() * 500.0f };
		tb.offset_x = 10;
		set_text(tb, line, FontManager::getFont("Basic"), { 15.0f, 15.0f });
		m_CorrectFeedback.push_back(tb);
	}

	c.close();

	std::ifstream w("res/feedback_w.txt");

	while (std::getline(w, line))
	{
		text_box tb;
		tb.tex = feedback_tex;
		tb.height = 40;
		tb.width = 300;
		tb.pos = { 150.0f + Random::Float() * 900.0f, 120.0f + Random::Float() * 500.0f };
		tb.offset_x = 10;
		set_text(tb, line, FontManager::getFont("Basic"), { 15.0f, 15.0f });
		m_WrongFeedback.push_back(tb);
	}

	w.close();
}
