#ifndef _VECTOR3D_HPP
#define _VECTOR3D_HPP

#define CHECK_VALID( _v ) 0
#define Assert( _exp ) ((void)0)

#include "main.hpp"

// from the leaked sdk

#define  FORCEINLINE			__forceinline

inline float BitsToFloat( unsigned long i ) {
	return *reinterpret_cast<float*>( &i );
}

#define FLOAT32_NAN_BITS     (unsigned long)0x7FC00000
#define FLOAT32_NAN          BitsToFloat( FLOAT32_NAN_BITS )

#define float_NAN FLOAT32_NAN

class Vector {
public:
	float x, y, z;

	Vector( void );
	Vector( float X, float Y, float Z );
	explicit Vector( float XYZ );

	void Init( float ix = 0.0f, float iy = 0.0f, float iz = 0.0f );

	bool IsValid() const;
	void Invalidate();

	float   operator[]( int i ) const;
	float&  operator[]( int i );

	float* Base();
	float const* Base() const;

	inline void Zero();

	bool operator==( const Vector& v ) const;
	bool operator!=( const Vector& v ) const;

	FORCEINLINE Vector& operator+=( const Vector& v );
	FORCEINLINE Vector& operator-=( const Vector& v );
	FORCEINLINE Vector& operator*=( const Vector& v );
	FORCEINLINE Vector& operator*=( float s );
	FORCEINLINE Vector& operator/=( const Vector& v );
	FORCEINLINE Vector& operator/=( float s );
	FORCEINLINE Vector& operator+=( float fl );
	FORCEINLINE Vector& operator-=( float fl );

	void Negate();

	inline float Length() const;

	FORCEINLINE float LengthSqr( void ) const {
		return ( x * x + y * y + z * z );
	}

	bool IsZero( float tolerance = 0.01f ) const {
		return ( x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance &&
			z > -tolerance && z < tolerance );
	}

	float NormalizeInPlace();

	FORCEINLINE bool WithinAABox( Vector const &boxmin, Vector const &boxmax );

	float DistTo( const Vector& vOther ) const;

	FORCEINLINE float DistToSqr( const Vector& vOther ) const {
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.LengthSqr();
	}

	void CopyToArray( float* rgfl ) const;
	void MulAdd( const Vector& a, const Vector& b, float scalar );

	float Dot( const Vector& vOther ) const;

	Vector& operator=( const Vector& vOther );

	float Length2D( void ) const;
	float Length2DSqr( void ) const;

	Vector operator-( void ) const;
	Vector operator+( const Vector& v ) const;
	Vector operator-( const Vector& v ) const;
	Vector operator*( const Vector& v ) const;
	Vector operator/( const Vector& v ) const;
	Vector operator*( float fl ) const;
	Vector operator/( float fl ) const;

	Vector Cross( const Vector& vOther ) const;

	Vector Min( const Vector& vOther ) const;
	Vector Max( const Vector& vOther ) const;
};

FORCEINLINE Vector ReplicateToVector( float x ) {
	return Vector( x, x, x );
}

inline Vector::Vector( void ) {

}

inline Vector::Vector( float X, float Y, float Z ) {
	x = X; y = Y; z = Z;
}

inline Vector::Vector( float XYZ ) {
	x = y = z = XYZ;
}

inline void Vector::Init( float ix, float iy, float iz ) {
	x = ix; y = iy; z = iz;
}

inline void Vector::Zero() {
	x = y = z = 0.0f;
}

inline void VectorClear( Vector& a ) {
	a.x = a.y = a.z = 0.0f;
}

inline Vector& Vector::operator=( const Vector& vOther ) {
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}

inline float& Vector::operator[]( int i ) {
	return ( ( float* )this )[ i ];
}

inline float Vector::operator[]( int i ) const {
	return ( ( float* )this )[ i ];
}



inline float* Vector::Base() {
	return ( float* )this;
}

