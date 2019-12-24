#include "stdafx.h"

// General
#include "PropertyDelegateVec3.h"

// Additional
#include "../../../Core/Core/PropertyVec3.h"
#include "../PropertyDelegateFactory.h"

void regVec3Delegates(QtnPropertyDelegateFactory &factory)
{
    factory.registerDelegateDefault(&QtnPropertyVec3Base::staticMetaObject, &qtnCreateDelegate<QtnPropertyDelegateVec3, QtnPropertyVec3Base>, "XYZ");
}

QtnPropertyDelegateVec3::QtnPropertyDelegateVec3(QtnPropertyVec3Base& owner)
    : QtnPropertyDelegateTypedEx<QtnPropertyVec3Base>(owner)
{
    addSubProperty(qtnCreateXProperty(0, &owner));
    addSubProperty(qtnCreateYProperty(0, &owner));
	addSubProperty(qtnCreateZProperty(0, &owner));
}

QWidget* QtnPropertyDelegateVec3::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
    return createValueEditorLineEdit(parent, rect, true, inplaceInfo);
}

bool QtnPropertyDelegateVec3::propertyValueToStrImpl(QString& strValue) const
{
    glm::vec3 value = owner().value();
    strValue = QString("%1 x %2 x %3").arg(value.x).arg(value.y).arg(value.z);
    return true;
}
