#pragma once

#include <SDL.h>
#include <string>

#include "../../CRSC/CRSC.h"

class UILabel : public Component
{
public:
	UILabel(int xpos, int ypos, std::string text, std::string font, Uint8 r, Uint8 g, Uint8 b, Uint8 a) :
		labelText(text), labelFont(font)
	{
		textColor = { r, g, b, a };
		position.x = xpos;
		position.y = ypos;

		SetLabelText(labelText, labelFont);
	}
	UILabel(int xpos, int ypos, std::string text, std::string font) :
		labelText(text), labelFont(font)
	{
		textColor = { 0, 0, 0, 255 };
		position.x = xpos;
		position.y = ypos;

		SetLabelText(labelText, labelFont);
	}
	~UILabel()
	{
		SDL_DestroyTexture(labelTexture);
	}

	void SetLabelText(std::string text, std::string font)
	{
		labelTexture = Load(text, font, textColor);
		Query(labelTexture, nullptr, nullptr, &position.x, &position.y);
	}

	void Render() override
	{
		Draw(labelTexture, nullptr, &position);
	}

private:
	SDL_Rect position;
	std::string labelText;
	std::string labelFont;

	SDL_Color textColor;
	SDL_Texture* labelTexture;
};

