#ifndef TEXTURESETTINGS_H_
#define TEXTURESETTINGS_H_

namespace glr
{
namespace glw
{


/**
 * Used to pass in TextureSettings settings (so we don't have to have a method with a whole ton of parameters).
 */
struct TextureSettings
{
	TextureSettings() : textureWrapS(GL_CLAMP_TO_EDGE), textureWrapT(GL_CLAMP_TO_EDGE)
	{
	}
	
	GLint textureWrapS;
	GLint textureWrapT;
};

}
}

#endif /* TEXTURESETTINGS_H_ */
