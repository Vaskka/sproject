#pragma once

#include <vector>

class CCamera;
class CModel;

class CScene
{
public:
	static CScene* getInstance()
	{
		if (nullptr == m_Instance) {
			m_Instance = new CScene();
		}
		return m_Instance;
	}

	void initScene();
	void destroyScene();

	void addModel(CModel* vModel) { m_ModelSet.push_back(vModel); }
	CModel* getModelAt(int vIndex) const { return m_ModelSet[vIndex]; }
	unsigned getNumModels() const { return m_ModelSet.size(); }

	CCamera* getCamera() const { return m_Camera; }

private:
	CScene();
	virtual ~CScene();

private:
	static CScene* m_Instance;
	CCamera* m_Camera;
	std::vector<CModel*> m_ModelSet;
};