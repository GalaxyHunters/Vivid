#include "Model.h"
#include "Decimate.h"
#include "ImportAndExport/ObjImportExport.h"

using namespace vivid;
using namespace std;

//TODO DO we need CPP file?
//CHNAGE h to HPP?
void CModel::ExportToObj(string aOutput, bool WithTexture){
    OBJExporter(*this, aOutput, WithTexture);
}
