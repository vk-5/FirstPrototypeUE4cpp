// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstPrototypeCPPGameMode.h"
#include "FirstPrototypeCPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFirstPrototypeCPPGameMode::AFirstPrototypeCPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
