#include <gl\glew.h>
#include <gl\freeglut.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "Utility.h"
#include "Load_Model_2.h"
#include "Controls.h"
#include <algorithm>

using namespace std;
using namespace glm;

int width = 1024;
int height = 768;

//27 36.1183 43.6869
//-27 - 0.030189 - 43.6869

//vec3 lookFrom(7.0f, 10.0f, -5.0f);
//vec3 direction(0.0f, 0.0f, -1.0f);

vec3 lookFrom(0, 40, 0);
vec3 direction(0.0f, -1.0f, 0.0f);

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

	//150 ->250 ao quan do trong tu

	//100 -> 200 chua phan mai phat sang
	//100 -> 150 " "
	//100 -> 125
	//125 -> 137 chua mai phat sang
	//125 -> 130 ao quan trong tu vut
	//130 -> 134 ao quan trong tu va vien vang phat sang

	//134
	//for (int i = 100; i < 190; ++i)
	//for(int i = 339; i < 340; ++i)
	//for (int i = 0; i < model.mats.size() - 1; ++i)

	//275 - 300 toan bo phan den tren tran

	//300 - 340

	//300 - 320 chua 2 buc tranh
	//chua 2 den giua nha va 2 buc tranh

	//321 - 330 ko thay do trong phong tam
	//331 - 340 co 2 cai bon rua ko thay

	//331 - 335 co 2 cai bon
	//335 - 337 tran
	//for (int i = 336; i <= 336; ++i)

	//300 -> 313 chua anh va 2 buc tranh treen tran

	//anh
	//311 Rectangle2106816067
	//312 Rectangle2106816068
	//313 Rectangle2106816071
	//314 Rectangle2106816072 doremon

	//2 cai den o giua wire_088199225
	//day den dai 

	//pictureGlass mat kinh tren tranh phai
	//201 - 300 2 buc anh dau giuong

	//251 - 275 loi 2 den vang giua nha

	//276 - 290 den chay doc tren tran nha

	//phan ben phai
	//Rectangle2106816069


	//Rectangle2106816073
	//phan dang sau anh ben trai

	//296 Rectangle2106816073
	//	297 Rectangle2106816074
	//	298 Object2063840844
	//	299 Object2063840845
	//	300 Object2063840846

	//for (int i = 296; i <= 300; ++i)
	//for (int i = 0; i < model.mats.size(); ++i)
	//for(int i = 0; i < 100; ++i)
	//125-130 ao quan ko thay

	//131 Obj3d66_877474_81_711 ao ko thay
	//132 Obj3d66_877474_83_437 ao ko thay
	//133 Shape004
	//134 Rectangle005
	//37 comVases_decors_04
	//	38 comVases_decors_05
	//	39 Rectangle001
	//	40 wire_000000000

	/*
	v 15494.8 3019.37 1130
	v 15494.8 3019.37 - 12206
	v 0 3019.37 - 12206
	v 0 3019.37 1130
	*/
	//344 half 172

	//for(int i = 133; i <= 134; ++i)//137
	//for(int i = 37; i < 40; ++i)//model.mats.size(); ++i)
	//for(int i = model.mats.size() - 2; i < model.mats.size(); ++i)

	//tuong ko thay
	//wire_154154229

	//white wall khong thay
	//136 - 154 co bon

	//xiadele_127_027_obj_13 bon tam
	//200-225 co phan vien tren tuong va duoi san

	//228 Rectangle016
	//229 Rectangle2133390650
	//230 Rectangle021
	//231 Rectangle024

	//Quan Trong
	//230 Rectangle021 o trang sau ke sach
	//231 Rectangle024 tuong nam sau ghe salon
	//Rectangle005 tuong chua 1 dong den
	//Object2063840510 phan ben trai tu
	//Roof tran cao nhat

	
	//dai tren tran
	////wire_214229166
	//wire_255191000
	//wire_0000 cua va 1 dong vat khac

	//2 phan nam tren den
	//Rectangle2106816065
	//Rectangle2133390649

//white wall khong thay
	//for (int i = 231; i <= 235; ++i)

/*239 Layer_02__NET_CAT
240 Object705440811
241 Object705440812
242 Object2063840508
243 Object2063840509
244 Layer_02__NET_CAT001
245 Object2063840510
246 Rectangle014
247 Rectangle018
*/

/*211 Obj3d66_676235_47_503
212 wire_199199199
213 wire_214229166
214 wire_255191000
215 wire_184228153
216 Cylinder887368
*/

	
	//phan vien tv 200 - 300
	//phan vang va do 250 - 275

	//Korp_zad
	//phan sau tv
	//wire_027177148
	//nut an tv ko thay
