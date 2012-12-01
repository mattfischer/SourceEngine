#ifndef BSP_FILE_HPP
#define BSP_FILE_HPP

#include "SharedPointer.hpp"
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

    BSPFile(sp<IReader> reader);

	Model *model() { return mModel; }

private:
	sp<IReader> mReader;
	Model *mModel;
};

#endif