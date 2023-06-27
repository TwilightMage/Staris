// Copyright DragonCap. All rights reserved.


#include "UI/SettingsPanel.h"

void USettingsPanel::AddFieldSeedNative(const FSettingsPanelNumberCallbackNative& NativeCallback)
{
	auto Handler = NewObject<USettingsInputNumberHandler>();
	Handler->Delegate = NativeCallback;
	Handlers.Add(Handler);
	FSettingsPanelNumberCallback Callback;
	Callback.BindDynamic(Handler, &USettingsInputNumberHandler::Call);
	AddFieldSeed(Callback);
}

void USettingsPanel::AddFieldNumberNative(bool bInteger, bool bHasMin, double Min, bool bHasMax, double Max, const FText& Title, double DefaultValue, const FString& Postfix, const FSettingsPanelNumberCallbackNative& NativeCallback)
{
	auto Handler = NewObject<USettingsInputNumberHandler>();
	Handler->Delegate = NativeCallback;
	Handlers.Add(Handler);
	FSettingsPanelNumberCallback Callback;
	Callback.BindDynamic(Handler, &USettingsInputNumberHandler::Call);
	AddFieldNumber(bInteger, bHasMin, Min, bHasMax, Max, Title, DefaultValue, Postfix, Callback);
}

void USettingsPanel::AddFieldMinMaxNative(bool bInteger, bool bHasMin, double Min, bool bHasMax, double Max, const FText& Title, double DefaultMin, double defaultMax, const FSettingsPanelNumberCallbackNative& NativeMinCallback, const FSettingsPanelNumberCallbackNative& NativeMaxCallback)
{
	auto MinHandler = NewObject<USettingsInputNumberHandler>();
	MinHandler->Delegate = NativeMinCallback;
	Handlers.Add(MinHandler);
	FSettingsPanelNumberCallback MinCallback;
	MinCallback.BindDynamic(MinHandler, &USettingsInputNumberHandler::Call);

	auto MaxHandler = NewObject<USettingsInputNumberHandler>();
	MaxHandler->Delegate = NativeMinCallback;
	Handlers.Add(MaxHandler);
	FSettingsPanelNumberCallback MaxCallback;
	MaxCallback.BindDynamic(MaxHandler, &USettingsInputNumberHandler::Call);
	
	AddFieldMinMax(bInteger, bHasMin, Min, bHasMax, Max, Title, DefaultMin, defaultMax, MinCallback, MaxCallback);
}

void USettingsPanel::AddFieldStringNative(const FText& Title, const FString& DefaultValue, const FSettingsPanelStringCallbackNative& NativeCallback)
{
	auto Handler = NewObject<USettingsInputStringHandler>();
	Handler->Delegate = NativeCallback;
	Handlers.Add(Handler);
	FSettingsPanelStringCallback Callback;
	Callback.BindDynamic(Handler, &USettingsInputStringHandler::Call);
	AddFieldString(Title, DefaultValue, Callback);
}

void USettingsPanel::AddFieldSelectNative(const FText& Title, const FString& DefaultValue, const TArray<FString>& Items, const FSettingsPanelSelectCallbackNative& NativeCallback)
{
	auto Handler = NewObject<USettingsInputSelectHandler>();
	Handler->Delegate = NativeCallback;
	Handlers.Add(Handler);
	FSettingsPanelSelectCallback Callback;
	Callback.BindDynamic(Handler, &USettingsInputSelectHandler::Call);
	AddFieldSelect(Title, DefaultValue, Items, Callback);
}
