#include <gl\glew.h>
#include <gl\freeglut.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Utility.h"
#include "Load_Model_Correct.h"
#include "Controls.h"

using namespace std;
using namespace glm;

int width = 1920;//1024
int height = 1080;//768


vec3 lookFrom(7.0f, 10.0f, -5.0f);
vec3 direction(0.0f, 0.0f, -1.0f);


vec3 Up(0, 1, 0);
float rotation_angle = 0.0f;
float increase = 0.015;

//vbo[0] : vertex + texture
//vbo[1] : normal

GLuint program;
//GLuint vao;
//GLuint vbo_vertices;
//GLuint vbo_texcoords;
//GLuint vbo_normals;
//GLuint ibo;

GLuint mvLoc;
GLuint pLoc;
GLuint mvpLoc;
GLuint nLoc;

//material base
GLuint useTextureLoc;
//GLuint useBumpMappingLoc;
GLuint useMaskLoc;
GLuint DiffuseLoc;
GLuint KsLoc;

//Num Mesh
int num_mesh;

//Light Loc

GLuint globalAmbLoc;
GLuint ambLoc;
GLuint diffLoc;
GLuint specLoc;
GLuint posLoc;

//Light Properties
float a = 0.2f;
float globalAmbient[4] = { a, a, a, 1.0f };
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };


void init_light(GLuint program, vec3 light_position)
{
	float light_pos[3];
	light_pos[0] = light_position.x;
	light_pos[1] = light_position.y;
	light_pos[2] = light_position.z;

	globalAmbLoc = glGetUniformLocation(program, "globalAmbient");

	ambLoc = glGetUniformLocation(program, "light.ambient");
	diffLoc = glGetUniformLocation(program, "light.diffuse");
	specLoc = glGetUniformLocation(program, "light.specular");
	posLoc = glGetUniformLocation(program, "light.position");

	glProgramUniform4fv(program, globalAmbLoc, 1, globalAmbient);
	glProgramUniform4fv(program, ambLoc, 1, lightAmbient);
	glProgramUniform4fv(program, diffLoc, 1, lightDiffuse);
	glProgramUniform4fv(program, specLoc, 1, lightSpecular);
	glProgramUniform3fv(program, posLoc, 1, light_pos);
}

void init_data(Model& model)
{
	mvLoc = glGetUniformLocation(program, "mv_matrix");
	pLoc = glGetUniformLocation(program, "proj_matrix");
	mvpLoc = glGetUniformLocation(program, "mvp_matrix");
	nLoc = glGetUniformLocation(program, "normal_matrix");
	
	useTextureLoc = glGetUniformLocation(program, "useTexture");
	useMaskLoc = glGetUniformLocation(program, "useMask");
	//useBumpMappingLoc = glGetUniformLocation(program, "useBumpMapping");
	

	DiffuseLoc = glGetUniformLocation(program, "Kd");
	KsLoc = glGetUniformLocation(program, "Ks");

	for (int i = 0; i < model.indices.size(); ++i)
	{
		glGenVertexArrays(1, &model.indices[i].vao);
		glBindVertexArray(model.indices[i].vao);

		if (model.mats[i].useTexture)
		{
			glGenBuffers(1, &model.indices[i].vbo_vertices);
			glBindBuffer(GL_ARRAY_BUFFER, model.indices[i].vbo_vertices);
			glBufferData(GL_ARRAY_BUFFER, model.indices[i].vertices.size() * sizeof(vec3), &model.indices[i].vertices[0], GL_STATIC_DRAW);

			glGenBuffers(1, &model.indices[i].vbo_texcoords);
			glBindBuffer(GL_ARRAY_BUFFER, model.indices[i].vbo_texcoords);
			glBufferData(GL_ARRAY_BUFFER, model.indices[i].texcoords.size() * sizeof(vec2), &model.indices[i].texcoords[0], GL_STATIC_DRAW);

			glGenBuffers(1, &model.indices[i].vbo_normals);
			glBindBuffer(GL_ARRAY_BUFFER, model.indices[i].vbo_normals);
			glBufferData(GL_ARRAY_BUFFER, model.indices[i].normals.size() * sizeof(vec3), &model.indices[i].normals[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, model.indices[i].vbo_vertices);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, model.indices[i].vbo_normals);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, model.indices[i].vbo_texcoords);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

			
		}
		else
		{
			glGenBuffers(1, &model.indices[i].vbo_vertices);
			glBindBuffer(GL_ARRAY_BUFFER, model.indices[i].vbo_vertices);
			glBufferData(GL_ARRAY_BUFFER, model.indices[i].vertices.size() * sizeof(vec3), &model.indices[i].vertices[0], GL_STATIC_DRAW);

			//glGenBuffers(1, &model.indices[i].vbo_texcoords);
			//glBindBuffer(GL_ARRAY_BUFFER, model.indices[i].vbo_texcoords);
			//glBufferData(GL_ARRAY_BUFFER, model.indices[i].texcoords.size() * sizeof(vec2), &model.indices[i].texcoords[0], GL_STATIC_DRAW);

			glGenBuffers(1, &model.indices[i].vbo_normals);
			glBindBuffer(GL_ARRAY_BUFFER, model.indices[i].vbo_normals);
			glBufferData(GL_ARRAY_BUFFER, model.indices[i].normals.size() * sizeof(vec3), &model.indices[i].normals[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, model.indices[i].vbo_vertices);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			//glEnableVertexAttribArray(1);
			//glBindBuffer(GL_ARRAY_BUFFER, model.indices[i].vbo_texcoords);
			//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, model.indices[i].vbo_normals);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		}

		glGenBuffers(1, &model.indices[i].IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.indices[i].IBO);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices[i].index.size() * 4, &model.indices[i].index[0], GL_STATIC_DRAW);

		glUniform1i(useMaskLoc, model.mats[i].useMask);
	}

	//diffuseLoc = glGetUniformLocation(program, "DiffuseTexture");
	
	

	num_mesh = model.indices.size();
}

