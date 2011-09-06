#ifndef _ASSIMP_SCENE_PRINTER_H_
#define _ASSIMP_SCENE_PRINTER_H_

struct aiScene;
struct aiNode;
struct aiMatrix4x4;

#include <set>
#include <string>

class AssimpScenePrinter
{
	std::set<std::string> bone_names;
	bool print_matrices;
	void Indent(int index) const;
public:
	inline void SetPrintMatrices(bool print);
	void PrintAllToFile(const aiScene * scene, const char * filename);
	void PrintSceneOverview(const aiScene * scene, int indent) const;

	void PrintAnimationData(const aiScene * scene, int index, int indent) const;
	void PrintCameraData(const aiScene * scene, int index, int indent) const;
	void PrintLightData(const aiScene * scene, int index, int indent) const;
	void PrintMaterialData(const aiScene * scene, int index, int indent) const;
	void PrintMeshData(const aiScene * scene, int index, int indent);
	void PrintTextureData(const aiScene * scene, int index, int indent) const;

	void PrintAllAnimationData(const aiScene * scene) const;
	void PrintAllCameraData(const aiScene * scene) const;
	void PrintAllLightData(const aiScene * scene) const;
	void PrintAllMaterialData(const aiScene * scene) const;
	void PrintAllMeshData(const aiScene * scene);
	void PrintAllTextureData(const aiScene * scene) const;

	void PrintAllBoneData(const aiScene * scene, int mesh_index, int indent);
	void PrintBoneData(const aiScene * scene, int mesh_index, int bone_index, int indent);

	void PrintNodeHierarchy(const aiNode * node, int indent) const;

	void PrintMatrix4x4(const aiMatrix4x4 * matrix, int indent) const;
};

inline void AssimpScenePrinter::SetPrintMatrices(bool print)
{
	print_matrices = print;
}

#endif