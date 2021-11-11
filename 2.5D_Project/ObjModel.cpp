#include "ObjModel.h"
#define STB_IMAGE_IMPLEMENTATION
#include"image.h"
ObjModel::ObjModel()
{

}

ObjModel::ObjModel(std::string path)
{
    loadMode(path);
}

ObjModel::ObjModel(std::string path, glm::vec3 _Modelpos)
{
	//this->butterYawPos=_Modelpos;
    this->butterpos = _Modelpos;
    this->butterpos.y = _Modelpos.y + 1.0f;
    loadMode(path);
}

void ObjModel::butterdraw(Shader* shader, float Yaw)
{
	this->shader = shader;
	//this->camera = _camera;

	//butterpos.y = 1.0f;

	this->shader->use();
	if (!Butter_Fight)
	{
		Butter_yaw = -Yaw * 100 + 180.0f;
		Butter_Fight = true;		//子弹被赋予一次朝向时，之后不能再重复赋予.防止视角转动会影响之前的子弹方向
	}

	//butterpos.x -= 0.5f;
	glm::mat4 model;
	butterpos -= glm::vec3(0, 0, 0.1f) * glm::mat3(glm::rotate(model, glm::radians(Butter_yaw), glm::vec3(0, 1.0f, 0)));
	model = glm::translate(model, butterpos);

	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(model));
	

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].texture_id = 9;
		meshes[i].draw(shader);
	}
	//shader->setunifor1f("texture_diffuse",9);
}

void ObjModel::enemybutterdraw(Shader* shader, glm::vec3 playpos)
{
	this->shader = shader;
	//this->camera = _camera;

	//butterpos.y = 1.0f;

	this->shader->use();
	if (!Butter_Fight)
	{
		enemyButter_yaw_x = (playpos.x-butterpos.x)/40;		//敌人发射子弹时与玩家之间的偏移量	40表明速度减缓幅度
		enemyButter_yaw_z = (playpos.z - butterpos.z)/40;
		Butter_Fight = true;		//子弹被赋予一次朝向时，之后不能再重复赋予.防止视角转动会影响之前的子弹方向
	}

	glm::mat4 model;
	//butterpos -= glm::vec3(0, 0, 0.05f) * glm::mat3(glm::rotate(model, glm::radians(Butter_yaw), glm::vec3(0, 1.0f, 0)));
	butterpos.x += enemyButter_yaw_x;
	butterpos.z += enemyButter_yaw_z;
	model = glm::translate(model, butterpos);

	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(model));

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].texture_id = 1;
		meshes[i].draw(shader);
	}
}

void ObjModel::wallInit()
{
	
}

void ObjModel::obstaclesInit()
{

}


void ObjModel::NPCdraw(Shader* shader)
{
	shader->use();
	NPCpos = glm::vec3(-3.0f, 0, -3.0f);
	glm::mat4 model;
	model = glm::translate(model, NPCpos);

	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(model));
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].draw(shader);
	}
}



void ObjModel::melee_weapons_draw(Shader* shader, glm::vec3 weaponsInitPos,float yaw)
{
	shader->use();
	weaponsInitPos.x = weaponsInitPos.x - 0.1f;
	weaponsInitPos.y = weaponsInitPos.y + 0.5f;
	weaponsInitPos.z = weaponsInitPos.z;
	//butterpos = weaponsInitPos;
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(weaponsInitPos.x, weaponsInitPos.y, weaponsInitPos.z));
	model = glm::rotate(model, glm::radians(yaw * 100 - 90.0f), glm::vec3(0, 1.0f, 0));
	if (melee_weapons_trans>0)
	{
		melee_weapons_trans -= 10.0f;
		//printf("%.2f\n", melee_weapons_trans);
		model = glm::rotate(model, glm::radians(melee_weapons_trans), glm::vec3(0.0f, 0, 1.0f));
	}
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(model));
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].texture_id = 8;
		meshes[i].draw(shader);
	}
}

void ObjModel::loadMode(std::string path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "assimp error" << importer.GetErrorString() << std::endl;
		return;
	}
	//directory = path.substr(0, path.find_last_of('/'));

	directoryTexture = path.substr(0, path.find_last_of('\\'));		//modelĿ¼
	//std::cout <<"success!" << directoryTexture << std::endl;

	processNode(scene->mRootNode, scene);
}


void ObjModel::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* curMesh = scene->mMeshes[node->mMeshes[i]];

		meshes.push_back(processMesh(curMesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh ObjModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> tempvertices;
	std::vector<unsigned int> tempindices;
	std::vector<Texture> temptextures;

	glm::vec3 tempvec;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex tempvertex;
		tempvec.x = mesh->mVertices[i].x;
		tempvec.y = mesh->mVertices[i].y;
		tempvec.z = mesh->mVertices[i].z;
		tempvertex.Position = tempvec;

		tempvec.x = mesh->mNormals[i].x;
		tempvec.y = mesh->mNormals[i].y;
		tempvec.z = mesh->mNormals[i].z;
		tempvertex.Normal = tempvec;

		if (mesh->mTextureCoords[0]) {

			tempvec.x = mesh->mTextureCoords[0][i].x;
			tempvec.y = mesh->mTextureCoords[0][i].y;
			tempvertex.TexCoords = tempvec;
		}
		else
		{
			tempvertex.TexCoords = glm::vec2(0, 0);
		}
		tempvertices.push_back(tempvertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
		{
			tempindices.push_back(mesh->mFaces[i].mIndices[j]);
		}

	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
	temptextures.insert(temptextures.end(), diffuseMaps.begin(), diffuseMaps.end());

	//vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	//temptextures.insert(temptextures.end(), specularMaps.begin(), specularMaps.end());
	//// 3. normal maps
	//std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	//temptextures.insert(temptextures.end(), normalMaps.begin(), normalMaps.end());
	//// 4. height maps
	//std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	//temptextures.insert(temptextures.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(tempvertices, tempindices, temptextures);
}

void ObjModel::dropdraw(Shader* shader)
{
	shader->use();
	glm::mat4 model;

	//enemyPos = glm::vec3(-2.0f + der, 0, 2.0f);

	model = glm::translate(model, dropPos);
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(model));

	//printf("%.2f\n",enemyPos.x);

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].draw(shader);
	}
}

vector<Texture> ObjModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;

		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}


		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;


			texture.id = TextureFromFile(str.C_Str(), this->directoryTexture);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

unsigned int ObjModel::TextureFromFile(const char* path, const string& directory, bool gamma)
{
	string pa;
	string filename = string(path);
	filename = directory + '\\' + filename;
	std::cout << "success!" << filename << std::endl;
	unsigned int textureID;
	glGenTextures(1, &textureID);
	
	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
