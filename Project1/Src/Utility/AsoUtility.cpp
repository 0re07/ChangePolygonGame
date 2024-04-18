#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <DxLib.h>
#include "AsoUtility.h"

int AsoUtility::Round(float v)
{
    return static_cast<int>(roundf(v));
}

std::vector<std::string> AsoUtility::Split(std::string& line, char delimiter)
{

    std::istringstream stream(line);
    std::string field;
    std::vector<std::string> result;
    
    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }

    return result;

}

double AsoUtility::Rad2DegD(double rad)
{
    return rad * (180.0 / DX_PI);
}

float AsoUtility::Rad2DegF(float rad)
{
    return rad * (180.0f / DX_PI_F);
}

int AsoUtility::Rad2DegI(int rad)
{
    return rad * Round(180.0f / DX_PI_F);
}

double AsoUtility::Deg2RadD(double deg)
{
    return deg * (DX_PI / 180.0);
}

float AsoUtility::Deg2RadF(float deg)
{
    return deg * (DX_PI_F / 180.0f);
}

int AsoUtility::Deg2RadI(int deg)
{
    return deg * Round(DX_PI_F / 180.0f);
}

double AsoUtility::DegIn360(double deg)
{
    deg = fmod(deg, 360.0);
    if (deg < 0.0f)
    {
        deg += 360.0;
    }
    return deg;
}

double AsoUtility::RadIn2PI(double rad)
{
    rad = fmod(rad, DX_TWO_PI);
    if (rad < 0.0)
    {
        rad += DX_TWO_PI;
    }
    return rad;
}

int AsoUtility::DirNearAroundRad(float from, float to)
{

    float ret = 1.0f;

    float diff = to - from;

    if (diff >= 0.0f)
    {

        // ��r���������v���Ɉʒu����

        if (diff > DX_PI_F)
        {
            // �ł��A180�x�ȏ㗣��Ă���̂ŁA�����v���̕����߂�
            ret = -1.0f;
        }
        else
        {
            // ���v���
            ret = 1.0f;
        }

    }
    else
    {

        // ��r�����������v���Ɉʒu����

        if (diff < -DX_PI_F)
        {
            // �ł��A180�x�ȏ㗣��Ă���̂ŁA���v���̕����߂�
            ret = 1.0f;
        }
        else
        {
            // �����v���
            ret = -1.0f;
        }

    }

    return static_cast<int>(ret);

}

int AsoUtility::DirNearAroundDeg(float from, float to)
{

    float ret = 1.0f;

    float diff = to - from;

    if (diff >= 0.0f)
    {

        // ��r���������v���Ɉʒu����

        if (diff > 180.0f)
        {
            // �ł��A180�x�ȏ㗣��Ă���̂ŁA�����v���̕����߂�
            ret = -1.0f;
        }
        else
        {
            // ���v���
            ret = 1.0f;
        }

    }
    else
    {

        // ��r�����������v���Ɉʒu����

        if (diff < -180.0f)
        {
            // �ł��A180�x�ȏ㗣��Ă���̂ŁA���v���̕����߂�
            ret = 1.0f;
        }
        else
        {
            // �����v���
            ret = -1.0f;
        }

    }

    return static_cast<int>(ret);

}

int AsoUtility::Lerp(int start, int end, float t)
{
    // ���`���
    if (t >= 1.0f)
    {
        return end;
    }

    int ret = start;
    ret += Round(t * static_cast<float>(end - start));
    return ret;
}

float AsoUtility::Lerp(float start, float end, float t)
{
    // ���`���
    if (t >= 1.0f)
    {
        return end;
    }

    float ret = start;
    ret += t * (end - start);
    return ret;
}

double AsoUtility::Lerp(double start, double end, double t)
{
    // ���`���
    if (t >= 1.0)
    {
        return end;
    }

    double ret = start;
    ret += t * (end - start);
    return ret;
}

