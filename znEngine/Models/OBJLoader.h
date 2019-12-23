#pragma once

#include "Materials/OBJMaterial.h"

// Print progress to console while loading (large models)
//#define OBJL_CONSOLE_OUTPUT

// Namespace: OBJL
//
// Description: The namespace that holds eveyrthing that
//	is needed and used for the OBJ Model Loader
namespace objl
{
	// Structure: Vertex
	//
	// Description: Model Vertex object that holds
	//	a Position, Normal, and Texture Coordinate
	struct Vertex
	{
		// Position Vector
		glm::vec3 Position;

		// Normal Vector
		glm::vec3 Normal;

		// Texture Coordinate Vector
		glm::vec2 TextureCoordinate;
	};

	struct Material
	{
		Material()
		{
			name;
			Ns = 0.0f;
			Ni = 0.0f;
			d = 0.0f;
			illum = 0;
		}

		// Material Name
		std::string name;
		// Ambient Color
		glm::vec3 Ka;
		// Diffuse Color
		glm::vec3 Kd;
		// Specular Color
		glm::vec3 Ks;
		// Specular Exponent
		float Ns;
		// Optical Density
		float Ni;
		// Dissolve
		float d;
		// Illumination
		int illum;
		// Ambient Texture Map
		std::string map_Ka;
		// Diffuse Texture Map
		std::string map_Kd;
		// Specular Texture Map
		std::string map_Ks;
		// Specular Hightlight Map
		std::string map_Ns;
		// Alpha Texture Map
		std::string map_d;
		// Bump Map
		std::string map_bump;
	};

	// Structure: Mesh
	//
	// Description: A Simple Mesh Object that holds
	//	a name, a vertex list, and an index list
	struct Mesh
	{
		// Default Constructor
		Mesh()
		{

		}

		// Variable Set Constructor
		Mesh(std::vector<Vertex>& _Vertices, std::vector<unsigned int>& _Indices)
		{
			Vertices = _Vertices;
			Indices = _Indices;
		}

		// Mesh Name
		std::string MeshName;

		// Vertex List
		std::vector<Vertex> Vertices;

		// Index List
		std::vector<unsigned int> Indices;

		// Material
		std::shared_ptr<COBJMaterial> MeshMaterial;
	};

	// Namespace: Math
	//
	// Description: The namespace that holds all of the math
	//	functions need for OBJL
	namespace math
	{
		// glm::vec3 Cross Product
		inline glm::vec3 CrossV3(const glm::vec3 a, const glm::vec3 b)
		{
			return glm::vec3(a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x);
		}

		// glm::vec3 Magnitude Calculation
		inline float MagnitudeV3(const glm::vec3 in)
		{
			return (sqrtf(powf(in.x, 2) + powf(in.y, 2) + powf(in.z, 2)));
		}

		// glm::vec3 DotProduct
		inline float DotV3(const glm::vec3 a, const glm::vec3 b)
		{
			return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
		}

		// Angle between 2 glm::vec3 Objects
		inline float AngleBetweenV3(const glm::vec3 a, const glm::vec3 b)
		{
			float angle = DotV3(a, b);
			angle /= (MagnitudeV3(a) * MagnitudeV3(b));
			return angle = acosf(angle);
		}

		// Projection Calculation of a onto b
		inline glm::vec3 ProjV3(const glm::vec3 a, const glm::vec3 b)
		{
			glm::vec3 bn = b / MagnitudeV3(b);
			return bn * DotV3(a, bn);
		}
	}

	// Namespace: Algorithm
	//
	// Description: The namespace that holds all of the
	// Algorithms needed for OBJL
	namespace algorithm
	{
		// A test to see if P1 is on the same side as P2 of a line segment ab
		inline bool SameSide(glm::vec3 p1, glm::vec3 p2, glm::vec3 a, glm::vec3 b)
		{
			glm::vec3 cp1 = math::CrossV3(b - a, p1 - a);
			glm::vec3 cp2 = math::CrossV3(b - a, p2 - a);

			if (math::DotV3(cp1, cp2) >= 0)
				return true;
			else
				return false;
		}

		// Generate a cross produect normal for a triangle
		inline glm::vec3 GenTriNormal(glm::vec3 t1, glm::vec3 t2, glm::vec3 t3)
		{
			glm::vec3 u = t2 - t1;
			glm::vec3 v = t3 - t1;

			glm::vec3 normal = math::CrossV3(u, v);

			return normal;
		}

