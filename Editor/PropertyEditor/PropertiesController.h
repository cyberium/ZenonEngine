#pragma once

#include "ui_MainEditor.h"

class CPropertiesController
	: public QObject
{
	Q_OBJECT
public:
	CPropertiesController(QtnPropertyWidget * PropertyWidget);
	virtual ~CPropertiesController();

	void OnSceneNodeSelected(ISceneNode3D* SceneNode);

public:
	void CreateProperty(QtnPropertySet* PropertiesSet, IProperty* Property);


private:
	QtnPropertyWidget * m_PropertyWidget;
};