/*
	325 Rectangle2106816073
326 PictureGlass
327 Rectangle2106816068
328 Rectangle2106816072
329 Rectangle006
330 Rectangle005
*/
/*326 Rectangle2106816068
327 Rectangle2106816072
328 Rectangle006
329 Rectangle005 p
330 che_rem*/

	for(int i = 0; i < model.mats.size(); ++i)
	//for(int i = 262; i < 265; ++i)
	//for(int i = 241; i <= 245; ++i)
	//for(int i = 320; i < 344; ++i)
	//237 - 243 chua them 1 vat mau nau
	//for (int i = 237; i < 243; ++i)

	//237 - 240 
	//for (int i = 237; i < 240; ++i)

	//200 - 212 co 1 que ngang mau trang dang ngo
	//for(int i = 212; i < 214; ++i)
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

void add_triangle(Model& model, int n, float& dx, float& dz)
{
	int ind = model.mats.size() - 1;

	int v0 = model.indices[ind].vertices.size();
	int vn0 = model.indices[ind].normals.size();
	int start_ind = model.indices[ind].index.size() - 1;

	float base_z = -11500;

	float start_x = 2910.0f;
	float start_z = -10594.0f;

	//v 2910.0 2800.002 - 11500
	//v 2910.0 2800.002 - 10594
	//v 2912.0 2800.002 - 10594
	//v 2912.0 2800.002 - 11500

	//f 4920888 / 0 / 4431241 4920890 / 0 / 4431241 4920889 / 0 / 4431241
	//f 4920888 / 0 / 4431241 4920891 / 0 / 4431241 4920890 / 0 / 4431241

	for (int i = 0; i < n; ++i)
	{
		model.indices[ind].vertices.push_back(vec3(start_x, 2800.002, base_z));
		model.indices[ind].vertices.push_back(vec3(start_x, 2800.002, start_z + dz));
		model.indices[ind].vertices.push_back(vec3(start_x + dx, 2800.002, start_z + dz));
		model.indices[ind].vertices.push_back(vec3(start_x + dx, 2800.002, base_z));

		model.indices[ind].normals.push_back(vec3(0, -1, 0));
		model.indices[ind].normals.push_back(vec3(0, -1, 0));
		model.indices[ind].normals.push_back(vec3(0, -1, 0));
		model.indices[ind].normals.push_back(vec3(0, -1, 0));

		model.indices[ind].index.push_back(start_ind);
		model.indices[ind].index.push_back(start_ind + 2);
		model.indices[ind].index.push_back(start_ind + 1);

		model.indices[ind].index.push_back(start_ind);
		model.indices[ind].index.push_back(start_ind + 3);
		model.indices[ind].index.push_back(start_ind + 2);

		start_x += dx;
		start_z += dz;
		start_ind += 4;
	}
}

