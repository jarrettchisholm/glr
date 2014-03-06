#include "models/Billboard.hpp"

namespace glr
{
namespace models
{

Billboard::Billboard(Id id, const std::string& name, glw::IMesh* mesh, glw::ITexture* texture, glw::IMaterial* material, glw::IOpenGlDevice* openGlDevice)
	: id_(id), name_(name), mesh_(mesh), texture_(texture), material_(material), openGlDevice_(openGlDevice)
{
}

Billboard::~Billboard()
{
}

const Id& Billboard::getId() const
{
	return id_;
}

const std::string& Billboard::getName() const
{
	return name_;
}

void Billboard::render(shaders::IShaderProgram* shader)
{
	// TODO: Implement
}

}
}
