#include "SceneBase_Tests.h"
#include "ExLib_RendererBase.h"

#include "Visual.h"
#include "SceneNode.h"

void SceneBase_Tests::Entry()
{
	Visual visual;
	SceneNode node;
	node.AttachChild(&visual);
}