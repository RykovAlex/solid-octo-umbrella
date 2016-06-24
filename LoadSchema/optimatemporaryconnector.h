#pragma once
#include "optimaview.h"

class OptimaTemporaryConnector : public
{
public:
	OptimaTemporaryConnector(OptimaView* view, bool param2) :
		mParam1(param1),
		mParam2(param2)
	{
	}

	~OptimaTemporaryConnector()
	{
	}

protected:

private:
	OptimaView*			mParam1;
	bool			mParam2;

};


