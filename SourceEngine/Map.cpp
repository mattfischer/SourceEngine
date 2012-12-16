#include "Map.hpp"

#include "File/VMT.hpp"

#include <math.h>

Map::Map(File::IReaderFactory *factory, const std::string &name)
{
	mBSP = File::BSP::open(factory, name);

	const File::BSP::Model &model = mBSP->model(0);

	mNumTextures = mBSP->numTexDatas();
	mTextures = new Texture[mNumTextures];
	for(int i=0; i<mNumTextures; i++) {
		const File::BSP::TexData &texData = mBSP->texData(i);
		const std::string &materialFilename = mBSP->texDataString(texData.nameStringTableID);

		mTextures[i].vtf = 0;
		File::VMT *vmt = File::VMT::open(factory, materialFilename);
		if(vmt && vmt->hasParameter("$basetexture")) {
			const std::string &textureFilename = vmt->parameter("$basetexture");
			File::VTF *vtf = File::VTF::open(factory, textureFilename);
			delete vmt;

			mTextures[i].vtf = vtf;
			glGenTextures(1, &mTextures[i].tex);
			glBindTexture(GL_TEXTURE_2D, mTextures[i].tex);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			for(int j=0; j<vtf->numMipMaps(); j++) {
				glTexImage2D(GL_TEXTURE_2D, vtf->numMipMaps() - j - 1, GL_RGBA, vtf->width(j), vtf->height(j), 0, GL_RGBA, GL_UNSIGNED_BYTE, vtf->data(j));
			}
		}
	}

	mNumFaces = mBSP->numFaces();
	mFaces = new Face[mNumFaces];
	for(int i=0; i<mNumFaces; i++) {
		const File::BSP::Face &bspFace = mBSP->face(i);
		const File::BSP::TexInfo &texInfo = mBSP->texInfo(bspFace.texInfo);
		Face &face = mFaces[i];

		for(int j=0; j<2; j++) {
			for(int k=0; k<4; k++) {
				face.textureVertices[j][k] = texInfo.textureVecs[j][k];
				face.lightMapVertices[j][k] = texInfo.lightmapVecs[j][k];
			}
		}

		if(bspFace.lightOfs == -1) {
			face.lightMap = 0;
		} else {
			glGenTextures(1, &face.lightMap);
			glBindTexture(GL_TEXTURE_2D, face.lightMap);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST );

			int lightmapWidth = bspFace.lightmapTextureSizeInLuxels[0] + 1;
			int lightmapHeight = bspFace.lightmapTextureSizeInLuxels[1] + 1;

			int textureWidth;
			int textureHeight;

			textureWidth = 1;
			while(textureWidth < lightmapWidth) {
				textureWidth *= 2;
			}

			textureHeight = 1;
			while(textureHeight < lightmapHeight) {
				textureHeight *= 2;
			}

			const unsigned char *lightMap = mBSP->lighting(bspFace.lightOfs);
			unsigned char *data = new unsigned char[textureWidth * textureHeight * 4];
			for(int y=0; y<textureHeight; y++) {
				for(int x=0; x<textureWidth; x++) {
					signed char exp = lightMap[(y * lightmapWidth + x) * 4 + 3];
					float scale = pow(2.0f, exp) * 20;
					for(int c=0; c<3; c++) {
						float color = lightMap[(y * lightmapWidth + x) * 4 + c] / 255.0f;
						color *= scale;
						if(color < 0) color = 0;
						if(color > 1) color = 1;
						data[(y * textureWidth + x) * 4 + c] = (unsigned char)(color * 255.0f);
					}
					data[(y * textureWidth + x) * 4 + 3] = 0xff;
				}
			}

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			delete[] data;
			face.lightMapWidth = textureWidth;
			face.lightMapHeight = textureHeight;
			face.lightMapMinU = bspFace.lightmapTextureMinsInLuxels[0];
			face.lightMapMinV = bspFace.lightmapTextureMinsInLuxels[1];
		}

		const File::BSP::Plane bspPlane = mBSP->plane(bspFace.planeNum);
		face.plane = Geo::Plane(Geo::Vector(bspPlane.normal.x, bspPlane.normal.y, bspPlane.normal.z), bspPlane.dist);
		face.texture = &mTextures[texInfo.texdata];
		face.gray = (rand() % 255) / 255.0f;
		face.numVertices = bspFace.numEdges;
		face.vertices = new Geo::Vector[face.numVertices];

		for(int j=0; j<face.numVertices; j++) {
			int surfEdge = mBSP->surfEdge(bspFace.firstEdge + j);
			int vertex;
			if(surfEdge > 0) {
				vertex = mBSP->edge(surfEdge).v[0];
			} else {
				vertex = mBSP->edge(-surfEdge).v[1];
			}
			const File::BSP::Vector &bspVector = mBSP->vertex(vertex);

			face.vertices[j] = Geo::Vector(bspVector.x, bspVector.y, bspVector.z);
		}
	}

	mNumLeaves = mBSP->numLeaves();
	mLeaves = new Leaf[mNumLeaves];
	for(int i=0; i<mNumLeaves; i++) {
		const File::BSP::Leaf &bspLeaf = mBSP->leaf(i);
		Leaf &leaf = mLeaves[i];

		leaf.number = bspLeaf.cluster;
		Geo::Vector minPoint = Geo::Vector(bspLeaf.mins[0], bspLeaf.mins[1], bspLeaf.mins[2]);
		Geo::Vector maxPoint = Geo::Vector(bspLeaf.maxs[0], bspLeaf.maxs[1], bspLeaf.maxs[2]);
		leaf.bbox = Geo::Box(minPoint, maxPoint);
		leaf.numFaces = bspLeaf.numLeafFaces;
		leaf.faces = new Face*[leaf.numFaces];
		for(int j=0; j<leaf.numFaces; j++) {
			int leafFace = bspLeaf.firstLeafFace + j;
			leaf.faces[j] = &mFaces[mBSP->leafFace(leafFace)];
		}

		if(bspLeaf.cluster == -1) {
			leaf.visibleLeaves = 0;
		} else {
			leaf.visibleLeaves = new bool[mBSP->numClusters()];
			for(int j=0; j<mBSP->numClusters(); j++) {
				leaf.visibleLeaves[j] = mBSP->clusterVisibleFrom(bspLeaf.cluster, j);
			}
		}
	}

	mNumNodes = mBSP->numNodes();
	mNodes = new Node[mNumNodes];
	for(int i=0; i<mNumNodes; i++) {
		const File::BSP::Node &bspNode = mBSP->node(i);
		Node &node = mNodes[i];

		const File::BSP::Plane &bspPlane = mBSP->plane(bspNode.planeNum);
		node.plane = Geo::Plane(Geo::Vector(bspPlane.normal.x, bspPlane.normal.y, bspPlane.normal.z), bspPlane.dist);

		Geo::Vector minPoint = Geo::Vector(bspNode.mins[0], bspNode.mins[1], bspNode.mins[2]);
		Geo::Vector maxPoint = Geo::Vector(bspNode.maxs[0], bspNode.maxs[1], bspNode.maxs[2]);
		node.bbox = Geo::Box(minPoint, maxPoint);

		for(int j=0; j<2; j++) {
			int child = bspNode.children[j];
			if(child > 0) {
				node.children[j] = &mNodes[child];
			} else {
				node.children[j] = &mLeaves[-child - 1];
			}
		}
	}
}

Map::Node *Map::rootNode()
{
	const File::BSP::Model &model = mBSP->model(0);
	return &mNodes[model.headNode];
}