/*
 * ImageLoader.h
 *
 *  Created on: 2011-05-01
 *      Author: jarrett
 */

#ifndef IMAGELOADER_H_
#define IMAGELOADER_H_

#include "FreeImage.h"

#include "../common/logging/Logger.h"

namespace utilities {

enum Format
{
	FORMAT_RGB = 0,
	FORMAT_RGBA
};

struct Image {
	char* data;
	int width;
	int height;
	Format format;
};

class ImageLoader {
public:
	ImageLoader()
	{
	}
	virtual ~ImageLoader()
	{
	}

	std::unique_ptr<Image> loadImageData(const std::string filename, bool hasAlpha = true)
	{
		return loadImageData(filename.c_str());
	}

	std::unique_ptr<Image> loadImageData(const char* filename, bool hasAlpha = true)
	{
		BOOST_LOG_TRIVIAL(debug) << "Loading image.";
		FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
		FIBITMAP* imageBitmap = FreeImage_Load(format, filename);

		BOOST_LOG_TRIVIAL(debug) << "image loaded.";

		FIBITMAP* temp = imageBitmap;
		if ( hasAlpha )
			imageBitmap = FreeImage_ConvertTo32Bits(imageBitmap);
		else
			imageBitmap = FreeImage_ConvertTo24Bits(imageBitmap);
		FreeImage_Unload(temp);

		int w = FreeImage_GetWidth(imageBitmap);
		int h = FreeImage_GetHeight(imageBitmap);
		BOOST_LOG_TRIVIAL(debug) << "The size of the image '" << filename << "' is: " << w << "*" << h;

		GLubyte* imageBytes;
		if ( hasAlpha )
			imageBytes = new GLubyte[4 * w * h];
		else
			imageBytes = new GLubyte[3 * w * h];

		char* pixels = (char*) FreeImage_GetBits(imageBitmap);

		std::unique_ptr<Image> retImage = std::unique_ptr<Image>();
		
		// FreeImage loads in BGR format, so you need to swap some bytes (Or use GL_BGR)
		if ( pixels != NULL )
		{
			int pixelSize = 3;
			if ( hasAlpha )
				pixelSize = 4;

			for ( int j = 0; j < w * h; j++ )
			{
				imageBytes[j * pixelSize + 0] = pixels[j * pixelSize + 2];
				imageBytes[j * pixelSize + 1] = pixels[j * pixelSize + 1];
				imageBytes[j * pixelSize + 2] = pixels[j * pixelSize + 0];
				if ( hasAlpha )
					imageBytes[j * pixelSize + 3] = pixels[j * pixelSize + 3];
				//cout<<j<<": "<<imageBytes[j*4+0]<<"**"<<imageBytes[j*4+1]<<"**"<<imageBytes[j*4+2]<<"**"<<imageBytes[j*4+3]<<endl;
			}

			retImage = std::unique_ptr<Image>(new Image());
			retImage->data = pixels;
			retImage->width = w;
			retImage->height = h;
			if (hasAlpha)
				retImage->format = FORMAT_RGBA;
			else
				retImage->format = FORMAT_RGB;
		}

		return retImage;
	}
};
}
#endif /* IMAGELOADER_H_ */












