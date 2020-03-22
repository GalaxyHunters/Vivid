#include "Surf.h"
using namespace std;

static const cord_t DoublePointThreshHold = 0.0001;

CSurf::CSurf() {};

CSurf::CSurf(const CSurf &surf2) {
	this->mInputPoints = surf2.mInputPoints;
	this->mMask = surf2.mMask;
	this->mQuan = surf2.mQuan;

	map < shared_ptr<CPoint>, shared_ptr<CPoint> > old_to_new_Points;
	this->mVecPoints.clear();
	for (size_t i = 0; i < surf2.mVecPoints.size(); i++) {
		this->mVecPoints.push_back(shared_ptr<CPoint>(new CPoint(*(surf2.mVecPoints[i]))));
		old_to_new_Points[surf2.mVecPoints[i]] = this->mVecPoints.back();
	}
	this->mVecFaces.clear();
	CSurfFace temp;
	for (vector<CSurfFace>::const_iterator it = (surf2.mVecFaces).begin(); it != surf2.mVecFaces.end(); it++) {
		temp = CSurfFace();
		temp.mColor = (*it).mColor;
		temp.mCPoints = (*it).mCPoints;
		temp.mPoints.clear();
		for (vector<shared_ptr<CPoint> >::const_iterator pIt = it->mPoints.begin(); pIt != it->mPoints.end(); pIt++){
			temp.mPoints.push_back(old_to_new_Points[*pIt]);
		}
		this->mVecFaces.push_back(temp);
	}
}

CSurf::CSurf(vector<vector<double >> aInputPoints, vector<bool> aMask, vector<cord_t> aQuan, cord_t aVMin, cord_t aVMax) {
    //check for input valdidlty
    if((aInputPoints.size() != aMask.size()) || (aQuan.size() != aInputPoints.size()) || (aQuan.size() != aMask.size())){
        if(aQuan.size() != 0) {
            throw "Input error, inputted vectors (points, mask, quan) must all be the same size";
        }
    }
    //check if aMask has both False and True values
    bool is_containing_true = false;
    bool is_containing_false = false;
    for(vector<bool>::iterator it = aMask.begin(); it != aMask.end(); it++){
        if(*it == true){
            is_containing_true = true;
        }
        if(*it == false){
            is_containing_false = true;
        }
    }
    if(!is_containing_false || !is_containing_true){
        throw "Input error, mask must have both True and false values";
    }
    //code
    vector<CPoint> temp;
    for (vector<vector<double> >::iterator it = aInputPoints.begin(); it != aInputPoints.end(); it++){
        temp.push_back(CPoint(*it));
    }
    temp.resize(temp.size());
	this->SetInputPoints(temp);
	this->SetMask(aMask);
	this->SetQuan(this->NormQuan(aQuan, aVMin, aVMax));
	this->RunVorn();
	this->CleanEdges();
	this->CleanFaces(aMask);
	this->CleanPoints();
}

vector<CSurf> CSurf::CreateSurf(vector<CPoint> aInputPoints, vector<vector<bool> > aMask, vector<cord_t> aQuan, cord_t aVMin, cord_t aVMax) {
	CSurf temp;
	temp.SetInputPoints(aInputPoints);
	temp.SetQuan(temp.NormQuan(aQuan, aVMin, aVMax));
	temp.RunVorn();
	temp.CleanEdges();
	vector<CSurf> output;
	CSurf newSurf;
	for (vector<vector<bool> >::iterator it = aMask.begin(); it != aMask.end(); it++) {
		newSurf = CSurf(temp);
		newSurf.SetMask(*it);
		newSurf.CleanFaces(*it);
		newSurf.CleanPoints();
		output.push_back(newSurf);
	}
	return output;
}

void CSurf::CleanFaces(vector<bool>& aMask) {
	vector<CSurfFace> new_faces;
	size_t mask_len = aMask.size();
	size_t c_point1;
	size_t c_point2;
	for (vector<CSurfFace>::iterator it = mVecFaces.begin(); it != mVecFaces.end(); it++) {
		c_point1 = get<0>(it->mCPoints);
		c_point2 = get<1>(it->mCPoints);
		if (mask_len > c_point1 && mask_len > c_point2) { //the indexs are both in range and not a part of the box
			if (aMask[c_point1] != aMask[c_point2]) { //the face is a part of the surf
				new_faces.push_back(*it);
				new_faces.back().mColor = (this->mQuan[c_point1] + this->mQuan[c_point2]) / 2;
			}
		}
	}
	this->mVecFaces = new_faces;
}

