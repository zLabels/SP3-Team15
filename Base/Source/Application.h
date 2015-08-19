#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "StudioProject.h"

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();

	static bool IsKeyPressed(unsigned short key);
	static bool IsMousePressed(unsigned short key);
	static bool GetMouseUpdate();
	bool GetKeyBoardUpdate();

	static void GetCursorPos(double *xpos, double *ypos);
	static int GetWindowWidth();
	static int GetWindowHeight();

	static double mouse_last_x, mouse_last_y, mouse_current_x,
		mouse_current_y, mouse_diff_x, mouse_diff_y;
	static double camera_yaw, camera_pitch;

private:
	Application();
	~Application();

	//Declare a window object
	StopWatch m_timer;
	double m_dElapsedTime;
	double m_dAccumulatedTime;

	// Declare the window width and height as constant integer
	const static int m_window_deadzone = 100;
	const static int m_window_width = 1920;
	const static int m_window_height = 1080;
};

#endif