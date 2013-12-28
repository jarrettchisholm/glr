#ifndef IBINDABLE_H_
#define IBINDABLE_H_

namespace glr {
namespace glw {

class IBindable {
public:
	virtual ~IBindable()
	{
	}
	;

	virtual void bind() = 0;
};

}
}

#endif /* IBINDABLE_H_ */