void CSurf::CleanPoints() {
	vector<shared_ptr<CPoint> > new_points;
	for (vector<shared_ptr<CPoint> >::iterator it = mVecPoints.begin(); it != mVecPoints.end(); it++) {
		if (it->use_count() > 1) { // the points is also being called from another obj, other then this vector
			new_points.push_back(*it);
		}
	}
	this->mVecPoints = new_points;
	RemoveDoublePoints();
}

//----------------------------------------remove doubles functions ----------------------------------------------------------------------
bool CompPointRD(shared_ptr<CPoint> aObj1, shared_ptr<CPoint> aObj2) {
	if (abs((*aObj1).GetX() - (*aObj2).GetX()) <= DoublePointThreshHold) { //the x value is nurmallcly the same
		if (abs((*aObj1).GetY() - (*aObj2).GetY()) <= DoublePointThreshHold) { //the y value is nurmallcly the same
			if (abs((*aObj1).GetZ() - (*aObj2).GetZ()) <= DoublePointThreshHold) { //the z value is nurmallcly the same
				return false;
			}
			else
			{ // we compare the points by z to see who needs to go first
				return (*aObj1).GetZ() > (*aObj2).GetZ();
			}
		}
		else // we compare by y to see who needs to go first
		{
			return (*aObj1).GetY() > (*aObj2).GetY();
		}
	}
	else // we compare by x to see who goes first
	{
		return (*aObj1).GetX() > (*aObj2).GetX();
	}
}

void CSurf::RemoveDoublePoints() {
	//sort the array
	std::sort(this->mVecPoints.begin(), this->mVecPoints.end(), CompPointRD);
	map < shared_ptr<CPoint>, shared_ptr<CPoint> > old_new_points; // will hold the new pointer fiting to each point
	vector<shared_ptr<CPoint> > cleaned_points;
	size_t j;
	for (size_t i = 0; i < this->mVecPoints.size(); i++) {
		j = i + 1;
		cleaned_points.push_back(this->mVecPoints[i]); // push the point to the cleaned data
		old_new_points[this->mVecPoints[i]] = this->mVecPoints[i];
		if (j >= mVecPoints.size())
		{
			break;
		}
		while ((*this->mVecPoints[i]).CalcDistance((*this->mVecPoints[j])) <= DoublePointThreshHold) { //check if the point has duplicates that we need to skip
			old_new_points[this->mVecPoints[j]] = this->mVecPoints[i];
			j += 1;
			if (j >= this->mVecPoints.size()) 
			{
				break;
			}
			//cout << "removed point" << endl;
		}
		i = j - 1; //set i to the last a duplicate (ot to i if there were no duplicates).
	}
	// change the points ptrs within the faces
	for (vector<CSurfFace>::iterator faceIt = this->mVecFaces.begin(); faceIt != this->mVecFaces.end(); faceIt++) {
		for (vector<shared_ptr<CPoint> >::iterator pointIt = faceIt->mPoints.begin(); pointIt != faceIt->mPoints.end(); pointIt++) {
			(*pointIt) = old_new_points[*pointIt];
		}
	}
	this->mVecPoints = cleaned_points;
}

//smooth functions----------------------------------------------------------------------------------------------------------

void UpdatePoutPin(vector<size_t>& aPOut, vector<size_t>& aPIn) {
	for (int i = 0; (unsigned)i < aPOut.size(); i++) {
		aPOut[i] = i;
	}
	for (int i = 0; (unsigned)i < aPIn.size(); i++) {
		aPIn[i] = i + aPOut.size();
	}
}

void CSurf::SetPinPout(vector<size_t>& arPOut, vector<size_t>& arPIn) { //define pin and pout
	map<size_t, bool> p_in_map;
	map<size_t, bool> p_out_map;
	size_t c_point1;
	size_t c_point2;
	for (vector<CSurfFace>::iterator it = this->mVecFaces.begin(); it != this->mVecFaces.end(); it++) {
		c_point1 = get<0>(it->mCPoints);
		c_point2 = get<1>(it->mCPoints);
		if (this->mMask[c_point1]) {
			if (p_in_map.count(c_point1) == 0) {
				arPIn.push_back(c_point1);
				p_in_map[c_point1] = true;
			}
			if (p_out_map.count(c_point2) == 0) {
				arPOut.push_back(c_point2);
				p_out_map[c_point2] = true;
			}
		}
		else
		{
			if (p_in_map.count(c_point2) == 0) {
				arPIn.push_back(c_point2);
				p_in_map[c_point2] = true;
			}
			if (p_out_map.count(c_point1) == 0) {
				arPOut.push_back(c_point1);
				p_out_map[c_point1] = true;
			}
		}
	}
}

