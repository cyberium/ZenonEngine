#include "stdafx.h"

// General
#include "ContextMenuUtils.h"

void CreateContextMenuFromPropertyRecursive(QMenu* OwnerMenu, std::shared_ptr<IProperty> Property)
{
	if (auto propertyAsGroup = std::dynamic_pointer_cast<IPropertiesGroup>(Property))
	{
		QMenu* subMenu = new QMenu(propertyAsGroup->GetName().c_str(), OwnerMenu);

		for (const auto childProperty : propertyAsGroup->GetProperties())
			CreateContextMenuFromPropertyRecursive(subMenu, childProperty.second);

		OwnerMenu->addMenu(subMenu);
	}
	else if (auto propertyAsAction = std::dynamic_pointer_cast<IPropertyAction>(Property))
	{
		QAction * action = ZN_NEW QAction(propertyAsAction->GetName().c_str(), OwnerMenu);
		if (false == propertyAsAction->ExecutePrecondition())
			action->setEnabled(false); // TODO: Refactor me

		OwnerMenu->connect(action, &QAction::triggered, OwnerMenu, [propertyAsAction] {
			propertyAsAction->ExecuteAction();
		});
		OwnerMenu->addAction(action);
	}
}

std::shared_ptr<QMenu> CreateContextMenuFromPropertiesGroup(std::shared_ptr<IPropertiesGroup> PropertiesGroup)
{
	std::shared_ptr<QMenu> menu = MakeShared(QMenu, PropertiesGroup->GetName().c_str());

	QAction* nameAction = ZN_NEW QAction(PropertiesGroup->GetName().c_str(), menu.get());
	nameAction->setEnabled(false);

	menu->addAction(nameAction);
	menu->addSeparator();

	for (const auto childProperty : PropertiesGroup->GetProperties())
		CreateContextMenuFromPropertyRecursive(menu.get(), childProperty.second);

	return menu;
}