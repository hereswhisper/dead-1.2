#pragma once
#include "SDK.hpp"

using namespace SDK;

namespace Globals {
	UFortEngine* Engine;
	UWorld* World;

	UGameplayStatics* GameplayStatics;

	APlayerController* LocalController;
	AFortPlayerController* GameController;

	APlayerPawn_Generic_C* Pawn;
	AFortGameStateZone* GameState;

	void* (*ProcessEvent)(UObject*, UFunction*, void*);

	static std::string map = "";

	static auto BaseTransform = FTransform{};
}