#include "Universe/CompositeDatabase.h"

#include "UObject/UnrealTypePrivate.h"

TSubclassOf<UCompositeRecordComponent> UCompositeRecord::GetRecordComponentType() const
{
	if (auto Database = Cast<UCompositeDatabase>(GetOuter()))
	{
		return Database->RecordComponentType;
	}

	return nullptr;
}

void UCompositeDatabase::ApplyPreset(UCompositeDatabasePreset* Preset)
{
	for (auto Record : Preset->Records)
	{
		auto MyRecord = GetOrCreateRecord(Record.Key);

		for (auto Component : Record.Value->Components)
		{
			MyRecord->Components.Add(Component->GetClass(), Component);
		}
	}
}
