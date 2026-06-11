// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FeatureCharacter.h"
#include "EnhancedInputComponent.h"
#include "Character/FeatureCharacterController.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/StatComponent.h"
#include "Animation/FeatureAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AFeatureCharacter::AFeatureCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 300.0f;
	SpringArmComponent->bUsePawnControlRotation = true;
	
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));

}
// Called when the game starts or when spawned
void AFeatureCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	IsAttacking = false;
	IsDead = false;
	
	
	
	AnimInstance = Cast<UFeatureAnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInstance)
	{
		AnimInstance->OnEndAttack.AddUObject(this, &AFeatureCharacter::AttackEnd);
	}
	
	StatComp->OnHealthChange.AddDynamic(this, &AFeatureCharacter::HealthLog);
	StatComp->OnDeath.AddDynamic(this,&AFeatureCharacter::HandleDeath);
	
}

// Called every frame
void AFeatureCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFeatureCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AFeatureCharacterController* CharaController = Cast<AFeatureCharacterController>(GetController()))
		{
			if (CharaController)
			{
				EnhancedInput->BindAction(
					CharaController->InputMove,
					ETriggerEvent::Triggered,
					this,
					&AFeatureCharacter::CharacterMove);
				
				EnhancedInput->BindAction(
					CharaController->InputLook,
					ETriggerEvent::Triggered,
					this,
					&AFeatureCharacter::CharacterLook);
				
				EnhancedInput->BindAction(
					CharaController->InputAttack,
					ETriggerEvent::Started,
					this,
					&AFeatureCharacter::CharacterAttack);
			}
		}
	}

}

void AFeatureCharacter::CharacterMove(const FInputActionValue& InputAction)
{
	
	FVector2D MovementVector = InputAction.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		if (!IsDead)
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.X);
			AddMovementInput(RightDirection, MovementVector.Y);
		}
		
	}
	
	
	
}

void AFeatureCharacter::CharacterLook(const FInputActionValue& InputAction)
{
	
	// input is a Vector2D
	FVector2D LookAxisVector = InputAction.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFeatureCharacter::CharacterAttack(const FInputActionValue& InputAction)
{
	UE_LOG(LogTemp, Warning, TEXT("Attack"));
	if (AttackMontage)
	{
		if (!IsAttacking && !IsDead)
		{
			IsAttacking = true;
			PlayAnimMontage(AttackMontage);
		}
		
	}
}

void AFeatureCharacter::HealthLog(float CurrentHealth, float MaxHealth)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.0f,
			FColor::Black,
			FString::Printf(TEXT("%f"), CurrentHealth)
		);
	}
}

void AFeatureCharacter::HandleDeath()
{
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	
	IsDead = true;
	
}

void AFeatureCharacter::AttackEnd()
{
	IsAttacking = false;
}
