#ifndef _MODEL_H_
#define _MODEL_H_
#include <gl\glew.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <glm\glm.hpp>
#include <unordered_map>
#include <string>
#include <SOIL2\SOIL2.h>
#include <sstream>

#define max(x, y) x > y ? x : y
#define min(x, y) x < y ? x : y

using namespace std;
using namespace glm;

static void fixIndex(int& v, const int& n)
{
	v = v < 0 ? v + n : v - 1;
}

static void fixIndex(int v[3], const int& n)
{
	v[0] = v[0] < 0 ? v[0] + n : v[0] - 1;
	v[1] = v[1] < 0 ? v[1] + n : v[1] - 1;
	v[2] = v[2] < 0 ? v[2] + n : v[2] - 1;
}

static void fixIndex2(int v[2], const int& n)
{
	v[0] = v[0] < 0 ? v[0] + n : v[0] - 1;
	v[1] = v[1] < 0 ? v[1] + n : v[1] - 1;
}

static void fixIndex(vec3& v, const int& n)
{
	v.x = v.x < 0 ? v.x + n : v.x - 1;
	v.y = v.y < 0 ? v.y + n : v.y - 1;
	v.z = v.z < 0 ? v.z + n : v.z - 1;
}

static void fixIndex(vec2& v, const int& n)
{
	v.x = v.x < 0 ? v.x + n : v.x - 1;
	v.y = v.y < 0 ? v.y + n : v.y - 1;
}

static int fixIndex_return(int& v, const int& n)
{
	int a = v;
	a = a < 0 ? a + n : a - 1;
	return a;
}

static void SkipSpace(char *&t)
{
	t += strspn(t, " \t");
}

void getdirection(const string& filepath, string& direction)
{
	size_t found = filepath.find_last_of('/\\');
	direction = filepath.substr(0, found + 1);
}

void getfilename(const string& filepath, string& filename)
{

	size_t found = filepath.find_last_of('/\\');
	filename = filepath.substr(found + 1);
}

void getdDirectionAndName(const string& filepath, string& direction, string& filename)
{
	size_t found = filepath.find_last_of('/\\');
	direction = filepath.substr(0, found + 1);
	filename = filepath.substr(found + 1);
}

static vec3 max_vec(const vec3& v1, const vec3& v2)
{
	return vec3(max(v1.x, v2.x), max(v1.y, v2.y), max(v1.z, v2.z));
}

static vec3 min_vec(const vec3& v1, const vec3& v2)
{
	return vec3(min(v1.x, v2.x), min(v1.y, v2.y), min(v1.z, v2.z));
}


struct Material
{
	GLuint Texture_Kd_Id = 0;
	GLuint Texture_Mask_Id = 0;
	bool useTexture = false;
	bool useMask = 0;
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	vec3 Ke;
	string name;
	float Ns;
};


GLuint LoadTexture(const string& filepath)
{
	GLuint texture_index = SOIL_load_OGL_texture(filepath.c_str(), 4, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	//cout << "tex ind: " << texture_index << "\n";
	if (texture_index == 0)
		cout << "Load Texture " << filepath << " Fail\n";

	glBindTexture(GL_TEXTURE_2D, texture_index);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	if (glewIsSupported("GL_EXT_texture_filter_anisotropic"))
	{
		GLfloat anisoset = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &anisoset);
		glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY, anisoset);
	}
	return texture_index;
}


enum face
{
	Single_Line, Double_Line
};

struct Triangle_index
{
	int v[3] = { -1, -1, -1 };
	int vt[3] = { -1, -1, -1 };
	int vn[3] = { -1, -1, -1 };
	int face_type;
	Triangle_index() {}
	Triangle_index(int v0, int vt0, int vn0, int v1, int vt1, int vn1, int v2, int vt2, int vn2)
	{
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;

		vt[0] = vt0;
		vt[1] = vt1;
		vt[2] = vt2;

		vn[0] = vn0;
		vn[1] = vn1;
		vn[2] = vn2;
	}
};

