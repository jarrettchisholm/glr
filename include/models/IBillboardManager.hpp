#ifndef IBILLBOARDMANAGER_H_
#define IBILLBOARDMANAGER_H_

#include <string>

#include "models/IBillboard.hpp"

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
class IBillboardManager
{
public:
	virtual ~IBillboardManager()
	{
	}
	;

	/**
	 * Creates a billboard with name 'name' and the corresponding mesh, texture, and material data.
	 * 
	 * Note that the name does not have to be unique, but if it isn't, searching for the billboard by name will
	 * not work as expected.
	 * 
	 * @param name The name to give the new IBillboard object.
	 * @param mesh The mesh to use for the new IBillboard object.
	 * @param texture The texture to use for the new IBillboard object.
	 * @param material The material to use for the new IBillboard object.
	 * 
	 * 
	 * @return A pointer to an IBillboard object.
	 */
	virtual IBillboard* createBillboard(const std::string& name, glw::IMesh* mesh, glw::ITexture* texture, glw::IMaterial* material) = 0;

	/**
	 * Returns a pointer to an IBillboard object that has id 'id'.  If an IBillboard object with id 'id' is not found, this
	 * method returns nullptr.
	 * 
	 * @param id The id of the IBillboard template you want to get.
	 * 
	 * @return The raw pointer to an IBillboard object, or nullptr if it was not found.
	 */
	virtual IBillboard* getBillboard(Id id) = 0;
	
	/**
	 * Returns a pointer to an IBillboard object that has name 'name'.  If an IBillboard object with name 'name' is not found, this
	 * method returns nullptr.
	 * 
	 * If you have created billboards with duplicate names, this method will return the first instance of an IBillboard object it can find
	 * with a name that matches the 'name' parameter passed in.  Note that there is no guarantee in which order this method will search the IBillboard
	 * objects.
	 * 
	 * @param name The name of the IBillboard template you want to get.
	 * 
	 * @return The raw pointer to an IBillboard object, or nullptr if it was not found.
	 */
	virtual IBillboard* getBillboard(const std::string& name) = 0;
	
	/**
	 * Destroys the IBillboard object with id 'id'.  Any pointers or references to this billboard will
	 * become defunct.
	 * 
	 * If no IBillboard object is found with id 'id', this method does nothing.
	 * 
	 * @param id The id of the IBillboard you want to destroy.
	 */
	virtual void destroyBillboard(Id id) = 0;

	/**
	 * Destroys the IBillboard object with name 'name'.  Any pointers or references to this billboard will
	 * become defunct.
	 * 
	 * If an IBillboard object with name 'name' is not found, this method does nothing.
	 * 
	 * If you have created billboards with duplicate names, this method will destroy the first instance of an IBillboard object it can find
	 * with a name that matches the 'name' parameter passed in.  Note that there is no guarantee in which order this method will search the IBillboard
	 * objects.
	 * 
	 * @param name The name of the IBillboard you want to destroy.
	 */
	virtual void destroyBillboard(const std::string& name) = 0;
	
	/**
	 * Destroys the the IBillboard object 'billboard'.  Any pointers or references to this billboard will
	 * become defunct.
	 * 
	 *  If no IBillboard is found that points to the IBillboard passed in, this method does nothing.
	 * 
	 * @param model A pointer to the IBillboard you want to destroy.
	 */
	virtual void destroyBillboard(IBillboard* billboard) = 0;
};

}
}

#endif /* IBILLBOARDMANAGER_H_ */
