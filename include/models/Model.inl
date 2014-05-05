namespace glr
{
namespace models
{

template<class Archive> void Model::serialize(Archive& ar, const unsigned int version)
{
	boost::serialization::split_member(ar, *this, version);
}

}
}
