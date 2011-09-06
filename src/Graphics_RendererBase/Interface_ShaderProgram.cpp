#include "Interface_ShaderProgram.h"

int ShaderProgram::FindUniform(const char * name)
{
	if (uniforms.GetSize() == 0) return -1;
	unsigned int array_length = uniforms.GetSize();
	int previous_index = -1;
	int index = array_length/2;
	int high = array_length;
	int low = 0;
	while (index != previous_index)
	{
		int compare = uniforms[index].name.compare(name);
		if (compare < 0)
		{
			previous_index = index;
			low = index;
			index = (high + index) * 0.5f;
		}
		else if (compare > 0)
		{
			previous_index = index;
			high = index;
			index = (low + index) * 0.5;
		}
		else
		{
			return index;
		}
	}
	return -1;
}