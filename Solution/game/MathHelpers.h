// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not
// be copied or disclosed except in accordance with the terms of that
// agreement.
//
//      Copyright (c) 1996-2007 Emergent Game Technologies.
//      All Rights Reserved.
//
// Emergent Game Technologies, Calabasas, CA 91302
// http://www.emergent.net

#pragma once

#ifndef EE_MATHHELPERS_H
#define EE_MATHHELPERS_H

#include <efd/Matrix3.h>
#include <efd/Point3.h>
#include <efd/Point2.h>
#include <efd/Color.h>
#include <NiBound.h>
#include <NxExtended.h>
#include <efd/EEMath.h>




/// @name Constants
//@{

/// The direction that is considered forward.
extern efd::Point3 FORWARD_VECTOR;
/// The direction that is considered up.
extern efd::Point3 UP_VECTOR;
/// The direction that is considered down.
extern efd::Point3 DOWN_VECTOR;
/// The direction that is considered right.
extern efd::Point3 RIGHT_VECTOR;
/// The direction that is considered left.
extern efd::Point3 LEFT_VECTOR;

#ifndef RAND_MAX
#define RAND_MAX 0x7fff
#endif

//@}

/**
A class containing helpful static functions for performing common mathematical operations
*/
class MathHelpers
{
public:

	///@name World Scale and Orientation Functions
	//@{

	/**
	Returns the up component of a vector. This could be either the Y or Z component depending
	on the current up vector setting.
	@param pos The vector.
	@return A reference to up component of the vector (an assignable l-value).
	*/
	static float& UP_COMPONENT(efd::Point3& pos);
	/**
	Returns the up component of a vector. This could be either the Y or Z component depending
	on the current up vector setting.
	@param pos The vector.
	@return The value of up component of the vector.
	*/
	static float UP_COMPONENT(const efd::Point3& pos);

	/// Returns the up component of a PhysX vector.  This could be the Y or Z component depending
	/// on the current up vector setting.
	/// @param pos The vector.
	/// @return The up component of the vector.
	static float& UP_COMPONENT(NxVec3 & pos);
	/// Returns the up component of a PhysX vector.  This could be the Y or Z component depending
	/// on the current up vector setting.
	/// @param pos The vector.
	/// @return The up component of the vector.
	static float UP_COMPONENT(const NxVec3& pos);

	/// Returns the up component of a PhysX extended vector.  This could be the Y or Z component
	/// depending on the current up vector setting.
	/// @param pos The vector.
	/// @return The up component of the vector.
	static double& UP_COMPONENT(NxExtendedVec3 & pos);
	/// Returns the up component of a PhysX extended vector.  This could be the Y or Z component
	/// depending on the current up vector setting.
	/// @param pos The vector.
	/// @return The up component of the vector.
	static double UP_COMPONENT(const NxExtendedVec3& pos);

	/// Returns the forward component of a vector.  This could be the X or Y component depending on
	/// the current up vector setting.
	/// @param pos The vector.
	/// @return The forward component of the vector.
	static float& FORWARD_COMPONENT(efd::Point3& pos);
	/// Returns the forward component of a vector.  This could be the X or Y component depending on
	/// the current up vector setting.
	/// @param pos The vector.
	/// @return The forward component of the vector.
	static float FORWARD_COMPONENT(const efd::Point3& pos);

	/// Returns the number of game units that equal one meter in the physical simulation.
	/// @return The current meter conversion factor.
	static float GetOneMeter();
	/// Sets the number of game units that equal one meter in the physical simulation.
	/// @param oneMeter The new meter conversion factor.
	static void SetOneMeter(float oneMeter);

	/// Sets whether Y is up or Z is up.
	/// @param b True if Y is up and X is forward.  False if Z is up and Y is forward.
	static void SetYUp(bool b);
	/// Checks whether Y is the current up axis.
	/// @return True if Y is up, false if Z is up.
	static bool IsYUp()
	{
		return ms_bYisUp;
	}
	/// Checks whether Z is the current up axis.
	/// @return True if Z is up, false if Y is up.
	static bool IsZUp()
	{
		return !ms_bYisUp;
	}