void CSurf::UpdateInputPoints(vector<size_t>& arPOut, vector<size_t>& arPIn) {
	vector<CPoint> new_points;
	vector<cord_t> quan;
	for (vector<size_t>::iterator it = arPOut.begin(); it != arPOut.end(); it++) {
		new_points.push_back(this->mInputPoints[*it]);
		quan.push_back(this->mQuan[*it]);
	}
	for (vector<size_t>::iterator it = arPIn.begin(); it != arPIn.end(); it++) {
		new_points.push_back(this->mInputPoints[*it]);
		quan.push_back(this->mQuan[*it]);
	}
	this->mInputPoints = new_points;
	this->mQuan = quan;
}
void CSurf::MakeMask(size_t aPOutSize, size_t aPInSize) {
	vector<bool> new_mask;
	for (size_t i = 0; i < aPOutSize; i++) {
		new_mask.push_back(false);
	}
	for (size_t i = 0; i < aPInSize; i++) {
		new_mask.push_back(true);
	}
	this->mMask = new_mask;
}

bool CompPointData_t(CPointData_t aObj1, CPointData_t aObj2) {
	if (abs(aObj1.mPoint.GetX() - aObj2.mPoint.GetX()) <= DoublePointThreshHold) { //the x value is nurmallcly the same
		if (abs(aObj1.mPoint.GetY() - aObj2.mPoint.GetY()) <= DoublePointThreshHold) { //the y value is nurmallcly the same
			if (abs(aObj1.mPoint.GetZ() - aObj2.mPoint.GetZ()) <= DoublePointThreshHold) { //the z value is nurmallcly the same
				return false;
			}
			else 
			{ // we compare the points by z to see who needs to go first
				return aObj1.mPoint.GetZ() > aObj2.mPoint.GetZ();
			}
		}
		else // we compare by y to see who needs to go first
		{
			return aObj1.mPoint.GetY() > aObj2.mPoint.GetY();
		}
	}
	else // we compare by x to see who goes first
	{
		return aObj1.mPoint.GetX() > aObj2.mPoint.GetX();
	}
}

void CSurf::CleanDoublePointsVorn(vector<CPoint>& arNewPoints, vector<cord_t>& arNewQuan, vector<size_t>& arNewIn, vector<size_t>& arNewOut)
{
	vector<CPointData_t> data;
	data.clear();
	for (vector<size_t>::iterator it = arNewIn.begin(); it != arNewIn.end(); it++) {
		data.push_back(CPointData_t(arNewPoints[*it], arNewQuan[*it], true));
	}
	for (vector<size_t>::iterator it = arNewOut.begin(); it != arNewOut.end(); it++) {
		data.push_back(CPointData_t(arNewPoints[*it], arNewQuan[*it], false));
	}
	data = RemoveDoublesVornInput(data);
	arNewPoints.clear();
	arNewQuan.clear();
	arNewIn.clear();
	arNewOut.clear();
	for (size_t i = 0; i < data.size(); i++) {
		arNewPoints.push_back(data[i].mPoint);
		arNewQuan.push_back(data[i].mQuan);
		if (data[i].mIsIn) {
			arNewIn.push_back(i);
		}
		else
		{
			arNewOut.push_back(i);
		}
	}
	this->mInputPoints = arNewPoints;
	this->mQuan = arNewQuan;
}
vector<CPointData_t> CSurf::RemoveDoublesVornInput(vector<CPointData_t>& arData) {
	//sort the array
	std::sort(arData.begin(), arData.end(), CompPointData_t);
	vector<CPointData_t> cleaned_data;
	size_t j;
	for (size_t i = 0; i < arData.size(); i++) {
		j = i+1;
		cleaned_data.push_back(arData[i]); // push the point to the cleaned data
		if (j >= arData.size())
		{
			break;
		}
		while (arData[i].mPoint.CalcDistance(arData[j].mPoint) <= DoublePointThreshHold) { //check if the point has duplicates that we need to skip
			j += 1;
			if (j >= arData.size())
			{
				break;
			}
			//cout << "removed point" << endl;
		}
		i = j - 1; //set i to the last a duplicate (ot to i if there were no duplicates).
	}
	
	return cleaned_data;
}
void CSurf::AddPoints(vector<size_t> * apPVec, vector<CPoint> * apNewPoints, vector<cord_t> * apNewQuan, size_t * apNewIndex, size_t aCPoint1, size_t aCPoint2)
{
	cord_t x, y, z;
	(*apPVec).push_back(*apNewIndex);
	x = (this->mInputPoints[aCPoint1].GetX() * 2 + this->mInputPoints[aCPoint2].GetX()) / 3.0;
	y = (this->mInputPoints[aCPoint1].GetY() * 2 + this->mInputPoints[aCPoint2].GetY()) / 3.0;
	z = (this->mInputPoints[aCPoint1].GetZ() * 2 + this->mInputPoints[aCPoint2].GetZ()) / 3.0;
	(*apNewPoints).push_back(CPoint(x, y, z));
	(*apNewQuan).push_back((this->mQuan[aCPoint1] + this->mQuan[aCPoint2]) / 2.0);
	(*apNewIndex)++;

	(*apPVec).push_back(*apNewIndex);
	x = (this->mInputPoints[aCPoint1].GetX() + this->mInputPoints[aCPoint2].GetX() * 2) / 3.0;
	y = (this->mInputPoints[aCPoint1].GetY() + this->mInputPoints[aCPoint2].GetY() * 2) / 3.0;
	z = (this->mInputPoints[aCPoint1].GetZ() + this->mInputPoints[aCPoint2].GetZ() * 2) / 3.0;
	(*apNewPoints).push_back(CPoint(x, y, z));
	(*apNewQuan).push_back((this->mQuan[aCPoint1] + this->mQuan[aCPoint2]) / 2.0);
	(*apNewIndex)++;
}

