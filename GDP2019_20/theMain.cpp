#include "globalStuff.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/string_cast.hpp>

#include <stdlib.h>		// c libs
#include <stdio.h>		// c libs
#include <iostream>		// C++ IO standard stuff-
#include "cShaderManager.h"

// The Physics function
#include "cPhysics.h"
#include "cLowPassFilter.h"
#include "DebugRenderer/cDebugRenderer.h"
#include "Scene.h"
#include "Camera.h"
#include "FollowCamera.h"
#include "SceneEditor.h"
#include "JSON_IO.h"
#include "AABBGrid.h"
#include "DollyCamera.h"
#include "cFBO.h"

// Keyboard, error, mouse, etc. are now here
#include "GFLW_callbacks.h"

// audio things
#include "RenderManager.h"

cShaderManager theShaderManager;
std::string shader_name = "SimpleShader";
std::string config_filename = "assets/config.json";
cVAOManager* theVAOManager = new cVAOManager();
GLFWwindow* ::window = 0;
cBasicTextureManager* ::g_pTextureManager = new cBasicTextureManager();
cDebugRenderer* ::g_pDebugRenderer = new cDebugRenderer();
AABBGrid* pAABBgrid = new AABBGrid();
DollyCamera* dollyCamera = DollyCamera::getTheCamera();
bool ::isDebug = false, ::isRunning = false;
GLuint g_programID = 0;
glm::mat4 viewTransform = glm::mat4(1), projTransform = glm::mat4(1);

int main(void)
{
	Camera* theCamera = FollowCamera::getTheCamera();
	SceneEditor *sceneEditor = SceneEditor::getTheEditor();
	glm::vec3 cameraOffset(0, 30 ,-50);

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	int init_width = 1600, init_height = 800;
	::window = glfwCreateWindow(init_width, init_height, "SimpleGame", NULL, NULL);
	if (!::window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(::window, key_callback);
	glfwMakeContextCurrent(::window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	cDebugRenderer* pDebugRenderer = new cDebugRenderer();
	pDebugRenderer->initialize();
	::g_pDebugRenderer->initialize();

	cShaderManager::cShader vertexShad;
	vertexShad.fileName = "assets/shaders/vertexShader01.glsl";

	cShaderManager::cShader fragShader;
	fragShader.fileName = "assets/shaders/fragmentShader01.glsl";

	if (!::theShaderManager.createProgramFromFile(::shader_name, vertexShad, fragShader))
	{
		std::cout << "Error: didn't compile the shader" << std::endl;
		std::cout << theShaderManager.getLastError();
		return -1;
	}

	::g_programID = ::theShaderManager.getIDFromFriendlyName(::shader_name);

	
	/*
	*   _____      _   _   _                              _____                          
	*  / ____|    | | | | (_)                            / ____|                         
	* | (___   ___| |_| |_ _ _ __   __ _   _   _ _ __   | (___   ___ ___ _ __   ___  ___ 
	*  \___ \ / _ \ __| __| | '_ \ / _` | | | | | '_ \   \___ \ / __/ _ \ '_ \ / _ \/ __|
	*  ____) |  __/ |_| |_| | | | | (_| | | |_| | |_) |  ____) | (_|  __/ | | |  __/\__ \
	* |_____/ \___|\__|\__|_|_| |_|\__, |  \__,_| .__/  |_____/ \___\___|_| |_|\___||___/
	*                               __/ |       | |                                      
	*                              |___/        |_|                                      
	*/
	if(!loadScenes(::config_filename))
	{
		return 1;
	}

	cLowPassFilter avgDeltaTimeThingy;

	// Get the initial time
	double lastTime = glfwGetTime();
	double flickerTimer = 0;

	//cFBO *pTheFBO = new cFBO();
	//std::string FBOError;
	//if (!pTheFBO->init(init_width, init_height, FBOError))
	//{
	//	printf("FBOError: %s\n", FBOError.c_str());
	//	return 1;
	//}

	theCamera->setTarget(glm::vec3(0, 30, 0));

	while (!glfwWindowShouldClose(window))
	{
		// Doesn't work if all the texture units aren't asigned
		glUniform1i(glGetUniformLocation(g_programID, "skyBox"), 26);	// Texture unit 26
	

		// Get the initial time
		double currentTime = glfwGetTime();

		// Frame time... (how many seconds since last frame)
		double deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		const double SOME_HUGE_TIME = 0.1;	// 100 ms;
		if (deltaTime > SOME_HUGE_TIME)
		{
			deltaTime = SOME_HUGE_TIME;
		}

		avgDeltaTimeThingy.addValue(deltaTime);
		double averageDeltaTime = avgDeltaTimeThingy.getAverage();

		glUseProgram(g_programID);

		//    _____                _           _                _____                           
		//   |  __ \              | |         (_)              / ____|                          
		//   | |__) |___ _ __   __| | ___ _ __ _ _ __   __ _  | (___   ___ ___ _ __   ___  ___  
		//   |  _  // _ \ '_ \ / _` |/ _ \ '__| | '_ \ / _` |  \___ \ / __/ _ \ '_ \ / _ \/ __| 
		//   | | \ \  __/ | | | (_| |  __/ |  | | | | | (_| |  ____) | (_|  __/ | | |  __/\__ \ 
		//   |_|  \_\___|_| |_|\__,_|\___|_|  |_|_| |_|\__, | |_____/ \___\___|_| |_|\___||___/ 
		//                                              __/ |                                   
		//                                             |___/                                    

		/*
		 * Scene 1 (Terrain and airships)
		 */
		//RenderManager::deferredDraw(
		//	theCamera,
		//	"Inside"
		//);
		
		//Camera tv_camera;
		/*
		 * Outside screen 
		 */
		//RenderManager::deferredDraw(
		//	theCamera->getPosition(),
		//	//glm::vec3(0, 0, -100),
		//	theCamera->getTarget(),
		//	//glm::vec3(0, 0, 0),
		//	"LastPass"
		//);
		
		RenderManager::renderStencilPortal(
			theCamera,
			"Outside",
			"Portal",
			"Inside"
		);
		
		RenderManager::deferredDraw(
			glm::vec3(0, 0, -10),
			glm::vec3(0,0,0),
			"LastPass"
		);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}