#ifndef VIVID_OBJIMPORTEXPORT_H
#define VIVID_OBJIMPORTEXPORT_H

#include "Model.h"


namespace vivid
{

//output functions
//TODO should habe return value, and maybe exception!
void OBJExporter(CModel &aModel, std::string aOutPutFile, bool WithTexture); //=1

//CModel ImportToObj(ostream* aInPutFile);

} // namespace vivid
#endif //VIVID_OBJIMPORTEXPORT_H
