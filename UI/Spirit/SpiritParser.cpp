//
// Created by s1935534 on 04/04/2022.
//

#include <iostream>

#include "SpiritParser.h"
#include "Push-Shove/Push.h"
#include "Push-Shove/Shove.h"
#include "Cherries/Cherries.h"
#include "Cherries/StackCherries.h"
#include "CombinatorialGame.h"
#include "TreeNodes.h"

namespace x3 = boost::spirit::x3;

namespace parser {
	CGDatabase& localCGDatabase = CGDatabase::getInstance();

	using x3::char_;
	using x3::no_skip;
	using x3::lit;
	using x3::blank;
	using x3::_attr;
	using x3::_val;
	using boost::fusion::at_c;

	CombinatorialGame& idToGame(AbstractId id) { return localCGDatabase.idToGame(id); }


	auto addLetter = [](auto& ctx) { _val(ctx) += _attr(ctx); };
	auto copy = [](auto& ctx) { _val(ctx) = _attr(ctx); };

	auto createShoveGame = [](auto& ctx) {
		_val(ctx) = &(createShovePosition(_attr(ctx)));
	};
	auto createPushGame = [](auto& ctx) {
		_val(ctx) = &(createPushPosition(_attr(ctx)));
	};

	auto pushShoveToAbstract = [](auto& ctx) {
		_val(ctx) = getAbstractFormId<PushShovePosition>(*_attr(ctx));
	};


	auto createCherriesGame = [](auto& ctx) {
		_val(ctx) = &(createCherriesPosition(_attr(ctx)));
	};
	auto createStackCherriesGame = [](auto& ctx) {
		_val(ctx) = &(createStackCherriesPosition(_attr(ctx)));
	};

	auto cherriesToAbstract = [](auto& ctx) {
		_val(ctx) = getAbstractFormId<CherriesPosition>(*_attr(ctx));
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
		_val(ctx) = new SpiritParserTreeNodeAdd();
	};
	auto createSubtractNode = [](auto& ctx) {
		_val(ctx) = new SpiritParserTreeNodeSubtract();
	};
	auto createCanonicalFormNode = [](auto& ctx) {
		_val(ctx) = new SpiritParserTreeNodeCanonical();
	};

	auto exploreTree = [](auto& ctx) {
		if (_attr(ctx)) _val(ctx) = _attr(ctx)->explore(_val(ctx));
	};

	auto abstractGetDisplay = [](auto& ctx) {
		_val(ctx) = idToGame(_attr(ctx)).getDisplayString();
	};

