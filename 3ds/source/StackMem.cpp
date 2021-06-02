/*
*   This file is part of StackMem
*   Copyright (C) 2021 SuperSaiyajinStackZ
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

/*
	Core related notes:

	Initialize a Game by doing:
		std::unique_ptr<StackMem> Game = std::make_unique<StackMem>(...);
		-- It expects the following parameters as the constructor:

		--- const size_t Pairs -> The amount of pairs (Default: 10).
		--- const bool AIUsed -> If an AI should be used (true) or not (false) (Default: true).
		--- const StackMem::AIMethod Method -> The AI Method (Default: StackMem::AIMethod::Random).


	Check the Current Turn State by doing:
		const StackMem::TurnState State = Game->GetState();

		The States:
			- StackMem::TurnState::DrawFirst -> This State means, that the first card is going to be drawn.
			- StackMem::TurnState::DrawSecond -> This State means, that the second card is going to be drawn.
			- StackMem::TurnState::DoCheck -> If both cards are drawn, it switches to this State so you can check for matches.


	Check the Current Player by doing:
		const StackMem::Players Player = Game->GetCurrentPlayer();

		The Players:
			- StackMem::Players::Player1 -> The first Player.
			- StackMem::Players::Player2 -> The second Player.


	Check the Game's State by doing:
		const StackMem::GameState State = Game->CheckState();

		The States:
			- StackMem::GameState::NotOver -> The Game is still going on.
			- StackMem::GameState::Tie -> The Game is over and both Player have the same amount of Pairs.
			- StackMem::GameState::Player1 -> The Game is over and Player 1 has more Pairs than Player 2.
			- StackMem::GameState::Player2 -> The Game is over and Player 2 has more Pairs than Player 1.


	AIMethods Explained:
		- StackMem::AIMethod::Random ->
			This Difficulty uses an std::mt19937 to randomly play a card.
			-- This AI is fairly easy to beat and the fastest AI method from all because of literally no checks.

		- StackMem::AIMethod::Medium ->
			This Difficulty is a "bit" more complex.
			-- It contains a std::vector<int> to store the last played cards.

			--- After each turn, if it can't find a playable card, it does a random turn.

			This Mode only takes effect on the DrawSecond State, else it uses the Random method.
			-- This Method checks each card from the AI's mind with the first played card.
				If the Indexes from it match with the first played card, it plays that.
				Else it will use the Random method.

			--- It potential can take a bit longer if the AI has a lot of last played cards in their mind.

		- StackMem::AIMethod::Hard ->
			Same as Medium, however instead of picking completely randomly when no match is found, it only does a random play for
				cards that haven't been played at all before.

		- StackMem::AIMethod::Extreme ->
			This is an improved version of the Hard method.
			-- It already does the checks on the DrawFirst State and uses the Hard method on the DrawSecond State,
			   because we don't need to do a lot of useless "work" at that point then.

			--- This may take a bit longer than the Hard Method, but not as long as the previous Extreme Method with the pairs.
				the checks are explained below:

			---- It basically checks all the indexes from the AI's mind with each other. If the CardType matches,
				and is not already collected, it plays it.

			If none of them go through, it will use the Random method.


	Use this to check if an AI is enabled:
		const bool AIEnabled = Game->AIEnabled();

	Use this to do an AI Turn:
		const int Card = Game->AIPlay();
		NOTE: You still need to call Game->DoPlay(Card); to finish the AI Turn.

	Use this to play a card:
		const bool Result = Game->DoPlay(Index);
		-- This function returns true if it was successfully able to play, false if not.

	Use this to set the next Player and with that.. the StackMem::TurnState::DrawFirst State:
		Game->NextPlayer();

	Use this to get the amount of Pairs of the Current Game:
		const size_t Pairs = Game->GetPairs();

	Use this to get the amount of collected Pairs from the Players:
		const size_t Player1Pairs = Game->GetPlayerPairs(StackMem::Players::Player1); // Player 1.
		const size_t Player2Pairs = Game->GetPlayerPairs(StackMem::Players::Player2); // Player 2.

	Use this to get the CardType from an Index of the Gamefield:
		const int CType = Game->GetCardType(Index);

	Use this to get the Collected State from an Index:
		const bool Collected = Game->IsCardCollected(Index);

	Use this to get the Shown State from an Index:
		const bool Shown = Game->IsCardShown(Index);

	After you are done with a turn (StackMem::TurnState::DoCheck) call this function:
		const bool Result = Game->DoCheck();
		-- This function returns true, if the played cards matched, or false if it did not.

		--- If it matches, just call:
			Game->SetState(StackMem::TurnState::DrawFirst);
			This will set the State so you can play again.

		---- If it didn't match, just call:
			Game->NextPlayer();
			This will set the proper continue State + switches to the next Player.


	Something nicely to have as well:
		const int ProperPair = Game->GetProperPair();
		-- This function returns the proper Index from the first played card.
		   ( Assuming the Current State is StackMem::TurnState::DrawSecond ).
*/


