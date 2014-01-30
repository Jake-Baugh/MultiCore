#include "RayIntersect.fx"

struct Light
{
	float4 pos;
	float4 ambient;
	float4 diffuse;
	float range;
	float3 pad;
};

float3 PointLightR(float4 pos, float4 norm, float4 color, int materialID, Light L, Material m)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);
	int		i;
	float	ni;
	float2	pad;
	float4	kd; 
	float4	ka; 
	float4	tf; 

	//The vector from surface to the light
	float3 lightVec = L.pos.xyz - pos.xyz;
	float lightintensity;
	float3 lightDir;
	float3 reflection;
	float4 specular;
	float3 ambient = L.ambient.xyz;
	float3 diffuse = L.diffuse.xyz;
	float shininess = 10;
	if(materialID != -1)
	{
		diffuse *= color.xyz;
		i = m.illum;
		ni = m.ni;
		pad = m.pad;
		kd = m.kd;
		ka = m.ka;
		tf = m.tf;
	}
	//the distance deom surface to light
	float d = length(lightVec);
	float fade;
	if(d > L.range)
		return float3(0.0f, 0.0f, 0.0f);
	fade = 1 - (d / L.range);
	//Normalize light vector
	lightVec /= d;

	//Add ambient light term
	litColor = ambient;

	lightintensity = saturate(dot(norm.xyz, lightVec));
	litColor += diffuse * lightintensity;
	lightDir = -lightVec;
	if(lightintensity > 0.0f)
	{
		float3 viewDir = normalize(pos.xyz - L.pos.xyz);
		float3 ref = reflect(-lightDir, normalize(norm.xyz));
		float specFac = pow(max(dot(ref, viewDir), 0.0f), shininess);
		litColor += float3(1.0f, 1.0f, 1.0f) * specFac;
	}
	litColor = litColor * color.xyz;

	return litColor*fade;
}

	int		illum;
	float	ni;
	float2	pad;
	float4	kd;
	float4	ka;
	float4	tf;