Vector2 AsoUtility::Lerp(const Vector2& start, const Vector2& end, float t)
{
    // ���`���
    if (t >= 1.0f)
    {
        return end;
    }

    Vector2 ret = start;
    ret.x += Round(t * static_cast<float>((end.x - start.x)));
    ret.y += Round(t * static_cast<float>((end.y - start.y)));
    return ret;
}

VECTOR AsoUtility::Lerp(const VECTOR& start, const VECTOR& end, float t)
{
    // ���`���
    if (t >= 1.0f)
    {
        return end;
    }

    VECTOR ret = start;
    ret.x += t * (end.x - start.x);
    ret.y += t * (end.y - start.y);
    ret.z += t * (end.z - start.z);

    return ret;
}

double AsoUtility::LerpDeg(double start, double end, double t)
{

    double ret;

    double diff = end - start;
    if (diff < -180.0)
    {
        end += 360.0;
        ret = Lerp(start, end, t);
        if (ret >= 360.0)
        {
            ret -= 360.0;
        }
    }
    else if (diff > 180.0)
    {
        end -= 360.0;
        ret = Lerp(start, end, t);
        if (ret < 0.0)
        {
            ret += 360.0;
        }
    }
    else
    {
        ret = Lerp(start, end, t);
    }

    return ret;

}

COLOR_F AsoUtility::Lerp(const COLOR_F& start, const COLOR_F& end, float t)
{
    // ���`���
    if (t >= 1.0f)
    {
        return end;
    }

    COLOR_F ret = start;
    ret.r += t * (end.r - start.r);
    ret.g += t * (end.g - start.g);
    ret.b += t * (end.b - start.b);
    ret.a += t * (end.a - start.a);
    return ret;
}

Vector2 AsoUtility::Bezier(const Vector2& p1, const Vector2& p2, const Vector2& p3, float t)
{
    Vector2 a = Lerp(p1, p2, t);
    Vector2 b = Lerp(p2, p3, t);
    return Lerp(a, b, t);
}

VECTOR AsoUtility::Bezier(const VECTOR& p1, const VECTOR& p2, const VECTOR& p3, float t)
{
    VECTOR a = Lerp(p1, p2, t);
    VECTOR b = Lerp(p2, p3, t);
    return Lerp(a, b, t);
}

VECTOR AsoUtility::RotXZPos(const VECTOR& centerPos, const VECTOR& radiusPos, float rad)
{
    // �������Ȃ��Ă悢
    float x = ((radiusPos.x - centerPos.x) * cosf(rad)) - ((radiusPos.z - centerPos.z) * sinf(rad));
    float z = ((radiusPos.x - centerPos.x) * sinf(rad)) + ((radiusPos.z - centerPos.z) * cosf(rad));
    return VGet(centerPos.x + x, radiusPos.y, centerPos.z + z);
}

double AsoUtility::Magnitude(const Vector2& v)
{
    return sqrt((v.x * v.x) + (v.y * v.y));
}

double AsoUtility::Magnitude(const VECTOR& v)
{
    return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

int AsoUtility::SqrMagnitude(const Vector2& v)
{
    return v.x * v.x + v.y * v.y;
}

float AsoUtility::SqrMagnitudeF(const VECTOR& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

double AsoUtility::SqrMagnitude(const VECTOR& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

double AsoUtility::Distance(const Vector2& v1, const Vector2& v2)
{
    return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2));
}

double AsoUtility::Distance(const VECTOR& v1, const VECTOR& v2)
{
    return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2));
}

double AsoUtility::SqrMagnitude(const VECTOR& v1, const VECTOR& v2)
{
    return pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2);
}