#include "StackMem.hpp" // main include.
#include <algorithm> // std::shuffle.
#include <ctime> // time().


/*
	StackMem's AI / Computer class implementation.
	Written by SuperSaiyajinStackZ.
*/
/*
	StackMem's AI Constructor.

	const StackMem::AIMethod Method: The AI's Method.
*/
StackMem::AI::AI(const StackMem::AIMethod Method) { this->Method = Method; };


/*
	In case the Game is over,
	we can reset the AI's mind.
*/
void StackMem::AI::ClearMind() {
	switch(this->Method) {
		case StackMem::AIMethod::Random:
			break; // The Random AI has no mind.

		case StackMem::AIMethod::Medium:
		case StackMem::AIMethod::Hard:
		case StackMem::AIMethod::Extreme:
			this->Mind.clear(); // The Medium, Hard and Extreme Method use the vector mind and hence needs to be cleared.
			break;
	};
};


/*
	Here we gonna set a card we want to add to the AI's mind.

	const int Idx1: The first index to add if not included.
	const int Idx2: The second index to add if not included.
*/
void StackMem::AI::UpdateMind(const int Idx1, const int Idx2) {
	switch(this->Method) {
		case StackMem::AIMethod::Random:
			break; // The Random AI has no mind.

		case StackMem::AIMethod::Medium:
		case StackMem::AIMethod::Hard:
		case StackMem::AIMethod::Extreme:
			{
				bool Included[2] = { false }; // Included state.
				const int Idxs[2] = { Idx1, Idx2 }; // The index to check.

				/* Check if that card is already included, so we don't have multiple of the same card. */
				for (size_t Idx = 0; Idx < 2; Idx++) {
					for (size_t Idx3 = 0; Idx3 < this->GetSize(); Idx3++) {
						if (this->Mind[Idx3] == Idxs[Idx]) {
							Included[Idx] = true;
							break;
						};
					};
				};

				for (size_t Idx = 0; Idx < 2; Idx++) {
					if (!Included[Idx]) this->Mind.push_back(Idxs[Idx]); // If not included -> Push it.
				};
			};
			break;
	};
};


/*
	Get an index from the AI's mind.

	const size_t Idx: The index of the mind's first card which to return it's index from.

	NOTE: This returns -1, if you'd check out of bounds, so keep that in mind!
*/
int StackMem::AI::GetMind(const size_t Idx) const {
	if (this->GetMethod() != StackMem::AIMethod::Random) { // Ensure the AI is not the Random one.
		/* Ensure Idx is larger THAN 0, and within the mind size. */
		if ((Idx >= 0) && (Idx < this->GetSize())) return this->Mind[Idx]; // Return the index.
	};

	return -1;
};


/*
	Erase an index from the AI's mind.

	const int Idx1: The first index to erase, if included.
	const int Idx2: The second index to erase, if included.
*/
void StackMem::AI::EraseMind(const int Idx1, const int Idx2) {
	if (this->GetMethod() != StackMem::AIMethod::Random) { // Ensure the AI is not the Random one.
		if (this->Mind.empty()) return; // Already empty, so don't.

		for (size_t Idx = this->GetSize() - 1; Idx > 0; Idx--) { // From last index to first, because erase.
			if (this->Mind[Idx] == Idx1 || this->Mind[Idx] == Idx2) {
				this->Mind.erase(this->Mind.begin() + Idx); // Included, so erase.
			};
		};
	};
};





