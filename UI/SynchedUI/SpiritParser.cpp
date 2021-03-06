//
// Created by Xander Lenstra on 22-05-22.
//

#include <chrono>
#include <boost/spirit/home/x3.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <Hackenbush/HackenbushUtil.h>
#include <Hackenbush/Hackenbush.h>
#include "SpiritParser.h"
#include "SynchronousGame/SynchronizedGameDatabase.h"
#include "Cherries/Cherries.h"
#include "Cherries/StackCherries.h"
#include "Push-Shove/PushShoveUtil.h"
#include "Push-Shove/Shove.h"
#include "Push-Shove/Push.h"

namespace x3 = boost::spirit::x3;

namespace synchedGamesParser {
	SGDatabase& localSGDatabase = SGDatabase::getInstance();
	std::set<std::string> rulesetsForWhichUndecidableErrorWasShown = {};
	bool ignoreNonSeparable = false;
	bool printInputAgain = false;
	bool printOutput = true;
	bool timeOn = false;

	using x3::char_;
	using x3::lit;
	using x3::_attr;
	using x3::_val;
	using boost::fusion::at_c;

	SynchronizedGame& idToGame(SynchedId id) { return localSGDatabase.idToGame(id); }

	auto addLetter = [](auto& ctx) { _val(ctx) += _attr(ctx); };
	auto copy = [](auto& ctx) { _val(ctx) = _attr(ctx); };

	auto createShoveGame = [](auto& ctx) {
		_val(ctx) = &(createShovePosition(_attr(ctx)));
	};
	auto createPushGame = [](auto& ctx) {
		_val(ctx) = &(createPushPosition(_attr(ctx)));
	};
	auto createCherriesGame = [](auto& ctx) {
		_val(ctx) = &(createCherriesPosition(_attr(ctx)));
	};
	auto createStackCherriesGame = [](auto& ctx) {
		_val(ctx) = &(createStackCherriesPosition(_attr(ctx)));
	};
	auto createHackenbushGame = [](auto& ctx) {
		_val(ctx) = &(createHackenbushPosition(at_c<0>(_attr(ctx)), at_c<2>(_attr(ctx))));
	};

	auto pushShoveToAbstract = [](auto& ctx) {
		_val(ctx) = determineSynchedId<PushShovePosition>(*_attr(ctx));
	};
	auto cherriesToAbstract = [](auto& ctx) {
		_val(ctx) = determineSynchedId<CherriesPosition>(*_attr(ctx));
	};
	auto hackenbushToAbstract = [](auto& ctx) {
		_val(ctx) = determineSynchedId<HackenbushPosition>(*_attr(ctx));
	};

	auto abstractGetBirthday = [](auto& ctx) {
		_val(ctx) = std::to_string(idToGame(_attr(ctx)).getBirthday());
	};
	auto abstractGetValue = [](auto& ctx) {
		_val(ctx) = std::to_string(idToGame(_attr(ctx)).getValue());
	};
	auto abstractGetWinners = [](auto& ctx) {
		_val(ctx) = idToGame(_attr(ctx)).getWinners();
	};

	auto winnersToString = [](auto& ctx) {
		_val(ctx) = "";
		for (const auto& winner : _attr(ctx)) {
			_val(ctx) += winningPlayerGetDisplayString(winner) + ", ";
		}
		_val(ctx).pop_back();
		_val(ctx).pop_back();
	};
	auto operatorEquality = [](auto& ctx) {
		_val(ctx) = at_c<0>(_attr(ctx)) == at_c<1>(_attr(ctx));
	};
	auto operatorInequality = [](auto& ctx) {
		_val(ctx) = at_c<0>(_attr(ctx)) != at_c<1>(_attr(ctx));
	};

	auto boolToString = [](auto& ctx) {
		_val(ctx) = _attr(ctx) ? "True" : "False";
	};




	const x3::rule<class quotedString, std::string> quotedString = "readString";
	const x3::rule<class string, std::string> string = "string";

	const x3::rule<class shoveGame, Push*> pushGame = "pushGame";
	const x3::rule<class shoveGame, Shove*> shoveGame = "shoveGame";
	const x3::rule<class cherriesGame, Cherries*> cherriesGame = "cherriesGame";
	const x3::rule<class stackCherriesGame, StackCherries*> stackCherriesGame = "stackCherriesGame";
	const x3::rule<class hackenbushGame, Hackenbush*> hackenbushGame = "hackenbushGame";

	const x3::rule<class synchedGame, SynchedId> synchedGame = "synchedGame";

	const x3::rule<class winnersSet, std::unordered_set<WinningPlayer>> winnersSet = "winnersSet";
	const x3::rule<class boolean, bool> boolean = "boolean";

	const x3::rule<class outputString, std::string> outputString = "outputString";


	const auto quotedString_def = x3::lexeme['\"' >> *(~x3::char_("\""))[addLetter] >> '\"'];
	const auto string_def =
		quotedString[copy]
		| x3::lexeme[*x3::char_("a-zA-Z0-9_ ")[addLetter]]
	;

	const auto shoveGame_def = (lit("Shove") > '(' > string > ')')[createShoveGame];
	const auto pushGame_def = (lit("Push") > '(' > string > ')')[createPushGame];
	const auto cherriesGame_def = (lit("Cherries") > '(' > string > ')')[createCherriesGame];
	const auto stackCherriesGame_def = (lit("StackCherries") > '(' > string > ')')[createStackCherriesGame];
	const auto hackenbushGame_def = lit("Hackenbush") > '(' > (x3::int_ > char_(',') > string)[createHackenbushGame] > ')';

