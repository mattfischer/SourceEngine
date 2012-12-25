#include "Map.hpp"

#include "File/VMT.hpp"
#include "File/MDL.hpp"
#include "File/VVD.hpp"
#include "File/VTX.hpp"

#include <math.h>

Map::Map(File::IReaderFactory *factory, const std::string &name)
{
	mBSP = File::BSP::open(factory, name);

	const File::BSP::Model &model = mBSP->model(0);

	mNumTextures = mBSP->numTexDatas();
	mTextures = new Texture[mNumTextures];
	for(unsigned int i=0; i<mNumTextures; i++) {
		const File::BSP::TexData &texData = mBSP->texData(i);
		const std::string &materialFilename = mBSP->texDataString(texData.nameStringTableID);

		mTextures[i].vtf = 0;
		File::VMT *vmt = File::VMT::open(factory, "materials/" + materialFilename + ".vmt");
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
	for(unsigned int i=0; i<mNumFaces; i++) {
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
		face.vertices = new Geo::Point[face.numVertices];

		for(int j=0; j<face.numVertices; j++) {
			int surfEdge = mBSP->surfEdge(bspFace.firstEdge + j);
			int vertex;
			if(surfEdge > 0) {
				vertex = mBSP->edge(surfEdge).v[0];
			} else {
				vertex = mBSP->edge(-surfEdge).v[1];
			}
			const File::BSP::Vector &bspVector = mBSP->vertex(vertex);

			face.vertices[j] = Geo::Point(bspVector.x, bspVector.y, bspVector.z);
		}
	}

	mNumLeaves = mBSP->numLeaves();
	mLeaves = new Leaf[mNumLeaves];
	for(unsigned int i=0; i<mNumLeaves; i++) {
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
			for(unsigned int j=0; j<mBSP->numClusters(); j++) {
				leaf.visibleLeaves[j] = mBSP->clusterVisibleFrom(bspLeaf.cluster, j);
			}
		}
	}

	mNumNodes = mBSP->numNodes();
	mNodes = new Node[mNumNodes];
	for(unsigned int i=0; i<mNumNodes; i++) {
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

	mNumEntities = mBSP->numEntities();
	mEntities = new Entity[mNumEntities];

	for(unsigned int i=0; i<mBSP->numEntities(); i++) {
		const File::BSP::Entity &bspEntity = mBSP->entity(i);
		Entity &entity = mEntities[i];

		entity.model = 0;

		if(bspEntity.section->hasParameter("origin")) {
			const std::string &position = bspEntity.section->parameter("origin");
			std::vector<std::string> posParts = StringUtils::split(position, " ");
			float x = (float)atof(posParts[0].c_str());
			float y = (float)atof(posParts[1].c_str());
			float z = (float)atof(posParts[2].c_str());
			entity.position = Geo::Point(x, y, z);
		}

		if(bspEntity.section->hasParameter("angles")) {
			const std::string &angles = bspEntity.section->parameter("angles");
			std::vector<std::string> angleParts = StringUtils::split(angles, " ");
			float pitch = (float)atof(angleParts[0].c_str());
			float yaw = (float)atof(angleParts[1].c_str());
			float roll = (float)atof(angleParts[2].c_str());
			entity.pitch = pitch;
			entity.yaw = yaw;
			entity.roll = roll;
		}

		if(bspEntity.section->hasParameter("model")) {
			const std::string &name = bspEntity.section->parameter("model");

			size_t pos = name.find(".mdl");
			if(name[0] != '*' && pos != name.npos) {
				File::MDL *mdl = File::MDL::open(factory, name);

				std::string vertices = name;
				vertices.replace(pos, 4, ".vvd");
				File::VVD *vvd = File::VVD::open(factory, vertices);

				std::string mesh = name;
				mesh.replace(pos, 4, ".vtx");
				File::VTX *vtx = File::VTX::open(factory, mesh);

				if(mdl != 0 && vvd != 0 && vtx != 0) {
					Model *model = new Model;
					model->mdl = mdl;
					model->vvd = vvd;
					model->vtx = vtx;

					model->numTextures = mdl->numTextures();
					model->textures = new Texture[model->numTextures];
					for(int j=0; j<model->numTextures; j++) {
						File::VMT *vmt = 0; //File::VMT::open(factory, "materials/" + mdl->texture(j) + ".vmt");
						if(vmt && vmt->hasParameter("$basetexture")) {
							const std::string &textureFilename = vmt->parameter("$basetexture");

							File::VTF *vtf = File::VTF::open(factory, textureFilename);
							delete vmt;

							model->textures[j].vtf = vtf;
							glGenTextures(1, &model->textures[j].tex);
							glBindTexture(GL_TEXTURE_2D, model->textures[j].tex);
							glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
							glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

							for(int k=0; k<vtf->numMipMaps(); k++) {
								glTexImage2D(GL_TEXTURE_2D, vtf->numMipMaps() - k - 1, GL_RGBA, vtf->width(k), vtf->height(k), 0, GL_RGBA, GL_UNSIGNED_BYTE, vtf->data(k));
							}
						}
					}

					entity.model = model;
				}
			}
		}
	}
}

Map::Node *Map::rootNode()
{
	const File::BSP::Model &model = mBSP->model(0);
	return &mNodes[model.headNode];
}