/*
	StackMem's main class implementation.
	Written by SuperSaiyajinStackZ.
*/
/*
	Constructor.

	const size_t Pairs: The pair amount.
	const bool AIUsed: If an AI should be used or not.
	const StackMem::AIMethod Method: The Method of the AI.
*/
StackMem::StackMem(const size_t Pairs, const bool AIUsed, const StackMem::AIMethod Method) {
	this->InitializeGame(Pairs, AIUsed, Method, true); // Initialization.
};


/*
	Generate the Gamefield.

	const size_t Pairs: The pair amount.
*/
void StackMem::GenerateField(const size_t Pairs) {
	this->Gamefield.clear();
	std::vector<int> TempCards;

	/* First card for the pair. */
	for (int Idx = 0; Idx < (int)Pairs; Idx++) TempCards.push_back(Idx);

	/* Second card for the pair. */
	for (int Idx = 0; Idx < (int)Pairs; Idx++) TempCards.push_back(Idx);

	/* Shuffle the Cards, so they're 'random'. */
	std::shuffle(TempCards.begin(), TempCards.end(), this->RandomEngine);

	for (size_t Idx = 0; Idx < TempCards.size(); Idx++) this->Gamefield.push_back({ TempCards[Idx], false, false }); // Index, Shown, Collected.

	this->Pairs = Pairs; // Also properly update the Pair amount.
};


/*
	Initialize a Game.

	const size_t Pairs: The amount of pairs.
	const bool AIUsed: If an AI should be used or not.
	const StackMem::AIMethod Method: The Method of the AI.
	const bool DoSeed: If the Random Engine should be re-seeded.
*/
void StackMem::InitializeGame(const size_t Pairs, const bool AIUsed, const StackMem::AIMethod Method, const bool DoSeed) {
	this->AIUsed = AIUsed; // Set AI used state.

	/* AI Handle. */
	if (this->AIEnabled()) {
		if (this->_AI) { // If AI is already initialized, just change it + clear mind.
			this->_AI->ClearMind();
			this->_AI->SetMethod(Method);

		} else {
			this->_AI = std::make_unique<StackMem::AI>(Method); // Else initialize the AI.
		};

	} else { // In case the AI got disabled, set it to a nullptr.
		if (this->_AI) this->_AI = nullptr;
	};

	/* Re-Seed the Random Engine. */
	if (DoSeed) this->RandomEngine.seed(time(nullptr));

	/* Init the Game. */
	this->GenerateField(Pairs);
	this->PlayerPairs[0] = 0, this->PlayerPairs[1] = 0; // Set Player Pairs to 0.
	this->PlayCards[0] = -1, this->PlayCards[1] = -1; // Set both turn play cards to -1.
	this->CurrentPlayer = StackMem::Players::Player1; // Set to Player 1.
	this->State = StackMem::TurnState::DrawFirst; // The current State is drawing the first card.
};


/* Get and Set Card Collected State. */
bool StackMem::IsCardCollected(const size_t Idx) const {
	if (Idx < (this->GetPairs() * 2)) return this->Gamefield[Idx].Collected;

	return true;
};
void StackMem::SetCardCollected(const size_t Idx, const bool Collected) {
	if (Idx < (this->GetPairs() * 2)) this->Gamefield[Idx].Collected = Collected;
};


/* Get and Set Card Shown State. */
bool StackMem::IsCardShown(const size_t Idx) const {
	if (Idx < (this->GetPairs() * 2)) return this->Gamefield[Idx].Shown;

	return false;
};
void StackMem::SetCardShown(const size_t Idx, const bool Shown) {
	if (Idx < (this->GetPairs() * 2)) this->Gamefield[Idx].Shown = Shown;
};


/* Return if both played cards matches. */
bool StackMem::CheckMatch() const {
	/* Ensure Cardindxes are in proper range + aren't -1. */
	if ((this->PlayCards[0] != -1) // Check if first card is NOT -1.
	&& (this->PlayCards[1] != -1) // Check if second card is NOT -1.
	&& (this->PlayCards[0] < ((int)this->GetPairs() * 2)) // Check if First Play Card is in range.
	&& (this->PlayCards[1] < ((int)this->GetPairs() * 2)) // Check if Second Play Card is in range.
	&& (this->GetState() == StackMem::TurnState::DoCheck)) { // Check if TurnState is the Check state.
		if (this->Gamefield[this->PlayCards[0]].CardType == this->Gamefield[this->PlayCards[1]].CardType) return true; // Match!
	};

	return false; // No match or not all requirements match.
};


