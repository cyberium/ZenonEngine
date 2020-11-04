#pragma once

class CEditedScene
	: public SceneBase
{
public:
	CEditedScene(IBaseManager& BaseManager, IRenderWindow& RenderWindow);
	virtual ~CEditedScene();

	// SceneBase
	void Initialize() override;
	void Finalize() override;
};