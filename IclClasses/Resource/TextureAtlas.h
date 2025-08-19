#pragma once
#include "Texture.h"

class TextureAtlas : public Texture2D {
public:

	//Constructor
	TextureAtlas();

	Texture2D get_texture(int from_x, int from_y, int to_x, int to_y);

private:

};

