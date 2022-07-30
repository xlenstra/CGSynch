//
// Created by Xander Lenstra on 04/04/2022.
//

#include <iostream>
#include <chrono>
#include <boost/algorithm/string.hpp>
#include <utility>

#include "SpiritParser.h"
#include "Push-Shove/Push.h"
#include "Push-Shove/Shove.h"
#include "Cherries/Cherries.h"
#include "Cherries/StackCherries.h"
#include "Hackenbush/Hackenbush.h"
#include "CombinatorialGame/CombinatorialGame.h"
#include "TreeNodes.h"

namespace x3 = boost::spirit::x3;

namespace alternatingGamesParser {
	CGDatabase& localCGDatabase = CGDatabase::getInstance();

	bool printInputAgain = false;
	bool printOutput = true;
	bool timeOn = false;

	using x3::char_;
	using x3::lit;
	using x3::_attr;
	using x3::_val;
	using boost::fusion::at_c;

	CombinatorialGame& idToGame(AlternatingId id) { return localCGDatabase.idToGame(id); }

//	struct error_handler
//	{
//		template <typename Iterator, typename Exception, typename Context>
//		x3::error_handler_result on_error(
//			Iterator& first, Iterator const& last
//			, Exception const& x, Context const& context)
//		{
//			auto& error_handler = x3::get<x3::error_handler_tag>(context).get();
//			std::string message = "Error! Expecting: " + x.which() + " here:";
//			error_handler(x.where(), message);
//			return x3::error_handler_result::fail;
//		}
//	};

	auto copy = [](auto& ctx) { _val(ctx) = _attr(ctx); };
	auto addLetter = [](auto& ctx) { _val(ctx) += _attr(ctx); };

