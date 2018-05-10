#include "temporalAATechnique.h"

CTAATechnique::CTAATechnique(CSampleGenerator *vSampleGenerator, const glm::mat4& vProjectionMatrix, const glm::mat4& vViewMatrix, unsigned int vWindowWidth, unsigned int vWindowHeight)
	: m_pSampleGenerator(vSampleGenerator), m_WindowWidth(vWindowWidth), m_WindowHeight(vWindowHeight)
{
	std::memset(&m_CurrentCameraInfo, 0, sizeof(SCameraMatrixInfo));
	std::memset(&m_LastCameraInfo, 0, sizeof(SCameraMatrixInfo));
	_ASSERTE(m_pSampleGenerator);

	m_pSampleGenerator->generateSamples();
	CSample Sample = m_pSampleGenerator->getNextSample();
	m_LastSample = m_CurrentSample = Sample;
	m_LastCameraInfo.setProjectionMatrix(vProjectionMatrix);
	m_LastCameraInfo.setViewMatrix(vViewMatrix);
	m_CurrentCameraInfo.setProjectionMatrix(vProjectionMatrix);
	m_CurrentCameraInfo.setViewMatrix(vViewMatrix);
	__modifyCurrentProjectionMatrix();
}

//************************************************************************
//Function:
void CTAATechnique::initTechniqueV()
{

}

//************************************************************************
//Function:
void CTAATechnique::setLastCameraInfo(const SCameraMatrixInfo &vCameraInfo)
{
	m_LastCameraInfo = vCameraInfo;
}

//************************************************************************
//Function:
void CTAATechnique::setCurrentCameraInfo(const SCameraMatrixInfo &vCameraInfo)
{
	m_CurrentCameraInfo = vCameraInfo;
}

//************************************************************************
//Function:
void CTAATechnique::setLastSample(const CSample &vLastSample)
{
	m_LastSample = vLastSample;
}

//************************************************************************
//Function:
void CTAATechnique::setCurrentSample(const CSample &vCurrentSample)
{
	m_CurrentSample = vCurrentSample;
}

//************************************************************************
//Function:
SCameraMatrixInfo& CTAATechnique::getLastCameraInfo()
{
	return m_LastCameraInfo;
}

//************************************************************************
//Function:
SCameraMatrixInfo& CTAATechnique::getCurrentCameraInfo()
{
	return m_CurrentCameraInfo;
}

//************************************************************************
//Function:
CSample CTAATechnique::getLastSample() const
{
	return m_LastSample;
}


//************************************************************************
//Function:
CSample CTAATechnique::getCurrentSample() const
{
	return m_CurrentSample;
}

//************************************************************************
//Function:
CSample CTAATechnique::getSample() const
{
	return m_pSampleGenerator->getNextSample();
}

//************************************************************************
//Function:
void CTAATechnique::update(const glm::mat4& vCurrentProjectionMatrix, const glm::mat4& vCurrentViewMatrix)
{
	m_LastSample = m_CurrentSample;
	m_LastCameraInfo = m_CurrentCameraInfo;

	m_CurrentSample = m_pSampleGenerator->getNextSample();
	m_CurrentCameraInfo.setProjectionMatrix(vCurrentProjectionMatrix);
	m_CurrentCameraInfo.setViewMatrix(vCurrentViewMatrix);
	__modifyCurrentProjectionMatrix();
}

//************************************************************************
//Function:
void CTAATechnique::undoProjectionOffset(glm::mat4& vDest, const glm::mat4& vSrc) const
{
	vDest = vSrc;
	vDest[2][0] = 0.0f;
	vDest[2][1] = 0.0f;
}

//************************************************************************
//Function:
void CTAATechnique::__modifyCurrentProjectionMatrix()
{
	m_CurrentCameraInfo.ProjectionMatrix[2][0] = (m_CurrentSample.first * 2.0f - 1.0f) / static_cast<float>(m_WindowWidth);
	m_CurrentCameraInfo.ProjectionMatrix[2][1] = (m_CurrentSample.second * 2.0f - 1.0f) / static_cast<float>(m_WindowHeight);
}