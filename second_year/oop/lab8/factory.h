#pragma once
#include<iostream>
#include"figure.h"
#include"pentagon.h"
#include"rhombus.h"
#include"trapeze.h"
struct factory {
public:
	virtual std::unique_ptr<figure> build(std::istream& is) = 0;
	virtual ~factory() = default;
};

struct pentagon_factory : factory {
	std::unique_ptr<figure> build(std::istream& is) override{
		std::unique_ptr<pentagon> temp;
		temp = std::make_unique<pentagon>();
		temp->read(is);
		return std::move(temp);
	}
};
struct trapeze_factory : factory {
	std::unique_ptr<figure> build(std::istream& is) override {
		std::unique_ptr<trapeze> temp;

		temp = std::make_unique<trapeze>();
		temp->read(is);
		return std::move(temp);
	}
};
struct rhombus_factory : factory {
	std::unique_ptr<figure> build(std::istream& is) override {
		std::unique_ptr<rhombus> temp;

		temp = std::make_unique<rhombus>();
		temp->read(is);
		return std::move(temp);
	}
};