// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "NeuralNetwork.generated.h"

#define RANDINRANGE(x) (rand() % ((x)) + 1)
#define ACTIVATIONFUNCTION(x) std::max(0.01f * x, x) //leaky ReLU
#define SIGMOID(x)
#define BIAS 5
#define WEIGHTVALRANGE 10

UENUM( BlueprintType )
enum ELinecastDirections
{
	front,
	back,
	right,
	left,
	bottom,
	topRight,
	topRight30,
	topLeft,
	topLeft30,
	bottomRight,
	bottomRight30,
	bottomLeft,
	bottomLeft30
};

UCLASS( BlueprintType, Blueprintable )
class UGMRACE_API ANeuralNetwork : public AActor
{
	GENERATED_BODY()
	  //
public:
	ANeuralNetwork();

	UFUNCTION( BlueprintCallable, Category = "Neural Network")
	void CreateNetwork(uint8 InputLayerNeuronAmount, uint8 HiddenLayerAmount, uint8 HiddenLayerNeuronAmount, uint8 OutputLayerNeuronAmount);

	UFUNCTION(BlueprintCallable, Category = "Neural Network")
	void UpdateNetwork(TArray<float> NewData);

	/*UFUNCTION(BlueprintCallable, Category = "Neural Network")
	void TriggerPositiveAction();
	
	UFUNCTION(BlueprintCallable, Category = "Neural Network")
	void TriggerNegativeAction();*/

	TArray<FNeuralLayer*> NeuralLayers;
};

USTRUCT(BlueprintType)
struct UGMRACE_API FNeuralLayer
{
	GENERATED_BODY()

	FNeuralLayer() { }
	FNeuralLayer(ANeuralNetwork* ParentNeuralNetwork) { _ParentNetwork = ParentNeuralNetwork; }

	void InitLayer(int NumberOfNeurons);

	void CreateConnections();
	void TriggerCalculations();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int LayerIndex;
	
	TArray<FNeuron*> _Neurons;
	ANeuralNetwork* _ParentNetwork;
};

USTRUCT(BlueprintType)
struct UGMRACE_API FConnection
{

	GENERATED_BODY()
		FConnection() {  }

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<int> NeuronToTheLeftIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int MyIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<int> NeuronToTheRightIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<float> LeftConnectionWeight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<float> RightConnectionWeight;
};

USTRUCT(BlueprintType)
struct UGMRACE_API FNeuron
{

	GENERATED_BODY()

	FNeuron() {}
	FNeuron(int NewLayerIndex, int NewNeuronIndex, float NewNeuronValue, ANeuralNetwork* NewParentNetwork);

	void SetConnections();
	void Calculate();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int LayerIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int NeuronIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float NeuronValue;

	TArray<float> IncomingValues;

	FConnection NeuronConnection;

	ANeuralNetwork* ParentNetwork;
};