void main()
{
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glewExperimental = GL_TRUE;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(width, height, "BedRoom", NULL, NULL);

	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
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
	
	//CRYTEK_SPONZA
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
	
	//Model model("E:\\a_a_Sang_Ray_Tracing\\Advance_Features\\669.Volume_Rendering\\Volume_Rendering_Original\\cornell-box.obj");

	//Model model("E:\\Models\\ClassRoom\\classroom\\classroom.obj");


	//Model model("E:\\a_a_Sang_Ray_Tracing\\Advance_Features\\69.Volume_Rendering\\Volume_Metric_Path_Tracing\\cornell_box.obj");

	//Model model("E:\\Models\\VolumeTric_Lighting_Room\\Volumetric_light_Monkey.obj");
	
	//Model model("E:\\Models\\Cornell_Box\\cornell-box_Final.obj");

	//Model model("D:\\AutoDesk\\Model\\9701_Download Free 3D Interior Bedroom Model By Lee Nguyen\\maps\\BedRoom.obj");
	
	//Model model("E:\\Models\\Amazon_Bistro\\exterior.obj");

	//Model model("E:\\1_a_a_Sang_Ray_Tracing\\Advance_Features\\100.Final_Scene\\Ajax.obj");
	//Model model("E:\\1_a_a_Sang_Ray_Tracing\\Advance_Features\\100.Final_Scene\\Ajax\\ajax.obj");
	//float inc_x = 0.5f;
	//float inc_z = -0.5f;

	//add_triangle(model, 1000, inc_x, inc_z);

	//3 buc tranh > 200 < cuoi
	//for (int i = 0; i < model.mats.size() - 6; ++i)
	//for(int i = 0; i < 100; ++i)
	//{
		//dot 1 -> 201 -> het
		//if (i < 100)
		//	model.mats[i].Kd = vec3(1, 0, 0);
		//else if(i >= 100 && i <= 200)
		//	model.mats[i].Kd = vec3(0, 1, 0);
		//else
		//	model.mats[i].Kd = vec3(0, 0, 1);

		//dot 2 -> 260 ->het
		//if(i >= 201 && i <= 230)
		//	model.mats[i].Kd = vec3(1, 0, 0);
		//else if(i >= 231 && i <= 260)
		//	model.mats[i].Kd = vec3(0, 1, 0);
		//else
		//	model.mats[i].Kd = vec3(0, 0, 1);	

		//281 -> 300 truc sau cua anh ben trai va mat sau cua anh phai 
		//301->het la mat truoc cua anh phai
		//if(i >= 261 && i <= 280)
		//	model.mats[i].Kd = vec3(1, 0, 0);
		//else if(i >= 281 && i <= 300)
		//	model.mats[i].Kd = vec3(1, 1, 0);
		//else
		//	model.mats[i].Kd = vec3(0, 0, 1);	

		//291 - 300 truoc sau cua anh trai va mat sau anh phai
		//if(i >= 282 && i <= 290)
		//	model.mats[i].Kd = vec3(1, 0, 0);
		//else if(i >= 291 && i <= 300)
		//	model.mats[i].Kd = vec3(1, 1, 0);
		//else
		//	model.mats[i].Kd = vec3(0, 0, 1);	

		//291 - 295 mat truoc ben trai
		//296 - 300 mat sau 2 ben

		//if(i >= 291 && i <= 295)
		//	model.mats[i].Kd = vec3(1, 0, 0);
		//else if(i >= 296 && i <= 300)
		//	model.mats[i].Kd = vec3(1, 1, 0);
		//else
		//	model.mats[i].Kd = vec3(0, 0, 1);	

		//mat truoc
		//if (i >= 291 && i <= 292)
		//	model.mats[i].Kd = vec3(1, 0, 0);
		//else if(i >= 293 && i <= 294)
		//	model.mats[i].Kd = vec3(1, 1, 0);
		//else
		//	model.mats[i].Kd = vec3(0, 0, 1);	


		//298 - 303
		//if (i >= 281 && i <= 287)
		//	model.mats[i].Kd = vec3(1, 0, 0);
		//else if (i >= 298 && i <= 293)
		//	model.mats[i].Kd = vec3(0, 1, 0);
		//else if (i >= 294 && i <= 303)
		//	model.mats[i].Kd = vec3(0, 0, 1);

		//mat truoc va sau tranh ben phai
		//Object2063840846
		//Object2063840847

		//mat sau tranh ben trai
		//Object2063840848
		//Cylinder4766
		/*if (i >= 298 && i <= 299)
			model.mats[i].Kd = vec3(1, 0, 0);
		else if (i >= 300 && i <= 301)//truoc va sau cua cai ben phai
		{
			cout << model.mats[i].name << "\n";
			model.mats[i].Kd = vec3(0, 1, 0);
		}
		else if (i >= 302 && i <= 303)//sau lung cua cai ben trai
		{
			model.mats[i].Kd = vec3(0, 0, 1);
			cout << model.mats[i].name << "\n";
		}*/
	//}

	//for (int i = 0; i < model.mats.size(); ++i)
	//{
	//	if (i & 1)
	//		model.mats[i].Kd = vec3(1, 0, 0);
	//	else
	//		model.mats[i].Kd = vec3(0, 1, 0);
	//}

	//Model model("BedRoom.obj");
	
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

	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	while (!glfwWindowShouldClose(window))
	{
		//glEnable(GL_CULL_FACE);
		//glFrontFace(GL_CCW);
		

		string str = "Pos: " + std::to_string(cam.p.x) + "," + std::to_string(cam.p.y) + "," + std::to_string(cam.p.z)
			+ " direction: " + to_string(cam.d.x) + " " + to_string(cam.d.y) + " " + to_string(cam.d.z);

		px = cam.p.x;
		py = cam.p.y;
		pz = cam.p.z;

		dx = cam.d.x;
		dy = cam.d.y;
		dz = cam.d.z;

		glfwSetWindowTitle(window, str.c_str());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Draw_Model(window, model, cam);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//ofstream ofs("coordinate2.txt");

	//ofs << "pos: " << cam.p.x << " " << cam.p.y << " " << cam.p.z << "\n";
	//ofs << "direction: " << cam.d.x << " " << cam.d.y << " " << cam.d.z << "\n";

	//ofs << "pos: " << px << " " << py << " " << pz << "\n";
	//ofs << "direction: " << dx << " " << dy << " " << dz << "\n";

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


/*
void main()
{
	//Model model("E:\\Models\\crytek_sponza\\textures\\crytek_sponza.obj");

	Model model("E:\\Models\\sibenik\\sibenik.obj");

	//cout << model.vertices.size() << "\n";

	//getchar();

}

*/