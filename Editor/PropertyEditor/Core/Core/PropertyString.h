#pragma once

#include "../Auxiliary/PropertyTemplates.h"


Q_DECLARE_METATYPE(std::string)


class QTN_PE_CORE_EXPORT QtnPropertyStringBase: public QtnSinglePropertyBase<std::string>
{
    Q_OBJECT
    QtnPropertyStringBase(const QtnPropertyStringBase& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyStringBase(QObject *parent)
        : QtnSinglePropertyBase<std::string>(parent)
    {}

protected:
    // string conversion implementation
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

    P_PROPERTY_DECL_MEMBER_OPERATORS(QtnPropertyStringBase)
};



P_PROPERTY_DECL_ALL_OPERATORS(QtnPropertyStringBase, std::string)



class QTN_PE_CORE_EXPORT QtnPropertyStringCallback: public QtnSinglePropertyCallback<QtnPropertyStringBase>
{
    Q_OBJECT
    QtnPropertyStringCallback(const QtnPropertyStringCallback& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyStringCallback(QObject *parent)
        : QtnSinglePropertyCallback<QtnPropertyStringBase>(parent)
    {}

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyStringCallback, QtnPropertyStringBase)
};



class QTN_PE_CORE_EXPORT QtnPropertyString: public QtnSinglePropertyValue<QtnPropertyStringBase>
{
    Q_OBJECT
    QtnPropertyString(const QtnPropertyString& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyString(QObject *parent)
        : QtnSinglePropertyValue<QtnPropertyStringBase>(parent)
    {}

    P_PROPERTY_DECL_MEMBER_OPERATORS2(QtnPropertyString, QtnPropertyStringBase)
};
