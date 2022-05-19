////
//// Created by s1935534 on 19/05/2022.
////
//
//#ifndef CGSYNCH_ABSTRACTSYNCHEDGAME_H
//#define CGSYNCH_ABSTRACTSYNCHEDGAME_H
//
//
//
//template<isPosition Position>
//class AbstractSynchedGame {
//public:
//
//	/** Get a string of the position of this game */
//	virtual std::string getDisplayString() = 0;
//
//	/** Gets any transposition; used for checking if it is in the transposition table */
//	virtual Position getAnyTransposition() const = 0;
//	/** Get a set of all transpositions of this position; added to the transposition table */
//	virtual std::unordered_set<Position> getTranspositions() const = 0;
////	virtual GameId getIdOrInsertIntoDatabase() = 0;
//	/** Returns whether the left and right options of this position have been calculated yet */
//	bool hasBeenExplored() { return explored; }
//	/** Determine the left and right options of this position. Does not recursively explore its options */
//	virtual void explore() = 0;
//
//	/** Returns the id of the abstract form of this game, or -1 if it has not been calculated yet */
//	AbstractId getAbstractForm() { return abstractForm; }
//	/** Set the abstract form of this game */
//	virtual void setAbstractForm(AbstractId id) { abstractForm = id; }
//	/** Can be overriden to give an alternative way to calculate the abstract ID of a position.
//	 *  If it returns false, the normal recursive exploration approach will be used.
//	 *  If it returns true, that will be skipped. This function must then set <code>abstractForm</code> themselves.
//	 *  Is automatically called in <code>getAbstractFormId()</code>.
//	 */
//	virtual bool tryToDetermineAbstractForm() { return false; }
//
//	/** Sets the ID of this game in the corresponding database */
//	void setId(GameId id) { rulesetId = id; }
//
//	GameId getId() const { return rulesetId; }
//private:
//	AbstractGame() = default;
//
//	bool explored = false;
//	std::vector<std::vector<GameId>> optionsMatrix;
//	GameId rulesetId = -1ul;
//	AbstractId abstractForm = -1;
//	std::string displayString;
//};
//
//#endif //CGSYNCH_ABSTRACTSYNCHEDGAME_H
