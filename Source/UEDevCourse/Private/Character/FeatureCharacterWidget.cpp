// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FeatureCharacterWidget.h"
#include "Character/FeatureCharacter.h"
#include "Character/StatComponent.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "Character/StatComponent.h"

void UFeatureCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	AFeatureCharacter* FeatureCharacter = Cast<AFeatureCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	UStatComponent* CurrentStat = FeatureCharacter->GetStatComponent();
	
	if (!FeatureCharacter)
	{
		return;
	}
	CurrentStat->OnHealthChange.AddDynamic(this,&UFeatureCharacterWidget::UpdateProgressBar);
	
	UpdateProgressBar(CurrentStat->GetCurrentHealth(), CurrentStat->GetMaxHealth());
	
}

void UFeatureCharacterWidget::UpdateProgressBar(float CurrentHealth, float MaxHealth)
{
	ProgressBar->SetPercent(CurrentHealth/MaxHealth);
}
