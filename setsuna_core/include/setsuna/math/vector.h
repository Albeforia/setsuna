#pragma once

#include <DirectXMath.h>

/** @file
@brief Header for vector classes
*/

namespace setsuna {

/**
@brief Storage type for @ref setsuna::vector4
*/
using float4 = DirectX::XMFLOAT4;

/**
@brief Storage type for @ref setsuna::vector3
*/
using float3 = DirectX::XMFLOAT3;

/**
@brief Storage type for @ref setsuna::vector2
*/
using float2 = DirectX::XMFLOAT2;

class vector3;
class vector2;

/**
@brief 4-components vector
*/
class vector4 {

public:
	/**
	@brief Default constructor, all components are zero
	*/
	inline vector4() :
	    m_v(DirectX::XMVectorZero()) {}

	/**
	@brief Construct by specifying a value for each component
	*/
	inline vector4(float x, float y, float z, float w = 1) :
	    m_v(DirectX::XMVectorSet(x, y, z, w)) {}

	/**
	@brief Construct by specifying a value for all components
	*/
	inline explicit vector4(float x) :
	    m_v(DirectX::XMVectorSet(x, x, x, x)) {}

	/**
	@brief Construct from a @ref setsuna::vector3
	*/
	inline explicit vector4(const vector3& v, float w);

	/**
	@brief Construct from a @ref setsuna::vector2
	*/
	inline explicit vector4(const vector2& v, float z, float w);

	/**
	@brief Construct from a @ref setsuna::float4
	*/
	inline vector4(const float4& v) :
	    m_v(DirectX::XMLoadFloat4(&v)) {}

#pragma region conversion_with_xmv

	inline explicit vector4(DirectX::XMVECTOR v) :
	    m_v(v) {}

	inline operator DirectX::XMVECTOR() const { return m_v; }

#pragma endregion

	/**
	@brief Get the X component
	*/
	inline float get_x() const { return DirectX::XMVectorGetX(m_v); }

	/**
	@brief Get the Y component
	*/
	inline float get_y() const { return DirectX::XMVectorGetY(m_v); }

	/**
	@brief Get the Z component
	*/
	inline float get_z() const { return DirectX::XMVectorGetZ(m_v); }

	/**
	@brief Get the W component
	*/
	inline float get_w() const { return DirectX::XMVectorGetW(m_v); }

	/**
	@brief Set the X component
	*/
	inline void set_x(float x) { m_v = DirectX::XMVectorSetX(m_v, x); }

	/**
	@brief Set the Y component
	*/
	inline void set_y(float y) { m_v = DirectX::XMVectorSetY(m_v, y); }

	/**
	@brief Set the Z component
	*/
	inline void set_z(float z) { m_v = DirectX::XMVectorSetZ(m_v, z); }

	/**
	@brief Set the W component
	*/
	inline void set_w(float w) { m_v = DirectX::XMVectorSetW(m_v, w); }

	/**
	@brief Negate
	*/
	inline vector4 operator-() const { return vector4(DirectX::XMVectorNegate(m_v)); }

	/**
	@brief Add
	*/
	inline vector4 operator+(const vector4& rhs) const {
		return vector4(DirectX::XMVectorAdd(m_v, rhs));
	}

	/**
	@brief Subtract
	*/
	inline vector4 operator-(const vector4& rhs) const {
		return vector4(DirectX::XMVectorSubtract(m_v, rhs));
	}

	/**
	@brief Multiply
	*/
	inline vector4 operator*(const vector4& rhs) const {
		return vector4(DirectX::XMVectorMultiply(m_v, rhs));
	}

	/**
	@brief Divide
	*/
	inline vector4 operator/(const vector4& rhs) const {
		// faster but lose precision
		//auto r = DirectX::XMVectorReciprocalEst(rhs);
		//return DirectX::XMVectorMultiply(m_v, r);

		return vector4(DirectX::XMVectorDivide(m_v, rhs));
	}

	/**
	@brief Add assignment
	*/
	inline vector4 operator+=(const vector4& rhs) {
		*this = *this + rhs;
		return *this;
	}

	/**
	@brief Subtract assignment
	*/
	inline vector4 operator-=(const vector4& rhs) {
		*this = *this - rhs;
		return *this;
	}

	/**
	@brief Multiply assignment
	*/
	inline vector4 operator*=(const vector4& rhs) {
		*this = *this * rhs;
		return *this;
	}