inline float const* Vector::Base() const {
	return ( float const* )this;
}

inline bool Vector::IsValid() const {
	return ( x == x && y == y && z == z );
}

inline void Vector::Invalidate() {
	x = y = z = float_NAN;
}

inline bool Vector::operator==( const Vector& src ) const {
	return ( src.x == x ) && ( src.y == y ) && ( src.z == z );
}

inline bool Vector::operator!=( const Vector& src ) const {
	return ( src.x != x ) || ( src.y != y ) || ( src.z != z );
}

FORCEINLINE void VectorCopy( const Vector& src, Vector& dst ) {
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}

inline void Vector::CopyToArray( float* rgfl ) const {
	rgfl[ 0 ] = x; rgfl[ 1 ] = y; rgfl[ 2 ] = z;
}

inline void Vector::Negate() {
	x = -x; y = -y; z = -z;
}

FORCEINLINE Vector& Vector::operator+=( const Vector& v ) {
	x += v.x; y += v.y; z += v.z;
	return *this;
}

FORCEINLINE Vector& Vector::operator-=( const Vector& v ) {
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

FORCEINLINE Vector& Vector::operator*=( float fl ) {
	x *= fl;
	y *= fl;
	z *= fl;
	return *this;
}

FORCEINLINE Vector& Vector::operator*=( const Vector& v ) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

FORCEINLINE Vector& Vector::operator+=( float fl ) {
	x += fl;
	y += fl;
	z += fl;
	return *this;
}

FORCEINLINE Vector& Vector::operator-=( float fl ) {
	x -= fl;
	y -= fl;
	z -= fl;
	return *this;
}

FORCEINLINE Vector& Vector::operator/=( float fl ) {
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	return *this;
}

FORCEINLINE Vector& Vector::operator/=( const Vector& v ) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

FORCEINLINE void VectorAdd( const Vector& a, const Vector& b, Vector& c ) {
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

FORCEINLINE void VectorSubtract( const Vector& a, const Vector& b, Vector& c ) {
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

FORCEINLINE void VectorMultiply( const Vector& a, float b, Vector& c ) {
	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
}

FORCEINLINE void VectorMultiply( const Vector& a, const Vector& b, Vector& c ) {
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
}

inline void VectorScale( const Vector& in, float scale, Vector& result ) {
	VectorMultiply( in, scale, result );
}

FORCEINLINE void VectorDivide( const Vector& a, float b, Vector& c ) {
	float oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
	c.z = a.z * oob;
}

FORCEINLINE void VectorDivide( const Vector& a, const Vector& b, Vector& c ) {
	c.x = a.x / b.x;
	c.y = a.y / b.y;
	c.z = a.z / b.z;
}

inline void Vector::MulAdd( const Vector& a, const Vector& b, float scalar ) {
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
	z = a.z + b.z * scalar;
}

inline void VectorLerp( const Vector& src1, const Vector& src2, float t, Vector& dest ) {
	dest.x = src1.x + ( src2.x - src1.x ) * t;
	dest.y = src1.y + ( src2.y - src1.y ) * t;
	dest.z = src1.z + ( src2.z - src1.z ) * t;
}

FORCEINLINE float DotProduct( const Vector& a, const Vector& b ) {
	return ( a.x * b.x + a.y * b.y + a.z * b.z );
}

inline float Vector::Dot( const Vector& vOther ) const {
	return DotProduct( *this, vOther );
}

inline void CrossProduct( const Vector& a, const Vector& b, Vector& result ) {
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
}

inline float VectorLength( const Vector& v ) {
	return ( float ) sqrt( v.x * v.x + v.y * v.y + v.z * v.z );
}

inline float Vector::Length( void ) const {
	return VectorLength( *this );
}

inline float VectorNormalize( Vector& v ) {
	float l = v.Length();

	if( l != 0.0f ) {
		v /= l;
	} else {
		v.x = v.y = 0.0f; v.z = 1.0f;
	}

	return l;
}

FORCEINLINE float VectorNormalizer( float * v ) {
	return VectorNormalize( *( reinterpret_cast<Vector *>( v ) ) );
}
inline float Vector::NormalizeInPlace() {
	return VectorNormalize( *this );
}

bool Vector::WithinAABox( Vector const &boxmin, Vector const &boxmax ) {
	return (
		( x >= boxmin.x ) && ( x <= boxmax.x ) &&
		( y >= boxmin.y ) && ( y <= boxmax.y ) &&
		( z >= boxmin.z ) && ( z <= boxmax.z )
		);
}

inline float Vector::DistTo( const Vector& vOther ) const {
	Vector delta;
	VectorSubtract( *this, vOther, delta );
	return delta.Length();
}

inline Vector Vector::Min( const Vector& vOther ) const {
	return Vector( x < vOther.x ? x : vOther.x,
		y < vOther.y ? y : vOther.y,
		z < vOther.z ? z : vOther.z );
}

inline Vector Vector::Max( const Vector& vOther ) const {
	return Vector( x > vOther.x ? x : vOther.x,
		y > vOther.y ? y : vOther.y,
		z > vOther.z ? z : vOther.z );
}

inline Vector Vector::operator-( void ) const {
	return Vector( -x, -y, -z );
}

inline Vector Vector::operator+( const Vector& v ) const {
	Vector res;
	VectorAdd( *this, v, res );
	return res;
}

inline Vector Vector::operator-( const Vector& v ) const {
	Vector res;
	VectorSubtract( *this, v, res );
	return res;
}

inline Vector Vector::operator*( float fl ) const {
	Vector res;
	VectorMultiply( *this, fl, res );
	return res;
}

inline Vector Vector::operator*( const Vector& v ) const {
	Vector res;
	VectorMultiply( *this, v, res );
	return res;
}

inline Vector Vector::operator/( float fl ) const {
	Vector res;
	VectorDivide( *this, fl, res );
	return res;
}

inline Vector Vector::operator/( const Vector& v ) const {
	Vector res;
	VectorDivide( *this, v, res );
	return res;
}

inline Vector operator*( float fl, const Vector& v ) {
	return v * fl;
}

inline Vector Vector::Cross( const Vector& vOther ) const {
	Vector res;
	CrossProduct( *this, vOther, res );
	return res;
}

inline float Vector::Length2D( void ) const {
	return ( float )::sqrtf( x * x + y * y );
}

inline float Vector::Length2DSqr( void ) const {
	return ( x * x + y * y );
}

inline Vector CrossProduct( const Vector& a, const Vector& b ) {
	return Vector( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x );
}

inline void VectorMin( const Vector& a, const Vector& b, Vector& result ) {
	result.x = min( a.x, b.x );
	result.y = min( a.y, b.y );
	result.z = min( a.z, b.z );
}

inline void VectorMax( const Vector& a, const Vector& b, Vector& result ) {
	result.x = max( a.x, b.x );
	result.y = max( a.y, b.y );
	result.z = max( a.z, b.z );
}


class VectorAligned : public Vector {
public:
	VectorAligned() {
		x = y = z = 0.0f;
	}

	VectorAligned( const Vector& v ) {
		x = v.x; y = v.y; z = v.z;
	}

	float w;
};

struct Vector2D {
	Vector2D( void ) {
		x = y = 0;
	}

	Vector2D( int X, int Y ) {
		x = X; y = Y;
	}

	int x, y;

	inline Vector2D operator=( Vector2D v ) {
		x = v.x; y = v.y;
		return *this;
	}

	inline Vector2D operator+( Vector2D v ) {
		return Vector2D( x + v.x, y + v.y );
	}

	inline Vector2D operator-( Vector2D v ) {
		return Vector2D( x - v.x, y - v.y );
	}
};

#endif // VECTOR_HPP