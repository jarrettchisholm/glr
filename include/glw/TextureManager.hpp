#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include <memory>
#include <map>
#include <string>

#include "IOpenGlDevice.hpp"

#include "ITextureManager.hpp"

#include "serialize/SplitMember.hpp"

namespace glr
{
namespace glw
{

class TextureManager : public ITextureManager
{
public:
	TextureManager();
	TextureManager(IOpenGlDevice* openGlDevice);
	virtual ~TextureManager();

	virtual Texture2D* getTexture2D(const std::string& name) const;
	virtual Texture2DArray* getTexture2DArray(const std::string& name) const;
	
	virtual Texture2D* addTexture2D(const std::string& name, const TextureSettings settings = TextureSettings());
	virtual Texture2D* addTexture2D(const std::string& name, const std::string& filename, const TextureSettings settings = TextureSettings());
	virtual Texture2D* addTexture2D(const std::string& name, utilities::Image* image, const TextureSettings settings = TextureSettings());
	virtual Texture2DArray* addTexture2DArray(const std::string& name, const TextureSettings settings = TextureSettings());
	virtual Texture2DArray* addTexture2DArray(const std::string& name, const std::vector<std::string>& filenames, const TextureSettings settings = TextureSettings());
	virtual Texture2DArray* addTexture2DArray(const std::string& name, const std::vector<utilities::Image*>& images, const TextureSettings settings = TextureSettings());
	
	virtual void serialize(const std::string& filename);
	virtual void serialize(serialize::TextOutArchive& outArchive);

	virtual void deserialize(const std::string& filename);
	virtual void deserialize(serialize::TextInArchive& inArchive);
	
private:
	IOpenGlDevice* openGlDevice_;

	std::map< std::string, std::unique_ptr<Texture2D> > textures2D_;
	std::map< std::string, std::unique_ptr<Texture2DArray> > textures2DArray_;
	
	friend class boost::serialization::access;
	
	template<class Archive> void serialize(Archive& ar, const unsigned int version);
	// Need to do these because boost serialization doesn't have a standard implementation for std::unique_ptr
	// Apparently, std::unique_ptr will have a serializable implementation in boost 1.56
	// TODO: Implement one myself?
	template<class Archive> void save(Archive & ar, const unsigned int version) const;
	template<class Archive> void load(Archive & ar, const unsigned int version);
};

}
}

#endif /* TEXTUREMANAGER_H_ */
