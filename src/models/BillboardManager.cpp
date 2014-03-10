#include "models/BillboardManager.hpp"

#include "models/Billboard.hpp"

namespace glr
{
namespace models
{

BillboardManager::BillboardManager(glw::IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
	billboards_ = std::vector< std::unique_ptr<IBillboard> >();
	
	idManager_ = IdManager();
}

BillboardManager::~BillboardManager()
{
}

IBillboard* BillboardManager::createBillboard(const std::string& name, glw::IMesh* mesh, glw::ITexture* texture, glw::IMaterial* material)
{
	LOG_DEBUG( "Creating billboard using name '" << name << "'." );
	
	billboards_.push_back( std::unique_ptr<IBillboard>( new Billboard(idManager_.createId(), name, mesh, texture, material, openGlDevice_) ) );
	
	return billboards_.back().get();
}

IBillboard* BillboardManager::getBillboard(Id id) const
{
	LOG_DEBUG( "Retrieving billboard with id '" << id << "'." );

	auto findFunction = [id](const std::unique_ptr<IBillboard>& node) { return node->getId() == id; };
	
	auto it = std::find_if(billboards_.begin(), billboards_.end(), findFunction);
	if (it != billboards_.end())
	{
		LOG_DEBUG( "Billboard found." );
		return it->get();
	}

	LOG_DEBUG( "Billboard not found." );
	
	return nullptr;
}

IBillboard* BillboardManager::getBillboard(const std::string& name) const
{
	LOG_DEBUG( "Retrieving billboard with name '" << name << "'." );

	auto findFunction = [&name](const std::unique_ptr<IBillboard>& node) { return node->getName() == name; };
	
	auto it = std::find_if(billboards_.begin(), billboards_.end(), findFunction);
	if (it != billboards_.end())
	{
		LOG_DEBUG( "Billboard found." );
		return it->get();
	}

	LOG_DEBUG( "Billboard not found." );
	
	return nullptr;
}

void BillboardManager::destroyBillboard(Id id)
{
	LOG_DEBUG( "Destroying billboard with id '" << id << "'." );
	
	auto findFunction = [id](const std::unique_ptr<IBillboard>& node) { return node->getId() == id; };
	
	auto it = std::find_if(billboards_.begin(), billboards_.end(), findFunction);
	
	if (it != billboards_.end())
		billboards_.erase( it );
}

void BillboardManager::destroyBillboard(const std::string& name)
{
	LOG_DEBUG( "Destroying billboard with name '" << name << "'." );
	
	auto findFunction = [&name](const std::unique_ptr<IBillboard>& node) { return node->getName() == name; };
	
	auto it = std::find_if(billboards_.begin(), billboards_.end(), findFunction);
	
	if (it != billboards_.end())
		billboards_.erase( it );
}

void BillboardManager::destroyBillboard(IBillboard* billboard)
{
	LOG_DEBUG( "Destroying billboard by pointer with id '" << billboard->getId() << "'." );
	
	auto findFunction = [billboard](const std::unique_ptr<IBillboard>& node) { return node.get() == billboard; };
	
	auto it = std::find_if(billboards_.begin(), billboards_.end(), findFunction);
	
	if (it != billboards_.end())
		billboards_.erase( it );
}

}
}
