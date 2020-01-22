#pragma once

#include "ui_MainEditor.h"

class CPropertiesController
	: QObject
{
	Q_OBJECT
public:
	CPropertiesController(QtnPropertyWidget * PropertyWidget);
	virtual ~CPropertiesController();

	void SceneNodeSelected(std::shared_ptr<ISceneNode3D> SceneNode);

public:
	void CreateProperty(QtnPropertySet* PropertiesSet, std::shared_ptr<IProperty> Property);


private:
	QtnPropertyWidget * m_PropertyWidget;
};