/*
	Does it's checks after both cards are played.
	This handles Pairs checks, if it was successfull, adds the pair to the player, updates the AI mind etc.

	const bool HideCards: If the cards should be hidden (true), or if you handle it on your own (false).

	Returns true if card matched, false if not.
*/
bool StackMem::DoCheck(const bool HideCards) {
	if (this->PlayCards[0] != -1 && this->PlayCards[1] != -1 && this->GetState() == StackMem::TurnState::DoCheck) { // Ensure they are not -1.
		if (this->CheckMatch()) { // Check if both current Cards match.
			/* Card matches, so set a pair. */
			switch(this->CurrentPlayer) {
				case StackMem::Players::Player1:
					this->PlayerPairs[0]++;
					break;

				case StackMem::Players::Player2:
					this->PlayerPairs[1]++;
					break;
			};

			/* Clean up the played cards from the AI's mind. */
			if (this->AIEnabled() && this->_AI && this->_AI->GetMethod() != StackMem::AIMethod::Random) {
				this->_AI->EraseMind(this->GetCardType(this->PlayCards[0]), this->GetCardType(this->PlayCards[1]));
			};

			/* Set that we used and collected it. */
			if (HideCards) {
				this->SetCardCollected(this->PlayCards[0], true); this->SetCardCollected(this->PlayCards[1], true);
			};

			return true;

		} else { // It did not match, so hide cards again + update the AI mind.
			if (this->AIEnabled() && this->_AI && this->_AI->GetMethod() != StackMem::AIMethod::Random) {
				this->_AI->UpdateMind(this->PlayCards[0], this->PlayCards[1]);
			}

			/* Optionally hide those automatically. This is set to false though on 3DZwei for animation purposes. */
			if (HideCards) {
				this->ResetTurn(false); // Hide the cards again.
			};
		};
	};

	/* Heh nope, index == -1 OR not matched. */
	return false;
};


/*
	Play if you can.

	const size_t Idx: The card-index which should be played.

	Returns true, if successfully played, or false if not.
*/
bool StackMem::DoPlay(const size_t Idx) {
	if (!this->IsCardShown(Idx)) { // Ensure the card is NOT shown.
		if (this->GetState() == StackMem::TurnState::DrawFirst) { // If we are on the first card state -> Set to first.
			this->SetCardShown(Idx, true);
			this->PlayCards[0] = Idx;
			this->State = StackMem::TurnState::DrawSecond;
			return true;

		} else if (this->GetState() == StackMem::TurnState::DrawSecond) { // If we are on the second card state -> Set to second.
			this->SetCardShown(Idx, true);
			this->PlayCards[1] = Idx;
			this->State = StackMem::TurnState::DoCheck;
			return true;
		};
	};

	return false; // Card already shown.
};


/*
	Get the Cardtype from a card.

	const size_t Idx: The card-index.
*/
int StackMem::GetCardType(const size_t Idx) const {
	if (Idx > (this->GetPairs() * 2) - 1) return -1; // Out of scope.

	return this->Gamefield[Idx].CardType;
};


/* Set the next Player. */
void StackMem::NextPlayer() {
	switch(this->GetCurrentPlayer()) {
		case StackMem::Players::Player1:
			this->SetCurrentPlayer(StackMem::Players::Player2);
			break;

		case StackMem::Players::Player2:
			this->SetCurrentPlayer(StackMem::Players::Player1);
			break;
	};

	this->SetState(StackMem::TurnState::DrawFirst);
};


/*
	Check the Game's State, and return a Winner, if the Game is over.

	Check for this after each turn. This returns StackMem::GameState::NotOver if the game is still going.
*/
StackMem::GameState StackMem::CheckGameState() const {
	/* This check is only valid, if the Player 1 & 2 Pairs matches the full size. */
	if (this->PlayerPairs[0] + this->PlayerPairs[1] == this->GetPairs()) {
		if (this->PlayerPairs[0] > this->PlayerPairs[1]) return StackMem::GameState::Player1; // Player 1 wins.
		else if (this->PlayerPairs[1] > this->PlayerPairs[0]) return StackMem::GameState::Player2; // Player 2 wins.
		else if (this->PlayerPairs[0] == this->PlayerPairs[1]) return StackMem::GameState::Tie; // No one wins.
	};

	return StackMem::GameState::NotOver; // Nah, Game is still going.
};


