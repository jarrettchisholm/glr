namespace glr
{
namespace glw
{

template<class Archive> void Mesh::serialize(Archive& ar, const unsigned int version)
{
	boost::serialization::void_cast_register<Mesh, IMesh>(
		static_cast<Mesh*>(nullptr),
		static_cast<IMesh*>(nullptr)
	);
	//ar & boost::serialization::base_object<IMesh>(*this); 
	ar & name_;
	ar & vertices_;
	ar & normals_;
	ar & textureCoordinates_;
	ar & colors_;
	ar & vertexBoneData_;
	ar & boneData_;
}

}
}
