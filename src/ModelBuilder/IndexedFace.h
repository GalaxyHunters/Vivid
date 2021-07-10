#ifndef VIVID_INDEXEDFACE_H
#define VIVID_INDEXEDFACE_H

#include "Point.h"

namespace vivid
{

class CIndexedFace
{

private:
    std::vector<size_t> mPoints;
    coord_t mColor;

public:
	inline CIndexedFace (){};
	inline CIndexedFace (const std::vector<size_t> & arPoints, coord_t aColor) : mPoints(arPoints), mColor(aColor){};
    inline CIndexedFace (const CIndexedFace& arIF) : mPoints(arIF.mPoints), mColor(arIF.mColor){};
    inline CIndexedFace (size_t aP0, size_t aP1, size_t aP2, coord_t aColor) : mPoints({aP0, aP1, aP2}), mColor(aColor){}; // TODO ??? // used after triangulation and decimation
    inline CIndexedFace& operator= (const CIndexedFace& arIF) { mPoints = arIF.mPoints; mColor = arIF.mColor;  return *this; }
	inline ~CIndexedFace(){};

	inline const std::vector<size_t> GetPoints() { return mPoints; }
	inline const size_t operator[](size_t I) {return mPoints[I];}
	inline const coord_t GetColor() { return mColor; }
	inline void SetColor(coord_t aColor) { mColor = aColor; }
	inline void SetPoints(std::vector<size_t>& arPoints) { mPoints = arPoints; }

};

} // namespace vivid
#endif //VIVID_INDEXEDFACE_H
