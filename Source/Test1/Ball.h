// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
UCLASS()
class TEST1_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABall();

private:
	//Mesh for the foot ball.
	UPROPERTY(VisibleAnywhere, Category="BallComponent")
	UStaticMeshComponent* BallMesh;
};