void CSurf::Stage2AddPoints(vector<size_t>& arPOut, vector<size_t>& arPIn) {
	size_t c_point1;
	size_t c_point2;
	size_t p_out_size = arPOut.size();
	size_t p_in_size = p_out_size + arPIn.size();
	vector<CPoint> new_points;
    vector<cord_t> new_quan;
	arPIn.clear();
	arPOut.clear();
	size_t new_index = 0; // the index for the new point to be added
	//go over pout
	for (vector<CSurfFace>::iterator it = this->mVecFaces.begin(); it != this->mVecFaces.end(); it++) {
		c_point1 = get<0>(it->mCPoints);
		c_point2 = get<1>(it->mCPoints);
		if (c_point1 < p_out_size && c_point2 < p_out_size) //pout - [1,2,3,4...pout.size] so we are checking if cpoint is a part of pout
		{ 
			AddPoints(&arPIn, &new_points, &new_quan, &new_index, c_point1, c_point2);
		}
		//go over pin
		if ((p_in_size > c_point1 && c_point1 >= p_out_size) && (p_in_size > c_point2 && c_point2 >= p_out_size)) //pin - [pout.size...pout.size+pin.size] so we are checking if cpoint is a part of pin
		{
			AddPoints(&arPOut, &new_points, &new_quan, &new_index, c_point1, c_point2);
		}
	}
	CleanDoublePointsVorn(new_points, new_quan, arPIn, arPOut);
}


//public functions

void CSurf::SmoothSurf() {
	//begin smooth part 1, collecting all the cpoints from the faces on the surf
    vector<size_t> p_out;
    vector<size_t> p_in;
    SetPinPout(p_out, p_in);
	UpdateInputPoints(p_out, p_in);
	this->RunVorn();
	//begin smooth part 2, adding new points between the cpoints
	UpdatePoutPin(p_out, p_in);
	Stage2AddPoints(p_out, p_in);
	//begin smooth part 3, running the model and cleaning it
	UpdateInputPoints(p_out, p_in);
	RunVorn();
	CleanEdges();
	MakeMask(p_out.size(), p_in.size());
	CleanFaces(this->mMask);
	CleanPoints();
	//done.
}

vector<cord_t>& CSurf::NormQuan(vector<cord_t>& arQuan, cord_t aVMin, cord_t aVMax) {
    if (arQuan.size() == 0){ //incase the user doesnt input any color
        arQuan = vector<cord_t>(this->mMask.size(), 1);
        return arQuan;
    }
    if(aVMin ==  aVMax) { //in case the user inputs color but not aVMin and aVMax
        aVMax = *max_element(arQuan.begin(), arQuan.end());
        aVMin = *min_element(arQuan.begin(), arQuan.end());
    }
	if (aVMin == aVMax) { //in case where Vmin-Vmax == 0 (aQuan is a vector where all the values are the same)
		arQuan = vector<cord_t>(arQuan.size(), 1);
		return arQuan;
	}
	for (vector<cord_t>::iterator it = arQuan.begin(); it != arQuan.end(); it++) {
		*it = 1- ((*it - aVMax) / (aVMin - aVMax));
		if (*it > 1) *it = 1;
		if (*it < 0) *it = 0;
	}
	return arQuan;
}


