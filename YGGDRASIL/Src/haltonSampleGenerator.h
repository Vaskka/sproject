#pragma once
#include "sampleGenerator.h"

class CHaltonSampleGenerator : public CSampleGenerator {
public:
    explicit CHaltonSampleGenerator(unsigned int vBase, unsigned int vNumSamples);
    virtual ~CHaltonSampleGenerator() = default;
    virtual void generateSamples() override;


private:
    float __radicalInverse(int vBase, int vIndexOfNumber) const;

private:
    unsigned int mBase;
};

