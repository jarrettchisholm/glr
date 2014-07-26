#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include <memory>
#include <exception>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"

#include "GlrInclude.hpp"
#include "glw/MeshManager.hpp"
#include "glw/OpenGlDevice.hpp"
#include "exceptions/InvalidArgumentException.hpp"

BOOST_AUTO_TEST_SUITE(meshManager)

BOOST_AUTO_TEST_CASE(getMesh)
{
	auto p = std::unique_ptr<glr::GlrProgram>( new glr::GlrProgram() );
    p->createWindow();
    
	auto openGlDevice = std::unique_ptr<glr::glw::OpenGlDevice>( new glr::glw::OpenGlDevice() );
	auto meshManager = std::unique_ptr<glr::glw::MeshManager>( new glr::glw::MeshManager(openGlDevice.get()) );
	
	{
		auto ptr = meshManager->getMesh( std::string("") );
		BOOST_CHECK( ptr == nullptr );
	}
	
	{
		auto ptr = meshManager->addMesh( std::string("test_mesh") );
		BOOST_CHECK( ptr != nullptr );
		
		auto ptr2 = meshManager->getMesh( std::string("test_mesh") );
		BOOST_CHECK( ptr2 != nullptr );
		BOOST_CHECK( ptr == ptr2 );
	}

	for (int i=0; i < 100; i++)
	{
		const auto name = std::string("test_mesh_") + std::to_string(i);
		
		auto ptr = meshManager->addMesh( name );
		BOOST_CHECK( ptr != nullptr );
		
		auto ptr2 = meshManager->getMesh( name );
		BOOST_CHECK( ptr2 != nullptr );
		BOOST_CHECK( ptr == ptr2 );
		BOOST_CHECK( ptr2->getName() == name );
	}
}

BOOST_AUTO_TEST_CASE(addMesh)
{
	auto p = std::unique_ptr<glr::GlrProgram>( new glr::GlrProgram() );
    p->createWindow();
    
	auto openGlDevice = std::unique_ptr<glr::glw::OpenGlDevice>( new glr::glw::OpenGlDevice() );
	auto meshManager = std::unique_ptr<glr::glw::MeshManager>( new glr::glw::MeshManager(openGlDevice.get()) );
	
	{
		auto ptr = meshManager->addMesh( std::string("test_mesh") );
		BOOST_CHECK( ptr != nullptr );
		
		auto ptr2 = meshManager->getMesh( std::string("test_mesh") );
		BOOST_CHECK( ptr2 != nullptr );
		BOOST_CHECK( ptr == ptr2 );
		
		auto ptr3 = meshManager->getMesh( std::string("test_mesh") );
		BOOST_CHECK( ptr3 != nullptr );
		BOOST_CHECK( ptr == ptr3 );
	}
	
	for (int i=0; i < 100; i++)
	{
		const auto name = std::string("test_mesh_") + std::to_string(i);
		
		auto ptr = meshManager->addMesh( name );
		BOOST_CHECK( ptr != nullptr );
		
		auto ptr2 = meshManager->getMesh( name );
		BOOST_CHECK( ptr2 != nullptr );
		BOOST_CHECK( ptr == ptr2 );
		BOOST_CHECK( ptr2->getName() == name );
	}
	
	for (int i=0; i < 10; i++)
	{
		const auto name = std::string("test_mesh_with_data_") + std::to_string(i);
		
		auto ptr = meshManager->addMesh(
			name,
			std::vector< glm::vec3 >(),
			std::vector< glm::vec3 >(),
			std::vector< glm::vec2 >(),
			std::vector< glm::vec4 >()
		);
		BOOST_CHECK( ptr != nullptr );
		
		auto ptr2 = meshManager->getMesh( name );
		BOOST_CHECK( ptr2 != nullptr );
		BOOST_CHECK( ptr == ptr2 );
		BOOST_CHECK( ptr2->getName() == name );
	}
	
	for (int i=0; i < 10; i++)
	{
		const auto name = std::string("test_mesh_with_data_no_initialize_") + std::to_string(i);
		
		auto ptr = meshManager->addMesh(
			name,
			std::vector< glm::vec3 >(),
			std::vector< glm::vec3 >(),
			std::vector< glm::vec2 >(),
			std::vector< glm::vec4 >(),
			false
		);
		BOOST_CHECK( ptr != nullptr );
		
		auto ptr2 = meshManager->getMesh( name );
		BOOST_CHECK( ptr2 != nullptr );
		BOOST_CHECK( ptr == ptr2 );
		BOOST_CHECK( ptr2->getName() == name );
	}
	
	for (int i=0; i < 10; i++)
	{
		const auto name = std::string("test_mesh_with_bone_data_") + std::to_string(i);
		
		auto ptr = meshManager->addMesh(
			name,
			std::vector< glm::vec3 >(),
			std::vector< glm::vec3 >(),
			std::vector< glm::vec2 >(),
			std::vector< glm::vec4 >(),
			std::vector< glr::glw::VertexBoneData >(),
			glr::glw::BoneData()
		);
		BOOST_CHECK( ptr != nullptr );
		
		auto ptr2 = meshManager->getMesh( name );
		BOOST_CHECK( ptr2 != nullptr );
		BOOST_CHECK( ptr == ptr2 );
		BOOST_CHECK( ptr2->getName() == name );
	}
	
	for (int i=0; i < 10; i++)
	{
		const auto name = std::string("test_mesh_with_bone_data_no_initialize_") + std::to_string(i);
		
		auto ptr = meshManager->addMesh(
			name,
			std::vector< glm::vec3 >(),
			std::vector< glm::vec3 >(),
			std::vector< glm::vec2 >(),
			std::vector< glm::vec4 >(),
			std::vector< glr::glw::VertexBoneData >(),
			glr::glw::BoneData(),
			false
		);
		BOOST_CHECK( ptr != nullptr );
		
		auto ptr2 = meshManager->getMesh( name );
		BOOST_CHECK( ptr2 != nullptr );
		BOOST_CHECK( ptr == ptr2 );
		BOOST_CHECK( ptr2->getName() == name );
	}
}

