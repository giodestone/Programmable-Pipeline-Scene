#pragma once
#include "DXF.h"

/// <summary>
/// This file contains the buffer types and setting for how many lights there are. Update them if adding more lights!
/// </summary>

constexpr size_t NO_OF_POINT_LIGHTS = 2;
constexpr size_t NO_OF_SPOT_LIGHTS = 2;
constexpr size_t NO_OF_SHADOWED_LIGHTS = 2;

struct LightMatrixBufferType
{
	XMMATRIX lightView[NO_OF_SHADOWED_LIGHTS];
	XMMATRIX lightProjection[NO_OF_SHADOWED_LIGHTS];
};

struct LightAndShadowBufferType
{
	XMFLOAT4 ambient;
	float shadowMapBias;
	XMFLOAT3 padding;
};

struct DirectionalLightBufferType
{
	XMFLOAT4 diffuse[NO_OF_SHADOWED_LIGHTS];
	XMFLOAT4 direction[NO_OF_SHADOWED_LIGHTS];
};

struct PointLightBufferType
{
	XMFLOAT4 diffuse[NO_OF_POINT_LIGHTS];
	XMFLOAT4 position[NO_OF_POINT_LIGHTS];
};

struct SpotLightBufferType
{
	XMFLOAT4 diffuse[NO_OF_SPOT_LIGHTS];
	XMFLOAT4 position[NO_OF_SPOT_LIGHTS];
	XMFLOAT4 direction[NO_OF_SPOT_LIGHTS];
};