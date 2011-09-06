#ifndef _SERIALIZATION_QUEUE
#define _SERIAlIZATION_QUEUE

#include <vector>

class SerializationQueue
{
public:
	std::vector<const int *> int_array;
	std::vector<const float *> float_array;
	std::vector<const double *> double_array;
	std::vector<const bool *> bool_array;
	std::vector<const short *> short_array;
	std::vector<const long *> long_array;
	std::vector<const Serializable *> object_array;
};

#endif