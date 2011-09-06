#ifndef _WINDOW_PIXEL_FORMAT_H_
#define _WINDOW_PIXEL_FORMAT_H_

class WindowPixelFormat
{
	int red_bits;
	int green_bits;
	int blue_bits;
	int alpha_bits;
	int depth_bits;
	int stencil_bits;
	int sample_buffers;
	int samples;
public:
	void SetDefaults();
	void SetAll(int red_bitcount, int green_bitcount, int blue_bitcount, int alpha_count, 
		int depth_bitcount, int stencil_bitcount, int sample_buffercount, int samples_count);

	inline int GetRedBits() const;
	inline int GetGreenBits() const;
	inline int GetBlueBits() const;
	inline int GetAlphaBits() const;
	inline int GetDepthBits() const;
	inline int GetStencilBits() const;
	inline int GetSampleBuffers() const;
	inline int GetSamples() const;

	inline int GetTotalColorBits() const;

	inline void SetRedBits(int count);
	inline void SetGreenBits(int count);
	inline void SetBlueBits(int count);
	inline void SetAlphaBits(int count);
	inline void SetDepthBits(int count);
	inline void SetStencilBits(int count);
	inline void SetSampleBuffers(int count);
	inline void SetSamples(int count);
};



inline int WindowPixelFormat::GetRedBits() const
{
	return red_bits;
}
inline int WindowPixelFormat::GetGreenBits() const
{
	return green_bits;

}
inline int WindowPixelFormat::GetBlueBits() const
{
	return blue_bits;
}
inline int WindowPixelFormat::GetTotalColorBits() const
{
	return red_bits + green_bits + blue_bits;
}
inline int WindowPixelFormat::GetAlphaBits() const
{
	return alpha_bits;
}
inline int WindowPixelFormat::GetDepthBits() const
{
	return depth_bits;
}
inline int WindowPixelFormat::GetStencilBits() const
{
	return stencil_bits;
}
inline int WindowPixelFormat::GetSampleBuffers() const
{
	return sample_buffers;
}
inline int WindowPixelFormat::GetSamples() const
{
	return samples;
}

inline void WindowPixelFormat::SetRedBits(int count)
{
	red_bits = count;
}
inline void WindowPixelFormat::SetGreenBits(int count)
{
	green_bits = count;
}
inline void WindowPixelFormat::SetBlueBits(int count)
{
	blue_bits = count;
}
inline void WindowPixelFormat::SetAlphaBits(int count)
{
	alpha_bits = count;
}
inline void WindowPixelFormat::SetDepthBits(int count)
{
	depth_bits = count;
}	
inline void WindowPixelFormat::SetStencilBits(int count)
{
	stencil_bits = count;
}
inline void WindowPixelFormat::SetSampleBuffers(int count)
{
	sample_buffers = count;
}
inline void WindowPixelFormat::SetSamples(int count)
{
	samples = count;
}

#endif