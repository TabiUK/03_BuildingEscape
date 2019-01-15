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
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();

}


// look for input component (only appears at run time)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("%s Have Input component"), *InputComponent->GetName());
		// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Released);

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing Input component"), *GetOwner()->GetName());
	}
}


// look for attached physics handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		PhysicsHandle = nullptr;
		UE_LOG(LogTemp, Error, TEXT("%s missing phsics handle component"), *GetOwner()->GetName());
		return;
	}
}


void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("grabbed pressed"));
	// TODO attach physic handle
	GetFirstPhysicsBodyInReach();

}


void UGrabber::Released()
{
	UE_LOG(LogTemp, Warning, TEXT("grabbed Released"));
	// TODO release physic handle
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}


FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	UE_LOG(LogTemp, Warning, TEXT("GetFirstPhysicsBodyInReach called"));
	FVector out_Location = { 0.0f, 0.0f, 0.0f };
	FRotator out_Rotation = { 0.0f, 0.0f, 0.0f };

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT out_Location,
		OUT out_Rotation);


	FVector LineTraceEnd = out_Location + (out_Rotation.Vector() * Reach);
	/* debug line trace
	{
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
	*/
	/// Line-trace out to reach distance
	FHitResult Hit = {};
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		OUT out_Location,
		OUT LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	if (Hit.GetActor())	UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *(Hit.GetActor()->GetName()));

	return  Hit;
}