/*
	Get the amount of Pairs from a Player.

	const Core::Players P: The Player from which to return the collected Pairs from.
*/
size_t StackMem::GetPlayerPairs(const StackMem::Players P) const {
	return (P == StackMem::Players::Player1 ? this->PlayerPairs[0] : this->PlayerPairs[1]);
};


/*
	AI Method: Random.

	This function just gets all available Indexes, and then selects one randomly through the std::mt19937.

	NOTE: This function returns -1, if no playable cards exist, so keep that in mind!
*/
int StackMem::AIRandomMethod() {
	std::vector<size_t> AvlIndexes;

	/* Get all available indexes. */
	for (size_t Idx = 0; Idx < this->GetPairs() * 2; Idx++) {
		/* Return available Indexes. */
		if (!this->IsCardShown(Idx)) AvlIndexes.push_back(Idx); // Push back indexes.
	};

	/* Return a random index from the available indexes here. */
	if (!AvlIndexes.empty()) return AvlIndexes[this->RandomEngine() % (AvlIndexes.size() - 1) + 0];
	return -1;
};


/*
	AI Method: Medium.

	This function checks deeper into the last played cards (mind) and does it's prediction for the second card.
	If no matches found -> Completely randomly do a play.
*/
int StackMem::AIMediumMethod() {
	/* Make sure we use the AI and our AI is not an nullptr. */
	if (this->AIEnabled() && this->_AI) {
		/* Check for the first AI mind cards. */
		for (size_t Idx = 0; Idx < this->_AI->GetSize(); Idx++) {
			if (this->GetCardType(this->_AI->GetMind(Idx)) == this->GetCardType(this->PlayCards[0])) {
				if (!this->IsCardShown(this->_AI->GetMind(Idx))) return this->_AI->GetMind(Idx);
			}
		};
	};

	return this->AIRandomMethod(); // Do Random Method, cause either AI is not used, or no card matches for a proper play.
};


/*
	AI Method: Hard.

	This function checks deeper into the last played cards (mind) and does it's prediction for the second card.
	If no matches found -> Only do a random play for not already known cards.
*/
int StackMem::AIHardMethod() {
	/* Make sure we use the AI and our AI is not an nullptr. */
	if (this->AIEnabled() && this->_AI) {
		/* Check for the first AI mind cards. */
		for (size_t Idx = 0; Idx < this->_AI->GetSize(); Idx++) {
			if (this->GetCardType(this->_AI->GetMind(Idx)) == this->GetCardType(this->PlayCards[0])) {
				if (!this->IsCardShown(this->_AI->GetMind(Idx))) return this->_AI->GetMind(Idx);
			};
		};

		/* A different variant of random: Only play not known and available cards. */
		std::vector<size_t> AvlIndexes;
		bool IsKnown = false;

		for (int Idx = 0; Idx < (int)this->GetPairs() * 2; Idx++) {
			if (!this->IsCardShown(Idx)) { // Ensure it is NOT shown and hence, available.
				IsKnown = false; // Reset each card.

				for (size_t Idx2 = 0; Idx2 < this->_AI->GetSize(); Idx2++) {
					if (this->_AI->GetMind(Idx2) == Idx) { // That card is known!
						IsKnown = true;
						break;
					};
				};

				if (!IsKnown) AvlIndexes.push_back(Idx); // Push back indexes.
			};
		};

		if (!AvlIndexes.empty()) return AvlIndexes[this->RandomEngine() % (AvlIndexes.size() - 1) + 0];
	};

	return this->AIRandomMethod(); // Do Random Method, cause either AI is not used, or no card matches for a proper play.
};