static void Draw_Model(GLFWwindow*& window, Model& model, Camera& cam)
{
	cam.Compute_Matrix(window);

	mat4 mvMat = cam.vMat * cam.mMat;
	mat4 pMat = cam.pMat;
	mat4 mvpMat = pMat * mvMat;//pMat * mvpMat;
	mat4 nMat = transpose(inverse(mvMat));
	//mat4 nMat = transpose(inverse(cam.vMat));

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, value_ptr(mvMat));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, value_ptr(pMat));
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, value_ptr(mvpMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, value_ptr(nMat));

	
	for (int i = 0; i < model.indices.size(); ++i)
	{
		
		glBindVertexArray(model.indices[i].vao);

		/*glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, model.indices[i].vbo_vertices);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, model.indices[i].vbo_texcoords);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, model.indices[i].vbo_normals);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);*/

		if (model.mats[i].useTexture)
		{
			glUniform1i(useTextureLoc, 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, model.mats[i].Texture_Kd_Id);
		}
		else
		{
			glUniform1i(useTextureLoc, 0);
			vec3 Kd = model.mats[i].Kd;
			glUniform4f(DiffuseLoc, Kd.x, Kd.y, Kd.z, 1.0f);
		}
	
		float Ns = model.mats[i].Ns;
		glUniform1f(KsLoc, Ns);

	

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.indices[i].IBO);
		glDrawElements(GL_TRIANGLES, model.indices[i].index.size(), GL_UNSIGNED_INT, 0);

		//int size = model.indices[i].ind.size();
		//glDrawArrays(GL_TRIANGLES, start, size / 2);
		//start += size / 2;
	}
	//glBindVertexArray(0);
}