	/// Extracts the forward vector from a rotation matrix based on the current up vector setting.
	/// @param rMatrix The matrix to extract the vector from.
	/// @return The extracted vector.
	static efd::Point3 GetForwardVectorFromMatrix(const efd::Matrix3& rMatrix)
	{
		efd::Point3 vRetVal;
		rMatrix.GetCol(AXIS_FORWARD(), vRetVal);

		if (IsYUp())
		{
			return vRetVal;
		}
		else
		{
			return vRetVal * -1.0f;
		}
	}
	/// Extracts the right vector from a rotation matrix based on the current up vector setting.
	/// @param rMatrix The matrix to extract the vector from.
	/// @return The extracted vector.
	static efd::Point3 GetRightVectorFromMatrix(const efd::Matrix3& rMatrix)
	{
		efd::Point3 vRetVal;
		rMatrix.GetCol(AXIS_RIGHT(), vRetVal);
		return vRetVal;
	}
	/// Extracts the up vector from a rotation matrix based on the current up vector setting.
	/// @param rMatrix The matrix to extract the vector from.
	/// @return The extracted vector.
	static efd::Point3 GetUpVectorFromMatrix(const efd::Matrix3& rMatrix)
	{
		efd::Point3 vRetVal;
		rMatrix.GetCol(AXIS_UP(), vRetVal);
		return vRetVal;
	}
	/// Extracts the forward, right, and up vectors from a rotation matrix based on the current up
	/// vector setting.
	/// @param rMatrix The matrix to extract the vectors from.
	/// @param forward The extracted forward vector will be placed here.
	/// @param right The extracted right vector will be placed here.
	/// @param up The extracted up vector will be placed here.
	static void GetRotationVectors(const efd::Matrix3& rMatrix,
		efd::Point3& forward, efd::Point3& right, efd::Point3& up)
	{
		efd::Point3 vRetVal;
		rMatrix.GetCol(AXIS_FORWARD(), forward);
		rMatrix.GetCol(AXIS_RIGHT(), right);
		rMatrix.GetCol(AXIS_UP(), up);
	}

	/// Calculates the forward vector from a set of euler angle rotations based on the current up
	/// vector setting.
	/// @param euler The euler angles to calculate the vector from.
	/// @return The calculated vector.
	static efd::Point3 GetForwardVectorFromEuler(const efd::Point3& euler);
	/// Calculates the forward vector from a set of euler angle rotations based on the current up
	/// vector setting.
	/// @param euler The euler angles to calculate the vector from.
	/// @return The calculated vector.
	static efd::Point3 GetRightVectorFromEuler(const efd::Point3& euler);
	/// Calculates the up vector from a set of euler angle rotations based on the current up vector
	/// setting.
	/// @param euler The euler angles to calculate the vector from.
	/// @return The calculated vector.
	static efd::Point3 GetUpVectorFromEuler(const efd::Point3& euler);

	/// Returns array position of the current up axis.
	/// @return 1 for Y or 2 for Z.
	static int AXIS_UP()
	{
		if (ms_bYisUp)
			return 1;

		return 2;
	}
	/// Returns array position of the current forward axis.
	/// @return 0 for X or 1 for Y.
	static int AXIS_FORWARD()
	{
		if (ms_bYisUp)
			return 0;

		return 1;
	}
	/// Returns array position of the current right axis.
	/// @return 0 for X or 2 for Z.
	static int AXIS_RIGHT()
	{
		if (ms_bYisUp)
			return 2;
		return 0;
	}

	//@}

	///@name Rotation Functions
	//@{

	/// Calculates the current yaw of a rotation matrix.
	/// @param rotation The matrix to calculate the yaw from.
	/// @return The calculated yaw in radians.
	static float GetCurrentYaw(const efd::Matrix3& rotation);

	/// Converts Euler angles to a 3x3 rotation matrix.
	/// @param eulerAngles The angles to convert.
	/// @return The matrix result of the conversion.
	static efd::Matrix3 EulerToMatrix3(const efd::Point3& eulerAngles);
	/// Converts a 3x3 rotation matrix to Euler angles.
	/// @param matrix The matrix to convert.
	/// @return The angles resulting from the conversion.
	static efd::Point3 Matrix3ToEuler(const efd::Matrix3& matrix);

