#pragma once
#include "ShadingTechnique.h"

class CYGGShadingTechnique : public CShadingTechnique
{
public:
	CYGGShadingTechnique();
	virtual ~CYGGShadingTechnique();

	virtual void initTechniqueV() override;
};