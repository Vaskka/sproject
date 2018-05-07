#pragma once

#include <vector>
#include <utility>

using CSample = std::pair<float, float>;

class CSampleGenerator
{
public:
	explicit CSampleGenerator(unsigned int numSamples) : mNumSamples(numSamples), mSampleSet(numSamples)
	{

	}

	virtual ~CSampleGenerator() = default;
	virtual void generateSamples() = 0;

	CSample getNextSample()
	{
		_ASSERTE(!mSampleSet.empty());

		static unsigned int nextSampleIndex = 0;

		if (nextSampleIndex == mSampleSet.size())
			nextSampleIndex = 0;

		return mSampleSet[nextSampleIndex++];
	}

protected:
	unsigned int mNumSamples;
	std::vector<CSample> mSampleSet;
};