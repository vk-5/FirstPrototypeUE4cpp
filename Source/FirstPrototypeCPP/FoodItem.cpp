// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodItem.h"

#include "FirstPrototypeCPPCharacter.h"
#include "PlayerStatComponent.h"

void UFoodItem::Use(AFirstPrototypeCPPCharacter* Character)
{
	if (Character)
	{
		Character->PlayerStatComp->IncreaseHealth(HealthToHeal);
	}
}
