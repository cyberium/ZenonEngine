#include "stdafx.h"

// Gerenal
#include "OBJLoader.h"

using namespace objl;

bool Loader::LoadFile(std::shared_ptr<IFile> File)
{
	// If the file is not an .obj file return false
	if (File->Extension() != "obj")
		return false;


	LoadedMeshes.clear();
	LoadedVertices.clear();
	LoadedIndices.clear();

	std::vector<glm::vec3> Positions;
	std::vector<glm::vec2> TCoords;
	std::vector<glm::vec3> Normals;

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	std::vector<std::string> MeshMatNames;

	bool listening = false;
	std::string meshname;

	Mesh tempMesh;

#ifdef OBJL_CONSOLE_OUTPUT
	const unsigned int outputEveryNth = 1000;
	unsigned int outputIndicator = outputEveryNth;
#endif

	std::string curline;
	while (File->readLine(&curline))
	{
#ifdef OBJL_CONSOLE_OUTPUT
		if ((outputIndicator = ((outputIndicator + 1) % outputEveryNth)) == 1)
		{
			if (!meshname.empty())
			{
				std::cout
					<< "\r- " << meshname
					<< "\t| vertices > " << Positions.size()
					<< "\t| texcoords > " << TCoords.size()
					<< "\t| normals > " << Normals.size()
					<< "\t| triangles > " << (Vertices.size() / 3)
					<< (!MeshMatNames.empty() ? "\t| material: " + MeshMatNames.back() : "");
			}
		}
#endif

		// Generate a Mesh Object or Prepare for an object to be created
		if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g" || curline[0] == 'g')
		{
			if (!listening)
			{
				listening = true;

				if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g")
				{
					meshname = algorithm::tail(curline);
				}
				else
				{
					meshname = "unnamed";
				}
			}
			else
			{
				// Generate the mesh to put into the array

				if (!Indices.empty() && !Vertices.empty())
				{
					// Create Mesh
					tempMesh = Mesh(Vertices, Indices);
					tempMesh.MeshName = meshname;

					// Insert Mesh
					LoadedMeshes.push_back(tempMesh);

					// Cleanup
					Vertices.clear();
					Indices.clear();
					meshname.clear();

					meshname = algorithm::tail(curline);
				}
				else
				{
					if (algorithm::firstToken(curline) == "o" || algorithm::firstToken(curline) == "g")
					{
						meshname = algorithm::tail(curline);
					}
					else
					{
						meshname = "unnamed";
					}
				}
			}
#ifdef OBJL_CONSOLE_OUTPUT
			std::cout << std::endl;
			outputIndicator = 0;
#endif
		}
		// Generate a Vertex Position
		if (algorithm::firstToken(curline) == "v")
		{
			std::vector<std::string> spos;
			glm::vec3 vpos;
			algorithm::split(algorithm::tail(curline), spos, " ");

			vpos.x = std::stof(spos[0]);
			vpos.y = std::stof(spos[1]);
			vpos.z = std::stof(spos[2]);

			Positions.push_back(vpos);
		}
		// Generate a Vertex Texture Coordinate
		if (algorithm::firstToken(curline) == "vt")
		{
			std::vector<std::string> stex;
			glm::vec2 vtex;
			algorithm::split(algorithm::tail(curline), stex, " ");

			vtex.x = std::stof(stex[0]);
			vtex.y = std::stof(stex[1]);

			TCoords.push_back(vtex);
		}
		// Generate a Vertex Normal;
		if (algorithm::firstToken(curline) == "vn")
		{
			std::vector<std::string> snor;
			glm::vec3 vnor;
			algorithm::split(algorithm::tail(curline), snor, " ");

			vnor.x = std::stof(snor[0]);
			vnor.y = std::stof(snor[1]);
			vnor.z = std::stof(snor[2]);

			Normals.push_back(vnor);
		}
		// Generate a Face (vertices & indices)
		if (algorithm::firstToken(curline) == "f")
		{
			// Generate the vertices
			std::vector<Vertex> vVerts;
			GenVerticesFromRawOBJ(vVerts, Positions, TCoords, Normals, curline);

			// Add Vertices
			for (int i = 0; i < int(vVerts.size()); i++)
			{
				Vertices.push_back(vVerts[i]);

				LoadedVertices.push_back(vVerts[i]);
			}

			std::vector<unsigned int> iIndices;

			VertexTriangluation(iIndices, vVerts);

			// Add Indices
			for (int i = 0; i < int(iIndices.size()); i++)
			{
				unsigned int indnum = (unsigned int)((Vertices.size()) - vVerts.size()) + iIndices[i];
				Indices.push_back(indnum);

				indnum = (unsigned int)((LoadedVertices.size()) - vVerts.size()) + iIndices[i];
				LoadedIndices.push_back(indnum);

			}
		}
		// Get Mesh Material Name
		if (algorithm::firstToken(curline) == "usemtl")
		{
			MeshMatNames.push_back(algorithm::tail(curline));

			// Create new Mesh, if Material changes within a group
			if (!Indices.empty() && !Vertices.empty())
			{
				// Create Mesh
				tempMesh = Mesh(Vertices, Indices);
				tempMesh.MeshName = meshname;
				int i = 2;
				while (1) {
					tempMesh.MeshName = meshname + "_" + std::to_string(i);

					for (auto &m : LoadedMeshes)
						if (m.MeshName == tempMesh.MeshName)
							continue;
					break;
				}

				// Insert Mesh
				LoadedMeshes.push_back(tempMesh);

				// Cleanup
				Vertices.clear();
				Indices.clear();
			}

#ifdef OBJL_CONSOLE_OUTPUT
			outputIndicator = 0;
#endif
		}
		// Load Materials
		if (algorithm::firstToken(curline) == "mtllib")
		{
			std::shared_ptr<IFile> materialsFile = GetManager<IFilesManager>(m_RenderDevice->GetBaseManager())->Open(File->Path() + "\\" + algorithm::tail(curline));

			// Load Materials
			LoadMaterials(materialsFile);
		}
	}

#ifdef OBJL_CONSOLE_OUTPUT
	std::cout << std::endl;
#endif

	// Deal with last mesh

	if (!Indices.empty() && !Vertices.empty())
	{
		// Create Mesh
		tempMesh = Mesh(Vertices, Indices);
		tempMesh.MeshName = meshname;

		// Insert Mesh
		LoadedMeshes.push_back(tempMesh);
	}

	// Set Materials for each Mesh
	for (int i = 0; i < MeshMatNames.size(); i++)
	{
		std::string matname = MeshMatNames[i];

		// Find corresponding material name in loaded materials when found copy material variables into mesh material
		for (int j = 0; j < LoadedMaterials.size(); j++)
		{
			if (LoadedMaterials[j]->GetName() == matname)
			{
				LoadedMeshes[i].MeshMaterial = LoadedMaterials[j];
				break;
			}
		}
	}

	if (LoadedMeshes.empty() && LoadedVertices.empty() && LoadedIndices.empty())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Loader::GenVerticesFromRawOBJ(std::vector<Vertex>& oVerts, const std::vector<glm::vec3>& iPositions, const std::vector<glm::vec2>& iTCoords, const std::vector<glm::vec3>& iNormals,
	std::string icurline)
{
	std::vector<std::string> sface, svert;
	Vertex vVert;
	algorithm::split(algorithm::tail(icurline), sface, " ");

	bool noNormal = false;

	// For every given vertex do this
	for (int i = 0; i < int(sface.size()); i++)
	{
		// See What type the vertex is.
		int vtype;

		algorithm::split(sface[i], svert, "/");

		// Check for just position - v1
		if (svert.size() == 1)
		{
			// Only position
			vtype = 1;
		}

		// Check for position & texture - v1/vt1
		if (svert.size() == 2)
		{
			// Position & Texture
			vtype = 2;
		}

		// Check for Position, Texture and Normal - v1/vt1/vn1
		// or if Position and Normal - v1//vn1
		if (svert.size() == 3)
		{
			if (svert[1] != "")
			{
				// Position, Texture, and Normal
				vtype = 4;
			}
			else
			{
				// Position & Normal
				vtype = 3;
			}
		}

		// Calculate and store the vertex
		switch (vtype)
		{
		case 1: // P
		{
			vVert.Position = algorithm::getElement(iPositions, svert[0]);
			vVert.TextureCoordinate = glm::vec2(0, 0);
			noNormal = true;
			oVerts.push_back(vVert);
			break;
		}
		case 2: // P/T
		{
			vVert.Position = algorithm::getElement(iPositions, svert[0]);
			vVert.TextureCoordinate = algorithm::getElement(iTCoords, svert[1]);
			noNormal = true;
			oVerts.push_back(vVert);
			break;
		}
		case 3: // P//N
		{
			vVert.Position = algorithm::getElement(iPositions, svert[0]);
			vVert.TextureCoordinate = glm::vec2(0, 0);
			vVert.Normal = algorithm::getElement(iNormals, svert[2]);
			oVerts.push_back(vVert);
			break;
		}
		case 4: // P/T/N
		{
			vVert.Position = algorithm::getElement(iPositions, svert[0]);
			vVert.TextureCoordinate = algorithm::getElement(iTCoords, svert[1]);
			vVert.Normal = algorithm::getElement(iNormals, svert[2]);
			oVerts.push_back(vVert);
			break;
		}
		default:
		{
			break;
		}
		}
	}

	// take care of missing normals
	// these may not be truly acurate but it is the 
	// best they get for not compiling a mesh with normals	
	if (noNormal)
	{
		glm::vec3 A = oVerts[0].Position - oVerts[1].Position;
		glm::vec3 B = oVerts[2].Position - oVerts[1].Position;

		glm::vec3 normal = math::CrossV3(A, B);

		for (int i = 0; i < int(oVerts.size()); i++)
		{
			oVerts[i].Normal = normal;
		}
	}
}

void Loader::VertexTriangluation(std::vector<unsigned int>& oIndices, const std::vector<Vertex>& iVerts)
{
	// If there are 2 or less verts,
	// no triangle can be created,
	// so exit
	if (iVerts.size() < 3)
	{
		return;
	}
	// If it is a triangle no need to calculate it
	if (iVerts.size() == 3)
	{
		oIndices.push_back(0);
		oIndices.push_back(1);
		oIndices.push_back(2);
		return;
	}

	// Create a list of vertices
	std::vector<Vertex> tVerts = iVerts;

	while (true)
	{
		// For every vertex
		for (int i = 0; i < int(tVerts.size()); i++)
		{
			// pPrev = the previous vertex in the list
			Vertex pPrev;
			if (i == 0)
			{
				pPrev = tVerts[tVerts.size() - 1];
			}
			else
			{
				pPrev = tVerts[i - 1];
			}

			// pCur = the current vertex;
			Vertex pCur = tVerts[i];

			// pNext = the next vertex in the list
			Vertex pNext;
			if (i == tVerts.size() - 1)
			{
				pNext = tVerts[0];
			}
			else
			{
				pNext = tVerts[i + 1];
			}

			// Check to see if there are only 3 verts left
			// if so this is the last triangle
			if (tVerts.size() == 3)
			{
				// Create a triangle from pCur, pPrev, pNext
				for (int j = 0; j < int(tVerts.size()); j++)
				{
					if (iVerts[j].Position == pCur.Position)
						oIndices.push_back(j);
					if (iVerts[j].Position == pPrev.Position)
						oIndices.push_back(j);
					if (iVerts[j].Position == pNext.Position)
						oIndices.push_back(j);
				}

				tVerts.clear();
				break;
			}
			if (tVerts.size() == 4)
			{
				// Create a triangle from pCur, pPrev, pNext
				for (int j = 0; j < int(iVerts.size()); j++)
				{
					if (iVerts[j].Position == pCur.Position)
						oIndices.push_back(j);
					if (iVerts[j].Position == pPrev.Position)
						oIndices.push_back(j);
					if (iVerts[j].Position == pNext.Position)
						oIndices.push_back(j);
				}

				glm::vec3 tempVec;
				for (int j = 0; j < int(tVerts.size()); j++)
				{
					if (tVerts[j].Position != pCur.Position
						&& tVerts[j].Position != pPrev.Position
						&& tVerts[j].Position != pNext.Position)
					{
						tempVec = tVerts[j].Position;
						break;
					}
				}

				// Create a triangle from pCur, pPrev, pNext
				for (int j = 0; j < int(iVerts.size()); j++)
				{
					if (iVerts[j].Position == pPrev.Position)
						oIndices.push_back(j);
					if (iVerts[j].Position == pNext.Position)
						oIndices.push_back(j);
					if (iVerts[j].Position == tempVec)
						oIndices.push_back(j);
				}

				tVerts.clear();
				break;
			}

			// If Vertex is not an interior vertex
			float angle = math::AngleBetweenV3(pPrev.Position - pCur.Position, pNext.Position - pCur.Position) * (180 / 3.14159265359);
			if (angle <= 0 && angle >= 180)
				continue;

			// If any vertices are within this triangle
			bool inTri = false;
			for (int j = 0; j < int(iVerts.size()); j++)
			{
				if (algorithm::inTriangle(iVerts[j].Position, pPrev.Position, pCur.Position, pNext.Position)
					&& iVerts[j].Position != pPrev.Position
					&& iVerts[j].Position != pCur.Position
					&& iVerts[j].Position != pNext.Position)
				{
					inTri = true;
					break;
				}
			}
			if (inTri)
				continue;

			// Create a triangle from pCur, pPrev, pNext
			for (int j = 0; j < int(iVerts.size()); j++)
			{
				if (iVerts[j].Position == pCur.Position)
					oIndices.push_back(j);
				if (iVerts[j].Position == pPrev.Position)
					oIndices.push_back(j);
				if (iVerts[j].Position == pNext.Position)
					oIndices.push_back(j);
			}

			// Delete pCur from the list
			for (int j = 0; j < int(tVerts.size()); j++)
			{
				if (tVerts[j].Position == pCur.Position)
				{
					tVerts.erase(tVerts.begin() + j);
					break;
				}
			}

			// reset i to the start
			// -1 since loop will add 1 to it
			i = -1;
		}

		// if no triangles were created
		if (oIndices.size() == 0)
			break;

		// if no more vertices
		if (tVerts.size() == 0)
			break;
	}
}



bool Loader::LoadMaterials(std::shared_ptr<IFile> File)
{
	// If the file is not a material file return false
	if (File->Extension() != "mtl")
		return false;

	std::shared_ptr<COBJMaterial> tempMaterial = std::make_shared<COBJMaterial>(m_RenderDevice);
	tempMaterial->SetWrapper(tempMaterial);

	bool listening = false;

	// Go through each line looking for material variables
	std::string curline;
	while (File->readLine(&curline))
	{
		// new material and material name
		if (algorithm::firstToken(curline) == "newmtl")
		{
			if (!listening)
			{
				listening = true;

				if (curline.size() > 7)
				{
					tempMaterial->SetName(algorithm::tail(curline));
				}
				else
				{
					tempMaterial->SetName("COBJMaterial");
				}
			}
			else
			{
				// Generate the material

				// Push Back loaded Material
				LoadedMaterials.push_back(tempMaterial);



				// Clear Loaded Material
				tempMaterial = std::make_shared<COBJMaterial>(m_RenderDevice);
				tempMaterial->SetWrapper(tempMaterial);
				if (curline.size() > 7)
				{
					tempMaterial->SetName(algorithm::tail(curline));
				}
				else
				{
					tempMaterial->SetName("COBJMaterial");
				}
			}
		}
		// Ambient Color
		if (algorithm::firstToken(curline) == "Ka")
		{
			std::vector<std::string> temp;
			algorithm::split(algorithm::tail(curline), temp, " ");

			if (temp.size() != 3)
				continue;

			tempMaterial->Set_Ka(glm::vec3(std::stof(temp[0]), std::stof(temp[1]), std::stof(temp[2])));
		}
		// Diffuse Color
		if (algorithm::firstToken(curline) == "Kd")
		{
			std::vector<std::string> temp;
			algorithm::split(algorithm::tail(curline), temp, " ");

			if (temp.size() != 3)
				continue;

			tempMaterial->Set_Kd(glm::vec3(std::stof(temp[0]), std::stof(temp[1]), std::stof(temp[2])));
		}
		// Specular Color
		if (algorithm::firstToken(curline) == "Ks")
		{
			std::vector<std::string> temp;
			algorithm::split(algorithm::tail(curline), temp, " ");

			if (temp.size() != 3)
				continue;

			tempMaterial->Set_Ks(glm::vec3(std::stof(temp[0]), std::stof(temp[1]), std::stof(temp[2])));
		}
		// Specular Exponent
		if (algorithm::firstToken(curline) == "Ns")
		{
			tempMaterial->Set_Ns(std::stof(algorithm::tail(curline)));
		}
		// Optical Density
		if (algorithm::firstToken(curline) == "Ni")
		{
			tempMaterial->Set_Ni(std::stof(algorithm::tail(curline)));
		}
		// Dissolve
		if (algorithm::firstToken(curline) == "d")
		{
			tempMaterial->Set_d(std::stof(algorithm::tail(curline)));
		}
		// Illumination
		if (algorithm::firstToken(curline) == "illum")
		{
			tempMaterial->Set_illum(std::stoi(algorithm::tail(curline)));
		}
		// Ambient Texture Map
		if (algorithm::firstToken(curline) == "map_Ka")
		{
			std::shared_ptr<ITexture> texture = m_RenderDevice->CreateTexture2D(File->Path() + "\\" + algorithm::tail(curline));
			tempMaterial->Set_AmbientTexture(texture);
		}
		// Diffuse Texture Map
		if (algorithm::firstToken(curline) == "map_Kd")
		{
			std::shared_ptr<ITexture> texture = m_RenderDevice->CreateTexture2D(File->Path() + "\\" + algorithm::tail(curline));
			tempMaterial->Set_DiffuseTexture(texture);
		}
		// Specular Texture Map
		if (algorithm::firstToken(curline) == "map_Ks")
		{
			std::shared_ptr<ITexture> texture = m_RenderDevice->CreateTexture2D(File->Path() + "\\" + algorithm::tail(curline));
			tempMaterial->Set_SpecularTexture(texture);
		}
		// Specular Hightlight Map
		if (algorithm::firstToken(curline) == "map_Ns")
		{
			std::shared_ptr<ITexture> texture = m_RenderDevice->CreateTexture2D(File->Path() + "\\" + algorithm::tail(curline));
			tempMaterial->Set_SpecularHightlightTexture(texture);
		}
		// Alpha Texture Map
		if (algorithm::firstToken(curline) == "map_d")
		{
			std::shared_ptr<ITexture> texture = m_RenderDevice->CreateTexture2D(File->Path() + "\\" + algorithm::tail(curline));
			tempMaterial->Set_AlphaTexture(texture);
		}
		// Bump Map
		if (algorithm::firstToken(curline) == "map_Bump" || algorithm::firstToken(curline) == "map_bump" || algorithm::firstToken(curline) == "bump")
		{
			std::shared_ptr<ITexture> texture = m_RenderDevice->CreateTexture2D(File->Path() + "\\" + algorithm::tail(curline));
			tempMaterial->Set_BumpTexture(texture);
		}
	}

	// Deal with last material

	// Push Back loaded Material
	LoadedMaterials.push_back(tempMaterial);

	// Test to see if anything was loaded
	// If not return false
	if (LoadedMaterials.empty())
		return false;
	// If so return true
	else
		return true;
}