#include "Scene.h"
#include "Camera.h"
#include "Model.h"

CScene* CScene::m_Instance = nullptr;

CScene::CScene() :m_Camera(nullptr)
{

}

CScene::~CScene()
{
	destroyScene();
}

//************************************************************************
//Function:
void CScene::initScene()
{
	//m_Camera = new CCamera(glm::vec3(4194.15, 3021.6, 1617.54));
	m_Camera = new CCamera();
}

//************************************************************************
//Function:
void CScene::destroyScene()
{
	if (m_Camera) { delete m_Camera; m_Camera = nullptr; }

	for (auto iter : m_ModelSet)
		delete iter;
	m_ModelSet.clear();
}
