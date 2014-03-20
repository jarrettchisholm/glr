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

#include "../extras/FpsCamera.cpp"

BOOST_AUTO_TEST_SUITE(fpsCamera)

BOOST_AUTO_TEST_CASE(createFpsCamera)
{
	auto p = std::unique_ptr<glr::GlrProgram>( new glr::GlrProgram() );
	
	p->createWindow();
	
	glr::ISceneManager* smgr = p->getSceneManager();
	glr::ICamera* camera = smgr->createCamera();
	camera->setPosition(0, 0, 0);
	
	auto fpsCamera = std::unique_ptr< glr::extras::FpsCamera >( new glr::extras::FpsCamera(camera, 0.060f) );
	
	glm::vec3 pos = camera->getPosition();
	BOOST_CHECK_EQUAL( pos.x, 0.0f );
	BOOST_CHECK_EQUAL( pos.y, 0.0f );
	BOOST_CHECK_EQUAL( pos.z, 0.0f );
}

BOOST_AUTO_TEST_SUITE_END()
