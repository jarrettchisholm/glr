namespace glr
{
namespace glw
{

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

}
}
