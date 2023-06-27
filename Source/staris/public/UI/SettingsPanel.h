// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsPanel.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FSettingsPanelNumberCallback, double, Value);
DECLARE_DELEGATE_OneParam(FSettingsPanelNumberCallbackNative, double);
DECLARE_DYNAMIC_DELEGATE_OneParam(FSettingsPanelStringCallback, const FString&, Value);
DECLARE_DELEGATE_OneParam(FSettingsPanelStringCallbackNative, const FString&);
DECLARE_DYNAMIC_DELEGATE_OneParam(FSettingsPanelSelectCallback, const FString&, Value);
DECLARE_DELEGATE_OneParam(FSettingsPanelSelectCallbackNative, const FString&);

UCLASS()
class USettingsInputNumberHandler : public UClass
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void Call(double a)
	{
		Delegate.ExecuteIfBound(a);
	}

	FSettingsPanelNumberCallbackNative Delegate;
};

UCLASS()
class USettingsInputStringHandler : public UClass
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void Call(const FString& a)
	{
		Delegate.ExecuteIfBound(a);
	}

	FSettingsPanelStringCallbackNative Delegate;
};

UCLASS()
class USettingsInputSelectHandler : public UClass
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void Call(const FString& a)
	{
		Delegate.ExecuteIfBound(a);
	}

	FSettingsPanelSelectCallbackNative Delegate;
};

#define SETTINGS_SECTION(Namespace, Name) SettingsPanel->AddSection(NSLOCTEXT(Namespace, Name, Name));

#define SETTINGS_FIELD_SEED(Name) SettingsPanel->AddFieldSeedNative(FSettingsPanelNumberCallbackNative::CreateLambda([&](float Value) \
{                                                                                                                                     \
	Json->SetNumberField(#Name, (int32)Value);                                                                                        \
}));                                                                                                                                  \

#define SETTINGS_FIELD_SELECT_ITEMS(...) TArray<FString>{__VA_ARGS__}

