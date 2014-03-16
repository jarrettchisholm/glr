#ifndef ITEXTUREMANAGER_H_
#define ITEXTUREMANAGER_H_

#include <string>

#include "Texture2D.hpp"
#include "Texture2DArray.hpp"

#include "ITexture.hpp"

#include "common/utilities/ImageLoader.hpp"

namespace glr
{
namespace glw
{
	
class Texture2D;
class Texture2DArray;
	
class ITextureManager
{
public:
	virtual ~ITextureManager()
	{
	}
	;

	//virtual Texture1D* getTexture1D(const std::string name) = 0;
	//virtual Texture1DArray* getTexture1DArray(const std::string name) = 0;
	//virtual Texture1D* addTexture1D(const std::string name, const std::string filename) = 0;
	//virtual Texture1DArray* addTexture1DArray(const std::string name, const std::string filename) = 0;

	/**
	 * Returns the texture with the given name.  If a texture with the given name doesn't exist, it returns
	 * nullptr.
	 * 
	 * @param name
	 * 
	 * @return The Texture2D object with name 'name', or nullptr if no texture exists with that name.
	 */
	virtual Texture2D* getTexture2D(const std::string& name) const = 0;
	
	/**
	 * Returns the texture array with the given name.  If a texture array with the given name doesn't exist, it returns
	 * nullptr.
	 * 
	 * @param name
	 * 
	 * @return The Texture2DArray object with name 'name', or nullptr if no texture array exists with that name.
	 */
	virtual Texture2DArray* getTexture2DArray(const std::string& name) const = 0;
	
	/**
	 * Creates an empty texture with the given name and using the provided texture settings.
	 * 
	 * If a texture already exists with the given name, it will return that texture.
	 * 
	 * @param name
	 * @param settings
	 * 
	 * @return The empty Texture2D object.
	 */
	virtual Texture2D* addTexture2D(const std::string& name, const TextureSettings settings = TextureSettings()) = 0;
	
	/**
	 * Creates a texture with the given name and using the provided texture settings.  It will fill the texture with the data
	 * loaded from the file 'filename'.  The file must be a valid image.
	 * 
	 * If a texture already exists with the given name, it will return that texture.
	 * 
	 * @param name
	 * @param filename
	 * @param settings
	 * 
	 * @return A Texture2D object.
	 */
	virtual Texture2D* addTexture2D(const std::string& name, const std::string& filename, const TextureSettings settings = TextureSettings()) = 0;
	
	/**
	 * Creates a texture with the given name and using the provided texture settings.  It will fill the texture with the data
	 * from the provided image.  The image must be valid and non-null.
	 * 
	 * If a texture already exists with the given name, it will return that texture.
	 * 
	 * @param name
	 * @param image
	 * @param settings
	 * 
	 * @return A Texture2D object.
	 */
	virtual Texture2D* addTexture2D(const std::string& name, utilities::Image* image, const TextureSettings settings = TextureSettings()) = 0;

	/**
	 * Creates an empty texture 2d array with the given name and using the provided texture settings.
	 * 
	 * If a texture array already exists with the given name, it will return that texture array.
	 * 
	 * @param name
	 * @param settings
	 * 
	 * @return The empty Texture2DArray object.
	 */
	virtual Texture2DArray* addTexture2DArray(const std::string& name, const TextureSettings settings = TextureSettings()) = 0;
	
	/**
	 * Creates a texture 2d array with the given name and using the provided texture settings.  It will fill the texture array with the data
	 * loaded from the files specified in 'filenames'.  The files must be valid images.
	 * 
	 * If a texture array already exists with the given name, it will return that texture.
	 * 
	 * @param name
	 * @param filenames
	 * @param settings
	 * 
	 * @return A Texture2DArray object.
	 */
	virtual Texture2DArray* addTexture2DArray(const std::string& name, const std::vector<std::string>& filenames, const TextureSettings settings = TextureSettings()) = 0;
	
	/**
	 * Creates a texture 2d array with the given name and using the provided texture settings.  It will fill the texture array with the data
	 * from the provided images.  The images must be valid and non-null.
	 * 
	 * If a texture array already exists with the given name, it will return that texture.
	 * 
	 * @param name
	 * @param images
	 * @param settings
	 * 
	 * @return A Texture2DArray object.
	 */
	virtual Texture2DArray* addTexture2DArray(const std::string& name, const std::vector<utilities::Image*>& images, const TextureSettings settings = TextureSettings()) = 0;

	// Do I want to do this one?
	//virtual Texture2DArray* addTexture2DArray(const std::string name, const std::vector<Texture2D*> textures) = 0;
	
	//virtual Texture3D* getTexture3D(const std::string name) = 0;
	//virtual Texture3D* addTexture3D(const std::string name, const std::string filename) = 0;
};

}
}

#endif /* ITEXTUREMANAGER_H_ */
