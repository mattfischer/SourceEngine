#ifndef BSPFILE_HPP
#define BSPFILE_HPP

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

    BSPFile(const std::string &filename);

	Model *model() { return mModel; }

private:
	std::ifstream mFile;
	Model *mModel;
};

#endif