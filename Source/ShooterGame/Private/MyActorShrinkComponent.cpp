// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Actor.h"
#include "ShooterGame.h"
#include "MyActorShrinkComponent.h"

UMyActorShrinkComponent::UMyActorShrinkComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	fTotalShrinkTime = fResidualShrinkTime = 0;
	bIsShrinked = false;
	bIsSizeRescaling = false;
	fTotalSizeRescalingTime = 5;
	fSizeRescalingTime = 0;
	FSmallSize = FVector(0.2, 0.2, 0.2);
	
	// ...
}


void UMyActorShrinkComponent::BeginPlay()
{
	Super::BeginPlay();

	MyCharacter = Cast<AMy_ShooterCharacter>(GetOwner());
	StartScale = MyCharacter->GetActorScale3D();

	fStartBaseHeight = MyCharacter->BaseEyeHeight;
	fShrinkedBaseHeight = fStartBaseHeight * 0.2f;
}


void UMyActorShrinkComponent::Shrink(bool hasToShrink, bool hasToSkipShrinkInterpolation)
{
	bIsShrinked = hasToShrink;
	if (!hasToSkipShrinkInterpolation)
	{
		bIsSizeRescaling = true;
		fSizeRescalingTime = 0;
		
	}
	else 
	{
		// setting the scale with lerping parameter equals to one is like 
		// skipping the interpolation
		SetScale(fSKIP_INTERPOLATION);
	}

}

float UMyActorShrinkComponent::GetResidualShrinkTime()
{
	return fResidualShrinkTime;
}

float UMyActorShrinkComponent::GetTotalShrinkTime()
{
	return fTotalShrinkTime;
}

void UMyActorShrinkComponent::SetTime(float fFullTime)
{
	fTotalShrinkTime = fResidualShrinkTime = fFullTime;

}


void UMyActorShrinkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsSizeRescaling && fSizeRescalingTime < 1)
	{
		ScaleCharacterWithInterpolation(DeltaTime);
		return;
	}

	DecreaseShrinkTimeAndAct(DeltaTime);
}

void UMyActorShrinkComponent::ScaleCharacterWithInterpolation(float DeltaTime)
{
	
	fSizeRescalingTime = FMath::Clamp<float>(fSizeRescalingTime + DeltaTime / fTotalSizeRescalingTime, 0.0f, 1);
	
	SetScale(fSizeRescalingTime);

	if (fSizeRescalingTime == 1)
	{
		bIsSizeRescaling = false;
	}
}

/// <summary>
/// Sets the scale of the character looking at bIsShrinked value.
/// </summary>
/// <param name="fLerpingParameter">Set to 1 to change the scale istantly</param>
void UMyActorShrinkComponent::SetScale(float fLerpingParameter)
{
	FVector CurrentScale;
	float TargetBaseHeight;
	if (bIsShrinked)
	{
		CurrentScale = FMath::Lerp(StartScale, FSmallSize, fLerpingParameter);
		TargetBaseHeight = FMath::Lerp(fStartBaseHeight, fShrinkedBaseHeight, fLerpingParameter);
	}
	else
	{
		CurrentScale = FMath::Lerp(FSmallSize, StartScale, fLerpingParameter);
		TargetBaseHeight = FMath::Lerp(fShrinkedBaseHeight, fStartBaseHeight, fLerpingParameter);

	}

	MyCharacter->SetActorScale3D(CurrentScale);
	MyCharacter->BaseEyeHeight = TargetBaseHeight;
}


void UMyActorShrinkComponent::DecreaseShrinkTimeAndAct(float DeltaTime)
{
	if (bIsShrinked && fResidualShrinkTime != 0)
	{
		fResidualShrinkTime = FMath::Clamp<float>(fResidualShrinkTime - DeltaTime / fTotalShrinkTime, 0.0f, fResidualShrinkTime);
	}
	else if (bIsShrinked && fResidualShrinkTime == 0)
	{
		if (MyCharacter->HasAuthority())
		{
			MyCharacter->ServerShrink(false);
		}
	}
}

