#pragma once

#include "../Utils/PropertyDelegateMisc.h"

class QtnPropertyStringBase;

class QTN_PW_EXPORT QtnPropertyDelegateQString: public QtnPropertyDelegateTyped<QtnPropertyStringBase>
{
    Q_DISABLE_COPY(QtnPropertyDelegateQString)

public:
    QtnPropertyDelegateQString(QtnPropertyStringBase& owner);

protected:
    bool acceptKeyPressedForInplaceEditImpl(QKeyEvent* keyEvent) const override;
    QWidget* createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo = nullptr) override;
    bool propertyValueToStrImpl(QString& strValue) const override;
};

class QTN_PW_EXPORT QtnPropertyDelegateQStringInvalidBase: public QtnPropertyDelegateQString
{
    Q_DISABLE_COPY(QtnPropertyDelegateQStringInvalidBase)

protected:
    QtnPropertyDelegateQStringInvalidBase(QtnPropertyStringBase& owner);

    void applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes) override;
    void drawValueImpl(QStylePainter& painter, const QRect& rect, const QStyle::State& state, bool* needTooltip = nullptr) const override;

    virtual bool isPropertyValid() const = 0;

private:
    QColor m_invalidColor;
};

class QTN_PW_EXPORT QtnPropertyDelegateQStringFile: public QtnPropertyDelegateQStringInvalidBase
{
    Q_DISABLE_COPY(QtnPropertyDelegateQStringFile)

public:
    QtnPropertyDelegateQStringFile(QtnPropertyStringBase& owner);

protected:
    void applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes) override;
    QWidget* createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo = nullptr) override;

    bool isPropertyValid() const override;

private:
    QtnPropertyDelegateAttributes m_editorAttributes;
};

class QTN_PW_EXPORT QtnPropertyDelegateQStringList: public QtnPropertyDelegateQString
{
    Q_DISABLE_COPY(QtnPropertyDelegateQStringList)

public:
    QtnPropertyDelegateQStringList(QtnPropertyStringBase& owner);

protected:
    void applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes) override;
    QWidget* createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo = nullptr) override;

private:
    QStringList m_items;
};


using QtnGetCandidatesFn = std::function<QStringList()>;
using QtnCreateCandidateFn = std::function<QString(QWidget*, QString)>;


Q_DECLARE_METATYPE(QtnGetCandidatesFn);
Q_DECLARE_METATYPE(QtnCreateCandidateFn);


class QTN_PW_EXPORT QtnPropertyDelegateQStringCallback: public QtnPropertyDelegateQString
{
    Q_DISABLE_COPY(QtnPropertyDelegateQStringCallback)

public:
    QtnPropertyDelegateQStringCallback(QtnPropertyStringBase& owner);

protected:
    void applyAttributesImpl(const QtnPropertyDelegateAttributes& attributes) override;
    QWidget* createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo = nullptr) override;

private:
    QtnPropertyDelegateAttributes m_editorAttributes;
};
