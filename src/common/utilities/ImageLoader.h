/*
 * ImageLoader.h
 *
 *  Created on: 2011-05-01
 *      Author: jarrett
 */

#ifndef IMAGELOADER_H_
#define IMAGELOADER_H_

#include <boost/log/trivial.hpp>

#include "FreeImage.h"

namespace utilities {

struct Image {
	char* data;
	int width;
	int height;
};

class ImageLoader {
public:
	ImageLoader() {}
	virtual ~ImageLoader() {}
	
	Image* loadImageData(const std::string filename, bool hasAlpha = false) {
		return loadImageData( filename.c_str() );
	}
	
	Image* loadImageData(const char* filename, bool hasAlpha = false) {
		BOOST_LOG_TRIVIAL(debug) <<"Loading image.";
		FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(filename,0);
		FIBITMAP* imagen = FreeImage_Load(formato, filename);
		
		BOOST_LOG_TRIVIAL(debug) <<"image loaded.";
		
		FIBITMAP* temp = imagen;
		if (hasAlpha)
			imagen = FreeImage_ConvertTo32Bits(imagen);
		else
			imagen = FreeImage_ConvertTo24Bits(imagen);
		FreeImage_Unload(temp);
	 
		int w = FreeImage_GetWidth(imagen);
		int h = FreeImage_GetHeight(imagen);
		BOOST_LOG_TRIVIAL(debug) <<"The size of the image is: "<<filename<<" es "<<w<<"*"<<h;
		
		GLubyte* textura;
		if (hasAlpha)
			textura = new GLubyte[4*w*h];
		else
			textura = new GLubyte[3*w*h];
		
		char* pixeles = (char*)FreeImage_GetBits(imagen);
		//FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).
		
		Image* retImage = 0;
		
		if (pixeles != NULL) {
			
			int pixelSize = 3;
			if (hasAlpha)
				pixelSize = 4;
				
			for(int j = 0; j < w*h; j++) {
				textura[j * pixelSize + 0] = pixeles[j * pixelSize + 2];
				textura[j * pixelSize + 1] = pixeles[j * pixelSize + 1];
				textura[j * pixelSize + 2] = pixeles[j * pixelSize + 0];
				if (hasAlpha)
					textura[j * pixelSize + 3] = pixeles[j * pixelSize + 3];
				//cout<<j<<": "<<textura[j*4+0]<<"**"<<textura[j*4+1]<<"**"<<textura[j*4+2]<<"**"<<textura[j*4+3]<<endl;
			}
			
			retImage = new Image();
			retImage->data = pixeles;
			retImage->width = w;
			retImage->height = h;
		}
		
		return retImage;	 
		
	}

};

}

#endif /* IMAGELOADER_H_ */












