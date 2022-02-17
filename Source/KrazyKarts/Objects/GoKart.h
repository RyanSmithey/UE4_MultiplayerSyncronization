// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

UCLASS()
class KRAZYKARTS_API AGoKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGoKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere) float CarForce = 10000.f;	//CarAcceleration force
	UPROPERTY(EditAnywhere) float SteeringAngle = 0.1f; //Rad/meter
	UPROPERTY(EditAnywhere) float Mass = 1000.f;		//Car Mass KG
	UPROPERTY(EditAnywhere) float DragCoefficient = 16.f;
	UPROPERTY(EditAnywhere) float RollingCoefficient = 0.01f;


	UFUNCTION(Server, Reliable, WithValidation) void Server_SetThrottle(float NewThrottle);
	UFUNCTION(Server, Reliable, WithValidation) void Server_SetSteering(float Input);
	float ForwardVelocity; //M/S

	float Throttle = 0.f;	//-1 - 1
	float Steering = 0.f;	//-1 - 1
};
