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

#include "game_pch.h"
#include "MathHelpers.h"
#include "egf/StandardModelLibraryPropertyIDs.h"
#include <NiMath.h>


    efd::Point3 FORWARD_VECTOR(0, -1, 0);
    efd::Point3 UP_VECTOR(0, 0, 1);
    efd::Point3 DOWN_VECTOR(0, 0, -1);
    efd::Point3 RIGHT_VECTOR(1, 0, 0);
    efd::Point3 LEFT_VECTOR(-1, 0, 0);



bool MathHelpers::ms_bYisUp = false;
float MathHelpers::ms_oneMeter = 1.0f;


static bool FloatEqual(float f1, float f2, float maxDelta = 0.01f)
{
    if (fabs(f1-f2)<maxDelta)
        return true;

    return false;
}


float& MathHelpers::UP_COMPONENT(efd::Point3 & pos)
{
    if (MathHelpers::IsYUp())
        return pos.y;

    return pos.z;
}

float MathHelpers::UP_COMPONENT(const efd::Point3& pos)
{
    if (MathHelpers::IsYUp())
        return pos.y;

    return pos.z;
}

float& MathHelpers::FORWARD_COMPONENT(efd::Point3 & pos)
{
    if (MathHelpers::IsYUp())
        return pos.z;

    return pos.y;
}
float MathHelpers::FORWARD_COMPONENT(const efd::Point3& pos)
{
    if (MathHelpers::IsYUp())
        return pos.z;

    return pos.y;
}

float MathHelpers::GetAngleBetweenVectors (const efd::Point3& u, const efd::Point3 & v)
{
    float dot = u.Dot (v);

    float cosTheta = (dot / (u.SqrLength() * v.SqrLength()));

    if (FloatEqual(cosTheta, 1.0f))
        return 0.0f;

    float arcCosTheta = acosf (cosTheta);
    return arcCosTheta; // result is in radians...
}

float MathHelpers::GetAngleBetweenActorDirections(
    const efd::Point3& vDir1,
    const efd::Point3& vDir2)
{
    float fAmt, fAmt2;

    if (IsYUp())
    {
        fAmt = NiATan2(vDir1.x, vDir1.z);
        fAmt2 = NiATan2 (vDir2.x, vDir2.z);
    }
    else
    {
        fAmt = NiATan2 (vDir1.x, vDir1.y);
        fAmt2 = NiATan2 (vDir2.x, vDir2.y);
    }

    float angle = fAmt - fAmt2;

    angle = RadToDeg(angle);

    if (fabs(angle) > 180.0f)
    {
        // nope, turn the other way!!
        if (angle >= 0)
            angle = -(360.0f - angle);
        else
            angle = 360.0f + angle;
    }

    if (IsYUp())
        return -angle;  // DT41003 Figure out why this is necessary....

    return angle;
}

float MathHelpers::GetAngleBetweenActorDirectionsRad(
    const efd::Point3& vDir1,
    const efd::Point3& vDir2)
{
    float fAmt, fAmt2;

    if (IsYUp())
    {
        fAmt = NiATan2(vDir1.x, vDir1.z);
        fAmt2 = NiATan2 (vDir2.x, vDir2.z);
    }
    else
    {
        fAmt = NiATan2 (vDir1.x, vDir1.y);
        fAmt2 = NiATan2 (vDir2.x, vDir2.y);
    }

    float angle = fAmt - fAmt2;

    return angle;
}

// note: this function assumes what is being passed is the rotation matrix of an object
// who's up vector is parallel to world up vector
float MathHelpers::GetCurrentYaw(const efd::Matrix3& rotation)
{
    efd::Point3 forward = MathHelpers::GetForwardVectorFromMatrix(rotation);

    UP_COMPONENT(forward) = 0;

    float dot = FORWARD_VECTOR * forward;
    float yaw = acosf(dot);

    if (forward * RIGHT_VECTOR > 0)
        yaw *=-1;

    return yaw;
}

