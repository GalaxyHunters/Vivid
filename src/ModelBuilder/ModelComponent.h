#ifndef VIVID_MODELCOMPONENT_H
#define VIVID_MODELCOMPONENT_H

#include <string>

namespace vivid
{

/* Abstract Class defining 3D component for a model. Can be mesh, line, point cloud or anything else. */
//TODO splines?
class CModelComponent {
    std::string mLabel = "";
    // set get


//    virtual void ExportToObj(const std::string &aOutputFilePath, bool WithTexture = 1) = 0;

//        /**
//         * transform CMesh points by transformation matrix
//         * @param[in] aTrans a 3x3 dimension matrix.
//         */
//        void TransformMesh(coord_t const aTrans[3][3]);
//        /**
//         * Rotate the CMesh points around a normal vector by an angel, counterclockwise
//         * @param[in] aNormVec the x,y.z normal to rotate around.
//         * @param[in] aRadAngel the angel to rotate by (in radians).
//         */
//        void RotateMesh(CPoint aNormVec, double aRadAngel);
//        /**
//         * Change CMesh points location by addition of x,y,z.
//         * @param[in] aDirectionVec the x,y.z direction to move by it.
//         */
//        void MoveMesh(CPoint aDirectionVec);
//        /**
//         * Change CMesh points scale in x,y,z direction by x,y,z of the scale vector
//         * @param[in] aScaleVec the x,y.z direction to move by it.
//         */
//        void ScaleMesh(CPoint aScaleVec);

};

}; // namespace vivid
#endif //VIVID_MODELCOMPONENT_H
