#pragma once

#include "../Auxiliary/PropertyTemplates.h"

Q_DECLARE_METATYPE(glm::vec3)

class QTN_PE_CORE_EXPORT QtnPropertyVec3Base : public QtnSinglePropertyBase<glm::vec3>
{
	Q_OBJECT
	QtnPropertyVec3Base(const QtnPropertyVec3Base& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyVec3Base(QObject *parent);

protected:
	bool fromStrImpl(const QString& str) override;
	bool toStrImpl(QString& str) const override;

	P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyVec3Base)
};



P_PROPERTY_DECL_EQ_OPERATORS(QtnPropertyVec3Base, glm::vec3)



QTN_PE_CORE_EXPORT QtnProperty* qtnCreateXProperty(QObject *parent, QtnPropertyVec3Base *propertyPoint);
QTN_PE_CORE_EXPORT QtnProperty* qtnCreateYProperty(QObject *parent, QtnPropertyVec3Base *propertyPoint);
QTN_PE_CORE_EXPORT QtnProperty* qtnCreateZProperty(QObject *parent, QtnPropertyVec3Base *propertyPoint);



class QTN_PE_CORE_EXPORT QtnPropertyVec3Callback : public QtnSinglePropertyCallback<QtnPropertyVec3Base>
{
	Q_OBJECT
	QtnPropertyVec3Callback(const QtnPropertyVec3Callback& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyVec3Callback(QObject *parent)
		: QtnSinglePropertyCallback<QtnPropertyVec3Base>(parent)
	{}

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyVec3Callback, QtnPropertyVec3Base)
};



class QTN_PE_CORE_EXPORT QtnPropertyVec3 : public QtnSinglePropertyValue<QtnPropertyVec3Base>
{
	Q_OBJECT
	QtnPropertyVec3(const QtnPropertyVec3& other) Q_DECL_EQ_DELETE;

public:
	explicit QtnPropertyVec3(QObject *parent)
		: QtnSinglePropertyValue<QtnPropertyVec3Base>(parent)
	{}

	P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyVec3, QtnPropertyVec3Base)
};
