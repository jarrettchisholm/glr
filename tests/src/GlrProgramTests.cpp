#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "GlrInclude.hpp"

BOOST_AUTO_TEST_SUITE(glrProgram)

BOOST_AUTO_TEST_CASE(bindBuffer)
{
    glr::GlrProgram* p = new glr::GlrProgram();
    
    p->createWindow();
    
    // Using a less complicated algorithm for now...
    /*
    BOOST_CHECK_EQUAL( p->bindBuffer(1), 1 );
    BOOST_CHECK_EQUAL( p->bindBuffer(1), 1 );
    BOOST_CHECK_EQUAL( p->bindBuffer(4), 2 );
    BOOST_CHECK_EQUAL( p->bindBuffer(99), 3 );
    BOOST_CHECK_EQUAL( p->bindBuffer(4), 2 );
    */
    
    /*
    BOOST_CHECK_EQUAL( p->bindBuffer(1), 1 );
    BOOST_CHECK_EQUAL( p->bindBuffer(1), 2 );
    BOOST_CHECK_EQUAL( p->bindBuffer(2), 3 );
    BOOST_CHECK_EQUAL( p->bindBuffer(99), 4 );
    BOOST_CHECK_EQUAL( p->bindBuffer(7), 5 );
    */
    
    delete p;
}

BOOST_AUTO_TEST_SUITE_END()
