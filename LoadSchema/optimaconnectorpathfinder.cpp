#include "stdafx.h"
#include "optimaconnectorpathfinder.h"
#include "tag.h"
#include "optimafigure.h"



OptimaConnectorPathFinder::OptimaConnectorPathFinder()
{

}

OptimaConnectorPathFinder::~OptimaConnectorPathFinder()
{

}

QRectF OptimaConnectorPathFinder::getMarkerRect(QPointF scenePos)
{
	return QRectF( scenePos - QPointF(3.0,3.0), scenePos + QPointF(3.0,3.0));
}

QGraphicsItem * OptimaConnectorPathFinder::findLinkedItem(const QGraphicsScene * scene, const QPointF & scenePos)
{
	if (scene == nullptr)
	{
		return nullptr;
	}

	QList<QGraphicsItem*> items = scene->items(scenePos);
	if (!items.isEmpty())
	{
		foreach(QGraphicsItem *item, items)
		{
			//учитаваем только фигуры и коннекторы, у них стоит признак что к ним можно прилинковываться		
			if( !item->data(tag::data::linkable).toBool() )
			{
				continue;
			}

			OptimaElement *element = dynamic_cast<OptimaElement*>(item);

			if (element )
			{
				//установим выделение элемента
				if (element->checkLinkedHighlight(scenePos))
				{
					return item;
				}
			}
		}
	}

	//фигуры надо проверять особенно, ведь их надо брать по описывающему контуру
	foreach(QGraphicsItem *item, scene->items())
	{
		if( !item->data(tag::data::linkable).toBool() )
		{
			continue;
		}		
		if (isFigure(item))
		{
			//QRectF itemRect = item->mapRectToScene(item->boundingRect());
			//if (itemRect.contains(scenePos))
			//	return item;

			//проверим выделилась ли часть элемента
			if (dynamic_cast<OptimaElement*>(item)->checkLinkedHighlight(scenePos))
			{
				return item;
			}
		}
	}

	return nullptr;
}

OptimaPointVector OptimaConnectorPathFinder::GetNewConnectorPoints_Free_Free(const OptimaPoint beginPoint, const OptimaPoint endPoint)
{
	OptimaPointVector pointVector;

	QLineF line(beginPoint, endPoint);
	QPointF center(line.pointAt(0.5));

	pointVector << beginPoint << QPointF(center.x(), beginPoint.y()) << QPointF(center.x(), endPoint.y()) << endPoint;

	return pointVector;
}

OptimaPointVector OptimaConnectorPathFinder::GetNewConnectorPoints_Free_Figure(const OptimaPoint beginPoint, const OptimaLinkedRect linkedRect)
{
	OptimaPointVector pointVector;

	QRectF sceneBoundingRect(linkedRect);
	OptimaLinkedRect::rectSide rectSide(linkedRect.side());

	QLineF diagonalLine(sceneBoundingRect.topLeft(), sceneBoundingRect.bottomRight());
	QPointF centerFigure(diagonalLine.pointAt(0.5));

	QLineF edgeLine(OptimaFigure::getEdgeLine(sceneBoundingRect, rectSide));
	QPointF centerEdgeLine(edgeLine.pointAt(0.5));
	QLineF finishLine(centerFigure, centerEdgeLine);
	finishLine.setLength(finishLine.length() + margin);

	QLineF line(beginPoint, finishLine.p2());
	QPointF center(line.pointAt(0.5));

	QLineF preFinishLine(QPointF(center.x(), finishLine.p2().y()), finishLine.p2());

	qreal finishAngle = preFinishLine.angleTo(finishLine);

	if (finishAngle == 0.0)
	{
		pointVector << beginPoint << QPointF(finishLine.p2().x(), beginPoint.y()) << finishLine.p2() << finishLine.p1();
	}
	else
	{
		pointVector << beginPoint << QPointF(center.x(), beginPoint.y()) << QPointF(center.x(), finishLine.p2().y()) << finishLine.p2() << finishLine.p1();
	}

	return pointVector;

}

OptimaPointVector OptimaConnectorPathFinder::GetNewConnectorPoints_Free_Connector(const OptimaPoint beginPoint, const OptimaLine lineConnector, const OptimaPoint endPoint)
{
	OptimaPointVector pointVector;

	QLineF line(beginPoint, endPoint);
	QPointF center(line.pointAt(0.5));

	pointVector << beginPoint << QPointF(center.x(), beginPoint.y()) << QPointF(center.x(), endPoint.y()) << endPoint;

	return pointVector;
}

OptimaPointVector OptimaConnectorPathFinder::GetNewConnectorPoints_Figure_Free(const OptimaLinkedRect beginLinkedRect, QPointF endPoint)
{
	OptimaPointVector pointVector;

	QLineF diagonalLineBegin(beginLinkedRect.topLeft(), beginLinkedRect.bottomRight());
	QPointF beginPoint(diagonalLineBegin.pointAt(0.5));

	QLineF line(beginPoint, endPoint);
	QPointF center(line.pointAt(0.5));

	pointVector << beginPoint << QPointF(center.x(), beginPoint.y()) << QPointF(center.x(), endPoint.y()) << endPoint;

	return pointVector;
}

