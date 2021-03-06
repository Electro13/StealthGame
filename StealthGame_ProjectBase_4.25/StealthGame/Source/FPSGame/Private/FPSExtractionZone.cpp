// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp")); // create instance
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore); // ignore other pawns
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // only work on the player
	OverlapComp->SetBoxExtent(FVector(200.0f)); // box dimensions
	RootComponent = OverlapComp; // assign as root component

	OverlapComp->SetHiddenInGame(false); // show box boundaries (temporarially)

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap); // alternative component

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.0f, 200.0f, 200.0f); // set size
	DecalComp->SetupAttachment(RootComponent); // attach to root (as child)
}

// gameover event
void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor); // get player
	if (MyPawn == nullptr) {
		return;
	}

	if (MyPawn->bIsCarryingObjective) // if play brought obj to exraction zone
	{
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM) {
			GM->CompleteMission(MyPawn, true); // pass in player that completed mission
		}

	}
	else 
	{
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound); // play sound
	}
	UE_LOG(LogTemp, Log, TEXT("Overlapped with extraction zone!"));
}