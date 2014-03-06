#ifndef BILLBOARD_H_
#define BILLBOARD_H_

#include "models/IBillboard.hpp"

#include "glw/IOpenGlDevice.hpp"

#include "glw/IMesh.hpp"
#include "glw/ITexture.hpp"
#include "glw/IMaterial.hpp"

namespace glr
{
namespace models
{

/**
 * 
 */
class Billboard : public IBillboard
{
public:
	Billboard(Id id, const std::string& name, glw::IMesh* mesh, glw::ITexture* texture, glw::IMaterial* material, glw::IOpenGlDevice* openGlDevice);
	virtual ~Billboard();

	virtual const Id& getId() const;
	virtual const std::string& getName() const;

	virtual void render(shaders::IShaderProgram* shader);

private:
	Id id_;
	std::string name_;
	
	glw::IOpenGlDevice* openGlDevice_;
	
	glw::IMesh* mesh_;
	glw::ITexture* texture_;
	glw::IMaterial* material_;
};

}
}

#endif /* BILLBOARD_H_ */
