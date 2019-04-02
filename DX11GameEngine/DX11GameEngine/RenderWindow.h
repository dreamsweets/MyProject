#pragma once
#include "ErrorLogger.h"

class RenderWindow
{
public:
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	RenderWindow();
	~RenderWindow();
};