	/**
	@brief Divide assignment
	*/
	inline vector4 operator/=(const vector4& rhs) {
		*this = *this / rhs;
		return *this;
	}

	/**
	@brief Multiply by scalar
	*/
	inline vector4 operator*(float rhs) const {
		return vector4(DirectX::XMVectorScale(m_v, rhs));
	}

	/**
	@brief Divide by scalar
	*/
	inline vector4 operator/(float rhs) const { return *this / vector4(rhs); }

	/**
	@brief Multiply by scalar assignment
	*/
	inline vector4 operator*=(float rhs) {
		*this = *this * rhs;
		return *this;
	}

	/**
	@brief Divide by scalar assignment
	*/
	inline vector4 operator/=(float rhs) {
		*this = *this / rhs;
		return *this;
	}

	inline friend vector4 operator*(float lhs, const vector4& rhs) { return rhs * lhs; }
	inline friend vector4 operator/(float lhs, const vector4& rhs) { return vector4(lhs) / rhs; }

	/**
	@brief Convert to a @ref setsuna::float4
	*/
	inline operator float4() const {
		float4 d;
		DirectX::XMStoreFloat4(&d, m_v);
		return d;
	}

	/**
	@brief Get the length of the vector
	*/
	inline float length() const { return DirectX::XMVectorGetX(DirectX::XMVector4Length(m_v)); }

	/**
	@brief Get the normalized vector
	*/
	inline vector4 normalize() const { return vector4(DirectX::XMVector4Normalize(m_v)); }

private:
	DirectX::XMVECTOR m_v;
};

/**
@brief 3-components vector
*/
class vector3 {

public:
	/**
	@brief Default constructor, all components are zero
	*/
	inline vector3() :
	    m_v(DirectX::XMVectorZero()) {}

	/**
	@brief Construct by specifying a value for each component
	*/
	inline vector3(float x, float y, float z) :
	    m_v(DirectX::XMVectorSet(x, y, z, 0)) {}

	/**
	@brief Construct by specifying a value for all components
	*/
	inline explicit vector3(float x) :
	    m_v(DirectX::XMVectorSet(x, x, x, 0)) {}

	/**
	@brief Construct from a @ref setsuna::vector4
	*/
	inline explicit vector3(const vector4& v) :
	    m_v(DirectX::XMVectorSelect(DirectX::XMVectorZero(), v, DirectX::g_XMSelect1110)) {}

	//inline vector3& operator=(const vector4& v) {
	//	m_v = DirectX::XMVectorSelect(DirectX::XMVectorSplatOne(), v, DirectX::g_XMSelect1110);
	//}

	/**
	@brief Construct from a @ref setsuna::vector2
	*/
	inline explicit vector3(const vector2& v, float z);

	/**
	@brief Construct from a @ref setsuna::float3
	*/
	inline vector3(const float3& v) :
	    m_v(DirectX::XMLoadFloat3(&v)) {}

#pragma region conversion_with_xmv

	inline explicit vector3(DirectX::XMVECTOR v) :
	    m_v(v) {}

	inline operator DirectX::XMVECTOR() const { return m_v; }

#pragma endregion

	/**
	@brief Get the X component
	*/
	inline float get_x() const { return DirectX::XMVectorGetX(m_v); }

	/**
	@brief Get the Y component
	*/
	inline float get_y() const { return DirectX::XMVectorGetY(m_v); }

	/**
	@brief Get the Z component
	*/
	inline float get_z() const { return DirectX::XMVectorGetZ(m_v); }

	/**
	@brief Set the X component
	*/
	inline void set_x(float x) { m_v = DirectX::XMVectorSetX(m_v, x); }

	/**
	@brief Set the Y component
	*/
	inline void set_y(float y) { m_v = DirectX::XMVectorSetY(m_v, y); }

	/**
	@brief Set the Z component
	*/
	inline void set_z(float z) { m_v = DirectX::XMVectorSetZ(m_v, z); }

	/**
	@brief Negate
	*/
	inline vector3 operator-() const { return vector3(DirectX::XMVectorNegate(m_v)); }

	/**
	@brief Add
	*/
	inline vector3 operator+(const vector3& rhs) const {
		return vector3(DirectX::XMVectorAdd(m_v, rhs));
	}

