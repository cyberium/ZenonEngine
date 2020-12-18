#pragma once


template <class DECODER>
void DecodeDXTBlock(uint8 * dstData, const uint8 * srcBlock, long dstPitch, int bw, int bh)
{
	DECODER decoder;
	decoder.Setup(srcBlock);
	for (int y = 0; y < bh; y++)
	{
		uint8 *dst = dstData - y * dstPitch;
		decoder.SetY(y);
		for (int x = 0; x < bw; x++)
		{
			decoder.GetColor(x, y, (DDSFormat::Color8888 &)*dst);
			dst += 4;
		}
	}
}