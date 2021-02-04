#pragma once
#include "DXF.h"

/// <summary>
/// Class for storing the transform.
/// </summary>
class Transform
{
	XMFLOAT3 scale;
	XMFLOAT3 rotation;
	XMFLOAT3 position;

public:

	Transform()
		: scale(1.f, 1.f, 1.f)
		, rotation(0.f, 0.f, 0.f)
		, position(0.f, 0.f, 0.f)

	{
		
	}

	void setScale(const float scale) noexcept
	{
		this->scale.x = scale;
		this->scale.y = scale;
		this->scale.z = scale;
	}

	void setScale(const float sx, const float sy, const float sz) noexcept
	{
		this->scale.x = sx;
		this->scale.y = sy;
		this->scale.z = sz;
	}

	XMFLOAT3 getScale() const noexcept
	{
		return scale;
	}

	void setRotation(const float pitch, const float yaw, const float roll) noexcept
	{
		this->rotation.x = pitch;
		this->rotation.y = yaw;
		this->rotation.z = roll;
	}

	void setRotation(const XMFLOAT3 rotation) noexcept
	{
		this->rotation = rotation;
	}

	/// <summary>
	/// Return rotation in the form of:
	/// * x = pitch
	/// * y = yaw
	/// * z = roll
	/// </summary>
	/// <returns>Vector with Pitch, Yaw, Roll.</returns>
	XMFLOAT3 getRotation() const noexcept
	{
		return rotation;
	}

	void setPosition(const float px, const float py, const float pz) noexcept
	{
		this->position.x = px;
		this->position.y = py;
		this->position.z = pz;
	}

	void setPosition(const XMFLOAT3 position) noexcept
	{
		this->position = position;
	}

	XMFLOAT3 getPosition() const noexcept
	{
		return position;
	}

	/// <summary>
	/// Get a premultiplied Scale, Rotation, Position matrix (in that order)
	/// </summary>
	/// <returns>Multiplied Scale, Rotation, Position matrix.</returns>
	XMMATRIX getTransformationMatrix() const noexcept
	{
		auto scaleMatrix = XMMatrixScaling(getScale().x, getScale().y, getScale().z);
		auto rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.z, rotation.y, rotation.x);
		auto posMatrix = XMMatrixTranslation(getPosition().x, getPosition().y, getPosition().z);

		auto scaleXrotation = XMMatrixMultiply(scaleMatrix, rotationMatrix);
		auto srtMatrix = XMMatrixMultiply(scaleXrotation, posMatrix);

		return srtMatrix;
	}
};