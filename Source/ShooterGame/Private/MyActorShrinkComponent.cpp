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


void UMyActorShrinkComponent::Shrink(bool shrink) 
{
	if (shrink) 
	{
		MyCapsuleComponent->SetWorldScale3D(FVector(0.2, 0.2, 0.2));
		MyCharacter->SetActorScale3D(FVector(0.2, 0.2, 0.2));
		
	}
	else 
	{
		MyCapsuleComponent->SetWorldScale3D(StartScale);
		MyCharacter->SetActorScale3D(StartScale);
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Actor" + MyCharacter->GetName() + "scale is " + MyCharacter->GetActorScale3D().ToString());

}




// Called every frame
void UMyActorShrinkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

