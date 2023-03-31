// Copyright Epic Games, Inc. All Rights Reserved.

#include "Test1Character.h"

#include "Ball.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


//////////////////////////////////////////////////////////////////////////
// ATest1Character

ATest1Character::ATest1Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 600.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Create a kick point on the right foot.
    KickerPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Kick point"));
	KickerPoint->SetupAttachment(RootComponent);
    
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ATest1Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATest1Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATest1Character::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATest1Character::Look);

		//Kicking
		EnhancedInputComponent->BindAction(KickAction, ETriggerEvent::Triggered, this, &ATest1Character::ExecuteKickAnimation);

	}

}

void ATest1Character::Move(const FInputActionValue& Value)
{
	
	if (CanMove)
	{
		// input is a Vector2D
		FVector2D MovementVector = Value.Get<FVector2D>();
		if (Controller != nullptr)
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}
	}
}

void ATest1Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

//Wait for kick animation complete before actually kick.
void ATest1Character::ExecuteKickAnimation()
{
	Kicking = true;
	//Negate move control, so the kick animation play complete.
	CanMove = false;
	GetWorld()->GetTimerManager().SetTimer(PosAnimHandler,this,&ATest1Character::KickBall,WaitTime,false);
}

//Use a collision sphere to detect object to kick.
void ATest1Character::KickBall()
{

	TArray<FHitResult> OutHits;
	
	FCollisionShape CollSphere = FCollisionShape::MakeSphere(30.f);
	
	const FVector Start = KickerPoint->GetComponentLocation();

	//DrawDebugSphere(GetWorld(),Start,CollSphere.GetSphereRadius(),12,FColor::Cyan,true);
	
	FCollisionQueryParams TraceParams;
	
	bool hit = GetWorld()->SweepMultiByChannel(OutHits,Start,Start,FQuat::Identity,ECC_Visibility,CollSphere);
	if (hit)
	{
		for(auto& Hit : OutHits)
		{
			UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent());
		
			if (MeshComponent && Hit.GetActor()->IsRootComponentMovable())
			{
				//Use vector component Z to try a parabolic shoot.
				FVector Forward = this->GetActorForwardVector() + FVector(0,0,0.25f);
				//Add impulse force to the ball for kicking it.
				MeshComponent->AddImpulse(Forward * ImpulseForce,"None",true);
			}
		}
	}
	Kicking = false;
	CanMove = true;
}





