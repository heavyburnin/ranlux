#pragma once

#include "../main.hpp"

#ifndef _ANGLE_HPP
#define _ANGLE_HPP

#define rnd_max 0x7FFF
#define FastSqrt( x )	( sqrtf( x ) )
#define FloatMakePositive( x ) ( fabsf( x ) )

//-----------------------------------------------------------------------------
// Degree Euler angle pitch, yaw, roll
//-----------------------------------------------------------------------------
class angleByValue;

class angle {
public:
	// Members
	float x, y, z;

	// Construction/destruction
	angle( void );
	angle( float X, float Y, float Z );
	//	angle(RadianEuler const &angles);	// evil auto type promotion!!!

	// Allow pass-by-value
	operator angleByValue &( ) { return *( ( angleByValue * ) ( this ) ); }
	operator const angleByValue &( ) const { return *( ( const angleByValue * ) ( this ) ); }

	// Initialization
	void Init( float ix = 0.0f, float iy = 0.0f, float iz = 0.0f );
	void Random( float minVal, float maxVal );

	// Got any nasty NAN's?
	bool IsFinite( float flx ) const;
	bool IsValid() const;
	void Invalidate();

	// array access...
	float operator[]( int i ) const;
	float& operator[]( int i );

	// Base address...
	float* Base();
	float const* Base() const;

	// equality
	bool operator==( const angle& v ) const;
	bool operator!=( const angle& v ) const;

	// arithmetic operations
	angle&	operator+=( const angle &v );
	angle&	operator-=( const angle &v );
	angle&	operator*=( float s );
	angle&	operator/=( float s );

	// Get the vector's magnitude.
	float	Length() const;
	float	LengthSqr() const;

	// negate the angle components
	//void	Negate();

	// No assignment operators either...
	angle& operator=( const angle& src );

#ifndef VECTOR_NO_SLOW_OPERATIONS
	// copy constructors

	// arithmetic operations
	angle	operator-( void ) const;

	angle	operator+( const angle& v ) const;
	angle	operator-( const angle& v ) const;
	angle	operator*( float fl ) const;
	angle	operator/( float fl ) const;
#else

private:
	// No copy constructors allowed if we're in optimal mode
	angle( const angle& vOther );

#endif
};

FORCEINLINE void NetworkVarConstruct( angle &q ) { q.x = q.y = q.z = 0.0f; }

//-----------------------------------------------------------------------------
// Allows us to specifically pass the vector by value when we need to
//-----------------------------------------------------------------------------
class angleByValue : public angle {
public:
	// Construction/destruction:
	angleByValue( void ) : angle() {}
	angleByValue( float X, float Y, float Z ) : angle( X, Y, Z ) {}
	angleByValue( const angleByValue& vOther ) { *this = vOther; }
};

inline void VectorAdd( const angle& a, const angle& b, angle& result ) {
	CHECK_VALID( a );
	CHECK_VALID( b );
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
}

inline void VectorMA( const angle &start, float scale, const angle &direction, angle &dest ) {
	CHECK_VALID( start );
	CHECK_VALID( direction );
	dest.x = start.x + scale * direction.x;
	dest.y = start.y + scale * direction.y;
	dest.z = start.z + scale * direction.z;
}

//-----------------------------------------------------------------------------
// constructors
//-----------------------------------------------------------------------------
inline angle::angle( void ) {
#ifdef _DEBUG
#ifdef VECTOR_PARANOIA
	// Initialize to NAN to catch errors
	x = y = z = float_NAN;
#endif
#endif
}