	/// Reorients a rotation matrix about the current up axis so its forward vector is facing in a
	/// direction.  This is a quick method to orient the matrix.  It will "work", but cause some
	/// instability when facing within eight degrees of the y-axis.
	/// @param matrix The matrix to rotate.
	/// @param cvDesiredFacing The direction that the matrix should face.
	static void FaceDirection(efd::Matrix3& matrix, const efd::Point3& cvDesiredFacing);
	/// Reorients a set of euler angles about the current up axis so its forward vector is facing
	/// in a direction.
	/// @param eulerAngles The angles to rotate.
	/// @param cvDesiredFacing The direction that the angles should face.
	static void FaceDirection(efd::Point3& eulerAngles, const efd::Point3& cvDesiredFacing);
	/// Reorients a rotation matrix about the up axis so its forward vector is facing in a
	/// direction.  This is a quick method to orient the matrix.  It will "work", but cause some
	/// instability when facing within eight degrees of the y-axis.
	/// @note This version of the function does not take the current up vector setting into account.
	/// @param matrix The matrix to rotate.
	/// @param cvDesiredFacing The direction that the matrix should face.
	static void FaceDirectionLight(efd::Matrix3& matrix, const efd::Point3& cvDesiredFacing);

	/// Performs spherical linear interpolation between two quaternions.
	/// @param dest The result of the interpolation will be placed here.
	/// @param q1 The first quaternion.
	/// @param q2 The second quaternion.
	/// @param t The Amount of interpolation from q1 to q2 in a range of 0 to 1.
	static void Slerp(
		efd::Quaternion& dest,
		const efd::Quaternion& q1,
		const efd::Quaternion& q2,
		float t);

	//@}

	///@name Random Number Functions
	//@{

	/// Generates a random integer.
	/// @return The generated number.
	static int GetRandomInteger()
	{
		return efd::Rand();
	}
	/// Generates a random integer within a range.
	/// @param iMin The minimum of the range.
	/// @param iMax The maximum of the range.
	/// @return The generated number.
	static int GetRandomIntegerRange(int iMin, int iMax)
	{
		return (GetRandomInteger() % (iMax - iMin + 1)) + iMin;
	}
	/// Generates a random integer between zero and a maximum.
	/// @param iMax The maximum of the range.
	/// @return The generated number.
	static int GetRandomIntegerRangeZeroTo(int iMax)
	{
		return GetRandomInteger() / (RAND_MAX / iMax + 1);
	}
	/// Generates a random floating point number within a range.
	/// @param min The minimum of the range.
	/// @param max The maximum of the range.
	/// @return The generated number.
	static float GetRandomFloat(float min, float max)
	{
		float num = min + (max - min) * efd::UnitRandom();
		return num;
	}

	//@}

	///@name 3D Vector Functions
	//@{

	/// Calculates the angle between two vectors.
	/// @param u The first vector.
	/// @param v The second vector.
	/// @return The angle between u and v in radians.
	static float GetAngleBetweenVectors(const efd::Point3& u, const efd::Point3& v);
	/// Calculates the angle between two actor directions in degrees.
	/// @param vDir1 The first direction.
	/// @param vDir2 The second direction.
	/// @return The angle between vDir1 and vDir2 in degrees.
	static float GetAngleBetweenActorDirections(
		const efd::Point3& vDir1,
		const efd::Point3& vDir2);
	/// Calculates the angle between two actor directions in radians.
	/// @param vDir1 The first direction.
	/// @param vDir2 The second direction.
	/// @return The angle between vDir1 and vDir2 in radians.
	static float GetAngleBetweenActorDirectionsRad(
		const efd::Point3& vDir1,
		const efd::Point3& vDir2);

	/// Normalizes a vector.
	/// @param vector The vector to normalize.
	/// @return The normalized vector.
	static efd::Point3 NormalizeVector(const efd::Point3& vector);

	/// Checks if a point is equal to zero within a given error threshold.
	/// @param point The point to test.
	/// @param fValue The comparison threshold to use.
	/// @return True if the point's distance from zero is below the threshold.
	static bool IsZero(const efd::Point3& point, float fValue = 0.0001f);

	//@}

	///@name 2D Vector Functions
	//@{

