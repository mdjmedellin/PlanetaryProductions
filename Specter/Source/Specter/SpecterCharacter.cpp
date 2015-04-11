// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Specter.h"
#include "SpecterCharacter.h"
#include "SpectCharMovementComponent.h"

ASpecterCharacter::ASpecterCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USpectCharMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->RelativeRotation = FRotator(0.f,180.f,0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Create the phase particle and attach it to the root
	PhaseParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PhaseParticle"));
	PhaseParticle->AttachTo(RootComponent);
	PhaseParticle->bAutoActivate = false;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASpecterCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAction("Phase", IE_Pressed, this, &ASpecterCharacter::Phase);
	InputComponent->BindAction("Phase", IE_Released, this, &ASpecterCharacter::StopPhasing);
	InputComponent->BindAxis("MoveRight", this, &ASpecterCharacter::MoveRight);

	InputComponent->BindTouch(IE_Pressed, this, &ASpecterCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ASpecterCharacter::TouchStopped);
}

void ASpecterCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

void ASpecterCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ASpecterCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

//JM
void ASpecterCharacter::SetPhasingState(bool active)
{
	USkeletalMeshComponent* mesh = GetMesh();
	if (active)
	{
		mesh->SetVisibility(false);
		PhaseParticle->SetActive(true);
	}
	else
	{
		mesh->SetVisibility(true);
		PhaseParticle->SetActive(false);
	}
}

void ASpecterCharacter::Phase()
{
	USpectCharMovementComponent* movementComponent = Cast<USpectCharMovementComponent>(GetCharacterMovement());

	if (movementComponent
		&& movementComponent->SetGhostState(EGhostState::GS_Phasing))
	{
		SetPhasingState(true);
	}
}

void ASpecterCharacter::StopPhasing()
{
	USpectCharMovementComponent* movementComponent = Cast<USpectCharMovementComponent>(GetCharacterMovement());

	if (movementComponent
		&& movementComponent->SetGhostState(EGhostState::GS_Specter))
	{
		SetPhasingState(false);
	}
}