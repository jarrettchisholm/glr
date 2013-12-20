#ifndef IMATERIAL_H_
#define IMATERIAL_H_

namespace glr {
namespace glw {

class IMaterial {
public:
	virtual ~IMaterial()
	{
	}
	;

	virtual void bind() = 0;
	
	virtual GLuint getBufferId() = 0;
};

}
}

#endif /* IMATERIAL_H_ */
