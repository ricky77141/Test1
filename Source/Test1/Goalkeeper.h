// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Goalkeeper.generated.h"

UCLASS()
class TEST1_API AGoalkeeper : public ACharacter
{
	GENERATED_BODY()
private:
	bool SelectSide();

public:
	// Sets default values for this pawn's properties
	AGoalkeeper();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goalkeeper Movement")
		float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goalkeeper Movement")
		float MinX = -500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goalkeeper Movement")
		float MaxX = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goalkeeper Movement")
		float MinY = -500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goalkeeper Movement")
		float MaxY = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goalkeeper Movement")
		float SpawnX = 1360.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goalkeeper Movement")
		float SpawnY = 2990.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goalkeeper Movement")
		float SpawnZ = 90.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn Mesh")
		USkeletalMeshComponent* Skmesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Goalkeeper Movement")
		void MoveRandomly();

};
