// Copyright Nicholas Panayis 2018

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	if (Owner == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Owner found"));
		return;
	}

	if (PressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No PressurePlate on object %s"), *Owner->GetName());
	}
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume every frame
   // If the ActorThatOpens is in the volume 
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) // TODO
	{
		// OpenDoor using BluePrint
		OnOpen.Broadcast();
	} else {
		// CloseDoor Using BluePrint
		OnClose.Broadcast();
	}
}


float UOpenDoor::GetTotalMassOfActorsOnPlate() const
{
	float TotalMass = 0.0f;
	if (PressurePlate == nullptr) return TotalMass;

	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors = {};
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (const auto* Actor : OverlappingActors)
	{
		if (Actor == nullptr) continue;
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("actor Object %s"), *Actor->GetName());
	}

	return TotalMass;
}