const CMesh CSurf::ToMesh(string aLabel, cord_t aAlpha) {
    //check input valdilty
    if(aAlpha > 1 || aAlpha < 0){
        throw "Alpha must be between 0 and 1";
    }
	vector<CPoint> points;
	size_t counter = 0;
	map < shared_ptr<CPoint>, size_t> indexes;
	for (vector<shared_ptr<CPoint> >::iterator it = this->mVecPoints.begin(); it != this->mVecPoints.end(); it++) {
		points.push_back(**it);
		indexes[*it] = counter;
		counter++;
	}
	vector<CIndexedFace> faces;
	vector<size_t> face_points;
	set<size_t> set_points;
	for (vector<CSurfFace>::iterator it = this->mVecFaces.begin(); it != this->mVecFaces.end(); it++) {
		for (vector<shared_ptr<CPoint> >::iterator point = it->mPoints.begin(); point != it->mPoints.end(); point++) {
			if (set_points.count(indexes[*point]) == 0) {
				face_points.push_back(indexes[*point]);
				set_points.insert(indexes[*point]);
			}
		}
		faces.push_back(CIndexedFace(face_points, it->mColor));
		face_points = vector<size_t>();
		set_points.clear();
	}
	return CMesh(points, faces, aLabel, aAlpha);
}

void CSurf::ExportToObj(string aOutput, string aLabel, cord_t aAlpha) {
	CMesh mesh = ToMesh(aLabel, aAlpha);
	mesh.ExportToObj(aOutput);
}

static bool ComparePoint(CPoint aPoint1, CPoint aPoint2) {
	double dis1 = aPoint1.CalcDistance(CPoint(0, 0, 0));
	double dis2 = aPoint2.CalcDistance(CPoint(0, 0, 0));
	return (dis2 > dis1);
}
static bool(*CompPoint)(CPoint, CPoint) = ComparePoint;

static vector<shared_ptr<CPoint> > ConvertFromVorn(vector<Vector3D> aVornPoints) {
	vector<shared_ptr<CPoint> > new_vec;
	for (vector<Vector3D>::iterator it = aVornPoints.begin(); it != aVornPoints.end(); it++) {
		new_vec.push_back(shared_ptr<CPoint>(new CPoint(it->x, it->y, it->z)));
	}
	return new_vec;
}

static double FindBoxR(vector<CPoint> aInputPoints) {
	CPoint box_r = *max_element(aInputPoints.begin(), aInputPoints.end(), CompPoint);
	return box_r.CalcDistance(CPoint(0, 0, 0));
}
void CSurf::RunVorn() {
	//find the box_r
	double box_R = FindBoxR(this->mInputPoints);
	cout << "start vorn" << endl;
	pair<vector<Vector3D>, vector<vector<size_t> > > vorn_out = compute_vornoi(this->mInputPoints, box_R * 2);
	cout << "vorn done" << endl;
	//set the points
	this->mVecPoints = ConvertFromVorn(get<0>(vorn_out));
	//set the faces
	vector<vector<size_t> > vorn_faces = get<1>(vorn_out);
	vector<CSurfFace> new_faces;
	size_t c_point1;
	size_t c_point2;
	cord_t quan;
	vector<shared_ptr<CPoint> > face_points;
	for (vector<vector<size_t> >::iterator face = vorn_faces.begin(); face != vorn_faces.end(); face++) {
		c_point1 = face->back();
		face->pop_back();
		c_point2 = face->back();
		face->pop_back();
		quan = 0; //quan will be defiened later in the program(clean faces) so for now this will be a place holder
		face_points = vector<shared_ptr<CPoint> >();
		for (vector<size_t>::iterator point = face->begin(); point != face->end(); point++) {
			face_points.push_back(this->mVecPoints[*point]);
		}
		new_faces.push_back(CSurfFace(face_points, quan, pair<size_t, size_t>(c_point1, c_point2)));
	}
	this->mVecFaces = new_faces;

}

void CSurf::CleanEdges() {
	double box_R = FindBoxR(this->mInputPoints);
	bool is_out_of_radius = false;
	vector<CSurfFace> new_faces;
	for (vector<CSurfFace>::iterator face = this->mVecFaces.begin(); face != mVecFaces.end(); face++) {
		for (vector<shared_ptr<CPoint> >::iterator point = face->mPoints.begin(); point != face->mPoints.end(); point++) {
			if ((**point).CalcDistance(CPoint(0, 0, 0)) > box_R * 1.1) {
				is_out_of_radius = true;
			}
		}
		if (!is_out_of_radius) { //the face is inside the box Radius and should be kept
			new_faces.push_back(*face);
		}
		else
		{
			is_out_of_radius = false;
		}
	}
	this->mVecFaces = new_faces;
 }