struct Mesh
{
	vector<Triangle_index> trs;
	int mtl;
	unsigned int IBO;
	Mesh() {}
};
/*
static void get_face_index(char*& t, const int& vs, const int& vts, const int& vns, vector<Triangle_index>& trs, int& face_type)
{
string s = t;
int length = s.find_last_of("0123456789");

//s = s.substr(0, length);
//cout << length << "\n";

int sign = 1;
int count = 0;
vector<int> index;
for (int i = 0; i <= length + 1; ++i)
{
if (s[i] == '-')
{
sign = -1;
}
else if (isdigit(s[i]))
{
count = 10 * count + s[i] - '0';
}
else if (s[i] == '/')
{
face_type = Single_Line;
index.emplace_back(sign * count);
sign = 1;
count = 0;

if (s[i + 1] == '/')
{
face_type = Double_Line;
++i;
}
}
else if (s[i] == ' ')
{
index.emplace_back(sign * count);
sign = 1;
count = 0;
face_type = Single_Line;
}
else if (i == length + 1)
{
index.emplace_back(sign * count);
sign = 1;
break;
}
}


int size = index.size();

if (size == 9)
{
for (int i = 0; i < 9; i += 3)
{
fixIndex(index[i], vs);
fixIndex(index[i + 1], vts);
fixIndex(index[i + 2], vns);
}
Triangle_index tr(index[0], index[1], index[2], index[3], index[4], index[5], index[6], index[7], index[8]);
tr.face_type = Single_Line;
trs.emplace_back(tr);
return;
}
else if (size == 12)
{
if (face_type == Single_Line)
{
for (int i = 0; i < 12; i += 3)
{
fixIndex(index[i], vs);
fixIndex(index[i + 1], vts);
fixIndex(index[i + 2], vns);

//cout << index[i] << "\n";
}

Triangle_index tr1(index[0], index[1], index[2], index[3], index[4], index[5], index[6], index[7], index[8]);
Triangle_index tr2(index[0], index[1], index[2], index[6], index[7], index[8], index[9], index[10], index[11]);

tr1.face_type = Single_Line;
tr2.face_type = Single_Line;

trs.emplace_back(tr1);
trs.emplace_back(tr2);
return;
}
if (face_type == Double_Line)
{
for (int i = 0; i < 12; i += 2)
{
fixIndex(index[i], vs);
fixIndex(index[i + 1], vns);
}

Triangle_index tr1(index[0], -1, index[1], index[2], -1, index[3], index[4], -1, index[5]);
Triangle_index tr2(index[0], -1, index[1], index[4], -1, index[5], index[6], -1, index[7]);
Triangle_index tr3(index[0], -1, index[1], index[6], -1, index[7], index[8], -1, index[9]);
Triangle_index tr4(index[0], -1, index[1], index[8], -1, index[9], index[10], -1, index[11]);

tr1.face_type = Double_Line;
tr2.face_type = Double_Line;
tr3.face_type = Double_Line;
tr4.face_type = Double_Line;

trs.emplace_back(tr1);
trs.emplace_back(tr2);
trs.emplace_back(tr3);
trs.emplace_back(tr4);
return;
}
}
else if (size == 15)
{
for (int i = 0; i < 15; i += 3)
{
fixIndex(index[i], vs);
fixIndex(index[i + 1], vts);
fixIndex(index[i + 2], vns);

//cout << index[i] << "\n";
}

Triangle_index tr1(index[0], index[1], index[2], index[3], index[4], index[5], index[6], index[7], index[8]);
Triangle_index tr2(index[0], index[1], index[2], index[6], index[7], index[8], index[9], index[10], index[11]);
Triangle_index tr3(index[0], index[1], index[2], index[9], index[10], index[11], index[12], index[13], index[14]);

tr1.face_type = Single_Line;
tr2.face_type = Single_Line;
tr3.face_type = Single_Line;

trs.emplace_back(tr1);
trs.emplace_back(tr2);
trs.emplace_back(tr3);
return;
}
//assum single line
else if (size == 18)
{
for (int i = 0; i < 18; i += 3)
{
fixIndex(index[i], vs);
fixIndex(index[i + 1], vts);
fixIndex(index[i + 2], vns);

//cout << index[i] << "\n";
}

Triangle_index tr1(index[0], index[1], index[2], index[3], index[4], index[5], index[6], index[7], index[8]);
Triangle_index tr2(index[0], index[1], index[2], index[6], index[7], index[8], index[9], index[10], index[11]);
Triangle_index tr3(index[0], index[1], index[2], index[9], index[10], index[11], index[12], index[13], index[14]);
Triangle_index tr4(index[0], index[1], index[2], index[12], index[13], index[14], index[15], index[16], index[17]);

tr1.face_type = Single_Line;
tr2.face_type = Single_Line;
tr3.face_type = Single_Line;
tr4.face_type = Single_Line;

trs.emplace_back(tr1);
trs.emplace_back(tr2);
trs.emplace_back(tr3);
trs.emplace_back(tr4);
return;
}
else if (size == 6)
{

if (face_type == Single_Line)
{
for (int i = 0; i < 6; i += 2)
{
fixIndex(index[i], vs);
fixIndex(index[i + 1], vts);
}
Triangle_index tr(index[0], index[1], -1, index[2], index[3], -1, index[4], index[5], -1);
tr.face_type = Single_Line;
trs.emplace_back(tr);
return;
}
else if (face_type == Double_Line)
{
for (int i = 0; i < 6; i += 2)
{
fixIndex(index[i], vs);
fixIndex(index[i + 1], vns);
}
Triangle_index tr(index[0], -1, index[1], index[2], -1, index[3], index[4], -1, index[5]);
tr.face_type = Double_Line;
trs.emplace_back(tr);
return;
}
}
else if (size == 8)
{
if (face_type == Single_Line)
{
for (int i = 0; i < 8; i += 2)
{
fixIndex(index[i], vs);
fixIndex(index[i + 1], vts);
}

Triangle_index tr1(index[0], index[1], -1, index[2], index[3], -1, index[4], index[5], -1);
Triangle_index tr2(index[0], index[1], -1, index[4], index[5], -1, index[6], index[7], -1);

tr1.face_type = Single_Line;
tr2.face_type = Single_Line;
trs.emplace_back(tr1);
trs.emplace_back(tr2);
return;
}
else if(face_type == Double_Line)
{
for (int i = 0; i < 8; i += 2)
{
fixIndex(index[i], vs);
fixIndex(index[i + 1], vns);
}

Triangle_index tr1(index[0], -1, index[1], index[2], -1, index[3], index[4], -1, index[5]);
Triangle_index tr2(index[0], -1, index[1], index[4], -1, index[5], index[6], -1, index[7]);
tr1.face_type = Double_Line;
tr2.face_type = Double_Line;

trs.emplace_back(tr1);
trs.emplace_back(tr2);
return;
}
}
else if (size == 10)
{
if (face_type == Single_Line)
{
for (int i = 0; i < 10; i += 2)
{
fixIndex(index[i], vs);
fixIndex(index[i + 1], vts);
}

Triangle_index tr1(index[0], index[1], -1, index[2], index[3], -1, index[4], index[5], -1);
Triangle_index tr2(index[0], index[1], -1, index[4], index[5], -1, index[6], index[7], -1);
Triangle_index tr3(index[0], index[1], -1, index[6], index[7], -1, index[8], index[9], -1);

tr1.face_type = Single_Line;
tr2.face_type = Single_Line;
tr3.face_type = Single_Line;

trs.emplace_back(tr1);
trs.emplace_back(tr2);
trs.emplace_back(tr3);
return;
}
else if(face_type == Double_Line)
{
for (int i = 0; i < 10; i += 2)
{
fixIndex(index[i], vs);
fixIndex(index[i + 1], vns);
}

Triangle_index tr1(index[0], -1, index[1], index[2], -1, index[3], index[4], -1, index[5]);
Triangle_index tr2(index[0], -1, index[1], index[4], -1, index[5], index[6], -1, index[7]);
Triangle_index tr3(index[0], -1, index[1], index[6], -1, index[7], index[8], -1, index[9]);

tr1.face_type = Double_Line;
tr2.face_type = Double_Line;
tr3.face_type = Double_Line;

trs.emplace_back(tr1);
trs.emplace_back(tr2);
trs.emplace_back(tr3);
return;
}
}
else if (size == 14)
{
if (face_type == Single_Line)
{
for (int i = 0; i < 14; i += 2)
{
fixIndex(index[i], vs);
fixIndex(index[i + 1], vts);
}

Triangle_index tr1(index[0], index[1], -1, index[2], index[3], -1, index[4], index[5], -1);
Triangle_index tr2(index[0], index[1], -1, index[4], index[5], -1, index[6], index[7], -1);
Triangle_index tr3(index[0], index[1], -1, index[6], index[7], -1, index[8], index[9], -1);
Triangle_index tr4(index[0], index[1], -1, index[8], index[9], -1, index[10], index[11], -1);

tr1.face_type = Single_Line;
tr2.face_type = Single_Line;
tr3.face_type = Single_Line;

trs.emplace_back(tr1);
trs.emplace_back(tr2);
trs.emplace_back(tr3);
return;
}
else if (face_type == Double_Line)
{
for (int i = 0; i < 10; i += 2)
{
fixIndex(index[i], vs);
fixIndex(index[i + 1], vns);
}

Triangle_index tr1(index[0], -1, index[1], index[2], -1, index[3], index[4], -1, index[5]);
Triangle_index tr2(index[0], -1, index[1], index[4], -1, index[5], index[6], -1, index[7]);
Triangle_index tr3(index[0], -1, index[1], index[6], -1, index[7], index[8], -1, index[9]);

tr1.face_type = Double_Line;
tr2.face_type = Double_Line;
tr3.face_type = Double_Line;

trs.emplace_back(tr1);
trs.emplace_back(tr2);
trs.emplace_back(tr3);
return;
}
}
}
*/


