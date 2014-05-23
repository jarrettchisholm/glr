#ifndef ANIMATIONMANAGER_H_
#define ANIMATIONMANAGER_H_

#include <string>
#include <memory>
#include <map>

#include "IAnimationManager.hpp"

#include "IOpenGlDevice.hpp"

#include "serialize/SplitMember.hpp"

namespace glr
{
namespace glw
{

class Animation;

class AnimationManager : public IAnimationManager
{
public:
	AnimationManager(IOpenGlDevice* openGlDevice);
	virtual ~AnimationManager();

	virtual Animation* getAnimation(const std::string& name) const;
	virtual Animation* addAnimation(const std::string& name, bool initialize = true);
	virtual Animation* addAnimation(const std::string& name, glm::detail::float64 duration, glm::detail::float64 ticksPerSecond, std::map< std::string, AnimatedBoneNode > animatedBoneNodes, bool initialize = true);
	
	virtual void serialize(const std::string& filename);
	virtual void serialize(serialize::TextOutArchive& outArchive);

	virtual void deserialize(const std::string& filename);
	virtual void deserialize(serialize::TextInArchive& inArchive);
	
private:
	/**
	 * Required by serialization.
	 */
	AnimationManager();

	IOpenGlDevice* openGlDevice_;
	
	std::map< std::string, std::unique_ptr<Animation> > animations_;
	
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

#endif /* ANIMATIONMANAGER_H_ */
