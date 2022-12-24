// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "NeuralNetwork.generated.h"

#define INPUTAMOUNT 13
#define OUTPUTAMOUNT 3

#define	NEURONAMOUNT 9
#define HIDDENLAYERAMOUNT 3

#define TWEAKINGSTEP 0.1f

#define MAXRNGRANGE 20
#define RANDINRANGE(x) (rand() % (( x + 1 )));

#define ACTIVATIONFUNCTION(x) class UGameplayStatics::Max(0.01f * x, x) //leaky ReLU

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

UCLASS(BlueprintType, Blueprintable)
class UGMRACE_API UNeuralNetwork : public UObject
{
	 GENERATED_BODY()

public:
	UNeuralNetwork();
	~UNeuralNetwork();

	FNeuralLayer* InputLayer;
	FNeuralLayer* HiddenLayers[HIDDENLAYERAMOUNT];
	FNeuralLayer* OutputLayer;
};

USTRUCT()
struct FNeuralLayer
 {
	
	GENERATED_BODY()
	
	//this constructor uses the predefined macro value for the amount of neurons to create
	FNeuralLayer();
	~FNeuralLayer();

	void Init(FNeuralLayer* PreviousLayer, FNeuralLayer* NextLayer, int NeuronAmount);

	FNeuralLayer *PreviousLayerRef, *NextLayerRef;
	TArray<FNeuron*> Neurons;


 };

USTRUCT()
struct FNeuron
 {
	GENERATED_BODY()

	 FNeuron();
	 ~FNeuron();

	 void Init(bool UseRandNeuronValues, int MaxRandRange);
	 void Init(bool UseRandNeuronValues);

	 float NeuronValue;
 };
 
USTRUCT()
struct FNeuronConnection
 {
	GENERATED_BODY()

	FNeuronConnection();
	~FNeuronConnection();

	FNeuron *OriginNeuron, *TargetNeuron;

	float Weight = 1;
 };
