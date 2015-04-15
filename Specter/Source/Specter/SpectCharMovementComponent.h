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

	bool m_originalGravityScaleIsSaved;
	float m_originalGravityScale;

protected:
	virtual void InitializeComponent() override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMove) override;

	virtual bool CanSetSpecifiedGhostState(EMovementMode currentMovementMode, EGhostState currentGhostState
		, EGhostState desiredGhostState) const;
	virtual void SetGhostSubMovementMode(EMovementMode previousMovementMode, EGhostState previousGhostState);
	virtual void HandleTransitionFromPhasingState();
	virtual void HandleTransitionFromSpecterState();

	bool SwitchGhostState(EGhostState previousGhostState, EGhostState newGhostState);
	float GetMaxWalkSpeed() const;


public:
	virtual void SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode = 0) override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void StartFalling(int32 Iterations, float remainingTime, float timeTick, const FVector& Delta, const FVector& subLoc) override;
	virtual FVector NewFallVelocity(const FVector& InitialVelocity, const FVector& Gravity, float DeltaTime) const override;
	virtual void PerformMovement(float DeltaTime) override;
	virtual void StartNewPhysics(float deltaTime, int32 Iterations) override;

	USpectCharMovementComponent(const FObjectInitializer& ObjectInitializer);
	bool CanSetSpecifiedGhostState(EGhostState desiredGhostState) const;
	bool SetGhostState(EGhostState desiredGhostState);
	FORCEINLINE EGhostState GetGhostState() { return static_cast<EGhostState>(CustomMovementMode); }
	
	virtual float GetMaxSpeed() const override;


	UPROPERTY(Category = "Ghost Movement", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float MaxPhaseSpeed;
};
