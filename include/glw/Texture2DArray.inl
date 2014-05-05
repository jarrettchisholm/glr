namespace glr
{
namespace glw
{

template<class Archive> void Texture2DArray::serialize(Archive& ar, const unsigned int version)
{
	boost::serialization::void_cast_register<Texture2DArray, ITexture>(
		static_cast<Texture2DArray*>(nullptr),
		static_cast<ITexture*>(nullptr)
	);

	ar & name_;
	ar & images_;
}

}
}
