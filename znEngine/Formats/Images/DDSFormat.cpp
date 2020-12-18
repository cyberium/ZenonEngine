#include "stdafx.h"

// General
#include "DDSFormat.h"

// Get the 4 possible colors for a block
//
void DDSFormat::GetBlockColors(const DDSFormat::DXTColBlock& block, DDSFormat::Color8888 colors[4], bool isDXT1)
{
	// expand from 565 to 888
	for (int i = 0; i < 2; i++)
	{
		colors[i].a = 0xff;
		/*
		colors[i].r = (uint8)(block.colors[i].r * 0xff / 0x1f);
		colors[i].g = (uint8)(block.colors[i].g * 0xff / 0x3f);
		colors[i].b = (uint8)(block.colors[i].b * 0xff / 0x1f);
		*/
		colors[i].r = (uint8)((block.colors[i].r << 3U) | (block.colors[i].r >> 2U));
		colors[i].g = (uint8)((block.colors[i].g << 2U) | (block.colors[i].g >> 4U));
		colors[i].b = (uint8)((block.colors[i].b << 3U) | (block.colors[i].b >> 2U));
	}

	uint16 *wCol = (uint16 *)block.colors;
	if (wCol[0] > wCol[1] || !isDXT1)
	{
		// 4 color block
		for (int i = 0; i < 2; i++)
		{
			colors[i + 2].a = 0xff;
			colors[i + 2].r = (uint8)((uint16(colors[0].r) * (2 - i) + uint16(colors[1].r) * (1 + i)) / 3);
			colors[i + 2].g = (uint8)((uint16(colors[0].g) * (2 - i) + uint16(colors[1].g) * (1 + i)) / 3);
			colors[i + 2].b = (uint8)((uint16(colors[0].b) * (2 - i) + uint16(colors[1].b) * (1 + i)) / 3);
		}
	}
	else
	{
		// 3 color block, number 4 is transparent
		colors[2].a = 0xff;
		colors[2].r = (uint8)((uint16(colors[0].r) + uint16(colors[1].r)) / 2);
		colors[2].g = (uint8)((uint16(colors[0].g) + uint16(colors[1].g)) / 2);
		colors[2].b = (uint8)((uint16(colors[0].b) + uint16(colors[1].b)) / 2);

		colors[3].a = 0x00;
		colors[3].g = 0x00;
		colors[3].b = 0x00;
		colors[3].r = 0x00;
	}
}
