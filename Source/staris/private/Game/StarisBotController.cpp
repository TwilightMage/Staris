// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StarisBotController.h"

#include "StarisStatics.h"

TArray<FString> BotNameList = {
	"Zeus",
	"Hera",
	"Poseidon",
	"Demeter",
	"Athena",
	"Apollo",
	"Artemis",
	"Ares",
	"Aphrodite",
	"Hephaestus",
	"Hermes",
	"Hestia",
	"Dionysus",
	"Hades",
	"Persephone",
	"Cronus",
	"Rhea",
	"Uranus",
	"Gaia",
	"Oceanus",
	"Tethys",
	"Hyperion",
	"Theia",
	"Coeus",
	"Mnemosyne",
	"Crius",
	"Iapetus",
	"Titans",
	"Helios",
	"Gigantes",
	"Erebus",
	"Nyx",
	"Eros",
	"Hemera",
	"Eirene",
	"Eleusinian",
	"Eris",
	"Morpheus",
	"Nemesis",
	"Nike",
	"Pan",
	"Proteus",
	"Selene",
	"Thanatos",
	"Triptolemus",
	"Charon",
	"Eleusis",
	"Echo",
	"Aeolus",
	"Aeon",
	"Erichthonius",
	"Hermaphroditus",
	"Asclepius",
	"Aetna",
	"Janus",
	"Iapetus",
	"Eumenides",
	"Eunomia",
	"Aegion",
	"Aegis",
	"Aegipan",
	"Aegir",
	"Aegle",
	"Aegnis",
	"Aegno",
	"Aegon",
	"Aegre",
	"Aegron",
	"Aedemon",
	"Aedesia",
	"Aedon",
	"Aedrom",
	"Aedron",
	"Aezon",
	"Aias",
	"Aigon",
	"Aidon",
	"Aithon",
	"Aithiop",
	"Aithiopion",
	"Aithiopia",
	"Aithios",
	"Aithirion",
	"Aithiria",
	"Aithirian"
};

// Sets default values
AStarisBotController::AStarisBotController()
{
	PrimaryActorTick.bCanEverTick = true;

	BotName = GetRandomArrayItem(BotNameList);
}

UEmpire* AStarisBotController::GetEmpire() const
{
	return OwnedEmpire;
}

void AStarisBotController::AssignEmpire(UEmpire* Empire)
{
	OwnedEmpire = Empire;
}
