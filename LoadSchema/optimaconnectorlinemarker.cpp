#include "stdafx.h"
#include "optimaconnectorlinemarker.h"
#include "OptimaConnectorMarker.h"
#include "optimaconnector.h"
#include "optimaconnectorbordermarkerend.h"
#include "optimaconnectorbordermarkerbegin.h"

OptimaConnectorLineMarker::OptimaConnectorLineMarker(OptimaConnector * parentConnector, const OptimaLine &line, Qt::CursorShape cursorShape) 
	: OptimaConnectorMarker(parentConnector, line.center(), cursorShape, tag::undo::line_start_index + line.getLineIndex() )
	, mLine(line)
{
	QList<QGraphicsItem *> items = parentConnector->childItems();
	
	Q_ASSERT(items.count() >= 2);
	//������� � ���������� ���������� �������� ������
	mConnectorEndMarker = dynamic_cast<OptimaConnectorBorderMarkerEnd*>(items.at(0));
	//����� ���������
	mConnectorBeginMarker = dynamic_cast<OptimaConnectorBorderMarkerBegin*>(items.at(1));
	
	//������� ������ ���� ������ � ����� �������
	Q_ASSERT(mConnectorBeginMarker != nullptr);
	Q_ASSERT(mConnectorEndMarker != nullptr);
}

int OptimaConnectorLineMarker::getLineIndex() const
{
	return mLine.getLineIndex();
}

OptimaPoint OptimaConnectorLineMarker::p1() const
{
	return mLine[OptimaLine::point1];
}

OptimaPoint OptimaConnectorLineMarker::p2() const
{
	return mLine[OptimaLine::point2];
}

OptimaPointVector OptimaConnectorLineMarker::getNewPoints()
{
	OptimaPointVector points( mNewConnector->points() );
	int indexLine(getLineIndex());

	OptimaPointVector pointVector;

	for (int i = 0; i < points.size() && i < indexLine; ++i)
	{
		pointVector << points.at(i);
	}

	pointVector << mLine;

	for (int i = indexLine + 2; i < points.size(); ++i)
	{
		pointVector << points.at(i);
	}

	return pointVector;
}

void OptimaConnectorLineMarker::setMarkerPos(const QPointF & scenePos)
{
	switch(mCursor.shape())
	{
	case Qt::SizeHorCursor:
		mLine.setP1(QPointF(scenePos.x(), mLine.p1().y()));
		mLine.setP2(QPointF(scenePos.x(), mLine.p2().y()));
		break;
	case Qt::SizeVerCursor:
		mLine.setP1(QPointF(mLine.p1().x(), scenePos.y()));
		mLine.setP2(QPointF(mLine.p2().x(), scenePos.y()));
		break;
	default:
		Q_ASSERT( false );
	}
	setPos(scenePos);

	//���������� ���������� ��� ���, �������� ���, ������ �� ����� ����������, ��� ���������� ������������ 
	//��� ��������� ����� ���������
	if (mNewConnector == nullptr)
	{
		OptimaConnector *connector = qgraphicsitem_cast<OptimaConnector*>(parentItem());

		//�������� ������������ ���������
		mNewConnector = new OptimaTemporaryConnector(connector);	
	}
	
	setNewConnectorPoints();
}