VECTOR AsoUtility::GetClosestPosTriangle(
    const VECTOR& tPos1, const VECTOR& tPos2, const VECTOR& tPos3, const VECTOR& sPos1)
{
    VECTOR ab = VSub(tPos2, tPos1);
    VECTOR ac = VSub(tPos3, tPos1);

    // �ŏ��ɎO�p�`�̊O���ɂ��邩�̔�����s���A
    // �O���ɂ�����A�����ɎO�p�`�ʏ�̍Őڋߍ��W��Ԃ�

    // closest to A
    VECTOR ap = VSub(sPos1, tPos1);
    float d1 = VDot(ab, ap);
    float d2 = VDot(ac, ap);
    if (d1 <= 0.0f && d2 <= 0.0f)
    {
        return tPos1;
    }

    // closest to B
    VECTOR bp = VSub(sPos1, tPos2);
    float d3 = VDot(ab, bp);
    float d4 = VDot(ac, bp);
    if (d3 >= 0.0f && d4 <= d3)
    {
        return tPos2;
    }

    // closest to C
    VECTOR cp = VSub(sPos1, tPos3);
    float d5 = VDot(ab, cp);
    float d6 = VDot(ac, cp);
    if (d6 >= 0.0f && d5 <= d6)
    {
        return tPos3;
    }

    // closest to AB
    float vc = d1 * d4 - d3 * d2;
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
    {
        float v = d1 / (d1 - d3);
        return VAdd(tPos1, VScale(ab, v));
    }

    // closest to AC
    float vb = d5 * d2 - d1 * d6;
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
    {
        float w = d2 / (d2 - d6);
        return VAdd(tPos1, VScale(ac, w));
    }

    // closest to BC
    float va = d3 * d6 - d5 * d4;
    if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
    {
        float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return VAdd(tPos2, VScale(VSub(tPos3, tPos2), w));
    }

    // �O�p�`�̓����ɂ���

    // �O�p�`�̖@���x�N�g�������߂�
    VECTOR n = VCross(ab, ac);
    n = VNorm(n);
    if (n.x == -1.0f && n.y == -1.0f && n.z == -1.0f)
    {
        // �꒼����ɒ��_�����сA�@���x�N�g�������Ȃ�
        // ���̏ꍇ�́A�R���_�̂����A�ł��߂����_��Ԃ�
        float aPow = SqrMagnitudeF(ap);
        float bPow = SqrMagnitudeF(bp);
        float cPow = SqrMagnitudeF(cp);
        if (aPow <= bPow)
        {
            if (aPow <= cPow) { return tPos1; }
            else { return tPos3; }
        }
        else
        {
            if (bPow <= cPow) { return tPos2; }
            else { return tPos3; }
        }
    }

    // ���̒��S�ƎO�p�`�̖ʂƂ̋��������߂�
    float d = VDot(n, ap);
    
    return VAdd(sPos1, VScale(n, -d));
}

