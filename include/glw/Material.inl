namespace glr
{
namespace glw
{

template<class Archive> void Material::serialize(Archive& ar, const unsigned int version)
{
	boost::serialization::void_cast_register<Material, IMaterial>(
		static_cast<Material*>(nullptr),
		static_cast<IMaterial*>(nullptr)
	);

	ar & name_;
	ar & ambient_;
	ar & diffuse_;
	ar & specular_;
	ar & emission_;
	ar & shininess_;
	ar & strength_;
}

}
}
