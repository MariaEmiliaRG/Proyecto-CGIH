/*
Semestre 2023-2
Animación:
1.- Simple o básica:Por banderas y condicionales
2.- Compleja: Por medio de funciones y algoritmos,Textura Animada.
3.- Técnicas de Animación: Por Keyframes
Adicional.- Texturizado con transparencia usando Blending: Requerimos dibujar lo que está atras primero

*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float toffsetu = 0.0f;
float toffsetv = 0.0f;
float reproduciranimacion, habilitaranimacion,
guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;


// Animación compleja 1
int medusa_bandera = 1;
float medusa_X = 0;
float medusa_Y = 0;
float medusa_Z = 0;
float medusa_rot = 0;

// Animación compleja 2
float neritantan_rot = 0;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

//MODELOS DEL PROYECTO 

Model escenario; 
Model blazeReap; 
Model brujula; 
Model cuchillo; 
Model faputa; 
Model gaburoon; 
Model neritantan; 
Model planta; 
Model reg;
Model reliquia; 
Model roca; 
Model medusa;


Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;



Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;
Model Dado_M;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//PARA INPUT CON KEYFRAMES 
void inputKeyframes(bool* keys);


//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

///////////////////////////////KEYFRAMES/////////////////////


bool animacion = false;


//NEW// Keyframes

#define MAX_FRAMES 30

int i_max_steps = 90;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int playIndex = 0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	KeyFrame[FrameIndex].giroAvion = giroAvion;

	FrameIndex++;
}

void resetElements(void)
{
}

void interpolation(void)
{
}


void animate(void)
{
	//Movimiento del objeto
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aquí\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se quedó aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			i_curr_steps++;
		}

	}
}

/* FIN KEYFRAMES*/



/* FIN KEYFRAMES*/


