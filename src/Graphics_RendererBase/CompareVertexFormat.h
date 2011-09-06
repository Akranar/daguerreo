#ifndef _COMPARE_VERTEX_FORMAT_H_
#define _COMPARE_VERTEX_FORMAT_H_

class DescVertexFormat;

class CompareVertexFormat
{
public:
	bool operator() (const DescVertexFormat & a, const DescVertexFormat & b);
};

#endif