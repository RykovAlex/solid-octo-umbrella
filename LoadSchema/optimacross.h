#pragma once
#include "QtCore\QString"
#include "QtGui\QPainterPath"
#include "optimapoint.h"

class OptimaCross
{
public:
	OptimaCross();

	~OptimaCross()
	{
	}


	void apply(const QString &crossType);

	void draw(QPainterPath &path, const QLineF & originalLine, OptimaPoint::OptimaLengthVector crossingLengths) const;

protected:

private:
	enum ConnectorCrossType
	{
		connector_cross_break
		, connector_cross_bridge
		, connector_cross_full
	};

	ConnectorCrossType mCrossType;
};


