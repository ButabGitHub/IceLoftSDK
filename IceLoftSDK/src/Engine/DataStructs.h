#include <glm/glm.hpp>

#include <iostream>

// IceLoft version definitions
#define ICL_ENGINE_VER_MAJOR 0
#define ICL_ENGINE_VER_MINOR 0
#define ICL_ENGINE_VER_PATCH 1
#define ICL_ENGINE_VER "0.0.1a"

// Vectors
// -----------------------------------------------------------------------
typedef struct Vec2 {
	float x;
	float y;
} Vec2;

typedef struct Vec3 {
	float x;
	float y;
	float z;
} Vec3;

typedef struct Vec4 {
	float x;
	float y;
	float z;
	float w;
} Vec4;

// Integer vectors
typedef struct IVec2 {
	int x;
	int y;
} IVec2;

typedef struct IVec3 {
	int x;
	int y;
	int z;
} IVec3;

typedef struct IVec4 {
	int x;
	int y;
	int z;
	int w;
} IVec4;

// Color
// -----------------------------------------------------------------------
typedef struct Color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} Color;

// Configs
// -----------------------------------------------------------------------
struct GameConfig {
	// Window-related properties
	const char* Title = "IceLoft Engine"; // 
	const char* WinIconPath = ""; // Path to the image of the window icon, "" = no icon
	IVec2 WinSize = { 1280, 720 };
	IVec2 WinAspectRatio = { 16, 9 };
	bool WinResizable = true;
	int FpsMax = 0; // FPS cap, 0 = unlimited unless VSync is enabled
	bool VSync = false;

};