BOOST_AUTO_TEST_CASE(destroyMesh)
{
	auto p = std::unique_ptr<glr::GlrProgram>( new glr::GlrProgram() );
    p->createWindow();
    
	auto openGlDevice = std::unique_ptr<glr::glw::OpenGlDevice>( new glr::glw::OpenGlDevice() );
	auto meshManager = std::unique_ptr<glr::glw::MeshManager>( new glr::glw::MeshManager(openGlDevice.get()) );
	
	{
		const std::string name = std::string("test_mesh");
		
		meshManager->addMesh( name );
		meshManager->destroyMesh( name );
		
		auto ptr = meshManager->getMesh( name );
		BOOST_CHECK( ptr == nullptr );
	}

	for (int i=0; i < 100; i++)
	{
		const auto name = std::string("test_mesh_") + std::to_string(i);
		
		meshManager->addMesh( name );
		meshManager->destroyMesh( name );
		
		auto ptr = meshManager->getMesh( name );
		BOOST_CHECK( ptr == nullptr );
	}
	
	for (int i=0; i < 10; i++)
	{
		const auto name = std::string("test_mesh_with_data_") + std::to_string(i);
		
		meshManager->addMesh(
			name,
			std::vector< glm::vec3 >(),
			std::vector< glm::vec3 >(),
			std::vector< glm::vec2 >(),
			std::vector< glm::vec4 >()
		);
		
		meshManager->destroyMesh( name );
			
		auto ptr = meshManager->getMesh( name );
		BOOST_CHECK( ptr == nullptr );
	}
	
	for (int i=0; i < 10; i++)
	{
		const auto name = std::string("test_mesh_with_data_no_initialize_") + std::to_string(i);
		
		meshManager->addMesh(
			name,
			std::vector< glm::vec3 >(),
			std::vector< glm::vec3 >(),
			std::vector< glm::vec2 >(),
			std::vector< glm::vec4 >(),
			false
		);
		
		meshManager->destroyMesh( name );
			
		auto ptr = meshManager->getMesh( name );
		BOOST_CHECK( ptr == nullptr );
	}
	
	for (int i=0; i < 10; i++)
	{
		const auto name = std::string("test_mesh_with_bone_data_") + std::to_string(i);
		
		meshManager->addMesh(
			name,
			std::vector< glm::vec3 >(),
			std::vector< glm::vec3 >(),
			std::vector< glm::vec2 >(),
			std::vector< glm::vec4 >(),
			std::vector< glr::glw::VertexBoneData >(),
			glr::glw::BoneData()
		);
		
		meshManager->destroyMesh( name );
			
		auto ptr = meshManager->getMesh( name );
		BOOST_CHECK( ptr == nullptr );
	}
	
	for (int i=0; i < 10; i++)
	{
		const auto name = std::string("test_mesh_with_bone_data_no_initialize_") + std::to_string(i);
		
		meshManager->addMesh(
			name,
			std::vector< glm::vec3 >(),
			std::vector< glm::vec3 >(),
			std::vector< glm::vec2 >(),
			std::vector< glm::vec4 >(),
			std::vector< glr::glw::VertexBoneData >(),
			glr::glw::BoneData(),
			false
		);
		meshManager->destroyMesh( name );
			
		auto ptr = meshManager->getMesh( name );
		BOOST_CHECK( ptr == nullptr );
	}
	
	for (int i=0; i < 10; i++)
	{
		const auto name = std::string("test_mesh_with_bone_data_") + std::to_string(i);
		
		auto ptr = meshManager->addMesh(
			name,
			std::vector< glm::vec3 >(),
			std::vector< glm::vec3 >(),
			std::vector< glm::vec2 >(),
			std::vector< glm::vec4 >(),
			std::vector< glr::glw::VertexBoneData >(),
			glr::glw::BoneData()
		);
		
		meshManager->destroyMesh( ptr );
			
		auto ptr2 = meshManager->getMesh( name );
		BOOST_CHECK( ptr2 == nullptr );
	}
	
	// Nullptr throws exception
	{
		bool threwException = false;
		
		try
		{
			meshManager->destroyMesh( nullptr );
		}
		catch (const glr::exception::InvalidArgumentException& e)
		{
			threwException = true;
		}
		
		BOOST_CHECK_EQUAL( threwException, true );
	}
	
	{
		meshManager->destroyMesh( std::string() );
	}
}

BOOST_AUTO_TEST_SUITE_END()
