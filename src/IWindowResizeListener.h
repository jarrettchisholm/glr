#ifndef IWINDOWRESIZELISTENER_H_
#define IWINDOWRESIZELISTENER_H_

#include <string>
#include <glm/glm.hpp>

namespace glr {

class IWindowResizeListener {
public:
	virtual ~IWindowResizeListener()
	{
	}
	;

	virtual void windowSizeUpdate(glm::detail::uint32 width, glm::detail::uint32 height) = 0;
};

}

#endif /* IWINDOWRESIZELISTENER_H_ */
