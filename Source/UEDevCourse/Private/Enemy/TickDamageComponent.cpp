#include "Enemy/TickDamageComponent.h"
#include "Components/SphereComponent.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "Components/BoxComponent.h"
#include "Character/StatComponent.h"
#include "Components/CapsuleComponent.h"

UTickDamageComponent::UTickDamageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Damage = 10.0f;								//Default Damage
}



void UTickDamageComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* ParentComp = GetOwner();
	
	UBoxComponent* BoxComponent = ParentComp->GetComponentByClass<UBoxComponent>();
	
	if (!BoxComponent) return;
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&UTickDamageComponent::OverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &UTickDamageComponent::OverlapEnd);
	
	
}

void UTickDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UTickDamageComponent::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FString Name = OtherActor->GetName();
	
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Name);
	
	UStatComponent* OtherActorStatComp = OtherActor->GetComponentByClass<UStatComponent>();
	
	if (!OtherActorStatComp) return;
	
	OtherActorStatComp->TakeDamage(Damage);
	
	UE_LOG(LogTemp, Warning, TEXT("%s Health : %f"), *Name, OtherActorStatComp->GetCurrentHealth());
	
}

void UTickDamageComponent::OverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Exit"));
}