void MathHelpers::SetYUp(bool b)
{
    ms_bYisUp = b;

    if (ms_bYisUp)
    {
        FORWARD_VECTOR = efd::Point3(1, 0, 0);
        UP_VECTOR = efd::Point3(0, 1, 0);
        RIGHT_VECTOR = efd::Point3(0, 0, -1);
    }
    else
    {
        FORWARD_VECTOR = efd::Point3(0, -1, 0);
        UP_VECTOR = efd::Point3(0, 0, 1);
        RIGHT_VECTOR = efd::Point3(1, 0, 0);
    }

}

void MathHelpers::FaceDirection(efd::Point3& eulerAngles, const efd::Point3 & cvDesiredFacing)
{
    efd::Matrix3 mat;
    FaceDirection(mat, cvDesiredFacing);
    eulerAngles = Matrix3ToEuler(mat);
}

void MathHelpers::FaceDirection(efd::Matrix3& matrix, const efd::Point3 & cvDesiredFacing)
{
    efd::Point3 vDesiredFacing = cvDesiredFacing * -1.0f;
    vDesiredFacing.Unitize();

    // Quick method to orient the matrix.  This will "work", but cause some instability
    // when facing within eight degrees of the y-axis.
    efd::Point3 vTempUp = UP_VECTOR;
    if (fabs(cvDesiredFacing.Dot(vTempUp)) > 0.99f)
    {
        vTempUp = FORWARD_VECTOR;
    }

    // Figure out left and up vectors
    efd::Point3 vLeft;
    vLeft = vTempUp.Cross(cvDesiredFacing);
    vLeft.Unitize();

    efd::Point3 vRealUp;
    vRealUp = vDesiredFacing.Cross(vLeft);

    // Set Mtx
    matrix.SetCol(AXIS_FORWARD(), vDesiredFacing);
    matrix.SetCol(AXIS_UP(), vRealUp);
    matrix.SetCol(AXIS_RIGHT(), -vLeft);
}

void MathHelpers::FaceDirectionLight(efd::Matrix3& Matrix, const efd::Point3& cvDesiredFacing)
{
    efd::Point3 vDesiredFacing = cvDesiredFacing;
    vDesiredFacing.Unitize();

    // Quick method to orient the matrix.  This will "work", but cause some instability
    // when facing within eight degrees of the y-axis.
    efd::Point3 vTempUp = UP_VECTOR;
    if (fabs(cvDesiredFacing.Dot(vTempUp)) > 0.99f)
    {
        vTempUp = FORWARD_VECTOR;
    }

    // Figure out left and up vectors
    efd::Point3 vLeft;
    vLeft = vTempUp.Cross(cvDesiredFacing);
    vLeft.Unitize();

    efd::Point3 vRealUp;
    vRealUp = vDesiredFacing.Cross(vLeft);

    // Set Mtx
    Matrix.SetCol(0, vDesiredFacing);
    Matrix.SetCol(1, vRealUp);
    Matrix.SetCol(2, -vLeft);
}

efd::Point3 MathHelpers::NormalizeVector(const efd::Point3& vector)
{
    efd::Point3 temp = vector;
    efd::Point3::UnitizeVector(temp);
    return temp;
}

efd::Point2 MathHelpers::RotateVector2D(const efd::Point2& vector, float degrees)
{
    // rotates a 2D vector counter-clockwise by degrees, maintaining its magnitude
    float theta = DegToRad(degrees);
    float cosTheta = cosf(theta);
    float sinTheta = sinf(theta);
    float xrotated = cosTheta * vector.x - sinTheta * vector.y;
    float yrotated = sinTheta * vector.x + cosTheta * vector.y;
    efd::Point2 temp = efd::Point2(xrotated, yrotated);
    return temp;
}

#include <efdPhysX/PhysXTypes.h>

NxExtendedVec3 MathHelpers::ToNxExtended(const efd::Point3& point)
{
    NxExtendedVec3 extended;
    extended.x = point.x;
    extended.y = point.y;
    extended.z = point.z;

    return extended;
}

