// Copyright DragonCap. All rights reserved.


#include "UI/FocusablePanel.h"

void UFocusablePanel::Close()
{
	OnClose.Broadcast();
	OnClose_K2.Broadcast();
}
