// Fill out your copyright notice in the Description page of Project Settings.


#include "../NotFastJustFurious/NeuralNetwork.h"

#pragma region Neural Network
UNeuralNetwork::UNeuralNetwork()
{
	InputLayer->Init(nullptr, HiddenLayers[0], INPUTAMOUNT);

}

UNeuralNetwork::~UNeuralNetwork()
{

}
#pragma endregion

#pragma region Neural Layer Struct

FNeuralLayer::FNeuralLayer()
{
}

FNeuralLayer::~FNeuralLayer()
{

}

void FNeuralLayer::Init(FNeuralLayer* PreviousLayer, FNeuralLayer* NextLayer, int NeuronAmount)
{
	PreviousLayerRef = PreviousLayer;
	NextLayerRef = NextLayer;


	Neurons.Empty();
	Neurons.SetNum(NeuronAmount);
}

#pragma endregion

#pragma region Neuron
FNeuron::FNeuron()
{
}

FNeuron::~FNeuron()
{

}

void FNeuron::Init(bool UseRandNeuronValues)
{
	NeuronValue = RANDINRANGE(MAXRNGRANGE);
}

void FNeuron::Init(bool UseRandNeuronValues, int MaxRandRange)
{
	NeuronValue = RANDINRANGE(MaxRandRange);
}

#pragma endregion

#pragma region Neuron Connection
FNeuronConnection::FNeuronConnection()
{

}

FNeuronConnection::~FNeuronConnection()
{

}

#pragma endregion

