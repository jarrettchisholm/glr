#ifndef MACROS_H_
#define MACROS_H_

#define GETSET(Type, MemberName, FaceName) \
	Type get##FaceName() const { \
		return MemberName; \
	}; \
	\
	void set##FaceName(Type value) { \
		MemberName = value; \
	};
	
#define GETSET_INTERFACE(Type, FaceName) \
	virtual Type get##FaceName() const = 0; \
	\
	virtual void set##FaceName(Type value) = 0;


/*
#define GETSET(ClassName, Type, MemberName, FaceName) \
	Type ClassName::get##FaceName() const { \
		return MemberName; \
	}; \
	\
	void ClassName::set##FaceName(Type value) { \
		MemberName = value; \
	}
*/

#endif /* MACROS_H_ */
