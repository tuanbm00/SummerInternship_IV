#include "stdafx.h"
#include "Singleton.h"

Singleton* Singleton::ms_pInstance = NULL;

Singleton::Singleton() {
	this->Init();
}

Singleton::~Singleton() {
}

void Singleton::Init() {

}

void Singleton::CreateInstance()
{
	if (ms_pInstance == NULL)
		ms_pInstance = new Singleton;
}
Singleton* Singleton::GetInstance()
{
	return ms_pInstance;
}

void Singleton::DestroyInstance()
{
	if (ms_pInstance)
	{
		delete ms_pInstance;
		ms_pInstance = NULL;
	}
}