// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstPrototypeCPPCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Item.h"
#include "InventoryComponent.h"
#include "PlayerStatComponent.h"
#include "Components/SkeletalMeshComponent.h"

//////////////////////////////////////////////////////////////////////////
// AFirstPrototypeCPPCharacter

AFirstPrototypeCPPCharacter::AFirstPrototypeCPPCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Inventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	Inventory->Capacity = 20.f;

	PlayerStatComp = CreateDefaultSubobject<UPlayerStatComponent>("PlayerStatComp");

	IsSprinting = false;

	ShirtMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("ShirtMesh"));
	ShirtMesh->SetupAttachment(GetMesh());
	
	PantsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("PantsMesh"));
	PantsMesh->SetupAttachment(GetMesh());

	/** Set speed **/
	WalkingSpeed = 400.0f;
	RunningSpeed = 800.0f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input
/**
void AFirstPrototypeCPPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ShirtMesh->SetMasterPoseComponet(GetMesh());
	PantsMesh->SetMasterPoseComponet(GetMesh());
}**/

void AFirstPrototypeCPPCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFirstPrototypeCPPCharacter::AttemptJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFirstPrototypeCPPCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFirstPrototypeCPPCharacter::StopSprint);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFirstPrototypeCPPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFirstPrototypeCPPCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFirstPrototypeCPPCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFirstPrototypeCPPCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AFirstPrototypeCPPCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AFirstPrototypeCPPCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AFirstPrototypeCPPCharacter::OnResetVR);
}

void AFirstPrototypeCPPCharacter::UseItem(UItem* Item)
{
	if(Item)
	{
		Item->Use(this);
		Item->OnUse(this);
	}
}

void AFirstPrototypeCPPCharacter::OnResetVR()
{
	// If FirstPrototypeCPP is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in FirstPrototypeCPP.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AFirstPrototypeCPPCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AFirstPrototypeCPPCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AFirstPrototypeCPPCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFirstPrototypeCPPCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFirstPrototypeCPPCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		if (IsSprinting)
		{
			Value = RunningSpeed;
		}
		else
		{
			Value = WalkingSpeed;
		}
		
		AddMovementInput(Direction, Value);
	}
}

void AFirstPrototypeCPPCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		if (IsSprinting)
		{
			Value = RunningSpeed;
		}
		else
		{
			Value = WalkingSpeed;
		}
		
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AFirstPrototypeCPPCharacter::StartSprint()
{
	if (PlayerStatComp->GetEnergy() > 0.0f)
	{
		IsSprinting = true;
		PlayerStatComp->ControlEnergyTimer(true);
	}
	else
	{
		IsSprinting = false;
		PlayerStatComp->ControlEnergyTimer(false);
	}
}

void AFirstPrototypeCPPCharacter::StopSprint()
{
	IsSprinting = false;
	PlayerStatComp->ControlEnergyTimer(false);
}

void AFirstPrototypeCPPCharacter::HandleSprint()
{
	if (IsSprinting)
	{
		PlayerStatComp->DecreaseEnergy(2.0f);
		if (PlayerStatComp->GetEnergy() < 2.0f )
		{
			StopSprint();
		}
	}
}

void AFirstPrototypeCPPCharacter::AttemptJump()
{
	if (PlayerStatComp->GetEnergy() >= 10.0f && !this->GetMovementComponent()->IsFalling())
	{
		PlayerStatComp->DecreaseEnergy(10.0f);
		Jump();
	}
}