		// Check to see if a glm::vec3 Point is within a 3 glm::vec3 Triangle
		inline bool inTriangle(glm::vec3 point, glm::vec3 tri1, glm::vec3 tri2, glm::vec3 tri3)
		{
			// Test to see if it is within an infinite prism that the triangle outlines.
			bool within_tri_prisim = SameSide(point, tri1, tri2, tri3) && SameSide(point, tri2, tri1, tri3)
				&& SameSide(point, tri3, tri1, tri2);

			// If it isn't it will never be on the triangle
			if (!within_tri_prisim)
				return false;

			// Calulate Triangle's Normal
			glm::vec3 n = GenTriNormal(tri1, tri2, tri3);

			// Project the point onto this normal
			glm::vec3 proj = math::ProjV3(point, n);

			// If the distance from the triangle to the point is 0
			//	it lies on the triangle
			if (math::MagnitudeV3(proj) == 0)
				return true;
			else
				return false;
		}

		// Split a String into a string array at a given token
		inline void split(const std::string &in,
			std::vector<std::string> &out,
			std::string token)
		{
			out.clear();

			std::string temp;

			for (int i = 0; i < int(in.size()); i++)
			{
				std::string test = in.substr(i, token.size());

				if (test == token)
				{
					if (!temp.empty())
					{
						out.push_back(temp);
						temp.clear();
						i += (int)token.size() - 1;
					}
					else
					{
						out.push_back("");
					}
				}
				else if (i + token.size() >= in.size())
				{
					temp += in.substr(i, token.size());
					out.push_back(temp);
					break;
				}
				else
				{
					temp += in[i];
				}
			}
		}

		// Get tail of string after first token and possibly following spaces
		inline std::string tail(const std::string &in)
		{
			size_t token_start = in.find_first_not_of(" \t");
			size_t space_start = in.find_first_of(" \t", token_start);
			size_t tail_start = in.find_first_not_of(" \t", space_start);
			size_t tail_end = in.find_last_not_of(" \t");
			if (tail_start != std::string::npos && tail_end != std::string::npos)
			{
				return in.substr(tail_start, tail_end - tail_start + 1);
			}
			else if (tail_start != std::string::npos)
			{
				return in.substr(tail_start);
			}
			return "";
		}

		// Get first token of string
		inline std::string firstToken(const std::string &in)
		{
			if (!in.empty())
			{
				size_t token_start = in.find_first_not_of(" \t");
				size_t token_end = in.find_first_of(" \t", token_start);
				if (token_start != std::string::npos && token_end != std::string::npos)
				{
					return in.substr(token_start, token_end - token_start);
				}
				else if (token_start != std::string::npos)
				{
					return in.substr(token_start);
				}
			}
			return "";
		}

		// Get element at given index position
		template <class T>
		inline const T & getElement(const std::vector<T> &elements, std::string &index)
		{
			int idx = std::stoi(index);
			if (idx < 0)
				idx = int(elements.size()) + idx;
			else
				idx--;
			return elements[idx];
		}
	}

	// Class: Loader
	//
	// Description: The OBJ Model Loader
	class Loader
	{
	public:
		// Default Constructor
		Loader(std::shared_ptr<IRenderDevice> RenderDevice)
			: m_RenderDevice(RenderDevice)
		{

		}
		~Loader()
		{
			LoadedMeshes.clear();
		}

		// Load a file into the loader
		//
		// If file is loaded return true
		//
		// If the file is unable to be found or unable to be loaded return false
		bool LoadFile(std::shared_ptr<IFile> File);

		// Loaded Mesh Objects
		std::vector<Mesh> LoadedMeshes;

		// Loaded Vertex Objects
		std::vector<Vertex> LoadedVertices;

		// Loaded Index Positions
		std::vector<unsigned int> LoadedIndices;

		// Loaded Material Objects
		std::vector<std::shared_ptr<COBJMaterial>> LoadedMaterials;

	private:
		// Generate vertices from a list of positions, tcoords, normals and a face line
		void GenVerticesFromRawOBJ(std::vector<Vertex>& oVerts, const std::vector<glm::vec3>& iPositions, const std::vector<glm::vec2>& iTCoords, const std::vector<glm::vec3>& iNormals, std::string icurline);
		
		// Triangulate a list of vertices into a face by printing inducies corresponding with triangles within it
		void VertexTriangluation(std::vector<unsigned int>& oIndices, const std::vector<Vertex>& iVerts);
		
		// Load Materials from .mtl file
		bool LoadMaterials(std::shared_ptr<IFile> File);

	private:
		std::shared_ptr<IRenderDevice> m_RenderDevice;
	};
}