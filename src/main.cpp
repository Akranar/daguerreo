#include "Core_MemoryMonitor.h"
#include "Core_FileSystem/FileSystem_LIB.h"


#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>



#include "Graphics_RendererBase/RendererBase_LIB.h"
#include "SceneGraph_SceneBase/SceneBase_Tests.h"
#include <string>

#include "IO_BinaryFileStream/BinaryFileStream_LIB.h"

#include "Assets_Converter/AssimpResources_Tests.h"

#include "Example_RendererBaseUsage/RendererBaseUsage_LIB.h"
  
int main(int argc, char ** argv)
{
	FileSystem::SetWorkingDirectoryResources();

	//MEMORYTRACK_VERBOSE;

	MEMORYTRACK_START;
	{

		//RendererBaseEntries renderer_base_entries;
		//renderer_base_entries.Entry();

		//SceneBase_Tests sbt;
		//sbt.Entry();
		
		//BinaryFileStream_Tests bfs;
		//bfs.Entry();

		AssimpResources_Tests ar_tests;
		ar_tests.Entry();

		//ApplicationLoop<Example_Main>().Run();
	}
	MEMORYTRACK_END;


	return 0;
}
