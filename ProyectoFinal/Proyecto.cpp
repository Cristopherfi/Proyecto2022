/*---------------------------------------------------------*/
/* ----------------   Proyecto Final  ---------------------*/
/*--------------------    2022-2   ------------------------*/
/*------- Navarrete Acosta Cristopher Antonio -------------*/
/*--------------- No. cuenta: 418048393 -------------------*/
/*---------------------------------------------------------*/

#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <stdio.h>
#include <string.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

FILE *animGato;

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor* monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 30.0f, 90.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, 0.0f);
glm::vec3 lightPosition2(0.0f, -1.0f, -10.0f);
glm::vec3 lightPosition3(0.0f, -1.0f, -10.0f);
double myVariable = 0.0,
	   myVariable2 = 0.0,
       myVariable3 = 0.0,
       varLuzR = 0.0,
       varLuzG = 0.0,
       varLuzB = 0.0;

// posiciones
//float x = 0.0f;
//float y = 0.0f;


/////   Variables de animación   //////
float	movAuto_x = 0.0f,
movAuto_z = 0.0f,
movAuto_y = 0.0f,
orienta = 180.0f,
giroLlanta = 0.0f,
giroGato = 0.0f,
movGato_x = 0.0f,
movGato_y = 0.0f,
movGato_z = 0.0f,
orientagato = 0.0f,
rotacion_pino = -0.0f;

bool	animacion = false,
animacion1 = false,
recorrido1 = true,
recorrido2 = false,
recorrido3 = false,
recorrido4 = false,
recorrido5 = false,
recorrido6 = false,
recorrido7 = false,
recorrido8 = false;

int estado;


//Keyframes (Manipulación y dibujo)

//SantaClaus///////////////////////////////////////////////
float	posX = 0.0f,
posY = 0.0f,
posZ = 0.0f,
giroS = 0.0f,
rotRodIzq = 0.0f,
rotRodDer = 0.0f,
rotPieIzq = 0.0f,
rotPieDer = 0.0f,
giroCabeza = 0.0f,
movCabeza = 0.0f,
movBrazoIzq = 0.0f,
movBrazoDer = 0.0f,
movManoIzq = 0.0f,
movManoDer = 0.0f,
mov1ManoDer = 0.0f;

bool saludoS;	

float Yaw;


float	incX = 0.0f,
incY = 0.0f,
incZ = 0.0f,
rotInc = 0.0f,
rotRodDerInc = 0.0f,
rotPieIzqInc = 0.0f,
rotPieDerInc = 0.0f,
giroCabezaInc = 0.0f,
movCabezaInc = 0.0f,
movBrazoIzqInc = 0.0f,
movBrazoDerInc = 0.0f,
movManoIzqInc = 0.0f,
movManoDerInc = 0.0f,
mov1ManoDerInc = 0.0f, 
duracionS = 0.0f, 
duracionR = 0.0f;
int estadoS = 0;


//Gato////////////////////////////////////////////////////
float gatoX = 0.0f,
gatoY = 0.0f,
gatoZ = 0.0f,
giraGato = 0.0f,
sitGato = 0.0f,
movCabGato = 0.0f,
rotPataD = 0.0f,
rotPataD1 = 0.0f,
rotPataT = 0.0f,
rotPataT1 = 0.0f;

//Aquí van las variables de incremento del gato
float gatoXInc = 0.0f,
gatoYInc = 0.0f,
gatoZInc = 0.0f, 
giraGatoInc = 0.0f,
sitGatoInc = 0.0f,
movCabGatoInc = 0.0f,
rotPataDInc = 0.0f,
rotPataD1Inc = 0.0f,
rotPataTInc = 0.0f,
rotPataT1Inc = 0.0f;

int estadoG = 0;


//Matices de jerarquía de geometría.
glm::mat4 temp1;
glm::mat4 temp2;
glm::mat4 temp3;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float gatoX;
	float gatoY;
	float gatoZ;
	float giraGato;
	float sitGato;
	float movCabGato;
	float rotPataD;
	float rotPataD1;
	float rotPataT;
	float rotPataT1;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;




