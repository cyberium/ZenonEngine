#pragma once

#include "../Utils/PropertyDelegateMisc.h"

class QtnPropertyVec3Base;

class QTN_PW_EXPORT QtnPropertyDelegateVec3: public QtnPropertyDelegateTypedEx<QtnPropertyVec3Base>
{
    Q_DISABLE_COPY(QtnPropertyDelegateVec3)

public:
	QtnPropertyDelegateVec3(QtnPropertyVec3Base& owner);

protected:
    QWidget* createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo = nullptr) override;
    bool propertyValueToStrImpl(QString& strValue) const override;
};
