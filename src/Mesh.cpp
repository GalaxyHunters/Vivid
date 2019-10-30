#include "Mesh.h"
#include <stdio.h>

using namespace boost::algorithm;

CMesh::~CMesh() {}

CMesh::CMesh(const CMesh &mesh){
    this->mAlpha = mesh.mAlpha;
    this->mFaces = mesh.mFaces;
    this->mLabel = mesh.mLabel;
    this->mPoints = mesh.mPoints;

}


CMesh::CMesh(vector<CPoint> aPoints, vector<CIndexedFace> aFaces, string aLabel, cord_t aAlpha) : mPoints(aPoints), mFaces(aFaces), mLabel(aLabel), mAlpha(aAlpha) {}

Color_t static Quan2Color(cord_t aQuan) { // calls function from ColorMap.h
	return GetColor(aQuan);
}

bool static CompareColor(Color_t aColor1, Color_t aColor2) {
	return (aColor1.R == aColor2.R && aColor1.G == aColor2.G && aColor1.B == aColor2.B);
}

bool static CompareQuan(CIndexedFace aFace1, CIndexedFace aFace2) {
	return (aFace1.GetColor() > aFace2.GetColor());
}
static bool(*CompFace)(CIndexedFace, CIndexedFace) = CompareQuan;


void CMesh::WriteNewMtl(ofstream& aOBJFile, ofstream& aMTLFile, size_t * apMtlCounter, Color_t aColor, cord_t aAlpha)
{
	aMTLFile << "newmtl surf_" + to_string(*apMtlCounter) + "\n" + \
		"Ns 96.078\n" + \
		"Ka 1.000 1.000 1.000 \n" + \
		"Kd " + to_string(aColor.R) + " " + to_string(aColor.G) + " " + to_string(aColor.B) + "\n" + \
		"Ks 0.000 0.000 0.000\n" + \
		"Ni 1.000000\n" + \
		"d " + to_string(aAlpha) + "\n" + \
		"illum 0\n" + \
		"em 0.000000\n\n\n";
	aOBJFile << "usemtl surf_" + int2str(*apMtlCounter) + "\n";
}

void CMesh::WriteNewFace(ofstream& aOBJFile, CIndexedFace aFace) 
{
	aOBJFile << "f ";
	vector<size_t> face_points = aFace.GetPoints();
	for (vector<size_t>::iterator ItPoint = face_points.begin(); ItPoint != face_points.end(); ItPoint++)
	{
		aOBJFile << int2str(*ItPoint + 1) + " ";
	}
	aOBJFile << "\n";
}

void CMesh::WriteObj(ofstream& aOBJFile, ofstream& aMTLFile, size_t * apMtlCounter) 
{
	aOBJFile << "o " + this->mLabel + "\n";
	//write points to obj file
	for (vector<CPoint>::iterator it = this->mPoints.begin(); it != this->mPoints.end(); it++) 
	{
		aOBJFile << "v " + to_string(it->GetX()) + " " + to_string(it->GetY()) + " " + to_string(it->GetZ()) + "\n";
	}
	//sort vecFaces by color
	sort(this->mFaces.begin(), this->mFaces.end(), CompFace); // NlogN
	//write faces to obj file + write colors to mtl file
	Color_t color = Quan2Color(this->mFaces[0].GetColor());
	WriteNewMtl(aOBJFile, aMTLFile, apMtlCounter, color, this->mAlpha);
	for (vector<CIndexedFace>::iterator it = this->mFaces.begin(); it != this->mFaces.end(); it++) 
	{
		if (CompareColor(color, Quan2Color(it->GetColor()))) //if true write the face to the obj file
		{
			WriteNewFace(aOBJFile, *it);
		}
		else // we reached a new color, and we need to write it in mtl before using it
		{
			color = Quan2Color(it->GetColor());
			(*apMtlCounter)++;
			WriteNewMtl(aOBJFile, aMTLFile, apMtlCounter, color, this->mAlpha);
			// now we can write the face in the obj file
			WriteNewFace(aOBJFile, *it);
		}
	}
}

