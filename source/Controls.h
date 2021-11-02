#ifndef _CONTROLS_
#define _CONTROLS_

#include <gl\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

//unsigned int screenWidth = 1280;
//unsigned int screenHeight = 720;

using namespace std;
using namespace glm;

struct Camera
{
	vec3 p = vec3(7.0f, 10.0f, -5.0f);

	float HorizontalAngle = 4.71f;
	float VerticalAngle = 0.0f;
	float mouseSpeed = 0.005f;
	float moveSpeed = 0.25f;
	float fov = 60.0f;
	vec3 d;
	mat4 pMat;
	mat4 vMat;
	mat4 mMat = mat4(1.0f);

	int screenWidth;
	int screenHeight;

	float respect_ratio;
	Camera()
	{
		//p = vec3(7.0f, 10.0f, -5.0f);
		//HorizontalAngle = 4.71f;
		//VerticalAngle = 0.0f;
		//mouseSpeed = 0.005f;
		//moveSpeed = 0.05f;
		//fov = 45.0f;
		//mMat = mat4(1.0f);
	}
	Camera(int w, int h) : screenWidth(w), screenHeight(h) { respect_ratio = (float)screenWidth / screenHeight; }

	Camera(vec3 pos, float HorizontalAngle_, float VerticalAngle_, float mouseSpeed_, float moveSpeed_, float fov_) :
		p(pos), HorizontalAngle(HorizontalAngle_), VerticalAngle(VerticalAngle_), mouseSpeed(mouseSpeed_), moveSpeed(moveSpeed_), fov(fov_)
	{}
	 
	void Compute_Matrix(GLFWwindow*& window)
	{
		double lastTime = glfwGetTime();
		double currentTime = glfwGetTime();

		float deltaTime = currentTime - lastTime;

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		glfwSetCursorPos(window, screenWidth / 2, screenHeight / 2);

		HorizontalAngle += mouseSpeed * float(screenWidth / 2 - xPos);
		VerticalAngle -= mouseSpeed * float(screenHeight / 2 - yPos);

		vec3 direction(
			cosf(VerticalAngle) * sinf(HorizontalAngle),
			sinf(VerticalAngle),
			cosf(VerticalAngle) * cosf(HorizontalAngle));
		d = direction;
		vec3 right(
			sinf(HorizontalAngle - 3.1415926 / 2.0f),
			0,
			cosf(HorizontalAngle - 3.1415926 / 2.0f));

		vec3 up(cross(right, direction));

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			p += direction * moveSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			p -= direction * moveSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			p += right * moveSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			p -= right * moveSpeed;
		}

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwTerminate();
		}

		pMat = perspective(fov * 0.5f, respect_ratio, 0.1f, 4000.0f);
		vMat = lookAt(p, p + direction, up);

		lastTime = currentTime;
	}

};

#endif // !_CONTROLS_
