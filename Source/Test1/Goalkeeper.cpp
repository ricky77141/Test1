// Fill out your copyright notice in the Description page of Project Settings.


#include "Goalkeeper.h"

// Sets default values
AGoalkeeper::AGoalkeeper()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;


    Skmesh = GetMesh();
    Skmesh->SetupAttachment(RootComponent);
    auto SkmeshAsset = ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("/Script/Engine.SkeletalMesh'/Game/Classic_Footballer/Meshes/Body/SK_Classic_Footballer_BODY.SK_Classic_Footballer_BODY'"));
    auto AnimAsset = ConstructorHelpers::FObjectFinder<UAnimBlueprint>(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Goalkeeper/ABP_Goalkeeper.ABP_Goalkeeper'"));

    if (SkmeshAsset.Succeeded()) {
        Skmesh->SetSkeletalMesh(SkmeshAsset.Object);
        Skmesh->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
        Skmesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

        if (AnimAsset.Succeeded())
            Skmesh->SetAnimClass(AnimAsset.Object->GeneratedClass);
    }

}

// Called when the game starts or when spawned
void AGoalkeeper::BeginPlay()
{
    Super::BeginPlay();

    FVector SpawnLocation = FVector(SpawnX, SpawnY, SpawnZ);
    SetActorLocation(SpawnLocation);
}

// Called every frame
void AGoalkeeper::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGoalkeeper::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void AGoalkeeper::MoveRandomly()
{
    bool bShouldMoveRight = SelectSide();
    FVector MovementDirection = FVector::ZeroVector;

    if (bShouldMoveRight)
        MovementDirection = FVector(0.f, 1.f, 0.f);
    else
        MovementDirection = FVector(0.f, -1.f, 0.f);

    FVector NextLocation = GetActorLocation() + MovementDirection * MovementSpeed * GetWorld()->DeltaTimeSeconds;
    NextLocation.X = FMath::Clamp(NextLocation.X, MinX, MaxX);
    NextLocation.Y = FMath::Clamp(NextLocation.Y, MinY, MaxY);

    SetActorLocation(NextLocation);
}
bool AGoalkeeper::SelectSide() {
    return FMath::RandBool();
}

