#pragma once

#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"

inline void RegisterLetterNames()
{
	static bool Done = false;
	if (Done) return;
	
	LOCTABLE_NEW("LetterNames", "LetterNames");
	LOCTABLE_SETSTRING("LetterNames", "a", "Alpha");
	LOCTABLE_SETSTRING("LetterNames", "b", "Beta");
	LOCTABLE_SETSTRING("LetterNames", "c", "Gamma");
	LOCTABLE_SETSTRING("LetterNames", "d", "Delta");
	LOCTABLE_SETSTRING("LetterNames", "e", "Epsilon");
	LOCTABLE_SETSTRING("LetterNames", "f", "Zeta");
	LOCTABLE_SETSTRING("LetterNames", "g", "Eta");
	LOCTABLE_SETSTRING("LetterNames", "h", "Theta");
	LOCTABLE_SETSTRING("LetterNames", "i", "Iota");
	LOCTABLE_SETSTRING("LetterNames", "j", "Kappa");
	LOCTABLE_SETSTRING("LetterNames", "k", "Lambda");
	LOCTABLE_SETSTRING("LetterNames", "l", "Mu");
	LOCTABLE_SETSTRING("LetterNames", "m", "Nu");
	LOCTABLE_SETSTRING("LetterNames", "n", "Xi");
	LOCTABLE_SETSTRING("LetterNames", "o", "Omicron");
	LOCTABLE_SETSTRING("LetterNames", "p", "Pi");
	LOCTABLE_SETSTRING("LetterNames", "q", "Rho");
	LOCTABLE_SETSTRING("LetterNames", "r", "Sigma");
	LOCTABLE_SETSTRING("LetterNames", "s", "Tau");
	LOCTABLE_SETSTRING("LetterNames", "t", "Upsilon");
	LOCTABLE_SETSTRING("LetterNames", "u", "Phi");
	LOCTABLE_SETSTRING("LetterNames", "v", "Chi");
	LOCTABLE_SETSTRING("LetterNames", "w", "Psi");
	LOCTABLE_SETSTRING("LetterNames", "x", "Omega");

	Done = true;
}

inline FString GetLetterName(int32 Index)
{
	static auto Table = FStringTableRegistry::Get().FindStringTable("LetterNames");
	static TArray<FTextKey> Keys;
	
	if (Keys.IsEmpty())
	{
		Table->EnumerateKeysAndSourceStrings([&](const FTextKey& Key, const FString& String)
		{
			Keys.Add(Key);
			return true;
		});
	}
	
	if (auto Entry = Table->FindEntry(Keys[Index]))
	{
		if (auto DisplayString = Entry->GetDisplayString())
		{
			return *DisplayString;
		}
	}
	
	return "<MISSING LETTER NAME>";
}