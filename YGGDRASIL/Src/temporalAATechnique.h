#pragma once

#include <utility>
#include <glm/glm.hpp>
#include "sampleGenerator.h"
#include "technique.h"

struct SCameraMatrixInfo
{
public:
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewMatrix;

public:
	void setProjectionMatrix(const glm::mat4& vProjectionMatrix) { ProjectionMatrix = vProjectionMatrix; };
	void setViewMatrix(const glm::mat4& vViewMatrix) { ViewMatrix = vViewMatrix; };
};

class CTAATechnique : public CTechnique
{
public:
	CTAATechnique(CSampleGenerator* vSampleGenerator, const glm::mat4& vProjectionMatrix, const glm::mat4& vViewMatrix, unsigned int vWindowWidth, unsigned int vWindowHeight);

	virtual void initTechniqueV() override;

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
	void undoProjectionOffset(glm::mat4& vDest, const glm::mat4& vSrc) const;

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