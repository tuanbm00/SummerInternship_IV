//#include "stdafx.h"
#include <soloud.h>
#include "ResourceManager.h"
#include <iostream>



ResourceManager* ResourceManager::s_Instance = NULL;

ResourceManager::ResourceManager(void)
{
	m_Soloud = SoLoud::Soloud();
	m_Soloud.init();
}

ResourceManager* ResourceManager::GetInstance()
{
	if (!s_Instance)
		s_Instance = new ResourceManager();
	return s_Instance;
}


ResourceManager::~ResourceManager()
{
	if (s_Instance)
	{
		delete s_Instance;
		s_Instance = NULL;
	}
}

void ResourceManager::Init() {
	FILE* f_RM;
	f_RM = fopen("../Resources/Managers/RM.txt", "r");
	if (f_RM != NULL) {
		this->ReadFile(f_RM);
	}
	else std::cout << "Khong tim thay file Scene Manager! \n";
}

void ResourceManager::ReadFile(FILE* f_RM)
{
	//Models
	int numOfModels;
	fscanf(f_RM, "#Models: %d\n", &numOfModels);
	for (register int i = 0; i < numOfModels; i++) {
		int ID;
		char File[250];
		fscanf(f_RM, "ID %d\n", &ID);
		fscanf(f_RM, "FILE %s\n", &File);

		auto model = new Model(ID, File);
		m_Models.push_back(model);
	}

	//Textures
	int numOfTextures;
	fscanf(f_RM, "#2D Textures : %d\n", &numOfTextures);
	for (register int i = 0; i < numOfTextures; i++) {
		int ID;
		char File[250];
		fscanf(f_RM, "ID %d\n", &ID);
		fscanf(f_RM, "FILE %s\n", &File);

		Texture* texture = new Texture(ID, File);
		m_Textures.push_back(texture);
	}

	//Cube Textures
	int numOfCubeTextures;
	fscanf(f_RM, "#Cube Textures: %d\n", &numOfCubeTextures);
	for (register int i = 0; i < numOfCubeTextures; i++) {
		int ID;
		std::vector<std::string> faces;
		char File[200], Wrap[30], Filter1[30], Filter2[30];
		fscanf(f_RM, "ID %d\n", &ID);
		fscanf(f_RM, "FILE1 %s\n", &File);
		faces.push_back(File);
		fscanf(f_RM, "FILE2 %s\n", &File);
		faces.push_back(File);
		fscanf(f_RM, "FILE3 %s\n", &File);
		faces.push_back(File);
		fscanf(f_RM, "FILE4 %s\n", &File);
		faces.push_back(File);
		fscanf(f_RM, "FILE5 %s\n", &File);
		faces.push_back(File);
		fscanf(f_RM, "FILE6 %s\n", &File);
		faces.push_back(File);
		fscanf(f_RM, "WRAP %s\n", &Wrap);
		fscanf(f_RM, "FILTER %s %s\n", &Filter1, &Filter2);


		CubeTextures* ctexture = new CubeTextures(ID, faces);
		m_CubeTextures.push_back(ctexture);
	}

	//Shaders
	int numOfShaders;
	fscanf(f_RM, "#Shaders: %d\n", &numOfShaders);
	for (register int i = 0; i < numOfShaders; i++) {
		int ID, states, depth_test = 0, cull_face = 0, blend = 0;
		char FileVS[250], FileFS[250];
		fscanf(f_RM, "ID %d\n", &ID);
		fscanf(f_RM, "VS %s\n", &FileVS);
		fscanf(f_RM, "FS %s\n", &FileFS);
		fscanf(f_RM, "STATES %d\n", &states);
		if (states > 0) {
			fscanf(f_RM, "GL_DEPTH_TEST %d\n", &depth_test);
			states--;
		}
		if (states > 0) {
			fscanf(f_RM, "GL_CULL_FACE %d\n", &cull_face);
			states--;
		}
		if (states > 0) {
			fscanf(f_RM, "GL_BLEND %d\n", &blend);
			states--;
		}

		Shaders* shader = new Shaders(ID, FileVS, FileFS, depth_test, cull_face, blend);
		m_Shaders.push_back(shader);
	}

	fclose(f_RM);
	
}

Model* ResourceManager::GetModelAtID(int ID) {
	for (register int i = 0; i < m_Models.size(); i++) {
		if (m_Models[i]->GetID() == ID) {
			return m_Models[i];
		}
	}
	std::cout << "No Model found! " << std::endl;
	return NULL;
}

Shaders* ResourceManager::GetShaderAtID(int ID) {
	for (register int i = 0; i < m_Shaders.size(); i++) {
		if (m_Shaders[i]->GetID() == ID) {
			return m_Shaders[i];
		}
	}
	std::cout << "No Shader found! " << std::endl;
	return NULL;
}

Texture* ResourceManager::GetTextureAtID(int ID) {
	for (register int i = 0; i < m_Textures.size(); i++) {
		if (m_Textures[i]->GetID() == ID) {
			return m_Textures[i];
		}
	}
	std::cout << "No Texture found! " << std::endl;
	return NULL;
}

CubeTextures* ResourceManager::GetCubeTextureAtID(int ID) {
	for (register int i = 0; i < m_CubeTextures.size(); i++) {
		if (m_CubeTextures[i]->GetID() == ID) {
			return m_CubeTextures[i];
		}
	}
	std::cout << "No Cube Texture found! " << std::endl;
	return NULL;
}

//TruongNV - Sound Functions
//Add Sound
void ResourceManager::AddSound(const std::string& name)
{
	auto it = m_MapWave.find(name);
	if (it != m_MapWave.end())
	{
		return;
	}
	std::shared_ptr<SoLoud::Wav> wave;
	std::string wav = name;
	wave = std::make_shared<SoLoud::Wav>();
	wave->load(wav.c_str()); //Load a wav file
	m_MapWave.insert(std::pair<std::string, std::shared_ptr<SoLoud::Wav>>(name, wave));
}

//Play Sound
void ResourceManager::PlaySound(const std::string& name, bool loop)
{
	std::shared_ptr<SoLoud::Wav> wave;

	auto it = m_MapWave.find(name);
	if (it != m_MapWave.end())
	{
		wave = it->second;
	}
	else {
		std::string wav = name;
		wave = std::make_shared<SoLoud::Wav>();
		wave->load(wav.c_str()); //Load a wav file
		m_MapWave.insert(std::pair<std::string, std::shared_ptr<SoLoud::Wav>>(name, wave));
	}
	int handle = m_Soloud.play(*wave);
	if (loop) {
		m_Soloud.setLooping(handle, 1);
	}
}

//Stop Sound
void ResourceManager::StopSound(const std::string& name)
{
	std::shared_ptr<SoLoud::Wav> wave;
	auto it = m_MapWave.find(name);
	if (it != m_MapWave.end())
	{
		wave = it->second;
	}

	m_Soloud.stopAudioSource(*wave);
}

//Stop All Sound
void ResourceManager::StopAllSound()
{
	m_Soloud.stopAll();
}

void ResourceManager::CleanUp() {
	for (int i = 0; i < m_Models.size(); i++) {
		delete m_Models[i];
	}
	for (int i = 0; i < m_Textures.size(); i++) {
		delete m_Textures[i];
	}
	for (int i = 0; i < m_Shaders.size(); i++) {
		delete m_Shaders[i];
	}
	for (int i = 0; i < m_CubeTextures.size(); i++) {
		delete m_CubeTextures[i];
	}
}