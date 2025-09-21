#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Include/stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

class Drawable {
public:
	Drawable(float vertices[], uint8_t line_size);
	~Drawable();

	void Draw();

private:

};

