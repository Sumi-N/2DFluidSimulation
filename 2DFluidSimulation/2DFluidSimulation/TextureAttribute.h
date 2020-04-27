#pragma once
#include <vector>
#include "lodepng.h"

struct color
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};


class TextureAttribute
{
public:
	std::vector<color> pixels;
	int width, height;

	virtual bool Load(char const* i_filename);
};

inline bool TextureAttribute::Load(char const* i_filename)
{
	pixels.clear();
	width = 0;
	height = 0;
	char const* name = i_filename ? i_filename : "";
	if (name[0] == '\0') return false;

	int len = (int)strlen(name);
	if (len < 3) return false;

	bool success = false;

	char ext[3] = { (char)tolower(name[len - 3]), (char)tolower(name[len - 2]), (char)tolower(name[len - 1]) };

	if (strncmp(ext, "png", 3) == 0)
	{
		std::vector<unsigned char> d;
		unsigned int w, h;
		unsigned int error = lodepng::decode(d, w, h, name, LCT_RGB);
		if (error == 0)
		{
			width = w;
			height = h;
			pixels.resize(width * height);
			memcpy(pixels.data(), d.data(), width * height * 3);
		}
		success = (error == 0);
	}

	if (success)
	{
		printf("Succeed to load texture %s \n", i_filename);
	}
	else
	{
		printf("Failed to load texture %s \n", i_filename);
	}

	return success;
}