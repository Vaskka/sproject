#include <string>
#include <FreeImage.h>

void createWhiteNoiseTexture(int vTexWidth, int vTexHeight, const std::string& vOutputFilePath)
{
	_ASSERTE(!vOutputFilePath.empty() && vTexWidth > 0 && vTexHeight > 0);

	FIBITMAP* pFIBitmap = FreeImage_Allocate(vTexWidth, vTexHeight, 24);
	_ASSERTE(pFIBitmap);

	for (unsigned int x = 0; x < vTexWidth; ++x)
	{
		for (unsigned int y = 0; y < vTexHeight; ++y)
		{
			RGBQUAD rgbQuad;
			rgbQuad.rgbRed = rand() % 256;
			rgbQuad.rgbGreen = rand() % 256;
			rgbQuad.rgbBlue = rand() % 256;
			FreeImage_SetPixelColor(pFIBitmap, x, y, &rgbQuad);
		}
	}

	FreeImage_Save(FIF_BMP, pFIBitmap, vOutputFilePath.c_str(), 0);
	FreeImage_Unload(pFIBitmap);
}

int main()
{
	createWhiteNoiseTexture(256, 256, "grayWhiteNoise01.bmp");

	return 0;
}