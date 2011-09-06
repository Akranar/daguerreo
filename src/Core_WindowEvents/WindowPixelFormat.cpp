#include "WindowPixelFormat.h"

void WindowPixelFormat::SetDefaults()
{
	red_bits = 8;
	green_bits = 8;
	blue_bits = 8;
	alpha_bits = 8;
	depth_bits = 24;
	stencil_bits = 8;
	sample_buffers = 1;
	samples = 1;
}
void WindowPixelFormat::SetAll(int red_bitcount, int green_bitcount, int blue_bitcount, int alpha_count, 
		int depth_bitcount, int stencil_bitcount, int sample_buffercount, int samples_count)
{
	red_bits = red_bitcount;
	green_bits = green_bitcount;
	blue_bits = blue_bitcount;
	alpha_bits = alpha_count;
	depth_bits = depth_bitcount;
	stencil_bits = stencil_bitcount;
	sample_buffers = sample_buffercount;
	samples = samples_count;
}