#pragma once

#include <setsuna/math/vector.h>
#include <setsuna/math/matrix.h>

/** @file
@brief Header for @ref setsuna::quaternion
*/

namespace setsuna {

/**
@brief Quaternion
*/
class quaternion {

public:
	/**
	@brief Default constructor, identity quaternion
	*/
	inline quaternion() :
	    m_v(DirectX::XMQuaternionIdentity()) {}

	/**
	@brief Construct from a rotation around @p axis by @p angle radians

	@attention @p axis should be normalized.
	*/
	inline explicit quaternion(const vector3& axis, float angle) :
	    m_v(DirectX::XMQuaternionRotationNormal(axis, angle)) {}

	/**
	@brief Construct from pitch, yaw and roll angles (in radians)
	*/
	inline explicit quaternion(float pitch, float yaw, float roll) :
	    m_v(DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll)) {}

	/**
	@brief Construct from a rotation matrix
	*/
	inline explicit quaternion(const matrix3& m) :
	    m_v(DirectX::XMQuaternionRotationMatrix(m)) {}

	/**
	@brief Construct from a @ref setsuna::float4
	*/
	inline quaternion(const float4& v) :
	    m_v(DirectX::XMLoadFloat4(&v)) {}

#pragma region conversion_with_xmv

	inline explicit quaternion(DirectX::XMVECTOR v) :
	    m_v(v) {}

	inline operator DirectX::XMVECTOR() const { return m_v; }

#pragma endregion

	/**
	@brief Negate
	*/
	inline quaternion operator-() const { return quaternion(DirectX::XMVectorNegate(m_v)); }

	/**
	@brief Conjugate
	*/
	inline quaternion conjugate() const { return quaternion(DirectX::XMQuaternionConjugate(m_v)); }

	/**
	@brief Multiply
	*/
	inline quaternion operator*(const quaternion& rhs) const {
		return quaternion(DirectX::XMQuaternionMultiply(rhs, m_v));
	}

	/**
	@brief Multiply assignment
	*/
	inline quaternion operator*=(const quaternion& rhs) {
		*this = *this * rhs;
		return *this;
	}

	/**
	@brief Multiply by vector
	*/
	inline vector3 operator*(const vector3& rhs) const {
		return vector3(DirectX::XMVector3Rotate(rhs, m_v));
	}

	/**
	@brief Convert to a @ref setsuna::float4
	*/
	inline operator float4() const {
		float4 d;
		DirectX::XMStoreFloat4(&d, m_v);
		return d;
	}

private:
	DirectX::XMVECTOR m_v;
};

}  // namespace setsuna
