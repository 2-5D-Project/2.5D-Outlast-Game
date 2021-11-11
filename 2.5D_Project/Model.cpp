#include "Model.h"
#include"image.h"

Model::Model()
{

}

Model::Model(std::string path)
{
	loadMode(path);
}

Model::Model(std::string path, glm::vec3 _Modelpos)
{
	//this->butterpos = _Modelpos;
	//this->butterpos.y = _Modelpos.y + 1.0f;
	//this->enemyPos = _Modelpos;
	loadMode(path);
}

void Model::draw(Shader* shader,glm::vec3 _pos, Camera _camera)
{

	this->shader = shader;
	this->camera = _camera;
	this->pos = _pos;
	this->shader->use();
	
	glm::mat4 model;
	
	model = glm::translate(model, this->pos);
	//model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));
	model = glm::rotate(model, glm::radians(camera.Yaw * 100), glm::vec3(0, 1.0f, 0));
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(model));

	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "bone_transforms"), nBoneCount, GL_FALSE, glm::value_ptr(currentPose[0]));
	//printf("%d\n", nBoneCount);

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].draw(shader);
	}

}

void Model::enemydraw(Shader* shader, glm::vec3 playpos)
{	
	
	shader->use();
	glm::mat4 model;

	//enemyPos = glm::vec3(-2.0f + der, 0, 2.0f);

	//enemy_yaw_x = (playpos.x - enemyPos.x)/10.0f;
	//enemy_yaw_z = (playpos.z - enemyPos.z) / 10.0f;
	model = glm::translate(model, enemyPos);
	model = glm::rotate(model, glm::radians(90.0f-enemyRotAngle), glm::vec3(0, 1.0f, 0));
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(model));
	
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].draw(shader);
	}
}


inline glm::mat4 Model::assimpToGlmMatrix(aiMatrix4x4 mat)
{
	glm::mat4 m;
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			m[x][y] = mat[y][x];
		}
	}
	return m;
}

inline glm::vec3 Model::assimpToGlmVec3(aiVector3D vec)
{
	return glm::vec3(vec.x, vec.y, vec.z);
}



void Model::loadMode(std::string path)
{
	Assimp::Importer importer;
	
	const aiScene* scene= importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "assimp error" << importer.GetErrorString() << std::endl;
		return;
	}
	 //directory = path.substr(0, path.find_last_of('/'));

	directoryTexture = path.substr(0, path.find_last_of('\\'));		//modelĿ¼
	std::cout <<"success!" << directoryTexture << std::endl;

	globalInverseTransform = assimpToGlmMatrix(scene->mRootNode->mTransformation);
	globalInverseTransform = glm::inverse(globalInverseTransform);

	processNode(scene->mRootNode, scene);
	loadAnimation(scene, animation);

	//currentPose is held in this vector and uploaded to gpu as a matrix array uniform

	currentPose.resize(nBoneCount, identity); // use this for no animation
}

void Model::processNode(aiNode* node, const aiScene* scene)
{

	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* curMesh = scene->mMeshes[node->mMeshes[i]];

		meshes.push_back(processMesh(curMesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}


Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
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

		tempvertex.boneIds = glm::ivec4(0);
		tempvertex.boneWeights = glm::vec4(0.0f);

		tempvertices.push_back(tempvertex);
	}

	std::unordered_map<std::string, std::pair<int, glm::mat4>> boneInfo = {};
	std::vector<uint> boneCounts;
	boneCounts.resize(tempvertices.size(), 0);

	nBoneCount = mesh->mNumBones;
	//打印骨骼总数
	std::cout << "loadModel() nBoneCount=" << nBoneCount << "\n" << std::endl;
	//loop through each bone
	for (uint i = 0; i < nBoneCount; i++) {
		aiBone* bone = mesh->mBones[i];
		glm::mat4 m = assimpToGlmMatrix(bone->mOffsetMatrix);	//!!!!!!!!!!!!!
		boneInfo[bone->mName.C_Str()] = { i, m };

		//loop through each vertex that have that bone
		for (int j = 0; j < bone->mNumWeights; j++) {
			uint id = bone->mWeights[j].mVertexId;
			float weight = bone->mWeights[j].mWeight;
			boneCounts[id]++;
			switch (boneCounts[id]) {
			case 1:
				tempvertices[id].boneIds.x = i;
				tempvertices[id].boneWeights.x = weight;
				break;
			case 2:
				tempvertices[id].boneIds.y = i;
				tempvertices[id].boneWeights.y = weight;
				break;
			case 3:
				tempvertices[id].boneIds.z = i;
				tempvertices[id].boneWeights.z = weight;
				break;
			case 4:
				tempvertices[id].boneIds.w = i;
				tempvertices[id].boneWeights.w = weight;
				break;
			default:
				//std::cout << "err: unable to allocate bone to vertex" << std::endl;
				break;

			}
		}
	}

	//normalize weights to make all weights sum 1
	for (int i = 0; i < tempvertices.size(); i++) {
		glm::vec4& boneWeights = tempvertices[i].boneWeights;
		float totalWeight = boneWeights.x + boneWeights.y + boneWeights.z + boneWeights.w;
		if (totalWeight > 0.0f) {
			tempvertices[i].boneWeights = glm::vec4(
				boneWeights.x / totalWeight,
				boneWeights.y / totalWeight,
				boneWeights.z / totalWeight,
				boneWeights.w / totalWeight
			);
		}
	}



	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
		{
			tempindices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}

	// create bone hirerchy
	
	readSkeleton(skeleton, scene->mRootNode, boneInfo);

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse",scene);
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

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene)
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

