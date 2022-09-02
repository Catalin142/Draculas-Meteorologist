#pragma once

#include <memory>
#include <vector>

#include "Engine/Core/Layer.h"
#include "engine/Graphics/Texture.h"
#include "engine/Utils/Camera.h"
#include "engine/text_box.h"

enum class weather
{
	rain = (1 << 0),
	good = (1 << 1),
	snow = (1 << 2),
	storm = (1 << 3),

	none = (1 << 4),
};

struct article
{
	std::string name;
	std::string text;

	glm::vec2 position;

	bool use_texture = false;
	std::shared_ptr<Texture> tex;

	int weather = (int)weather::none;
};

class GameScene : public Layer
{
public:
	GameScene() = default;
	~GameScene() = default;

	virtual void onAttach() override;
	virtual void onUpdate(float deltaTime) override;

	static void loadArticles();

private:
	void Reset();
	void drawArticles();
	void openArticles();
	void drawOpenedArticle();

private:
	std::shared_ptr<Camera> m_Camera;
	std::shared_ptr<Texture> m_MainPage;
	std::shared_ptr<Texture> m_NoPhoto;

	static std::vector<article> m_Articles;
	std::vector<article*> m_ActiveArticles;

	int m_CurrentPage = 0;
	int m_MaxArticlesPerPage = 6;
	float m_MaxTime = 0.2f;
	float m_CurrentTime = 0.0f;

	int m_ArticleWidth = 150.0f;
	int m_ArticleHeight = 150.0f;

	int m_Vol;
	int m_NO;

	bool m_OpenArticle = false;
	int m_OpenedArticle = -1;

	weather m_CurrentWeather;
	bool m_AvailableWeather[4];

	text_box m_TextBox;

	enum class layer_state
	{
		search,
		predict,
		feedback,
		finish,

	} m_State = layer_state::search;

	bool m_CorrectPredicion = false;
	std::vector<text_box> m_CorrectFeedback;
	std::vector<text_box> m_WrongFeedback;

private:
	void loadFeedback();

	void search(float deltaTime);
	void predict(float deltaTime);
	void finish();

	struct weather_pick
	{
		std::shared_ptr<Texture> tex;
		glm::vec2 pos;
		weather type;

		weather_pick() = default;
		weather_pick(const std::shared_ptr<Texture> t) : tex(t) {}
	};

	std::vector<weather_pick> m_Weather;

	void feedback(float deltaTime);
};