NxExtendedVec3 MathHelpers::ToNxExtended(const NxVec3& point)
{
    NxExtendedVec3 extended;
    extended.x = point.x;
    extended.y = point.y;
    extended.z = point.z;

    return extended;
}

efd::Point3 MathHelpers::ToPoint3(const NxExtendedVec3& point)
{
    efd::Point3 kTemp;
    kTemp.x = (float)point.x;
    kTemp.y = (float)point.y;
    kTemp.z = (float)point.z;

    return kTemp;
}

double& MathHelpers::UP_COMPONENT(NxExtendedVec3& pos)
{
    if (IsYUp())
        return pos.y;

    if (IsZUp())
        return pos.z;

    return pos.x;
}

double MathHelpers::UP_COMPONENT(const NxExtendedVec3& pos)
{
    if (IsYUp())
        return pos.y;

    if (IsZUp())
        return pos.z;

    return pos.x;
}

float& MathHelpers::UP_COMPONENT(NxVec3& pos)
{
    if (IsYUp())
        return pos.y;

    if (IsZUp())
        return pos.z;

    return pos.x;
}

float MathHelpers::UP_COMPONENT(const NxVec3& pos)
{
    if (IsYUp())
        return pos.y;

    if (IsZUp())
        return pos.z;

    return pos.x;
}

NxVec3 MathHelpers::ToNxVec3(const NxExtendedVec3& point)
{
    NxVec3 temp;
    temp.x = (float)point.x;
    temp.y = (float)point.y;
    temp.z = (float)point.z;
    return temp;
}

NxVec3 MathHelpers::ToNxVec3(const efd::Point3& point)
{
    NxVec3 nxPoint;
    efdPhysX::PhysXTypes::Point3ToNxVec3(point, nxPoint);
    return nxPoint;
}

efd::Point3 MathHelpers::ToPoint3(const NxVec3& point)
{
    efd::Point3 niPoint;
    efdPhysX::PhysXTypes::NxVec3ToPoint3(point, niPoint);
    return niPoint;
}

efd::Matrix3 MathHelpers::ToMatrix3(const NxMat33& matrix)
{
    efd::Matrix3 niMatrix;
    efdPhysX::PhysXTypes::NxMat33ToMatrix3(matrix, niMatrix);
    return niMatrix;
}

NxMat33 MathHelpers::ToNxMat33(const efd::Matrix3& matrix)
{
    NxMat33 nxMatrix;
    efdPhysX::PhysXTypes::Matrix3ToNxMat33(matrix, nxMatrix);
    return nxMatrix;
}

void MathHelpers::Slerp(
    efd::Quaternion& dest,
    const efd::Quaternion& q1,
    const efd::Quaternion& q2,
    float t)
{
    Slerp((float*)&dest, (const float*)&q1, (const float*)&q2, t);
}

void MathHelpers::Slerp(float* pDest, const float* pQ1, const float* pQ2, float t)
{
    float rot1q[4];
    float omega, cosom, sinom;
    float scalerot0, scalerot1;
    int i, j;


    // Calculate the cosine
    cosom = pQ1[0]*pQ2[0] + pQ1[1]*pQ2[1] + pQ1[2]*pQ2[2] + pQ1[3]*pQ2[3];

    // adjust signs if necessary
    if (cosom < 0.0f)
    {
        cosom = -cosom;
        for (j = 0; j < 4; j++)
            rot1q[j] = -pQ2[j];
    }
    else
    {
        for (j = 0; j < 4; j++)
            rot1q[j] = pQ2[j];
    }

    // calculate interpolating coeffs
    if ((1.0 - cosom) > 0.00001f)
    {
        // standard case
        omega = (float)acos(cosom);
        sinom = (float)sin(omega);
        scalerot0 = (float)sin((1.0 - t) * omega) / sinom;
        scalerot1 = (float)sin(t * omega) / sinom;
    }
    else
    {
        // rot0 and rot1 very close - just do linear interp.
        scalerot0 = 1.0f - t;
        scalerot1 = t;
    }

    for (i = 0; i < 4; i++) // build the new quarternion
        pDest[i] = (float)(scalerot0 * pQ1[i] + scalerot1 * rot1q[i]);
}

