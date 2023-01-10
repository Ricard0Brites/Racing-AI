// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NeuralNetwork.generated.h"

#define WEIGHTMAXVALUE 1001

class UNFJFSaveGame;

struct Neuron
{
	Neuron()
	{
		Weights.Empty();
	}

	void AddWeight() { Weights.Add(abs(rand() % WEIGHTMAXVALUE)); }

	float Value = floor(rand()) / RAND_MAX;
	TArray<float> Weights;
};
struct NeuralLayer
{
	TArray<Neuron> Neurons;
};
struct NN
{
	NeuralLayer InputLayer;
	TArray<NeuralLayer> HiddenLayers;
	NeuralLayer OutputLayer;
};

UCLASS()
class UGMRACE_API ANeuralNetwork : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANeuralNetwork();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	#pragma region Setup
	UFUNCTION(BlueprintCallable, Category = "Neural Network")
		void InitNeuralNetwork(int32 InputLayerNeuronAmount, int32 HiddenLayerAmount, int32 NeuronAmountPerHiddenLayer, int32 OutputAmount);

	UFUNCTION(BlueprintImplementableEvent, Category = "Neural Network|Activation Function")
		float ActivationFunction(float Input);

	UFUNCTION(BlueprintCallable, Category = "Neural Network|Save")
		void SaveNeuralNetworkState(bool& Success);

	UFUNCTION(BlueprintCallable, Category = "Neural Network|Save")
		bool LoadNeuralNetworkValues();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Neural Network|Save")
		void ResetNetwork();

	void LoadSaveFileToMemory();
#pragma endregion

	UFUNCTION(BlueprintCallable, Category = "Neural Network|Learning")
	void Incentivise();

	UFUNCTION(BlueprintCallable, Category = "Neural Network|Learning")
	void Punish();

	UFUNCTION(BlueprintCallable, Category = "Neural Network|Learning")
	void NeuralInput(TArray<float> InputValues);

	UFUNCTION(BlueprintImplementableEvent, Category = "Neural Network|Learning")
	void NeuralOutput(const TArray<float> &Out);

	#pragma region Properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Neural Network|Save")
		FString SaveFileName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Neural Network|Learning")
		float IncentiveOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Neural Network|Learning")
		float PunishmentOffset;
#pragma endregion

private:
	NN* _NeuralNetwork = new NN;
	
	UPROPERTY( BlueprintReadWrite, Category = "Neural Network|Save", meta = (AllowPrivateAccess = true))
	UNFJFSaveGame* _Save;

	//NN info
	int32 _InputLayerNeuronAmount, _HiddenLayerAmount, _HiddenLayerNeuronAmount, _OutputAmount;
};
