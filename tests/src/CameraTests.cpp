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


BOOST_AUTO_TEST_SUITE(camera)

BOOST_AUTO_TEST_CASE(createDeleteCamera)
{
    auto p = std::unique_ptr<glr::GlrProgram>( new glr::GlrProgram() );
    
    p->createWindow();
	auto smgr = p->getSceneManager();
	
	// Single create/destroy test
	auto camera = smgr->createCamera();

	BOOST_REQUIRE( camera != nullptr ); 
}

BOOST_AUTO_TEST_CASE(cameraMovement)
{
    auto p = std::unique_ptr<glr::GlrProgram>( new glr::GlrProgram() );
    
    p->createWindow();
	auto smgr = p->getSceneManager();
	
	// Single create/destroy test
	auto camera = smgr->createCamera();
	
	// Position test
	camera->setPosition(14.0f, 100.0f, 2.0f);
	glm::vec3 pos = camera->getPosition();
	
	BOOST_CHECK( pos == glm::vec3(14.0f, 100.0f, 2.0f) );
	
	
	// Translation test
	camera->setPosition(100.0f, 100.0f, 100.0f);
	camera->translate(1.0f, 1.0f, 1.0f);
	pos = camera->getPosition();
	
	BOOST_CHECK( pos == glm::vec3(101.0f, 101.0f, 101.0f) );
	
	camera->translate( glm::vec3(-2.0f, -2.0f, -2.0f) );
	pos = camera->getPosition();
	
	BOOST_CHECK( pos == glm::vec3(99.0f, 99.0f, 99.0f) );
	
	
	// Rotation test
	float errorMargin = 0.000005f;
	camera->rotate( 90.0f, glm::vec3(1.0f, 0.0f, 0.0f) );
	camera->rotate( 90.0f, glm::vec3(0.0f, 1.0f, 0.0f) );
	camera->rotate( 90.0f, glm::vec3(0.0f, 0.0f, 1.0f) );
	
	glm::quat expectedQuat = glm::quat(0.707107f, 0.0f, 0.707107f, 0.0f);
	auto quat = camera->getOrientation();
	
	BOOST_CHECK_CLOSE_FRACTION( quat.w, expectedQuat.w, errorMargin );
	BOOST_CHECK(std::fabs(expectedQuat.x - quat.x) < errorMargin);
	BOOST_CHECK_CLOSE_FRACTION( quat.y, expectedQuat.y, errorMargin );
	BOOST_CHECK(std::fabs(expectedQuat.z - quat.z) < errorMargin);
}

BOOST_AUTO_TEST_CASE(cameraLookAt)
{
	auto camera = std::unique_ptr< glr::Camera >( new glr::Camera(glr::Id(1), nullptr) );
	camera->setPosition(0.0f, 0.0f, 1.0f);

	auto pos = camera->getPosition();
	BOOST_CHECK( pos == glm::vec3(0, 0, 1.0f) );

	auto q = camera->getOrientation();
	BOOST_CHECK( q == glm::quat(1,0,0,0) );

	camera->lookAt( glm::vec3(0.0f, 0.0f, 0.0f) );
	q = camera->getOrientation();
	
	float errorMargin = 0.000005f;
	BOOST_CHECK_CLOSE_FRACTION( q.w, 1.0f, errorMargin );
	BOOST_CHECK_CLOSE_FRACTION( q.x, 0.0f, errorMargin );
	BOOST_CHECK_CLOSE_FRACTION( q.y, 0.0f, errorMargin );
	BOOST_CHECK_CLOSE_FRACTION( q.z, 0.0f, errorMargin );

	pos = camera->getPosition();
	BOOST_CHECK( pos == glm::vec3(0, 0, 1.0f) );
}

BOOST_AUTO_TEST_CASE(cameraTick)
{
	auto camera = std::unique_ptr< glr::Camera >( new glr::Camera(glr::Id(1), nullptr) );
	camera->setPosition(0.0f, 0.0f, 1.0f);

	auto pos = camera->getPosition();
	BOOST_CHECK( pos == glm::vec3(0, 0, 1.0f) );

	auto q = camera->getOrientation();
	BOOST_CHECK( q == glm::quat(1,0,0,0) );

	camera->lookAt( glm::vec3(5.0f, 1.0f, -3.0f) );
	q = camera->getOrientation();
	
	float errorMargin = 0.000005f;
	BOOST_CHECK_CLOSE_FRACTION( q.w, 0.898601, errorMargin );
	BOOST_CHECK_CLOSE_FRACTION( q.x, -0.0697462, errorMargin );
	BOOST_CHECK_CLOSE_FRACTION( q.y, 0.43189, errorMargin );
	BOOST_CHECK_CLOSE_FRACTION( q.z, -0.0335218, errorMargin );

	camera->tick(0.33f);

	pos = camera->getPosition();
	BOOST_CHECK( pos == glm::vec3(0, 0, 1.0f) );
}

BOOST_AUTO_TEST_SUITE_END()