static void get_face_index(char*& t, const int& vs, const int& vts, const int& vns, vector<Triangle_index>& trs, int& face_type)
{
	string s = t;
	int length = s.find_last_of("0123456789");
	s = s.substr(0, length + 1);

	int sign = 1;
	int count = 0;
	vector<int> index;
	//vector<int> indices;
	face_type = Single_Line;

	int num_data_per_vertex = 0;
	bool found_num_data_per_vertex = false;

	for (int i = 0; i <= length + 1; ++i)
	{
		if (s[i] == '-')
		{
			sign = -1;
		}
		else if (isdigit(s[i]))
		{
			count = 10 * count + s[i] - '0';
		}
		else if (s[i] == '/')
		{
			if (!found_num_data_per_vertex)
			{
				++num_data_per_vertex;
			}
			face_type = Single_Line;
			index.emplace_back(sign * count);
			sign = 1;
			count = 0;

			if (s[i + 1] == '/')
			{
				face_type = Double_Line;
				++i;
			}
		}
		else if (s[i] == ' ')
		{

			index.emplace_back(sign * count);
			sign = 1;
			count = 0;
			if (!found_num_data_per_vertex)
			{
				++num_data_per_vertex;
				found_num_data_per_vertex = true;
			}
		}
		else if (i == length + 1)
		{
			index.emplace_back(sign * count);
			sign = 1;
			break;
		}
	}

	int size = index.size();

	if (num_data_per_vertex == 3) // v/vt/vn case   12 18 24 30 
	{
		//cout << "line 3\n";
		for (int i = 0; i < size; i += 3)
		{
			fixIndex(index[i], vs);
			fixIndex(index[i + 1], vts);
			fixIndex(index[i + 2], vns);
		}

		int start_v = 0;
		int start_vt = 1;
		int start_vn = 2;

		int num_Triangle = size / 3 - 2;

		for (int i = 0; i < num_Triangle; ++i)
		{
			Triangle_index tr(index[start_v], index[start_vt], index[start_vn], index[3 * i + 3], index[3 * i + 4], index[3 * i + 5], index[3 * i + 6], index[3 * i + 7], index[3 * i + 8]);
			tr.face_type = Single_Line;
			trs.emplace_back(tr);
			//faces.emplace_back(f);
		}
	}
	else if (num_data_per_vertex == 2)  //  v/vt or v//vn
	{
		//cout << "line 2\n";
		if (face_type == Single_Line) // v / vt
		{
			//cout << "single\n";
			for (int i = 0; i < size; i += 2)
			{
				fixIndex(index[i], vs);
				fixIndex(index[i + 1], vts);
			}

			int num_Triangle = size / 2 - 2;

			int start_v = 0;
			int start_vt = 1;

			for (int i = 0; i < num_Triangle; ++i)
			{
				//01 23 45    01 45 67
				Triangle_index tr(index[start_v], index[start_vt], -1, index[2 * i + 2], index[2 * i + 3], -1, index[2 * i + 4], index[2 * i + 5], -1);
				tr.face_type = Single_Line;
				trs.emplace_back(tr);
			}
		}
		else if (face_type == Double_Line)// v // vn
		{
			//cout << "double\n";
			for (int i = 0; i < size; i += 2)
			{
				fixIndex(index[i], vs);
				fixIndex(index[i + 1], vns);
			}

			int num_Triangle = size / 2 - 2;

			int start_v = 0;
			int start_vn = 1;

			for (int i = 0; i < num_Triangle; ++i)
			{
				Triangle_index tr(index[start_v], -1, index[start_vn], index[2 * i + 2], -1, index[2 * i + 3], index[2 * i + 4], -1, index[2 * i + 5]);
				tr.face_type = Double_Line;
				trs.emplace_back(tr);
			}
		}
	}
}


