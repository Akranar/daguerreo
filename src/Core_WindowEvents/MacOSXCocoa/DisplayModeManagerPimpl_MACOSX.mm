#include "DisplayModeManagerPimpl_MACOSX.h"
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

class CGDisplayModeCompare
{
public:
    bool operator() (const CGDisplayModeRef & a, const CGDisplayModeRef & b)
    {
        if (CGDisplayModeGetWidth(a) < CGDisplayModeGetWidth(b))
        {
            return true;
        }
        if (CGDisplayModeGetWidth(a) > CGDisplayModeGetWidth(b))
        {
            return false;
        }
        if (CGDisplayModeGetHeight(a) < CGDisplayModeGetHeight(b))
        {
            return true;
        }
        return false;
    }

};


DisplayModeManagerPimpl::DisplayModeManagerPimpl()
{
	
	CGDisplayModeRef normal_display_mode = CGDisplayCopyDisplayMode(kCGDirectMainDisplay);
    CFArrayRef all_modes = CGDisplayCopyAllDisplayModes(kCGDirectMainDisplay, NULL);
	std::vector<CGDisplayModeRef> sort_modes;
	sort_modes.push_back(normal_display_mode);

	int normal_bpp = GetBitsPerPixel(normal_display_mode);
	int mode_count_temp = CFArrayGetCount(all_modes);
	
    for(int i = 0; i < mode_count_temp; i++)	{
		CGDisplayModeRef mode = (CGDisplayModeRef)CFArrayGetValueAtIndex(all_modes, i);
        int current_bpp = GetBitsPerPixel(mode);
		if (current_bpp == normal_bpp)
		{
			sort_modes.push_back(CGDisplayModeRetain(mode));
		}
	}
	
	CFRelease(all_modes);
	
	std::sort(sort_modes.begin()+1, sort_modes.end(), CGDisplayModeCompare());
	mode_count = sort_modes.size();
	
	modes = new CGDisplayModeRef[mode_count];
	for (unsigned int i = 0; i < mode_count; ++i)
	{
		modes[i] = sort_modes[i];
	}

}
DisplayModeManagerPimpl::~DisplayModeManagerPimpl()
{
	for (unsigned int i = 0; i < mode_count; ++i)
	{
		CGDisplayModeRelease(modes[i]);
	}
	delete modes;
}

int DisplayModeManagerPimpl::GetBitsPerPixel(CGDisplayModeRef & mode)
{
	size_t depth = 0;
	
	CFStringRef pixEnc = CGDisplayModeCopyPixelEncoding(mode);
	if(CFStringCompare(pixEnc, CFSTR(IO32BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
		depth = 32;
	else if(CFStringCompare(pixEnc, CFSTR(IO16BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
		depth = 16;
	else if(CFStringCompare(pixEnc, CFSTR(IO8BitIndexedPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
		depth = 8;
	
	return depth;
}