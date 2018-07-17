#ifndef METHOD_H_H
#define METHOD_H_H

#include "intrusiveptr.h"

namespace Vdl
{

class MethodRequest : public ReferenceCounter
{
public:
	virtual void call()=0;
};

}

#endif