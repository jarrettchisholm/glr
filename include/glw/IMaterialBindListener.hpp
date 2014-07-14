#ifndef IMATERIALBINDLISTENER_H_
#define IMATERIALBINDLISTENER_H_

namespace glr
{
namespace glw
{

class IMaterial;

class IMaterialBindListener
{
public:
	virtual ~IMaterialBindListener()
	{
	}
	;

	virtual void materialBindCallback(IMaterial* material) = 0;
};

}
}

#endif /* IMATERIALBINDLISTENER_H_ */
