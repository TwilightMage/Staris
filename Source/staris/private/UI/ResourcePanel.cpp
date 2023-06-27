// Copyright DragonCap. All rights reserved.


#include "UI/ResourcePanel.h"

#include "Empire/Empire.h"
#include "Game/StarisPlayerController.h"

void UResourcePanel::NativeConstruct()
{
	Super::NativeConstruct();

	if (auto Player = Cast<AStarisPlayerController>(GetOwningLocalPlayer()->PlayerController))
	{
		if (auto Empire = Player->GetEmpire())
		{
			Empire->OnBalanceUpdated.AddUObject(this, &UResourcePanel::Update);
			Update(Empire->GetLatestBalanceUpdate());
		}
	}
}