	/**
	@brief Subtract
	*/
	inline vector3 operator-(const vector3& rhs) const {
		return vector3(DirectX::XMVectorSubtract(m_v, rhs));
	}

	/**
	@brief Multiply
	*/
	inline vector3 operator*(const vector3& rhs) const {
		return vector3(DirectX::XMVectorMultiply(m_v, rhs));
	}

	/**
	@brief Divide
	*/
	inline vector3 operator/(const vector3& rhs) const {
		// faster but lose precision
		//auto r = DirectX::XMVectorReciprocalEst(rhs);
		//return DirectX::XMVectorMultiply(m_v, r);

		return vector3(DirectX::XMVectorDivide(m_v, rhs));
	}

	/**
	@brief Add assignment
	*/
	inline vector3 operator+=(const vector3& rhs) {
		*this = *this + rhs;
		return *this;
	}

	/**
	@brief Subtract assignment
	*/
	inline vector3 operator-=(const vector3& rhs) {
		*this = *this - rhs;
		return *this;
	}

	/**
	@brief Multiply assignment
	*/
	inline vector3 operator*=(const vector3& rhs) {
		*this = *this * rhs;
		return *this;
	}

	/**
	@brief Divide assignment
	*/
	inline vector3 operator/=(const vector3& rhs) {
		*this = *this / rhs;
		return *this;
	}

	/**
	@brief Multiply by scalar
	*/
	inline vector3 operator*(float rhs) const {
		return vector3(DirectX::XMVectorScale(m_v, rhs));
	}

	/**
	@brief Divide by scalar
	*/
	inline vector3 operator/(float rhs) const { return *this / vector3(rhs); }

	/**
	@brief Multiply by scalar assignment
	*/
	inline vector3 operator*=(float rhs) {
		*this = *this * rhs;
		return *this;
	}

	/**
	@brief Divide by scalar assignment
	*/
	inline vector3 operator/=(float rhs) {
		*this = *this / rhs;
		return *this;
	}

	inline friend vector3 operator*(float lhs, const vector3& rhs) { return rhs * lhs; }
	inline friend vector3 operator/(float lhs, const vector3& rhs) { return vector3(lhs) / rhs; }

	/**
	@brief Convert to a @ref setsuna::float3
	*/
	inline operator float3() const {
		float3 d;
		DirectX::XMStoreFloat3(&d, m_v);
		return d;
	}

	/**
	@brief Get the length of the vector
	*/
	inline float length() const { return DirectX::XMVectorGetX(DirectX::XMVector3Length(m_v)); }

	/**
	@brief Get the normalized vector
	*/
	inline vector3 normalize() const { return vector3(DirectX::XMVector3Normalize(m_v)); }

private:
	DirectX::XMVECTOR m_v;
};

/**
@brief 2-components vector
*/
class vector2 {

public:
	/**
	@brief Default constructor, all components are zero
	*/
	inline vector2() :
	    m_v(DirectX::XMVectorZero()) {}

	/**
	@brief Construct by specifying a value for each component
	*/
	inline vector2(float x, float y) :
	    m_v(DirectX::XMVectorSet(x, y, 0, 0)) {}

	/**
	@brief Construct by specifying a value for all components
	*/
	inline explicit vector2(float x) :
	    m_v(DirectX::XMVectorSet(x, x, 0, 0)) {}

	/**
	@brief Construct from a @ref setsuna::vector4
	*/
	inline explicit vector2(const vector4& v) :
	    m_v(DirectX::XMVectorSelect(DirectX::XMVectorZero(), v, DirectX::g_XMSelect1100)) {}

	/**
	@brief Construct from a @ref setsuna::vector3
	*/
	inline explicit vector2(const vector3& v) :
	    m_v(DirectX::XMVectorSelect(DirectX::XMVectorZero(), v, DirectX::g_XMSelect1100)) {}

	//inline vector2& operator=(const vector4& v) {
	//	m_v = DirectX::XMVectorSelect(DirectX::XMVectorSplatOne(), v, DirectX::g_XMSelect1100);
	//}

	//inline vector2& operator=(const vector3& v) {
	//	m_v = DirectX::XMVectorSelect(DirectX::XMVectorSplatOne(), v, DirectX::g_XMSelect1100);
	//}

	/**
	@brief Construct from a @ref setsuna::float2
	*/
	inline vector2(const float2& v) :
	    m_v(DirectX::XMLoadFloat2(&v)) {}

#pragma region conversion_with_xmv

