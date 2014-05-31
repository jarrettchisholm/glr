#ifndef BILLBOARDMANAGER_H_
#define BILLBOARDMANAGER_H_

#include <mutex>

#include "models/IBillboardManager.hpp"

#include "glw/IOpenGlDevice.hpp"

#include "IdManager.hpp"

namespace glr
{
namespace models
{

/**
 * 
 */
class BillboardManager : public IBillboardManager
{
public:
	BillboardManager(glw::IOpenGlDevice* openGlDevice);
	virtual ~BillboardManager();

	virtual IBillboard* createBillboard(const std::string& name, glw::IMesh* mesh, glw::ITexture* texture, glw::IMaterial* material);

	virtual IBillboard* getBillboard(Id id) const;
	virtual IBillboard* getBillboard(const std::string& name) const;

	virtual void destroyBillboard(Id id);
	virtual void destroyBillboard(const std::string& name);
	virtual void destroyBillboard(IBillboard* billboard);

private:
	glw::IOpenGlDevice* openGlDevice_;
	
	std::vector< std::unique_ptr<IBillboard> > billboards_;
	
	IdManager idManager_;
	
	mutable std::mutex accessMutex_;
};

}
}

#endif /* BILLBOARDMANAGER_H_ */
