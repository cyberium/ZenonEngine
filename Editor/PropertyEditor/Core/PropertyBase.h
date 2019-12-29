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

#ifndef QTN_PROPERTY_BASE_H
#define QTN_PROPERTY_BASE_H

#include "Auxiliary/PropertyAux.h"
#include "Auxiliary/PropertyDelegateInfo.h"
#include <QVariant>
#include <functional>

class QScriptEngine;
class QtnPropertySet;
class QtnProperty;
class QtnPropertyDelegateInfoGetter;

class QTN_PE_CORE_EXPORT QtnPropertyBase: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QtnPropertyBase)

public:
    virtual ~QtnPropertyBase();

    QString name() const { return objectName(); }
    void setName(const QString& name);

    QString displayName() const { return m_displayName; }
    void setDisplayName(const QString& displayName);

    QString description() const { return m_description; }
    void setDescription(const QString& description);

    QtnPropertyID id() const { return m_id; }
    void setId(QtnPropertyID id);

    // states
    QtnPropertyState state() const { return m_stateLocal|m_stateInherited; }
    QtnPropertyState stateLocal() const { return m_stateLocal; }
    QtnPropertyState stateInherited() const { return m_stateInherited; }

    void setState(QtnPropertyState stateToSet, bool force = false);
    void addState(QtnPropertyState stateToAdd, bool force = false);
    void removeState(QtnPropertyState stateToRemove, bool force = false);
    void switchState(QtnPropertyState stateToSwitch, bool switchOn, bool force = false);
    void switchStateAuto(QtnPropertyState stateToSwitch, bool force = false);

    bool isEditable() const;
    bool isEditableByUser() const;
    bool isVisible() const;
    bool isSimple() const { return !m_stateLocal.testFlag(QtnPropertyStateNonSimple); }

    // string conversion
    bool fromStr(const QString& str);
    bool toStr(QString& str) const;

    // casts
    virtual QtnProperty* asProperty() { return nullptr; }
    virtual const QtnProperty* asProperty() const { return nullptr; }
    virtual QtnPropertySet* asPropertySet() { return nullptr; }
    virtual const QtnPropertySet* asPropertySet() const { return nullptr; }

    // delegates
    const QtnPropertyDelegateInfo* delegate() const;
    void setDelegate(const QtnPropertyDelegateInfo& delegate);
    void setDelegateCallback(const std::function<QtnPropertyDelegateInfo*()>& callback);

    void setDelegateAttribute(const QByteArray& attributeName, const QVariant& attributeValue);

    // reset callback
    bool hasResetCallback() const { return m_resetCallback != nullptr; }
    void setResetCallback(const std::function<void(QtnPropertyBase&)>& resetCallback);
    bool reset();

    static void connectMasterSignals(const QtnPropertyBase& masterProperty, QtnPropertyBase& slaveProperty);
    static void disconnectMasterSignals(const QtnPropertyBase& masterProperty, QtnPropertyBase& slaveProperty);

Q_SIGNALS:
    void propertyWillChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason, QtnPropertyValuePtr newValue);
    void propertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason);

protected:
    QtnPropertyBase(QObject* parent);

    // string conversion implementation
    virtual bool fromStrImpl(const QString& str) { Q_UNUSED(str); return false; }
    virtual bool toStrImpl(QString& str) const { Q_UNUSED(str); return false; }

    // inherited states support
    virtual void updateStateInherited(bool force) { Q_UNUSED(force); /* does nothing by default */ }
    void setStateInherited(QtnPropertyState stateToSet, bool force = false);

private:
    void masterPropertyWillChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason, QtnPropertyValuePtr newValue);
    void masterPropertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason);

    QString m_displayName;
    QString m_description;
    QtnPropertyID m_id;

    QtnPropertyState m_stateLocal;
    QtnPropertyState m_stateInherited;

    QScopedPointer<QtnPropertyDelegateInfoGetter> m_delegateInfoGetter;

    std::function<void(QtnPropertyBase&)> m_resetCallback;

    friend class QtnPropertySet;
};

Q_DECLARE_METATYPE(const QtnPropertyBase*)
Q_DECLARE_METATYPE(QtnPropertyBase*)

#endif // QTN_PROPERTY_BASE_H
