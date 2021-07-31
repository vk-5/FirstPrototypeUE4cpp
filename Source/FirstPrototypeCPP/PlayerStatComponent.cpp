// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatComponent.h"

// Sets default values for this component's properties
UPlayerStatComponent::UPlayerStatComponent()
{
	MaximumHealth = 100.0f;
	Health = MaximumHealth;

	MaximumEnergy = 100.0f;
	Energy = MaximumEnergy;
	EnergyRecoverValue = 1.0f;
}


// Called when the game starts
void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(EnergyTimerHandle, this, &UPlayerStatComponent::HandleEnergy, 1.0f, true);
}

void UPlayerStatComponent::HandleEnergy()
{
	IncreaseEnergy(EnergyRecoverValue);
}

void UPlayerStatComponent::IncreaseHealth(float Value)
{
	if (Health + Value > MaximumHealth)
	{
		Health = MaximumHealth;
	}
	else
	{
		Health += Value;
	}
}

void UPlayerStatComponent::DecreaseHealth(float Value)
{
	if (Health - Value < 0.0f)
	{
		Health = 0.0f;
	}
	else
	{
		Health -= Value;
	}
}

void UPlayerStatComponent::IncreaseEnergy(float Value)
{
	if (Energy + Value > MaximumEnergy)
	{
		Energy = MaximumEnergy;
	}
	else
	{
		Energy += Value;
	}
}

void UPlayerStatComponent::DecreaseEnergy(float Value)
{
	if (Energy - Value < 0.0f)
	{
		Energy = 0.0f;
	}
	else
	{
		Energy -= Value;
	}
}

float UPlayerStatComponent::GetHealth()
{
	return Health;
}

float UPlayerStatComponent::GetEnergy()
{
	return Energy;
}

void UPlayerStatComponent::ControlEnergyTimer(bool Stop)
{
	if (Stop)
	{
		GetWorld()->GetTimerManager().PauseTimer(EnergyTimerHandle);
	}
	else
	{
		GetWorld()->GetTimerManager().UnPauseTimer(EnergyTimerHandle);
	}
}
