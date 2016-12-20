#include "CallBackglBegin.hpp"

void mtl::gl::begin(GLenum what, std::function<void(void)> callback)
{
	glBegin(what);
	callback();
	glEnd();
}
