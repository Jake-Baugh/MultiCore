#include "Mesh.h"
using namespace std;

Mesh::Mesh()
{
	m_meshTriangles.clear();
	m_material = Material();
	m_nrOfFaces = 0;
}

Mesh::~Mesh()
{
	m_meshTriangles.clear();
}

void Mesh::init()
{

}

void Mesh::loadObj(char* textFile)
{
	
	vector<XMVECTOR> positions;
	vector<XMVECTOR> normals;
	vector<XMFLOAT2> texCoords;

	fstream file( textFile );

	string srcMaterial;
	string line;
	string prefix;
	int id = 0;
	while( !file.eof() )
	{
		prefix = "NULL";
		stringstream lineStream;

		getline( file, line );
		lineStream << line;
		lineStream >> prefix;
		if(prefix == "mtllib")
		{	
			stringstream ss;
			string s;
			lineStream >> s;
			ss << "Meshi/";
			ss << s;
			ss >> prefix;
			loadMaterial(prefix.c_str());
		}

		if( prefix == "v" )
		{
			XMVECTOR pos;
			lineStream >> pos.m128_f32[0] >> pos.m128_f32[1] >> pos.m128_f32[2];
			pos.m128_f32[2] *= -1.f;
			pos.m128_f32[3] = 1.0f;
			positions.push_back( pos );
		}
		else if( prefix == "vn" )
		{
			XMVECTOR normal;
			lineStream >> normal.m128_f32[0] >> normal.m128_f32[1] >> normal.m128_f32[2];
			normal.m128_f32[2] *= -1.f;
			normal.m128_f32[3] = 0.0f;
			normals.push_back( normal );
		}
		else if( prefix == "vt" )
		{
			XMFLOAT2 texC;
			lineStream >> texC.x >> texC.y;
			texC.y = 1 - texC.y;
			texCoords.push_back( texC );
		}
		else if( prefix == "f" )
		{
			m_nrOfFaces++;
			unsigned int iPosition, iTexCoord, iNormal;
			MeshTriangle vertex;
			char slash;
	
			lineStream >> iPosition >> slash >> iTexCoord >> slash >> iNormal;
			vertex.pos0		= positions	[ iPosition - 1 ];
			vertex.normal	= normals	[ iNormal - 1 ];
			vertex.textureCoordinate0 = texCoords	[ iTexCoord - 1 ];
			//pos1
			lineStream >> iPosition >> slash >> iTexCoord >> slash >> iNormal;
			vertex.pos1		= positions	[ iPosition - 1 ];
			vertex.textureCoordinate1 = texCoords	[ iTexCoord - 1 ];
			//pos2
			lineStream >> iPosition >> slash >> iTexCoord >> slash >> iNormal;
			vertex.pos2		= positions	[ iPosition - 1 ];
			vertex.textureCoordinate2 = texCoords	[ iTexCoord - 1 ];
			vertex.ID = id;
			m_meshTriangles.push_back( vertex );
			id++;
			vertex.pad = 1.0f;
		}
	}
	m_meshTriangles.shrink_to_fit();
	file.close();
	positions.clear();
	normals.clear();
	texCoords.clear();
}

void Mesh::loadMaterial(string filename)
{
	fstream file( filename.c_str() );

	string line;
	string prefix;
	
	while( !file.eof() )
	{
		prefix = "NULL";
		stringstream lineStream;
		
		getline( file, line );
		lineStream << line;
		lineStream >> prefix;

		if(prefix == "Ns")
		{
			lineStream >> m_material.Ns;
		}
		if( prefix == "Ni" )
		{
			lineStream >> m_material.Ni;
			m_material2.ni = m_material.Ni;
		}
		else if( prefix == "d" )
		{
			lineStream >> m_material.d;
		}
		else if( prefix == "Tr" )
		{
			lineStream >> m_material.Tr;
		}
		else if( prefix == "Tf" )
		{
			lineStream >> m_material.Tf.x >> m_material.Tf.y >> m_material.Tf.z;
			m_material2.tf = XMVectorSet(m_material.Tf.x, m_material.Tf.y, m_material.Tf.z, 0.f);
		}
		else if( prefix == "illum" )
		{
			lineStream >> m_material.illum;
			m_material2.illum = m_material.illum;
		}
		else if( prefix == "Ka" )
		{
			lineStream >> m_material.Ka.x >> m_material.Ka.y >> m_material.Ka.z;
			m_material2.ka = XMVectorSet(m_material.Ka.x, m_material.Ka.y, m_material.Ka.z, 0.f);
		}
		else if( prefix == "Kd" )
		{
			lineStream >> m_material.Kd.x >> m_material.Kd.y >> m_material.Kd.z;
			m_material2.kd = XMVectorSet(m_material.Kd.x, m_material.Kd.y, m_material.Kd.z, 0.f);
		}
		else if( prefix == "Ks" )
		{
			lineStream >> m_material.Ks.x >> m_material.Ks.y >> m_material.Ks.z;
		}
		else if( prefix == "Ke" )
		{
			lineStream >> m_material.Ke.x >> m_material.Ke.y >> m_material.Ke.z;
		}
		else if( prefix == "map_Ka" )
		{
			lineStream >> m_material.map_Ka;
		}
		else if( prefix == "map_Kd" )
		{
			lineStream >> m_material.map_Kd;
		}
	}
	file.close();
}

vector<MeshTriangle>* Mesh::getTriangles()
{
	return &m_meshTriangles;
}

MeshTriangle* Mesh::getTriangles2()
{
	return &m_meshTriangles[0];
}

Material* Mesh::getMaterial()
{
	return &m_material;
}

int Mesh::getFaces()
{
	return m_nrOfFaces;
}

Material2 Mesh::getMaterial2()
{
	return m_material2;
}