#pragma once

struct SJoint
{

};

class ZN_API CGameModel
	: public ModelProxie
{
public:
	CGameModel(const std::shared_ptr<IModel>& Model);
	virtual ~CGameModel();

	// IModel
	virtual bool Render(const RenderEventArgs& renderEventArgs) const override;
	virtual void Accept(IVisitor* visitor) override;

};