void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	//Traslación en el escenario
	KeyFrame[FrameIndex].gatoX = gatoX;
	KeyFrame[FrameIndex].gatoY= gatoY;
	KeyFrame[FrameIndex].gatoZ = gatoZ;

	//Rotación sobre Y
	KeyFrame[FrameIndex].giraGato = giraGato;

	//Sentar Gato
	KeyFrame[FrameIndex].sitGato = sitGato;

	//Giro Cabeza
	KeyFrame[FrameIndex].movCabGato = movCabGato;

	//Patas
	KeyFrame[FrameIndex].rotPataD = rotPataD;
	KeyFrame[FrameIndex].rotPataD1 = rotPataD1;
	KeyFrame[FrameIndex].rotPataT = rotPataT;
	KeyFrame[FrameIndex].rotPataT1 = rotPataT1;

	//Guarda el frame en el Archivo
	if (animGato) {
		fwrite(KeyFrame, sizeof(struct _frame), 9, animGato);
	}
	

	FrameIndex++;
}

void resetElements(void)
{

	gatoX = KeyFrame[0].gatoX;
	gatoY = KeyFrame[0].gatoY;
	gatoZ = KeyFrame[0].gatoZ;
	giraGato = KeyFrame[0].giraGato;
	sitGato = KeyFrame[0].sitGato;
	movCabGato = KeyFrame[0].movCabGato;
	rotPataD = KeyFrame[0].rotPataD;
	rotPataD1 = KeyFrame[0].rotPataD1;
	rotPataT = KeyFrame[0].rotPataT;
	rotPataT1 = KeyFrame[0].rotPataT1;
}

void interpolation(void)
{
	gatoXInc = (KeyFrame[playIndex + 1].gatoX - KeyFrame[playIndex].gatoX) / i_max_steps;
	gatoYInc = (KeyFrame[playIndex + 1].gatoY - KeyFrame[playIndex].gatoY) / i_max_steps;
	gatoZInc = (KeyFrame[playIndex + 1].gatoZ - KeyFrame[playIndex].gatoZ) / i_max_steps;

	giraGato = (KeyFrame[playIndex + 1].giraGato - KeyFrame[playIndex].giraGato) / i_max_steps;
	sitGato = (KeyFrame[playIndex + 1].sitGato - KeyFrame[playIndex].sitGato) / i_max_steps;

	rotPataD = (KeyFrame[playIndex + 1].rotPataD - KeyFrame[playIndex].rotPataD) / i_max_steps;
	rotPataD1 = (KeyFrame[playIndex + 1].rotPataD1 - KeyFrame[playIndex].rotPataD1) / i_max_steps;
	rotPataT = (KeyFrame[playIndex + 1].rotPataT - KeyFrame[playIndex].rotPataT) / i_max_steps;
	rotPataT1 = (KeyFrame[playIndex + 1].rotPataT1 - KeyFrame[playIndex].rotPataT1) / i_max_steps;

	movCabGato = (KeyFrame[playIndex + 1].movCabGato - KeyFrame[playIndex].movCabGato) / i_max_steps;
}


