//
// Created by Xander Lenstra on 11/04/2022.
//
//
//#include <boost/spirit/home/x3.hpp>
//#include <boost/algorithm/string/join.hpp>
//#include <CombinatorialGameDatabase.h>
//#include <set>
//#include "RulesetMathUtil.h"
//
//namespace x3 = boost::spirit::x3;
//
//enum class MathPositionType {
//	Game,
//	Int,
//	Rational,
//};
//
//struct MathPositionTemp {
//	MathPositionType type = MathPositionType::Game;
//	std::set<MathPositionTemp> game;
//	int integer;
//	DyadicRational rational;
//};
//
//
//namespace mathGameParser {
//	using x3::char_;
//	using x3::no_skip;
//	using x3::lit;
//	using x3::blank;
//	using x3::_attr;
//	using x3::_val;
//	using boost::fusion::at_c;
//
//	auto intToGame = [](auto& ctx) {
//		_val(ctx) = MathRuleset(std::to_string(_attr(ctx)));
//		_val(ctx).setAbstractId(CGDatabase::getInstance().getInteger(_attr(ctx)));
//	};
//
//	auto rationalToGame = [](auto& ctx) {
//		int numerator = at_c<0>(_attr(ctx));
//		int denominator = at_c<1>(_attr(ctx));
//		_val(ctx) = MathRuleset(std::to_string(numerator) + '/' + std::to_string(denominator));
//		_val(ctx).setAbstractId(CGDatabase::getInstance().getDyadicRational(numerator, denominator));
//	};
//
//	auto listsToGame = [](auto& ctx) {
//		std::vector<MathRuleset> leftOptions = std::move(at_c<0>(_attr(ctx)));
//		std::vector<MathRuleset> rightOptions = std::move(at_c<1>(_attr(ctx)));
//
//	};
//
//	const x3::rule<class mathRuleset, MathRuleset> mathRuleset = "mathRuleset";
//	const auto mathRuleset_def =
//		x3::int_[intToGame]
//		| (x3::int_ >> '/' >> x3::int_)[rationalToGame]
//		| (char_('{') >> (mathRuleset % ',') >> char_('|') >> (mathRuleset % ',') >> char_('}'))[listsToGame]
//	;
//
//	const x3::rule<class quotedString, std::string> quotedString = "readString";
//
//	BOOST_SPIRIT_DEFINE(mathRuleset);
//}