	inline explicit vector2(DirectX::XMVECTOR v) :
	    m_v(v) {}

	inline operator DirectX::XMVECTOR() const { return m_v; }

#pragma endregion

	/**
	@brief Get the X component
	*/
	inline float get_x() const { return DirectX::XMVectorGetX(m_v); }

	/**
	@brief Get the Y component
	*/
	inline float get_y() const { return DirectX::XMVectorGetY(m_v); }

	/**
	@brief Set the X component
	*/
	inline void set_x(float x) { m_v = DirectX::XMVectorSetX(m_v, x); }

	/**
	@brief Set the Y component
	*/
	inline void set_y(float y) { m_v = DirectX::XMVectorSetY(m_v, y); }

	/**
	@brief Negate
	*/
	inline vector2 operator-() const { return vector2(DirectX::XMVectorNegate(m_v)); }

	/**
	@brief Add
	*/
	inline vector2 operator+(const vector2& rhs) const {
		return vector2(DirectX::XMVectorAdd(m_v, rhs));
	}

	/**
	@brief Subtract
	*/
	inline vector2 operator-(const vector2& rhs) const {
		return vector2(DirectX::XMVectorSubtract(m_v, rhs));
	}

	/**
	@brief Multiply
	*/
	inline vector2 operator*(const vector2& rhs) const {
		return vector2(DirectX::XMVectorMultiply(m_v, rhs));
	}

	/**
	@brief Divide
	*/
	inline vector2 operator/(const vector2& rhs) const {
		// faster but lose precision
		//auto r = DirectX::XMVectorReciprocalEst(rhs);
		//return DirectX::XMVectorMultiply(m_v, r);

		return vector2(DirectX::XMVectorDivide(m_v, rhs));
	}

	/**
	@brief Add assignment
	*/
	inline vector2 operator+=(const vector2& rhs) {
		*this = *this + rhs;
		return *this;
	}

	/**
	@brief Subtract assignment
	*/
	inline vector2 operator-=(const vector2& rhs) {
		*this = *this - rhs;
		return *this;
	}

	/**
	@brief Multiply assignment
	*/
	inline vector2 operator*=(const vector2& rhs) {
		*this = *this * rhs;
		return *this;
	}

	/**
	@brief Divide assignment
	*/
	inline vector2 operator/=(const vector2& rhs) {
		*this = *this / rhs;
		return *this;
	}

	/**
	@brief Multiply by scalar
	*/
	inline vector2 operator*(float rhs) const {
		return vector2(DirectX::XMVectorScale(m_v, rhs));
	}

	/**
	@brief Divide by scalar
	*/
	inline vector2 operator/(float rhs) const { return *this / vector2(rhs); }

	/**
	@brief Multiply by scalar assignment
	*/
	inline vector2 operator*=(float rhs) {
		*this = *this * rhs;
		return *this;
	}

	/**
	@brief Divide by scalar assignment
	*/
	inline vector2 operator/=(float rhs) {
		*this = *this / rhs;
		return *this;
	}

	inline friend vector2 operator*(float lhs, const vector2& rhs) { return rhs * lhs; }
	inline friend vector2 operator/(float lhs, const vector2& rhs) { return vector2(lhs) / rhs; }

	/**
	@brief Convert to a @ref setsuna::float2
	*/
	inline operator float2() const {
		float2 d;
		DirectX::XMStoreFloat2(&d, m_v);
		return d;
	}

	/**
	@brief Get the length of the vector
	*/
	inline float length() const { return DirectX::XMVectorGetX(DirectX::XMVector2Length(m_v)); }

	/**
	@brief Get the normalized vector
	*/
	inline vector2 normalize() const { return vector2(DirectX::XMVector2Normalize(m_v)); }

private:
	DirectX::XMVECTOR m_v;
};

inline vector4::vector4(const vector3& v, float w) :
    m_v(DirectX::XMVectorSetW(v, w)) {}

inline vector4::vector4(const vector2& v, float z, float w) :
    m_v(DirectX::XMVectorSelect(DirectX::XMVectorSet(0, 0, z, w), v, DirectX::g_XMSelect1100)) {}

inline vector3::vector3(const vector2& v, float z) :
    m_v(DirectX::XMVectorSelect(DirectX::XMVectorSet(0, 0, z, 0), v, DirectX::g_XMSelect1100)) {}

}  // namespace setsuna
