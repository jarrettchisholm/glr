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


BOOST_AUTO_TEST_SUITE(sceneNode)

BOOST_AUTO_TEST_CASE(createDeleteSceneNodes)
{
    
}

BOOST_AUTO_TEST_CASE(sceneNodeMovement)
{
    auto node = std::unique_ptr< glr::BasicSceneNode >( new glr::BasicSceneNode(glr::Id(1), nullptr) );

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
	float errorMargin = 0.000005f;
	node->rotate( 90.0f, glm::vec3(1.0f, 0.0f, 0.0f) );
	node->rotate( 90.0f, glm::vec3(0.0f, 1.0f, 0.0f) );
	node->rotate( 90.0f, glm::vec3(0.0f, 0.0f, 1.0f) );
	
	glm::quat expectedQuat = glm::quat(0.707107f, 0.0f, 0.707107f, 0.0f);
	auto quat = node->getOrientation();
	
	BOOST_CHECK_CLOSE_FRACTION( quat.w, expectedQuat.w, errorMargin );
	BOOST_CHECK(std::fabs(expectedQuat.x - quat.x) < errorMargin);
	BOOST_CHECK_CLOSE_FRACTION( quat.y, expectedQuat.y, errorMargin );
	BOOST_CHECK(std::fabs(expectedQuat.z - quat.z) < errorMargin);
	
	// TODO: think of more tests?
}

BOOST_AUTO_TEST_CASE(sceneNodeLookAt)
{
	auto n = std::unique_ptr< glr::BasicSceneNode >( new glr::BasicSceneNode(glr::Id(1), nullptr) );

	auto pos = n->getPosition();
	BOOST_CHECK( pos == glm::vec3(0,0,0) );

	auto q = n->getOrientation();
	BOOST_CHECK( q == glm::quat(1,0,0,0) );

	n->lookAt( glm::vec3(1,1,1) );
	q = n->getOrientation();
	
	float errorMargin = 0.000005f;
	BOOST_CHECK_CLOSE_FRACTION( q.w, 0.364705, errorMargin );
	BOOST_CHECK_CLOSE_FRACTION( q.x, -0.115917, errorMargin );
	BOOST_CHECK_CLOSE_FRACTION( q.y, 0.880476, errorMargin );
	BOOST_CHECK_CLOSE_FRACTION( q.z, -0.279848, errorMargin );

	pos = n->getPosition();
	BOOST_CHECK( pos == glm::vec3(0,0,0) );
}

BOOST_AUTO_TEST_SUITE_END()
