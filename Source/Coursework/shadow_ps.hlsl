#define NO_OF_POINT_LIGHTS 2
#define NO_OF_SPOT_LIGHTS 2
#define NO_OF_SHADOWED_LIGHTS 2

Texture2D objectTexture : register(t0);
Texture2D depthMapTexture[NO_OF_SHADOWED_LIGHTS] : register(t1);

SamplerState diffuseSampler : register(s0);
SamplerState shadowSampler : register(s1);

cbuffer LightAndShadowBuffer : register(b0)
{
	float4 ambient; //only have 1 ambient
	float shadowMapBias;
	float3 padding2;
};

cbuffer DirectionalLightBuffer : register(b1)
{
	float4 diffuse[NO_OF_SHADOWED_LIGHTS];
	float4 direction[NO_OF_SHADOWED_LIGHTS];
};

cbuffer PointLightBufferType : register(b2)
{
	float4 diffusePoint[NO_OF_POINT_LIGHTS];
	float4 positionPoint[NO_OF_POINT_LIGHTS];
};

cbuffer SpotLightBufferType : register(b3)
{
	float4 diffuseSpot[NO_OF_SPOT_LIGHTS];
	float4 positionSpot[NO_OF_SPOT_LIGHTS];
	float4 directionSpot[NO_OF_SPOT_LIGHTS];
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 lightViewPos[NO_OF_SHADOWED_LIGHTS]: TEXCOORD1;
	float4 worldPosition : TEXCOORD10;
};

/// Calculate directional lighting based on the normal.
float4 calculateLightingDirectional(float3 lightDirection, float3 normal, float4 diffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	return colour;
}

// Calculate a point light, REMEMBER TO INVERT LIGHT DIRECTION!
float4 calculateLightingPoint(float3 normal, float4 lightPos, float4 diffuse, float3 worldPosition, float attenuationConstant, float attenuationLinear, float attenuationExp)
{
	float4 colour = float4(0.f, 0.f, 0.f, 1.f); // Set the colour to zero, as ambient will be added later.

	float3 pxToLightVec = worldPosition.xyz - lightPos; // Get vector between pos and light.
	float distance = length(pxToLightVec); // Get the distance between the two.
	pxToLightVec = normalize(pxToLightVec); // Now make it unit after getting length.

	float normalIntensity = saturate(dot(normal, -pxToLightVec)); //cCalculate the angle between the normal at the surface and the negative normal between the light and px (as to have a valid angle comparison - otherwise you would have two normals that face away from eachother).

	float4 lightColour = saturate(normalIntensity * diffuse); // Add the intensity to the diffuse value.

	lightColour /= attenuationConstant + (attenuationLinear * distance) + (attenuationExp * (distance * distance)); // Divide the output colour at the pixel by the attenuation factor.

	colour = saturate(lightColour + colour); // Finally add the calculated light intensity at the pixel to the final colour and clamp between 0 and 1.

	return colour;
}

