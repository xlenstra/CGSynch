//
// Created by s1935534 on 04/04/2022.
//

#include <iostream>

#include "SpiritParser.h"

namespace x3 = boost::spirit::x3;

namespace parser {
	using x3::char_;
	using x3::lexeme;
	using x3::lit;
	using x3::_attr;
	using x3::_val;

	auto addLetter = [](auto& ctx) { _val(ctx) += _attr(ctx); };
	const x3::rule<class quotedString, std::string> quotedString = "readString";
	const auto quotedString_def = lexeme['\"' >> *(char_[addLetter] - '\"') >> '\"'];
	BOOST_SPIRIT_DEFINE(quotedString);

	auto createShoveGame = [](auto& ctx) {
		_val(ctx) = getAbstractFormId<PushShovePosition, Shove>(createShovePosition(_attr(ctx)));
	};
	const x3::rule<class shoveGame, AbstractId> shoveGame = "shoveGame";
	const auto shoveGame_def = lit("Shove") >> '(' >> quotedString[createShoveGame] >> ')';
	BOOST_SPIRIT_DEFINE(shoveGame);

}

void parseStringMain() {
	std::string input;
	std::cin >> input;
	auto first = input.begin();
	auto last = input.end();

	AbstractId output;
	bool r = parse(first, last, parser::shoveGame, output);
	std::cout << "Parsing successful? " << r << std::endl;
	if (r) {
		std::cout << ". Result: " << cgDatabase.idToGame(output).getDisplayString() << std::endl;
	}
}