OptimaPointVector OptimaConnectorPathFinder::GetNewConnectorPoints_Figure_Connector(const OptimaLinkedRect beginLinkedRect, const OptimaLine endLineConnector, const OptimaPoint endPoint)
{
	OptimaPointVector pointVector;

	QLineF diagonalLineBegin(beginLinkedRect.topLeft(), beginLinkedRect.bottomRight());
	QPointF beginPoint(diagonalLineBegin.pointAt(0.5));

	QLineF line(beginPoint, endPoint);
	QPointF center(line.pointAt(0.5));

	pointVector << beginPoint << QPointF(center.x(), beginPoint.y()) << QPointF(center.x(), endPoint.y()) << endPoint;

	return pointVector;
}

OptimaPointVector OptimaConnectorPathFinder::GetNewConnectorPoints_Figure_Figure(const OptimaLinkedRect beginLinkedRect, const OptimaLinkedRect endLinkedRect)
{
	OptimaPointVector pointVector;

	QLineF diagonalLineBegin(beginLinkedRect.topLeft(), beginLinkedRect.bottomRight());
	QPointF beginPoint(diagonalLineBegin.pointAt(0.5));

	QRectF sceneBoundingRect(endLinkedRect);
	OptimaLinkedRect::rectSide rectSide(endLinkedRect.side());

	QLineF diagonalLine(sceneBoundingRect.topLeft(), sceneBoundingRect.bottomRight());
	QPointF centerFigure(diagonalLine.pointAt(0.5));

	QLineF edgeLine(OptimaFigure::getEdgeLine(sceneBoundingRect, rectSide));
	QPointF centerEdgeLine(edgeLine.pointAt(0.5));
	QLineF finishLine(centerFigure, centerEdgeLine);
	finishLine.setLength(finishLine.length() + margin);

	QLineF line(beginPoint, finishLine.p2());
	QPointF center(line.pointAt(0.5));

	QLineF preFinishLine(QPointF(center.x(), finishLine.p2().y()), finishLine.p2());

	qreal finishAngle = preFinishLine.angleTo(finishLine);

	if (finishAngle == 0.0)
	{
		pointVector << beginPoint << QPointF(finishLine.p2().x(), beginPoint.y()) << finishLine.p2() << finishLine.p1();
	}
	else
	{
		pointVector << beginPoint << QPointF(center.x(), beginPoint.y()) << QPointF(center.x(), finishLine.p2().y()) << finishLine.p2() << finishLine.p1();
	}

	return pointVector;

}

OptimaPointVector OptimaConnectorPathFinder::GetNewConnectorPoints_Connector_Free(const OptimaLine lineConnector, const OptimaPoint beginPoint, const OptimaPoint endPoint)
{
	qDebug() << lineConnector;

	OptimaPointVector pointVector;

	QLineF line(beginPoint, endPoint);
	QPointF center(line.pointAt(0.5));

	pointVector << beginPoint << QPointF(center.x(), beginPoint.y()) << QPointF(center.x(), endPoint.y()) << endPoint;

	return pointVector;
}

OptimaPointVector OptimaConnectorPathFinder::GetNewConnectorPoints_Connector_Figure(const OptimaLine lineConnector, const OptimaPoint beginPoint, const OptimaLinkedRect linkedRect)
{
	OptimaPointVector pointVector;

	QRectF sceneBoundingRect(linkedRect);
	OptimaLinkedRect::rectSide rectSide(linkedRect.side());

	QLineF diagonalLine(sceneBoundingRect.topLeft(), sceneBoundingRect.bottomRight());
	QPointF centerFigure(diagonalLine.pointAt(0.5));

	QLineF edgeLine(OptimaFigure::getEdgeLine(sceneBoundingRect, rectSide));
	QPointF centerEdgeLine(edgeLine.pointAt(0.5));
	QLineF finishLine(centerFigure, centerEdgeLine);
	finishLine.setLength(finishLine.length() + margin);

	QLineF line(beginPoint, finishLine.p2());
	QPointF center(line.pointAt(0.5));

	QLineF preFinishLine(QPointF(center.x(), finishLine.p2().y()), finishLine.p2());

	qreal finishAngle = preFinishLine.angleTo(finishLine);

	if (finishAngle == 0.0)
	{
		pointVector << beginPoint << QPointF(finishLine.p2().x(), beginPoint.y()) << finishLine.p2() << finishLine.p1();
	}
	else
	{
		pointVector << beginPoint << QPointF(center.x(), beginPoint.y()) << QPointF(center.x(), finishLine.p2().y()) << finishLine.p2() << finishLine.p1();
	}

	return pointVector;
}

OptimaPointVector OptimaConnectorPathFinder::GetNewConnectorPoints_Connector_Connector(const OptimaLine beginLineConnector, const OptimaPoint beginPoint, const OptimaLine endLineConnector, const OptimaPoint endPoint)
{
	OptimaPointVector pointVector;

	QLineF line(beginPoint, endPoint);
	QPointF center(line.pointAt(0.5));

	pointVector << beginPoint << QPointF(center.x(), beginPoint.y()) << QPointF(center.x(), endPoint.y()) << endPoint;

	return pointVector;
}