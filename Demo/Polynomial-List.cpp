#include <iostream>
#include "List.hpp"

struct PolynomialTerm final {
	decltype(0.) Coefficient = 0.;
	decltype(0.) Degree = 0.;
	PolynomialTerm(double Coefficient = 0., double Degree = 0.) {
		this->Coefficient = Coefficient;
		this->Degree = Degree;
	}
	PolynomialTerm(PolynomialTerm &&) = default;
	PolynomialTerm(const PolynomialTerm &) = default;
	auto operator=(PolynomialTerm &&)->PolynomialTerm & = default;
	auto operator=(const PolynomialTerm &)->PolynomialTerm & = default;
	~PolynomialTerm() = default;
	auto operator*(const PolynomialTerm &RightHandSideTerm) const {
		return PolynomialTerm{ Coefficient * RightHandSideTerm.Coefficient, Degree + RightHandSideTerm.Degree };
	}
	auto operator<(const PolynomialTerm &RightHandSideTerm) const {
		if (Degree != RightHandSideTerm.Degree)
			return Degree < RightHandSideTerm.Degree;
		else
			return Coefficient < RightHandSideTerm.Coefficient;
	}
};

auto main()->int {
	using Polynomial = StandardTemplateLibrary::List<PolynomialTerm>;
	auto PolynomialA = Polynomial{ { 1., 0. },{ 1., 1. } };
	auto PolynomialB = Polynomial{ { 1., 0. },{ 2., 1. },{ 6., 2. } };
	auto SimplifyPolynomial = [](auto &Expression) {
		auto CombineLikeTerms = [&]() {
			auto Combine = [&]() {
				auto Cursor = Expression.begin();
				auto GetNext = [](auto Iterator) {
					return ++Iterator;
				};
				while (GetNext(Cursor) != Expression.end())
					if (Cursor->Degree == GetNext(Cursor)->Degree) {
						Cursor->Coefficient += GetNext(Cursor)->Coefficient;
						Expression.Erase(GetNext(Cursor));
					}
					else
						++Cursor;
			};
			Expression.Sort();
			Combine();
		};
		auto Cleanup = [&]() {
			auto Cursor = Expression.begin();
			while (Cursor != Expression.end())
				if (Cursor->Coefficient == 0.)
					Cursor = Expression.Erase(Cursor);
				else
					++Cursor;
		};
		CombineLikeTerms();
		Cleanup();
		Expression.Reverse();
	};
	auto MultiplyPolynomial = [&]() {
		auto Result = Polynomial{};
		for (auto &x : PolynomialA)
			for (auto &y : PolynomialB)
				Result += x * y;
		SimplifyPolynomial(Result);
		return Result;
	};
	auto PrintPolynomial = [](auto &&Expression) {
		auto &LastPolynomialTerm = *--Expression.end();
		auto PrintCoefficient = [](auto &Value) {
			if (Value.Coefficient != 1.)
				std::cout << Value.Coefficient;
			else if (Value.Degree == 0.)
				std::cout << Value.Coefficient;
			else
				return;
		};
		auto PrintIndeterminate = [](auto &Value) {
			if (Value.Degree != 0. && Value.Degree != 1.)
				std::cout << "x^" << Value.Degree;
			else if (Value.Degree == 1.)
				std::cout << "x";
			else
				return;
		};
		if (Expression.Empty())
			std::cout << "0";
		else
			for (auto &x : Expression) {
				PrintCoefficient(x);
				PrintIndeterminate(x);
				if (&x != &LastPolynomialTerm)
					std::cout << " + ";
			}
		std::cout << std::endl;
	};
	auto Pause = []() {
		std::cout << "Press ENTER to continue . . . ";
		std::cin.get();
	};
	std::cout << "PolynomialA: ";
	SimplifyPolynomial(PolynomialA);
	PrintPolynomial(PolynomialA);
	std::cout << "PolynomialB: ";
	SimplifyPolynomial(PolynomialB);
	PrintPolynomial(PolynomialB);
	std::cout << "PolynomialA * PolynomialB: ";
	PrintPolynomial(MultiplyPolynomial());
	Pause();
}