	auto createFraction = [](auto& ctx) {
		_val(ctx) = std::make_pair(at_c<0>(_attr(ctx)), at_c<1>(_attr(ctx)));
    };

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
		_val(ctx) = determineAlternatingId<PushShovePosition>(*_attr(ctx));
	};
	auto cherriesToAbstract = [](auto& ctx) {
		_val(ctx) = determineAlternatingId<CherriesPosition>(*_attr(ctx));
	};
	auto hackenbushToAbstract = [](auto& ctx) {
		_val(ctx) = determineAlternatingId<HackenbushPosition>(*_attr(ctx));
	};
	auto integerToAbstract = [](auto& ctx) {
		_val(ctx) = localCGDatabase.getInteger(_attr(ctx)).getId();
	};
	auto fractionToAbstract = [](auto& ctx) {
		_val(ctx) = localCGDatabase.getDyadicRational(_attr(ctx).first, _attr(ctx).second).getId();
	};


	auto unaryMinusGame = [](auto& ctx) {
		_val(ctx) = (-idToGame(_attr(ctx))).getId();
	};

	auto addLeftChildNode = [](auto& ctx) {
		_val(ctx)->setLeftChild(idToGame(_attr(ctx)).getId());
	};
	auto addRightChildNode = [](auto& ctx) {
		_val(ctx)->setRightChild(_attr(ctx));
	};
	auto makeNull = [](auto& ctx) {
		_val(ctx) = nullptr;
	};
	auto createAddNode = [](auto& ctx) {
		_val(ctx) = std::make_shared<SpiritParserTreeNodeAdd>();
	};
	auto createSubtractNode = [](auto& ctx) {
		_val(ctx) = std::make_shared<SpiritParserTreeNodeSubtract>();
	};
	auto createCanonicalFormNode = [](auto& ctx) {
		_val(ctx) = std::make_shared<SpiritParserTreeNodeCanonical>();
	};

	auto exploreTree = [](auto& ctx) {
		if (_attr(ctx)) _val(ctx) = _attr(ctx)->explore(_val(ctx));
	};

	auto abstractFromSets = [](auto& ctx) {
		_val(ctx) = localCGDatabase.getGameId(at_c<0>(x3::_attr(ctx)), at_c<1>(x3::_attr(ctx)));
	};

	auto abstractGetDisplay = [](auto& ctx) {
		_val(ctx) = idToGame(_attr(ctx)).getDisplayString();
	};
	auto abstractIsNumber = [](auto& ctx) {
		_val(ctx) = idToGame(_attr(ctx)).isNumber();
	};
	auto abstractGetWinner = [](auto& ctx) {
		_val(ctx) = idToGame(_attr(ctx)).getWinner();
	};
	auto abstractGetBirthday = [](auto& ctx) {
		_val(ctx) = std::to_string(idToGame(_attr(ctx)).getBirthday());
	};
	auto abstractIsCanonical = [](auto& ctx) {
		_val(ctx) = idToGame(_attr(ctx)).isInCanonicalForm();
	};

	auto winnerToString = [](auto& ctx) {
		_val(ctx) = winningPlayerGetDisplayString(_attr(ctx));
	};

	auto compareGameLess = [](auto& ctx) {
		_val(ctx) = idToGame(at_c<0>(x3::_attr(ctx))) < idToGame(at_c<1>(x3::_attr(ctx)));
	};
	auto compareGameGreater = [](auto& ctx) {
		_val(ctx) = idToGame(at_c<0>(x3::_attr(ctx))) > idToGame(at_c<1>(x3::_attr(ctx)));
	};
	auto compareGameLessEqual = [](auto& ctx) {
		_val(ctx) = idToGame(at_c<0>(x3::_attr(ctx))) <= idToGame(at_c<1>(x3::_attr(ctx)));
	};
	auto compareGameGreaterEqual = [](auto& ctx) {
		_val(ctx) = idToGame(at_c<0>(x3::_attr(ctx))) >= idToGame(at_c<1>(x3::_attr(ctx)));
	};
	auto compareGameEqual = [](auto& ctx) {
		_val(ctx) = idToGame(at_c<0>(x3::_attr(ctx))) <=> idToGame(at_c<1>(x3::_attr(ctx))) == 0;
	};
	auto compareGameUnequal = [](auto& ctx) {
		_val(ctx) = idToGame(at_c<0>(x3::_attr(ctx))) <=> idToGame(at_c<1>(x3::_attr(ctx))) != 0;
	};
	auto compareGameIncomparable = [](auto& ctx) {
		_val(ctx) = idToGame(at_c<0>(x3::_attr(ctx))) <=> idToGame(at_c<1>(x3::_attr(ctx))) == std::partial_ordering::unordered;
	};
	auto compareGameLessIncomparable = [](auto& ctx) {
		_val(ctx) =
			at_c<0>(x3::_attr(ctx)) <=> at_c<1>(x3::_attr(ctx)) == std::partial_ordering::unordered
			|| at_c<0>(x3::_attr(ctx)) < at_c<1>(x3::_attr(ctx))
			;
	};
	auto compareGameGreaterIncomparable = [](auto& ctx) {
		_val(ctx) =
			at_c<0>(x3::_attr(ctx)) <=> at_c<1>(x3::_attr(ctx)) == std::partial_ordering::unordered
			|| at_c<0>(x3::_attr(ctx)) > at_c<1>(x3::_attr(ctx))
			;
	};
	auto compareLess = [](auto& ctx) {
		_val(ctx) = at_c<0>(x3::_attr(ctx)) < at_c<1>(x3::_attr(ctx));
	};
	auto compareGreater = [](auto& ctx) {
		_val(ctx) = at_c<0>(x3::_attr(ctx)) > at_c<1>(x3::_attr(ctx));
	};
	auto compareEqual = [](auto& ctx) {
		_val(ctx) = at_c<0>(x3::_attr(ctx)) == at_c<1>(x3::_attr(ctx));
	};
	auto compareUnequal = [](auto& ctx) {
		_val(ctx) = at_c<0>(x3::_attr(ctx)) != at_c<1>(x3::_attr(ctx));
	};
	auto compareLessEqual = [](auto& ctx) {
		_val(ctx) = at_c<0>(x3::_attr(ctx)) <= at_c<1>(x3::_attr(ctx));
	};
	auto compareGreaterEqual = [](auto& ctx) {
		_val(ctx) = at_c<0>(x3::_attr(ctx)) >= at_c<1>(x3::_attr(ctx));
	};

	auto boolToString = [](auto& ctx) {
		_val(ctx) = _attr(ctx) ? "True" : "False";
	};



	const x3::rule<class quotedString, std::string> quotedString = "readString";
	const x3::rule<class string, std::string> string = "string";
	const x3::rule<class fraction, std::pair<long long, long long>> fraction = "fraction";

	const x3::rule<class shoveGame, Shove*> shoveGame = "shoveGame";
	const x3::rule<class pushGame, Push*> pushGame = "pushGame";
	const x3::rule<class cherriesGame, Cherries*> cherriesGame = "cherriesGame";
	const x3::rule<class stackCherriesGame, StackCherries*> stackCherriesGame = "stackCherriesGame";
	const x3::rule<class hackenbushGame, Hackenbush*> hackenbushGame = "hackenbushGame";

	const x3::rule<class abstractGame, AlternatingId> abstractGame = "abstractGame";
	const x3::rule<class abstractGamePrime, std::shared_ptr<SpiritParserTreeNode>> abstractGamePrime = "abstractGamePrime";
	const x3::rule<class abstractGameTerminal, AlternatingId> abstractGameTerminal = "abstractGameTerminal";

	const x3::rule<class abstractGameSet, std::set<AlternatingId>> abstractGameSet = "abstractGameSet";

	const x3::rule<class boolean, bool> boolean = "boolean";
	const x3::rule<class winner, WinningPlayer> winner = "winner";

	const x3::rule<class outputString, std::string> outputString = "outputString";


	const auto quotedString_def = x3::lexeme['\"' >> *(~x3::char_("\""))[addLetter] >> '\"'];
	const auto string_def =
		quotedString[copy]
		| x3::lexeme[*x3::char_("a-zA-Z0-9_ ")[addLetter]]
	;
	const auto fraction_def = (x3::long_long >> '/' > x3::long_long)[createFraction];

	const auto shoveGame_def = lit("Shove") > '(' > string[createShoveGame] > ')';
	const auto pushGame_def = lit("Push") > '(' > string[createPushGame] > ')';
	const auto cherriesGame_def = lit("Cherries") > '(' > string[createCherriesGame] > ')';
	const auto stackCherriesGame_def = lit("StackCherries") > '(' > string[createStackCherriesGame] > ')';
	const auto hackenbushGame_def = lit("Hackenbush") > '(' > (x3::int_ > char_(',') > string)[createHackenbushGame] > ')';

	// Split to remove left recursion, causing out of stack crashes.
	// The `Terminal` should contain things that aren't left-recursive.
	// The `Prime` should contain things that are left-recursive.
	const auto abstractGame_def = abstractGameTerminal[copy] > abstractGamePrime[exploreTree];

	const auto abstractGamePrime_def =
		(
			(
				(char_('+')[createAddNode] > abstractGame[addLeftChildNode])
				| (char_('-')[createSubtractNode] > abstractGame[addLeftChildNode])
				| lit(".CanonicalForm()")[createCanonicalFormNode]
			) > abstractGamePrime[addRightChildNode]
		)
		| x3::eps[makeNull]
	;

	const auto abstractGameTerminal_def =
		shoveGame[pushShoveToAbstract]
		| pushGame[pushShoveToAbstract]
		| cherriesGame[cherriesToAbstract]
		| stackCherriesGame[cherriesToAbstract]
		| hackenbushGame[hackenbushToAbstract]
		| fraction[fractionToAbstract]
		| x3::long_long[integerToAbstract]
		| ('(' > abstractGame[copy] > ')')
		| ('-' > abstractGame[unaryMinusGame])
		| ('{' > abstractGameSet > '|' >> abstractGameSet > '}')[abstractFromSets]
	;

	const auto abstractGameSet_def =
		(abstractGame % ',')
		| x3::eps
	;

	const auto winner_def =
		abstractGame[abstractGetWinner] >> ".GetWinner()"
	;

	const auto boolean_def =
			(abstractGame >> '<' >> abstractGame)[compareGameLess]
			| (abstractGame >> '>' >> abstractGame)[compareGameGreater]
			| (abstractGame >> "==" > abstractGame)[compareGameEqual]
			| (abstractGame >> "=" > abstractGame)[compareGameEqual]
			| (abstractGame >> "!=" > abstractGame)[compareGameUnequal]
			| (abstractGame >> "<=" > abstractGame)[compareGameLessEqual]
			| (abstractGame >> ">=" > abstractGame)[compareGameGreaterEqual]
			| (abstractGame >> "||" > abstractGame)[compareGameIncomparable]
			| (abstractGame >> "|>" > abstractGame)[compareGameGreaterIncomparable]
			| (abstractGame >> "<|" > abstractGame)[compareGameLessIncomparable]
			| (string >> '<' > string)[compareLess]
			| (string >> '>' > string)[compareGreater]
			| (string >> '=' > string)[compareEqual]
			| (string >> "==" > string)[compareEqual]
			| (string >> "!=" > string)[compareUnequal]
			| (winner >> '=' > winner)[compareEqual]
			| (winner >> "==" > winner)[compareEqual]
			| (winner >> "!=" > winner)[compareUnequal]
			| (winner >> "<" >> winner)[compareLess]
			| (winner >> "<=" > winner)[compareLessEqual]
			| (winner >> ">=" > winner)[compareGreaterEqual]
			| (winner >> ">" >> winner)[compareGreater]
			| (abstractGame >> ".IsNumber()")[abstractIsNumber]
			| (abstractGame >> ".IsInCanonicalForm()")[abstractIsCanonical]
	;

	const auto outputString_def =
		(
			boolean[boolToString]
			| winner[winnerToString]
			| (abstractGame[abstractGetBirthday] >> lit(".GetBirthday()"))
			| (abstractGame[abstractGetDisplay] >> lit(".DisplayString()"))
			| abstractGame[abstractGetDisplay]
			| string[copy]
		) > x3::eoi
	;

	BOOST_SPIRIT_DEFINE(quotedString, string, fraction);
	BOOST_SPIRIT_DEFINE(shoveGame, pushGame, cherriesGame, stackCherriesGame, hackenbushGame);
	BOOST_SPIRIT_DEFINE(abstractGame, abstractGameTerminal, abstractGamePrime, abstractGameSet);
	BOOST_SPIRIT_DEFINE(outputString);
	BOOST_SPIRIT_DEFINE(winner);
	BOOST_SPIRIT_DEFINE(boolean);
}



