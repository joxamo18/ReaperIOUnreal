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
	if (fireTimerCooldown > 0)
	{
		fireTimerCooldown -= DeltaTime;
		if (fireTimerCooldown <= 0)
		{
			fireTimerCooldown = 0;
			CanFire = true;
		}
	}
}

// Called to bind functionality to input
void AFlyingUfo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFlyingUfo::RemoveHeatlh(float HealthToRemove)
{
	UFOHealth -= HealthToRemove;
	if (UFOHealth <= 0)
	{
		Destroy();
	}
}

void AFlyingUfo::FireShotUFO()
{

	float range = 300.0f; 
	float offset = 0.0f;
	float currentOffset = 0.0f;
	if (CanFire)
	{

		currentOffset = range / 2;
		offset = range / static_cast<float>(ProjectileAmount);
		
		for (int i = 0; i < ProjectileAmount; i++)
		{
			const FVector actorLocation = this->GetActorLocation();
			FVector newOffset(0, currentOffset, 0);
			FVector XDisplacement = actorLocation + newOffset;
			FVector YDisplacement = this->GetActorUpVector() * -100.0f;
			const FVector SpawnLocation = XDisplacement + YDisplacement;
			const FRotator SpawnRotation(0, 0, 0);
			FActorSpawnParameters params;
			params.Instigator = this;
			//(T::StaticClass(), NAME_None, NULL, NULL, NULL, bNoCollisionFail, false, Owner, Instigator)
			GetWorld()->SpawnActor(Projectile, &SpawnLocation, &SpawnRotation, params);

			if (Projectile->StaticClass())
			{
				FString display = Projectile->GetDisplayNameText().ToString();
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, display);

			}
			currentOffset = currentOffset - offset;
		}
			CanFire = false;
			fireTimerCooldown = fireRate;
	}
}

