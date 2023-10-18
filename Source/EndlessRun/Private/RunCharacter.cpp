// Fill out your copyright notice in the Description page of Project Settings.


#include "RunCharacter.h"
#include "EndlessRunnerGameModeBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARunCharacter::ARunCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->TargetArmLength = 350.0f;
	CameraArm->SocketOffset = FVector(0.f, 0.f, 100.f);
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camer"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void ARunCharacter::BeginPlay()
{
	Super::BeginPlay();

	RunGameMode = Cast<AEndlessRunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(RunGameMode);
}

// Called every frame

void ARunCharacter::AddCoin()
{
	RunGameMode->AddCoin();
}

void ARunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator ControlRot = GetControlRotation();
	ControlRot.Roll = 0.f;
	ControlRot.Pitch = 0.f;

	AddMovementInput(ControlRot.Vector());
}

// Called to bind functionality to input

void ARunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARunCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARunCharacter::StopJumping);
	
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ARunCharacter::MoveLeft);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ARunCharacter::MoveRight);
	PlayerInputComponent->BindAction("MoveDown", IE_Pressed, this, &ARunCharacter::MoveDown);

}

void ARunCharacter::ChangeLaneUpdate(const float Value)
{
	FVector Location = GetCapsuleComponent()->GetComponentLocation();
	Location.Y = FMath::Lerp(RunGameMode->LaneSwitchValues[CurrentLane], RunGameMode->LaneSwitchValues[NextLane], Value);
	SetActorLocation(Location);
}

void ARunCharacter::ChangeLaneFinished(float Value)
{
	CurrentLane = NextLane;
}

void ARunCharacter::Death()
{
	if (!bIsDead)
	{
		const FVector Location = GetActorLocation();
		UWorld* World = GetWorld();

		if (World)
		{
			bIsDead = true;
			DisableInput(nullptr);
		
			if (DeathParticleSystem)
			{
				UGameplayStatics::SpawnEmitterAtLocation(World,DeathParticleSystem,Location);	
			}
			if (DeathSound)
			{
				UGameplayStatics::PlaySoundAtLocation(World,DeathSound,Location);
			}

			GetMesh()->SetVisibility(false);

			World->GetTimerManager().SetTimer(RestartTimerHandle,this,&ARunCharacter::OnDeath, 1.f);
		}	
	}
}

void ARunCharacter::OnDeath()
{
	bIsDead = false;
	if (RestartTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(RestartTimerHandle);
	}
	
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(),TEXT("RestartLevel"));
}

void ARunCharacter::MoveLeft()
{
	NextLane = FMath::Clamp(CurrentLane - 1, 0, 2);
	ChangeLane();
}

void ARunCharacter::MoveRight()
{
	NextLane = FMath::Clamp(CurrentLane + 1, 0, 2);
	ChangeLane();
}

void ARunCharacter::MoveDown()
{
	UE_LOG(LogTemp,Warning,TEXT("MoveDownPressed"));
}

