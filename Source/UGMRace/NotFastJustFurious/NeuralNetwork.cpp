// Fill out your copyright notice in the Description page of Project Settings.


#include "../NotFastJustFurious/NeuralNetwork.h"
#include "Engine/Engine.h"
#include "Misc/EnumRange.h"

#pragma region Neural Network
ANeuralNetwork::ANeuralNetwork()
{
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

void ANeuralNetwork::UpdateNetwork(TArray<float> NewData)
{
	int counter = 0;
	for (float entry : NewData)
	{
		NeuralLayers[0]->_Neurons[counter]->NeuronValue = entry; 
		counter++; 
	}
}

#pragma endregion

#pragma region Layer

void FNeuralLayer::InitLayer(int NumberOfNeurons)
{
	_Neurons.Empty();
	for (int i = 0; i < NumberOfNeurons; ++i)
	{
		_Neurons.Add(new FNeuron(LayerIndex, i, 0, _ParentNetwork));
	}
}

void FNeuralLayer::CreateConnections()
{
	for (FNeuron* Neuron : _Neurons)
	{
		Neuron->SetConnections();
	}
}

void FNeuralLayer::TriggerCalculations()
{
	for (int i = 0; i < _ParentNetwork->Layers.Num() - 1; ++i)
	{
		for (FNeuron* Neuron : _ParentNetwork->NeuralLayers[i]->_Neurons)
		{
			Neuron->Calculate();
		}
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
			NeuronConnection.LeftConnectionWeight[counter] = RANDINRANGE(WEIGHTVALRANGE);
			counter++;
		}
	}

	if (LayerIndex + 1 <= ParentNetwork->NeuralLayers.Num() - 1)
	{
		counter = 0;
		for (FNeuron* neuron : ParentNetwork->NeuralLayers[LayerIndex + 1]->_Neurons)
		{
			NeuronConnection.NeuronToTheRightIndex[counter] = ParentNetwork->NeuralLayers[LayerIndex + 1]->_Neurons[counter]->NeuronIndex;
			NeuronConnection.RightConnectionWeight[counter] = RANDINRANGE(WEIGHTVALRANGE);
			counter++;
		}
	}

	NeuronConnection.MyIndex = NeuronIndex;
}

void FNeuron::Calculate()
{
	if (LayerIndex == ParentNetwork->NeuralLayers.Num() - 1)
	{
		//TODO trigger an output node... this is the output layer
		return;
	}
	if (LayerIndex - 1 < 0)
	{
		//send ActivationFucntion( --current value-- * --the weight in question-- ) to the next neurons' IncomingValues array
	}
	if (LayerIndex - 1 >= 0 && (LayerIndex + 1 < ParentNetwork->NeuralLayers.Num() - 1))
	{
		//calculate the SUM of the incoming values
		float InputSum = 0;
		for (float Val : IncomingValues)
		{
			InputSum += Val;
		}
		//send the final value through the activation function
		float PAFValue = ACTIVATIONFUNCTION(InputSum - BIAS);
		
		//multiply the result by the weight in question send the value to the next neurons' IncomingValues array
		int counter = 0;
		for (FNeuron* Neuron : ParentNetwork->NeuralLayers[LayerIndex + 1]->_Neurons)
		{
			float weightValue = NeuronConnection.RightConnectionWeight[counter];
			Neuron->IncomingValues.Add(PAFValue * weightValue);
			counter++;
		}
	}
}

#pragma endregion