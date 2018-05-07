#include "temporalAntialiasing.h"

CTemporalAntialiasing::CTemporalAntialiasing(CSampleGenerator *vSampleGenerator, const glm::mat4& vProjectionMatrix, glm::mat4& vViewMatrix, unsigned int vWindowWidth, unsigned int vWindowHeight)
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
void CTemporalAntialiasing::setLastCameraInfo(const SCameraMatrixInfo &vCameraInfo)
{
	m_LastCameraInfo = vCameraInfo;
}

//************************************************************************
//Function:
void CTemporalAntialiasing::setCurrentCameraInfo(const SCameraMatrixInfo &vCameraInfo)
{
	m_CurrentCameraInfo = vCameraInfo;
}

//************************************************************************
//Function:
void CTemporalAntialiasing::setLastSample(const CSample &vLastSample)
{
	m_LastSample = vLastSample;
}

//************************************************************************
//Function:
void CTemporalAntialiasing::setCurrentSample(const CSample &vCurrentSample)
{
	m_CurrentSample = vCurrentSample;
}

//************************************************************************
//Function:
SCameraMatrixInfo& CTemporalAntialiasing::getLastCameraInfo()
{
	return m_LastCameraInfo;
}

//************************************************************************
//Function:
SCameraMatrixInfo& CTemporalAntialiasing::getCurrentCameraInfo()
{
	return m_CurrentCameraInfo;
}

//************************************************************************
//Function:
CSample CTemporalAntialiasing::getLastSample() const
{
	return m_LastSample;
}


//************************************************************************
//Function:
CSample CTemporalAntialiasing::getCurrentSample() const
{
	return m_CurrentSample;
}

//************************************************************************
//Function:
CSample CTemporalAntialiasing::getSample() const
{
	return m_pSampleGenerator->getNextSample();
}

//************************************************************************
//Function:
void CTemporalAntialiasing::update(const glm::mat4& vCurrentProjectionMatrix, const glm::mat4& vCurrentViewMatrix)
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
void CTemporalAntialiasing::__modifyCurrentProjectionMatrix()
{
	//m_CurrentCameraInfo.ProjectionMatrix[2 * 4] = (m_CurrentSample.first * 2.0f - 1.0f) / static_cast<float>(m_WindowWidth);
	//m_CurrentCameraInfo.ProjectionMatrix[2 * 4 + 1] = (m_CurrentSample.second * 2.0f - 1.0f) / static_cast<float>(m_WindowHeight);
	m_CurrentCameraInfo.ProjectionMatrix[2][0] = -(m_CurrentSample.first * 2.0f - 1.0f) / static_cast<float>(m_WindowWidth);
	m_CurrentCameraInfo.ProjectionMatrix[2][1] = -(m_CurrentSample.second * -2.0f + 1.0f) / static_cast<float>(m_WindowHeight);
}