VECTOR AsoUtility::GetClosestPosTriangleBeta(
    const VECTOR& tPos1, const VECTOR& tPos2, const VECTOR& tPos3, const VECTOR& sPos1)
{

    VECTOR ab = VSub(tPos2, tPos1);
    VECTOR ac = VSub(tPos3, tPos1);
    VECTOR ba = VSub(tPos1, tPos2);
    VECTOR bc = VSub(tPos3, tPos2);
    VECTOR ca = VSub(tPos1, tPos3);
    VECTOR cb = VSub(tPos2, tPos3);

    // �ŋߐړ_�@���_�`
    VECTOR ap = VSub(sPos1, tPos1);
    float dotABAP = VDot(ab, ap);
    float dotACAP = VDot(ac, ap);
    if (dotABAP <= 0.0f && dotACAP <= 0.0f)
    {
        return tPos1;
    }

    // �ŋߐړ_�@���_�a
    VECTOR bp = VSub(sPos1, tPos2);
    float dotBABP = VDot(ba, bp);
    float dotBCBP = VDot(bc, bp);
    if (dotBABP <= 0.0f && dotBCBP <= 0.0f)
    {
        return tPos2;
    }

    // �ŋߐړ_�@���_�b
    VECTOR cp = VSub(sPos1, tPos3);
    float dotCACP = VDot(ca, cp);
    float dotCBCP = VDot(cb, cp);
    if (dotCACP <= 0.0f && dotCBCP <= 0.0f)
    {
        return tPos3;
    }

    // �ŋߐړ_�@�ӂ`�a
    VECTOR crossABCA = VCross(ab, ca);
    VECTOR crossAPBP = VCross(ap, bp);
    float dotAP = VDot(crossABCA, crossAPBP);
    if (dotAP >= 0.0f)
    {
        // ���_A�Ɠ_P�̎ˉe���W
        float dotAPAB = VDot(ap, ab);
        float disAB = Distance(tPos1, tPos2);
        return VAdd(tPos1, VScale(VNorm(ab), dotAPAB / disAB));
    }

    // �ŋߐړ_�@�ӂa�b
    VECTOR crossBCAB = VCross(bc, ab);
    VECTOR crossBPCP = VCross(bp, cp);
    float dotBP = VDot(crossBCAB, crossBPCP);
    if (dotBP >= 0.0f)
    {
        // ���_B�Ɠ_P�̎ˉe���W
        float dotBPBC = VDot(bp, bc);
        float disBC = Distance(tPos2, tPos3);
        return VAdd(tPos2, VScale(VNorm(bc), dotBPBC / disBC));
    }

    // �ŋߐړ_�@�ӂb�`
    VECTOR crossCABC = VCross(ca, bc);
    VECTOR crossCPAP = VCross(cp, ap);
    float dotCP = VDot(crossCABC, crossCPAP);
    if (dotCP >= 0.0f)
    {
        // ���_C�Ɠ_P�̎ˉe���W
        float dotCPCA = VDot(cp, ca);
        float disCA = Distance(tPos3, tPos1);
        return VAdd(tPos3, VScale(VNorm(ca), dotCPCA / disCA));
    }

    // �ŋߐړ_�@�ʂ`�a�b
    VECTOR tCrossN = VNorm(crossABCA);
    if (tCrossN.x == -1.0f && tCrossN.y == -1.0f && tCrossN.z == -1.0f)
    {
        // �꒼����ɒ��_�����сA�@���x�N�g�������Ȃ�
        // ���̏ꍇ�́A�R���_�̂����A�ł��߂����_��Ԃ�
        float aPow = SqrMagnitudeF(ap);
        float bPow = SqrMagnitudeF(bp);
        float cPow = SqrMagnitudeF(cp);
        if (aPow <= bPow)
        {
            if (aPow <= cPow) { return tPos1; }
            else { return tPos3; }
        }
        else
        {
            if (bPow <= cPow) { return tPos2; }
            else { return tPos3; }
        }
    }

    // �O�p�`�̖@���ƃx�N�g��AP���g�p���Ėʏ�̎ˉe�𗎂Ƃ�
    float d = VDot(tCrossN, ap);

    // ���̂̒��S���W����ˉe�̒������A�O�p�`�̖@�������Ƀx�N�g����L�΂��ƁA
    // �O�p�`�ʏ�̍ŋߐړ_�����߂���
    //DrawTriangle3D(tPos1, sPos1, VAdd(sPos1, VScale(tCrossN, -d)), 0xff0000, true);
    return VAdd(sPos1, VScale(tCrossN, -d));
}

bool AsoUtility::IsHitSphere(const VECTOR& pos1, float radius1, const VECTOR& pos2)
{// ���̓��m�̏Փ˔���
    bool ret = false;

    // ���݂��̔��a�̍��v
    float radius = radius1;

    // ���W�̍����炨�݂��̋��������
    VECTOR diff = VSub(pos2, pos1);

    // �O�����̒藝�Ŕ�r(SqrMagnitude�Ɠ���)
    float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
    if (dis < (radius * radius))
    {
        ret = true;
    }

    return ret;
}

