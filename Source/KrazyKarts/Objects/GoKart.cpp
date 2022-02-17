// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKart.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGoKart::AGoKart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FString GetEnumText(ENetRole Role) {
	switch (Role)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ROLE_Authority:
		return "Authority";
	case ROLE_MAX:
		return "Max";
	default:
		return "Error";
	}
	return "Error";
}

// Called when the game starts or when spawned
void AGoKart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float G = -GetWorld()->GetGravityZ() / 100.f;//M/S^2

	FVector Force(0);
	
	Force.X = Throttle * CarForce;
	Force.Y = 0.f;
	Force.Z = 0.f;

	Force.X -= FMath::Square(ForwardVelocity) * DragCoefficient * FMath::Sign(ForwardVelocity);
	Force.X -= RollingCoefficient * G * Mass * FMath::Sign(ForwardVelocity);

	FHitResult Hit;

	float Acceleration = Force.X/Mass;	//M/S^2
	ForwardVelocity += Acceleration * DeltaTime;	//M/S
	//Velocity -= FVector(Velocity.X * Velocity.X * DragCoefficient * FMath::Sign(Velocity.X), 0.f, 0.f);

	AddActorLocalOffset(FVector::ForwardVector * ForwardVelocity * 100 * DeltaTime, true, &Hit, ETeleportType::None);

	if (Hit.bBlockingHit)
	{
		ForwardVelocity = 0.f;
	}


	FQuat RotationDelta(GetActorUpVector(), ForwardVelocity * SteeringAngle * DeltaTime * Steering);

	AddActorLocalRotation(RotationDelta);

	DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(GetLocalRole()), this, FColor::White, 0.f);
}

// Called to bind functionality to input
void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (PlayerInputComponent != nullptr) 
	{
		PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AGoKart::Server_SetThrottle);
		PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AGoKart::Server_SetSteering);
		PlayerInputComponent->BindAxis(TEXT("LookUp"));
		PlayerInputComponent->BindAxis(TEXT("LookRight"));
	}
}

bool AGoKart::Server_SetThrottle_Validate(float NewThrottle)
{
	return true;
	//Throttle = NewThrottle;
}
void AGoKart::Server_SetThrottle_Implementation(float NewThrottle) 
{
	Throttle = NewThrottle;
}

bool AGoKart::Server_SetSteering_Validate(float Input)
{
	return true;
	//Throttle = NewThrottle;
}
void AGoKart::Server_SetSteering_Implementation(float Input)
{
	Steering = Input;
}