#define SETTINGS_FIELD_SELECT(Namespace, Title, Default, Items, Name)                                                                                               \
SettingsPanel->AddFieldSelectNative(NSLOCTEXT(Namespace, Title, Title), Default, Items, FSettingsPanelSelectCallbackNative::CreateLambda([&](const FString& Value){ \
	Json->SetStringField(#Name, Value);                                                                                                                             \
}));                                                                                                                                                                \
Json->SetStringField(#Name, Default);                                                                                                                               \

#define SETTINGS_FIELD_STRING(Namespace, Title, Default, Name)                                                                                               \
SettingsPanel->AddFieldStringNative(NSLOCTEXT(Namespace, Title, Title), Default, FSettingsPanelSelectCallbackNative::CreateLambda([&](const FString& Value){ \
	Json->SetStringField(#Name, Value);                                                                                                                      \
}));                                                                                                                                                         \
Json->SetStringField(#Name, Default);                                                                                                                        \

#define SETTINGS_FIELD_SELECT_CUSTOM(Namespace, Title, Default, Items, Handler) SettingsPanel->AddFieldSelectNative(NSLOCTEXT(Namespace, Title, Title), Default, Items, Handler); \

#define SETTINGS_FIELD_NUMBER(Namespace, Title, IsInteger, HasMin, Min, HasMax, Max, Default, Postfix, Type, Name)                                                                                \
SettingsPanel->AddFieldNumberNative(IsInteger, HasMin, Min, HasMax, Max, NSLOCTEXT(Namespace, Title, Title), Default, Postfix, FSettingsPanelNumberCallbackNative::CreateLambda([&](double Value) \
{                                                                                                                                                                                                 \
	Json->SetNumberField(#Name, (Type)Value);                                                                                                                                                     \
}));                                                                                                                                                                                              \
Json->SetNumberField(#Name, (Type)Default)                                                                                                                                                        \

#define SETTINGS_FIELD_NUMBER_CUSTOM(Namespace, Title, IsInteger, HasMin, Min, HasMax, Max, Default, Postfix, Type, Handler)             \
SettingsPanel->AddFieldNumberNative(IsInteger, HasMin, Min, HasMax, Max, NSLOCTEXT(Namespace, Title, Title), Default, Postfix, Handler); \

#define SETTINGS_FIELD_MIN_MAX(Namespace, Title, IsInteger, HasMin, Min, HasMax, Max, DefaultMin, DefaultMax, Type, Name)                                                                               \
SettingsPanel->AddFieldMinMaxNative(IsInteger, HasMin, Min, HasMax, Max, NSLOCTEXT(Namespace, Title, Title), DefaultMin, DefaultMax, FSettingsPanelNumberCallbackNative::CreateLambda([&](double Value) \
{                                                                                                                                                                                                       \
	if (!Json->HasTypedField<EJson::Object>(#Name)) Json->SetObjectField(#Name, MakeShared<FJsonObject>());                                                                                             \
	Json->GetObjectField(#Name)->SetNumberField("Min", Value);                                                                                                                                          \
}), FSettingsPanelNumberCallbackNative::CreateLambda([&](double Value)                                                                                                                                  \
{                                                                                                                                                                                                       \
	if (!Json->HasTypedField<EJson::Object>(#Name)) Json->SetObjectField(#Name, MakeShared<FJsonObject>());                                                                                             \
	Json->GetObjectField(#Name)->SetNumberField("Max", Value);                                                                                                                                          \
}));                                                                                                                                                                                                    \
Json->SetObjectField(#Name, MakeShared<FJsonObject>());                                                                                                                                                 \
Json->GetObjectField(#Name)->SetNumberField("Min", DefaultMin);                                                                                                                                         \
Json->GetObjectField(#Name)->SetNumberField("Max", DefaultMax);   

UCLASS()
class STARIS_API USettingsPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void AddFieldSeed(const FSettingsPanelNumberCallback& Callback);

	void AddFieldSeedNative(const FSettingsPanelNumberCallbackNative& NativeCallback);
	
	UFUNCTION(BlueprintImplementableEvent)
	void AddFieldNumber(bool bInteger, bool bHasMin, double Min, bool bHasMax, double Max, const FText& Title, double DefaultValue, const FString& Postfix, const FSettingsPanelNumberCallback& Callback);

	void AddFieldNumberNative(bool bInteger, bool bHasMin, double Min, bool bHasMax, double Max, const FText& Title, double DefaultValue, const FString& Postfix, const FSettingsPanelNumberCallbackNative& NativeCallback);
	
	UFUNCTION(BlueprintImplementableEvent)
	void AddFieldMinMax(bool bInteger, bool bHasMin, double Min, bool bHasMax, double Max, const FText& Title, double DefaultMin, double defaultMax, const FSettingsPanelNumberCallback& MinCallback, const FSettingsPanelNumberCallback& MaxCallback);

	void AddFieldMinMaxNative(bool bInteger, bool bHasMin, double Min, bool bHasMax, double Max, const FText& Title, double DefaultMin, double defaultMax, const FSettingsPanelNumberCallbackNative& NativeMinCallback, const FSettingsPanelNumberCallbackNative& NativeMaxCallback);
	
	UFUNCTION(BlueprintImplementableEvent)
	void AddFieldString(const FText& Title, const FString& DefaultValue, const FSettingsPanelStringCallback& Callback);

	void AddFieldStringNative(const FText& Title, const FString& DefaultValue, const FSettingsPanelStringCallbackNative& NativeCallback);

	UFUNCTION(BlueprintImplementableEvent)
	void AddFieldSelect(const FText& Title, const FString& DefaultValue, const TArray<FString>& Items, const FSettingsPanelSelectCallback& Callback);

	void AddFieldSelectNative(const FText& Title, const FString& DefaultValue, const TArray<FString>& Items, const FSettingsPanelSelectCallbackNative& NativeCallback);
	
	UFUNCTION(BlueprintImplementableEvent)
	void AddSection(const FText& Title);

private:
	TArray<UObject*> Handlers;
};