unsigned int Model::TextureFromFile(const char* path, const string& directory, bool gamma)
{
	string pa;
	string filename = string(path);
	filename = directory + '\\' + filename;
	//std::cout << "success!" << filename << std::endl;
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

void Model::loadAnimation(const aiScene* scene, Animation& animation)
{
	//loading  first Animation
	aiAnimation* anim = scene->mAnimations[0];

	if (anim->mTicksPerSecond != 0.0f)
		animation.ticksPerSecond = anim->mTicksPerSecond;
	else
		animation.ticksPerSecond = 1;


	animation.duration = anim->mDuration * anim->mTicksPerSecond;
	animation.boneTransforms = {};

	//duration 动画时间间隔  ticksPerSecond 时间单位
	std::cout << "loadAnimation() ticksPerSecond=" << animation.ticksPerSecond << " duration=" << animation.duration << "\n" << std::endl;

	//现在来修复这个bug
	bool checkAssimpFbx = false;						//是否是AssimpFbx动画
	std::string assimpFbxStr;							//存储AssimpFbx动画的名称
	std::vector<BoneTransformTrack> assimpFbxVector;	//存储AssimpFbx的动画序列
	//load positions rotations and scales for each bone
	// each channel represents each bone
	for (int i = 0; i < anim->mNumChannels; i++) {
		aiNodeAnim* channel = anim->mChannels[i];
		BoneTransformTrack track;
		for (int j = 0; j < channel->mNumPositionKeys; j++) {
			track.positionTimestamps.push_back(channel->mPositionKeys[j].mTime);
			track.positions.push_back(assimpToGlmVec3(channel->mPositionKeys[j].mValue));
		}
		for (int j = 0; j < channel->mNumRotationKeys; j++) {
			track.rotationTimestamps.push_back(channel->mRotationKeys[j].mTime);
			track.rotations.push_back(assimpToGlmQuat(channel->mRotationKeys[j].mValue));

		}
		for (int j = 0; j < channel->mNumScalingKeys; j++) {
			track.scaleTimestamps.push_back(channel->mScalingKeys[j].mTime);
			track.scales.push_back(assimpToGlmVec3(channel->mScalingKeys[j].mValue));

		}

		std::string nName(channel->mNodeName.C_Str());
		std::string::size_type ret = nName.find("_$AssimpFbx$_");
		//如果是FBX动画则先暂存
		if (ret != std::string::npos)
		{
			checkAssimpFbx = true;
			assimpFbxStr = nName.substr(0, ret);
			assimpFbxVector.push_back(track);
			std::cout << "loadAnimation() print assimpFbxStr=" << assimpFbxStr << " OldStr=" << nName << std::endl;
		}
		else {
			//如果是FBX动画刚结束，则暂存动画保存至animation
			if (checkAssimpFbx)
			{
				checkAssimpFbx = false;
				//用于暂存的变量
				BoneTransformTrack outTrack;
				for (int i = 0; i < assimpFbxVector.size(); i++)
				{
					BoneTransformTrack item = assimpFbxVector[i];
					if (item.positions.size() > 1)
					{
						outTrack.positionTimestamps = item.positionTimestamps;
						outTrack.positions = item.positions;
					}
					if (item.rotations.size() > 1)
					{
						outTrack.rotationTimestamps = item.rotationTimestamps;
						outTrack.rotations = item.rotations;
					}
					if (item.scales.size() > 1)
					{
						outTrack.scaleTimestamps = item.scaleTimestamps;
						outTrack.scales = item.scales;
					}
				}
				std::cout << "loadAnimation() animation FBX=" << assimpFbxStr << std::endl;
				animation.boneTransforms[assimpFbxStr] = outTrack;

			}
			//打印动画信息 骨骼(bone)与动画(animation)信息是一对一关系!
			std::cout << "loadAnimation() animation=" << channel->mNodeName.C_Str() << std::endl;
			animation.boneTransforms[channel->mNodeName.C_Str()] = track;
		}

	}
}

std::pair<uint, float> Model::getTimeFraction(std::vector<float>& times, float& dt)
{
	uint segment = 0;//times是个空数组 故报错	
	//dt = 276.320007 但times[1]=276 segment百分百的越界
	while (dt > times[segment])
	{
		segment++;
		if (segment >= times.size()) {
			segment--;
			break;
		}
	}

	float start = times[segment - 1];
	float end = times[segment];
	float frac = (dt - start) / (end - start);
	return { segment, frac };
}

void Model::getPose(Animation& animation, Bone& skeletion, float dt, std::vector<glm::mat4>& output, glm::mat4& parentTransform, glm::mat4& globalInverseTransform)
{
	BoneTransformTrack& btt = animation.boneTransforms[skeletion.name];//根据骨骼名"mixamorig:Hips" btt没有找到动画信息! 这里是出事地点，却不是案发现场
	//因为骨骼(bone)与动画(animation)信息是一对一关系
	//所以问题就很明显: 
	//在控制台里 骨骼mixamorig:Hips 对应的动画信息是 mixamorig:Hips_$AssimpFbx$_Translation Hips_$AssimpFbx$_Rotation Hips_$AssimpFbx$_Scaling三个动画
	//所以需要将刚才三个动画合成一个动画，并且名称改成 mixamorig:Hips

	//再一次的vector溢出表示仍然存在着更深的bug
	//如果老实的一句一句的跟断点将永远找不着出事地点
	//从控制台里可以看到bone=空

	//但愿这是最后一次bug。 非常确定这个bug藏得更深。
	//从控制台可以看到 打印bone到 mixamorig:LeftHand 卡住。但却有无数个循环。。。接着看吧
	//图像编程里最常见的BUG就是循环里或者嵌套循环里有BUG。。。
	//利用时间刻度来打印看看吧。
	//利用时间刻度和骨骼名两重断点的判断就能找到出事地点
	if (btt.positions.size() == 0 || btt.rotations.size() == 0 || btt.scales.size() == 0)
		return;

	//if(skeletion.name == "mixamorig:LeftHand" )
		//std::cout << "getPose() bone=" << skeletion.name << std::endl;

	dt = fmod(dt, animation.duration);
	std::pair<uint, float> fp;
	//calculate interpolated position
	fp = getTimeFraction(btt.positionTimestamps, dt);

	glm::vec3 position1 = btt.positions[fp.first - 1];
	glm::vec3 position2 = btt.positions[fp.first];

	glm::vec3 position = glm::mix(position1, position2, fp.second);

	//calculate interpolated rotation
	fp = getTimeFraction(btt.rotationTimestamps, dt);
	glm::quat rotation1 = btt.rotations[fp.first - 1];
	glm::quat rotation2 = btt.rotations[fp.first];

	glm::quat rotation = glm::slerp(rotation1, rotation2, fp.second);

	//calculate interpolated scale
	fp = getTimeFraction(btt.scaleTimestamps, dt);
	glm::vec3 scale1 = btt.scales[fp.first - 1];
	glm::vec3 scale2 = btt.scales[fp.first];

	glm::vec3 scale = glm::mix(scale1, scale2, fp.second);

	glm::mat4 positionMat = glm::mat4(1.0),
		scaleMat = glm::mat4(1.0);


	// calculate localTransform
	positionMat = glm::translate(positionMat, position);
	glm::mat4 rotationMat = glm::toMat4(rotation);
	scaleMat = glm::scale(scaleMat, scale);
	glm::mat4 localTransform = positionMat * rotationMat * scaleMat;
	glm::mat4 globalTransform = parentTransform * localTransform;

	output[skeletion.id] = globalInverseTransform * globalTransform * skeletion.offset;
	//update values for children bones
	for (Bone& child : skeletion.children) {
		//printf("ssssdd\n");
		getPose(animation, child, dt, output, globalTransform, globalInverseTransform);
	}
	//std::cout << dt << " => " << position.x << ":" << position.y << ":" << position.z << ":" << std::endl;
}

bool Model::readSkeleton(Bone& boneOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, glm::mat4>>& boneInfoTable)
{
	if (boneInfoTable.find(node->mName.C_Str()) != boneInfoTable.end()) { // if node is actually a bone
		boneOutput.name = node->mName.C_Str();
		boneOutput.id = boneInfoTable[boneOutput.name].first;
		boneOutput.offset = boneInfoTable[boneOutput.name].second;

		//打印骨骼信息
		std::cout << "readSkeleton() bone=" << boneOutput.name << std::endl;

		for (int i = 0; i < node->mNumChildren; i++) {
			Bone child;
			readSkeleton(child, node->mChildren[i], boneInfoTable);
			boneOutput.children.push_back(child);
		}
		return true;
	}
	else { // find bones in children
		for (int i = 0; i < node->mNumChildren; i++) {
			if (readSkeleton(boneOutput, node->mChildren[i], boneInfoTable)) {
				return true;
			}

		}
	}
	return false;
}

void Model::Action(float elapsedTime)
{
	
	getPose(animation, skeleton, elapsedTime, currentPose, identity, globalInverseTransform);
}