inline angle::angle( float X, float Y, float Z ) {
	x = X; y = Y; z = Z;
	CHECK_VALID( *this );
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------
inline void angle::Init( float ix, float iy, float iz ) {
	x = ix; y = iy; z = iz;
	CHECK_VALID( *this );
}

inline void angle::Random( float minVal, float maxVal ) {
	x = minVal + ( ( float ) rand() / rnd_max ) * ( maxVal - minVal );
	y = minVal + ( ( float ) rand() / rnd_max ) * ( maxVal - minVal );
	z = minVal + ( ( float ) rand() / rnd_max ) * ( maxVal - minVal );
	CHECK_VALID( *this );
}

#ifndef VECTOR_NO_SLOW_OPERATIONS

inline angle RandomAngle( float minVal, float maxVal ) {
	angle random;
	random.Random( minVal, maxVal );
	angle ret( random.x, random.y, random.z );
	return ret;
}

#endif

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------
inline angle& angle::operator=( const angle &vOther ) {
	CHECK_VALID( vOther );
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}

//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------
inline float& angle::operator[]( int i ) {
	Assert( ( i >= 0 ) && ( i < 3 ) );
	return ( ( float* )this )[ i ];
}

inline float angle::operator[]( int i ) const {
	Assert( ( i >= 0 ) && ( i < 3 ) );
	return ( ( float* )this )[ i ];
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------
inline float* angle::Base() {
	return ( float* )this;
}

inline float const* angle::Base() const {
	return ( float const* )this;
}

//-----------------------------------------------------------------------------
// IsFinite
//-----------------------------------------------------------------------------
inline bool angle::IsFinite( float flx ) const {
	return ( ( *reinterpret_cast< unsigned long* >( &flx ) & 0x7F800000 ) != 0x7F800000 );
}

//-----------------------------------------------------------------------------
// IsValid
//-----------------------------------------------------------------------------
inline bool angle::IsValid() const {
	return IsFinite( x ) && IsFinite( y ) && IsFinite( z );
}

//-----------------------------------------------------------------------------
// Invalidate
//-----------------------------------------------------------------------------
inline void angle::Invalidate() {
	//#ifdef _DEBUG
	//#ifdef VECTOR_PARANOIA
	x = y = z = float_NAN;
	//#endif
	//#endif
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------
inline bool angle::operator==( const angle& src ) const {
	CHECK_VALID( src );
	CHECK_VALID( *this );
	return ( src.x == x ) && ( src.y == y ) && ( src.z == z );
}

inline bool angle::operator!=( const angle& src ) const {
	CHECK_VALID( src );
	CHECK_VALID( *this );
	return ( src.x != x ) || ( src.y != y ) || ( src.z != z );
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
inline void VectorCopy( const angle& src, angle& dst ) {
	CHECK_VALID( src );
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}

//-----------------------------------------------------------------------------
// standard math operations
//-----------------------------------------------------------------------------
inline angle& angle::operator+=( const angle& v ) {
	CHECK_VALID( *this );
	CHECK_VALID( v );
	x += v.x; y += v.y; z += v.z;
	return *this;
}

inline angle& angle::operator-=( const angle& v ) {
	CHECK_VALID( *this );
	CHECK_VALID( v );
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

inline angle& angle::operator*=( float fl ) {
	x *= fl;
	y *= fl;
	z *= fl;
	CHECK_VALID( *this );
	return *this;
}

inline angle& angle::operator/=( float fl ) {
	Assert( fl != 0.0f );
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	CHECK_VALID( *this );
	return *this;
}

//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------
inline float angle::Length() const {
	CHECK_VALID( *this );
	return ( float ) FastSqrt( LengthSqr() );
}

inline float angle::LengthSqr() const {
	CHECK_VALID( *this );
	return x * x + y * y + z * z;
}

//-----------------------------------------------------------------------------
// Vector equality with tolerance
//-----------------------------------------------------------------------------
inline bool anglesAreEqual( const angle& src1, const angle& src2, float tolerance = 0.0f ) {
	if( FloatMakePositive( src1.x - src2.x ) > tolerance )
		return false;
	if( FloatMakePositive( src1.y - src2.y ) > tolerance )
		return false;
	return ( FloatMakePositive( src1.z - src2.z ) <= tolerance );
}

//-----------------------------------------------------------------------------
// arithmetic operations (SLOW!!)
//-----------------------------------------------------------------------------
inline angle angle::operator-( void ) const {
	angle ret( -x, -y, -z );
	return ret;
}

inline angle angle::operator+( const angle& v ) const {
	angle res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}

inline angle angle::operator-( const angle& v ) const {
	angle res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}

inline angle angle::operator*( float fl ) const {
	angle res;
	res.x = x * fl;
	res.y = y * fl;
	res.z = z * fl;
	return res;
}

inline angle angle::operator/( float fl ) const {
	angle res;
	res.x = x / fl;
	res.y = y / fl;
	res.z = z / fl;
	return res;
}

inline angle operator*( float fl, const angle& v ) {
	angle ret( v * fl );
	return ret;
}

#endif // !_ANGLE_HPP