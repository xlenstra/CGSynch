//
// Created by ardour on 22-05-22.
//

#include <chrono>
#include <boost/spirit/home/x3.hpp>
#include <boost/algorithm/string/predicate.hpp>
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
	bool ignoreNonSeparable = false;
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
		if (!ignoreNonSeparable)
			std::cout << "Note that in general, the value of a cherries position doesn't make sense." << std::endl
					  << "This follows from the fact that it can contain decided positions with undecided subpositions" << std::endl
					  << "For more information, read the paper \"Combinatorial games and imperfect information variants\"" << std::endl
					  << "By Mark van den Bergh, which discusses synchronization of non-separable games like cherries." << std::endl << std::endl
					  << "Type 'ignoreNonSeparable' to disable this warning (this session)" << std::endl << std::endl << std::endl;
		_val(ctx) = &(createCherriesPosition(_attr(ctx)));
	};
	auto createStackCherriesGame = [](auto& ctx) {
		if (!ignoreNonSeparable)
			std::cout << "Note that in general, the value of a (stack)cherries position doesn't make sense." << std::endl
			          << "This follows from the fact that it can contain decided positions with undecided subpositions" << std::endl
			          << "For more information, read the paper \"Combinatorial games and imperfect information variants\"" << std::endl
			          << "By Mark van den Bergh, which discusses synchronization of non-separable games like cherries." << std::endl << std::endl
			          << "Type 'ignoreNonSeparable' to disable this warning (this session)" << std::endl << std::endl << std::endl;
		_val(ctx) = &(createStackCherriesPosition(_attr(ctx)));
	};

	auto pushShoveToAbstract = [](auto& ctx) {
		_val(ctx) = getSynchedId<PushShovePosition>(*_attr(ctx));
	};
	auto cherriesToAbstract = [](auto& ctx) {
		_val(ctx) = getSynchedId<CherriesPosition>(*_attr(ctx));
	};

	auto abstractGetBirthday = [](auto& ctx) {
		_val(ctx) = std::to_string(idToGame(_attr(ctx)).getBirthday());
	};
	auto abstractGetValue = [](auto& ctx) {
		_val(ctx) = "Value: " + std::to_string(idToGame(_attr(ctx)).getValue());
	};


	const x3::rule<class quotedString, std::string> quotedString = "readString";
	const x3::rule<class string, std::string> string = "string";

	const x3::rule<class shoveGame, Push*> pushGame = "pushGame";
	const x3::rule<class shoveGame, Shove*> shoveGame = "shoveGame";
	const x3::rule<class cherriesGame, Cherries*> cherriesGame = "cherriesGame";
	const x3::rule<class stackCherriesGame, StackCherries*> stackCherriesGame = "stackCherriesGame";

	const x3::rule<class synchedGame, SynchedId> synchedGame = "synchedGame";

	const x3::rule<class outputString, std::string> outputString = "outputString";


	const auto quotedString_def = x3::lexeme['\"' >> *(~x3::char_("\""))[addLetter] >> '\"'];
	const auto string_def =
		quotedString[copy]
		| x3::lexeme[*x3::char_("a-zA-Z0-9_ ")[addLetter]]
	;

	const auto shoveGame_def = lit("Shove") > '(' > string[createShoveGame] > ')';
	const auto pushGame_def = lit("Push") > '(' > string[createPushGame] > ')';
	const auto cherriesGame_def = lit("Cherries") > '(' > string[createCherriesGame] > ')';
	const auto stackCherriesGame_def = lit("StackCherries") > '(' > string[createStackCherriesGame] > ')';

	const auto synchedGame_def =
		(
			shoveGame[pushShoveToAbstract]
			| pushGame[pushShoveToAbstract]
			| cherriesGame[cherriesToAbstract]
			| stackCherriesGame[cherriesToAbstract]
//			| hackenbushGame[hackenbushToAbstract]
//			| ('(' > synchedGame[copy] > ')')
		)
	;

	const auto outputString_def =
		(
//			boolean[boolToString]
//			| winner[winnerToString]
//          |
			(synchedGame[abstractGetBirthday] >> lit(".GetBirthday()"))
//			| (abstractGame[abstractGetDisplay] >> lit(".DisplayString()"))
//			| synchedGame[abstractGetDisplay]
			| synchedGame[abstractGetValue]
			| string[copy]
		) > x3::eoi
	;

	BOOST_SPIRIT_DEFINE(quotedString, string, shoveGame, pushGame, cherriesGame, stackCherriesGame/*, hackenbushGame*/);
	BOOST_SPIRIT_DEFINE(synchedGame);
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
		} else if (boost::iequals(input, "ignoreNonSeparable")) {
			synchedGamesParser::ignoreNonSeparable = true;
			std::cout << "Disabled warning from analyzing non-separable rulesets" << std::endl;
			continue;
		}
//		else if (boost::iequals(input, "ignoreIllDefinedRulesets")) {
//			synchedGamesParser::ignoreIllDefinedRulesets = true;
//			std::cout << "Disabled warning from analyzing rulesets with multiple interpretations" << std::endl;
//			continue;
//		}
		if (input == "help" || input == "h") {
			std::cout << "You can enter the following games:" << std::endl
//			          << "Hackenbush(a, b) -- a: the number of nodes, b: the string representation of the adjacency matrix" << std::endl
			          << "Cherries(a) -- a: The strip representation of the position" << std::endl
			          << "StackCherries(a) -- a: The strip representation of the position" << std::endl
			          << "Push(a) -- a: The strip representation of the position" << std::endl
			          << "Shove(a) -- a: The strip representation of the position" << std::endl
			          << std::endl
//			          << "These can be added together with +, subtracted with -, compared with <, <=, ==, >=, !=, <| or |> or bracketed" << std::endl
			          << "Additionally, the functions `.GetBirthday()` and `getWinner()` can be called" << std::endl
			          << std::endl
			          << "The following commands are also supported:" << std::endl
			          << "  (h)elp -- show this message" << std::endl
			          << "  (q)uit -- quit" << std::endl
			          << "  (t)ime -- time how long execution takes and print that time" << std::endl
					  << "  ignoreNonSeparable -- disable the warnings for analyzing non-separable games" << std::endl
			          << std::endl;
		}

		auto first = input.begin();
		auto last = input.end();

		std::string output;
		bool result;


		if (synchedGamesParser::timeOn)
			startTime = std::chrono::steady_clock::now();

		try {
			result = phrase_parse(first, last, synchedGamesParser::outputString, x3::ascii::space, output);
		} catch (std::exception& exception) {
			std::string error = exception.what();
			std::cout << "Parsing error, please try again" << std::endl;
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
		std::cout << "  " << output << '.' << std::endl;

		if (synchedGamesParser::timeOn) {
			auto takenTime = endTime - startTime;
			std::cout << "Caculation time: " << std::chrono::duration_cast<std::chrono::microseconds>(takenTime).count() << " microseconds" << std::endl;
		}
	}
}