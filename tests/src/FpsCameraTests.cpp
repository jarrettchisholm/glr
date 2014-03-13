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

#include "GlrInclude.hpp"

#include "../extras/FpsCamera.cpp"

BOOST_AUTO_TEST_SUITE(fpsCamera)

BOOST_AUTO_TEST_CASE(createFpsCamera)
{
	auto p = std::unique_ptr<glr::GlrProgram>( new glr::GlrProgram() );
	
	p->createWindow();
	
	glr::ISceneManager* smgr = p->getSceneManager();
	
	auto camera = std::unique_ptr< glr::extras::FpsCamera >( new glr::extras::FpsCamera(p->getOpenGlDevice(), 0.020f) );
	camera->setPosition(0, 0, 0);
	
	glm::vec3 pos = camera->getPosition();
	BOOST_CHECK_EQUAL( pos.x, 0.0f );
	BOOST_CHECK_EQUAL( pos.y, 0.0f );
	BOOST_CHECK_EQUAL( pos.z, 0.0f );
	
	// TODO: This seems to be causing a memory access violation...not sure why.
	//smgr->addCamera( std::move(camera) );
}

BOOST_AUTO_TEST_SUITE_END()