void alternatingGameUI() {
	std::cout << "Enter a command. 'help' for help" << std::endl;
	auto startTime = std::chrono::steady_clock::now();
	auto endTime = std::chrono::steady_clock::now();
	while (true) {
		std::string input;
		std::getline(std::cin, input);
		if (input == "q" || input == "quit") return;
		if (input == "time" || input == "t") {
			alternatingGamesParser::timeOn = !alternatingGamesParser::timeOn;
			std::cout << "Timing on: " << alternatingGamesParser::timeOn << std::endl;
			continue;
		} else if (boost::iequals(input, "timeOn")) {
			alternatingGamesParser::timeOn = true;
			std::cout << "Timing on: " << alternatingGamesParser::timeOn << std::endl;
			continue;
		} else if (boost::iequals(input, "timeOff")) {
			alternatingGamesParser::timeOn = false;
			std::cout << "Timing on: " << alternatingGamesParser::timeOn << std::endl;
			continue;
		} else if (input == "help" || input == "h") {
			std::cout << "You can enter the following games:" << std::endl
			          << "Hackenbush(a, b) -- a: the number of nodes, b: the string representation of the adjacency matrix" << std::endl
			          << "Cherries(a) -- a: The strip representation of the position" << std::endl
			          << "StackCherries(a) -- a: The strip representation of the position" << std::endl
			          << "Push(a) -- a: The strip representation of the position" << std::endl
			          << "Shove(a) -- a: The strip representation of the position" << std::endl
			          << std::endl
			          << "These can be added together with +, subtracted with -, compared with <, <=, ==, >=, !=, <| or |> or bracketed" << std::endl
			          << "Additionally, the functions `.CanonicalForm()`, `.IsInCanonicalForm()`, `.GetBirthday()`, `isNumber()` and `getWinner()` can be called" << std::endl
			          << std::endl
			          << "The following commands are also supported:" << std::endl
			          << "  (h)elp       -- show this message" << std::endl
			          << "  (q)uit       -- quit" << std::endl
			          << "  (t)ime       -- time how long execution takes and print that time" << std::endl
			          << "  toggleOutput -- toggles whether output is printed" << std::endl
					  << "  printInput   -- toggles whether input is printed again" << std::endl
			          << std::endl;
			continue;
		} else if (boost::iequals(input, "toggleOutput")) {
			alternatingGamesParser::printOutput = !alternatingGamesParser::printOutput;
			if (alternatingGamesParser::printOutput)
				std::cout << "Enabled printing of output" << std::endl;
			continue;
		} else if (boost::iequals(input, "printInput")) {
			alternatingGamesParser::printInputAgain = !alternatingGamesParser::printInputAgain;
			continue;
		}

		if (alternatingGamesParser::printInputAgain)
			std::cout << input << std::endl;

		auto first = input.begin();
		auto last = input.end();

		std::string output;
		bool result;
		std::string error;

		if (alternatingGamesParser::timeOn)
			startTime = std::chrono::steady_clock::now();

		try {
			result = phrase_parse(first, last, alternatingGamesParser::outputString, x3::ascii::space, output);
		} catch (std::exception& exception) {
			result = false;
			error = exception.what();
		}

		if (alternatingGamesParser::timeOn)
			endTime = std::chrono::steady_clock::now();



		if (!result) {
			std::cout << "Error while parsing input:" << std::endl;
			if (!error.empty()) {
				std::cout << error << std::endl;
			}
			continue;
		}

		if (alternatingGamesParser::printOutput)
			std::cout << "  " << output << '.' << std::endl;

		if (alternatingGamesParser::timeOn) {
			auto takenTime = endTime - startTime;
			std::cout << "Caculation time: " << std::chrono::duration_cast<std::chrono::microseconds>(takenTime).count() << " microseconds" << std::endl;
		}
	}
}