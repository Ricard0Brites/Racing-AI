// Fill out your copyright notice in the Description page of Project Settings.


#include "NeuralNetwork.h"
#include "NFJFSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ANeuralNetwork::ANeuralNetwork()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ANeuralNetwork::BeginPlay()
{
	Super::BeginPlay();
	LoadSaveFileToMemory();
}

// Called every frame
void ANeuralNetwork::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANeuralNetwork::InitNeuralNetwork(int32 InputLayerNeuronAmount, int32 HiddenLayerAmount, int32 NeuronAmountPerHiddenLayer, int32 OutputAmount)
{
	// TODO->OUTPUT
	if(!_NeuralNetwork) return;

	_InputLayerNeuronAmount = InputLayerNeuronAmount;
	_HiddenLayerAmount = HiddenLayerAmount;
	_HiddenLayerNeuronAmount = NeuronAmountPerHiddenLayer;
	_OutputAmount = OutputAmount;

	//creates hidden layers
	_NeuralNetwork->HiddenLayers.Empty();
	_NeuralNetwork->HiddenLayers.SetNum(HiddenLayerAmount);
	for (int i = 0; i < HiddenLayerAmount; ++i) // for each hidden layer
	{
		//create Neurons
		_NeuralNetwork->HiddenLayers[i].Neurons.Empty();
		_NeuralNetwork->HiddenLayers[i].Neurons.SetNum(NeuronAmountPerHiddenLayer);

		//create weights
		for (int u = 0; u < NeuronAmountPerHiddenLayer; ++u) // for each neuron
		{
			for (int j = 0; j < NeuronAmountPerHiddenLayer; ++j) // create X weights
			{
				_NeuralNetwork->HiddenLayers[i].Neurons[j].AddWeight();
			}
		}
	}

	//Input Layer
	_NeuralNetwork->InputLayer.Neurons.Empty();
	_NeuralNetwork->InputLayer.Neurons.SetNum(InputLayerNeuronAmount);
	for (int NeuronIndex = 0; NeuronIndex < _NeuralNetwork->InputLayer.Neurons.Num(); ++NeuronIndex) // for each neuron
	{
		for (int u = 0; u < NeuronAmountPerHiddenLayer; ++u) //Make X weights
		{
			_NeuralNetwork->InputLayer.Neurons[NeuronIndex].AddWeight();
		}
	}

	//Output Layer
	_NeuralNetwork->OutputLayer.Neurons.Empty();
	_NeuralNetwork->OutputLayer.Neurons.SetNum(OutputAmount);

}

void ANeuralNetwork::SaveNeuralNetworkState(bool &Success)
{	
	if (_Save)
	{
		_Save->NeuronValues.Empty();
		_Save->NeuronWeights.Empty();
		
		//Input Layer
		for (Neuron N : _NeuralNetwork->InputLayer.Neurons)
		{
			_Save->NeuronValues.Add(N.Value);

			for (float Weight : N.Weights)
			{
				_Save->NeuronWeights.Add(Weight);
			}
		}

		// Hidden Layers
		for (NeuralLayer NL : _NeuralNetwork->HiddenLayers)
		{
			for (Neuron N : NL.Neurons)
			{
				_Save->NeuronValues.Add(N.Value);
				
				for (float Weight : N.Weights)
				{
					_Save->NeuronWeights.Add(Weight);
				}
			}
		}

		//Output Layer
		for (Neuron N : _NeuralNetwork->OutputLayer.Neurons)
		{
			_Save->NeuronValues.Add(N.Value);
		}

		Success = UGameplayStatics::SaveGameToSlot(_Save, SaveFileName, 0);
		return;
	}
	LoadSaveFileToMemory();
	Success = false;
}

