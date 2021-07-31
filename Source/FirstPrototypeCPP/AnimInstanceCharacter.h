// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROTOTYPECPP_API UAnimInstanceCharacter : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MovementSpeed;

	UFUNCTION(BlueprintCallable, Category="Animations")
	virtual void UpdateAnimationProperties();
};
