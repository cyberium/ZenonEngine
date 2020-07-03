#include "PropertyButton.h"

void QtnPropertyButton::invokeClick()
{
    emit click(this);
}

void QtnPropertyButton::setClickHandler(const std::function<void(const QtnPropertyButton*)>& clickHandler)
{
    QObject::connect(this, &QtnPropertyButton::click, clickHandler);
}

void QtnPropertyButton::invokePreDrawButton(QStyleOptionButton* option)
{
    Q_ASSERT(option);
    emit preDrawButton(this, option);
}

bool QtnPropertyButton::fromStrImpl(const QString& str)
{
    Q_UNUSED(str);
    return true;
}

bool QtnPropertyButton::toStrImpl(QString& str) const
{
    Q_UNUSED(str);
    return true;
}