	/// Rotates a 2D vector by an angle in degrees
	/// @param vector The vector to rotate.
	/// @param degrees The angle to rotate the vector by in degrees.
	/// @return The rotated vector.
	static efd::Point2 RotateVector2D(const efd::Point2& vector, float degrees);

	/// Calculates the distance between two 2D points
	/// @param pos1 The first point.
	/// @param pos2 The second point.
	/// @return The distance between the two points.
	static float Dist2D(const efd::Point3& pos1, const efd::Point3& pos2);

	/// Calculates the dot product of two 2D vectors
	/// @param v1 The first vector.
	/// @param v2 The second vector.
	/// @return The dot product.
	static float Dot2D(const efd::Point3& v1, const efd::Point3& v2);

	//@}

	///@name PhysX Vector Conversion Functions
	//@{

	/// Converts a Gamebryo point to a PhysX extended vector.
	/// @param point The point to convert.
	/// @return The converted vector.
	static NxExtendedVec3 ToNxExtended(const efd::Point3& point);
	/// Converts a PhysX vector to a PhysX extended vector.
	/// @param point The vector to convert.
	/// @return The converted vector.
	static NxExtendedVec3 ToNxExtended(const NxVec3& point);
	/// Converts a Gamebryo point to a PhysX vector.
	/// @param point The point to convert.
	/// @return The converted vector.
	static NxVec3 ToNxVec3(const efd::Point3& point);
	/// Converts a PhysX extended vector to a PhysX vector.
	/// @param point The vector to convert.
	/// @return The converted vector.
	static NxVec3 ToNxVec3(const NxExtendedVec3& point);
	/// Converts a PhysX vector to a Gamebryo point.
	/// @param point The vector to convert.
	/// @return The converted point.
	static efd::Point3 ToPoint3(const NxVec3& point);
	/// Converts a PhysX extended vector to a Gamebryo point.
	/// @param point The vector to convert.
	/// @return The converted point.
	static efd::Point3 ToPoint3(const NxExtendedVec3& point);
	/// Converts a PhysX rotation matrix to a Gamebryo rotation matrix.
	/// @param matrix The matrix to convert.
	/// @return The converted matrix.
	static efd::Matrix3 ToMatrix3(const NxMat33& matrix);
	/// Converts a Gamebryo rotation matrix to a PhysX rotation matrix.
	/// @param matrix The matrix to convert.
	/// @return The converted matrix.
	static NxMat33 ToNxMat33(const efd::Matrix3& matrix);

	//@}

	///@name Angle Functions
	//@{

	/// Converts from degrees to radians.
	/// @param fDegrees An angle in degrees.
	/// @return The angle converted to radians.
	static float DegToRad(const float& fDegrees)
	{
		return fDegrees * efd::EE_DEGREES_TO_RADIANS;
	}
	/// Converts radians to degrees.
	/// @param fRad An angle in radians.
	/// @return The angle converted to degrees.
	static float RadToDeg(const float& fRad)
	{
		return fRad * efd::EE_RADIANS_TO_DEGREES;
	}

	/// Calculates a coterminal angle in the standard position (0 to 2 pi) of the given angle in
	/// radians.
	/// @param input The angle to fix in radians.
	/// @return The fixed angle in radians.
	static float FixAngle(const float input);
	/// Calculates a coterminal angle in the standard position (0 to 360 degrees) of the given angle
	/// in degrees.
	/// @param input The angle to fix in degrees.
	/// @return The fixed angle in degrees.
	static float FixAngleDeg(const float input);
	/// Calculates coterminal angles in the standard position (0 to 2 pi) for all of the components
	/// of the given vector.
	/// @param angle The vector containing the angles to fix in radians.
	/// @return A vector containing the fixed angles in radians.
	static void FixAngle(efd::Point3* angle);

	//@}

private:
	static float ms_oneMeter;
	static void Slerp(float* pDest, const float* pQ1, const float* pQ2, float t);
	/// If true: Y up, X forward
	/// If false: Z up, Y forward
	static bool ms_bYisUp;
};

#define ONE_METER MathHelpers::GetOneMeter()

#define EE_YAW(rotation)   rotation.z
#define EE_PITCH(rotation)   rotation.x
#define EE_ROLL(rotation)   rotation.y



#endif  // EE_MATHHELPERS_H

