//--------------------------------------------------------------------------------------
// Intersection.fx
// Direct3D 11 Shader Model 5.0
//--------------------------------------------------------------------------------------
#ifndef INTERSECTIONSTAGE
#define INTERSECTIONSTAGE
#include "Structs.fx"
#include "RayIntersect.fx"

RWStructuredBuffer<Ray> Rays : register(u0);
RWStructuredBuffer<HitData> Output : register(u1);
Texture2D MeshTexture : register(t0);
StructuredBuffer<MeshTriangle> MeshTriangles : register(t1);

cbuffer ConstBuffer : register(b0)
{
	Sphere sphere;
	Triangle triangles[NROFTRIANGLES];
	int nrOfFaces;
	float3 pad;
};

cbuffer FirstPass : register (b1)
{
	bool firstpass;
};



[numthreads(32, 32, 1)]
void main( uint3 threadID : SV_DispatchThreadID )
{
	int index = threadID.x + (threadID.y * WIDTH);
	Ray r = Rays[index];

	HitData hd;
	hd.pos = float4(0.f, 0.f, 0.f, 0.f);
	hd.normal = float4(0.f, 0.f, 0.f, 0.f);
	hd.distance = -1.0;
	hd.color = float4(0.f, 1.f, 0.f, 0.f);
	hd.ID = -1;
	hd.materialID = -1;
	hd.pad = 0;

	if(firstpass)
		hd.ID = -1;
	else
		hd.ID = Output[index].ID;

	// ## SPHERE ## //
	if(hd.ID != sphere.ID)
	{
		float hit = RaySphereIntersect(r, sphere, hd.distance);
		if(hit > -1.0f)
		{
			hd.pos = r.origin + r.direction * hit;
			hd.normal = normalize(hd.pos - sphere.center);
			hd.color = sphere.color;
			hd.distance = hit;
			hd.ID = sphere.ID;
			hd.materialID = -1;
		}
	}

	// ## CUBE ## // 
	for(int i = 0; i < NROFTRIANGLES; i++)
	{
			if(hd.ID != triangles[i].ID)
			{
				float hit = RayTriangleIntersect(r, triangles[i], hd.distance);
				if(hit > -1.0f)
				{
					hd.pos = r.origin + r.direction * hit;
					hd.normal = triangles[i].normal;
					hd.color = triangles[i].color;					
					hd.ID = triangles[i].ID;
					hd.distance = hit;
					hd.materialID = -1;
				}
			}
	}

	// ## MESH ## //
	for(i = 0; i < nrOfFaces; i++)
	{
		if(hd.ID != MeshTriangles[i].ID)
		{
			float3 hit = RayTriangleIntersects(r, MeshTriangles[i], hd.distance);
			if(hit.x > -1.0f)
			{
				hd.pos = r.origin + r.direction * hit.x;
				hd.normal = MeshTriangles[i].normal;
				hd.color = MeshTexture[hit.yz*512.f];
				hd.ID = MeshTriangles[i].ID;
				hd.distance = hit.x;
				hd.materialID = 1;
			}
		}
	}

	Output[index] = hd;

	if(hd.ID != -1)
	{
		float4 bounceDir = reflect(r.direction, hd.normal);
		//bounceDir = normalize(bounceDir); //Slow operation might not be needed.
		r.origin = hd.pos;
		r.direction = bounceDir;
		Rays[index] = r;
	}
}

#endif