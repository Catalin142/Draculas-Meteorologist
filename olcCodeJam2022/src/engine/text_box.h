#pragma once
#include "Graphics/Texture.h"
#include "Graphics/Renderer.h"
#include "Utils/Font.h"

#include <memory>
#include <glm/glm.hpp>
#include <sstream>

struct text_box
{
	std::shared_ptr<Texture> tex;
	glm::vec2 pos;
	int width;
	int height;
	float offset_x = 10.0f;
	float offset_y = 10.0f;

	std::vector<std::string> texts;
};

static void set_text(text_box& box, const std::string& text, const std::shared_ptr<Font>& font, const glm::vec2 text_scale)
{
	box.texts.clear();
	box.texts.resize(5);

	float text_width = font->getTextWidth(text, text_scale);
	if (text_width > box.width)
	{
		int index = 0;

		std::stringstream str(text);
		std::string word;

		while (str >> word)
		{
			box.texts[index] += word;

			if (font->getTextWidth(box.texts[index], text_scale) > box.width)
			{
				int pos = box.texts[index].find_last_of(' ');
				box.texts[index] = box.texts[index].substr(0, pos);
				index++;
				box.texts[index] += word;
				box.texts[index] += " ";
			}
			box.texts[index] += " ";

		}
	
	}
	else box.texts[0] = text;
}

static void draw_text_box(const text_box& box, const std::shared_ptr<Font>& font, const glm::vec2 text_scale)
{
	Renderer::drawQuad(box.tex, { 1.0f, 1.0f, 1.0f, 1.0f }, glm::vec3(box.pos, 1.0f), { box.width + box.offset_x * 2, box.height + box.offset_y * 2, 1.0f });

	int index = 0;
	for (const auto& line : box.texts)
	{
		float line_width = font->getTextWidth(line, text_scale);
		Renderer::drawString(line, font, glm::vec3(box.pos - glm::vec2(line_width / 2.0f + 5.0f, (index * (text_scale.y + 2))) + 10.0f, 1.0f), glm::vec3(text_scale, 1.0f));
		index++;
	}
}