float MathHelpers::Dist2D(const efd::Point3& pos1, const efd::Point3 & pos2)
{
    efd::Point3 p1 = pos1;
    efd::Point3 p2 = pos2;

    UP_COMPONENT(p1) = 0;
    UP_COMPONENT(p2) = 0;

    return (p1 - p2).Length();
}

float MathHelpers::Dot2D(const efd::Point3& v1, const efd::Point3& v2)
{
    efd::Point3 tempV1 = v1;
    efd::Point3 tempV2 = v2;

    UP_COMPONENT(tempV1) = 0;
    UP_COMPONENT(tempV2) = 0;

    tempV1.Unitize();
    tempV2.Unitize();

    return tempV1 * tempV2;
}

float MathHelpers::GetOneMeter()
{
    return ms_oneMeter;
}

void MathHelpers::SetOneMeter(float fOneMeter)
{
    ms_oneMeter = fOneMeter;
}

bool MathHelpers::IsZero(const efd::Point3& point, float fValue)
{
    if (point.Length() < fValue)
        return true;

    return false;
}

efd::Matrix3 MathHelpers::EulerToMatrix3(const efd::Point3& eulerAngles)
{
    efd::Matrix3 kXRot, kYRot, kZRot;
    kXRot.MakeXRotation(eulerAngles.x * -efd::EE_DEGREES_TO_RADIANS); // Pitch  +x
    kYRot.MakeYRotation(eulerAngles.y * -efd::EE_DEGREES_TO_RADIANS); // Roll +y
    kZRot.MakeZRotation(eulerAngles.z * -efd::EE_DEGREES_TO_RADIANS); // Yaw   +z

    efd::Matrix3 mat3;

    mat3 = kXRot * kYRot * kZRot;

    return mat3;
}

efd::Point3 MathHelpers::Matrix3ToEuler(const efd::Matrix3& matrix)
{
    efd::Point3 eulerAngles;

    matrix.ToEulerAnglesXYZ(eulerAngles.x, eulerAngles.y, eulerAngles.z);

    eulerAngles *= (efd::EE_RADIANS_TO_DEGREES * -1.0f);

    return eulerAngles;
}

efd::Point3 MathHelpers::GetForwardVectorFromEuler(const efd::Point3& euler)
{
    efd::Matrix3 mat = EulerToMatrix3(euler);
    return GetForwardVectorFromMatrix(mat);
}

efd::Point3 MathHelpers::GetRightVectorFromEuler(const efd::Point3& euler)
{
    efd::Matrix3 mat = EulerToMatrix3(euler);
    return GetRightVectorFromMatrix(mat);

}

efd::Point3 MathHelpers::GetUpVectorFromEuler(const efd::Point3& euler)
{
    efd::Matrix3 mat = EulerToMatrix3(euler);
    return GetUpVectorFromMatrix(mat);
}


float MathHelpers::FixAngle(const float input)
{
    int wrap = int(input / efd::EE_PI);
    if (wrap & 1)
    {
        wrap += wrap < 0 ? -1 : 1;
    }
    return input - float(wrap * efd::EE_PI);
}

float MathHelpers::FixAngleDeg(const float input)
{
    int wrap = int(input / 180.0f);
    if (wrap & 1)
    {
        wrap += wrap < 0 ? -1 : 1;
    }
    return input - float(wrap * efd::EE_PI);
}

void MathHelpers::FixAngle(efd::Point3* angle)
{
    angle->x = FixAngle(angle->x);
    angle->y = FixAngle(angle->y);
    angle->z = FixAngle(angle->z);
}