bool AsoUtility::IsHitSpheres(const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2)
{
    // ���̓��m�̏Փ˔���
    bool ret = false;

    // ���݂��̔��a�̍��v
    float radius = radius1 + radius2;

    // ���W�̍����炨�݂��̋��������
    VECTOR diff = VSub(pos2, pos1);

    // �O�����̒藝�Ŕ�r(SqrMagnitude�Ɠ���)
    float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
    if (dis < (radius * radius))
    {
        ret = true;
    }

    return ret;
}

bool AsoUtility::IsHitTriangleSphere(
    const VECTOR& tPos1, const VECTOR& tPos2, const VECTOR& tPos3, const VECTOR& sPos1, float radius)
{

    // �O�p�`�ʏ�̍ŋߐړ_�����߂�
    VECTOR pos = GetClosestPosTriangle(tPos1, tPos2, tPos3, sPos1);

    // �ŋߐړ_�Ƌ��̂̒��S�_�Ƃ̒���(�Q��)�����߂�
    float disPow = SqrMagnitudeF(VSub(sPos1, pos));

    // ���a�̂Q��Ɣ�r����
    if (disPow <= radius * radius)
    {
        return true;
    }

    return false;
}

AABB AsoUtility::GetAABBTriangle(const VECTOR& tPos1, const VECTOR& tPos2, const VECTOR& tPos3)
{

    AABB ret = AABB();

    ret.min.x = tPos1.x;
    ret.max.x = tPos1.x;
    ret.min.y = tPos1.y;
    ret.max.y = tPos1.y;
    ret.min.z = tPos1.z;
    ret.max.z = tPos1.z;

    if (ret.min.x > tPos2.x) { ret.min.x = tPos2.x; }
    else if (ret.max.x < tPos2.x) { ret.max.x = tPos2.x; }
    if (ret.min.x > tPos3.x) { ret.min.x = tPos3.x; }
    else if (ret.max.x < tPos3.x) { ret.max.x = tPos3.x; }

    if (ret.min.y > tPos2.y) { ret.min.y = tPos2.y; }
    else if (ret.max.y < tPos2.y) { ret.max.y = tPos2.y; }
    if (ret.min.y > tPos3.y) { ret.min.y = tPos3.y; }
    else if (ret.max.y < tPos3.y) { ret.max.y = tPos3.y; }

    if (ret.min.z > tPos2.z) { ret.min.z = tPos2.z; }
    else if (ret.max.z < tPos2.z) { ret.max.z = tPos2.z; }
    if (ret.min.z > tPos3.z) { ret.min.z = tPos3.z; }
    else if (ret.max.z < tPos3.z) { ret.max.z = tPos3.z; }

    return ret;
}

AABB AsoUtility::GetAABBSphere(const VECTOR& sPos1, float radius)
{
    AABB ret = AABB();

    ret.min.x = sPos1.x - radius;
    ret.max.x = sPos1.x + radius;
    ret.min.y = sPos1.y - radius;
    ret.max.y = sPos1.y + radius;
    ret.min.z = sPos1.z - radius;
    ret.max.z = sPos1.z + radius;

    return ret;
}

