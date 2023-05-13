#include "GameEngineMath.h"

// 초기화용 정적객체
const float4 float4::ZERO = {0.0f, 0.0f, 0.0f, 0.0f};

// 방향값 초기화용 정적객체
const float4 float4::LEFT = { -1.0f, 0.0f, 0.0f, 0.0f };
const float4 float4::RIGHT = { 1.0f, 0.0f, 0.0f, 0.0f };
const float4 float4::UP = { 0.0f, -1.0f, 0.0f, 0.0f };
const float4 float4::DOWN = { 0.0f, 1.0f, 0.0f, 0.0f };
