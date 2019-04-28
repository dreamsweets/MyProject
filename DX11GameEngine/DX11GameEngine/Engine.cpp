#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height) 
{
	timer.Start();
	if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height))
		return false;
	if (!gfx.Initialize(this->render_window.GetHWND(), width, height))
		return false;

	return true;
}

bool Engine::ProcessMessages()
{
	return this->render_window.ProcessMessages();
}

void Engine::Update()
{
	Timer::deltaTime = timer.GetMilisecondsElapsed();
	timer.Restart();
	while (!keyboard.CharBufferIsEmpty())
	{
		unsigned char ch = keyboard.ReadChar();
		std::string outmsg = "Chr: ";
		outmsg += ch;
		outmsg += "\n";
		OutputDebugStringA(outmsg.c_str());
	}
	while (!keyboard.KeyBufferIsEmpty())
	{
		KeyboardEvent kbe = keyboard.ReadKey();
		unsigned char keycode = kbe.GetkeyCode();
		std::string outmsg = "";
		if (kbe.IsPress())
		{
			outmsg += "Key press: ";
		}
		if (kbe.IsRelease())
		{
			outmsg += "Key Release: ";
		}
		outmsg += keycode;
		outmsg += "\n";
		OutputDebugStringA(outmsg.c_str());
	}
	while (!mouse.EventBufferIsEmpty())
	{
		MouseEvent me = mouse.ReadEvent();
		if (mouse.IsMiddleDown()) {
			if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
			{
				this->gfx.camera.AdjustRotation(
					(float)me.GetPosY() * 0.006f * Timer::deltaTime,
					(float)me.GetPosX() * 0.006f * Timer::deltaTime,
					0.0f);
			}
		}
	}
	const float cameraSpeed = 0.006f;
	if (keyboard.KeyIsPressed('W'))
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetForwardVector() * cameraSpeed * Timer::deltaTime);
	if (keyboard.KeyIsPressed('S'))
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetBackwardVector() * cameraSpeed * Timer::deltaTime);
	if (keyboard.KeyIsPressed('A'))
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetLeftVector() * cameraSpeed * Timer::deltaTime);
	if (keyboard.KeyIsPressed('D'))
		this->gfx.camera.AdjustPosition(this->gfx.camera.GetRightVector() * cameraSpeed * Timer::deltaTime);
	if (keyboard.KeyIsPressed(VK_SPACE))
		this->gfx.camera.AdjustPosition(0.0f, cameraSpeed * Timer::deltaTime, 0.0f);
	if (keyboard.KeyIsPressed('Z'))
		this->gfx.camera.AdjustPosition(0.0f, -cameraSpeed * Timer::deltaTime, 0.0f);
}

void Engine::RenderFrame()
{
	gfx.RenderFrame();
}
