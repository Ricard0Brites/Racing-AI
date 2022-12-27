// Fill out your copyright notice in the Description page of Project Settings.


#include "../NotFastJustFurious/NeuralNetwork.h"
#include "Engine/Engine.h"

#pragma region Neural Network
ANeuralNetwork::ANeuralNetwork()
{
}

void ANeuralNetwork::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ANeuralNetwork::CreateNetwork(uint8 InputLayerNeuronAmount, uint8 HiddenLayerAmount, uint8 HiddenLayerNeuronAmount, uint8 OutputLayerNeuronAmount)
{
	#pragma region Input Layer
	NeuralLayers.AddUnique(new FNeuralLayer(this));
	NeuralLayers[0]->InitLayer(InputLayerNeuronAmount);
	#pragma endregion

	#pragma region Hidden Layers
	for (int i = 0; i < HiddenLayerAmount; ++i)
	{
		NeuralLayers.AddUnique(new FNeuralLayer(this));
		NeuralLayers[i + 1]->InitLayer(HiddenLayerNeuronAmount);
	}
	#pragma endregion

	#pragma region Output Layer
	NeuralLayers.AddUnique(new FNeuralLayer(this));
	NeuralLayers[NeuralLayers.Num() - 1]->InitLayer(OutputLayerNeuronAmount);
	#pragma endregion

	#pragma region Create Connection
	for (FNeuralLayer* NLayer : NeuralLayers)
	{
		NLayer->CreateConnections();
	}
	#pragma endregion
}

#pragma endregion

#pragma region Layer

void FNeuralLayer::InitLayer(int NumberOfNeurons)
{
	_Neurons.Empty();
	for (int i = 0; i < NumberOfNeurons; ++i)
	{
		_Neurons.Add(new FNeuron(LayerIndex, i, RANDINRANGE(20), _ParentNetwork));
	}
}

void FNeuralLayer::CreateConnections()
{
	for (FNeuron* Neuron : _Neurons)
	{
		Neuron->SetConnections();
	}
}

#pragma endregion

#pragma region Neuron

FNeuron::FNeuron(int NewLayerIndex, int NewNeuronIndex, float NewNeuronValue, ANeuralNetwork* NewParentNetwork)
{
	LayerIndex = NewLayerIndex;
	NeuronIndex = NewNeuronIndex;
	NeuronValue = NewNeuronValue;
	ParentNetwork = NewParentNetwork;
}

void FNeuron::SetConnections()
{
	int counter = 0;

	if (LayerIndex - 1 >= 0)
	{
		for (FNeuron* neuron : ParentNetwork->NeuralLayers[LayerIndex - 1]->_Neurons)
		{
			NeuronConnection.NeuronToTheLeftIndex[counter] = ParentNetwork->NeuralLayers[LayerIndex - 1]->_Neurons[counter]->NeuronIndex;
			NeuronConnection.LeftConnectionWeight[counter] = RANDINRANGE(5);
			counter++;
		}
	}

	if (LayerIndex + 1 <= ParentNetwork->NeuralLayers.Num() - 1)
	{
		counter = 0;
		for (FNeuron* neuron : ParentNetwork->NeuralLayers[LayerIndex + 1]->_Neurons)
		{
			NeuronConnection.NeuronToTheRightIndex[counter] = ParentNetwork->NeuralLayers[LayerIndex + 1]->_Neurons[counter]->NeuronIndex;
			NeuronConnection.RightConnectionWeight[counter] = RANDINRANGE(5);
			counter++;
		}
	}

	NeuronConnection.MyIndex = NeuronIndex;
}

#pragma endregion
