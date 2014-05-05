namespace glr
{
namespace glw
{

template<class Archive> void Animation::serialize(Archive& ar, const unsigned int version)
{
	boost::serialization::void_cast_register<Animation, IAnimation>(
		static_cast<Animation*>(nullptr),
		static_cast<IAnimation*>(nullptr)
	);

	ar & name_;
	ar & duration_;
	ar & ticksPerSecond_;
	ar & animatedBoneNodes_;
}

}
}
