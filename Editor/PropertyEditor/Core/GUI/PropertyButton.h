#pragma once

#include "../Property.h"

class QStyleOptionButton;

class QTN_PE_CORE_EXPORT QtnPropertyButton: public QtnProperty
{
    Q_OBJECT
    QtnPropertyButton(const QtnPropertyButton& other) Q_DECL_EQ_DELETE;

public:
    explicit QtnPropertyButton(QObject *parent)
        : QtnProperty(parent)
    {
    }

    QtnPropertyButton& operator=(const QtnPropertyButton&)
    {
        // do nothing
        return *this;
    }

    void invokeClick();
    void setClickHandler(const std::function<void(const QtnPropertyButton*)>& clickHandler);

Q_SIGNALS:
    void click(const QtnPropertyButton* property);

public:
    void invokePreDrawButton(QStyleOptionButton* option);

Q_SIGNALS:
    void preDrawButton(const QtnPropertyButton* property, QStyleOptionButton* option);

protected:
    bool fromStrImpl(const QString& str) override;
    bool toStrImpl(QString& str) const override;

};
