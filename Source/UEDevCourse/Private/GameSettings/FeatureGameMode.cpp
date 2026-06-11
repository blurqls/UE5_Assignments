// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettings/FeatureGameMode.h"
#include "Character/FeatureCharacterController.h"
#include "Character/FeatureCharacter.h"

AFeatureGameMode::AFeatureGameMode()
{
	DefaultPawnClass = AFeatureCharacter::StaticClass();
	PlayerControllerClass = AFeatureCharacterController::StaticClass();
	
}
