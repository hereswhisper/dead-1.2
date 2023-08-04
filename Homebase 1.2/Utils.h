#pragma once
#include <Windows.h>
#include "SDK.hpp"
#include "Globals.h"
#include <detours.h>
#include <fstream>
#include <format>
using namespace SDK;

namespace Utils {
	bool IsNull(void* In)
	{
		if (In == nullptr)
			return true;

		return false;
	}

	template<typename T = AActor>
	T* SpawnActor(UClass* Class = nullptr, FVector Location = {}, FRotator Rotation = {})
	{
		if (IsNull(Class))
			Class = T::StaticClass();

		FQuat Quat;
		FTransform Transform;
		Quat.W = 0;
		Quat.X = Rotation.Pitch;
		Quat.Y = Rotation.Roll;
		Quat.Z = Rotation.Yaw;
		Transform.Rotation = Quat;
		Transform.Scale3D = FVector{ 1,1,1 };
		Transform.Translation = Location;

		auto GS = Globals::GameplayStatics;
		auto Actor = GS->BeginDeferredActorSpawnFromClass(Globals::World, Class, Transform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, nullptr);
		GS->FinishSpawningActor(Actor, Transform);
		return static_cast<T*>(Actor);
	}
}