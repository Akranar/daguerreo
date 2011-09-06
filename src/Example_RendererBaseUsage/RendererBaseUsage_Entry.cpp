#include "RendererBaseUsage_Entry.h"
#include "Example_Main.h"
#include "ApplicationLoop.h"

void RendererBaseUsage_Entry::Entry()
{
	ApplicationLoop<Example_Main>().Run();
}