struct Index
{
	vector<vec3> vertices;
	vector<vec2> texcoords;
	vector<vec3> normals;
	vector<vec3> tagents;//Bump mapping
	vector<vec3> bi_tagents;//Bump mapping, havent added yet due to time constrain, will have this feature later

	vector<int> index;
	int mtl;
	//bool useTexture;
	unsigned int vao;
	unsigned int vbo_vertices;
	unsigned int vbo_texcoords;
	unsigned int vbo_normals;
	unsigned int IBO;

};

struct normal_struct
{
	vec3 sum_normal = vec3(0.0f);
	int num_normal = 0;
};

struct Model
{
	Model() {}
	Model(const string& filepath)
	{
		Read_Model(filepath);
	}



	//vector<vec3> vertices;
	//vector<vec2> texcoords;
	//vector<vec3> normals;



	vector<Index> indices;

	vector<Material> mats;

	unordered_map<string, int> material_map;

	vec3 light_pos;

	bool use_texture = true;

	string direction;

	vec3 max_vector = vec3(-1e20f, -1e20f, -1e20f);
	vec3 min_vector = vec3(1e20f, 1e20f, 1e20f);

	void Clear_Before_Render()
	{
		//vertices.swap(vector<vec3>());
		//texcoords.swap(vector<vec2>());
		//normals.swap(vector<vec3>());

		//final_vertices.swap(vector<float>());

		//indices.swap(vector<Index>());
		//material_map.clear();

		//mats.swap(vector<Material>());

	}

