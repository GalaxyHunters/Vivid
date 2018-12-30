#ifndef UTILS_H
#define UTILS_H

#include "Point.h"
#include "Voronoi3D.hpp"
using namespace std;

inline vector<Vector3D> convertToVorn(vector<Point> inputPoints) {
	vector<Vector3D> newVec;
	for (vector<Point>::iterator it = inputPoints.begin(); it != inputPoints.end(); it++) {
		newVec.push_back(Vector3D(it->getX(), it->getY(), it->getZ()));
	}
	return newVec;
}

inline pair<vector<Vector3D>, vector<vector<size_t>>> compute_vornoi(vector<Point> inputPoints, double box_R) {
	vector<Vector3D> vornPoints = convertToVorn(inputPoints);

/////////////////////////////////////debug////////////////////////////////////////////////////
	cout << vornPoints.size() << endl;
	ofstream o;
	o.open("../../test_models/voroni_input.txt");
	o << "box size input:\n";
	o << to_string(-box_R) + " " + to_string(-box_R) + " " + to_string(-box_R) + "\n";
	o << to_string(box_R) + " " + to_string(box_R) + " " + to_string(box_R) + "\n";
	o << "points input:\n";
	for (vector<Vector3D>::iterator it = vornPoints.begin(); it != vornPoints.end(); it++) {
                o << to_string(it->x) + " " + to_string(it->y) + " " + to_string(it->z) + "\n";
	}
	o.close();
/////////////////////////////////////////debug over/////////////////////////////////////////////
	Voronoi3D temp(Vector3D(-box_R, -box_R, -box_R), Vector3D(box_R, box_R, box_R));
	temp.Build(vornPoints);
	vornPoints = temp.GetFacePoints();

	vector<vector<size_t>> faces;
	size_t totalCells = temp.GetAllCellFaces().size();
	vector<size_t> cell;
	for (size_t i = 0; i < totalCells; i++) {
		cell = temp.GetCellFaces(i);
		for (vector<size_t>::iterator face = cell.begin(); face != cell.end(); face++) {
			size_t Cpoint1 = get<0>(temp.GetFaceNeighbors(*face));
			size_t Cpoint2 = get<1>(temp.GetFaceNeighbors(*face));
			if (!(Cpoint1 < i) && !(Cpoint2 < i)) { //the face doent belong to a cell we read already
				faces.push_back(temp.GetPointsInFace(*face));
				faces.back().push_back(Cpoint1);
				faces.back().push_back(Cpoint2);
			}
		}
	}
	pair<vector<Vector3D>, vector<vector<size_t>>> output(vornPoints, faces);
	return output;
}

#endif
