#pragma once

#include "ui_EditorUIFrame.h"

class CPropertiesController
	: public QObject
{
	Q_OBJECT
public:
	CPropertiesController(QtnPropertyWidget * PropertyWidget);
	virtual ~CPropertiesController();

	void OnSceneNodeSelected(ISceneNode3D* SceneNode);

public:
	void CreateProperty(QtnPropertySet* PropertiesSet, const std::shared_ptr<IProperty>& Propert);

private:
	QtnPropertyWidget * m_PropertyWidget;
	QtnPropertySet* m_PropertiesSet;
};