#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
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


// look for attached physics handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s missing phsics handle component"), *GetOwner()->GetName());
	}
}


// look for input component (only appears at run time)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Released);

	} else {
		UE_LOG(LogTemp, Error, TEXT("%s missing Input component"), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle && PhysicsHandle->GrabbedComponent)
	{
		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineTraceEnd());
	}
}


void UGrabber::Grab()
{
	// LINE TRACE and see if we reach any actor with phsics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh in our case
	auto ActorHit = HitResult.GetActor();

	// if we hit something then attach a physics handle
	if (PhysicsHandle && ActorHit)
	{
		/// use GrabComponentAtLocation to allow object to rotate freely
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			{ 0.0f, 0.0f, 0.0f }
		);
	}

}


void UGrabber::Released()
{
	if (PhysicsHandle) PhysicsHandle->ReleaseComponent();
}


FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{

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
	/// Line-trace (AKA ray-cast) out to reach distance
	FHitResult HitResult = {};
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		OUT GetReachLineTraceEndStart(),
		OUT GetReachLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return HitResult;
}


FVector UGrabber::GetReachLineTraceEndStart() const
{
	FVector Location = { 0.0f, 0.0f, 0.0f };
	FRotator Rotation = { 0.0f, 0.0f, 0.0f };

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT Location,
		OUT Rotation);

	return Location;
}


FVector UGrabber::GetReachLineTraceEnd() const
{
	FVector Location = { 0.0f, 0.0f, 0.0f };
	FRotator Rotation = { 0.0f, 0.0f, 0.0f };

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT Location,
		OUT Rotation);

	return (Location + (Rotation.Vector() * Reach));
}