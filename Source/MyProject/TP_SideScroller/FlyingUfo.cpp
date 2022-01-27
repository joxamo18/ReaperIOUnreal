// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingUfo.h"

// Sets default values
AFlyingUfo::AFlyingUfo()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFlyingUfo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlyingUfo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFlyingUfo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFlyingUfo::RemoveHeatlh(float HealthToRemove)
{
	UFOHealth -= HealthToRemove;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Removing Health"));
	if (UFOHealth <= 0)
	{
		Destroy();
	}
}