void main()
{
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glewExperimental = GL_TRUE;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(width, height, "SanMiguel", NULL, NULL);

	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	Model model("E:\\Models_For_Rendering\\textures\\san-miguel.obj");

	//Model model("E:\\a_a_Final_Model_Rendering\\bathroom_one_tube\\contemporary_bathroom.obj");

	//Model model("E:\\Models\\Storm_Trooper\\helmet.obj");

	//Model model("E:\\Models\\stair_case\\final_model\\StairCase_no_Carpet2.obj");

	//Model model("E:\\Models\\Stanford_Bunny\\Bunny_Plane.obj");

	//Model model("E:\\Models\\Material_Test_Ball\\Painter\\painter_with_light.obj");

	//Model model("E:\\Models\\kitchen\\blender\\Country_Kitchen.obj");

	//Model model("E:\\Models\\salle_de_bin_bath_room\\salle_de_bin\\textures\\salle_de_bain_stainless_full_wall.obj");

	//Model model("E:\\Models\\Victorian_House\\blender\\Victorian_House.obj");

	//Model model("E:\\Models\\BedRoom\\pbrt\\pbrt_to_obj\\pbr_to_blend_floor_texture.obj");
	//Model model("E:\\Models\\blender_to_obj\\bedroom_slykdrako\\bedroom.obj");

	//Model model("E:\\Models\\stair_case\\final_model\\StairCase_no_Carpet2.obj");

	//Model model("E:\\Models\\blender_to_obj\\wooden_staircase\\StairCase_with_mtl.obj");

	//Model model("E:\\Models\\stair_case\\original\\The Wooden Staircase BS.obj");

	//Model model("E:\\Models\\coffe_maker\\coffer\\copy_coffe_maker\\final_good\\coffe_maker_modify_final_good_braun.obj");

	//Model model("E:\\Models\\coffe_maker\\coffer\\coffe_maker.obj");

	//Model model("E:\\Models\\blender_to_obj\\bathroom\\blender\\textures\\contemporary_bathroom.obj");

	//Model model("E:\\Models\\blender_to_obj\\bathroom\\blender\\textures\\contemporary_bathroom.obj");

	//Model model("E:\\Models\\blender_to_obj\\bedroom_slykdrako\\bedroom.obj");


	//Model model("E:\\Models\\blender_to_obj\\bedroom\\blender_bedroom.obj");

	//Model model("E:\\Models\\blender_to_obj\\bedroom_interior\\slykdrako_quarto01_blender.obj");

	//Model model("E:\\Models\\blender_to_obj\\wooden_staircase\\The Wooden Staircase BS.obj");

	//Model model("E:\\Models\\blender_to_obj\\victorian_house\\Victorian House Blendswap.obj");

	//Model model("E:\\Models\\blender_to_obj\\country_kitchen\\Country-Kitchen.obj");

	//Model model("E:\\Models\\Modern_Hall\\Modern_Hall_Obj\\Hall10.obj");

	//Model model("E:\\Models\\blender_to_obj\\bathroom\\blender\\textures\\contemporary_bathroom.obj");
	
	//Model model("E:\\Models\\blender_to_obj\\bedroom\\bedroom.obj");
	//Model model("E:\\Models\\Stanford_Dragon\\dragon.obj");

	//Model model("E:\\Models\\gallery\\textures\\gallery.obj");

	//Model model("E:\\Models\\blender\\coffer\\coffer.obj");

	//Model model("E:\\Models\\blender\\SpaceShip\\obj\\SpaceShip.obj");
	//Model model("E:\\Models\\blender\\modern_hall\\obj\\Modern_Hall.obj");

	
	
	//Model model("E:\\Models_For_Rendering\\textures\\san-miguel.obj");

	//Model model("E:\\Models\\RungHolt\\rungholt.obj");
	//Model model("E:\\Models\\RungHolt\\house.obj");
	//Model model("E:\\Models\\crytek_sponza\\textures\\crytek_sponza.obj");
			
	//Model model("E:\\Models\\fireplace_room\\FireRoom\\fireplace_room.obj");
	//Model model("E:\\Models\\fireplace_room\\fireplace_room.obj");
	//Model model("E:\\Models\\salle_de_bin\\textures\\salle_de_bain.obj");
	
	//Model model("E:\\Models\\Glass\\glass-of-water\\models\\Cup_of_Water_light.obj");


	//Model model("E:\\Models\\Cornell_Box\\cornell-box.obj");
	
	//Model model("E:\\Models\\dining_room\\blender\\The_Breakfast_Room.obj");



	//Model model("E:\\Models\\sibenik\\sibenik.obj");


	//Model model("E:\\Models\\dabrovic_sponza\\sponza.obj");

	//Model model("E:\\Models\\Cube\\cube.obj");

	//Model model("E:\\Models\\Living_Room\\living_room\\textures\\living_room.obj");

	//Model model("E:\\Models\\Power_Plant\\powerplant.obj");
	//Model model("E:\\Models\\SexRoom\\iscv2.obj");
	//Model model("E:\\Models\\Conference_Hall\\conference.obj");
	
	//Model model("E:\\Models\\mori_knob\\testObj.obj");
	//Model model("E:\\Models\\Mitsuba\\mitsuba.obj");//pos: -0.136471, 3.496326, 2.057827 dir: 0.018116 -0.819192 -0.573234
	//Model model("E:\\Models\\Mitsuba\\mitsuba-sphere.obj");

	//Model model("E:\\Models\\holodeck\\holodeck.obj");

	//Model model("E:\\Models\\z_blender_dom\\blender\\SpaceShip\\obj\\SpaceShip.obj");

	//Model model("E:\\Models\\z_blender_dom\\blender\\coffer\\coffer.obj");

	//Model model("E:\\Models\\z_blender_dom\\blender\\bedroom\\textures\\BedRoom.obj");

	//Model model("E:\\Models\\blender_to_obj\\bedroom_slykdrako\\bedroom.obj");

	//Model model("E:\\Models\\blender_to_obj\\bedroom_slykdrako\\textures\\quarto01-cycles_2.63.obj");

	//Model model("E:\\test_model\\contemporary_bathroom.obj");

	//Model model("E:\\z_final_model_export\\contemporary_bathroom.obj");

	//Model model("E:\\a_a_Final_Model_Rendering\\good_bath_room\\contemporary_bathroom.obj");

	//Model model("E:\\a_a_Final_Model_Rendering\\bath_room\\contemporary_bathroom.obj");

	//Model model("E:\\a_a_Final_Model_Rendering\\bathroom_one_tube\\contemporary_bathroom.obj");
	         
	Utility utils;
	//if (model.use_texture)
		program = utils.CreateProgram("vs.glsl", "fs.glsl");
	//else
	//	program = utils.CreateProgram("vs2.glsl", "fs2.glsl");

	Camera cam(width, height);

	init_data(model);

	vec3 max_vector = model.max_vector;
	vec3 min_vector = model.min_vector;

	vec3 center = (max_vector + min_vector) * 0.5f;

	vec3 light_position = center;//vec3(center.x, max_vector.y + 20.0f, center.z);

	

	glUseProgram(program);

	init_light(program, light_position);

	glClearColor(0.0, 0.0, 0.0, 1.0);

	//cout << model.fs.size() << "\n";

	//for (int i = 0; i < model.mats.size(); ++i)
		//cout << model.mats[i].Kd.x << " " << model.mats[i].Kd.y << " " << model.mats[i].Kd.z << "\n";

	model.Clear_Before_Render();

	float px, py, pz;
	float dx, dy, dz;

	while (!glfwWindowShouldClose(window))
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		//string str = "Pos: " + std::to_string(cam.p.x) + "," + std::to_string(cam.p.y) + "," + std::to_string(cam.p.z)
		//	+ " direction: " + to_string(cam.d.x) + " " + to_string(cam.d.y) + " " + to_string(cam.d.z);

		px = cam.p.x;
		py = cam.p.y;
		pz = cam.p.z;

		dx = cam.d.x;
		dy = cam.d.y;
		dz = cam.d.z;

		//glfwSetWindowTitle(window, str.c_str());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Draw_Model(window, model, cam);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ofstream ofs("coordinate2.txt");

	//ofs << "pos: " << cam.p.x << " " << cam.p.y << " " << cam.p.z << "\n";
	//ofs << "direction: " << cam.d.x << " " << cam.d.y << " " << cam.d.z << "\n";

	ofs << "pos: " << px << " " << py << " " << pz << "\n";
	ofs << "direction: " << dx << " " << dy << " " << dz << "\n";

	getchar();

	model.ClearMemory();

	for (int i = 0; i < model.mats.size(); ++i)
	{
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &model.indices[i].vao);

		glBindBuffer(model.indices[i].vbo_vertices, 0);
		glDeleteBuffers(1, &model.indices[i].vbo_vertices);

		glBindBuffer(model.indices[i].vbo_texcoords, 0);
		glDeleteBuffers(1, &model.indices[i].vbo_texcoords);

		glBindBuffer(model.indices[i].vbo_normals, 0);
		glDeleteBuffers(1, &model.indices[i].vbo_normals);
	}
	for (int i = 0; i < model.indices.size(); ++i)
	{
		glBindBuffer(model.indices[i].IBO, 0);
	}
	for (int i = 0; i < model.mats.size(); ++i)
	{
		glDeleteTextures(1, &model.mats[i].Texture_Kd_Id);
	}
	//glBindBuffer(ibo, 0);
}

/*
void main()
{
	Model model;

	model.Read_Material("E:\\a_Sang_Ray_Tracing\\Models\\bathroom\\bathroom_obj\\textures\\bathroom.mtl");
	
	for (auto& v : model.material_map)
	{
		cout << v.first << " " << v.second << "\n";
	}

	model.Read_Model("E:\\a_Sang_Ray_Tracing\\Models\\bathroom\\bathroom_obj\\textures\\bathroom.obj");
	int mtl_size = model.mats.size();

	for (int i = 0; i < mtl_size; ++i)
	{
		cout << " mtl " << i << " " << model.mats[i].name << " ";
		cout << model.indices[i].ind.size() << "\n";
	}

	getchar();
}
*/

/*
void main()
{
	//Model model("E:\\Models\\crytek_sponza\\textures\\crytek_sponza.obj");

	Model model("E:\\Models\\sibenik\\sibenik.obj");

	//cout << model.vertices.size() << "\n";

	//getchar();

}

*/
