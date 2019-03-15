#include "Renderer.h"

Renderer* Renderer::renderer = nullptr;

Renderer& Renderer::getInstance() {
	if (!Renderer::renderer) renderer = new Renderer();
	return *renderer;
}