	void ClearMemory()
	{
		indices.swap(vector<Index>());
		mats.swap(vector<Material>());

		//vertices.swap(vector<vec3>());
		//texcoords.swap(vector<vec2>());
		//normals.swap(vector<vec3>());


		//final_vertices.swap(vector<float>());

		//indices.swap(vector<Index>());
		material_map.clear();

		//mats.swap(vector<Material>());
	}

	void Read_Material(const string& filepath)
	{
		//cout << "Read mtl sec vtion: \n";

		ifstream f(filepath);
		if (!f)
			cout << "Mtl file not exist\n";

		char line[256];
		int numberOfMaterial = 0;
		while (f.getline(line, 256))
		{
			if (line[0] == 'n' && line[1] == 'e' && line[2] == 'w')
				numberOfMaterial++;
		}

		mats.resize(numberOfMaterial);


		f.clear();
		f.seekg(0, ios::beg);


		int countMaterial = -1;
		int countTexture = -1;


		unordered_map<string, int> texture_map;

		while (f.getline(line, 256))
		{
			char* t = line;

			SkipSpace(t);

			if (strncmp(t, "newmtl", 6) == 0)
			{
				countMaterial++;

				int post_space = strspn(t + 6, " \t");

				t += post_space + 6;
				string s = t;
				int length = s.find_last_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

				s = s.substr(0, length + 1);
				string name = "";
				for (int i = 0; i <= length; ++i)
				{
					//if (isalpha(s[i]))
					name += s[i];
				}

				material_map[name] = countMaterial;
				mats[countMaterial].name = name;
			}
			else if (t[0] == 'N')
			{
				if (t[1] == 's')
					sscanf_s(t += 3, "%f", &mats[countMaterial].Ns);
			}
			else if (t[0] == 'K')
			{
				if (t[1] == 'a')
					sscanf_s(t += 2, "%f %f %f", &mats[countMaterial].Ka.x, &mats[countMaterial].Ka.y, &mats[countMaterial].Ka.z);
				else if (t[1] == 'd')
				{
					sscanf_s(t += 2, "%f %f %f", &mats[countMaterial].Kd.x, &mats[countMaterial].Kd.y, &mats[countMaterial].Kd.z);
					//if (mats[countMaterial].Kd.x == mats[countMaterial].Kd.y && mats[countMaterial].Kd.y == mats[countMaterial].Kd.z &&
					//	mats[countMaterial].Kd.x > 0 && mats[countMaterial].Kd.x < 1)
					//{
					//	cout << mats[countMaterial].name << " " << mats[countMaterial].Kd.x << " " << mats[countMaterial].Kd.y << " " << mats[countMaterial].Kd.z << "\n";

					//}


				}
				else if (t[1] == 's')
				{
					sscanf_s(t += 2, "%f %f %f", &mats[countMaterial].Ks.x, &mats[countMaterial].Ks.y, &mats[countMaterial].Ks.z);
				}
				else if (t[1] == 'e')
				{
					sscanf_s(t += 2, "%f %f %f", &mats[countMaterial].Ke.x, &mats[countMaterial].Ke.y, &mats[countMaterial].Ke.z);
				}
			}
			else if (strncmp(t, "map_Kd", 6) == 0)
			{
				//cout << "use Texture\n";
				mats[countMaterial].useTexture = true;
				char tex_name[256];
				sscanf_s(t += 6, "%s", tex_name);

				string file_path = tex_name;

				string realname;
				//getfilename(name, realname);

				string tex_path;
				//string 
				getdDirectionAndName(file_path, tex_path, realname);

				string path = direction + realname;
				//cout << path << "\n";
				if (texture_map.find(realname) == texture_map.end())
				{
					GLuint TextureId = LoadTexture(path);
					texture_map[realname] = TextureId;
					mats[countMaterial].Texture_Kd_Id = TextureId;
				}
				else
				{
					GLuint TextureId = texture_map[realname];
					mats[countMaterial].Texture_Kd_Id = texture_map[realname];
				}
			}
			else if (strncmp(t, "map_d", 5) == 0)
			{
				mats[countMaterial].useMask = 1;
				char tex_name[256];
				sscanf_s(t += 6, "%s", tex_name);

				string file_path = tex_name;

				string realname;
				//getfilename(name, realname);

				string tex_path;
				
				getdDirectionAndName(file_path, tex_path, realname);

				string path = direction + tex_path + realname;

				if (texture_map.find(realname) == texture_map.end())
				{
					GLuint TextureId = LoadTexture(path);
					texture_map[realname] = TextureId;
					mats[countMaterial].Texture_Mask_Id = TextureId;
				}
				else
				{
					GLuint TextureId = texture_map[realname];
					mats[countMaterial].Texture_Mask_Id = texture_map[realname];
				}
			}
		}
		//cout << "End Read mtl section: \n";
	}



