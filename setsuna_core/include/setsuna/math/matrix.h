#pragma once

#include <setsuna/math/vector.h>

/** @file
@brief Header for matrix classes
*/

namespace setsuna {

/**
@brief Storage type for @ref setsuna::matrix4
*/
using float4x4 = DirectX::XMFLOAT4X4;

/**
@brief Storage type for @ref setsuna::matrix3
*/
using float3x3 = DirectX::XMFLOAT3X3;

class matrix3;

/**
@brief 4x4 matrix
*/
class matrix4 {

public:
	/**
	@brief Default constructor, identity matrix
	*/
	inline matrix4() :
	    m_v(DirectX::XMMatrixIdentity()) {}

	/**
	@brief Construct by specifying four column vectors
	*/
	inline matrix4(const vector4& c0, const vector4& c1, const vector4& c2, const vector4& c3) :
	    m_v(c0, c1, c2, c3) {}

	/**
	@brief Construct from a @ref setsuna::matrix3
	*/
	inline explicit matrix4(const matrix3& v, const vector4& c3);

	/**
	@brief Construct from a @ref setsuna::float4x4
	*/
	inline matrix4(const float4x4& v) :
	    m_v(DirectX::XMLoadFloat4x4(&v)) {}

#pragma region conversion_with_xmv

	inline explicit matrix4(DirectX::XMMATRIX v) :
	    m_v(v) {}

	inline operator DirectX::XMMATRIX() const { return m_v; }

#pragma endregion

	/**
	@brief Get the first column
	*/
	inline vector4 get_c0() const { return vector4(m_v.r[0]); }

	/**
	@brief Get the second column
	*/
	inline vector4 get_c1() const { return vector4(m_v.r[1]); }

	/**
	@brief Get the third column
	*/
	inline vector4 get_c2() const { return vector4(m_v.r[2]); }

	/**
	@brief Get the fourth column
	*/
	inline vector4 get_c3() const { return vector4(m_v.r[3]); }

	/**
	@brief Set the first column
	*/
	inline void set_c0(const vector4& c) { m_v.r[0] = c; }

	/**
	@brief Set the second column
	*/
	inline void set_c1(const vector4& c) { m_v.r[1] = c; }

	/**
	@brief Set the third column
	*/
	inline void set_c2(const vector4& c) { m_v.r[2] = c; }

	/**
	@brief Set the fourth column
	*/
	inline void set_c3(const vector4& c) { m_v.r[3] = c; }

	/**
	@brief Get the transpose matrix
	*/
	inline matrix4 transpose() const { return matrix4(DirectX::XMMatrixTranspose(m_v)); }

	/**
	@brief Get the inverse matrix
	*/
	inline matrix4 inverse() const { return matrix4(DirectX::XMMatrixInverse(nullptr, m_v)); }

	/**
	@brief Get the determinant of the matrix
	*/
	inline float determinant() const { return DirectX::XMVectorGetX(DirectX::XMMatrixDeterminant(m_v)); }

	/**
	@brief Multiply by matrix
	*/
	inline matrix4 operator*(const matrix4& rhs) const {
		return matrix4(DirectX::XMMatrixMultiply(rhs, m_v));
	}

	/**
	@brief Multiply by matrix assignment
	*/
	inline matrix4 operator*=(const matrix4& rhs) {
		*this = *this * rhs;
		return *this;
	}

	/**
	@brief Multiply by vector
	*/
	inline vector4 operator*(const vector4& rhs) const {
		return vector4(DirectX::XMVector4Transform(rhs, m_v));
	}

	/**
	@brief Convert to a @ref setsuna::float4x4
	*/
	inline operator float4x4() const {
		float4x4 d;
		DirectX::XMStoreFloat4x4(&d, m_v);
		return d;
	}

private:
	DirectX::XMMATRIX m_v;
};

/**
@brief 3x3 matrix
*/
class matrix3 {

public:
	/**
	@brief Default constructor, identity matrix
	*/
	inline matrix3() :
	    m_v(DirectX::XMMatrixIdentity()) {}

	/**
	@brief Construct by specifying three column vectors
	*/
	inline matrix3(const vector3& c0, const vector3& c1, const vector3& c2) :
	    m_v(c0, c1, c2, DirectX::XMVectorSet(0, 0, 0, 1)) {}

	/**
	@brief Construct from a @ref setsuna::matrix4
	*/
	inline explicit matrix3(const matrix4& v) :
	    m_v(v) {
		m_v.r[0] = DirectX::XMVectorSetW(m_v.r[0], 0);
		m_v.r[1] = DirectX::XMVectorSetW(m_v.r[1], 0);
		m_v.r[2] = DirectX::XMVectorSetW(m_v.r[2], 0);
		m_v.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);
	}

	/**
	@brief Construct from a @ref setsuna::float3x3
	*/
	inline matrix3(const float3x3& v) :
	    m_v(DirectX::XMLoadFloat3x3(&v)) {}

#pragma region conversion_with_xmv

	inline explicit matrix3(DirectX::XMMATRIX v) :
	    m_v(v) {}

	inline operator DirectX::XMMATRIX() const { return m_v; }

#pragma endregion

	/**
	@brief Get the first column
	*/
	inline vector3 get_c0() const { return vector3(m_v.r[0]); }

	/**
	@brief Get the second column
	*/
	inline vector3 get_c1() const { return vector3(m_v.r[1]); }

	/**
	@brief Get the third column
	*/
	inline vector3 get_c2() const { return vector3(m_v.r[2]); }

	/**
	@brief Set the first column
	*/
	inline void set_c0(const vector3& c) { m_v.r[0] = c; }

	/**
	@brief Set the second column
	*/
	inline void set_c1(const vector3& c) { m_v.r[1] = c; }

	/**
	@brief Set the third column
	*/
	inline void set_c2(const vector3& c) { m_v.r[2] = c; }

	/**
	@brief Get the transpose matrix
	*/
	inline matrix3 transpose() const { return matrix3(DirectX::XMMatrixTranspose(m_v)); }

	/**
	@brief Get the inverse matrix
	*/
	inline matrix3 inverse() const { return matrix3(DirectX::XMMatrixInverse(nullptr, m_v)); }

	/**
	@brief Get the determinant of the matrix
	*/
	inline float determinant() const { return DirectX::XMVectorGetX(DirectX::XMMatrixDeterminant(m_v)); }

	/**
	@brief Multiply by matrix
	*/
	inline matrix3 operator*(const matrix3& rhs) const {
		return matrix3(DirectX::XMMatrixMultiply(rhs, m_v));
	}

	/**
	@brief Multiply by matrix assignment
	*/
	inline matrix3 operator*=(const matrix3& rhs) {
		*this = *this * rhs;
		return *this;
	}

	/**
	@brief Multiply by vector
	*/
	inline vector3 operator*(const vector3& rhs) const {
		return vector3(DirectX::XMVector3Transform(rhs, m_v));
	}

	/**
	@brief Convert to a @ref setsuna::float3x3
	*/
	inline operator float3x3() const {
		float3x3 d;
		DirectX::XMStoreFloat3x3(&d, m_v);
		return d;
	}

private:
	DirectX::XMMATRIX m_v;
};

inline matrix4::matrix4(const matrix3& v, const vector4& c3) :
    m_v(v) {
	m_v.r[0] = DirectX::XMVectorSetW(m_v.r[0], 0);
	m_v.r[1] = DirectX::XMVectorSetW(m_v.r[1], 0);
	m_v.r[2] = DirectX::XMVectorSetW(m_v.r[2], 0);
	m_v.r[3] = c3;
}

}  // namespace setsuna
