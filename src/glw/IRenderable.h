#ifndef IRENDERABLE_H_
#define IRENDERABLE_H_

namespace glr
{
namespace glw
{

class IRenderable
{
public:
	virtual ~IRenderable()
	{
	}
	;

	virtual void render() = 0;
};

}
}

#endif /* IRENDERABLE_H_ */
