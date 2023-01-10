// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "NeuralNetwork.h"
#include "NFJFSaveGame.generated.h"

UCLASS()
class UGMRACE_API UNFJFSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<float> NeuronValues;
	UPROPERTY(BlueprintReadOnly)
	TArray<float> NeuronWeights;
};
