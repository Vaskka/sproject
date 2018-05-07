#include "haltonSampleGenerator.h"

CHaltonSampleGenerator::CHaltonSampleGenerator(unsigned int vBase, unsigned vNumSamples) : CSampleGenerator(vNumSamples), mBase(vBase)
{

}

//************************************************************************
//Function:
void CHaltonSampleGenerator::generateSamples()
{
    for (auto i = 0u; i < mNumSamples; ++ i) {
        float x = static_cast<float>(i) / static_cast<float>(mNumSamples);
        //x = __radicalInverse(2, i);
        float y = __radicalInverse(2, i);
        mSampleSet[i] = {x, y};
    }
}

//************************************************************************
//Function:
float CHaltonSampleGenerator::__radicalInverse(int vBase, int vIndexOfNumber) const
{
    int Value = vIndexOfNumber;
    int InverseNumber = 0;
    int Division = 1;

    while (Value > 0) {
        InverseNumber = InverseNumber * vBase + (Value % vBase);
        Division *= vBase;
        Value /= vBase;
    }
    return static_cast<float>(InverseNumber) / static_cast<float>(Division);
}
