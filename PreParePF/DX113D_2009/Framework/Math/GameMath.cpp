#include "Framework.h"
#include "GameMath.h"

float GameMath::Saturate(const float& value)
{
    return max(0.0f, min(1.0f, value));
}

int GameMath::Random(int min, int max)
{
    return rand() % (max - min) + min;
}

float GameMath::Random(float min, float max)
{
    float normal = rand() / (float)RAND_MAX;
    return min + (max - min) * normal;
}

float GameMath::Distance(const Vector3& v1, const Vector3& v2)
{
    return (v1 - v2).Length();
}

Vector3 GameMath::ClosestPointOnLineSegment(const Vector3& v1, const Vector3& v2, const Vector3& point)
{
    Vector3 line = v2 - v1;
    float t = Vector3::Dot(line, point - v1) / Vector3::Dot(line, line);
    t = Saturate(t);

    return v1 + line * t;
}

Vector3 GameMath::WorldToScreen(const Vector3& worldPos)
{
    Vector3 screenPos;

    screenPos = XMVector3TransformCoord(worldPos.data, CAMERA->GetView());
    screenPos = XMVector3TransformCoord(screenPos.data, Environment::Get()->GetProjection());
    //NDC°ø°£ ÁÂÇ¥(-1 ~ 1) -> ½ºÅ©¸° ÁÂÇ¥(0 ~ screenSize)

    screenPos.y *= -1.0f;

    screenPos = (screenPos + 1.0f) * 0.5f;//(0 ~ 1)

    screenPos.x *= WIN_WIDTH;
    screenPos.y *= WIN_HEIGHT;

    return screenPos;
}
