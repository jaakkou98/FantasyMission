// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMessage.h"

void UPlayerMessage::SetMessageType(EPlayerMessageType NewType)
{
	Type = NewType;
}

void UPlayerMessage::SetMessageValues(FString NewSender, FString NewReceiver)
{
	Sender = NewSender;
	Receiver = NewReceiver;
	switch (Type)
	{
	case EPlayerMessageType::WarEnd:
		bChoice = true;
		Content = "Peace Deal\n\n" + Sender + " has sent us a peace proposal. Do you accept?";
		break;
	case EPlayerMessageType::WarStart:
		bChoice = false;
		Content = "War Declaration\n\n" + Sender + " has declared war on us.";
		break;
	case EPlayerMessageType::AllianceEnd:
		bChoice = false;
		Content = "Alliance Ends\n\n" + Sender + " is no longer our ally.";
		break;
	case EPlayerMessageType::AllianceStart:
		bChoice = true;
		Content = "Alliance Request\n\n" + Sender + " wants to have an alliance with us. Do you accept?";
		break;
	case EPlayerMessageType::AllianceAccepted:
		bChoice = false;
		Content = "New Alliance\n\n" + Sender + " accepted your alliance request.";
		break;
	case EPlayerMessageType::PeaceAccepted:
		bChoice = false;
		Content = "War Ends\n\n" + Sender + " accepted your peace proposal.";
		break;
	case EPlayerMessageType::AllianceRejected:
		bChoice = false;
		Content = "Rejection\n\n" + Sender + " rejected your alliance request.";
		break;
	case EPlayerMessageType::PeaceRejected:
		bChoice = false;
		Content = "War Continues\n\n" + Sender + " rejected your peace proposal.";
		break;
	case EPlayerMessageType::Eliminated:
		bChoice = false;
		Content = TEXT("Game Over\n\nWith no cities and no units to your name, you have been eliminated from the game.");
		break;
	case EPlayerMessageType::Victory:
		bChoice = false;
		Content = TEXT("Victory!\n\nCongratulations! You are the last faction standing! You won the game!");
		break;
	}
}