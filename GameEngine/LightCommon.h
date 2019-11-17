#pragma once
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionLight.h"

enum class LightType
{
	DirectionLightType = 0,
	PointLightType,
	SpotLightType,

	MaxType
};