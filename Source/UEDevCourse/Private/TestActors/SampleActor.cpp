#include "TestActors/SampleActor.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ASampleActor::ASampleActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComponent=  CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(SceneComponent);
	
	StaticMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneComponent);
	
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	Collider->SetupAttachment(SceneComponent);
	

}

void ASampleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASampleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


