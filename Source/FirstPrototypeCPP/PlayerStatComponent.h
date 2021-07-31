// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FIRSTPROTOTYPECPP_API UPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerStatComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "PlayerStats")
	float MaximumHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "PlayerStats")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "PlayerStats")
	float MaximumEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "PlayerStats")
	float Energy;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "PlayerStats")
	float EnergyRecoverValue;

	FTimerHandle EnergyTimerHandle;

	// Called when the game starts
	virtual void BeginPlay() override;

	void HandleEnergy();

public:
	void IncreaseHealth(float Value);
	void DecreaseHealth(float Value);
	void IncreaseEnergy(float Value);
	void DecreaseEnergy(float Value);
	float GetHealth();
	float GetEnergy();
	void ControlEnergyTimer(bool Stop);
};
