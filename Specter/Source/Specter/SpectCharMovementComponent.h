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

	bool SwitchGhostState(EGhostState previousGhostState, EGhostState newGhostState);
	float GetMaxWalkSpeed() const;


public:
	USpectCharMovementComponent(const FObjectInitializer& ObjectInitializer);
	bool SetGhostState(EGhostState newGhostState);
	FORCEINLINE EGhostState GetGhostState() { return m_ghostState; }
	
	virtual float GetMaxSpeed() const override;


	UPROPERTY(Category = "Ghost Movement", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float MaxPhaseSpeed;
};
