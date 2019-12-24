#include "stdafx.h"

// General
#include "PropertyVec3.h"

// Additional
#include "PropertyFloat.h"

QtnPropertyVec3Base::QtnPropertyVec3Base(QObject *parent)
    : QtnSinglePropertyBase<glm::vec3>(parent)
{
}

bool QtnPropertyVec3Base::fromStrImpl(const QString& str)
{
      return true;
}

bool QtnPropertyVec3Base::toStrImpl(QString& str) const
{
    return true;
}

QtnProperty* qtnCreateXProperty(QObject *parent, QtnPropertyVec3Base *propertyPoint)
{
	QtnPropertyFloatCallback *xProperty = new QtnPropertyFloatCallback(parent);
    xProperty->setName(QObject::tr("X"));
    xProperty->setDescription(QObject::tr("X coordinate of the %1.").arg(propertyPoint->name()));
    xProperty->setCallbackValueGet([propertyPoint]()->float { return propertyPoint->value().x; });
    xProperty->setCallbackValueSet([propertyPoint](float newX) 
	{
        glm::vec3 point = propertyPoint->value();
        point.x = newX;
        propertyPoint->setValue(point);
    });
	xProperty->setStepValue(0.1f);
    QtnPropertyBase::connectMasterSignals(*propertyPoint, *xProperty);

    return xProperty;
}

QtnProperty* qtnCreateYProperty(QObject *parent, QtnPropertyVec3Base *propertyPoint)
{
	QtnPropertyFloatCallback *yProperty = new QtnPropertyFloatCallback(parent);
    yProperty->setName(QObject::tr("Y"));
    yProperty->setDescription(QObject::tr("Y coordinate of the %1.").arg(propertyPoint->name()));
    yProperty->setCallbackValueGet([propertyPoint]()->int { return propertyPoint->value().y; });
    yProperty->setCallbackValueSet([propertyPoint](int newY) 
	{
		glm::vec3 point = propertyPoint->value();
        point.y = newY;
        propertyPoint->setValue(point);
    });
	yProperty->setStepValue(0.1f);
    QtnPropertyBase::connectMasterSignals(*propertyPoint, *yProperty);

    return yProperty;
}

QtnProperty* qtnCreateZProperty(QObject *parent, QtnPropertyVec3Base *propertyPoint)
{
	QtnPropertyFloatCallback *zProperty = new QtnPropertyFloatCallback(parent);
	zProperty->setName(QObject::tr("Z"));
	zProperty->setDescription(QObject::tr("Z coordinate of the %1.").arg(propertyPoint->name()));
	zProperty->setCallbackValueGet([propertyPoint]()->int { return propertyPoint->value().z; });
	zProperty->setCallbackValueSet([propertyPoint](int newZ) 
	{
		glm::vec3 point = propertyPoint->value();
		point.z = newZ;
		propertyPoint->setValue(point);
	});
	zProperty->setStepValue(0.1f);
	QtnPropertyBase::connectMasterSignals(*propertyPoint, *zProperty);

	return zProperty;
}