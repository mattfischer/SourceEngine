#include "Map.hpp"

#include "File/VMT.hpp"

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
		if(vmt) {
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
		Face &face = mFaces[i];

		face.texInfo = bspFace.texInfo;
		face.texture = &mTextures[mBSP->texInfo(face.texInfo).texdata];
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
}