	auto compareLess = [](auto& ctx) {
		_val(ctx) = at_c<0>(x3::_attr(ctx)) < at_c<1>(x3::_attr(ctx));
	};
	auto compareMore = [](auto& ctx) {
		_val(ctx) = at_c<0>(x3::_attr(ctx)) > at_c<1>(x3::_attr(ctx));
	};
	auto compareEqual = [](auto& ctx) {
		_val(ctx) = at_c<0>(x3::_attr(ctx)) <=> at_c<1>(x3::_attr(ctx)) == 0;
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
	auto compareIncomparable = [](auto& ctx) {
		_val(ctx) = at_c<0>(x3::_attr(ctx)) <=> at_c<1>(x3::_attr(ctx)) == std::partial_ordering::unordered;
	};
	auto compareLessIncomparable = [](auto& ctx) {
		_val(ctx) =
			at_c<0>(x3::_attr(ctx)) <=> at_c<1>(x3::_attr(ctx)) == std::partial_ordering::unordered
			|| at_c<0>(x3::_attr(ctx)) < at_c<1>(x3::_attr(ctx))
		;
	};
	auto compareGreaterIncomparable = [](auto& ctx) {
		_val(ctx) =
			at_c<0>(x3::_attr(ctx)) <=> at_c<1>(x3::_attr(ctx)) == std::partial_ordering::unordered
			|| at_c<0>(x3::_attr(ctx)) > at_c<1>(x3::_attr(ctx))
		;
	};


	auto boolToString = [](auto& ctx) {
		_val(ctx) = _attr(ctx) ? "true" : "false";
	};



	const x3::rule<class quotedString, std::string> quotedString = "readString";
	const x3::rule<class string, std::string> string = "string";

	const x3::rule<class shoveGame, Shove*> shoveGame = "shoveGame";
	const x3::rule<class pushGame, Push*> pushGame = "pushGame";
	const x3::rule<class pushGame, Cherries*> cherriesGame = "cherriesGame";
	const x3::rule<class pushGame, StackCherries*> stackCherriesGame = "stackCherriesGame";

	const x3::rule<class abstractGame, AbstractId> abstractGame = "abstractGame";
	const x3::rule<class abstractGamePrime, SpiritParserTreeNode*> abstractGamePrime = "abstractGamePrime";
	const x3::rule<class abstractGameTerminal, AbstractId> abstractGameTerminal = "abstractGameTerminal";

	const x3::rule<class boolean, bool> boolean = "bool";

	const x3::rule<class outputString, std::string> outputString = "outputString";


	const auto quotedString_def = x3::lexeme['\"' >> *(~x3::char_("\""))[addLetter] >> '\"'];
	const auto string_def = quotedString[copy] | x3::lexeme[*x3::char_("a-zA-Z0-9_ ")[addLetter]];

	const auto shoveGame_def = lit("Shove") >> '(' >> string[createShoveGame] >> ')';
	const auto pushGame_def = lit("Push") >> '(' >> string[createPushGame] >> ')';
	const auto cherriesGame_def = lit("Cherries") >> '(' >> string[createCherriesGame] >> ')';
	const auto stackCherriesGame_def = lit("StackCherries") >> '(' >> string[createStackCherriesGame] >> ')';

	// Split to remove left recursion, causing out of stack crashes.
	// The `Terminal` should contain things that aren't left-recursive.
	// The `Prime` should contain things that are left-recursive.
	const auto abstractGame_def = abstractGameTerminal[copy] >> abstractGamePrime[exploreTree];

	const auto abstractGamePrime_def =
		(
			(char_('+')[createAddNode] >> abstractGame[addLeftChildNode])
			| (char_('-')[createSubtractNode] >> abstractGame[addLeftChildNode])
			| lit(".CanonicalForm()")[createCanonicalFormNode]
		) >> abstractGamePrime[addRightChildNode]
		| x3::eps[makeNull]
	;

	const auto abstractGameTerminal_def =
		shoveGame[pushShoveToAbstract]
		| pushGame[pushShoveToAbstract]
		| cherriesGame[cherriesToAbstract]
		| stackCherriesGame[cherriesToAbstract]
		| '(' >> abstractGame[copy] >> ')'
	;

	const auto boolean_def =
		(
			(abstractGame >> '<' >> abstractGame)[compareLess]
			| (abstractGame >> '>' >> abstractGame)[compareMore]
			| (abstractGame >> "==" >> abstractGame)[compareEqual]
			| (abstractGame >> "=" >> abstractGame)[compareEqual]
			| (abstractGame >> "!=" >> abstractGame)[compareUnequal]
			| (abstractGame >> "<=" >> abstractGame)[compareLessEqual]
			| (abstractGame >> ">=" >> abstractGame)[compareGreaterEqual]
			| (abstractGame >> "||" >> abstractGame)[compareIncomparable]
			| (abstractGame >> "|>" >> abstractGame)[compareGreaterIncomparable]
			| (abstractGame >> "<|" >> abstractGame)[compareLessIncomparable]
			| (string >> '<' >> string)[compareLess]
			| (string >> '>' >> string)[compareMore]
		)
	;

	const auto outputString_def =
		(
			boolean[boolToString]
			| (abstractGame[abstractGetDisplay] >> lit(".DisplayString()"))
			| abstractGame[abstractGetDisplay]
			| string[copy]
		) >> x3::eoi
	;

	BOOST_SPIRIT_DEFINE(quotedString, string, shoveGame, pushGame, cherriesGame, stackCherriesGame);
	BOOST_SPIRIT_DEFINE(abstractGame);
	BOOST_SPIRIT_DEFINE(abstractGameTerminal);
	BOOST_SPIRIT_DEFINE(abstractGamePrime);
	BOOST_SPIRIT_DEFINE(boolean);
	BOOST_SPIRIT_DEFINE(outputString);
}



void parseStringMain() {
	std::cout << "Enter a command." << std::endl;
	while (true) {
		std::string input;
		std::getline(std::cin, input);
		if (input == "q" || input == "quit") return;

		auto first = input.begin();
		auto last = input.end();

		std::string output;
		bool r = phrase_parse(first, last, parser::outputString, x3::ascii::space, output);

		std::cout << "Parsing successful? " << r << "." << std::endl;
		if (!r) continue;
		r = (first == last);
		std::cout << "Input fully parsed? " << r << "." << std::endl;
		std::cout << "  " << output << '.' << std::endl;
	}
}