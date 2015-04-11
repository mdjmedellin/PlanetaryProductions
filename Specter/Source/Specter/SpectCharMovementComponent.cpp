// Fill out your copyright notice in the Description page of Project Settings.

#include "Specter.h"
#include "SpectCharMovementComponent.h"


USpectCharMovementComponent::USpectCharMovementComponent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	m_ghostState = EGhostState::GS_StateCount;
	MaxPhaseSpeed = 1200.f;
}

void USpectCharMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	m_ghostState = EGhostState::GS_Specter;
}

/*
/	Attempts to switch the ghost state from previousGhostState to newGhostState
/	@return boolean describing wether there is a viable transition between states
*/
bool USpectCharMovementComponent::SwitchGhostState(EGhostState previousGhostState, EGhostState newGhostState)
{
	bool transitionIsValid = false;
	m_ghostState = newGhostState;

	switch (previousGhostState)
	{
	case EGhostState::GS_Specter:
		//we were previously a specter and are switching to another state
		if (newGhostState == EGhostState::GS_Phasing)
		{
			//we want to turn off collision and increment the speed of travel
			transitionIsValid = true;
		}
		break;

	case EGhostState::GS_Phasing:
		//we were previously phasing and are now switching to another state
		if (newGhostState == EGhostState::GS_Specter)
		{
			//we want to turn on collision and decrease the speed of travel
			transitionIsValid = true;
		}
		break;

	default:
		break;
	}

	return transitionIsValid;
}

float USpectCharMovementComponent::GetMaxWalkSpeed() const
{
	if (IsCrouching())
	{
		return MaxWalkSpeedCrouched;
	}
	else
	{
		//check our ghost state
		switch (m_ghostState)
		{
		case EGhostState::GS_Specter:
			return MaxWalkSpeed;
			break;

		case EGhostState::GS_Phasing:
			return MaxPhaseSpeed;
			break;

		default:
			return MaxWalkSpeed;
		}
	}
}

/*
/	Function sets the new ghost state of the character
/	If the character is already in the desired state, or it cannot go into the desired state
/	the function will return FALSE
/	Otherwise, the function will return TRUE
*/
bool USpectCharMovementComponent::SetGhostState(EGhostState newGhostState)
{
	//check that we are not currently in the new ghost state and that it is valid
	if (m_ghostState != newGhostState
		&& newGhostState != EGhostState::GS_StateCount)
	{
		return SwitchGhostState(m_ghostState, newGhostState);
	}
	else
	{
		return false;
	}
}

float USpectCharMovementComponent::GetMaxSpeed() const
{
	switch (MovementMode)
	{
	case MOVE_Walking:
		return GetMaxWalkSpeed();
	case MOVE_Falling:
		return MaxWalkSpeed;
	case MOVE_Swimming:
		return MaxSwimSpeed;
	case MOVE_Flying:
		return MaxFlySpeed;
	case MOVE_Custom:
		return MaxCustomMovementSpeed;
	case MOVE_None:
	default:
		return 0.f;
	}
}