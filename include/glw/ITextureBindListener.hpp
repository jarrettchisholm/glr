#ifndef ITEXTUREBINDLISTENER_H_
#define ITEXTUREBINDLISTENER_H_

namespace glr
{
namespace glw
{

class ITexture;

class ITextureBindListener
{
public:
	virtual ~ITextureBindListener()
	{
	}
	;

	virtual void textureBindCallback(ITexture* texture) = 0;
};

}
}

#endif /* ITEXTUREBINDLISTENER_H_ */