//////   Función de animación    //////////////////
void animate(void)
{

	//Saludo Robot
	if (saludoS) {
		switch (estadoS)
		{
		case 0:
			movBrazoDer = 0.0f;
			movManoDer = 0.0f;
			mov1ManoDer = 0.0f;
			estadoS = 1;
			break;

		case 1:
			if (movBrazoDer < 180.0f) {
				movBrazoDer += 4.0f;
				movManoDer += 2.0f;
				mov1ManoDer = 0.0f;
			}
			else {
				estadoS = 2;
			}
			break;
		case 2:
			if (duracionS < 500.0f) {
				movBrazoDer = 180.0f;
				movManoDer = 90.0f;
				mov1ManoDerInc += 5.0f; //velocidad
				mov1ManoDer = -10.0f + 10.0f * glm::sin(glm::radians(mov1ManoDerInc)); //amplitud
				movCabezaInc += 10.0f;
				movCabeza = 0.2 * glm::sin(glm::radians(movCabezaInc));
				duracionS++;
			}
			else {
				estadoS = 3;
			}
			break;
		case 3:
			if (movBrazoDer >=0.0f) {
				movBrazoDer -= 2.0f;
				movManoDer -= 1.0f;
				mov1ManoDer = 0.0f;
			}
			else {
				estadoS = 4;
			}
			break;
		case 4:
			movBrazoDer = 0.0f;
			movManoDer = 0.0f;
			mov1ManoDer = 0.0f;
			duracionS = 0.0f;
			saludoS = false;
			estadoS = 0;
			break;

		default:
			movBrazoDer = .0f;
			movManoDer = .0f;
			mov1ManoDer = 0.0f;
			saludoS = false;
			estadoS = 0;
			break;
		}
	}
	else {
		//Aminación Robot ligado a la camara ///////////////////////////////////////////////////////////////////////////
		posY = glm::sin(glm::radians(incY)) * 0.2f;
		movBrazoDer = glm::sin(glm::radians(movBrazoDerInc)) * 15.0f;
		movBrazoIzq = glm::cos(glm::radians(movBrazoIzqInc)) * 15.0f;
		rotRodDer = glm::sin(glm::radians(rotRodDerInc)) * 30.0f;
		rotPieDer = glm::sin(glm::radians(rotPieDerInc)) * 30.0f;
		if (rotPieDer >= 0.0f) {
			rotPieDer = 0.0f;
		}
		rotPieIzq = glm::sin(glm::radians(-rotPieDerInc)) * 20.0f;
		if (rotPieIzq >= 0.0f) {
			rotPieIzq = 0.0f;
		}
	}


	if (play)
	{
		fclose(animGato);
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}

		// Corre la animación guardada por key frames
		else
		{
			//Draw animation
			gatoX += gatoXInc;
			gatoY += gatoYInc;
			gatoZ += gatoZInc;

			rotPataD += rotPataDInc;
			rotPataD1 += rotPataD1Inc;
			rotPataT += rotPataTInc;
			rotPataT1 += rotPataT1Inc;

			movCabGato += movCabGatoInc;
			sitGato += sitGatoInc;

			i_curr_steps++;
		}
	}

	//Vehículo


	//////////////////////////////////////////////////////////////////////


	if (animacion1) {
		if (recorrido1) {
			movAuto_z -= 3.0f;
			giroLlanta += 9.0f;
			orienta = 180.0f;
			if (movAuto_z <= -300.0f) {
				recorrido1 = false;
				recorrido4 = true;
			}
		}
		if (recorrido2) {
			movAuto_x += 3.0f;
			giroLlanta += 9.0f;
			orienta = 90.0f;
			if (movAuto_x >= 0.0f)
			{
				recorrido2 = false;
				recorrido1 = true;
			}
		}
		if (recorrido3) {
			movAuto_z += 3.0f;
			giroLlanta = 9.0f;
			orienta = 0.0f;
			if (movAuto_z <= 400.0f) {
				recorrido3 = false;
				recorrido2 = true;
			}
		}
		if (recorrido4) {
			movAuto_x -= 3.0f;
			giroLlanta += 9.0f;
			orienta = 270.0f;
			if (movAuto_x <= -300.0f) {
				recorrido4 = false;
				recorrido5 = true;
			}
		}
		if (recorrido5) {
			movAuto_z += 3.0f;
			giroLlanta = 9.0f;
			orienta = 0.0;
			if (movAuto_z >= -150.0f)
			{
				recorrido5 = false;
				recorrido6 = true;
			}
		}
		if (recorrido6) {
			movAuto_x += 3.0f;
			giroLlanta += 9.0f;
			orienta = 90.0;
			if (movAuto_x >= -70.0f)
			{
				recorrido6 = false;
				recorrido7 = true;
			}
		}
		if (recorrido7) {
			movAuto_z += 3.0f;
			giroLlanta += 9.0f;
			orienta = 180.0f;
			if (movAuto_z >= 0.0f)
			{
				recorrido7 = false;
				recorrido8 = true;
			}
		}
		if (recorrido8) {
			movAuto_z += 3.0f;
			giroLlanta += 9.0f;
			orienta = 0.0f;
			if (movAuto_z >= 50.0f)
			{
				recorrido8 = false;
				//recorrido8 = true;
			}
		}
	}

	

}

void getResolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);



	/////////////////////////          Carga de modelos            //////////////////////////////
	///////////////////////// ----------------------------------   //////////////////////////////
	Model piso("resources/objects/piso/piso.obj");


	//Robot
	Model Cabeza("resources/objects/Robot/cabeza.obj");
	Model Torso("resources/objects/Robot/torso.obj");
	Model BrazoDer("resources/objects/Robot/brazoD.obj");
	Model BrazoIzq("resources/objects/Robot/brazoI.obj");
	Model ManoDer("resources/objects/Robot/manoD.obj");
	Model ManoIzq("resources/objects/Robot/manoI.obj");
	Model PiernaDer("resources/objects/Robot/piernaD.obj");
	Model PiernaIzq("resources/objects/Robot/piernaI.obj");
	Model PieDer("resources/objects/Robot/pieD.obj");
	Model PieIzq("resources/objects/Robot/pieI.obj");

	//Auto
	Model Carro("resources/objects/auto/Tesla.obj");
	Model Llanta("resources/objects/auto/rueda.obj");


	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		/*KeyFrame[i].gatoX = 0;
		KeyFrame[i].gatoY = 0;
		KeyFrame[i].gatoZ = 0;
		KeyFrame[i].giraGato = 0;
		KeyFrame[i].sitGato = 0;
		KeyFrame[i].movCabGato = 0;
		KeyFrame[i].rotPataD = 0;
		KeyFrame[i].rotPataD1 = 0;
		KeyFrame[i].rotPataT = 0;
		KeyFrame[i].rotPataT1 = 0;*/
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);



		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);


		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//Piso
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);


		// Carro ///////////////////////////////////////////////////////////////////////////////////////////////
		//model = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(15.0f + movAuto_x, movAuto_y, movAuto_z));
		//tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//staticShader.setMat4("model", model);
		//Carro.Draw(staticShader);


		//model = glm::translate(tmp, glm::vec3(8.5f, 4.5f, 15.9f));
		//model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//staticShader.setMat4("model", model);
		//Llanta.Draw(staticShader);	//Izq delantera

		//model = glm::translate(tmp, glm::vec3(-8.5f, 4.5f, 15.9f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//Llanta.Draw(staticShader);	//Der delantera

		//model = glm::translate(tmp, glm::vec3(-8.5f, 4.5f, -18.5f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//staticShader.setMat4("model", model);
		//Llanta.Draw(staticShader);	//Der trasera

		//model = glm::translate(tmp, glm::vec3(8.5f, 4.5f, -18.5f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		//staticShader.setMat4("model", model);
		//Llanta.Draw(staticShader);	//Izq trase


		//Robot. Cálculo de transformaciones y Dibujo ///////////////////////////////////////////////////////////////////
		//Torso
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.0f, -20.0f + posY, -15.0f) + camera.Position);
		model = glm::rotate(model, glm::radians(giroS), glm::vec3(0.0f, 1.0f, 0.0f));
		temp1 = model = glm::scale(model, glm::vec3(0.5f));
		model = glm::translate(model, glm::vec3(0.0f, 17.5f, 0.0f));
		staticShader.setMat4("model", model);
		Torso.Draw(staticShader);

		//Cabeza
		model = glm::mat4(temp1);
		model = glm::translate(model, glm::vec3(0.0f, 0.2f + movCabeza, 0.0f)); //movCabeza en el eje Y para moverla
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //giroCabeza en el eje Y para girar
		model = glm::translate(model, glm::vec3(0.0f, 18.5f, -1.0f));
		staticShader.setMat4("model", model);
		Cabeza.Draw(staticShader);

		//Brazo Derecho
		model = glm::mat4(temp1);
		model = glm::translate(model, glm::vec3(0.0f, 17.5f, 0.0f));
		temp2 = model = glm::rotate(model, glm::radians(movBrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		BrazoDer.Draw(staticShader);

		//Mano derecha
		model = glm::mat4(temp2);
		model = glm::translate(model, glm::vec3(4.0f, -4.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movManoDer), glm::vec3(0.0f, 1.0f, 0.0f)); //
		model = glm::rotate(model, glm::radians(mov1ManoDer), glm::vec3(1.0f, 0.0f, 0.0f)); //
		staticShader.setMat4("model", model);
		ManoDer.Draw(staticShader);

		//Brazo Izquierdo
		model = glm::mat4(temp1);
		model = glm::translate(model, glm::vec3(0.0f, 17.5f, 0.0f));
		temp2 = model = glm::rotate(model, glm::radians(movBrazoIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		BrazoIzq.Draw(staticShader);

		//Mano Izquierda
		model = glm::mat4(temp2);
		model = glm::translate(model, glm::vec3(-4.0f, -4.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //
		staticShader.setMat4("model", model);
		ManoIzq.Draw(staticShader);

		//Pierna Derecha
		model = glm::mat4(temp1);
		model = glm::translate(model, glm::vec3(0.0f, 9.5f, 0.0f));
		temp2 = model = glm::rotate(model, glm::radians(rotRodDer), glm::vec3(1.0f, 0.0f, 0.0f)); //
		staticShader.setMat4("model", model);
		PiernaDer.Draw(staticShader);


		//Pie Derecho
		model = glm::mat4(temp2);
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPieDer), glm::vec3(1.0f, 0.0f, 0.0f)); //
		staticShader.setMat4("model", model);
		PieDer.Draw(staticShader);

		//Pierna Izquierda
		model = glm::mat4(temp1);
		model = glm::translate(model, glm::vec3(0.0f, 9.5f, 0.0f));
		temp2 = model = glm::rotate(model, glm::radians(-rotRodDer), glm::vec3(1.0f, 0.0f, 0.0f)); //
		staticShader.setMat4("model", model);
		PiernaIzq.Draw(staticShader);

		//Pie Izquierdo
		model = glm::mat4(temp2);
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotPieIzq), glm::vec3(1.0f, 0.0f, 0.0f)); //
		staticShader.setMat4("model", model);
		PieIzq.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Objetos Transparentes
		// -------------------------------------------------------------------------------------------------------------------------
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		/*model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		Vidrios.Draw(staticShader);*/

		glDisable(GL_BLEND);


		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		posZ = camera.ProcessKeyboard(FORWARD, (float)deltaTime);
		Yaw = camera.Yaw;
		incY += 30.0f;
		movBrazoDerInc += 15.0f;
		movBrazoIzqInc += 15.0f;
		rotRodDerInc -= 15.0f;
		rotPieDerInc += 15.0f;
	}
		
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
		incY -= 20.0f;
		movBrazoDerInc -= 15.0f;
		movBrazoIzqInc -= 15.0f;
		rotRodDerInc += 15.0f;
		rotPieDerInc -= 15.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
		incY += 20.0f;
		movBrazoDerInc += 15.0f;
		movBrazoIzqInc += 15.0f;
		rotRodDerInc -= 15.0f;
		rotPieDerInc += 15.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
		incY += 20.0f;
		movBrazoDerInc += 15.0f;
		movBrazoIzqInc += 15.0f;
		rotRodDerInc -= 15.0f;
		rotPieDerInc += 15.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.ProcessKeyboard(UP, (float)deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.ProcessKeyboard(DOWN, (float)deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {		//Giro Santa
		giroS += 2.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {		
		giroS -= 2.0f;
	}

	//Animación saludo Robot
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
		saludoS = true;
		estadoS = 0;
	}




	//To Configure Model Gato
	/*if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		gatoZ++;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		gatoZ--;*/

	//}
	//if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
	//	rotRodIzq++;
	//	rotRodDer--;
	//}
	//if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
	//	giroCabeza--;
	//if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	//	giroCabeza++;
	//if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
	//	movBrazoIzq += 3.0f;
	//	movBrazoDer -= 3.0f;
	//}
	//if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
	//	movBrazoIzq -= 3.0f;
	//	movBrazoDer += 3.0f;
	//}
	//if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	//	movCabeza++;
	//if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	//	movCabeza--;


	//Car animation TECLA ESPACIO PARA QUE COCHE HAGA SU RECORRIDO 
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion1 ^= true;
	if (key == GLFW_KEY_M && action == GLFW_PRESS && movAuto_z < 200)
		movAuto_z = 0.0f;
	estado = 0;

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

//glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}