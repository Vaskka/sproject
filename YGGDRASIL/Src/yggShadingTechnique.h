#pragma once

#include "shadingTechnique.h"

class CYGGShadingTechnique : public CShadingTechnique
{
public:
	CYGGShadingTechnique();
	virtual ~CYGGShadingTechnique();

	virtual void initTechniqueV() override;
};