bool ANeuralNetwork::LoadNeuralNetworkValues()
{
	if (_Save)
	{
		#pragma region Verify NN Size
		if (_Save->NeuronValues.Num() == 0) return false;
		int32 neuronVerifierResult = _Save->NeuronValues.Num();
		neuronVerifierResult -= _InputLayerNeuronAmount; // verifier - input layer neuron amount
		neuronVerifierResult -= (_HiddenLayerAmount * _HiddenLayerNeuronAmount); // verifer - hidden layer neuron amount
		neuronVerifierResult -= _OutputAmount; // verifier - output layer neuron amount
		if (neuronVerifierResult != 0) return false;
#pragma endregion

		int neuronCounter = 0, weightCounter = 0;

		//Input Layer
		for (int i = 0; i < _NeuralNetwork->InputLayer.Neurons.Num(); ++i) // for each neuron in INPUT LAYER
		{
			Neuron* N = &(_NeuralNetwork->InputLayer.Neurons[i]);
			N->Value = _Save->NeuronValues[neuronCounter++];
			
			for (int u = 0; u < _NeuralNetwork->InputLayer.Neurons[0].Weights.Num(); ++u) // for each weight in a neuron
			{
					N->Weights[u] = _Save->NeuronWeights[weightCounter++];
			}
		}

		//Hidden Layers
		for (int i = 0; i < _NeuralNetwork->HiddenLayers.Num(); ++i) //for each Hidden Layer in a Neural network
		{
			NeuralLayer* NL = &(_NeuralNetwork->HiddenLayers[i]);

			for (int u = 0; u < NL->Neurons.Num(); ++u) // for each neuron in a layer
			{
				Neuron* N = &(NL->Neurons[u]);
				N->Value = _Save->NeuronValues[neuronCounter++];
				
				for (int k = 0; k < N->Weights.Num(); ++k) //for each weight in a neuron
				{
					N->Weights[k] = _Save->NeuronWeights[weightCounter++];
				}
			}
		}

		//Output Layer
		for (int i = 0; i < _NeuralNetwork->OutputLayer.Neurons.Num(); ++i) // for each neuron in OUTPUT LAYER
		{
			_NeuralNetwork->OutputLayer.Neurons[i].Value = _Save->NeuronValues[neuronCounter++];
		}

		return true;
	}
	return false;
}

void ANeuralNetwork::ResetNetwork()
{
	UGameplayStatics::DeleteGameInSlot(SaveFileName, 0);
}

void ANeuralNetwork::LoadSaveFileToMemory()
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveFileName, 0))
	{
		_Save = (UNFJFSaveGame*)UGameplayStatics::CreateSaveGameObject(UNFJFSaveGame::StaticClass());
		UGameplayStatics::SaveGameToSlot(_Save, SaveFileName, 0);
	}
	_Save = (UNFJFSaveGame*)UGameplayStatics::LoadGameFromSlot(SaveFileName, 0);
}

void ANeuralNetwork::Incentivise()
{
	
}	

void ANeuralNetwork::Punish()
{

}

void ANeuralNetwork::NeuralInput(TArray<float> InputValues)
{
	if (InputValues.Num() != _InputLayerNeuronAmount) return; 
	
	TArray<NeuralLayer*> NeuralLayers;
	NeuralLayers.Empty();
	NeuralLayers.AddUnique(&_NeuralNetwork->InputLayer);
	for (int i = 0; i < _NeuralNetwork->HiddenLayers.Num(); ++i)
	{
		NeuralLayers.AddUnique(&_NeuralNetwork->HiddenLayers[i]);
	}
	NeuralLayers.AddUnique(&_NeuralNetwork->OutputLayer);

	for (int LayerIndex = 0; LayerIndex < NeuralLayers.Num(); ++LayerIndex) // for each layer
	{
		if (LayerIndex == 0) // input layer
		{
			for (int i = 0; i < _NeuralNetwork->InputLayer.Neurons.Num(); ++i) // for each neuron in input layer
			{
				_NeuralNetwork->InputLayer.Neurons[i].Value = InputValues[i];
			}
			continue;
		}
		if (LayerIndex > 0 && LayerIndex < NeuralLayers.Num() ) // Hidden Layers
		{
			for (int NeuronIndex = 0; NeuronIndex < NeuralLayers[LayerIndex]->Neurons.Num(); ++NeuronIndex) // for each neuron in the layer
			{
				float sum = 0;

				for (int PLNeuronIndex = 0; PLNeuronIndex < NeuralLayers[LayerIndex - 1]->Neurons.Num(); ++PLNeuronIndex)
				{	
					//Weight * NeuronValue
					float nv = NeuralLayers[LayerIndex - 1]->Neurons[PLNeuronIndex].Value;
					float wv = NeuralLayers[LayerIndex - 1]->Neurons[PLNeuronIndex].Weights[NeuronIndex];
					
					sum += ( nv * wv );
				}

				NeuralLayers[LayerIndex]->Neurons[NeuronIndex].Value = ActivationFunction(sum);
			}
		}
		if (LayerIndex == NeuralLayers.Num() - 1)
		{
			TArray<float> outputValues;
			outputValues.Empty();
			for (Neuron N : _NeuralNetwork->OutputLayer.Neurons)
			{
				outputValues.Add(N.Value);
			}
			NeuralOutput(outputValues);
		}
	}
}
