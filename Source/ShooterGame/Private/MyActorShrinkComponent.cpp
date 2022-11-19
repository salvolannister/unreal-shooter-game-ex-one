// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/Actor.h"
#include "ShooterGame.h"
#include "MyActorShrinkComponent.h"

// Sets default values for this component's properties
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


// Called when the game starts
void UMyActorShrinkComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	// scale the character 
	MyCharacter = Cast<AMy_ShooterCharacter>(GetOwner());
	MyCapsuleComponent = MyCharacter->GetCapsuleComponent();
	StartScale = MyCharacter->GetCapsuleComponent()->GetRelativeScale3D();


	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Actor scale is " + StartScale.ToString());

}


void UMyActorShrinkComponent::Shrink(bool hasToShrink)
{
	bIsShrinked = hasToShrink;
	bIsSizeRescaling = true;
	fSizeRescalingTime = 0;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Actor" + MyCharacter->GetName() + "scale is " + MyCharacter->GetActorScale3D().ToString());

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




// Called every frame
void UMyActorShrinkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsSizeRescaling && fSizeRescalingTime  < 1)
	{
		ScaleCharacter(DeltaTime);
		return;
	}

	DecreaseShrinkTimeAndAct(DeltaTime);
}

void UMyActorShrinkComponent::ScaleCharacter(float DeltaTime)
{
	FVector CurrentScale;
	fSizeRescalingTime = FMath::Clamp<float>(fSizeRescalingTime + DeltaTime / fTotalSizeRescalingTime, 0.0f, 1);
	if (bIsShrinked)
	{
		CurrentScale = FMath::Lerp(StartScale, FSmallSize, fSizeRescalingTime);

	}
	else
	{
		CurrentScale = FMath::Lerp(FSmallSize, StartScale, fSizeRescalingTime);

	}


	GEngine->AddOnScreenDebugMessage(-1, 15.0f,
		FColor::Red,
		"while current scale is " + FString::SanitizeFloat(CurrentScale.X));

	MyCapsuleComponent->SetWorldScale3D(CurrentScale);
	MyCharacter->SetActorScale3D(CurrentScale);

	if (fSizeRescalingTime == 1)
	{
		bIsSizeRescaling = false;

	}
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

