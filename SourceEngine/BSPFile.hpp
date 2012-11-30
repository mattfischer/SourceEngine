#ifndef BSPFILE_HPP
#define BSPFILE_HPP

#include "IReader.hpp"

#include <fstream>
#include <string>

class BSPFile {
public:
	struct Point {
		float x;
		float y;
		float z;
	};

	struct Poly {
		int numPoints;
		float gray;
		Point *points;
	};

	struct Model {
		int numPolys;
		Poly *polys;
	};

    BSPFile(IReader *reader);

	Model *model() { return mModel; }

private:
	IReader *mReader;
	Model *mModel;
};

#endif