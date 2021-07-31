// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstanceCharacter.h"

UAnimInstanceCharacter::UAnimInstanceCharacter()
{
	MovementSpeed = 0.0f;
}

void UAnimInstanceCharacter::UpdateAnimationProperties()
{
	APawn* Pawn = TryGetPawnOwner();
	
	if (Pawn)
	{
		MovementSpeed = Pawn->GetVelocity().Size();
	}
}