// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    FBullCowCount Count;
    Isograms = GetValidWords(Words);
    InitGame();

    //PrintLine(TEXT("There are %i possible words"), Words.Num());//debug line
    //PrintLine(TEXT("The number of valid words is %i"), GetValidWords(Words).Num());//debug line
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        InitGame();
    }
    else //check player guess
    {
        CheckGuess(Input);
    }  
}

void UBullCowCartridge::InitGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num()-1)];
    Lives = HiddenWord.Len() * 2;
    bGameOver = false;

    //Welcome the player
    PrintLine(TEXT("Welcome to the Bull Cow game!"));
    PrintLine(TEXT("Guess the hidden word"));
    PrintLine(TEXT("It is %i letters long"), HiddenWord.Len());
    PrintLine(TEXT("you have %i lives"),Lives);
    PrintLine(TEXT("Please type your guess and hit enter"));

    //PrintLine(TEXT("The hidden word is: %s."), *HiddenWord); // debug line

}

void UBullCowCartridge::EndLoss()
{
    bGameOver = true;
    PrintLine(TEXT("The game is over!"));
    PrintLine(TEXT("The hidden word was %s"), *HiddenWord);
    PrintLine(TEXT("Press enter to play again"));
}

void UBullCowCartridge::EndWin()
{
    bGameOver = true;
    PrintLine(TEXT("Congratulations! You win!"));
    PrintLine(TEXT("Press enter to play again"));
}

void UBullCowCartridge::CheckGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        ClearScreen();
        EndWin();
        return;
    }

    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("An isogram has no repeating leters"));
        PrintLine(TEXT("make sure your guess is an isogram"));
    }

    if (Lives > 0 && Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("You have the wrong number of letters"));
        PrintLine(TEXT("The word is %i letters long"), HiddenWord.Len());
        --Lives;
        PrintLine(TEXT("%i Lives left!"), Lives);
    }

    if (Lives <= 0)
    {
        ClearScreen();
        EndLoss();
    }
    FBullCowCount Score = GetBullCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 i = 0; i < Word.Len(); i++)
    {
        for (int32 comparison = i + 1; comparison < Word.Len(); comparison++)
        {
            if (Word[i] == Word[comparison])
            {
                return false;
            }
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString TempWord : WordList)
    {
        if (TempWord.Len() >= 4 && TempWord.Len() <= 8 && IsIsogram(TempWord))
        {
            ValidWords.Emplace(TempWord);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    //for every index of guess, if same as Hidden, BullCount ++
    //if not a bull was it a cow? if yes CowCount ++

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}