	const auto synchedGame_def =
		(
			shoveGame[pushShoveToAbstract]
			| pushGame[pushShoveToAbstract]
			| cherriesGame[cherriesToAbstract]
			| stackCherriesGame[cherriesToAbstract]
			| hackenbushGame[hackenbushToAbstract]
			| ('(' > synchedGame[copy] > ')')
		)
	;

	const auto winnersSet_def = (synchedGame >> lit(".GetWinners()"))[abstractGetWinners];

	const auto boolean_def =
		(
			(synchedGame >> "==" > synchedGame)[operatorEquality]
			| (synchedGame >> "=" > synchedGame)[operatorEquality]
			| (synchedGame >> "!=" > synchedGame)[operatorInequality]
			| (winnersSet >> "==" > winnersSet)[operatorEquality]
			| (winnersSet >> "=" > winnersSet)[operatorEquality]
			| (winnersSet >> "!=" > winnersSet)[operatorInequality]
			| (string >> "==" > string)[operatorEquality]
			| (string >> "=" > string)[operatorEquality]
			| (string >> "!=" > string)[operatorInequality]
		)
	;

	const auto outputString_def =
		(
			boolean[boolToString]
			| winnersSet[winnersToString]
			| (synchedGame >> lit(".GetBirthday()"))[abstractGetBirthday]
			| synchedGame[abstractGetValue]
			| string[copy]
		) > x3::eoi
	;

	BOOST_SPIRIT_DEFINE(quotedString, string, shoveGame, pushGame, cherriesGame, stackCherriesGame, hackenbushGame);
	BOOST_SPIRIT_DEFINE(synchedGame, winnersSet, boolean);
	BOOST_SPIRIT_DEFINE(outputString);
}


void synchedGameUI() {
	std::cout << "Enter a command. 'help' for help" << std::endl;
	auto startTime = std::chrono::steady_clock::now();
	auto endTime = std::chrono::steady_clock::now();
	while (true) {
		std::string input;
		std::getline(std::cin, input);
		if (input == "q" || input == "quit") return;
		if (input == "time" || input == "t") {
			synchedGamesParser::timeOn = !synchedGamesParser::timeOn;
			std::cout << "Timing on: " << synchedGamesParser::timeOn << std::endl;
			continue;
		} else if (boost::iequals(input, "timeOn")) {
			synchedGamesParser::timeOn = true;
			std::cout << "Timing on: " << synchedGamesParser::timeOn << std::endl;
			continue;
		} else if (boost::iequals(input, "timeOff")) {
			synchedGamesParser::timeOn = false;
			std::cout << "Timing on: " << synchedGamesParser::timeOn << std::endl;
			continue;
		} else if (input == "i" || boost::iequals(input, "ignoreNonSeparable")) {
			synchedGamesParser::ignoreNonSeparable = true;
			std::cout << "Disabled warning from analyzing non-separable rulesets" << std::endl;
			continue;
		}
		else if (input == "help" || input == "h") {
			std::cout << "You can enter the following games:" << std::endl
			          << "Hackenbush(a, b) -- a: the number of nodes, b: the string representation of the adjacency matrix" << std::endl
			          << "Cherries(a) -- a: The strip representation of the position" << std::endl
			          << "StackCherries(a) -- a: The strip representation of the position" << std::endl
			          << "Push(a) -- a: The strip representation of the position" << std::endl
			          << "Shove(a) -- a: The strip representation of the position" << std::endl
			          << std::endl
					  << "By default, the value of this game is calculated and printed."
			          << "Additionally, the functions `.GetBirthday()` and `.GetWinner()` can be called on these synchronized games" << std::endl
			          << std::endl
			          << "The following commands are also supported:" << std::endl
			          << "  (h)elp -- show this message" << std::endl
			          << "  (q)uit -- quit" << std::endl
			          << "  (t)ime -- time how long execution takes and print that time" << std::endl
					  << "  (i)gnoreNonSeparable -- disable the warnings for analyzing non-separable games" << std::endl
					  << "  toggleOutput -- toggles whether output is printed" << std::endl
					  << "  printInput   -- toggles whether input is printed again" << std::endl
			          << std::endl;
			continue;
		} else if (boost::iequals(input, "toggleOutput")) {
			synchedGamesParser::printOutput = !synchedGamesParser::printOutput;
			if (synchedGamesParser::printOutput)
				std::cout << "Enabled printing of output" << std::endl;
			continue;
		} else if (boost::iequals(input, "printInput")) {
			synchedGamesParser::printInputAgain = !synchedGamesParser::printInputAgain;
			continue;
		}

		auto first = input.begin();
		auto last = input.end();

		std::string output;
		bool result;

		if (synchedGamesParser::printInputAgain)
			std::cout << input << std::endl;

		if (synchedGamesParser::timeOn)
			startTime = std::chrono::steady_clock::now();

		try {
			result = phrase_parse(first, last, synchedGamesParser::outputString, x3::ascii::space, output);
		} catch (std::exception& exception) {
			std::string error = exception.what();
			std::cout << "Error while parsing input:" << std::endl;
			if (!error.empty()) {
				std::cout << error << std::endl;
			}
			continue;
		}

		if (synchedGamesParser::timeOn)
			endTime = std::chrono::steady_clock::now();

		if (!result) {
			std::cout << "Program is in an invalid state. Please send your query to `xanderlenstra (at) gmail (dot) com`" << std::endl;
			continue;
		}
		if (synchedGamesParser::printOutput)
			std::cout << "  " << output << '.' << std::endl;

		if (synchedGamesParser::timeOn) {
			auto takenTime = endTime - startTime;
			std::cout << "Caculation time: " << std::chrono::duration_cast<std::chrono::microseconds>(takenTime).count() << " microseconds" << std::endl;
		}
	}
}