#ifndef VIVID_MODEL_H
#define VIVID_MODEL_H

#include "Mesh.h"
#include "Surface.h"


namespace vivid
{

class CModel
{
private:
    std::string mLabel = ""; //TODO should it be default string
    std::vector<CMesh> mMeshes = {}; //TODO ModelComponent refactor

public:

    // Should we have surfaces here? in what way? is there a better way that is still easy to the user?
    CModel(){};
    CModel(const std::vector<CMesh>& arMeshes, const std::string& arLabel) : mMeshes(arMeshes), mLabel(arLabel){};
	CModel(const CMesh& arMesh) {mMeshes.push_back(arMesh);};
    inline CModel& operator= (const CModel& arModel) { mMeshes = arModel.mMeshes, mLabel = arModel.mLabel;  return *this; }
	~CModel(){};

	inline void AddMesh(const CMesh& arMesh){mMeshes.push_back(arMesh);};
    inline void AddMeshes (const std::vector<CMesh>& arMeshes){mMeshes.insert(mMeshes.end(), arMeshes.begin(), arMeshes.end() );};
    inline void Concatenate (const CModel& arModel){mMeshes.insert(mMeshes.end(), arModel.mMeshes.begin(), arModel.mMeshes.end() );};
	//TODO remove Mesh??
	inline const vector<CMesh>& GetMeshes() {return mMeshes;}

	// Add Importers
    void ExportToObj(std::string aOutput, bool WithTexture = 1); //TODO const std::string &arOutputFilePath
//    void ExportToFBX(rotation bla bla, bool WithTexture = 1);

    //TODO export to BLOB
};

}; // namespace vivid
#endif //VIVID_MODEL_H