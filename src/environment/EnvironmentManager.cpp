#include "environment/EnvironmentManager.hpp"

namespace glr
{
namespace env
{

EnvironmentManager::EnvironmentManager(glw::IOpenGlDevice* openGlDevice, models::IModelManager* modelManager)
	: openGlDevice_(openGlDevice), modelManager_(modelManager), followTarget_(nullptr)
{
	assert(openGlDevice_ != nullptr);
	assert(modelManager_ != nullptr);

	idManager_ = IdManager();
	skyBoxes_ = std::vector< std::unique_ptr<SkyBox> >();
}

EnvironmentManager::~EnvironmentManager()
{
}

ISky* EnvironmentManager::createSkyBox()
{
	skyBoxes_.push_back( std::unique_ptr<SkyBox>(new SkyBox(idManager_.createId(), openGlDevice_, modelManager_)) );

	return skyBoxes_.back().get();
}

ISky* EnvironmentManager::getSkyBox(Id id)
{
	auto findFunction = [&id](const std::unique_ptr<SkyBox>& node) { return node->getId() == id; };
	
	auto it = std::find_if(skyBoxes_.begin(), skyBoxes_.end(), findFunction);
	
	if (it != skyBoxes_.end())
		return it->get();

	return nullptr;
}

ISky* EnvironmentManager::getSkyBox(const std::string& name)
{
	auto findFunction = [&name](const std::unique_ptr<SkyBox>& node) { return node->getName() == name; };
	
	auto it = std::find_if(skyBoxes_.begin(), skyBoxes_.end(), findFunction);
	
	if (it != skyBoxes_.end())
		return it->get();

	return nullptr;
}

void EnvironmentManager::destroySkyBox(Id id)
{
	auto findFunction = [&id](const std::unique_ptr<SkyBox>& node) { return node->getId() == id; };
	
	auto it = std::find_if(skyBoxes_.begin(), skyBoxes_.end(), findFunction);
	
	if (it != skyBoxes_.end())
		skyBoxes_.erase(it);
}

void EnvironmentManager::destroySkyBox(const std::string& name)
{
	auto findFunction = [&name](const std::unique_ptr<SkyBox>& node) { return node->getName() == name; };
	
	auto it = std::find_if(skyBoxes_.begin(), skyBoxes_.end(), findFunction);
	
	if (it != skyBoxes_.end())
		skyBoxes_.erase(it);
}

void EnvironmentManager::destroySkyBox(ISky* skybox)
{
	auto findFunction = [skybox](const std::unique_ptr<SkyBox>& node) { return node.get() == skybox; };
	
	auto it = std::find_if(skyBoxes_.begin(), skyBoxes_.end(), findFunction);
	
	if (it != skyBoxes_.end())
		skyBoxes_.erase(it);
}

void EnvironmentManager::render()
{
	for ( auto& skybox : skyBoxes_ )
	{
		if (followTarget_ != nullptr)
			skybox->move( followTarget_->getPosition() );
		skybox->render();
	}
}

void EnvironmentManager::setFollowTarget(ISceneNode* target)
{
	followTarget_ = target;
}

}
}
