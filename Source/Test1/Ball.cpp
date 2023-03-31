// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABall::ABall()
{
	//Set the foot ball mesh to the root component.
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball mesh"));
	BallMesh->SetupAttachment(RootComponent);
	
}







