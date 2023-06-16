// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonUtils.h"

TSharedPtr<FJsonObject> UJsonUtils::StringToJson(const FString& string)
{
	TSharedPtr<FJsonObject> result;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(string);
	FJsonSerializer::Deserialize(JsonReader, result);

	return result;
}
