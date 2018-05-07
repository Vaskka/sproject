#pragma once

#include <utility>
#include <glm/glm.hpp>
#include "sampleGenerator.h"

struct SCameraMatrixInfo
{
public:
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;

public:
	void setProjectionMatrix(const glm::mat4& vProjectionMatrix) { ProjectionMatrix = vProjectionMatrix; };
	void setViewMatrix(const glm::mat4& vViewMatrix) { ViewMatrix = vViewMatrix; };
};

class CTemporalAntialiasing
{
public:
	CTemporalAntialiasing(CSampleGenerator* vSampleGenerator, const glm::mat4& vProjectionMatrix, glm::mat4& vViewMatrix, unsigned int vWindowWidth, unsigned int vWindowHeight);
	void setLastSample(const CSample& vLastSample);
	void setCurrentSample(const CSample& vCurrentSample);
	void setLastCameraInfo(const SCameraMatrixInfo& vCameraInfo);
	void setCurrentCameraInfo(const SCameraMatrixInfo& vCameraInfo);
	CSample getLastSample() const;
	CSample getCurrentSample() const;
	SCameraMatrixInfo& getLastCameraInfo();
	SCameraMatrixInfo& getCurrentCameraInfo();
	CSample getSample() const;
	void update(const glm::mat4& vCurrentProjectionMatrix, const glm::mat4& vCurrentViewMatrix);

private:
	void __modifyCurrentProjectionMatrix();

private:
	CSampleGenerator *m_pSampleGenerator;
	SCameraMatrixInfo m_LastCameraInfo;
	SCameraMatrixInfo m_CurrentCameraInfo;
	std::pair<float, float> m_LastSample;
	std::pair<float, float> m_CurrentSample;
	unsigned int m_WindowWidth;
	unsigned int m_WindowHeight;
};