void CMesh::ExportToObj(string aOutput){
	if (ends_with(aOutput, ".obj")) { //check if the output file ends with .obj, and delete it if it does
		aOutput.erase(aOutput.length() - 4, 4);
	}
	string lines;
    #if defined(_WIN32)
	    lines = '\\';
    #elif defined(_linux_)
        lines = '/';
    #elif defined __APPLE__
        lines = '/';
    #endif

	ofstream o; // the obj file
	o.open(aOutput + ".obj");
	ofstream m; //the mtl file
	m.open(aOutput + ".mtl");
	string mtl = aOutput + ".mtl";
	while (mtl.find(lines) != string::npos) {
		mtl = mtl.substr(mtl.find(lines) + 1, string::npos);
	}
	//write obj file starter
	o << "# This 3D code was produced by Vivid \n\n\n";
	o << "mtllib " + mtl + "\n";

	size_t mtl_counter = 0;
	this->WriteObj(o, m, &mtl_counter);
	o.close();
	m.close();
}

void CMesh::ExportToObjTexture(string aOutput){
    if (ends_with(aOutput, ".obj")) { //check if the output file ends with .obj, and delete it if it does
        aOutput.erase(aOutput.length() - 4, 4);
    }
    string lines;
#if defined(_WIN32)
    lines = '\\';
#elif defined(_linux_)
    lines = '/';
    #elif defined __APPLE__
        lines = '/';
#endif

    ofstream o; // the obj file
    o.open(aOutput + ".obj");
    ofstream m; //the mtl file
    m.open(aOutput + ".mtl");
    string mtl = aOutput + ".mtl";
    while (mtl.find(lines) != string::npos) {
        mtl = mtl.substr(mtl.find(lines) + 1, string::npos);
    }
    //write texture
    vector<unsigned char> texture;
    texture = GetColorTexture();
    encodePNG((aOutput + "Texture.png").c_str(), texture, 1, texture.size()/4);
/*    //write obj file starter
    o << "# This 3D code was produced by Vivid \n\n\n";
    o << "mtllib " + mtl + "\n";

    size_t mtl_counter = 0;
    this->WriteObj(o, m, &mtl_counter);
    o.close();
    m.close();*/
}

void CMesh::Decimation(cord_t aVerticlePercent, cord_t aMaxError)
{
    //check input valdilty
    if( aVerticlePercent < 0 || aVerticlePercent > 1){
        throw "input error, DecimationPercent must be between 0 and 1";
    }
    if( aMaxError < 0 || aMaxError > 1){
        throw "input error, MaxError must be between 0 and 1";
    }
	//triangulation
	this->Triangulation();
	//call decimation from External
	int targetVerticesN = int(aVerticlePercent * this->mPoints.size());
	int targetTrianglesN = int(aVerticlePercent * this->mFaces.size());
	pair<vector<CPoint>, vector<CIndexedFace> > temp = DecimateMesh(this->mPoints, this->mFaces, targetVerticesN, targetTrianglesN, aMaxError);
	this->mPoints = get<0>(temp);
	this->mFaces = get<1>(temp);
}

void CMesh::Triangulation() {
	vector<CIndexedFace> triangles;
	for (vector<CIndexedFace>::iterator fIt = this->mFaces.begin(); fIt != this->mFaces.end(); fIt++) {
		for (size_t i = 1; i < fIt->GetPoints().size()-1; i++) { // go over all the vertices from 1 to n-1 and connect them with vertice 0 to create triangles 
			triangles.push_back(CIndexedFace((*fIt)[0], (*fIt)[i], (*fIt)[i + 1], fIt->GetColor()));
		}
	}
	this->mFaces = triangles;
}
//geters seters

string CMesh::GetLabel() { return this->mLabel; }
cord_t CMesh::GetAlpha() { return this->mAlpha; }
vector<CPoint> CMesh::GetPoints() { return this->mPoints; }
vector<CIndexedFace> CMesh::GetFaces() { return this->mFaces; }
void CMesh::SetFaces(vector<CIndexedFace> aFaces) { this->mFaces = aFaces; }
void CMesh::SetPoints(vector<CPoint> aPoints) { this->mPoints = aPoints; }
void CMesh::SetLabel(string aLabel) { this->mLabel = aLabel; }
void CMesh::SetAlpha(cord_t aAlpha){
    //check input valdilty
    if(aAlpha > 1 || aAlpha < 0){
        throw "Alpha must be between 0 and 1";
    }
    this->mAlpha = aAlpha;
}