// Calculate a spot light, REMEMBER TO INVERT LIGHT DIRECTION!
float4 calculateLightingSpot(float3 lightDirection, float3 lightPosition, float3 normal, float4 diffuse, float4 worldPosition, float attenuationConstant, float attenuationLinear, float attenuationExp)
{
	float4 colour = float4(0.f, 0.f, 0.f, 1.f); //Set colour to ambient colour

	float halfAngle = cos(0.785f / 2.f); //0.785 radians to cos which is 45 degrees. Cosined because then the dot product doesn't need to be acos'd back to a radian value and compared.

	float3 pxToLightVec = worldPosition.xyz - lightPosition; // Get vector between pos and light
	float distance = length(pxToLightVec); // Get the length between the two.
	pxToLightVec = normalize(pxToLightVec); // Now make it unit after getting length.

	//with the negative it beings light to pixel vector
	float normalIntensity = saturate(dot(normal, -pxToLightVec)); // Calculate the angle between the normal at the surface and the negative normal between the light and px (as to have a valid angle comparison - otherwise you would have two normals that face away from eachother (see Erin's notes)).

	float cosOfAngleBetweenPxAndLight = dot(lightDirection, -pxToLightVec /* Negative makes it from light to px. */); // Get angle between the two to see if the light is actually within the cone of light.

	if (cosOfAngleBetweenPxAndLight > halfAngle) // Check the angle between the light to px, and the cut off angle.
	{
		// The angle is within the point!
		float4 lightColour = saturate(normalIntensity * diffuse); // Add the instensity.

		lightColour /= attenuationConstant + (attenuationLinear * distance) + (attenuationExp * (distance * distance)); // Divide the intensity at the pixel by the attenuation factor.

		colour = saturate(lightColour + colour); // Finally add the calculated light intensity at the pixel to the final colour and clamp between 0 and 1.
	}

	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	float4 colour = ambient;
	float4 textureColour = objectTexture.Sample(diffuseSampler, input.tex);

	/*To make debugging easier!*/
	float4 shadowedColours[NO_OF_SHADOWED_LIGHTS] = { float4(0.f, 0.f, 0.f, 1.f), float4(0.f, 0.f, 0.f, 1.f) };
	bool isShadowed[NO_OF_SHADOWED_LIGHTS] = { true, true };
	
	/*For each of the shadowed lights see if the pixel is lit by that specific light.*/
	for (int i = 0; i < NO_OF_SHADOWED_LIGHTS; i++)
	{
		/* Calculate the projected texture coordinates. */
		float2 pTexCoord = input.lightViewPos[i].xy / input.lightViewPos[i].w;
		pTexCoord *= float2(0.5, -0.5);
		pTexCoord += float2(0.5f, 0.5f);

		shadowedColours[i] = float4(0.f, 0.f, 0.f, 1.f); //not lit by default

		/*If out of map range, dont light.*/
		if (pTexCoord.x < 0.f || pTexCoord.x > 1.f || pTexCoord.y < 0.f || pTexCoord.y > 1.f)
		{
		}
		else
		{
			/* Sample height map to get coordinates*/
			float depthValue = depthMapTexture[i].Sample(shadowSampler, pTexCoord).r;
			
			/*Get the depth value*/
			float lightDepthValue = input.lightViewPos[i].z / input.lightViewPos[i].w; // Make the value homogenous to match the scale on the depth map.
			/*Try to strike a balance between accuracy and ugliness.*/
			lightDepthValue -= shadowMapBias;

			/* Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel. */
			if (lightDepthValue < depthValue)
			{
				/*Its lit!*/
				shadowedColours[i] = calculateLightingDirectional(-direction[i].xyz, input.normal, diffuse[i]);
				isShadowed[i] = false;
			}
		}
	}

	/* Add the shadowed lights to the pixel. */
	int noOfShadowedLights = 0;
	float4 totalShadowedColour = float4(0.f, 0.f, 0.f, 1.f);
	for (int i = 0; i < NO_OF_SHADOWED_LIGHTS; i++)
	{
		if (isShadowed[i])
			noOfShadowedLights++;
				
		totalShadowedColour += shadowedColours[i];

	}
	if (noOfShadowedLights > 0) //decrease the strengh of the directional light if there is more than 1.
		totalShadowedColour /= noOfShadowedLights;
	
	colour += totalShadowedColour;
	
	/*Add point light colour.*/
	float4 pointColor =float4(0.f, 0.f, 0.f, 1.f);
	for (int i = 0; i < NO_OF_POINT_LIGHTS; i++)
	{
		pointColor += calculateLightingPoint(input.normal, positionPoint[i], diffusePoint[i], input.worldPosition.xyz, 1.f, 2.f, 1.f);
	}

	colour += pointColor;

	/*Add spot light colour.*/
	float4 spotColour = float4(0.f, 0.f, 0.f, 1.f);
	for (int i = 0; i < NO_OF_SPOT_LIGHTS; i++)
	{
		spotColour += calculateLightingSpot(-directionSpot[i].xyz, positionSpot[i].xyz, input.normal, diffuseSpot[i], input.worldPosition, 2.f, 4.f, 1.f);
	}
	colour += spotColour;

	/*Finally add texture to colour.*/
	return saturate(colour) * textureColour;
}

/*Note to self: one of the shadows being lighter than the other is correct! because the angle of the light! set both shadows directions to be 0.7x/z, -0.7y.

/*
	find which light doesn't light the pixel
	then add the light that do light the pixel
	if both they are black?


	ok

	1. set light of pixel to the directional.
	2. if its in shadow set it to red
	3. 
*/