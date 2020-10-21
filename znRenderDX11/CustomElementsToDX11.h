#pragma once

namespace CustomElements
{
	namespace DX11
	{
		SCustomInputElement DX11ToCustom(const D3D11_INPUT_ELEMENT_DESC& DX11InputElement);
		D3D11_INPUT_ELEMENT_DESC CustomToDX11(const SCustomInputElement& CustomInputElement);

		void MergeCustom(std::vector<SCustomInputElement>& CustomInputElements);

		std::vector<SCustomInputElement> DX11ToCustom(const std::vector<D3D11_INPUT_ELEMENT_DESC>& CustomInputElements);
		std::vector<D3D11_INPUT_ELEMENT_DESC> CustomToDX11(const std::vector<SCustomInputElement>& CustomInputElements);
	}
}