void OptimaConnectorLineMarker::setNewConnectorPoints()
{
	OptimaFigure *beginFigure = dynamic_cast<OptimaFigure*>(mConnectorBeginMarker->getLinkedElement());
	OptimaFigure *endFigure = dynamic_cast<OptimaFigure*>(mConnectorEndMarker->getLinkedElement());
	OptimaConnector *beginConnector = dynamic_cast<OptimaConnector*>(mConnectorBeginMarker->getLinkedElement());
	OptimaConnector *endConnector = dynamic_cast<OptimaConnector*>(mConnectorEndMarker->getLinkedElement());
	OptimaLine linkedLineBegin;
	OptimaLine linkedLineEnd;

	OptimaPointVector newPoints;

	//������ ����� ������ ����� ������� ���� ������� ��� ����������� ���� ���������� ��
	//��������� � �������� �����
	switch(getRelationship())
	{
	case OptimaTemporaryConnector::free_free:
		newPoints = OptimaConnectorPathFinder::GetResizableConnectorPoints_Free_Free( getNewPoints(), getLineIndex());
		break;
	case OptimaTemporaryConnector::free_figure:
		Q_ASSERT(endFigure != nullptr);

		newPoints = OptimaConnectorPathFinder::GetResizableConnectorPoints_Free_Figure( getNewPoints(), getLineIndex(), endFigure->sceneBoundingRect());
		break;
	case OptimaTemporaryConnector::figure_free:
		Q_ASSERT(beginFigure != nullptr);

		newPoints = OptimaConnectorPathFinder::GetResizableConnectorPoints_Free_Figure( getNewPoints(), getLineIndex(), beginFigure->sceneBoundingRect());
		break;
	case OptimaTemporaryConnector::figure_figure:
		Q_ASSERT(beginFigure != nullptr);
		Q_ASSERT(endFigure != nullptr);

		newPoints = OptimaConnectorPathFinder::GetResizableConnectorPoints_Figure_Figure( getNewPoints(), getLineIndex(), beginFigure->sceneBoundingRect(), endFigure->sceneBoundingRect());
		break;
	case OptimaTemporaryConnector::free_connector:
		Q_ASSERT(endConnector != nullptr);
		
		Q_ASSERT(endConnector->getLinkedLine(mConnectorEndMarker->scenePos(), linkedLineEnd));

		newPoints = OptimaConnectorPathFinder::GetResizableConnectorPoints_Free_Connector( getNewPoints(), getLineIndex(), linkedLineEnd);
		break;	
	case OptimaTemporaryConnector::connector_connector:
		Q_ASSERT(beginConnector != nullptr);
		Q_ASSERT(beginConnector->getLinkedLine(mConnectorBeginMarker->scenePos(), linkedLineBegin));
		Q_ASSERT(endConnector != nullptr);
		Q_ASSERT(endConnector->getLinkedLine(mConnectorEndMarker->scenePos(), linkedLineEnd));

		newPoints = OptimaConnectorPathFinder::GetResizableConnectorPoints_Connector_Connector( getNewPoints(), getLineIndex(), linkedLineBegin, linkedLineEnd);
		break;
	case OptimaTemporaryConnector::connector_free:
		Q_ASSERT(beginConnector != nullptr);
		Q_ASSERT(beginConnector->getLinkedLine(mConnectorBeginMarker->scenePos(), linkedLineBegin));

		newPoints = OptimaConnectorPathFinder::GetResizableConnectorPoints_Free_Connector( getNewPoints(), getLineIndex(), linkedLineBegin);
		break;
	case OptimaTemporaryConnector::connector_figure:
		Q_ASSERT(beginConnector != nullptr);
		Q_ASSERT(beginConnector->getLinkedLine(mConnectorBeginMarker->scenePos(), linkedLineBegin));
		Q_ASSERT(endFigure != nullptr);

		newPoints = OptimaConnectorPathFinder::GetResizableConnectorPoints_Connector_Figure( getNewPoints(), getLineIndex(), linkedLineBegin, endFigure->sceneBoundingRect());
		break;
	case OptimaTemporaryConnector::figure_connector:
		Q_ASSERT(beginFigure != nullptr);
		Q_ASSERT(endConnector != nullptr);
		Q_ASSERT(endConnector->getLinkedLine(mConnectorEndMarker->scenePos(), linkedLineEnd));

		newPoints = OptimaConnectorPathFinder::GetResizableConnectorPoints_Figure_Connector( getNewPoints(), getLineIndex(), beginFigure->sceneBoundingRect(), linkedLineEnd);
		break;
	}

	mNewConnector->setPoints(newPoints);
}
