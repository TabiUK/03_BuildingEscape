// Copyright Nicholas Panayis 2018

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector out_Location = { 0.0f, 0.0f, 0.0f };
	FRotator out_Rotation = { 0.0f, 0.0f, 0.0f };

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT out_Location, 
		OUT out_Rotation);


	FVector LineTraceEnd = out_Location + (out_Rotation.Vector() * Reach);



	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), 
	//	*out_Location.ToString(),
	//	*out_Rotation.ToString());

	DrawDebugLine(
		GetWorld(),
		out_Location,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.0f,
		0,
		10.0f);
}

