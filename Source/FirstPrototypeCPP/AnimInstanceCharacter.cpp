// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstanceCharacter.h"

UAnimInstanceCharacter::UAnimInstanceCharacter()
{
	Super:UAnimInstance();

	this->MovementSpeed = 800.0f;
}

void UAnimInstanceCharacter::UpdateAnimationProperties()
{
	APawn* Pawn = TryGetPawnOwner();
	
	if(Pawn)
	{
		MovementSpeed = Pawn->GetVelocity().Size();
	}
}