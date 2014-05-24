#include <sstream>
#include <utility>

#include "glw/Texture2D.hpp"

#include "exceptions/GlException.hpp"
#include "exceptions/FormatException.hpp"
#include "exceptions/InvalidArgumentException.hpp"

namespace glr
{
namespace glw
{

Texture2D::Texture2D() : internalFormat_(utilities::Format::FORMAT_UNKNOWN), bufferId_(0)
{
	openGlDevice_ = nullptr;
	name_ = std::string();
	settings_ = TextureSettings();
	
	isLocalDataLoaded_ = false;
	isDirty_ = false;
}

Texture2D::Texture2D(IOpenGlDevice* openGlDevice, std::string name, TextureSettings settings, bool initialize) : openGlDevice_(openGlDevice), name_(std::move(name)), settings_(std::move(settings)), internalFormat_(utilities::Format::FORMAT_UNKNOWN), bufferId_(0)
{
	isLocalDataLoaded_ = false;
	isDirty_ = false;
}

Texture2D::Texture2D(utilities::Image* image, IOpenGlDevice* openGlDevice, std::string name, TextureSettings settings, bool initialize) : openGlDevice_(openGlDevice), name_(std::move(name)), settings_(std::move(settings)), internalFormat_(utilities::Format::FORMAT_UNKNOWN), bufferId_(0)
{
	isLocalDataLoaded_ = false;
	isDirty_ = false;
	
	if ( image == nullptr )
	{
		std::string message = std::string("No image data sent into Texture2D constructor - use different constructor if no image data is available.");
		LOG_ERROR(message);
		throw exception::InvalidArgumentException(message);
	}
	
	// Copy image data
	setData( image );
	
	if (initialize)
	{
		allocateVideoMemory();
		pushToVideoMemory();
	}
}

Texture2D::~Texture2D()
{
}

void Texture2D::bind(GLuint texturePosition) const
{
	glActiveTexture(GL_TEXTURE0 + texturePosition);
	glBindTexture(GL_TEXTURE_2D, bufferId_);
	
	//bindPoint_ = openGlDevice_->bindBuffer( bufferId_ );
	//std::cout << "texture: " << name_ << " | " << bufferId_ << " | " << bindPoint_ << std::endl;
	// to unbind, we use the following
	//glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture2D::getBufferId() const
{
	return bufferId_;
}

GLuint Texture2D::getBindPoint() const
{
	return bindPoint_;
}

void Texture2D::setData(utilities::Image* image)
{
	image_ = *image;
	internalFormat_ = utilities::getOpenGlImageFormat(image_.format);
}

utilities::Image* Texture2D::getData()
{
	return &image_;
}

void Texture2D::pushToVideoMemory()
{
	bind();
	
	if (internalFormat_ == utilities::Format::FORMAT_UNKNOWN)
	{
		std::string msg = std::string( "Texture2D::pushToVideoMemory: Unknown image format." );
		LOG_ERROR( msg );
		throw exception::FormatException( msg );
	}
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,  image_.width, image_.height, internalFormat_, GL_UNSIGNED_BYTE, &(image_.data[0]));

	// error check
	GlError err = openGlDevice_->getGlError();
	if (err.type != GL_NONE)
	{
		// Cleanup
		freeVideoMemory();
		
		std::string msg = std::string( "Error while loading texture '" + name_ + "' in OpenGL: " + err.name);
		LOG_ERROR( msg );
		throw exception::GlException( msg );
	}
	else
	{
		LOG_DEBUG( "Successfully loaded texture." );
	}
}

void Texture2D::pullFromVideoMemory()
{
	
	// TODO: implement
	/*
	//bind();
	GLfloat* pixels = new GLfloat[size * size];
	glGetTexImage( GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, pixels );
	
	pixelDataVector.resize( size * size );
	for ( int i = 0; i < size * size; i++ )
	{
	  pixelDataVector[i] =  (float) pixels[i];
	}
	*/
}

void Texture2D::loadLocalData()
{
}

void Texture2D::freeLocalData()
{
	// Clear up the data, but leave the width, height, and format unchanged
	image_.data = std::vector<char>();
}

void Texture2D::freeVideoMemory()
{
	if (bufferId_ == 0)
	{
		LOG_WARN( "Cannot free video memory - buffer does not exist for texture." );
		return;
	}
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &bufferId_);
	
	bufferId_ = 0;
}

void Texture2D::allocateVideoMemory()
{
	if (bufferId_ != 0)
	{
		std::string msg = std::string( "Cannot allocate video memory - buffer already exists for texture.");
		LOG_ERROR( msg );
		throw exception::GlException( msg );
	}
	
	glGenTextures(1, &bufferId_);

	glBindTexture(GL_TEXTURE_2D, bufferId_);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, settings_.textureWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, settings_.textureWrapT);
	
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat_,  image_.width, image_.height, 0, internalFormat_, GL_UNSIGNED_BYTE, nullptr);
}

bool Texture2D::isVideoMemoryAllocated() const
{
	return (bufferId_ != 0);
}

bool Texture2D::isLocalDataLoaded() const
{
	return isLocalDataLoaded_;
}

bool Texture2D::isDirty() const
{
	return isDirty_;
}

const std::string& Texture2D::getName() const
{
	return name_;
}

void Texture2D::serialize(const std::string& filename)
{
	std::ofstream ofs(filename.c_str());
	serialize::TextOutArchive textOutArchive(ofs);
	serialize(textOutArchive);
}

void Texture2D::serialize(serialize::TextOutArchive& outArchive)
{
	outArchive << *this;
}

void Texture2D::deserialize(const std::string& filename)
{
	std::ifstream ifs(filename.c_str());
	serialize::TextInArchive textInArchive(ifs);
	deserialize(textInArchive);
}

void Texture2D::deserialize(serialize::TextInArchive& inArchive)
{
	inArchive >> *this;
}

/*
template<class Archive> void Texture2D::serialize(Archive& ar, const unsigned int version)
{
	boost::serialization::void_cast_register<Texture2D, ITexture>(
		static_cast<Texture2D*>(nullptr),
		static_cast<ITexture*>(nullptr)
	);

	ar & name_;
	ar & image_;
	ar & internalFormat_;
}
*/

}
}

BOOST_CLASS_EXPORT(glr::glw::ITexture)
BOOST_CLASS_EXPORT_GUID(glr::glw::Texture2D, "glr::glw::Texture2D")