int main()
{
	mainWindow = Window(1960, 1080); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);


	escenario = Model();
	escenario.LoadModel("Obj/Escenario.obj");
	blazeReap = Model();
	blazeReap.LoadModel("Obj/BlazeReap.obj");
	brujula = Model();
	brujula.LoadModel("Obj/Brujula.obj");
	cuchillo = Model();
	cuchillo.LoadModel("Obj/Cuchillo.obj");
	faputa = Model();
	faputa.LoadModel("Obj/Faputa_full.obj");
	gaburoon = Model();
	gaburoon.LoadModel("Obj/Gaburoon_full.obj");
	neritantan = Model();
	neritantan.LoadModel("Obj/Neritantan.obj");
	planta = Model();
	planta.LoadModel("Obj/Planta.obj");
	reg = Model();
	reg.LoadModel("Obj/Reg_full.obj");
	reliquia = Model();
	reliquia.LoadModel("Obj/Reliquia.obj");
	roca = Model();
	roca.LoadModel("Obj/Roca.obj");
	medusa = Model();
	medusa.LoadModel("Obj/Swarm-shocker.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;


	//KEYFRAMES DECLARADOS INICIALES

	KeyFrame[0].movRoca_y = 0.0f;
	KeyFrame[1].movRoca_y = 2.0f;
	KeyFrame[2].movRoca_y = 6.0f;
	KeyFrame[3].movRoca_y = 8.0f;
	KeyFrame[4].movRoca_y = 10.0f;
	KeyFrame[5].movRoca_y = 10.0f;
	KeyFrame[6].movRoca_y = 8.0f;
	KeyFrame[7].movRoca_y = 6.0f;
	KeyFrame[8].movRoca_y = 2.0f;
	KeyFrame[9].movRoca_y = 0.0f;

	FrameIndex = 10;
	i_curr_steps = 10;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	int i = 0;
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		// Animación por keyframes
		animate();

		// Animación compleja 
		switch (medusa_bandera)
		{
		case 1:
			medusa_X += 0.1 * deltaTime;
			if (medusa_X >= 40) {
				medusa_X = 40;
				medusa_bandera++;
			}
			break;
		case 2:
			medusa_rot += 1.0 * deltaTime;
			if (medusa_rot >= 90) {
				medusa_rot = 90;
				medusa_bandera++;
			}
			break;
		case 3:
			medusa_Z += 0.1 * deltaTime;
			if (medusa_Z >= 40) {
				medusa_Z = 40;
				medusa_bandera++;
			}
			break;
		case 4:
			medusa_rot += 1.0 * deltaTime;
			if (medusa_rot >= 180) {
				medusa_rot = 180;
				medusa_bandera++;
			}
			break;
		case 5:
			medusa_X -= 0.1 * deltaTime;
			if (medusa_X <= 0) {
				medusa_X = 0;
				medusa_bandera++;
			}
			break;
		case 6:
			medusa_rot += 1.0 * deltaTime;
			if (medusa_rot >= 270) {
				medusa_rot = 270;
				medusa_bandera++;
			}
			break;
		case 7:
			medusa_Z -= 0.1 * deltaTime;
			if (medusa_Z <= 0) {
				medusa_Z = 0;
				medusa_bandera++;
			}
			break;
		case 8:
			medusa_rot += 1.0 * deltaTime;
			if (medusa_rot >= 360) {
				medusa_rot = 0;
				medusa_bandera = 1;
			}
			break;
		default:
			medusa_X = 0;
			medusa_Y = 0;
			medusa_Z = 0;
			medusa_bandera = 1;
			medusa_rot = 0;
			break;
		}

		medusa_Y += 2.0 * deltaTime;
		neritantan_rot += 0.7 * deltaTime;

		if (medusa_Y >= 360) {
			medusa_Y = 0;
		}

		if (neritantan_rot >= 360) {
			neritantan_rot = 0;
		}

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		//animate();


		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);


		//ESCENARIO

		model = glm::mat4(1.0); 
		model = glm::translate(model, glm::vec3(0.0f, -50.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f, 4.0f, 7.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess); 
		escenario.RenderModel(); 

		//COSAS VARIAS
		
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(17.0f, 0.0f, 25.0f));
		model = glm::scale(model, glm::vec3(0.5f,0.5f, 0.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); 
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		blazeReap.RenderModel();

		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-35.0f, 5.0f, -28.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brujula.RenderModel();

		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, 19.0));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); 
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuchillo.RenderModel();


		float rocas[] = {
			10.0f, 2.0f, -56.0f, 5.0f, 1.0f, 5.0f,
			15.0f, 2.0f, -49.0f, 3.0f, 1.0f, 5.0f,
			-38.0f, 2.0f, -16.0f, 3.0f, 2.0f, 4.0f,
			-25.0f, 3.0f, -20.0f, 4.0f, 1.0f, 3.0f,
			-30.0f, 2.0f, -18.0f, 5.0f, 2.0f, 1.5f,
			-48.0f, 2.0f, 35.0f, 4.0f, 2.0f, 2.0f,
			-36.0f, 2.0f, 42.0f, 3.0f, 2.0f, 5.0f,
			20.0f, 2.0f, 65.0f, 6.0f, 3.0f, 3.0f,
			25.0f, 2.0f, 50.0f, 3.0f, 1.0f, 2.0f,
			23.0f, 2.0f, 55.0f, 3.0f, 1.0f, 5.0f,
			85.0f, 2.0f, 35.0f, 5.0f, 3.5f, 1.0f,
			64.0f, 2.0f, 15.0f, 6.0f, 3.0f, 2.5f,
			95.0f, 2.0f, 26.0f, 4.5f, 2.0f, 8.0f, 
			72.0f, 2.0f, 45.0f, 5.0f, 3.5f, 4.0f,
			34.0f, 2.0f, 65.0f, 3.0f, 3.0f, 6.5f,
			86.0f, 2.0f, 32.0f, 6.5f, 2.0f, 7.0f,
			20.0f, 2.0f, -65.0f, 6.0f, 3.0f, 3.0f,
			25.0f, 2.0f, -50.0f, 2.0f, 1.0f, 2.0f,
			23.0f, 2.0f, -55.0f, 3.0f, 1.0f, 5.0f,
			85.0f, 2.0f, -35.0f, 4.0f, 3.5f, 9.0f,
			64.0f, 2.0f, -15.0f, 6.0f, 3.0f, 2.5f,
			95.0f, 2.0f, -26.0f, 4.5f, 2.0f, 4.0f,
			72.0f, 2.0f, -45.0f, 1.0f, 3.5f, 5.0f,
			34.0f, 2.0f, -65.0f, 3.0f, 3.0f, 6.5f,
			86.0f, 2.0f, -32.0f, 7.5f, 2.0f, 4.0f,
			-34.0f, 2.0f, -65.0f, 3.0f, 1.0f, 6.5f,
			-86.0f, 2.0f, -32.0f, 4.5f, 2.0f, 6.0f,
			-20.0f, 2.0f, -65.0f, 1.0f, 1.0f, 3.0f,
			-25.0f, 2.0f, -50.0f, 2.0f, 1.0f, 2.0f,
			-23.0f, 2.0f, -55.0f, 3.0f, 1.0f, 5.0f,
			-85.0f, 2.0f, -35.0f, 4.0f, 3.5f, 8.0f,
			-64.0f, 2.0f, -15.0f, 5.0f, 2.0f, 2.5f,
			-95.0f, 2.0f, -26.0f, 4.5f, 2.0f, 4.0f,
			-72.0f, 2.0f, -45.0f, 1.0f, 1.5f, 5.0f,
			-34.0f, 2.0f, -65.0f, 3.0f, 3.0f, 3.5f,
			-86.0f, 2.0f, -32.0f, 2.5f, 2.0f, 5.0f,

		};

		int j;

		for (i = 0; i < (sizeof(rocas)/sizeof(rocas[0]))/6 ; i++) {

			j = 6 * i;
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(rocas[j], rocas[j+1], rocas[j+2]));
			model = glm::scale(model, glm::vec3(rocas[j+3], rocas[j+4], rocas[j+5]));
			model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			roca.RenderModel();
		}


		float rocasF[] = {
			100, 30, -120, 10,8,17,
			95, 36, -98, 20,7,10, 
			75, 30, -80, 10,9,12,
			-87, 30, -120, 12,8,11,
			-65, 20, -98, 25,7,13,
			-79, 25, -80, 14,10,12

		};

		for (i = 0; i < (sizeof(rocasF) / sizeof(rocasF[0])) / 6; i++) {

			j = 6 * i;
			model = glm::mat4(1.0);
			model = glm::scale(model, glm::vec3(rocasF[j + 3], rocasF[j + 4], rocasF[j + 5]));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			roca.RenderModel();
		}

		if (!play) {
			resetElements();			
			interpolation();
			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}

		float plantas[] = {
			12.0f, 1.0f, -51.0f, 1.0f, 2.0f, 1.0f,
			10.0f, 2.0f, -40.0f, 0.5f, 0.5f, 0.5f,
			-38.0f, 2.0f, -16.0f, 1.0f, 2.0f, 1.0f,
			-27.0f, 3.0f, -23.0f, 1.5f, 1.0f, 1.0f,
			-30.0f, 2.0f, -18.0f, 1.0f, 2.0f, 1.0f,
			-45.0f, 2.0f, 27.0f, 1.0f, 0.5f, 1.0f,
			-30.0f, 2.0f, 32.0f, 3.0f, 2.0f, 1.0f,
			14.0f, 2.0f, 45.0f, 1.0f, 1.0f, 1.0f,
			30.0f, 2.0f, 36.0f, 1.0f, 1.5f, 1.0f,
			32.0f, 2.0f, 25.0f, 0.5f, 0.5f, 0.5f
		};

		for (i = 0; i < (sizeof(plantas) / sizeof(plantas[0])) / 6; i++) {

			j = 6 * i;
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(plantas[j], plantas[j + 1], plantas[j + 2]));
			model = glm::scale(model, glm::vec3(plantas[j + 3], plantas[j + 4], plantas[j + 5]));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			planta.RenderModel();
		}

		model = glm::mat4(1.0);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		neritantan.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(medusa_X - 20, 50.0f + (sin(glm::radians(medusa_Y))) * 4, medusa_Z - 20));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, medusa_rot * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		medusa.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(62.0f, 13.0f, 10.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		reliquia.RenderModel();

		//AVATARS
		
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 17.0f, .0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		gaburoon.RenderModel();
		

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(48.0f, -3.0f, 36.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)); 
		model = glm::rotate(model, -115 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		faputa.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, -3.0f, -44.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, -105 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		reg.RenderModel();


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)

		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\n presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;
			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			printf("movAvion_x es: %f\n", movAvion_x);
			//printf("movAvion_y es: %f\n", movAvion_y);
			printf(" \npresiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
		}
	}


	if (keys[GLFW_KEY_1])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			movAvion_x += 1.0f;
			printf("\n movAvion_x es: %f\n", movAvion_x);
			ciclo++;
			ciclo2 = 0;
			printf("\n reinicia con 2\n");
		}

	}
	if (keys[GLFW_KEY_2])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
		}
	}

}

