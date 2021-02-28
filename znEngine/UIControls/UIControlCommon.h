#pragma once

class ZN_API CUIControlCommon
	: public CUIControl
	, public IUIControlCommon
{
public:
	CUIControlCommon(IScene& Scene);
	virtual ~CUIControlCommon();

	// CUIControl
	void Initialize() override;

	// IUIControlCommon
	void AddSubgeometry(const std::shared_ptr<IUIControlCommonModel>& Model) override;
	const std::vector<std::shared_ptr<IUIControlCommonModel>>& GetSubgeometries() const override;
	void ClearSubgeometries();

protected:
	EVisitResult AcceptContent(IVisitor* visitor);

protected:
	std::vector<std::shared_ptr<IUIControlCommonModel>>& GetSubgeometries();

private:
	std::vector<std::shared_ptr<IUIControlCommonModel>> m_Subgeometries;
};