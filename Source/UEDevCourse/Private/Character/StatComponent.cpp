#include "Character/StatComponent.h"

void UStatComponent::TakeDamage(float TakeDamage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - TakeDamage, 0.f, MaxHealth);
	
	if (OnHealthChange.IsBound())
	{
		OnHealthChange.Broadcast(CurrentHealth, MaxHealth);
	}
	
	if (CurrentHealth <= 0)
	{
		OnDeath.Broadcast();
	}
}

void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
	OnHealthChange.Broadcast(CurrentHealth, MaxHealth);
}
