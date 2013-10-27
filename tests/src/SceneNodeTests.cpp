#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include <memory>
#include <exception>

#include "glm/glm.hpp"

#include "GlrInclude.h"


BOOST_AUTO_TEST_SUITE(sceneNode)

BOOST_AUTO_TEST_CASE(createDeleteSceneNodes)
{
    glr::GlrProgram* p = new glr::GlrProgram();
    
    p->createWindow();
    
    glr::ISceneManager* smgr = p->getSceneManager();
	
	
	// Single create/destroy test
	glr::ISceneNode* node = smgr->createSceneNode( std::string("test") );

	BOOST_REQUIRE( node != nullptr ); 
	BOOST_CHECK_EQUAL( smgr->getNumSceneNodes(), 1 );
	
	smgr->destroySceneNode( node );
	
	BOOST_CHECK_EQUAL( smgr->getNumSceneNodes(), 0 );
	
	// Make sure node was actually deleted
	node = smgr->getSceneNode( std::string("test") );
	
	BOOST_REQUIRE( node == nullptr );
	BOOST_CHECK_EQUAL( smgr->getNumSceneNodes(), 0 );
	
	
	// Create/destroy multiple scene nodes
	for ( int i=0; i < 30; i++ )
	{
		std::stringstream ss;
		ss << std::string("test_node_") << i;
		smgr->createSceneNode(  ss.str() );
	}
	
	BOOST_CHECK_EQUAL( smgr->getNumSceneNodes(), 30 );
	
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
	
	BOOST_CHECK_EQUAL( smgr->getNumSceneNodes(), 29 );
	
	// Test destroying the remaining nodes
	smgr->destroyAllSceneNodes();
	
	BOOST_CHECK_EQUAL( smgr->getNumSceneNodes(), 0 );
	
    delete p;    
}

BOOST_AUTO_TEST_CASE(sceneNodeMovement)
{
    glr::GlrProgram* p = new glr::GlrProgram();
    
    p->createWindow();
    
    glr::ISceneManager* smgr = p->getSceneManager();
	
	glr::ISceneNode* node = smgr->createSceneNode( std::string("test") );


	// Scale test
	node->setScale(0.1f, 0.1f, 0.1f);
	glm::vec3 scale = node->getScale();
	
	BOOST_CHECK_EQUAL( scale.x, 0.1f );
	BOOST_CHECK_EQUAL( scale.y, 0.1f );
	BOOST_CHECK_EQUAL( scale.z, 0.1f );
	
	
	// Position test
	node->setPosition(14.0f, 100.0f, 2.0f);
	glm::vec3 pos = node->getPosition();
	
	BOOST_CHECK_EQUAL( pos.x, 14.0f );
	BOOST_CHECK_EQUAL( pos.y, 100.0f );
	BOOST_CHECK_EQUAL( pos.z, 2.0f );
	
	
	// Translation test
	node->setPosition(100.0f, 100.0f, 100.0f);
	node->translate(1.0f, 1.0f, 1.0f);
	pos = node->getPosition();
	
	BOOST_CHECK_EQUAL( pos.x, 101.0f );
	BOOST_CHECK_EQUAL( pos.y, 101.0f );
	BOOST_CHECK_EQUAL( pos.z, 101.0f );
	
	node->translate( glm::vec3(-2.0f, -2.0f, -2.0f) );
	pos = node->getPosition();
	
	BOOST_CHECK_EQUAL( pos.x, 99.0f );
	BOOST_CHECK_EQUAL( pos.y, 99.0f );
	BOOST_CHECK_EQUAL( pos.z, 99.0f );
	
	
	// Rotation test
	node->rotate( glm::vec3(90.0f, 90.0f, 90.0f) );
	glm::vec3 rotation = node->getRotation();
	
	BOOST_CHECK_EQUAL( rotation.x, 90.0f );
	BOOST_CHECK_EQUAL( rotation.y, 90.0f );
	BOOST_CHECK_EQUAL( rotation.z, 90.0f );
	
	// TODO: think of more tests?
	
    delete p;    
}

BOOST_AUTO_TEST_SUITE_END()