void AsoUtility::DrawAABB(const AABB& aabb, int color)
{
    // X
    DrawLine3D(
        { aabb.min.x, aabb.min.y, aabb.min.z },
        { aabb.max.x, aabb.min.y, aabb.min.z }, color);
    DrawLine3D(
        { aabb.min.x, aabb.min.y, aabb.max.z },
        { aabb.max.x, aabb.min.y, aabb.max.z }, color);
    DrawLine3D(
        { aabb.min.x, aabb.max.y, aabb.min.z },
        { aabb.max.x, aabb.max.y, aabb.min.z }, color);
    DrawLine3D(
        { aabb.min.x, aabb.max.y, aabb.max.z },
        { aabb.max.x, aabb.max.y, aabb.max.z }, color);

    // Y
    DrawLine3D(
        { aabb.min.x, aabb.min.y, aabb.min.z },
        { aabb.min.x, aabb.max.y, aabb.min.z }, color);
    DrawLine3D(
        { aabb.min.x, aabb.min.y, aabb.max.z },
        { aabb.min.x, aabb.max.y, aabb.max.z }, color);
    DrawLine3D(
        { aabb.max.x, aabb.min.y, aabb.min.z },
        { aabb.max.x, aabb.max.y, aabb.min.z }, color);
    DrawLine3D(
        { aabb.max.x, aabb.min.y, aabb.max.z },
        { aabb.max.x, aabb.max.y, aabb.max.z }, color);

    // Z
    DrawLine3D(
        { aabb.min.x, aabb.min.y, aabb.min.z },
        { aabb.min.x, aabb.min.y, aabb.max.z }, color);
    DrawLine3D(
        { aabb.min.x, aabb.max.y, aabb.min.z },
        { aabb.min.x, aabb.max.y, aabb.max.z }, color);
    DrawLine3D(
        { aabb.max.x, aabb.min.y, aabb.min.z },
        { aabb.max.x, aabb.min.y, aabb.max.z }, color);
    DrawLine3D(
        { aabb.max.x, aabb.max.y, aabb.min.z },
        { aabb.max.x, aabb.max.y, aabb.max.z }, color);
}

bool AsoUtility::Equals(const VECTOR& v1, const VECTOR& v2)
{
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
    {
        return true;
    }
    return false;
}

bool AsoUtility::EqualsVZero(const VECTOR& v1)
{
    const VECTOR& v2 = VECTOR_ZERO;
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
    {
        return true;
    }
    return false;
}

VECTOR AsoUtility::Normalize(const Vector2& v)
{
    VECTOR ret = VGet(
        static_cast<float>(v.x),
        static_cast<float>(v.y),
        0.0f
    );
    float len = static_cast<float>(Magnitude(v));
    ret.x /= len;
    ret.y /= len;
    ret.z /= len;
    return ret;
}

VECTOR AsoUtility::VNormalize(const VECTOR& v)
{
    if (AsoUtility::EqualsVZero(v))
    {
        // Quaternion�v�Z�Ń[����n���āA
        // �G���[(-1, -1, -1)���Ԃ��Ă���ƍ���
        return v;
    }
    return VNorm(v);
}

double AsoUtility::AngleDeg(const VECTOR& from, const VECTOR& to)
{
    // sqrt(a) * sqrt(b) = sqrt(a * b) -- valid for real numbers
    auto fLen = SqrMagnitude(from);
    auto tLen = SqrMagnitude(to);
    auto denominator = sqrt(fLen * tLen);
    if (denominator < kEpsilonNormalSqrt)
    {
        return 0.0f;
    }

    //float dot = std::clamp(Dot(from, to) / denominator, -1.0f, 1.0f);
    //auto dot = Dot(from, to) / denominator;
    auto dot = VDot(from, to) / denominator;
    if (dot < -1.0f)
    {
        dot = -1.0f;
    }
    if (dot > 1.0f)
    {
        dot = 1.0f;
    }

    return acos(dot) * (180.0 / DX_PI);
}

void AsoUtility::DrawLineDir(const VECTOR& pos, const VECTOR& dir, int color, float len)
{
    auto nDir = AsoUtility::VNormalize(dir);
    auto sPos = VAdd(pos, VScale(nDir, -len));
    auto ePos = VAdd(pos, VScale(nDir, len));
    DrawLine3D(sPos, ePos, color);
    DrawSphere3D(ePos, 5.0f, 5, color, color, true);
}

void AsoUtility::DrawLineXYZ(const VECTOR& pos, const Quaternion& rot, float len)
{

    VECTOR dir;

    // X
    dir = rot.GetRight();
    DrawLineDir(pos, dir, 0xff0000, len);

    // Y
    dir = rot.GetUp();
    DrawLineDir(pos, dir, 0x00ff00, len);

    // Z
    dir = rot.GetForward();
    DrawLineDir(pos, dir, 0x0000ff, len);

}


