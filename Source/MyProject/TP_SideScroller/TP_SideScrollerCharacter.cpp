// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_SideScrollerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

float getSignFloat(float value)
{
	if (value > 0)
	{
		return 1;
	}
	if (value < 0)
	{
		return -1;
	}
	return 0;
}

ATP_SideScrollerCharacter::ATP_SideScrollerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 1.0f;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
	GetCharacterMovement()->FallingLateralFriction = 1000.0f;
	this->JumpMaxCount = 2;
	this->JumpMaxHoldTime = 0.0f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATP_SideScrollerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATP_SideScrollerCharacter::PlayerJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ATP_SideScrollerCharacter::PlayerStopJumping);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ATP_SideScrollerCharacter::DashPressed);

	PlayerInputComponent->BindAxis("MoveRight", this, &ATP_SideScrollerCharacter::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ATP_SideScrollerCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ATP_SideScrollerCharacter::TouchStopped);
}

void ATP_SideScrollerCharacter::MoveRight(float Value)
{
	// add movement in that direction
	MoveRightValue = Value;
	PlayerDirection = getSignFloat(Value);
	
	if (DashTimer <= 0) {
		AddMovementInput(FVector(0.f,-1.f,0.f), Value);
	}
}

void ATP_SideScrollerCharacter::PlayerJump()
{
	Jump();
}

void ATP_SideScrollerCharacter::PlayerStopJumping()
{
	StopJumping();
	FVector currentVelocity = GetCharacterMovement()->Velocity;
	if (currentVelocity.Z > 0)
	{
		GetCharacterMovement()->Velocity.Set(currentVelocity.X, currentVelocity.Y, 0.0f);
	}

}

void ATP_SideScrollerCharacter::DashPressed()
{
	DashTimer = MaxDashTimeDuration;
	DashDirection = MoveRightValue;
}



void ATP_SideScrollerCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ATP_SideScrollerCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void ATP_SideScrollerCharacter::Tick(float DeltaSeconds) {
	TickDeltaSeconds = DeltaSeconds;
	DashTick();
}

void ATP_SideScrollerCharacter::DashTick()
{
	if (DashTimer > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("This message will appear on the screen!"));

		DashTimer = DashTimer - TickDeltaSeconds;

		if (DashTimer <= 0)
		{
			//reset everything
			GetCharacterMovement()->FallingLateralFriction = MaxFallingLateralFriction;
			GetCharacterMovement()->Velocity.Set(0.0f, 0.0f, 0.0f);
			DashDirection = 0.0f;
			DashTimer = 0.0f;
		}
		else
		{
			if (DashDirection == 0) {
				GetCharacterMovement()->Velocity.Set(0.0f,  ((TickDeltaSeconds * DashSpeed) * (getSignFloat(GetActorForwardVector().Y))), 0.0f);
			}
			else
			{
				GetCharacterMovement()->Velocity.Set(0.0f, ((TickDeltaSeconds * DashSpeed) * (-1 * DashDirection)), 0.0f);
			}
		}
	}
}

