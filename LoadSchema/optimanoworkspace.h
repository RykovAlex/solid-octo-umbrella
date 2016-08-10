#pragma once
#include "optimaxml.h"

class OptimaNoWorkspace: public OptimaXml
{
public:
	OptimaNoWorkspace();

	~OptimaNoWorkspace();

	virtual void apply();

};



