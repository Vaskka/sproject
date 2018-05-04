#pragma once

#include "technique.h"

class CYGGTechnique : public CTechnique
{
public:
	CYGGTechnique();
	virtual ~CYGGTechnique();

	virtual bool initTechniqueV() override;
};