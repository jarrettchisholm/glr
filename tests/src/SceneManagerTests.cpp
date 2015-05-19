#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include <memory>
#include <exception>
#include <utility>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "GlrInclude.hpp"


BOOST_AUTO_TEST_SUITE(sceneNodeManager)

BOOST_AUTO_TEST_CASE(createSceneNodeManager)
{
	//auto snm = std::make_unique<glr::BasicSceneManager>( nullptr, nullptr, nullptr, nullptr );
	
	auto p = std::unique_ptr<glr::GlrProgram>( new glr::GlrProgram() );
    
    p->createWindow();
    
    glr::ISceneManager* smgr = p->getSceneManager();
	
	
	// Single create/destroy test
	glr::ISceneNode* node = smgr->createSceneNode( std::string("test") );

	BOOST_REQUIRE( node != nullptr ); 
	BOOST_CHECK_EQUAL( smgr->getNumSceneNodes(), 1u );
	
	smgr->destroySceneNode( node );
	
	BOOST_CHECK_EQUAL( smgr->getNumSceneNodes(), 0u );
	
	// Make sure node was actually deleted
	node = smgr->getSceneNode( std::string("test") );
	
	BOOST_REQUIRE( node == nullptr );
	BOOST_CHECK_EQUAL( smgr->getNumSceneNodes(), 0u );
	
	
	// Create/destroy multiple scene nodes
	for ( int i=0; i < 30; i++ )
	{
		std::stringstream ss;
		ss << std::string("test_node_") << i;
		smgr->createSceneNode(  ss.str() );
	}
	
	BOOST_CHECK_EQUAL( smgr->getNumSceneNodes(), 30u );
	
	// Make sure we can get all of the scene nodes by name
	for ( int i=0; i < 30; i++ )
	{
		std::stringstream ss;
		ss << std::string("test_node_") << i;
		node = smgr->getSceneNode( ss.str() );
		BOOST_REQUIRE( node != nullptr );
	}
	
	// Make sure we don't get an erroneous node
	node = smgr->getSceneNode( std::string("test_node_30") );
	BOOST_REQUIRE( node == nullptr );
	
	// Test destroying one of the nodes
	smgr->destroySceneNode( std::string("test_node_4") );
	node = smgr->getSceneNode( std::string("test_node_4") );
	BOOST_REQUIRE( node == nullptr );
	
	BOOST_CHECK_EQUAL( smgr->getNumSceneNodes(), 29u );
	
	// Test destroying the remaining nodes
	smgr->destroyAllSceneNodes();
	
	BOOST_CHECK_EQUAL( smgr->getNumSceneNodes(), 0u );
}

BOOST_AUTO_TEST_SUITE_END()
