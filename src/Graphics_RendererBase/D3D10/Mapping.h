#ifndef _MAPPING_H_
#define _MAPPING_H_

#include <d3d10.h>

extern D3D10_USAGE MappingBufferUsage [];
extern UINT MappingCPUAccess [];
extern D3D10_MAP MappingMapAccess [];
extern D3D10_BLEND MappingBlendFactor [];
extern D3D10_BLEND_OP MappingBlendOp [];
extern D3D10_STENCIL_OP MappingDSOperation [];
extern D3D10_COMPARISON_FUNC MappingComparisonMode [];
extern D3D10_FILL_MODE MappingRSFill [];
extern D3D10_CULL_MODE MappingRSCull [];
extern D3D10_FILTER MappingSamplerFilter [];
extern D3D10_TEXTURE_ADDRESS_MODE MappingSamplerEdge [];
extern D3D10_PRIMITIVE_TOPOLOGY MappingPrimitiveType [];
extern unsigned int MappingPrimitiveType_IndicesPerType[];

#endif