	void Read_Model(const string& filepath)
	{
		vector<Mesh> meshes;

		getdirection(filepath, direction);
		//cout << direction << "\n";
		ifstream file(filepath);

		if (!file)
			cout << "Obj file not exist\n";
		//string line;

		vector<vec3> v;
		vector<vec2> vt;
		//vector<vec3> vn;

		int num_v = 0;
		int num_vt = 0;
		int num_vn = 0;

		//int read_length = 512;
		char line[1024];

		int line_ind = 0;
		bool find_mtl = false;
		while (file.getline(line, 1024) && line_ind < 1000)
		{
			char* t = line;
			int prev_space = strspn(t, " \t");
			t += prev_space;

			if (strncmp(t, "m", 1) == 0)
			{
				find_mtl = true;
				break;
			}
			++line_ind;
		}

		if (!find_mtl)
		{
			mats.resize(1);
			mats[0].name = "default mtl";
			mats[0].Kd = vec3(0.5f);
			mats[0].useTexture = false;
			meshes.resize(1);
		}

		file.clear();
		file.seekg(0, ios::beg);

		vec3 max_vector(-1e20);
		vec3 min_vector(1e20);

		int count_mtllib = 0;
		int face_type;
		int current_mesh = 0;
		int count_vn = 0;

		while (file.getline(line, 1024))
		{
			char* t = line;
			int prev_space = strspn(t, " \t");
			t += prev_space;
			if (strncmp(t, "v", 1) == 0)
			{
				float x, y, z;
				t += 1;
				if (strncmp(t, " ", 1) == 0)
				{
					//float x2, y2, z2;
					int post_space = strspn(t, " \t");
					t += post_space;
					//sscanf_s(t += post_space, "%f %f %f %f %f %f", &x, &y, &z, &x2, &y2, &z2);		
					sscanf_s(t, "%f %f %f", &x, &y, &z);

					vec3 temp(x, y, z);

					v.emplace_back(temp);

					max_vector = max_vec(max_vector, temp);
					min_vector = min_vec(min_vector, temp);

					//if (x2 != 0.0f && y2 != 0.0f &&z2 != 0.0f)
					//	v.emplace_back(vec3(x2, y2, z2));
					++num_v;
				}
				else if (strncmp(t, "t", 1) == 0)
				{
					t += 1;
					int post_space = strspn(t, " \t");
					sscanf_s(t += post_space, "%f %f", &x, &y);
					vt.emplace_back(vec2(x, y));
					++num_vt;
				}
				else if (strncmp(t, "n", 1) == 0)
				{
					++num_vn;
				}
			}
			else if (strncmp(t, "f", 1) == 0)
			{
				int post_space = strspn(t + 1, " \t");
				t += post_space + 1;

				vector<Triangle_index> trs;
				int face_type;
				get_face_index(t, num_v, num_vt, num_vn, trs, face_type);
				//cout << trs.size() << "\n";
				for (int i = 0; i < trs.size(); ++i)
				{
					meshes[current_mesh].trs.emplace_back(trs[i]);
				}
			}
			else if (strncmp(t, "usemtl", 6) == 0)
			{
				int post_space = strspn(t + 6, " \t");

				t += post_space + 6;
				string s = t;
				int length = s.find_last_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

				s = s.substr(0, length + 1);
				string name = "";
				for (int i = 0; i <= length; ++i)
				{
					//if (isalpha(s[i]))
					name += s[i];
				}

				if (material_map.find(name) == material_map.end())
					cout << "\n" << name << " not exist\n";

				current_mesh = material_map[name];
				//cout << name << " " << current_mesh << "\n";
				int mtlId = material_map[name];
				meshes[current_mesh].mtl = mtlId;
			}
			else if (line[0] == 'm' && count_mtllib == 0)
			{
				char* t = line;
				char mat_name[256];

				sscanf_s(t += 6, "%s", mat_name);
				string name = mat_name;
				//cout <<"lib name :"<<name << "\n";
				string mat_lib = direction + name;
				//cout << mat_lib << "\n";
				Read_Material(mat_lib);
				cout << "mat size: " << mats.size() << "\n";

				if (mats.size() == 0)
				{
					mats.resize(1);
					mats[0].name = "default mtl";
					mats[0].Kd = vec3(0.5f);
					mats[0].useTexture = false;
					meshes.resize(1);
				}
				else
					meshes.resize(mats.size());
				count_mtllib = 1;
			}
		}

		cout << max_vector.x << " " << max_vector.y << " " << max_vector.z << "\n";
		cout << min_vector.x << " " << min_vector.y << " " << min_vector.z << "\n";



		light_pos = (min_vector + max_vector) * 0.5f + (max_vector - min_vector) * 0.25f;

		file.clear();
		file.seekg(0, ios::beg);
		cout << "v:  " << v.size() << "\n";
		cout << "vt: " << vt.size() << "\n";
		cout << "vn: " << num_vn << "\n";

		int count = 0;

		/*
		int count_mtllib = 0;
		int face_type;
		int current_mesh = 0;
		//char line[256];
		while (file.getline(line, 1024))
		{
		char* t = line;
		int prev_space = strspn(t, " \t");
		t += prev_space;
		if (strncmp(t, "f", 1) == 0)
		{
		int post_space = strspn(t + 1, " \t");
		t += post_space + 1;

		vector<Triangle_index> trs;
		int face_type;
		get_face_index(t, num_v, num_vt, num_vn, trs, face_type);
		//cout << trs.size() << "\n";
		for (int i = 0; i < trs.size(); ++i)
		{
		meshes[current_mesh].trs.emplace_back(trs[i]);
		}
		}
		else if (strncmp(t, "usemtl", 6) == 0)
		{
		int post_space = strspn(t + 6, " \t");

		t += post_space + 6;
		string s = t;
		int length = s.find_last_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

		s = s.substr(0, length + 1);
		string name = "";
		for (int i = 0; i <= length; ++i)
		{
		//if (isalpha(s[i]))
		name += s[i];
		}

		if (material_map.find(name) == material_map.end())
		cout << "\n" << name << " not exist\n";

		current_mesh = material_map[name];
		//cout << name << " " << current_mesh << "\n";
		int mtlId = material_map[name];
		meshes[current_mesh].mtl = mtlId;
		}
		else if (line[0] == 'm' && count_mtllib == 0)
		{
		char* t = line;
		char mat_name[256];

		sscanf_s(t += 6, "%s", mat_name);
		string name = mat_name;
		//cout <<"lib name :"<<name << "\n";
		string mat_lib = direction + name;
		//cout << mat_lib << "\n";
		Read_Material(mat_lib);
		cout << "mat size: " << mats.size() << "\n";

		if (mats.size() == 0)
		{
		mats.resize(1);
		mats[0].name = "default mtl";
		mats[0].Kd = vec3(0.5f);
		mats[0].useTexture = false;
		meshes.resize(1);
		}
		else
		meshes.resize(mats.size());
		count_mtllib = 1;
		}
		}*/

		//build vertices
		//unordered_map<string, int> vertex_map;
		int num_mesh = mats.size();

		int vsize = v.size();
		int vtsize = vt.size();
		cout << "num mesh: " << num_mesh << "\n";


		indices.resize(num_mesh);


		use_texture = num_vt > 0;
		bool use_normal = num_vn > 0;
		unordered_map<int, normal_struct> normal_map;
		//vertices.reserve(45000000);
		//normals.reserve(num_v);

		int c = 0;

		//compute normal map

		//int i = 0;
		//for (int i = 0; i < num_mesh; ++i)

		//int start = 31;
		//int end = 33;
		//for (int i = 0; i < num_mesh - 1; ++i)
		//for(int i = start; i <= end; ++i)

		//300 - 320 2 buc tranh

		//vec3 v_max(INT_MIN, INT_MIN, INT_MIN);
		//vec3 v_min(INT_MAX, INT_MAX, INT_MAX);

		for (int i = 0; i < mats.size(); ++i)
		{

			int num_index = meshes[i].trs.size();

			for (int j = 0; j < num_index; ++j)
			{
				//compute normal
				int ind_v0 = meshes[i].trs[j].v[0];
				int ind_v1 = meshes[i].trs[j].v[1];
				int ind_v2 = meshes[i].trs[j].v[2];

				vec3 v0(v[ind_v0]);
				vec3 v1(v[ind_v1]);
				vec3 v2(v[ind_v2]);

				/*if (mats[i].name == "Rectangle006")
				{
				v_max = max_vec(v0, v_max);
				v_max = max_vec(v1, v_max);
				v_max = max_vec(v2, v_max);

				v_min = min_vec(v0, v_min);
				v_min = min_vec(v1, v_min);
				v_min = min_vec(v2, v_min);
				}*/

				vec3 normal_v0 = normalize(cross(v1 - v0, v2 - v0));
				vec3 normal_v1 = normalize(cross(v2 - v1, v0 - v1));
				vec3 normal_v2 = normalize(cross(v0 - v2, v1 - v2));

				normal_map[ind_v0].sum_normal += normal_v0;
				normal_map[ind_v0].num_normal++;

				normal_map[ind_v1].sum_normal += normal_v1;
				normal_map[ind_v1].num_normal++;

				normal_map[ind_v2].sum_normal += normal_v2;
				normal_map[ind_v2].num_normal++;
			}
		}
		/*
		mat size : 344
		15494.8 3592.37 1130
		0 - 1426.08 - 12206
		v : 4920917
		vt : 2455770
		vn : 4431242
		num mesh : 344
		v_max : 3275 2900 - 6665.32
		v_min : 815 2800 - 10595.9
		*/

		//cout << "v_max: " << v_max.x << " " << v_max.y << " " << v_max.z << "\n";
		//cout << "v_min: " << v_min.x << " " << v_min.y << " " << v_min.z << "\n";


		//normalize data
		for (auto& v : normal_map)
		{
			v.second.sum_normal /= v.second.num_normal;
		}


		//int global_index = 0;

		//90 -> 260 chua phan mai tren dau
		//300 - > 339 noi that bon cau

		//100 -> 125 chua vien cua va cua so sau co con mac bikini xanh 2.jpg
		//134 -> 136 chua vien vang va mai phat sang

		//for (int i = 100; i < 190; ++i)//num_mesh; ++i)
		//for(int i = start; i <= end; ++i)
		//for (int i = 339; i < 340; ++i)
		//for(int i = 0; i < mats.size(); ++i)

		//291 wire_141171233
		//292 tiet110163
		//293 Object2063840844
		//294 Rectangle2106816069
		//295 Rectangle2106816070

		//Rectangle2106816069
		//phan nen dang sau cua tranh

		/*262 Obj3d66_981776_154_168
		263 Korpus
		264 Rectangle2106816053
		265 Rectangle2106816064
		266 Korp_zad
		267 wire_088088225
		268 wire_027177148
		269 Rectangle2106816061
		*/
		//for(int i = 296; i <= 300; ++i)
		for (int i = 0; i < mats.size(); ++i)
			//for(int i = mats.size() - 2; i < mats.size(); ++i)
		{
			int num_index = meshes[i].trs.size();
			//cout << i << " " << mats[i].name << "\n";
			//cout << mats[i].name;

			//if (mats[i].name == "Rectangle006")
			//{

			//}
			int tmp_index = 0;
			if (mats[i].useTexture)
			{
				unordered_map<string, int> global_vertex_map;
				for (int j = 0; j < num_index; ++j)
				{
					for (int k = 0; k < 3; ++k)
					{
						int ind_v = meshes[i].trs[j].v[k];
						int ind_vt = meshes[i].trs[j].vt[k];

						string s = to_string(ind_v) + "|" + to_string(ind_vt);

						if (global_vertex_map.find(s) == global_vertex_map.end())
						{
							++c;
							//if (c % 100000 == 0)
							//	cout << c << "\n";

							indices[i].index.emplace_back(tmp_index);

							vec3 v0 = v[ind_v];
							vec2 vt0 = ind_vt >= 0 ? vt[ind_vt] : vec2(0.0f, 0.0f);
							vec3 vn0 = normal_map[ind_v].sum_normal;

							indices[i].vertices.emplace_back(v0);
							indices[i].texcoords.emplace_back(vt0);
							indices[i].normals.emplace_back(vn0);

							global_vertex_map[s] = tmp_index;

							++tmp_index;

						}
						else
						{
							unsigned int current_ind = global_vertex_map[s];
							indices[i].index.emplace_back(current_ind);
						}
					}
				}
				global_vertex_map.clear();
			}
			else
			{
				unordered_map<int, int> global_vertex_map;
				for (int j = 0; j < num_index; ++j)
				{
					for (int k = 0; k < 3; ++k)
					{
						int ind_v = meshes[i].trs[j].v[k];

						if (global_vertex_map.find(ind_v) == global_vertex_map.end())
						{
							++c;
							//if (c % 100000 == 0)
							//	cout << c << "\n";

							indices[i].index.emplace_back(tmp_index);

							vec3 v0 = v[ind_v];
							vec3 vn0 = normal_map[ind_v].sum_normal;

							indices[i].vertices.emplace_back(v0);
							indices[i].normals.emplace_back(vn0);

							global_vertex_map[ind_v] = tmp_index;

							++tmp_index;

						}
						else
						{
							unsigned int current_ind = global_vertex_map[ind_v];
							indices[i].index.emplace_back(current_ind);
						}
					}
				}

				global_vertex_map.clear();
			}
		}

		//cout<<"vertices :" << v.size() << "\n";

		cout << "faces: " << c << "\n";
		cout << "v:" << v.size() << "\n";
		cout << "indices: " << indices.size() << "\n";
		cout << "mats: " << mats.size() << "\n";

		//cout << "usemtl count: " << count << "\n";
		/*for (int i = 0; i < mats.size(); ++i)
		{
		string name = mats[i].name;
		cout << name << " " << material_map[name] << "\n";
		}*/
		//cout << "indices size: " << indices.size() << "\n";
		//cout <<"floor size : "<< indices[15].ind.size() << "\n";

		meshes.swap(vector<Mesh>());

		v.swap(vector<vec3>());
		vt.swap(vector<vec2>());
		//vn.swap(vector<vec3>());

		material_map.clear();
		//vertex_map.clear();
	}
};

#endif // !_MODEL_H_

