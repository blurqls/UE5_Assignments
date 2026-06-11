// Copyright Epic Games, Inc. All Rights Reserved.

#include "UEDevCourseGameMode.h"
#include "UEDevCourseCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUEDevCourseGameMode::AUEDevCourseGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
