#pragma once

#include <memory>

#include "Engine/Core/Layer.h"

#include "engine/Graphics/Texture.h"
#include "engine/Graphics/Renderer.h"

#include "engine/Utils/Camera.h"
#include "engine/Utils/FontManager.h"

#include "glm/gtc/matrix_transform.hpp"
#include "engine/text_box.h"
#include "game_scene.h"

class IntroScene : public Layer
{
public:
	IntroScene() = default;
	~IntroScene() = default;

	virtual void onAttach() override;
	virtual void onUpdate(float deltaTime) override;
	static void readIntro();

private:
	std::shared_ptr<Camera> m_Camera;
	std::shared_ptr<Texture> m_MainScreen;
	std::shared_ptr<Texture> m_MainPage;
	std::shared_ptr<Texture> m_PressEnter;

	text_box m_TextBox;

	int m_CurrentPage = 0;
	float m_MaxTime = 0.2f;
	float m_CurrentTime = 0.0f;

	static int m_NoOfPages;
	static std::vector<std::string> m_IntroText;

	enum class state
	{
		MENU,
		STORY,

	} m_LayerState = state::MENU;
	bool m_Transition = false;
};