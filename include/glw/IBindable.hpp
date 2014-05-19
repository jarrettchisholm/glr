#ifndef IBINDABLE_H_
#define IBINDABLE_H_

namespace glr
{
namespace glw
{

class IBindable
{
public:
	virtual ~IBindable()
	{
	}
	;

	/**
	 * Will bind the current resource for use in an OpenGL shader program.
	 * 
	 * **Note**: This method is *not* thread safe - it should only be called from the OpenGL thread.
	 */
	virtual void bind() const = 0;
};

}
}

#endif /* IBINDABLE_H_ */
