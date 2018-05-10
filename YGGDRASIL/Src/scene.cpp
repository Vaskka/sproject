#include "scene.h"
#include "camera.h"
#include "model.h"

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