/*
	AI Method: Extreme.

	This function is the hardest Method of StackMem. This is also known to take a bit longer, depending on
	how many turns are already over and how many cards are in the AI's mind vector.
	It checks for EVERY card of the AI's mind directly on the DrawFirst State.

	For the DrawSecond State, it switches over to the Hard Method, because that is 100% enough for it.
*/
int StackMem::AIExtremeMethod() {
	/* Make sure we use the AI and our AI is valid. */
	if (this->AIEnabled() && this->_AI) {
		if (this->GetState() == StackMem::TurnState::DrawFirst) { // Ensure the Current State is the DrawFirst one.

			/* Check for card indexes. */
			for (size_t Idx = 0; Idx < this->_AI->GetSize(); Idx++) {

				/* Ensure it is not shown already. */
				if (!this->IsCardShown(this->_AI->GetMind(Idx))) {

					/* Check further through the indexes. */
					for (size_t Idx2 = 0; Idx2 < this->_AI->GetSize(); Idx2++) {

						if (this->_AI->GetMind(Idx) != this->_AI->GetMind(Idx2)) { // Ensure it is not the same card.

							/* Check if CardType matches. */
							if (this->GetCardType(this->_AI->GetMind(Idx)) == this->GetCardType(this->_AI->GetMind(Idx2))) {
								return this->_AI->GetMind(Idx);
							};
						};
					};
				};
			};

			/* A different variant of random: Only play not known and available cards. */
			std::vector<size_t> AvlIndexes;
			bool IsKnown = false;

			for (int Idx = 0; Idx < (int)this->GetPairs() * 2; Idx++) {
				if (!this->IsCardShown(Idx)) { // Ensure it is NOT shown and hence, available.
					IsKnown = false; // Reset each card.

					for (size_t Idx2 = 0; Idx2 < this->_AI->GetSize(); Idx2++) {
						if (this->_AI->GetMind(Idx2) == Idx) { // That card is known!
							IsKnown = true;
							break;
						};
					};

					if (!IsKnown) AvlIndexes.push_back(Idx); // Push back indexes.
				};
			};

			if (!AvlIndexes.empty()) return AvlIndexes[this->RandomEngine() % (AvlIndexes.size() - 1) + 0];

			/* That should solve it for us. You ONLY need the Extreme Method on the DrawFirst State. */
		} else if (this->GetState() == StackMem::TurnState::DrawSecond) return this->AIHardMethod();
	};

	return this->AIRandomMethod(); // No AI, or no proper matches found, do Random Method.
};


/*
	This is the function that you should call instead of manually calling the methods which you can't anyways.

	Returns an index selected by the AI.
*/
int StackMem::AIPlay() {
	if (this->AIEnabled() && this->_AI) { // Ensure AI is enabled and valid.
		switch(this->_AI->GetMethod()) {
			case StackMem::AIMethod::Random: return this->AIRandomMethod(); // Totally Randomly.
			case StackMem::AIMethod::Medium: return this->AIMediumMethod(); // Predict on second card state and totally randomly if no matches.
			case StackMem::AIMethod::Hard: return this->AIHardMethod(); // Predict on second card state and not totally randomly if no matches.
			case StackMem::AIMethod::Extreme: return this->AIExtremeMethod(); // Predict on first card state.
		};
	};

	return this->AIRandomMethod(); // AI disabled, or invalid -> Random Method.
};


/*
	Get the proper second card for the pair, because why not.

	Returns the index of the proper card from the first card.
*/
int StackMem::GetProperPair() const {
	if (this->GetState() != StackMem::TurnState::DrawFirst) {
		for (size_t Idx = 0; Idx < this->GetPairs() * 2; Idx++) {
			if ((this->GetCardType(Idx) == this->GetCardType(this->PlayCards[0]))) return Idx;
		};
	};

	return -1; // It would normally only return -1, if you are on the DrawFirst State.
};

/* Sets a random Player as the Current Player using the Random Engine. */
void StackMem::SelectRandomPlayer() {
	const uint8_t Res = this->RandomEngine() % 2 + 1; // Returns 1, or 2.

	switch(Res) {
		case 1:
			this->SetCurrentPlayer(StackMem::Players::Player1);
			break;

		case 2:
			this->SetCurrentPlayer(StackMem::Players::Player2);
			break;
	};
};

/*
	Get the AI's Method.

	Returns Random, if the AI is disabled / invalid.
*/
StackMem::AIMethod StackMem::GetMethod() const {
	if (this->AIEnabled() && this->_AI) return this->_AI->GetMethod();

	return StackMem::AIMethod::Random;
};

/*
	Resets a turn by resetting the Playcards and set to hidden

	const bool Correct: If the card was correct (true, doesn't set the status to hidden) or not (false, hide the card).
*/
void StackMem::ResetTurn(const bool Correct) {
	if (!Correct) {
		this->SetCardShown(this->PlayCards[0], false); this->SetCardShown(this->PlayCards[1], false);
	};

	this->PlayCards[0] = -1, this->PlayCards[1] = -1;
};