#pragma once
#include "DXF.h"

/// <summary>
/// Extra params for the SimpleShader.
/// </summary>
struct SimpleShaderParams
{
	ID3D11ShaderResourceView* texture;
};


/// <summary>
/// A simple shader which just renders the object with directional lighting
/// </summary>
class SimpleShader :
	public BaseShader
{
public:
	SimpleShader(ID3D11Device* device, HWND hwnd);
	~SimpleShader();

protected:
	/// <summary>
	/// Load the vertex and pixel shader.
	/// </summary>
	/// <param name="vsFileName">Vertex shader path. Remember .cso</param>
	/// <param name="psFileName">Pixel shader path. Remember .cso</param>
	void initShader(const wchar_t* vsFileName, const wchar_t* psFileName) override;

public:
	/// <summary>
	/// Set the shader paramaters.
	/// </summary>
	/// <param name="simpleShaderParams"></param>
	void setShaderParams(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, const SimpleShaderParams& simpleShaderParams);
private:
	ID3D11Buffer* VSMatrixBuffer;
	ID3D11SamplerState* PSSamplerState;
};

