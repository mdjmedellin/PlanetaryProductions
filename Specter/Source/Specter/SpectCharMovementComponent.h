// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "SpectCharMovementComponent.generated.h"

UENUM(BlueprintType)
enum class EGhostState : uint8
{
	GS_Specter			UMETA(DisplayName = "Specter"),
	GS_Phasing			UMETA(DisplayName = "Phasing"),
	GS_StateCount		UMETA(Hidden),
};

/**
 * 
 */
UCLASS()
class SPECTER_API USpectCharMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	EGhostState m_ghostState;


protected:
	virtual void InitializeComponent() override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMove) override;

	bool SwitchGhostState(EGhostState previousGhostState, EGhostState newGhostState);
	float GetMaxWalkSpeed() const;


public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void StartFalling(int32 Iterations, float remainingTime, float timeTick, const FVector& Delta, const FVector& subLoc) override;
	virtual FVector NewFallVelocity(const FVector& InitialVelocity, const FVector& Gravity, float DeltaTime) const override;
	virtual void PerformMovement(float DeltaTime) override;
	virtual void StartNewPhysics(float deltaTime, int32 Iterations) override;

	USpectCharMovementComponent(const FObjectInitializer& ObjectInitializer);
	bool SetGhostState(EGhostState newGhostState);
	FORCEINLINE EGhostState GetGhostState() { return m_ghostState; }
	
	virtual float GetMaxSpeed() const override;


	UPROPERTY(Category = "Ghost Movement", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float MaxPhaseSpeed;
};
