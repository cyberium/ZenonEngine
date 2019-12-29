/*
   Copyright (c) 2012-2016 Alex Zhondin <lexxmark.dev@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "PropertyBase.h"
#include <QDebug>

class QtnPropertyDelegateInfoGetter
{
    Q_DISABLE_COPY(QtnPropertyDelegateInfoGetter)

public:
    virtual QtnPropertyDelegateInfo* delegate() = 0;

    virtual ~QtnPropertyDelegateInfoGetter() {}

protected:
    QtnPropertyDelegateInfoGetter() {}
};

class QtnPropertyDelegateInfoGetterValue: public QtnPropertyDelegateInfoGetter
{
public:
    QtnPropertyDelegateInfoGetterValue(const QtnPropertyDelegateInfo& delegate)
        : m_delegate(delegate)
    {
    }

    QtnPropertyDelegateInfo* delegate() override
    {
        return &m_delegate;
    }

private:
    QtnPropertyDelegateInfo m_delegate;
};

class QtnPropertyDelegateInfoGetterCallback: public QtnPropertyDelegateInfoGetter
{
public:
    QtnPropertyDelegateInfoGetterCallback(const std::function<QtnPropertyDelegateInfo* ()>& callback)
        : m_callback(callback)
    {
    }

    QtnPropertyDelegateInfo* delegate() override
    {
        if (m_delegate.isNull())
        {
            m_delegate.reset(m_callback());
        }

        return m_delegate.data();
    }

private:
    std::function<QtnPropertyDelegateInfo* ()> m_callback;
    mutable QScopedPointer<QtnPropertyDelegateInfo> m_delegate;
};


static int qtnPropertyChangeReasonMetaId = qRegisterMetaType<QtnPropertyChangeReason>("QtnPropertyChangeReason");
static int qtnPropertyPtrId = qRegisterMetaType<QtnPropertyBase*>("QtnPropertyBase*");
static quint16 qtnPropertyMagicNumber = 0x1984;

// extern declaration
void qtnAddPropertyAsChild(QObject* parent, QtnPropertyBase* child, bool moveOwnership);
void qtnRemovePropertyAsChild(QObject* parent, QtnPropertyBase* child);


QtnPropertyBase::QtnPropertyBase(QObject *parent)
    : QObject(parent),
      m_id(QtnPropertyIDInvalid),
      m_stateLocal(QtnPropertyStateNone),
      m_stateInherited(QtnPropertyStateNone)
{
//    qtnAddPropertyAsChild(parent, this, false);
}

QtnPropertyBase::~QtnPropertyBase()
{
    qtnRemovePropertyAsChild(parent(), this);
}

void QtnPropertyBase::setName(const QString& name)
{
    if (name.contains(' ') || name.contains('.'))
    {
        // cannot set non-identifier name
        // use displayName instead
        Q_ASSERT(false);
        return;
    }

    Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonDisplayName|QtnPropertyChangeReasonName, QtnPropertyValuePtr(&name));

    setObjectName(name);
    m_displayName = name;

    Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonDisplayName|QtnPropertyChangeReasonName);
}

void QtnPropertyBase::setDisplayName(const QString& displayName)
{
    Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonDisplayName, QtnPropertyValuePtr(&displayName));

    m_displayName = displayName;

    Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonDisplayName);
}

void QtnPropertyBase::setDescription(const QString& description)
{
    Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonDescription, QtnPropertyValuePtr(&description));

    m_description = description;

    Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonDescription);
}

void QtnPropertyBase::setId(QtnPropertyID id)
{
    Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonId, QtnPropertyValuePtr(&id));

    m_id = id;

    Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonId);
}

void QtnPropertyBase::setState(QtnPropertyState stateToSet, bool force)
{
    if (!force && (m_stateLocal == stateToSet))
        return;

    Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonStateLocal, QtnPropertyValuePtr(&stateToSet));

    m_stateLocal = stateToSet;

    Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonStateLocal);

    updateStateInherited(force);
}

void QtnPropertyBase::addState(QtnPropertyState stateToAdd, bool force)
{
    QtnPropertyState stateToSet = m_stateLocal|stateToAdd;

    if (!force && (m_stateLocal == stateToSet))
        return;

    Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonStateLocal, QtnPropertyValuePtr(&stateToSet));

    m_stateLocal = stateToSet;

    Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonStateLocal);

    updateStateInherited(force);
}

void QtnPropertyBase::removeState(QtnPropertyState stateToRemove, bool force)
{
    QtnPropertyState stateToSet = m_stateLocal&~stateToRemove;

    if (!force && (m_stateLocal == stateToSet))
        return;

    Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonStateLocal, QtnPropertyValuePtr(&stateToSet));

    m_stateLocal = stateToSet;

    Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonStateLocal);

    updateStateInherited(force);
}

void QtnPropertyBase::switchState(QtnPropertyState stateToSwitch, bool switchOn, bool force)
{
    if (switchOn)
        addState(stateToSwitch, force);
    else
        removeState(stateToSwitch, force);
}

void QtnPropertyBase::switchStateAuto(QtnPropertyState stateToSwitch, bool force)
{
    if (!(stateLocal() & stateToSwitch))
        addState(stateToSwitch, force);
    else
        removeState(stateToSwitch, force);
}

bool QtnPropertyBase::isEditable() const
{
    return !(state()&(QtnPropertyStateImmutable));
}

bool QtnPropertyBase::isEditableByUser() const
{
    return !(state()&(QtnPropertyStateImmutable|QtnPropertyStateInvisible));
}

bool QtnPropertyBase::isVisible() const
{
    return !(state()&(QtnPropertyStateInvisible));
}

bool QtnPropertyBase::fromStr(const QString& str)
{
    if (!isEditable())
        return true;

    QString trimmedStr = str.trimmed();
    return fromStrImpl(trimmedStr);
}

bool QtnPropertyBase::toStr(QString& str) const
{
    return toStrImpl(str);
}

const QtnPropertyDelegateInfo* QtnPropertyBase::delegate() const
{
    if (m_delegateInfoGetter.isNull())
        return 0;

    return m_delegateInfoGetter->delegate();
}

void QtnPropertyBase::setDelegate(const QtnPropertyDelegateInfo& delegate)
{
    m_delegateInfoGetter.reset(new QtnPropertyDelegateInfoGetterValue(delegate));
}

void QtnPropertyBase::setDelegateCallback(const std::function<QtnPropertyDelegateInfo*()>& callback)
{
    m_delegateInfoGetter.reset(new QtnPropertyDelegateInfoGetterCallback(callback));
}

void QtnPropertyBase::setDelegateAttribute(const QByteArray& attributeName, const QVariant& attributeValue)
{
    if (m_delegateInfoGetter.isNull())
    {
        Q_ASSERT(false);
        return;
    }

    auto delegate = m_delegateInfoGetter->delegate();
    delegate->attributes[attributeName] = attributeValue;
}

void QtnPropertyBase::setResetCallback(const std::function<void(QtnPropertyBase&)>& resetCallback)
{
    m_resetCallback = resetCallback;
}

bool QtnPropertyBase::reset()
{
    if (!m_resetCallback)
        return false;

    m_resetCallback(*this);
    return true;
}


void QtnPropertyBase::connectMasterSignals(const QtnPropertyBase& masterProperty, QtnPropertyBase& slaveProperty)
{
    slaveProperty.setStateInherited(masterProperty.state());
    QObject::connect(&masterProperty, &QtnPropertyBase::propertyWillChange, &slaveProperty, &QtnPropertyBase::masterPropertyWillChange);
    QObject::connect(&masterProperty, &QtnPropertyBase::propertyDidChange, &slaveProperty, &QtnPropertyBase::masterPropertyDidChange);
}

void QtnPropertyBase::disconnectMasterSignals(const QtnPropertyBase& masterProperty, QtnPropertyBase& slaveProperty)
{
    QObject::disconnect(&masterProperty, &QtnPropertyBase::propertyWillChange, &slaveProperty, &QtnPropertyBase::masterPropertyWillChange);
    QObject::disconnect(&masterProperty, &QtnPropertyBase::propertyDidChange, &slaveProperty, &QtnPropertyBase::masterPropertyDidChange);
}

void QtnPropertyBase::setStateInherited(QtnPropertyState stateToSet, bool force)
{
    if (!force && (m_stateInherited == stateToSet))
        return;

    Q_EMIT propertyWillChange(this, this, QtnPropertyChangeReasonStateInherited, QtnPropertyValuePtr(&stateToSet));

    m_stateInherited = stateToSet;

    Q_EMIT propertyDidChange(this, this, QtnPropertyChangeReasonStateInherited);

    updateStateInherited(force);
}

void QtnPropertyBase::masterPropertyWillChange(const QtnPropertyBase*, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason, QtnPropertyValuePtr)
{
    if (reason & QtnPropertyChangeReasonValue)
        Q_EMIT propertyWillChange(this, firedProperty, reason, nullptr);
}

void QtnPropertyBase::masterPropertyDidChange(const QtnPropertyBase *changedProperty, const QtnPropertyBase *firedProperty, QtnPropertyChangeReason reason)
{
    // state has changed and not from child property
    if ((reason & QtnPropertyChangeReasonState) && (changedProperty == firedProperty))
    {
        setStateInherited(changedProperty->state());
    }

    if (reason & QtnPropertyChangeReasonValue)
        Q_EMIT propertyDidChange(this, firedProperty, reason);
}
