#include "DisplayModeManager.h"

unsigned int DisplayModeManager::SelectBest(int width, int height) const
{
	for (unsigned int i = 1; i < GetModeCount(); ++i)
	{
		int current_width = GetWidth(i);
		int current_height = GetHeight(i);

		if (current_width > width && current_height > height)
		{
			return i;
		}
	}
	return 0;
}