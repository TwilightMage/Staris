#include "UI/SceneLabel.h"

#include "UI/LabelExtension.h"
#include "Universe/Labeled.h"

void USceneLabel::SetLabelExtension(const TSubclassOf<ULabelExtension>& ExtensionClass)
{
	if (ExtensionClass)
	{
		auto newExtension = NewObject<ULabelExtension>(this, ExtensionClass);
		newExtension->SetOwningPlayer(GetOwningPlayer());
		PutExtension(newExtension);
		newExtension->Setup(Cast<UObject>(Labeled));
	}
	else
	{
		PutExtension(nullptr);
	}
}

TScriptInterface<ILabeled> USceneLabel::GetLabeled() const
{
	return Cast<UObject>(Labeled);
}
