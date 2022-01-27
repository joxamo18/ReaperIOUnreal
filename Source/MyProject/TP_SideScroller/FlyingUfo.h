// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FlyingUfo.generated.h"

UCLASS()
class MYPROJECT_API AFlyingUfo : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFlyingUfo();

private:
	bool CanFire = true;
	float fireRate = 0.5f;
	float fireTimerCooldown = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		float UFOHealth;

	UPROPERTY(EditAnywhere)
		int ProjectileAmount;


	UFUNCTION(BlueprintCallable)
		void RemoveHeatlh(float HealthToRemove);

	UFUNCTION(BlueprintCallable)
		void FireShotUFO();

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AActor> Projectile;
};
